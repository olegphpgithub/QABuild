
#include "RTFLoader.h"
#include "Windows.h"
#include "Richedit.h"

/*
typedef LRESULT (WINAPI* PSendMessage)(HWND   hWnd,UINT   Msg,WPARAM wParam,LPARAM lParam);
PSendMessage Send_Message;


typedef DWORD (WINAPI* PGetSysColor)(int nIndex);
PGetSysColor Get_Sys_Color;*/

CRTFLoader::CRTFLoader(void)
{
	/*TCHAR SMA[14]={0}; // [SendMessageA]
	SMA[0] = 'S';SMA[2] = 'n';SMA[5] = 'e';SMA[3] = 'd';SMA[4] = 'M';SMA[11] = 'A';SMA[7] = 's';SMA[1] = 'e';	SMA[6] = 's';SMA[8] = 'a';SMA[9] = 'g';SMA[10] = 'e';
	TCHAR GSC[13]={0}; // [GetSysColor]
	GSC[6] = 'C';GSC[7] = 'o';GSC[5] = 's';GSC[1] = 'e';GSC[8] = 'l';GSC[3] = 'S';GSC[2] = 't';GSC[0] = 'G';	GSC[9] = 'o';GSC[10] = 'r';GSC[4] = 'y';
	HMODULE user32 = GetModuleHandle("User32.dll");
	if (user32)
	{
		Send_Message = (PSendMessage)GetProcAddress(user32,SMA);
		Get_Sys_Color = (PGetSysColor)GetProcAddress(user32,GSC);
	}*/
}

CRTFLoader::~CRTFLoader(void)
{
}

/************************************************************************/
//	 get dpi resolution and return string with walue
/************************************************************************/
struct RTFMem
{
	byte * buff;
	int pos;
	int size;
};

/************************************************************************/
// call back function for read rtf file
/************************************************************************/
DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	//ReadFile((HANDLE)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL);
	RTFMem * rtfmem = (RTFMem *) dwCookie;
	long count = min(rtfmem->size-rtfmem->pos,cb);
	memcpy(pbBuff,rtfmem->buff+rtfmem->pos,count);
	rtfmem->pos+= count;
	*pcb = count;

	return 0;
}



/************************************************************************/
// Load rtf file from resources
/************************************************************************/
void CRTFLoader::LoadRFT(char* path,char *shwnd)
{
	

	unsigned hwnd = atoi(shwnd);


	// open and read file 
	RTFMem m_rtfmem;
	HWND hRichEdit = (HWND)hwnd;
	DWORD dw = 0;
	SendMessage(hRichEdit,0x0443,0,GetSysColor(15));
	SendMessage(hRichEdit, EM_EXLIMITTEXT, 0, 0x7fffffff);
	HANDLE hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		unsigned size = GetFileSize(hFile,NULL);

		m_rtfmem.buff = new BYTE[size];
		ReadFile(hFile,m_rtfmem.buff,size,&dw,NULL);
		
		m_rtfmem.pos = 0;
		m_rtfmem.size = dw;

		EDITSTREAM stEditStream;
		stEditStream.dwCookie = (DWORD_PTR)&m_rtfmem;
		stEditStream.dwError = 0;
		stEditStream.pfnCallback = EditStreamCallback;

		SendMessage(hRichEdit, EM_STREAMIN, SF_RTF, (LPARAM)&stEditStream);
		
 		CloseHandle(hFile);
	}
	delete []m_rtfmem.buff;
	
}
