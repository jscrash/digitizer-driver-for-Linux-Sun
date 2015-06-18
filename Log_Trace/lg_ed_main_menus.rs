*T MENU
,100
*N LG_EDIT_MENU
*S PR 0 PU 1
LogEdit
4 D M
*I LG_EDIT_LINEAR
Transform: Slope & Intercept
*I LG_EDIT_LINEAR_MM
Transform: Minimum & Maximum
*I LG_EDIT_SPLICE
Splice Curves
*I LG_EDIT_QUIT
Quit Log Editor/Q



*T MENU
,200
*N LG_DB_MENU
*S PR 0 PU 1
LogDB
4 D M
*I LG_DB_MENU_SHOW
Show Traces
*I LG_DB_MENU_RENAME
Rename Trace
*I LG_DB_MENU_COPY
Copy Trace
*I LG_DB_MENU_DELETE
Delete Traces



*T MENU
,300
*N LG_WELL_MENU
*S PR 0 PU 1
Well
1 D M
*I LG_WELL_ENTER_UWI
Select a UWI



*T CDLG
,301
*N LG_ED_MAIN_CDLG
*S PR 0 PU 1
100 100 250 400 
Log Editor
3

*I LG_ED_MAIN_MSG_1
StatText Enabled
10 20 26 280 
Select a well for editing.

*I LG_ED_MAIN_PROMPT_1
StatText Enabled
50 20 80 110 
Current UWI:

*I LG_ED_MAIN_UWI
StatText Enabled
50 112 80 280 
<--->
