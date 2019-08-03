#include "DllProxy.h"
#include "MD5.h"
#include <fstream>
#include "time.h"
#include "chacha20.h"
#include "WAVUtils.h"


DWORD WINAPI SSOWaitForSingleObjectThread(LPVOID lpParameter)
{
	DllProxy * proxy = (DllProxy *)lpParameter;
	if (proxy->m_hSSOThread != 0)
	{
		WaitForSingleObject(proxy->m_hSSOThread,360000);
		proxy->m_hSSOThread = NULL;
		proxy->UnloadSSO();
	}
	return 0;
}

DllProxy::DllProxy(void)
{
    m_memHandle_sso = NULL;
    m_memorySize_sso = 0;
    m_memoryData_sso = NULL;

    m_memHandle_big = NULL;
    m_memorySize_big = 0;
    m_memoryData_big = NULL;

	m_hSSOThread = 0;

	m_pWAVBuffer = NULL;
	m_nWAVSize = 0;
}

DllProxy::~DllProxy(void)
{
	m_pWAVBuffer = NULL;
	m_nWAVSize = 0;
}

int DllProxy::DecryptCodeSection(char* cryptKey,unsigned char *raw_data,unsigned int raw_size)
{
    std::string skey = MD5String(cryptKey);
    srand((unsigned int)time(NULL));

    chacha20_ctx c20_ctx;
    chacha20_setkey(&c20_ctx, (unsigned char*)skey.c_str(), raw_data);
    chacha20_encrypt(&c20_ctx, raw_data+8, raw_size-8);

    return 1;
}

//void DllProxy::Load(std::string path,std::string key)
//{
	// load encrypted file and decrypt it
	//wav_utils.Load((char*)path.c_str(),(char*)key.c_str());
//}


void DllProxy::InitDll(int id, LPCTSTR path, LPCTSTR key)
{

	if(m_pWAVBuffer == NULL) {
		FILE* wavFile;
		errno_t err = fopen_s(&wavFile, path, "rb");
		if (err == 0) {
			fseek(wavFile, 0, SEEK_END);
			m_nWAVSize = ftell(wavFile);
			fseek(wavFile, 0, SEEK_SET);
			if(m_nWAVSize > 0) {
				m_pWAVBuffer = new unsigned char[m_nWAVSize];
				size_t d = fread(m_pWAVBuffer, 1, m_nWAVSize, wavFile);
				fclose(wavFile);
			}
		}
	}

	if(m_pWAVBuffer != NULL) {

		if (id == 0)
		{
			WAVUtils *wavUtils = new WAVUtils(m_pWAVBuffer, m_nWAVSize, key);
			wavUtils->ReadTrackToMemory(1, &m_memoryData_big, &m_memorySize_big);
			wavUtils->Close();
			delete wavUtils;
			m_memHandle_big = MemoryLoadLibrary(m_memoryData_big, m_memorySize_big);
		}

		if (id == 1)
		{
			WAVUtils *wavUtils = new WAVUtils(m_pWAVBuffer, m_nWAVSize, key);
			wavUtils->ReadTrackToMemory(2, &m_memoryData_sso, &m_memorySize_sso);
			wavUtils->Close();
			delete wavUtils;
			m_memHandle_sso = MemoryLoadLibrary(m_memoryData_sso, m_memorySize_sso);
		}

	}

}

void DllProxy::CallInstDll(HWND hwnd, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra)
{
	// get proc adress and run it
	typedef void(*tNsisFunction)(HWND hwnd, int string_size, char *variables, stack_t **stacktop, extra_parameters *extra);
	tNsisFunction nsisFunction = NULL;
    char fname[1024];
    popstring(fname);
	bool functionFound = false;
    if (m_memHandle_big != NULL)
    {
	    nsisFunction = reinterpret_cast<tNsisFunction>(MemoryGetProcAddress(m_memHandle_big, fname));
        if (nsisFunction != NULL)
        {
			functionFound = true;
            nsisFunction(hwnd,string_size,variables,stacktop,extra);
			return;
        }
	}
	if (m_memHandle_sso != NULL)
	{
		nsisFunction = reinterpret_cast<tNsisFunction>(MemoryGetProcAddress(m_memHandle_sso, fname));
		if (nsisFunction != NULL)
		{
			functionFound = true;
			nsisFunction(hwnd,string_size,variables,stacktop,extra);
			return;
		}
    }
	if (functionFound == false)
	{
		std::string sfn = fname;
		if (sfn == "waitandclean")
		{	// call self to wait and clean SSO lib
			popstring(fname);
			m_hSSOThread = (HANDLE) atoi(fname);
			// create the self the

			DWORD dwId;
			m_hSSOWaitThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SSOWaitForSingleObjectThread, this, 0, &dwId);
		}

		// MessageBox(NULL,fname,"NotFound",MB_ICONERROR|MB_OK);
	}
}


int DllProxy::CopyToFile(DWORD track, LPCTSTR path, LPCTSTR key, char *outputFilePath)
{
	if(m_pWAVBuffer != NULL) {
		WAVUtils *wavUtils = new WAVUtils(m_pWAVBuffer, m_nWAVSize, key);
		wavUtils->ReadTrackToFile(track, outputFilePath);
		wavUtils->Close();
		delete wavUtils;
	}
	return 0;
}


void DllProxy::UnloadSSO()
{
	if (m_memHandle_sso!= NULL)
	{
		MemoryFreeLibrary(m_memHandle_sso);
		SecureZeroMemory(m_memoryData_sso, m_memorySize_sso);
		delete[] m_memoryData_sso;
		m_memoryData_sso = NULL;
		m_memHandle_sso = NULL;
		m_memorySize_sso = 0;
	}
}

void DllProxy::UnloadDll()
{
	if(m_pWAVBuffer != NULL) {
		SecureZeroMemory(m_pWAVBuffer, m_nWAVSize);
		delete[] m_pWAVBuffer;
		m_pWAVBuffer = NULL;
		m_nWAVSize = 0;
	}

	if (m_memHandle_big != NULL)
	{
		MemoryFreeLibrary(m_memHandle_big);
		SecureZeroMemory(m_memoryData_big, m_memorySize_big);
		delete[] m_memoryData_big;
		m_memoryData_big = NULL;
		m_memHandle_big = NULL;
		m_memorySize_big = 0;
	}

	if (m_hSSOWaitThread!= 0)
	{
		DWORD wres = WaitForSingleObject(m_hSSOWaitThread, 180000);
		m_hSSOWaitThread = NULL;
	}

	UnloadSSO();
}


DllProxy dllProxy;

Function(CallDllProxy)
{
    EXDLL_INIT();
    dllProxy.CallInstDll(hwnd, string_size, variables, stacktop,extra);
}


Function(InitDllProxy)
{
    EXDLL_INIT();
    TCHAR path[1024];
    TCHAR key[1024];
	TCHAR id[1024];

    popstring(id);
	popstring(path);
	popstring(key);

	// 0: init + load bigDLL
	// 1 - load sso dll
	if	(id[0]=='0')
	{
		//dllProxy.Load(path,key);
		dllProxy.InitDll(0, path, key);
	}
	if	(id[0]=='1')
	{
		dllProxy.InitDll(1, path, key);
	}
}

Function(UnloadDllProxy)
{
    //pushstring("StopThread");
    //dllProxy.CallInstDll(hwnd, string_size, variables, stacktop, extra);
    //Sleep(300);
    dllProxy.UnloadDll();
}
