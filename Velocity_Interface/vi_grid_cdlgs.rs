*T CDLG
,128
*N VI_GRID_CDLG
*S PR 0 PU 1
45 50 350 500
Grid from Lines
15

*I VI_GRID_TITLE
StatText Enabled
0 120 35 410
Compute Grid from Velocity Analysis

*I VI_VEL_TYPE
StatText Enabled
75 30 110 150
Velocity Type

*I VI_GRID
StatText Enabled
75 175 110 300
Grid Smoothing

*I VI_MODE
StatText Enabled
75 330 110 410
Mode

*I VI_GRID_OK
BtnItem Enabled
244 340 270 410
O K

*I VI_GRID_CANCEL
BtnItem Enabled
244 30 270 100 
Cancel

Setgroup 1
*I VI_GRID_STK
RadioItem Enabled
120 35 140 150
Stacking

*I VI_GRID_INTER
RadioItem Enabled
150 35 170 150
Interval

*I VI_GRID_AVG
RadioItem Enabled
180 35 200 150
Average

Setgroup 2
*I VI_GRID_NONE
RadioItem Enabled
120 180 140 300
NONE

*I VI_GRID_W1
RadioItem Enabled
150 180 170 300
Weighted 1

*I VI_GRID_W2
RadioItem Enabled
180 180 200 300
Weighted 2

*I VI_GRID_MED
RadioItem Enabled
210 180 230 300
Median

Setgroup 3
*I VI_GRID_NOR
RadioItem Enabled
120 330 140 450 
Normal

*I VI_GRID_RES
RadioItem Enabled
150 330 170 450
Residual

,129
*N VI_GRID_COMP_VEL
*S PR 0 PU 1
45 5 340 440
Compute Velocity Grid from lines
9

*I VI_GRID_TITLE
StatText Enabled
0 100 35 400
Compute Grid from Velocity Analysis

*I VI_GRID_VSOURCE
StatText Enabled
40 30 75 200
Choose Velocity Source

*I VI_GRID_HSOURCE
StatText Enabled
40 245 75 400
Choose Horizon Source

*I VI_GRID_COM_OK
BtnItem Enabled
255 315 281 385
O K
  
*I VI_GRID_COM_CANCEL
BtnItem Enabled
255 35 281 105
Cancel

Setgroup 1
*I VI_GRID_V_SELECTOR
Selector Enabled
85 15 230 180
    
*I VI_GRID_V_VSCROLL
Vscroll Enabled
86 182 231 192
  
Setgroup 2
*I VI_GRID_H_SELECTOR
Selector Enabled
85 230 230 395
    
*I VI_GRID_H_VSCROLL
Vscroll Enabled
86 397 231 407
  
,130
*N VI_GRID_REF
*S PR 0 PU 1
45 5 440 440
Reference Velocity Function
14

*I VI_GRID_REF_TITLE
StatText Enabled
0 120 35 400
Reference Velocity Function

*I VI_GRID_REF_TIME
StatText Enabled
40 40 70 200
Time

*I VI_GRID_REF_VEL
StatText Enabled
40 225 70 365
Velocity

*I VI_GRID_SEL_TIME
StatText Enabled
130 40 160 200
Time

*I VI_GRID_SEL_VEL
StatText Enabled
130 225 160 300
Velocity

*I VI_GRID_TIME_TEXT
EditText Enabled
VerifyFloat
75 40 105 200

*I VI_GRID_VEL_TEXT
EditText Enabled
VerifyFloat
75 225 105 385

*I VI_GRID_REF_ADD
BtnItem Enabled
110 315 136 385
Add
  
*I VI_GRID_REF_OK
BtnItem Enabled
345 315 371 385
O K
  
*I VI_GRID_REF_CANCEL
BtnItem Enabled
345 40 371 110
Cancel

*I VI_GRID_REF_DELETE
BtnItem Enabled
345 177 371 247
Delete

Setgroup 1
*I VI_GRID_TIME_SELECTOR
Selector Enabled
160 35 320 205
    
*I VI_GRID_VEL_SELECTOR
Selector Enabled
160 212 320 365
    
*I VI_GRID_VEL_VSCROLL
Vscroll Enabled
161 369 321 379
  

