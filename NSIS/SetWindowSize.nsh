Function SetWindowSize
	
		# надпись в низу
	GetDlgItem $R0 $HWNDPARENT 1028 ;
 ; ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1034 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1035 ;
 ; ShowWindow $R0 0  ; 1-активной, 0 - не активной 

  GetDlgItem $R0 $HWNDPARENT 1036 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1037 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1038 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	#1039 - иконка в правом углу 
	GetDlgItem $R0 $HWNDPARENT 1039 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 
	
	GetDlgItem $R0 $HWNDPARENT 1041 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 
	
	GetDlgItem $R0 $HWNDPARENT 1043 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1045 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1201 ;
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 

  ;  GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
  ;  ShowWindow $MUI_TEMP1 ${SW_HIDE}

	
	GetDlgItem $R0 $HWNDPARENT 0 ;делаем кнопку "" активной 
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 1 ;делаем кнопку "Next" активной 
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 2 ;делаем кнопку "Cansel" активной 
  ShowWindow $R0 0  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 3 ;делаем кнопку "Back" активной 
 ;
	/**/
FunctionEnd

;	функция которая востанавливает gui после продвинутого интерфейса
Function	RestoreGui
	GetDlgItem $R0 $HWNDPARENT 103 ;
  ShowWindow $R0 1  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 1034 ;
  ShowWindow $R0 1  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 1036 ;
  ShowWindow $R0 1  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 1037 ;
  ShowWindow $R0 1  ; 1-активной, 0 - не активной 
	GetDlgItem $R0 $HWNDPARENT 1038 ;
  ShowWindow $R0 1  ; 1-активной, 0 - не активной 

	GetDlgItem $R0 $HWNDPARENT 1035
	ShowWindow $R0 ${SW_SHOW}
	
	GetDlgItem $R0 $HWNDPARENT 1216
	ShowWindow $R0 ${SW_SHOW}
	
;	GetDlgItem $R0 $HWNDPARENT 1028
;	ShowWindow $R0 ${SW_SHOW}
	
	GetDlgItem $R0 $HWNDPARENT 1045
;	ShowWindow $R0 ${SW_SHOW}
	


	
FunctionEnd
