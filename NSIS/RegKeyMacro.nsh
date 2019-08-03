
Var reg_key_found


Function CheckRegKey_HKLM 
	Pop $R0
	StrCpy $0 0
  ClearErrors
  EnumRegKey $1 HKLM $R0 $0
	
  IfErrors done 
		StrCpy $reg_key_found "true"
		goto end
	done:
		goto end
	end:
FunctionEnd


Function CheckRegKey_HKCU
	Pop $R0
	StrCpy $0 0
  ClearErrors
  EnumRegKey $1 HKCU $R0 $0
	
  IfErrors done 
		StrCpy $reg_key_found "true"
		goto end
	done:
		goto end
	end:
FunctionEnd

Function CheckRegKey_HKCR
	Pop $R0
	StrCpy $0 0
  ClearErrors
  EnumRegKey $1 HKCU $R0 $0
	
  IfErrors done 
		StrCpy $reg_key_found "true"
		goto end
	done:
		goto end
	end:
FunctionEnd


!macro Check_HKLM_key_exist _key
	SetRegView 32
	Push ${_key}
	Call CheckRegKey_HKLM
	
	SetRegView 64
	Push ${_key}
	Call CheckRegKey_HKLM

!macroend

!macro Check_HKCU_key_exist _key
	Push ${_key}
	Call CheckRegKey_HKCU
!macroend

!macro Check_HKCR_key_exist _key
	Push ${_key}
	Call CheckRegKey_HKCR
!macroend

Function CheckRegKeyHKCUUniversal
	Pop $R0
	StrCpy $0 0
  ClearErrors
  EnumRegKey $1 HKCU $R0 $0
	
  IfErrors done 
		StrCpy $KeyFound "true"
		goto end
	done:
		goto end
	end:
FunctionEnd

