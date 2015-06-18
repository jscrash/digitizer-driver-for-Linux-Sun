*T CDLG
,128
*N VI_COMP_AVG
*S PR 0 PU 1
45 50 330 450
Compute Average Velocity
10

*I VI_CA_TITLE1
StatText Enabled
15 100 45 300
Transform Velocity Analysis

*I VI_CA_TITLE2
StatText Enabled
50 100 80 300
(Compute Average Velocity)

*I VI_INP_TYPE
StatText Enabled
75 30 105 200
Input Velocity Type

*I VI_INP_SOURCE
StatText Enabled
75 195 105 400
Input Velocity Source

*I VI_CA_OK
BtnItem Enabled
220 305 246 375 
O K

*I VI_CA_CANCEL
BtnItem Enabled
220 30 246 100 
Cancel

Setgroup 1
*I VI_STK
RadioItem Enabled
120 50 140 180
Stacking

*I VI_INT
RadioItem Enabled
150 50 170 180
Interval

Setgroup 2
*I VI_CA_SELECTOR 
Selector Enabled
115 185 205 345

*I VI_CA_VSCROLL
Vscroll Enabled
115 347 205 357


