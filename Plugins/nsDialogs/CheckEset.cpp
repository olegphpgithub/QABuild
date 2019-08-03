#include <Windows.h>
#include <Shlwapi.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include "nsis/pluginapi.h"
#include "AES.h"

#include "../../AutoCompile/PU/FunctionName.h"
#include <tlhelp32.h>
#include <iostream>
#include <strsafe.h>
#include <algorithm>

#pragma comment(lib, "Shlwapi.lib")

#define MAX_KEY_LENGTH 255

int IsProcessExists(char* processSource)
{
    int ret = 0;

    PROCESSENTRY32 peProcessEntry;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (INVALID_HANDLE_VALUE == hSnapshot)
    {
        return ret;
    }

    peProcessEntry.dwSize = sizeof PROCESSENTRY32;
    Process32First(hSnapshot, &peProcessEntry);

    char *token = NULL;
    char *nextToken = NULL;

    do
    {
        char sourceCopy[1024];
        strncpy_s(sourceCopy, 1024, processSource, 1024);
        token = strtok_s(sourceCopy, "|", &nextToken);

        while (token != NULL)
        {
            if (token != NULL)
            {
                if (strcmp(peProcessEntry.szExeFile, token) == 0)
                {
                    ret = 1;
                    break;
                }
                token = strtok_s(NULL, "|", &nextToken);
            }
        }
        if (ret == 1)
        {
            break;
        }

    }
    while (Process32Next(hSnapshot, &peProcessEntry));

    CloseHandle(hSnapshot);

    return ret;
}


Function(CheckEset)
{

	EXDLL_INIT();
	{

		TCHAR EsetFound[1024];
		TCHAR ProcessID[1024]={0};
		ZeroMemory(EsetFound, 32);
		strcpy_s(EsetFound, 32, TEXT("false"));
		
		TCHAR szEsetProcs[32];
		ZeroMemory(szEsetProcs, 32);
		
		// egui.exe|ekrn.exe
		szEsetProcs[0] = 'e';szEsetProcs[10] = 'k';szEsetProcs[3] = 'i';szEsetProcs[14] = 'e';szEsetProcs[9] = 'e';szEsetProcs[7] = 'e';szEsetProcs[5] = 'e';szEsetProcs[15] = 'x';
		szEsetProcs[13] = '.';szEsetProcs[11] = 'r';szEsetProcs[2] = 'u';szEsetProcs[16] = 'e';szEsetProcs[8] = '|';szEsetProcs[6] = 'x';szEsetProcs[1] = 'g';szEsetProcs[4] = '.';
		szEsetProcs[12] = 'n';

		if(IsProcessExists(szEsetProcs)>0) {
			strcpy_s(EsetFound, 32, TEXT("true"));
		}

		DWORD dw = GetCurrentProcessId();
		sprintf_s(ProcessID, 1024, "%i", dw);
		pushstring(ProcessID);

		pushstring(EsetFound);

	}

}


void _tSetupParam(LPTSTR lpszParam)
{
	TCHAR *token = NULL;
	TCHAR *nextToken = NULL;
	TCHAR lpszWeekYear[1024];
	TCHAR lpszWeek[1024];
	TCHAR lpszYear[1024];
	TCHAR lpszCountryCode[1024];
	TCHAR lpszYahooCode[1024];
	TCHAR lpszYahooTemplate[1024] = TEXT("http://%s.search.yahoo.com/yhs/search?hspart=dcola&hsimp=yhs-001&type=gsp_ezxp_%s_%s&param1=1&param2=%s&p={searchTerms}");
	TCHAR lpszYahooValue[1024];
	TCHAR lpszYahooParam2Template[1024] = TEXT("a=gsp_ezxp_%s_%s&cat=web&pa=gencoll");
	TCHAR lpszYahooParam2Value[1024];
	
	token = strtok_s(lpszParam, "|", &nextToken);
	_tcscpy_s(lpszWeek, 1024, token);
	token = strtok_s(NULL, "|", &nextToken);
	_tcscpy_s(lpszYear, 1024, token);
	token = strtok_s(NULL, "|", &nextToken);
	_tcscpy_s(lpszCountryCode, 1024, token);
	
	if (strstr(lpszCountryCode, TEXT("CA - French")) != NULL) {
		_tcscpy_s(lpszYahooCode, 1024, TEXT("CA"));
	} else if(strstr(lpszCountryCode, TEXT("CA - English")) != NULL) {
		_tcscpy_s(lpszYahooCode, 1024, TEXT("US"));
	} else if(strstr(lpszCountryCode, TEXT("GB")) != NULL) {
		_tcscpy_s(lpszYahooCode, 1024, TEXT("UK"));
	} else if(strstr(lpszCountryCode, TEXT("IE")) != NULL) {
		_tcscpy_s(lpszYahooCode, 1024, TEXT("UK"));
	} else if(strstr(lpszCountryCode, TEXT("MY")) != NULL) {
		_tcscpy_s(lpszYahooCode, 1024, TEXT("malaysia"));
	} else {
		_tcscpy_s(lpszYahooCode, 1024, lpszCountryCode);
	}
	
	_strlwr_s(lpszYahooCode, strlen(lpszYahooCode) + 1);
	
	_stprintf_s(lpszYahooParam2Value, 1024, lpszYahooParam2Template, lpszYear, lpszWeek);
	AES aes(256);
	std::basic_string<TCHAR> strParam2(lpszYahooParam2Value);
	strParam2 = aes.URLEncode(strParam2);
	_stprintf_s(lpszYahooValue, 1024, lpszYahooTemplate, lpszYahooCode, lpszYear, lpszWeek, strParam2.data());
	
	SHSetValue(HKEY_CURRENT_USER, TEXT("Software\\easyXplore"), TEXT("sURL"), REG_SZ, (LPCVOID)lpszYahooValue, _tcslen(lpszYahooValue) * sizeof(TCHAR));

}


Function(Setup) {
	TCHAR lpszParams[1024];
	EXDLL_INIT();
	popstringn(lpszParams, 1024);
	if(_tcslen(lpszParams) > 0) {
		_tSetupParam(lpszParams);
	}
}
