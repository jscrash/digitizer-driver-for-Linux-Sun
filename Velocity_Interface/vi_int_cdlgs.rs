*T CDLG
,128
*N VI_INT_CDLG
*S PR 0 PU 1
45 50 350 550
Interpolate Lines from Grid
16

*I VI_INT_TITLE
StatText Enabled
0 130 35 480
Back Interpolate Lines from Grid

*I VI_INT_SOURCE
StatText Enabled
50 35 90 200
Input Grid Source

*I VI_INT_TYPE
StatText Enabled
60 265 90 380
Velocity Type

*I VI_INT_MODE
StatText Enabled
60 390 90 480
Mode

*I VI_INT_DES1
StatText Enabled
200 265 230 480
Data Decimation: Store every Nth

*I VI_INT_DES2
StatText Enabled
230 265 260 380
CDP where N =

*I VI_INT_EDITTEXT
EditText Enabled
VerifyInteger
230 390 260 460

*I VI_INT_OK
BtnItem Enabled
274 390 300 460
O K

*I VI_INT_CANCEL
BtnItem Enabled
274 30 300 100 
Cancel

Setgroup 1
*I VI_INT_SELECTOR
Selector Enabled
95 30 200 180

*I VI_INT_VSCROLL
Vscroll Enabled
96 182 201 192

Setgroup 2
*I VI_INT_STK
RadioItem Enabled
100 270 120 380
Stacking

*I VI_INT_INT
RadioItem Enabled
130 270 150 380
Interval

*I VI_INT_AVG
RadioItem Enabled
160 270 180 380
Average

Setgroup 3
*I VI_INT_NOR
RadioItem Enabled
100 390 120 500 
Normal

*I VI_INT_RES
RadioItem Enabled
130 390 150 500
Residual

,129
*N VI_INT_COMP_VEL
*S PR 0 PU 1
45 45 340 440
Back Interpolate Lines from Grid
6

*I VI_INT_TITLE2
StatText Enabled
0 95 35 380
Back Interpolate Lines from Grid

*I VI_INT_HSOURCE
StatText Enabled
40 120 75 370
Choose Horizon Source

*I VI_INT_COM_OK
BtnItem Enabled
255 290 281 360
OK

*I VI_INT_COM_CANCEL
BtnItem Enabled
255 35 281 105
Cancel

Setgroup 1
*I VI_INT_H_SELECTOR
Selector Enabled
85 100 230 260

*I VI_GRID_H_VSCROLL
Vscroll Enabled
86 262 231 272

