*T CDLG
,128
*N VI_SM_GRID
*S PR 0 PU 1
45 50 300 450
Smooth Velocity Grid
10

*I VI_SM_TITLE
StatText Enabled
10 110 40 300
Smooth Velocity Grid

*I VI_SM_SOURCE
StatText Enabled
45 35 80 200
Input Grid Source	

*I VI_SM_ARG
StatText Enabled
45 230 80 400
Smoothing Algorithm

*I VI_SM_OK
BtnItem Enabled
210 270 236 340 
O K

*I VI_SM_CANCEL
BtnItem Enabled
210 40 236 100 
Cancel

Setgroup 1
*I VI_SM_SELECTOR
Selector Enabled
95 30 200 180

*I VI_SM_VSCROLL
Vscroll Enabled
96 182 201 192

Setgroup 2
*I VI_SM_W1
RadioItem Enabled
100 240 120 380
Weighted 1

*I VI_SM_W2
RadioItem Enabled
130 240 150 380
Weighted 2

*I VI_SM_MEDIAN
RadioItem Enabled
160 240 180 380
Median

