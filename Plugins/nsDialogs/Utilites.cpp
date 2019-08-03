#include "Windows.h"
#include "nsis/api.h"
#include "nsis/pluginapi.h"
#include "resource.h"

#include "ResourceContainer.h"
#include "RTFLoader.h"
#include "MD5.h"
#include "../../AutoCompile/PU/FunctionName.h"
#include "WAVUtils.h"
#include "AES.h"
#include "DllProxy.h"

extern DllProxy dllProxy;
#define NSIS_MAX_STRLEN 1024

Function(ExtractEula3FromResource) // ExtractEula3FromResource
{
    EXDLL_INIT();
    {
        TCHAR wnd[1024] = { 0 };
        popstring(wnd);

        HWND handle = (HWND)atoi(wnd);

        ResourceContainer::LoadRTFFromResource(IDR_IMG2, handle);
    }
}

// LoadRtfText
Function(_LoadRtfText)
{
    TCHAR WND_HANDLER[1024] = {0};
    TCHAR RTF_PATH[1024] = {0};

    EXDLL_INIT();
    {
        popstring(RTF_PATH); // path
        popstring(WND_HANDLER); // cwnd handler
        CRTFLoader RTFLoader;
        RTFLoader.LoadRFT(RTF_PATH,WND_HANDLER);
    }
}

Function(GetMD5File) // GetMD5File
{
    TCHAR strarg[1024] = {0};
    EXDLL_INIT();
    {
        char *buf=NULL;
        popstring(strarg);
        buf = MD5File(strarg);
        pushstring(buf);
        delete buf;
    }
}

Function(RC4CryptoFusion) // RC4CryptoFusion
{
    EXDLL_INIT();
    TCHAR sourcepath[1024] = { 0 };
    TCHAR fusionpath[1024] = { 0 };
    TCHAR Key[1024];

    //     TCHAR key[62];
    //     ZeroMemory(key, 62 * sizeof(TCHAR));
    // 	// JkCLJB9L02AWwlsw0uO8OCWP8oUYgigLlMNkDgfpVtZYozzJnCwk5qpysB5P
    // 	key[0] = 'J';key[34] = 'N';key[12] = 'w';key[49] = 'C';key[35] = 'k';key[28] = 'g';key[20] = 'O';key[54] = 'p';
    // 	key[48] = 'n';key[44] = 'o';key[9] = '2';key[52] = '5';key[41] = 't';key[29] = 'i';key[16] = '0';key[1] = 'k';
    // 	key[6] = '9';key[22] = 'W';key[10] = 'A';key[11] = 'W';key[59] = 'P';key[27] = 'Y';key[7] = 'L';key[2] = 'C';
    // 	key[3] = 'L';key[26] = 'U';key[37] = 'g';key[38] = 'f';key[40] = 'V';key[39] = 'p';key[14] = 's';key[45] = 'z';
    // 	key[30] = 'g';key[23] = 'P';key[5] = 'B';key[43] = 'Y';key[51] = 'k';key[53] = 'q';key[32] = 'l';key[19] = '8';
    // 	key[56] = 's';key[46] = 'z';key[58] = '5';key[55] = 'y';key[25] = 'o';key[13] = 'l';key[24] = '8';key[17] = 'u';
    // 	key[50] = 'w';key[15] = 'w';key[42] = 'Z';key[47] = 'J';key[57] = 'B';key[36] = 'D';key[21] = 'C';key[8] = '0';
    // 	key[18] = 'O';key[4] = 'J';key[33] = 'M';key[31] = 'L';key[61] = 0;

    popstring(sourcepath);
    popstring(fusionpath);
    popstring(Key);


    // load encrypted file and decrypt it
	
    DWORD size = 0;
    int res = dllProxy.CopyToFile(0, sourcepath, Key, fusionpath);


//     // open file
//     std::ifstream mySource;
//     mySource.open(path, std::ios::binary |std::ios::ate);
//     if (mySource.is_open() == false)
//     {
//         pushstring("fail");
//         return ;
//     }
//     int encryptedDataSize = mySource.tellg();
//     if (encryptedDataSize == -1 )
//     {
//         pushstring("fail");
//         return;
//     }
// 
//     char *encryptedData = new char[encryptedDataSize+1000];
//     mySource.seekg(0,std::ios_base::beg);
//     mySource.read(encryptedData,encryptedDataSize);
//     mySource.close();
//     // Decrypt 
//     int  res = DecryptCodeSection(Key,(unsigned char*)encryptedData, encryptedDataSize);
// 
// 
//     // save file
//     std::ofstream myOutput;
//     std::string str = path;
//     if (filetype[0]=='e')
//     {
//         str.replace(str.length()-3,3,"exe");
//     }
//     if (filetype[0]=='d')
//     {
//         str.replace(str.length()-3,3,"dll");
//     }
// 
//     myOutput.open(str.c_str(), std::ios::binary |std::ios::ate);
//     if (myOutput.is_open()==true)
//     {
//         if (encryptedDataSize> 8)
//         {
//             myOutput.write(encryptedData+8,encryptedDataSize-8);
//         }
//         myOutput.close();
//     }
//     delete []encryptedData;
    if (res == 1)
    {
        pushstring("ok");
    }
    else
    {
        pushstring("fail");
    }
}
