*T CDLG
,501
*N WE_SHOW_WELL_SELECTION
*S PR 0 PU 1
47 18 270 490
Well Information
11

*I WE_SHOW_UWI_PROMPT
StatText Enabled
0 20 30 150
Well Identifier:

*I WE_SHOW_OPER_PROMPT
StatText Enabled
35 20 65 150
Operator:

*I WE_SHOW_NAME_PROMPT
StatText Enabled
70 20 100 150
Well Name:

*I WE_SHOW_NUMB_PROMPT
StatText Enabled
105 20 135 150
Number:

*I WE_SHOW_ELEV_PROMPT
StatText Enabled
140 20 170 150
Elevation:

*I WE_SHOW_UWI_FIELD
StatText Enabled
0 150 30 330
<Well_Id>

*I WE_SHOW_OPER_FIELD
StatText Enabled
35 150 65 455
<Well_Operator>

*I WE_SHOW_NAME_FIELD
StatText Enabled
70 150 100 450
<Well_Name>

*I WE_SHOW_NUMB_FIELD
StatText Enabled
105 150 135 450
<Well_Number>

*I WE_SHOW_ELEV_FIELD
StatText Enabled
140 150 170 450
<Well_Elevation>


*I WE_SHOW_OK
BtnItem Enabled
180 351 205 451
OK


*T CDLG
,502
*N WE_SHOW_WELL_BAD_SELECTION
*S PR 0 PU 1
47 18 200 375
Well Information
4

*I WE_SHOW_BAD_UWI_PROMPT
StatText Enabled
20 20 50 150
Requested Well:

*I WE_SHOW_BAT_OPER_PROMPT
StatText Enabled
60 20 90 150
NOT FOUND!!!

*I WE_SHOW_BAD_UWI_FIELD
StatText Enabled
20 150 50 330
<Well_Not_Found>

*I WE_SHOW_BAD_OK
BtnItem Enabled
110 250 135 330
OK


*T CDLG 
,503
*N WE_SELECT_UWI
*S PR 0 PU 1 
52 60 330 500
Current Wells
5

*I WE_SLT_PROMPT
StatText Enabled
7 27 47 400
Select a well and press Display Header for more information.

*I WE_SLT_BTN_CANCEL
BtnItem Enabled
240 330 265 415
Done

*I WE_SLT_BTN_INFO
BtnItem Enabled
117 320 145 425 
Display Header

SETGROUP 2
*I WE_SLT_SELECT_LIST
Selector Enabled
50 27 230 280
<Well_Select_List #1>
<Well_Select_List #2>
<Well_Select_List #3>
<Well_Select_List #4>
<Well_Select_List #5>
<Well_Select_List #6>
<Well_Select_List #7>
<Well_Select_List #8>
<Well_Select_List #9>

*I WE_SLT_SELECT_BAR
Vscroll Enabled
50 285 230 300 


