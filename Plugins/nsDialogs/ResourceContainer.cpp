#include "ResourceContainer.h"
#include "Richedit.h"
#include "RTFLoader.h"

extern HINSTANCE g_hInstance;

ResourceContainer::ResourceContainer(void)
{
}

ResourceContainer::~ResourceContainer(void)
{
}

void ResourceContainer::extractFileFromResource(DWORD resource_id, LPCTSTR outputFileName)
{

    HRSRC hRes = 0;
    HGLOBAL hData = 0;
    LPVOID pData;

    hRes = FindResource(g_hInstance, MAKEINTRESOURCE(resource_id), TEXT("IMG"));
    if(hRes == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function FindResource failed with error"), -1);
    }

    DWORD dwSize = SizeofResource(g_hInstance, hRes);
    if(dwSize == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function SizeofResource failed with error"), -1);
    }

    hData = LoadResource(g_hInstance, hRes);
    if(hData == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function LoadResource failed with error"), -1);
    }

    pData = LockResource(hData);
    if(pData == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function LockResource failed with error"), -1);
    }

    HANDLE File = CreateFile(outputFileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, OPEN_ALWAYS, 0, 0);
    if(File == INVALID_HANDLE_VALUE)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function CreateFile failed with error"), -1);
    }

    DWORD Written=0;

    if(WriteFile(File, pData, dwSize, &Written, 0)==NULL)
    {
        CloseHandle(File);
        //RAISE_APPLICATION_ERROR(TEXT("Couldn't write to file"), -1);
    }
    
    CloseHandle(File);
}

struct RTFMem
{
    byte * buff;
    int pos;
    int size;
};


void ResourceContainer::LoadRTFFromResource(DWORD resource_id, HWND hwnd)
{

    HRSRC hRes = 0;
    HGLOBAL hData = 0;
    LPVOID pData;

    hRes = FindResource(g_hInstance, MAKEINTRESOURCE(resource_id), TEXT("IMG"));
    if(hRes == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function FindResource failed with error"), -1);
    }

    DWORD dwSize = SizeofResource(g_hInstance, hRes);
    if(dwSize == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function SizeofResource failed with error"), -1);
    }

    hData = LoadResource(g_hInstance, hRes);
    if(hData == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function LoadResource failed with error"), -1);
    }

    pData = LockResource(hData);
    if(pData == NULL)
    {
        //RAISE_APPLICATION_ERROR(TEXT("Function LockResource failed with error"), -1);
    }

    // open and read file 
    RTFMem m_rtfmem;
    HWND hRichEdit = (HWND)hwnd;
    DWORD dw = 0;
    SendMessage(hRichEdit,0x0443,0,GetSysColor(15));
    SendMessage(hRichEdit, EM_EXLIMITTEXT, 0, 0x7fffffff);
    
    m_rtfmem.buff = (byte*)pData;
    

    m_rtfmem.pos = 0;
    m_rtfmem.size = dwSize;

    EDITSTREAM stEditStream;
    stEditStream.dwCookie = (DWORD_PTR)&m_rtfmem;
    stEditStream.dwError = 0;
    stEditStream.pfnCallback = EditStreamCallback;

    SendMessage(hRichEdit, EM_STREAMIN, SF_RTF, (LPARAM)&stEditStream);
    

}
