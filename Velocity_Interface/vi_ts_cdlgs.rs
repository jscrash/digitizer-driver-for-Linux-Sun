*T CDLG
,128
*N VI_TRANS_SURFACE
*S PR 0 PU 1
45 50 300 590
Compute Average Velocity
13

*I VI_TS_TITLE
StatText Enabled
0 160 30 450
Transform Surface Velocities

*I VI_TS_CA_TITLE
StatText Enabled
35 147 66 450
(Compute Depth/Average Velocity)

*I VI_TS_TIME_SOURCE
StatText Enabled
70 40 100 190
Input Time Source

*I VI_TS_INP_TYPE
StatText Enabled
70 215 100 330
Velocity Type

*I VI_TS_VEL_SOURCE
StatText Enabled
70 345 100 520
Input Velocity Source

*I VI_TS_OK
BtnItem Enabled
215 420 241 490 
O K

*I VI_TS_CANCEL
BtnItem Enabled
215 35 241 105 
Cancel

Setgroup 1
*I VI_TS_SELECTOR 
Selector Enabled
100 35 200 155 

*I VI_TS_VSCROLL
Vscroll Enabled
101 157 201 167 

Setgroup 2
*I VI_TS_STK 
RadioItem Enabled
105 220 125 330 
Stacking

*I VI_TS_INT
RadioItem Enabled
135 220 155 330 
Interval

Setgroup 3
*I VI_TS_SELECTOR2 
Selector Enabled
100 340 200 460

*I VI_TS_VSCROLL2
Vscroll Enabled
101 462 201 472

