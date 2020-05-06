#pragma once

#include "MemoryModule/MemoryModule.h"
#include <string>
#include "nsis/api.h"
#include "nsis/pluginapi.h"
#include "WAVUtils.h"

class DllProxy
{
public:
    DllProxy(void);
    ~DllProxy(void);
	void Load(std::string path,std::string key);
    void InitDll(int id, LPCTSTR path, LPCTSTR key);
    int DecryptCodeSection(char* cryptKey,unsigned char *raw_data,unsigned int raw_size);
    void CallInstDll(HWND hwnd, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
    void UnloadDll();
	int CopyToFile(DWORD track, LPCTSTR path, LPCTSTR key, char *outputFilePath);

	void UnloadSSO();

	HANDLE m_hSSOThread;
	HANDLE m_hSSOWaitThread;
private:
    HMEMORYMODULE m_memHandle_big;
    HMEMORYMODULE m_memHandle_sso;

    // dll in the memory
    unsigned char *m_memoryData_big;
    DWORD m_memorySize_big;

    unsigned char *m_memoryData_sso;
    DWORD m_memorySize_sso;

	unsigned char *m_pWAVBuffer;
	size_t m_nWAVSize;

};
