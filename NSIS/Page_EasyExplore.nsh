Var InstallNTT
Var SkipNTT
Var ShowNTTLastChance


Function OnNextClickNTT
	# эмулируем нажатие кнопки next
	GetDlgItem $R0 $HWNDPARENT 1 ;делаем кнопку "Next" активной 
  SendMessage $R0 ${WM_LBUTTONDOWN} 0 0
	SendMessage $R0 ${WM_LBUTTONUP} 0 0	
FunctionEnd

Function OnCancelClickNTT
	
			StrCpy $InstallNTT "false"
			# эмулируем нажатие кнопки next
			GetDlgItem $R0 $HWNDPARENT 1 ;делаем кнопку "Next" активной 
			SendMessage $R0 ${WM_LBUTTONDOWN} 0 0
			SendMessage $R0 ${WM_LBUTTONUP} 0 0
	
FunctionEnd



Function onClickEULANTT
	ExecShell "Open" "https://$MainDomain/redirect.php?id=18"
FunctionEnd

Function onClickPrivacyNTT
	ExecShell "Open" "https://$MainDomain/redirect.php?id=19"
FunctionEnd

Function CheckRegKeyHKLMUniversal
	Pop $R0
	StrCpy $0 0
  ClearErrors
  EnumRegKey $1 HKLM $R0 $0
	
  IfErrors done 
		StrCpy $KeyFound "true"
		goto end
	done:
		goto end
	end:
FunctionEnd


Function CheckNTTInstall
	
	;${if} $SkipNTT == "true"
	;	StrCpy $Result "1732"	
	;	Goto end
	;${endif}
	
	StrCpy $Result ""
	
	StrCpy $KeyFound ""
	
	SetRegView 64
	
	; HKCU\Software\Avira
	Push "Software\Avira"
	Call CheckRegKeyHKCUUniversal
		
	;HKLM\SOFTWARE\Avira\AntiVir 
	Push "SOFTWARE\Avira\AntiVir"
	Call CheckRegKeyHKLMUniversal
	${if} $KeyFound != ""
		StrCpy $SkipNTT "true"
	${endif}
	

	;HKLM\SOFTWARE\AVAST Software 	
;	Push "SOFTWARE\AVAST Software"
;	Call CheckRegKeyHKLMUniversal
	;HKLM\SOFTWARE\AVAST 
;	Push "SOFTWARE\AVAST"
;	Call CheckRegKeyHKLMUniversal
	;HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\avast
;	Push "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\avast"
;	Call CheckRegKeyHKLMUniversal
	${if} $KeyFound != ""
		StrCpy $SkipNTT "true"
	${endif}
	
	SetRegView 32
	
	; HKCU\Software\Avira
	Push "Software\Avira"
	Call CheckRegKeyHKCUUniversal
	
	
	;HKLM\SOFTWARE\Avira\AntiVir 
	Push "SOFTWARE\Avira\AntiVir"
	Call CheckRegKeyHKLMUniversal
	${if} $KeyFound != ""
		StrCpy $SkipNTT "true"
	${endif}
	
	;HKLM\SOFTWARE\AVAST 
;	Push "SOFTWARE\AVAST"
;	Call CheckRegKeyHKLMUniversal

	;HKLM\SOFTWARE\AVAST Software 	
;	Push "SOFTWARE\AVAST Software"
;	Call CheckRegKeyHKLMUniversal
	
	;HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\avast
;	Push "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\avast"
;	Call CheckRegKeyHKLMUniversal
	${if} $KeyFound != ""
		StrCpy $SkipNTT "true"
	${endif}
	
	${if} $KeyFound == "true"
		StrCpy $SkipNTT "true"
	${endif}

	;MessageBox MB_OK "$APPDATA\AGData\config.json"
	IfFileExists "$APPDATA\AGData\config.json" 0 end
		StrCpy $SkipNTT "true"
	end:

		
FunctionEnd	

Function PostInstallCheckNTT

	IfFileExists "$APPDATA\AGData\add.json" end 0
		
		Nop
	end:
	
FunctionEnd

Function fnNTTInit 
	;	установка размера окна
	Call SetWindowSize
	
	;	пр€чем кнопку 
	GetDlgItem $R0 $HWNDPARENT 3 
	ShowWindow $R0 0

	# сработает если у нас найден ключ в реестре.
	Call CheckNTTInstall
	
	StrCmp $SkipNTT  "true" breakNTTPage 0
	
	StrCpy $InstallNTT "true"
	# get HWND
	StrCpy $0 $HWNDPARENT
	# set top most
	System::Call "user32::SetWindowPos(i r0, i -1, i 0, i 0, i 0, i 0, i 3)"
	
	
	SetOutPath $PLUGINSDIR
	;	установим топовую картинку.
	SetBrandingImage Image.bmp
		
	# create dialog
	nsDialogs::Create 1044
	Pop $DIALOG
	# set dialog Size
	System::Call "User32::SetWindowPos(i, i, i, i, i, i, i) b ($DIALOG, 0, 0, 0, 730, 575, 0)"
	
	${NSD_CreateLabel}  10u 43u 300u 20u 'EasyXplore'
	Pop $Ctrl
	SetCtlColors $Ctrl 0x646464 transparent
	SendMessage $Ctrl ${WM_SETFONT} $FONT12 0

	
	${NSD_CreateLabel}  10u 70u 300u 10u 'Explore the web from your desktop.'
	Pop $Ctrl
	SendMessage $Ctrl ${WM_SETFONT} $FONT10 0
	SetCtlColors $Ctrl 0x646464 transparent
	
	${NSD_CreateLabel}  10u 80u 300u 10u 'All you need seamlessly in front of your eyes.'
	Pop $Ctrl
	SendMessage $Ctrl ${WM_SETFONT} $FONT10n 0
	SetCtlColors $Ctrl 0x646464 transparent
	
	
	#кнопка отменить
	${NSD_CreateButton}  264u 201u 50u 14u "Accept >"
	Pop $Ctrl
	${NSD_AddStyle} $Ctrl "${BS_ICON}" 		
	System::Call 'user32::LoadImage(i 0, t "$PLUGINSDIR\accept.ico", i 1, i 60, i 30, i 80) i.s'
	Pop $6
	SendMessage $Ctrl ${BM_SETIMAGE} 1 $6 

	${NSD_OnClick} $Ctrl OnNextClickNTT
	#кнопка отменить
	${NSD_CreateButton}  20u 201u 50u 14u "DecIine"
	Pop $Ctrl
	${NSD_AddStyle} $Ctrl "${BS_ICON}" 	
	System::Call 'user32::LoadImage(i 0, t "$PLUGINSDIR\decline.ico", i 1, i 60, i 30, i 80) i.s'
	Pop $6
	SendMessage $Ctrl ${BM_SETIMAGE} 1 $6 
	${NSD_OnClick} $Ctrl OnCancelClickNTT

	# get HWND
	StrCpy $0 $HWNDPARENT
	# return normal state
	System::Call "user32::SetWindowPos(i r0, i -2, i 0, i 0, i 0, i 0, i 3)"

	nsDialogs::Show
	

	/**/
	breakNTTPage:
FunctionEnd

Function fnNTTDestroy
	; ${if} $InstallNTT == "true"
		; ${NSD_GetState} $Checkbox1 $0
		; ${if} $0 == ${BST_CHECKED}	
		; ${else}
			; StrCpy $InstallNTT ""
		; ${endif}
	; ${endif}
FunctionEnd

 