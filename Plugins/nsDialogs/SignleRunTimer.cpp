#include "windows.h"
#include "nsis/api.h"
#include "nsis/pluginapi.h"

extern HINSTANCE g_hInstance;

typedef struct _THREADPARAM {
	int iInterval;
	int iFunction;
	extra_parameters *extra;
} THREADPARAM, *PTHREADPARAM;


static THREADPARAM g_ThreadParam;
static BOOLEAN g_bStopImmediately;

static DWORD WINAPI SingleTimerThreadProc(void *);
static UINT_PTR PluginCallback(enum NSPIM);

Function(StartTimer)
{
	int iTemp;
	DWORD uThreadID;
	HANDLE hThread;
	EXDLL_INIT();
	extra->RegisterPluginCallback(g_hInstance, PluginCallback);
	iTemp = popint();
	if (iTemp <= 0) {
		extra->exec_flags->exec_error = TRUE;
		pushint(1);
		return;
	}
	g_ThreadParam.iInterval = iTemp;

	iTemp = popint();
	if (iTemp <= 0) {
		extra->exec_flags->exec_error = TRUE;
		pushint(3);
		return;
	}
	g_ThreadParam.iFunction = iTemp;
	g_ThreadParam.extra = extra;
	g_bStopImmediately = FALSE;

	hThread = (HANDLE)CreateThread(NULL, 0, SingleTimerThreadProc, &g_ThreadParam, 0, &uThreadID);
	CloseHandle(hThread);
}

Function(StopTimer)
{
	g_bStopImmediately = TRUE;
}

static DWORD WINAPI SingleTimerThreadProc(void *lParam)
{
	PTHREADPARAM pThreadParam;
	pThreadParam = (PTHREADPARAM)lParam;
	
	int sleep_count = 0;
	while (!g_bStopImmediately) 
	{
		Sleep(1000);
		sleep_count++;
		if (sleep_count > pThreadParam->iInterval)
		{
			pThreadParam->extra->ExecuteCodeSegment(pThreadParam->iFunction - 1, 0);
			sleep_count = 0;
		}
	}

	return 0;
}
static UINT_PTR PluginCallback(enum NSPIM msg)
{
	return 0;
}
