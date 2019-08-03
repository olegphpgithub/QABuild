Var UID
Var ID2

Function NewAction1Function
	# ������ ������� �������� ��� �����
	GetTempFileName $R0 
	;get first quant 
	StrCpy $0 "1"
	nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$0  $R0 /END
	Pop $0 
	${if} $0 != "OK"
	
		# ������ ������� �������� ��� �����
		GetTempFileName $R0 
		;get first quant 
		StrCpy $0 "1"
		nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$0  $R0 /END
		Pop $0 
		
		${if} $0 != "OK"
			
			# ������ ������� �������� ��� �����
			GetTempFileName $R0 
			;get first quant 
			StrCpy $0 "1"
			nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$0  $R0 /END
			Pop $0
			
			${if} $0 != "OK"
			
				# ������ ������� �������� ��� �����
				StrCpy $R0 "$PLUGINSDIR\quid.txt"
				;get first quant 
				StrCpy $0 "1"
				nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$0  $R0 /END
				Pop $0
			${endif}
		${endif}
	${endif}
FunctionEnd

Var IPDUrl
; ��������� quant
Function GenerateQuant	

	StrCpy $MainDomain "freezeprogs.com"

	;	create quid
	System::Call 'ole32::CoCreateGuid(g .s)'
	Pop $0 ;contains GUID
	StrCpy $1 $0 36 1
	StrCpy $UID $1
	StrCpy $ReportUrl "https://$MainDomain/installer.php?CODE=PUTGQ&UID=$1&action="
	
	
	StrCpy $quant "0"
	;	creare url with quant and guid
	StrCpy $ReportUrl "https://$MainDomain/installer.php?CODE=PUTGQ&UID=$uid&quant=$quant&action="
	StrCpy $MD5Url    "https://$MainDomain/info.php?&quant=$quant"
		
	# IPD.php encripted version -- start
	;create new random 12 character string  
	System::Call 'ole32::CoCreateGuid(g .s)'
	Pop $0 ;contains GUID
	StrCpy $1 $0 8 1
	StrCpy $2 $0 4 10
	
	StrCpy $ID2 "$1$2"
	
	System::Call 'ole32::CoCreateGuid(g .s)'
	Pop $0 ;contains GUID
	StrCpy $1 $0 8 1
	StrCpy $2 $0 4 10
	
	

FunctionEnd

