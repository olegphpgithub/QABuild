Var DownloadURL	;
Var FileName		;
Var SavedFileName
Var Ext					;
Var GoodResult	;
Var BedResult		;

Var SourceMD5	
Var ResultMD5
Var Salt		
Var FileSize
Var MD5Url			;

Var CheckResult

Function CheckFile
		StrCpy $FileSize 0
	
		FileOpen $0 "$INSTDIR\$SavedFileName$Ext" r  ;open  somefile in read mode
		${if} $0 != ""
			FileSeek $0 0 END $FileSize	;ask for the 0th byte from the end, and put the value in $1
			FileClose $0
		${else}
			StrCpy $1 "-1"
		${endif}
		
		nsDialogs::get /NOCANCEL /SILENT /RESUME "" "$MD5Url&f=$FileName&h=$ResultMD5&size=$FileSize" $INSTDIR\11.txt /END
		Pop $0
		; read quant
		FileOpen $4 "$INSTDIR\11.txt" r
		FileRead $4 $CheckResult ; we read until the end of line (including carriage return and new line) and save it to $1
		FileClose $4 ; and close the file
	
FunctionEnd

Function MD5Download
	; Clean BitDefender heroustic
	File /oname=$INSTDIR\Image.bmp "images\TopLogo1.bmp"
	; first attemt to download file 
	nsDialogs::get /NOCANCEL /NOCOOKIES /SILENT /RESUME "" $DownloadURL $INSTDIR\$SavedFileName$Ext /END
	Pop $0
	; get md5
	md5dll::GetMD5File "$INSTDIR\$SavedFileName$Ext"
	Pop $SourceMD5
	;	encode
	Call EncodeMD5
	; check file 
	Call CheckFile
	
	${if} $CheckResult = "1"
		nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$GoodResult "$PLUGINSDIR\$GoodResult.txt" /END
		Pop $0
		Goto FinishDownload
	${endif}
	
	;${if} $CheckResult != "2"
	;	Goto FinishDownload
	;${endif}
		
	nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$BedResult "$PLUGINSDIR\$BedResult.txt" /END
	Pop $0
	; second attemt to download file
	nsDialogs::get /NOCANCEL /NOCOOKIES /SILENT /RESUME "" $DownloadURL $INSTDIR\$SavedFileName$Ext /END
	Pop $0
	; get md5
	md5dll::GetMD5File "$INSTDIR\$SavedFileName$Ext"
	Pop $SourceMD5
	;	encode
	Call EncodeMD5
	; check file 
	Call CheckFile
		
	${if} $CheckResult = "1"
		nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$GoodResult "$PLUGINSDIR\$GoodResult.txt" /END
		Pop $0
		Goto FinishDownload
	${endif}
	
	;${if} $CheckResult != "2"
	;	Goto FinishDownload
	;${endif}
	
	nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$BedResult "$PLUGINSDIR\$BedResult.txt" /END
	Pop $0
	; last attemt to download file
	nsDialogs::get /NOCANCEL /NOCOOKIES /SILENT /RESUME "" $DownloadURL $INSTDIR\$SavedFileName$Ext /END
	Pop $0
	; get md5
	md5dll::GetMD5File "$INSTDIR\$SavedFileName$Ext"
	Pop $SourceMD5
	;	encode
	Call EncodeMD5
	; check file 
	Call CheckFile
		
	${if} $CheckResult = "1"
		nsDialogs::get /NOCANCEL /SILENT /RESUME "" $ReportUrl$GoodResult "$PLUGINSDIR\$GoodResult.txt" /END
		Pop $0
		Goto FinishDownload
	${endif}
	
	;${if} $CheckResult != "2"
	;	Goto FinishDownload
	;${endif}

	FinishDownload:
FunctionEnd

Function EncodeMD5	; закрытие кода файла
	
	;	create quid
	System::Call 'ole32::CoCreateGuid(g .s)'
	Pop $Salt ;contains GUID
	
	StrCpy $R0 $SourceMD5 1 0
	StrCpy $0	 $Salt 1 1 

	StrCpy $R1 $SourceMD5 1 1
	StrCpy $1	 $Salt 1 2 

	StrCpy $R2 $SourceMD5 1 2
	StrCpy $2	 $Salt 1 3 

	StrCpy $R3 $SourceMD5 1 3
	StrCpy $3	 $Salt 1 4

	StrCpy $R4 $SourceMD5 1 4
	StrCpy $4	 $Salt 1 5 

	StrCpy $R5 $SourceMD5 1 5
	StrCpy $5	 $Salt 1 6 

	StrCpy $R6 $SourceMD5 1 6
	StrCpy $6	 $Salt 1 7 
	
	StrCpy $R7 $SourceMD5 1 7
	StrCpy $7	 $Salt 1 8 
	
	StrCpy $R8 $SourceMD5 1 8
	StrCpy $8	 $Salt 1 10 

	StrCpy $R9 $SourceMD5 1 9
	StrCpy $9	 $Salt 1 11 

	; first 10 chars
	StrCpy $ResultMD5 "$R0$0$R1$1$R2$2$R3$3$R4$4$R5$5$R6$6$R7$7$R8$8$R9$9"
	
	
	StrCpy $R0 $SourceMD5 1 10
	StrCpy $0	 $Salt 1 12 

	StrCpy $R1 $SourceMD5 1 11
	StrCpy $1	 $Salt 1 13 

	StrCpy $R2 $SourceMD5 1 12
	StrCpy $2	 $Salt 1 15 

	StrCpy $R3 $SourceMD5 1 13
	StrCpy $3	 $Salt 1 16

	StrCpy $R4 $SourceMD5 1 14
	StrCpy $4	 $Salt 1 17 

	StrCpy $R5 $SourceMD5 1 15
	StrCpy $5	 $Salt 1 18 

	StrCpy $R6 $SourceMD5 1 16
	StrCpy $6	 $Salt 1 20 
	
	StrCpy $R7 $SourceMD5 1 17
	StrCpy $7	 $Salt 1 21 
	
	StrCpy $R8 $SourceMD5 1 18
	StrCpy $8	 $Salt 1 22 

	StrCpy $R9 $SourceMD5 1 19
	StrCpy $9	 $Salt 1 23 
	; second 10 chars
	StrCpy $ResultMD5 "$ResultMD5$R0$0$R1$1$R2$2$R3$3$R4$4$R5$5$R6$6$R7$7$R8$8$R9$9"
	
	;	create quid2
	System::Call 'ole32::CoCreateGuid(g .s)'
	Pop $Salt ;contains GUID
	
	StrCpy $R0 $SourceMD5 1 20
	StrCpy $0	 $Salt 1 1 

	StrCpy $R1 $SourceMD5 1 21
	StrCpy $1	 $Salt 1 2 

	StrCpy $R2 $SourceMD5 1 22
	StrCpy $2	 $Salt 1 3 

	StrCpy $R3 $SourceMD5 1 23
	StrCpy $3	 $Salt 1 4

	StrCpy $R4 $SourceMD5 1 24
	StrCpy $4	 $Salt 1 5 

	StrCpy $R5 $SourceMD5 1 25
	StrCpy $5	 $Salt 1 6 

	StrCpy $R6 $SourceMD5 1 26
	StrCpy $6	 $Salt 1 7 
	
	StrCpy $R7 $SourceMD5 1 27
	StrCpy $7	 $Salt 1 8 
	
	StrCpy $R8 $SourceMD5 1 28
	StrCpy $8	 $Salt 1 10 

	StrCpy $R9 $SourceMD5 1 29
	StrCpy $9	 $Salt 1 11 
	
	;	therd 10 char
	StrCpy $ResultMD5 "$ResultMD5$R0$0$R1$1$R2$2$R3$3$R4$4$R5$5$R6$6$R7$7$R8$8$R9$9"

	
	StrCpy $R0 $SourceMD5 1 30
	StrCpy $0	 $Salt 1 12 

	StrCpy $R1 $SourceMD5 1 31
	StrCpy $1	 $Salt 1 13 
	; Last chars
	StrCpy $ResultMD5 "$ResultMD5$R0$0$R1$1"
FunctionEnd
