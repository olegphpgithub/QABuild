#include <windows.h>
#include <iostream>
#include <string>

#include "AES.h"


AES::AES(int keyLen = 256)
{
  this->Nb = 4;
  switch (keyLen)
  {
  case 128:
    this->Nk = 4;
    this->Nr = 10;
    break;
  case 192:
    this->Nk = 6;
    this->Nr = 12;
    break;
  case 256:
    this->Nk = 8;
    this->Nr = 14;
    break;
  default:
    throw "Incorrect key length";
  }

  blockBytesLen = 4 * this->Nb * sizeof(unsigned char);
}

unsigned char * AES::EncryptECB(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    EncryptBlock(alignIn + i, out + i, key);
  }
  
  delete[] alignIn;
  
  return out;
}

unsigned char * AES::DecryptECB(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    DecryptBlock(alignIn + i, out + i, key);
  }
  
  delete[] alignIn;
  
  return out;
}


unsigned char *AES::EncryptCBC(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned char * iv, unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  unsigned char *block = new unsigned char[blockBytesLen];
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    XorBlocks(block, alignIn + i, block, blockBytesLen);
    EncryptBlock(block, out + i, key);
	memcpy(block, out + i, blockBytesLen);
  }
  
  delete[] block;
  delete[] alignIn;

  return out;
}

unsigned char *AES::DecryptCBC(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned char * iv, unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  unsigned char *block = new unsigned char[blockBytesLen];
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    DecryptBlock(alignIn + i, out + i, key);
    XorBlocks(block, out + i, out + i, blockBytesLen);
	memcpy(block, alignIn + i, blockBytesLen);
  }
  
  delete[] block;
  delete[] alignIn;

  return out;
}

unsigned char *AES::EncryptCFB(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned char * iv, unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  unsigned char *block = new unsigned char[blockBytesLen];
  unsigned char *encryptedBlock = new unsigned char[blockBytesLen];
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    EncryptBlock(block, encryptedBlock, key);
    XorBlocks(alignIn + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, out + i, blockBytesLen);
  }
  
  delete[] block;
  delete[] encryptedBlock;
  delete[] alignIn;

  return out;
}

unsigned char *AES::DecryptCFB(unsigned char in[], unsigned int inLen, unsigned  char key[], unsigned char * iv, unsigned int &outLen)
{
  outLen = GetPaddingLength(inLen);
  unsigned char *alignIn  = PaddingNulls(in, inLen, outLen);
  unsigned char *out = new unsigned char[outLen];
  unsigned char *block = new unsigned char[blockBytesLen];
  unsigned char *encryptedBlock = new unsigned char[blockBytesLen];
  memcpy(block, iv, blockBytesLen);
  for (unsigned int i = 0; i < outLen; i+= blockBytesLen)
  {
    EncryptBlock(block, encryptedBlock, key);
    XorBlocks(alignIn + i, encryptedBlock, out + i, blockBytesLen);
    memcpy(block, alignIn + i, blockBytesLen);
  }
  
  delete[] block;
  delete[] encryptedBlock;
  delete[] alignIn;

  return out;
}

unsigned char * AES::PaddingNulls(unsigned char in[], unsigned int inLen, unsigned int alignLen)
{
  unsigned char * alignIn = new unsigned char[alignLen];
  memcpy(alignIn, in, inLen);
  return alignIn;
}

unsigned int AES::GetPaddingLength(unsigned int len)
{
  return (len / blockBytesLen) * blockBytesLen;
}

void AES::EncryptBlock(unsigned char in[], unsigned char out[], unsigned  char key[])
{
  unsigned char *w = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, w);
  unsigned char **state = new unsigned char *[4];
  state[0] = new unsigned  char[4 * Nb];
  int i, j, round;
  for (i = 0; i < 4; i++)
  {
    state[i] = state[0] + Nb * i;
  }


  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++)
    {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, w);

  for (round = 1; round <= Nr - 1; round++)
  {
    SubBytes(state);
    ShiftRows(state);
    MixColumns(state);
    AddRoundKey(state, w + round * 4 * Nb);
  }

  SubBytes(state);
  ShiftRows(state);
  AddRoundKey(state, w + Nr * 4 * Nb);

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++)
    {
      out[i + 4 * j] = state[i][j];
    }
  }

  delete[] state[0];
  delete[] state;
  delete[] w;
}

void AES::DecryptBlock(unsigned char in[], unsigned char out[], unsigned  char key[])
{
  unsigned char *w = new unsigned char[4 * Nb * (Nr + 1)];
  KeyExpansion(key, w);
  unsigned char **state = new unsigned char *[4];
  state[0] = new unsigned  char[4 * Nb];
  int i, j, round;
  for (i = 0; i < 4; i++)
  {
    state[i] = state[0] + Nb * i;
  }


  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++) {
      state[i][j] = in[i + 4 * j];
    }
  }

  AddRoundKey(state, w + Nr * 4 * Nb);

  for (round = Nr - 1; round >= 1; round--)
  {
    InvSubBytes(state);
    InvShiftRows(state);
    AddRoundKey(state, w + round * 4 * Nb);
    InvMixColumns(state);
  }

  InvSubBytes(state);
  InvShiftRows(state);
  AddRoundKey(state, w);

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++) {
      out[i + 4 * j] = state[i][j];
    }
  }

  delete[] state[0];
  delete[] state;
  delete[] w;
}


void AES::SubBytes(unsigned char **state)
{
  int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++)
    {
      t = state[i][j];
      state[i][j] = sbox[t / 16][t % 16];
    }
  }

}

void AES::ShiftRow(unsigned char **state, int i, int n)    // shift row i on n positions
{
  unsigned char t;
  int k, j;
  for (k = 0; k < n; k++)
  {
    t = state[i][0];
    for (j = 0; j < Nb - 1; j++)
    {
      state[i][j] = state[i][j + 1];
    }
    state[i][Nb - 1] = t;
  }
}

void AES::ShiftRows(unsigned char **state)
{
  ShiftRow(state, 1, 1);
  ShiftRow(state, 2, 2);
  ShiftRow(state, 3, 3);
}

unsigned char AES::xtime(unsigned char b)    // multiply on x
{
  unsigned char mask = 0x80, m = 0x1b;
  unsigned char high_bit = b & mask;
  b = b << 1;
  if (high_bit) {    // mod m(x)
    b = b ^ m;
  }
  return b;
}

unsigned char AES::mul_bytes(unsigned char a, unsigned char b)
{
  unsigned char c = 0, mask = 1, bit, d;
  int i, j;
  for (i = 0; i < 8; i++)
  {
    bit = b & mask;
    if (bit)
    {
      d = a;
      for (j = 0; j < i; j++)
      {    // multiply on x^i
        d = xtime(d);
      }
      c = c ^ d;    // xor to result
    }
    b = b >> 1;
  }
  return c;
}

void AES::MixColumns(unsigned char **state)
{
  unsigned char s[4], s1[4];
  int i, j;

  for (j = 0; j < Nb; j++)
  {
    for (i = 0; i < 4; i++)
    {
      s[i] = state[i][j];
    }

    s1[0] = mul_bytes(0x02, s[0]) ^ mul_bytes(0x03, s[1]) ^ s[2] ^ s[3];
    s1[1] = s[0] ^ mul_bytes(0x02, s[1]) ^ mul_bytes(0x03, s[2]) ^ s[3];
    s1[2] = s[0] ^ s[1] ^ mul_bytes(0x02, s[2]) ^ mul_bytes(0x03, s[3]);
    s1[3] = mul_bytes(0x03, s[0]) ^ s[1] ^ s[2] ^ mul_bytes(0x02, s[3]);
    for (i = 0; i < 4; i++)
    {
      state[i][j] = s1[i];
    }

  }

}

void AES::AddRoundKey(unsigned char **state, unsigned char *key)
{
  int i, j;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++)
    {
      state[i][j] = state[i][j] ^ key[i + 4 * j];
    }
  }
}

void AES::SubWord(unsigned char *a)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    a[i] = sbox[a[i] / 16][a[i] % 16];
  }
}

void AES::RotWord(unsigned char *a)
{
  unsigned char c = a[0];
  a[0] = a[1];
  a[1] = a[2];
  a[2] = a[3];
  a[3] = c;
}

void AES::XorWords(unsigned char *a, unsigned char *b, unsigned char *c)
{
  int i;
  for (i = 0; i < 4; i++)
  {
    c[i] = a[i] ^ b[i];
  }
}

void AES::Rcon(unsigned char * a, int n)
{
  int i;
  unsigned char c = 1;
  for (i = 0; i < n - 1; i++)
  {
    c = xtime(c);
  }

  a[0] = c;
  a[1] = a[2] = a[3] = 0;
}

void AES::KeyExpansion(unsigned char key[], unsigned char w[])
{
  unsigned char *temp = new unsigned char[4];
  unsigned char *rcon = new unsigned char[4];

  int i = 0;
  while (i < 4 * Nk)
  {
    w[i] = key[i];
    i++;
  }

  i = 4 * Nk;
  while (i < 4 * Nb * (Nr + 1))
  {
    temp[0] = w[i - 4 + 0];
    temp[1] = w[i - 4 + 1];
    temp[2] = w[i - 4 + 2];
    temp[3] = w[i - 4 + 3];

    if (i / 4 % Nk == 0)
    {
        RotWord(temp);
        SubWord(temp);
        Rcon(rcon, i / (Nk * 4));
      XorWords(temp, rcon, temp);
    }
    else if (Nk > 6 && i / 4 % Nk == 4)
    {
      SubWord(temp);
    }

    w[i + 0] = w[i - 4 * Nk] ^ temp[0];
    w[i + 1] = w[i + 1 - 4 * Nk] ^ temp[1];
    w[i + 2] = w[i + 2 - 4 * Nk] ^ temp[2];
    w[i + 3] = w[i + 3 - 4 * Nk] ^ temp[3];
    i += 4;
  }

  delete []rcon;
  delete []temp;

}


void AES::InvSubBytes(unsigned char **state)
{
  int i, j;
  unsigned char t;
  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < Nb; j++)
    {
      t = state[i][j];
      state[i][j] = inv_sbox[t / 16][t % 16];
    }
  }
}

void AES::InvMixColumns(unsigned char **state)
{
  unsigned char s[4], s1[4];
  int i, j;

  for (j = 0; j < Nb; j++)
  {
    for (i = 0; i < 4; i++)
    {
      s[i] = state[i][j];
    }
    s1[0] = mul_bytes(0x0e, s[0]) ^ mul_bytes(0x0b, s[1]) ^ mul_bytes(0x0d, s[2]) ^ mul_bytes(0x09, s[3]);
    s1[1] = mul_bytes(0x09, s[0]) ^ mul_bytes(0x0e, s[1]) ^ mul_bytes(0x0b, s[2]) ^ mul_bytes(0x0d, s[3]);
    s1[2] = mul_bytes(0x0d, s[0]) ^ mul_bytes(0x09, s[1]) ^ mul_bytes(0x0e, s[2]) ^ mul_bytes(0x0b, s[3]);
    s1[3] = mul_bytes(0x0b, s[0]) ^ mul_bytes(0x0d, s[1]) ^ mul_bytes(0x09, s[2]) ^ mul_bytes(0x0e, s[3]);

    for (i = 0; i < 4; i++)
    {
      state[i][j] = s1[i];
    }
  }
}

void AES::InvShiftRows(unsigned char **state)
{
  ShiftRow(state, 1, Nb - 1);
  ShiftRow(state, 2, Nb - 2);
  ShiftRow(state, 3, Nb - 3);
}

void AES::XorBlocks(unsigned char *a, unsigned char * b, unsigned char *c, unsigned int len)
{
  for (unsigned int i = 0; i < len; i++)
  {
    c[i] = a[i] ^ b[i];
  }
}


std::string AES::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) 
{
	char  base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}
	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while((i++ < 3))
			ret += '=';
	}
	return ret;

}

std::string AES::URLEncode(std::string value)
{
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";

	std::string escaped="";
	int max = value.size();
	for(int i=0; i<max; i++)
	{
		if( value[i] == '$' || value[i] == '&'|| value[i] == '+' || value[i] == ',' || value[i] == '/' || value[i] == ':' || value[i] == '[' || value[i] == ']' ||
			value[i] == '\\'|| value[i] == ';'|| value[i] == '=' || value[i] == '?' || value[i] == '@' || value[i] == '#' || value[i] < 0x20 || value[i] > 0x7E)
		{
			// escape this char 
			escaped+= '%';
			escaped+= DEC2HEX[(value[i] >> 4)&0x0F];
			escaped+= DEC2HEX[value[i] & 0x0F];
		}
		else
		{
			if( value[i] == ' ')
			{
				escaped += '+';
			}
			else
			{
				escaped += value[i];
			}
		}
	}
	return escaped;
}



std::string AES::WrapperEncrypt(std::string sdata)
{
	char *outbuff = new char[sdata.size()+100];
	DWORD osize = sdata.size()+100;

	char *inbuff = new char[sdata.size()+100];
	memset(inbuff,0,sdata.size()+100);
	memcpy(inbuff,sdata.c_str(),sdata.length());
	DWORD isize = sdata.length()+15;
	isize = isize / 16;
	isize = isize * 16;

	unsigned char keystr[32] = { 0 };

	
	unsigned int len = 0;
	unsigned char iv[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	ZeroMemory(keystr, 32);
	// "freezeprogs.com"
	// 2966B453BC877B56CD55345386A8D
	keystr[0] = '2';keystr[16] = 'C';keystr[6] = '5';keystr[24] = '8';keystr[17] = 'D';keystr[13] = 'B';keystr[10] = '8';keystr[26] = 'A';
	keystr[23] = '3';keystr[20] = '3';keystr[4] = 'B';keystr[25] = '6';keystr[19] = '5';keystr[12] = '7';keystr[7] = '3';keystr[1] = '9';
	keystr[3] = '6';keystr[11] = '7';keystr[5] = '4';keystr[8] = 'B';keystr[28] = 'D';keystr[15] = '6';keystr[2] = '6';keystr[9] = 'C';
	keystr[14] = '5';keystr[21] = '4';keystr[22] = '5';keystr[27] = '8';keystr[18] = '5';

	unsigned char *out = EncryptCBC((unsigned char *)inbuff, isize, keystr, iv, len);
	memset(keystr,32,32);
	std::string edata = base64_encode(out, len);
	edata = URLEncode(edata);
	delete[] out;
	return edata;
}




