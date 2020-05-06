Var CustomLabel1
Var Custom1
Var Custom2

Var InstallBrowserSafer
Var ShowBrowserSaferLastChance
Var CustomLabel2
Var EditCtrl
Var IronTestAppFound
Var InstallIronTestApp
Var SSOFileName
Var BSFileName
Var ShowFDLastChance
Var SkipGP

Var SkipKNCTR
Var ShowLastChanceKNCTR


Function onClickPrivacy
	Pop $0						
	ExecShell "Open" "https://$MainDomain/redirect.php?id=22"
FunctionEnd

Function onClickTerms
	Pop $0						
	ExecShell "Open" "https://$MainDomain/redirect.php?id=23"
FunctionEnd


Function OnNextClick
	# эмулируем нажатие кнопки next
	GetDlgItem $R0 $HWNDPARENT 1 ;делаем кнопку "Next" активной 
  SendMessage $R0 ${WM_LBUTTONDOWN} 0 0
	SendMessage $R0 ${WM_LBUTTONUP} 0 0
FunctionEnd

Function OnCancelClick
	# эмулируем нажатие кнопки next
	GetDlgItem $R0 $HWNDPARENT 2 ;делаем кнопку "Next" активной 
  SendMessage $R0 ${WM_LBUTTONDOWN} 0 0
	SendMessage $R0 ${WM_LBUTTONUP} 0 0

FunctionEnd


!macro TextNormal9
	Pop $ctrl
	SetCtlColors $Ctrl 0x646464 transparent
	SendMessage $ctrl ${WM_SETFONT} $FONT9n 0
!macroend

!macro TextLinkNormal9
	Pop $Ctrl
	SendMessage $Ctrl ${WM_SETFONT} $FONT9URL2 0
	SetCtlColors $Ctrl 0x5793d9 transparent
!macroend



Function fnPrimeUpdaterInit 
	
	GetDlgItem $R0 $HWNDPARENT 3 
	ShowWindow $R0 0
	
	SetBrandingImage Image.bmp
	Call SetWindowSize
	
		
	
  System::Call 'user32::GetWindowLong(i $hwndparent,i -16)i.r0'
	intop $1 0x30000 ~
	intop $0 $0 & $1
	System::Call 'user32::SetWindowLong(i $hwndparent,i -16,i $0)i'
			
	# get HWND
	StrCpy $0 $HWNDPARENT
	# set top most
	System::Call "user32::SetWindowPos(i r0, i -1, i 0, i 0, i 0, i 0, i 3)"
		

		# get HWND
		StrCpy $0 $HWNDPARENT
		# set top most
		System::Call "user32::SetWindowPos(i r0, i -1, i 0, i 0, i 0, i 0, i 3)"
		
		# create dialog
		nsDialogs::Create 1044
		Pop $DIALOG
		# set dialog Size
		System::Call "User32::SetWindowPos(i, i, i, i, i, i, i) b ($DIALOG, 0, 0, 0, 730, 575, 0)"
	
		${NSD_CreateLabel}  10u 40u 310u 20u "Please read the  follow  licence  Agreement. You must  accept  the terms  of this agreement before continue with installation  "
		Pop $ctrl
		SetCtlColors $Ctrl 0x646464 transparent
		SendMessage $ctrl ${WM_SETFONT} $FONT10n 0

		${NSD_CreateLabel}  10u 170u 224u 8u 'By clicking "Accept" you confirm that you have read and agree to the'
		Pop $ctrl
		SetCtlColors $Ctrl 0x646464 transparent
		SendMessage $ctrl ${WM_SETFONT} $FONT8n 0
	
	
		${NSD_CreateLink} 234u 170u 19u 8u "EULA"
		Pop $Ctrl
		SendMessage $Ctrl ${WM_SETFONT} $FONT8URL2 0
		SetCtlColors $Ctrl 0x5793d9 transparent
		${NSD_OnClick} $Ctrl onClickTerms
		
		${NSD_CreateLabel}  254u 170u 15u 8u "and"
		Pop $ctrl
		SetCtlColors $Ctrl 0x646464 transparent
		SendMessage $ctrl ${WM_SETFONT} $FONT8n 0
		
		# link	
		${NSD_CreateLink} 268u 170u 48u 8u "Privacy Policy"
		Pop $Ctrl
		SendMessage $Ctrl ${WM_SETFONT} $FONT8URL2 0
		SetCtlColors $Ctrl 0x5793d9 transparent
		${NSD_OnClick} $Ctrl onClickPrivacy
		
		${NSD_CreateLabel}  10u 178u 200u 8u "and to install your choice above."
		Pop $ctrl
		SetCtlColors $Ctrl 0x646464 transparent
		SendMessage $ctrl ${WM_SETFONT} $FONT8n 0
		
		File eula3.rtf
	
		nsDialogs::CreateControl "RICHEDIT50W" ${ES_READONLY}|${WS_VISIBLE}|${WS_CHILD}|${WS_TABSTOP}|${WS_VSCROLL}|${ES_MULTILINE}|${ES_WANTRETURN} ${WS_EX_STATICEDGE} 10u 62u 310u 105u ''
		Pop $Ctrl
		${if} $Ctrl == 0 
			nsDialogs::CreateControl "RichEdit20A" ${ES_READONLY}|${WS_VISIBLE}|${WS_CHILD}|${WS_TABSTOP}|${WS_VSCROLL}|${ES_MULTILINE}|${ES_WANTRETURN} ${WS_EX_STATICEDGE} 10u 62u 310u 105u ''
			Pop $Ctrl
		${endif}

		Push $PLUGINSDIR\eula3.rtf
		Push $ctrl
		Call LoadRTF
	
		System::Call user32::GetSysColor(i15)i.r1
		SendMessage $Ctrl 0x0443 0 $1
		
		${NSD_CreateButton}  216u 201u 100u 14u "Accept && Install"
		Pop $Ctrl
		SendMessage $Ctrl ${WM_SETFONT} $FONT10n 0
		${NSD_OnClick} $Ctrl OnNextClick
		
		${NSD_CreateButton}  20u 201u 50u 14u "Cancel"
		Pop $Ctrl
		SendMessage $Ctrl ${WM_SETFONT} $FONT10n 0
		${NSD_OnClick} $Ctrl OnCancelClick
		# get HWND
		StrCpy $0 $HWNDPARENT
		# return normal state
		System::Call "user32::SetWindowPos(i r0, i -2, i 0, i 0, i 0, i 0, i 3)"
		
		
		nsDialogs::Show


FunctionEnd

Function fnPrimeUpdaterDestroy
	StrCpy $InstallPrimeUpdater "ok"
FunctionEnd
