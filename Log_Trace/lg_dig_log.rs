*UX
*T CDLG
,100
*N LG_RETRIEVE_TRACE
*S PR 0 PU 1
44 26 326 500
Retrieve Trace
4

*I LG_RTR_SELECT_LIST
Selector Enabled
7 17 220 432
tops...

*I LG_RTR_SELECT_BAR
Vscroll Enabled
7 433 120 456 


*I LG_RTR_BTN_CANCEL
BtnItem Enabled
240 67 265 147
Cancel

*I LG_RTR_BTN_RETRIEVE
BtnItem Enabled
240 329 265 409
Retrieve

*T CDLG
,101
*N GET_LOG_HEADER
*S PR 0 PU 1
45 27 275 475
Log Header
9

*I DIG_LOG_WELL_ID_LBL
StatText Enabled
35 20 65 119
UWI

*I DIG_LOG_WELL_ID
EditText Enabled
35 117 67 243


*I DIG_LOG_TRACE_NAME_LBL
StatText Enabled
85 20 115 119
Trace Name

*I DIG_LOG_TRACE_NAME
EditText Enabled
85 117 117 413

*I DIG_LOG_COMMENTS_LBL
StatText Enabled
135 20 165 119
Comments

*I DIG_LOG_COMMENTS
EditText Enabled
135 117 167 380

*I DIG_LOG_BTN_OK
BtnItem Enabled
185 314 210 413
Proceed

*I DIG_LOG_BTN_EXIT
BtnItem Enabled
185 27 210 107
Exit

*I DIG_LOG_STAT_MESSAGE
StatText Enabled
0 22 30 405



*T CDLG
,102
*N DIG_PARM_PARAMETERS
54 4 465 503
Log Parameters
26

*I DIG_PARM_STAT_MESSAGE
StatText Enabled
0 5 30 428


*I DIG_PARM_MIN_TD_LBL
StatText Enabled
35 5 65 195
Minimum Time/Depth Value

*I DIG_PARM_MAX_TD_LBL
StatText Enabled
75 5 105 195
Maximum Time/Depth Value

*I DIG_PARM_TOP_SCALE_LBL
StatText Disabled
115 5 145 195
Upper(Top) Log Scale Value

*I DIG_PARM_BTM_SCALE_LBL
StatText Enabled
155 5 185 215
Lower(Bottom) Log Scale Value

*I DIG_PARM_OFFSCALE_LBL
StatText Enabled
195 5 225 215
Number of Offscales(Up,Down)

*I DIG_PARM_SAMPLE_LBL
StatText Enabled
235 5 265 175
Log Sample Increment

*I DIG_PARM_TRACE_UNITS_LBL
StatText Enabled
275 5 305 175
Time/Depth Units

*I DIG_PARM_LOG_UNITS_LBL
StatText Enabled
315 5 345 175
Log Scale Units


*I DIG_PARM_MIN_TD
EditText Enabled
35 217 67 303


*I DIG_PARM_MAX_TD
EditText Enabled
75 217 107 303


*I DIG_PARM_TOP_SCALE
EditText Enabled
115 217 147 303


*I DIG_PARM_BTM_SCALE
EditText Enabled
155 217 187 303


*I DIG_PARM_OFFSCALE_UP
EditText Enabled
195 217 227 255


*I DIG_PARM_OFFSCALE_DOWN
EditText Enabled
195 265 227 303

*I DIG_PARM_SAMPLE
EditText Enabled
235 217 267 303

*I DIG_PARM_TRACE_UNITS
EditText Enabled
275 217 307 303

*I DIG_PARM_LOG_UNITS
EditText Enabled
315 217 347 303


SetGroup 1

*I DIG_PARM_RADIO_ENGLISH
RadioItem Enabled
35 320 65 475
Units are English

*I DIG_PARM_RADIO_METRIC
RadioItem Disabled
70 320 100 475
Units are Metric

SetGroup 2

*I DIG_PARM_RADIO_LINEAR
RadioItem Enabled
155 320 185 506
Log Scale is Linear

*I DIG_PARM_RADIO_LOG
RadioItem Enabled
190 320 230 506
Log Scale is Logarithmic

SetGroup 3

*I DIG_PARM_RADIO_DEPTH
RadioItem Enabled
275 320 305 475
Trace Scale is Depth

*I DIG_PARM_RADIO_TIME
RadioItem Disabled
310 320 350 475
Trace Scale is Time

SetGroup 4

*I DIG_PARM_BTN_OK
BtnItem Enabled
365 390 390 477
Proceed

*I DIG_PARM_BTN_CANCEL
BtnItem Enabled
365 20 390 100
Cancel


*T CDLG
,103
*N DIG_REG_LOG_SEGMENT
*S PR 0 PU 1
52 30 350 473
Register Log Segment
17


*I DIG_REG_STAT_MESSAGE
StatText Enabled
0 3 30 438


*I DIG_REG_X_LBL
StatText Enabled
35 18 65 190
The X-axis is aligned with

*I DIG_REG_X_LBL_UNITS
StatText Enabled
35 196 65 274


*I DIG_REG_Y_LBL
StatText Enabled
70 19 100 299
The Y-axis is aligned with the log scale

*I DIG_REG_LL_CP_LBL
StatText Enabled
145 20 175 192
Lower Left Control Point

*I DIG_REG_X_COL_LBL
StatText Enabled
110 251 140 280
X

*I DIG_REG_Y_COL_LBL
StatText Enabled
110 361 140 389
Y

*I DIG_REG_UL_CP_LBL
StatText Enabled
180 20 210 212
Upper Left Control Point

*I DIG_REG_LR_CP_LBL
StatText Enabled
215 20 245 201
Lower Right Control Point

*I DIG_REG_LL_CP_X
EditText Enabled
145 217 177 300


*I DIG_REG_LL_CP_Y
EditText Enabled
145 327 177 410


*I DIG_REG_UL_CP_X
EditText Enabled
180 218 212 300

*I DIG_REG_UL_CP_Y
EditText Enabled
180 327 212 410

*I DIG_REG_LR_CP_X
EditText Enabled
215 217 247 300


*I DIG_REG_LR_CP_Y
EditText Enabled
215 327 247 410

*I DIG_REG_BTN_OK
BtnItem Enabled
260 340 285 410
Proceed

*I DIG_REG_BTN_CANCEL
BtnItem Enabled
260 30 285 102
Cancel


*T CDLG
,104
*N DIG_DIGITIZE
68 42 320 472
Log Trace Digitization
32

*I DIG_STAT_MESSAGE
StatText Enabled
0 20 30 400


*I DIG_SHOW_X_LBL
StatText Enabled
35 5 65 95


*I DIG_SHOW_X
StatText Enabled
35 100 65 199


*I DIG_SHOW_Y_LBL
StatText Enabled
70 5 100 86
Log Value

*I DIG_SHOW_Y
StatText Enabled
70 100 100 198


*I DIG_PUCK_1
BtnItem Disabled
110 5 128 21
1

*I DIG_PUCK_2
BtnItem Disabled
130 5 148 21
2

*I DIG_PUCK_3
BtnItem Disabled
150 5 168 21
3

*I DIG_PUCK_4
BtnItem Disabled
170 5 188 21
4

*I DIG_PUCK_5
BtnItem Disabled
190 5 208 21
5

*I DIG_PUCK_6
BtnItem Disabled
210 5 228 21
6

*I DIG_PUCK_7
BtnItem Disabled
110 230 128 246
7

*I DIG_PUCK_8
BtnItem Disabled
130 230 148 246
8

*I DIG_PUCK_9
BtnItem Disabled
150 230 168 246
9

*I DIG_PUCK_A
BtnItem Disabled
170 230 188 246
A


*I DIG_PUCK_C
BtnItem Disabled
190 230 208 246
C

*I DIG_PUCK_D
BtnItem Disabled
210 230 228 246
D

*I DIG_PUCK_LBL_1
StatText Enabled
110 25 135 135
Add a Point

*I DIG_PUCK_LBL_2
StatText Enabled
130 25 155 135
Offscale Up

*I DIG_PUCK_LBL_3
StatText Enabled
150 25 175 200
Offscale Down

*I DIG_PUCK_LBL_4
StatText Enabled
170 25 195 200
Delete Last Point

*I DIG_PUCK_LBL_5
StatText Enabled
190 25 215 200
Start No-Data Zone

*I DIG_PUCK_LBL_6
StatText Enabled
210 25 235 200
End This Segment

*I DIG_PUCK_LBL_7
StatText Disabled
110 255 135 420
Zoom In

*I DIG_PUCK_LBL_8
StatText Disabled
130 255 155 420
Zoom Out

*I DIG_PUCK_LBL_9
StatText Disabled
150 255 175 420
Pan

*I DIG_PUCK_LBL_C
StatText Disabled
190 255 215 420
Cancel This Trace

*I DIG_PUCK_LBL_D
StatText Enabled
210 255 235 420
Done With This Trace

*I DIG_UPPER_LIMIT_LBL
StatText Enabled
35 225 65 312
Upper Limit

*I DIG_UPPER_LIMIT
StatText Enabled
35 320 65 414


*I DIG_LOWER_LIMIT_LBL
StatText Enabled
70 225 100 312
Lower Limit

*I DIG_LOWER_LIMIT
StatText Enabled
70 320 100 413



*UM
*T CDLG
,100
*N LG_RETRIEVE_TRACE
*S PR 0 PU 1
44 26 326 494
Retrieve Trace
4

*I LG_RTR_SELECT_LIST
Selector Enabled
7 17 201 430
tops...

*I LG_RTR_SELECT_BAR
Vscroll Enabled
14 436 194 456 


*I LG_RTR_BTN_CANCEL
BtnItem Enabled
230 67 262 147
Cancel

*I LG_RTR_BTN_RETRIEVE
BtnItem Enabled
230 329 262 409
Retrieve

*T CDLG
,101
*N GET_LOG_HEADER
*S PR 0 PU 1
45 27 336 482
Log Header
9

*I DIG_LOG_WELL_ID_LBL
StatText Enabled
30 20 46 119
UWI

*I DIG_LOG_WELL_ID
EditText Enabled
27 117 49 243


*I DIG_LOG_TRACE_NAME_LBL
StatText Enabled
105 20 121 119
Trace Name

*I DIG_LOG_TRACE_NAME
EditText Enabled
102 117 124 413

*I DIG_LOG_COMMENTS_LBL
StatText Enabled
180 20 196 119
Comments

*I DIG_LOG_COMMENTS
EditText Enabled
177 117 199 380

*I DIG_LOG_BTN_OK
BtnItem Enabled
246 324 273 423
Proceed

*I DIG_LOG_BTN_EXIT
BtnItem Enabled
246 27 273 107
Exit

*I DIG_LOG_STAT_MESSAGE
StatText Enabled
7 22 24 405



*T CDLG
,102
*N DIG_PARM_PARAMETERS
54 4 320 503
Log Parameters
26

*I DIG_PARM_STAT_MESSAGE
StatText Enabled
6 5 22 428


*I DIG_PARM_MIN_TD_LBL
StatText Enabled
30 5 45 195
Minimum Time/Depth Value

*I DIG_PARM_MAX_TD_LBL
StatText Enabled
53 5 68 195
Maximum Time/Depth Value

*I DIG_PARM_TOP_SCALE_LBL
StatText Disabled
76 5 92 195
Upper(Top) Log Scale Value

*I DIG_PARM_BTM_SCALE_LBL
StatText Enabled
99 5 114 215
Lower(Bottom) Log Scale Value

*I DIG_PARM_OFFSCALE_LBL
StatText Enabled
122 5 137 215
Number of Offscales(Up,Down)

*I DIG_PARM_SAMPLE_LBL
StatText Enabled
145 5 160 175
Log Sample Increment

*I DIG_PARM_TRACE_UNITS_LBL
StatText Enabled
168 5 183 175
Time/Depth Units

*I DIG_PARM_LOG_UNITS_LBL
StatText Enabled
191 5 216 175
Log Scale Units


*I DIG_PARM_MIN_TD
EditText Enabled
27 217 45 293


*I DIG_PARM_MAX_TD
EditText Enabled
50 217 68 293


*I DIG_PARM_TOP_SCALE
EditText Enabled
73 217 91 293


*I DIG_PARM_BTM_SCALE
EditText Enabled
96 217 114 293


*I DIG_PARM_OFFSCALE_UP
EditText Enabled
119 217 137 243


*I DIG_PARM_OFFSCALE_DOWN
EditText Enabled
119 267 137 293

*I DIG_PARM_SAMPLE
EditText Enabled
142 217 160 293

*I DIG_PARM_TRACE_UNITS
EditText Enabled
165 217 183 303

*I DIG_PARM_LOG_UNITS
EditText Enabled
188 217 206 303


SetGroup 1

*I DIG_PARM_RADIO_ENGLISH
RadioItem Enabled
25 320 41 453
Units are English

*I DIG_PARM_RADIO_METRIC
RadioItem Disabled
40 320 56 452
Units are Metric

SetGroup 2

*I DIG_PARM_RADIO_LINEAR
RadioItem Enabled
100 320 116 468
Log Scale is Linear

*I DIG_PARM_RADIO_LOG
RadioItem Enabled
120 320 136 506
Log Scale is Logarithmic

SetGroup 3

*I DIG_PARM_RADIO_DEPTH
RadioItem Enabled
180 320 196 497
Trace Scale is Depth

*I DIG_PARM_RADIO_TIME
RadioItem Disabled
195 320 211 484
Trace Scale is Time

SetGroup 4

*I DIG_PARM_BTN_OK
BtnItem Enabled
226 390 256 477
Proceed

*I DIG_PARM_BTN_CANCEL
BtnItem Enabled
226 20 256 100
Cancel


*T CDLG
,103
*N DIG_REG_LOG_SEGMENT
*S PR 0 PU 1
52 30 331 473
Register Log Segment
17


*I DIG_REG_STAT_MESSAGE
StatText Enabled
5 3 21 438


*I DIG_REG_X_LBL
StatText Enabled
30 18 46 190
The X-axis is aligned with

*I DIG_REG_X_LBL_UNITS
StatText Enabled
30 196 46 274


*I DIG_REG_Y_LBL
StatText Enabled
47 19 63 299
The Y-axis is aligned with the log scale

*I DIG_REG_LL_CP_LBL
StatText Enabled
100 20 116 192
Lower Left Control Point

*I DIG_REG_X_COL_LBL
StatText Enabled
70 251 86 270
X

*I DIG_REG_Y_COL_LBL
StatText Enabled
70 361 86 379
Y

*I DIG_REG_UL_CP_LBL
StatText Enabled
130 20 146 212
Upper Left Control Point

*I DIG_REG_LR_CP_LBL
StatText Enabled
160 20 176 201
Lower Right Control Point

*I DIG_REG_LL_CP_X
EditText Enabled
97 217 119 293


*I DIG_REG_LL_CP_Y
EditText Enabled
97 327 119 403


*I DIG_REG_UL_CP_X
EditText Enabled
127 218 149 294

*I DIG_REG_UL_CP_Y
EditText Enabled
127 327 149 403

*I DIG_REG_LR_CP_X
EditText Enabled
157 217 179 293


*I DIG_REG_LR_CP_Y
EditText Enabled
157 327 179 403

*I DIG_REG_BTN_OK
BtnItem Enabled
230 340 260 410
Proceed

*I DIG_REG_BTN_CANCEL
BtnItem Enabled
230 30 260 102
Cancel


*T CDLG
,104
*N DIG_DIGITIZE
68 42 304 472
Log Trace Digitization
32

*I DIG_STAT_MESSAGE
StatText Enabled
5 20 21 400


*I DIG_SHOW_X_LBL
StatText Enabled
45 5 61 95


*I DIG_SHOW_X
StatText Enabled
45 100 61 199


*I DIG_SHOW_Y_LBL
StatText Enabled
75 5 91 86
Log Value

*I DIG_SHOW_Y
StatText Enabled
75 100 91 198


*I DIG_PUCK_1
BtnItem Disabled
110 5 126 21
1

*I DIG_PUCK_2
BtnItem Disabled
130 5 146 21
2

*I DIG_PUCK_3
BtnItem Disabled
150 5 166 21
3

*I DIG_PUCK_4
BtnItem Disabled
170 5 186 21
4

*I DIG_PUCK_5
BtnItem Disabled
190 5 206 21
5

*I DIG_PUCK_6
BtnItem Disabled
210 5 226 21
6

*I DIG_PUCK_7
BtnItem Disabled
110 230 126 246
7

*I DIG_PUCK_8
BtnItem Disabled
130 230 146 246
8

*I DIG_PUCK_9
BtnItem Disabled
150 230 166 246
9

*I DIG_PUCK_A
BtnItem Disabled
170 230 186 246
A


*I DIG_PUCK_C
BtnItem Disabled
190 230 206 246
C

*I DIG_PUCK_D
BtnItem Disabled
210 230 226 246
D

*I DIG_PUCK_LBL_1
StatText Enabled
110 25 126 135
Add a Point

*I DIG_PUCK_LBL_2
StatText Enabled
130 25 146 135
Offscale Up

*I DIG_PUCK_LBL_3
StatText Enabled
150 25 166 200
Offscale Down

*I DIG_PUCK_LBL_4
StatText Enabled
170 25 186 200
Delete Last Point

*I DIG_PUCK_LBL_5
StatText Enabled
190 25 206 200
Start No-Data Zone

*I DIG_PUCK_LBL_6
StatText Enabled
210 25 226 200
End This Segment

*I DIG_PUCK_LBL_7
StatText Disabled
110 255 126 420
Zoom In

*I DIG_PUCK_LBL_8
StatText Disabled
130 255 146 420
Zoom Out

*I DIG_PUCK_LBL_9
StatText Disabled
150 255 166 420
Pan

*I DIG_PUCK_LBL_C
StatText Disabled
190 255 206 420
Cancel This Trace

*I DIG_PUCK_LBL_D
StatText Enabled
210 255 226 420
Done With This Trace

*I DIG_UPPER_LIMIT_LBL
StatText Enabled
45 225 61 312
Upper Limit

*I DIG_UPPER_LIMIT
StatText Enabled
45 320 61 414


*I DIG_LOWER_LIMIT_LBL
StatText Enabled
75 225 91 312
Lower Limit

*I DIG_LOWER_LIMIT
StatText Enabled
75 320 91 413



