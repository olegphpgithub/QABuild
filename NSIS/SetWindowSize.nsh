Function SetWindowSize
	
		# ������� � ����
	GetDlgItem $R0 $HWNDPARENT 1028 ;
 ; ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1034 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1035 ;
 ; ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

  GetDlgItem $R0 $HWNDPARENT 1036 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1037 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1038 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	#1039 - ������ � ������ ���� 
	GetDlgItem $R0 $HWNDPARENT 1039 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 
	
	GetDlgItem $R0 $HWNDPARENT 1041 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 
	
	GetDlgItem $R0 $HWNDPARENT 1043 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1045 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1201 ;
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 

  ;  GetDlgItem $MUI_TEMP1 $HWNDPARENT 1256
  ;  ShowWindow $MUI_TEMP1 ${SW_HIDE}

	
	GetDlgItem $R0 $HWNDPARENT 0 ;������ ������ "" �������� 
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 1 ;������ ������ "Next" �������� 
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 2 ;������ ������ "Cansel" �������� 
  ShowWindow $R0 0  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 3 ;������ ������ "Back" �������� 
 ;
	/**/
FunctionEnd

;	������� ������� �������������� gui ����� ������������ ����������
Function	RestoreGui
	GetDlgItem $R0 $HWNDPARENT 103 ;
  ShowWindow $R0 1  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 1034 ;
  ShowWindow $R0 1  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 1036 ;
  ShowWindow $R0 1  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 1037 ;
  ShowWindow $R0 1  ; 1-��������, 0 - �� �������� 
	GetDlgItem $R0 $HWNDPARENT 1038 ;
  ShowWindow $R0 1  ; 1-��������, 0 - �� �������� 

	GetDlgItem $R0 $HWNDPARENT 1035
	ShowWindow $R0 ${SW_SHOW}
	
	GetDlgItem $R0 $HWNDPARENT 1216
	ShowWindow $R0 ${SW_SHOW}
	
;	GetDlgItem $R0 $HWNDPARENT 1028
;	ShowWindow $R0 ${SW_SHOW}
	
	GetDlgItem $R0 $HWNDPARENT 1045
;	ShowWindow $R0 ${SW_SHOW}
	


	
FunctionEnd
