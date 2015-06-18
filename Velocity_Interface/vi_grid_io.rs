*T CDLG
,128
*N VI_OUT_GRID
*S PR 0 PU 1
45 50 190 450
Output Grid name
5

*I VI_OUT_GRID_TITLE
StatText Enabled
20 85 40 350
Enter Output Grid Name Prefix:

*I VI_OUT_EDITTEXT
EditText Enabled
50 30 85 360

*I VI_OUT_GRID_OK
BtnItem Enabled
100 275 126 345 
O K

*I VI_OUT_GRID_CANCEL
BtnItem Enabled
100 35 126 105 
Cancel

*I VI_OUT_GRID_LIST
BtnItem Enabled
100 155 126 225 
List

,129
*N VI_IN_GRID
*S PR 0 PU 1
45 50 190 450
Input Grid name
5

*I VI_IN_GRID_TITLE
StatText Enabled
20 90 40 350
Enter Input Grid Name Prefix:

*I VI_IN_EDITTEXT
EditText Enabled
50 30 85 360

*I VI_IN_GRID_OK
BtnItem Enabled
100 275 126 345 
O K

*I VI_IN_GRID_CANCEL
BtnItem Enabled
100 35 126 105 
Cancel

*I VI_IN_GRID_LIST
BtnItem Enabled
100 155 126 225 
List

,130
*N VI_LIST_GRID
*S PR 0 PU 1
45 50 350 500
Existing Grids List   
4

*I VI_LIST_GRID_TITLE
StatText Enabled
0 150 30 350
Existing Grids

*I VI_LIST_GRID_OK
BtnItem Enabled
264 365 290 435 
O K

Setgroup 1
*I VI_LIST_SELECTOR
Selector Enabled
40 15 250 410

*I VI_LIST_VSCROLL
Vscroll Enabled
41 412 251 422

,131
*N VI_GRID_W_MSG
*S PR 0 PU 1
45 50 190 450
Msg box for existing grids
3

*I VI_MSG_W_TITLE
StatText Enabled
50 30 70 380
One or more grids already exist!  OK to overwrite?

*I VI_MSG_W_OK
BtnItem Enabled
100 275 126 345 
O K

*I VI_MSG_W_CANCEL
BtnItem Enabled
100 35 126 105 
Cancel

,132
*N VI_GRID_P_MSG
*S PR 0 PU 1
45 50 190 450
Msg box for non_existing grids
3

*I VI_MSG_P_TITLE
StatText Enabled
50 30 70 380
One or more input grids do not exist!  OK to proceed?

*I VI_MSG_P_OK
BtnItem Enabled
100 275 126 345 
O K

*I VI_MSG_P_CANCEL
BtnItem Enabled
100 35 126 105 
Cancel

