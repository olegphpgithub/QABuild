Var DebugOSVersion
Var MajorVersion
Var MinorVersion
Function GetOsVersion

	ReadRegStr $R0 HKLM  "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
	ReadRegStr $DebugOSVersion HKLM  "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentBuild
	StrCpy $MajorVersion $R0 1
	StrCpy $MinorVersion $R0 1 2
	${if} $MajorVersion == "6"
		${if} $MinorVersion == "2"
			;8.0,8.1,10.x
			ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentVersion"
			${if} $0 == "6.3"
				;; 8.1 or 10.x
				StrCpy $MinorVersion "3"
				ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMajorVersionNumber"
				${if} $0 != ""
					ReadRegStr $MajorVersion HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMajorVersionNumber"
					ReadRegStr $MinorVersion HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMinorVersionNumber"
				${endif}
			${endif}
		${endif}
		${if} $MinorVersion == "3"
				ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMajorVersionNumber"
				${if} $0 != ""
					ReadRegStr $MajorVersion HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMajorVersionNumber"
					ReadRegStr $MinorVersion HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentMinorVersionNumber"
				${endif}
		${endif}
	${endif}

FunctionEnd