#pragma once
#include <string>
#include "Windows.h"

DWORD CALLBACK EditStreamCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb);

class CRTFLoader
{
public:
	CRTFLoader(void);
	~CRTFLoader(void);
	//static std::string GetDPIDimension();
	//static std::string GetMajorOsVer();
	//;static std::string GetMinorOsVer();
	static void LoadRFT(char* path,char *shwnd);
};
