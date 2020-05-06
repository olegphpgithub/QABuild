#include <windows.h>
#include "nsis/api.h"
#include "nsis/pluginapi.h"

char captionText[1024];
char bodyText1[1024];
char bodyText2[1024];
char comboText[1024];
char buttonText1[1024];
char buttonText2[1024];
char lpszTimeOut[1024];
char ipret[1024];

HANDLE hTimedOutThread;

HWND hipStatic1;
HWND hipStatic2;
HWND hipButton1;
HWND hipButton2;


#define ID_INPUT_BUTTON1	1003
#define ID_INPUT_BUTTON2	1004
#define ID_INPUT_STATIC1	1001
#define ID_INPUT_STATIC2	1012
#define ID_INPUT_STATIC3	1013

extern HINSTANCE g_hInstance;


DWORD WINAPI nsMessageBoxTimedOutThread(_In_ LPVOID lpParameter)
{
	
	DWORD dwTimeOut;
	dwTimeOut = atoi(lpszTimeOut);
	if(dwTimeOut == 0) {
		hTimedOutThread = NULL;
		ExitThread(0);
	}
	Sleep(dwTimeOut);
	SendMessage(hipButton2, WM_LBUTTONDOWN, 0, 0);
	SendMessage(hipButton2, WM_LBUTTONUP, 0, 0);

	hTimedOutThread = NULL;
	ExitThread(0);
	
}

int GetDPIDimension()
{
    int result = 96;
    HDC hdc = GetDC(NULL);
    if (hdc)
    {
        result = GetDeviceCaps(hdc, LOGPIXELSX);
        ReleaseDC(NULL, hdc);
    }
    return result;
}

HWND nsCreateControl(HWND hwnd, int nID, LPCSTR lpControl, LPCSTR lpCaption, int x, int y, int w, int h, DWORD dwEstiloEx, DWORD dwEstilos)
{
    HFONT hFont;

    float coeff = (float)GetDPIDimension()/96;
    HWND hCtrl = 0;
    hCtrl = CreateWindowEx(dwEstiloEx,lpControl,lpCaption,dwEstilos,x,y,w,h,hwnd,(HMENU)nID,GetModuleHandle(NULL),0);
    hFont = CreateFont((int)(13*coeff),0,0,0,0,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH| FF_DONTCARE,"Tahoma");
    SendMessage(hCtrl,WM_SETFONT,(WPARAM) hFont, (LPARAM)MAKELPARAM(TRUE, 0));

    return hCtrl;
}

LRESULT CALLBACK nsDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) //Listo!
{
    switch (msg)
    {
    case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {
            case ID_INPUT_BUTTON1:
                {
                    if(hTimedOutThread != NULL) {
                        TerminateThread(hTimedOutThread, 0);
                    }
                    setuservariable(atoi(ipret), "OK");
                    EndDialog(hWnd, false); break;
                }
            case ID_INPUT_BUTTON2:
                {
                    if(hTimedOutThread != NULL) {
                        TerminateThread(hTimedOutThread, 0);
                    }
                    setuservariable(atoi(ipret), "CANCEL");
                    EndDialog(hWnd, false);
                    break;
                }
            }
            break;
        }
    case WM_CTLCOLORSTATIC:
        {
            if ((HWND)lParam ==  hipStatic1)
            {
                HDC hdcStatic = (HDC) wParam;
                SetBkColor(hdcStatic, RGB(0xFF,0xFF,0xFF));
                return (INT_PTR)CreateSolidBrush(RGB(0xFF,0xFF,0xFF));
            }
            break;
        }
    case WM_CLOSE:
        {
            if(hTimedOutThread != NULL) {
                TerminateThread(hTimedOutThread, 0);
            }
            setuservariable(atoi(ipret), "BREAK");
            EndDialog(hWnd, false);	break;
        }break;

    case WM_DESTROY:
        {
            EndDialog(hWnd, false); break;
        }
    case WM_INITDIALOG:
        {
            float coeff = (float)GetDPIDimension()/96;
            hipStatic2 = nsCreateControl(hWnd, ID_INPUT_STATIC2, "STATIC", "", 0, 0, (int)(400*coeff),(int)(80*coeff), 0, WS_VISIBLE|WS_CHILD|SS_WHITERECT);
            hipStatic1 = nsCreateControl(hWnd, ID_INPUT_STATIC1, "STATIC", "", (int)(10*coeff), (int)(15*coeff), (int)(370*coeff), (int)(55*coeff), 0, WS_VISIBLE|WS_CHILD);
            hipButton1 = nsCreateControl(hWnd, ID_INPUT_BUTTON1, "BUTTON", "", (int)(220*coeff), (int)(90*coeff), (int)(80*coeff), (int)(23*coeff), 0, WS_VISIBLE|WS_CHILD|WS_TABSTOP);
            hipButton2 = nsCreateControl(hWnd, ID_INPUT_BUTTON2, "BUTTON", "", (int)(310*coeff), (int)(90*coeff), (int)(80*coeff), (int)(23*coeff), 0, WS_VISIBLE|WS_CHILD|WS_TABSTOP);
            SetWindowText(hWnd, captionText);
            SetWindowText(hipStatic1, bodyText1);
            SetWindowText(hipButton1, buttonText1);
            SetWindowText(hipButton2, buttonText2);
            break;
        }
    }
    return FALSE;
}


int nsCreateDialog(HINSTANCE hInst, HWND hwnd, DLGPROC dlgproc)
{
	HGLOBAL hgbl;
	LPDLGTEMPLATE lpdt;
	LPWORD lpw;
	LPWSTR lpwsz;

	int dem = GetDPIDimension();
	/*
	CHAR buff[100];
	itoa(dem,buff,10);
	MessageBox(NULL,buff,"s",MB_OK);
	*/
	float coeff = 1;
	if (dem == 144)
	{
		coeff = (float)1.2;		
	}
	// +++ Creación del diálogo +++
	hgbl = GlobalAlloc(GMEM_ZEROINIT, 1024);
	lpdt = (LPDLGTEMPLATE)GlobalLock(hgbl);
	lpdt->cdit = 0;
	lpdt->cx = (int)(coeff*200); 
	lpdt->cy = (int)(coeff*60);
	lpdt->style = WS_POPUP|WS_BORDER|DS_CENTER|DS_MODALFRAME|WS_CAPTION|WS_SYSMENU;
	lpdt->x = 0; lpdt->y = 0;
	lpw = (LPWORD) (lpdt + 1);
	*lpw++ = 0; *lpw++ = 0; lpwsz = (LPWSTR) lpw;
	// +++ Fin del diálogo +++

	GlobalUnlock(hgbl);
	
	DWORD ThreadId;
	hTimedOutThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)nsMessageBoxTimedOutThread, (LPVOID)hwnd, 0, &ThreadId);
	int ret = DialogBoxIndirect(hInst, (LPDLGTEMPLATE)hgbl, hwnd, (DLGPROC)dlgproc);
	GlobalFree(hgbl); 
	return ret;
}


Function(nsMessageBox) 
{
    EXDLL_INIT();
    {
        popstring(captionText);
        popstring(bodyText1);
        popstring(buttonText1);
        popstring(buttonText2);
        popstring(lpszTimeOut);
        popstring(ipret);
        nsCreateDialog(g_hInstance, hwnd, (DLGPROC)nsDialogProc);
    }
}

Function(GetGUIDimension)
{
    EXDLL_INIT();
    {
        int result = GetDPIDimension();
        char buff[20] = {0};
        _itoa_s((int)result,buff,20,10);
        setuservariable(0, buff);
    }

}

Function (ConvertDPoint)
{
    EXDLL_INIT();
    {
        LONG pp = GetDialogBaseUnits();

        char var[MAX_PATH];

        popstring(var);
        HWND hwnd = (HWND)atoi(var);
        popstring(var);
        DWORD pos1 = atoi(var);
        popstring(var);
        DWORD pos2 = atoi(var);
        popstring(var);
        DWORD pos3 = atoi(var);
        popstring(var);
        DWORD pos4 = atoi(var);

        RECT rt;
        rt.left = pos1;
        rt.top = pos2;
        rt.right = pos3;
        rt.bottom = pos4;
        MapDialogRect(hwnd,&rt);
        _itoa_s((int)rt.left,var,1024,10);
        setuservariable(0,var);
        _itoa_s((int)rt.top,var,1024,10);
        setuservariable(1,var);
        _itoa_s((int)rt.right,var,1024,10);
        setuservariable(2,var);
        _itoa_s((int)rt.bottom,var,1024,10);
        setuservariable(3,var);
    }
}