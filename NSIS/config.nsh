!define GetParamString "!insertmacro read_ini_string "

!macro read_ini_string user_var ini_filename section_name entry_name

	Push ${entry_name}
	Push ${section_name}
	Push ${ini_filename}
	
	 CallInstDLL $DllPath\${guard}.dll ${ReadCryptedParam} 
	Pop ${user_var} 
	; develop version - real ini file
;	ReadINIStr ${user_var} ${ini_filename} ${section_name} ${entry_name}
!macroend

!macro save_ini_string user_var ini_filename section_name entry_name path_name

	Push ${path_name}
	Push ${entry_name}
	Push ${section_name}
	Push ${ini_filename}
	CallInstDLL $DllPath\${guard}.dll ${SaveCryptedParamToFile} 
!macroend
