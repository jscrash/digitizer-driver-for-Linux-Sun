*UX
*L ALL
*T MENU
,311
*N MO_MENU
*S PR 0 PU 1
Model Control
8 D M
*I MO_SELECT_TYPE
Select Model Type
*I MO_DATA_ENTER
Enter velocity and density data
*I MO_DIGITIZE
Edit layers
*I GENERATE_MODEL
Generate Model
*I MO_DRAW_TIEPLOT
Draw tieplot
*I GENERATE_TRACES
Generate traces
*I MO_TRACES
Draw traces
*I MO_QUIT
Quit




*T CDLG
,130
*N MO_DIALOG
*S PR 0 PU 1
100 100 250 400
Strat Modeling
1
*I MO_TEXT
StatText Enabled
8 10 115 288
Finder Strat Modelling Version 0.5






,131
*N MO_DATA_ENTRY
*S PR 0 PU 1
39 89 355 481
Empirical Modeling Data Entry
17

*I MO_DE_DENTEXT
StatText Enabled
55 30 77 158
Density (g/cc)

*I MO_DE_OK
BtnItem Enabled
DEFAULTBUTTON
270 272 295 353
OK

*I MO_DE_CANCEL
BtnItem Enabled
270 33 295 113
CANCEL


*I MO_DE_VELTEXT
StatText Enabled
23 31 50 180
Velocity (ft-m/sec)

*I MO_DE_VELOCITY
EditText Enabled
14 185 46 292
v

*I MO_DE_DENSITY
EditText Enabled
54 185 86 292
d

*I MO_DE_MESSAGE
StatText Enabled
240 31 270 379
msg

*I MO_DE_B1
StatText Disabled
90 31 115 173
Button 1 - Add Point

*I MO_DE_B2
StatText Disabled
115 31 140 174
Button 2 - Pick Point

*I MO_DE_B3
StatText Disabled
140 31 165 173
Button 3 - Edit Point

*I MO_DE_B4
StatText Disabled
165 31 190 193
Button 4 - Delete Point

*I MO_DE_B8
StatText Disabled
90 210 115 371
Button 8 - Zoom in

*I MO_DE_BC
StatText Disabled
115 210 140 373
Button C - Cancel

*I MO_DE_BD
StatText Disabled
140 210 165 390
Button D - Done

*I MO_DE_BF
StatText Disabled
165 210 190 390
Button F - Reset Window

*I MO_DE_B5
StatText Disabled
190 31 215 216
Button 5 - Move point

*I MO_DE_B7
StatText Disabled
215 31 240 216
Button 7 - Redraw model



,133
*N MO_CHANGE_TYPE
*S PR 0 PU 1
62 59 209 330
Set Model Type
4

SETMODAL

SETGROUP 1

*I MO_CT_PURE_EMP
RadioItem Enabled
23 90 39 310
Empirical

*I MO_CT_WELL_TIE
RadioItem Enabled
50 90 65 310
Well Tie


*I MO_CT_OK
BtnItem Enabled
110 150 135 230
OK

*I MO_CT_CANCEL
BtnItem Enabled
110 25 135 105
CANCEL

*T CDLG
,134
*N MO_PARM_EMP
*S PR 0 PU 1
44 24 335 491 
Emp Model Parameters
15

*I MO_PARM_STAT_MESSAGE
StatText Enabled
30 10 60 450

*I MO_PARM_TITLE_LBL
StatText Enabled
75 10 105 138 
Title-comment

*I MO_PARM_MIN_DEPTH_LBL
StatText Enabled
120 10 150 120 
Minimum Depth

*I MO_PARM_MAX_DEPTH_LBL
StatText Enabled
120 235 150 347 
Maximum Depth

*I MO_PARM_MAX_X_LBL
StatText Enabled
160 235 190 347 
Maximum X

*I MO_PARM_MIN_X_LBL
StatText Enabled
160 10 190 120 
Minimum X

*I MO_PARM_DEPTH_UNITS_LBL
StatText Enabled
200 15 230 200 
UOM (FEET/METERS)

*I MO_PARM_TITLE
EditText Disabled
75 122 107 443 
<Title>

*I MO_PARM_MIN_DEPTH
EditText Disabled
120 122 152 208 
<MinDp>

*I MO_PARM_MAX_DEPTH
EditText Disabled
120 355 152 443 
<MaxDp>

*I MO_PARM_MIN_X
EditText Disabled
160 122 192 208 
<MinX>

*I MO_PARM_MAX_X
EditText Disabled
160 355 192 443 
<MaxX>

*I MO_PARM_DEPTH_UNITS
EditText Disabled
200 190 232 280 
<UOM>

*I MO_PARM_BTN_CANCEL
BtnItem Enabled
245 50 270 122
Cancel

*I MO_PARM_BTN_OK
BtnItem Enabled
DEFAULTBUTTON
245 360 270 431
Proceed






*T CDLG
,135
*N MO_REG_MOD_SEGMENT
*S PR 0 PU 1
52 30 331 473
Register Model Segment
14


*I MO_REG_STAT_MESSAGE
StatText Enabled
5 3 35 433

*I MO_REG_LL_CP_LBL
StatText Enabled
100 20 130 192
Lower Left Control Point

*I MO_REG_X_COL_LBL
StatText Enabled
70 245 100 280
X

*I MO_REG_Y_COL_LBL
StatText Enabled
70 340 100 390
Depth

*I MO_REG_UL_CP_LBL
StatText Enabled
140 20 170 212
Upper Left Control Point

*I MO_REG_LR_CP_LBL
StatText Enabled
180 20 210 201
Lower Right Control Point

*I MO_REG_LL_CP_X
EditText Enabled
100 217 132 293
<LL_X>

*I MO_REG_LL_CP_Y
EditText Enabled
100 327 132 403
<LL_Y>

*I MO_REG_UL_CP_X
EditText Enabled
140 218 172 294
<UL_X>

*I MO_REG_UL_CP_Y
EditText Enabled
140 327 172 403
<UL_Y>

*I MO_REG_LR_CP_X
EditText Enabled
180 217 212 293
<LR_X>

*I MO_REG_LR_CP_Y
EditText Enabled
180 327 212 403
<LR_Y>

*I MO_REG_BTN_OK
BtnItem Enabled
DEFAULTBUTTON
230 340 255 410
Proceed

*I MO_REG_BTN_CANCEL
BtnItem Enabled
230 30 255 102
Cancel


,136
*N MO_GET_SED_TYPE
*S PR 0 PU 1
52 60 261 350
Set Sedimentation Type
6

*I MO_SED_STAT_MESSAGE
StatText Enabled
10 5 40 270
Select Method of Sub-Layer Computation

SETGROUP 1

*I MO_SED_NORMAL
RadioItem Disabled
45 90 66 242
Normal

*I MO_SED_UNCONFORM
RadioItem Enabled
80 90 101 242
Unconformity

*I MO_SED_ONLAP
RadioItem Enabled
115 90 136 242
Onlap

*I MO_SED_OK
BtnItem Enabled
DEFAULTBUTTON
160 180 185 250
Go

*I MO_SED_CANCEL
BtnItem Enabled
160 20 185 90
Cancel

,137
*N MO_GRID_INFO
*S PR 0 PU 1
52 60 261 290
Set Model Grid Options
8

*I MO_GRID_STAT_MESSAGE
StatText Enabled
10 5 40 220

*I MO_GRID_ON_OFF
ChkItem Enabled
45 40 70 220
Grid ON/OFF

*I MO_GRID_X_TEXT
StatText Enabled
80 20 105 170
X grid interval

*I MO_GRID_X_VALUE
EditText Enabled
80 150 112 210

*I MO_GRID_Y_TEXT
StatText Enabled
120 20 150 170
Y grid interval

*I MO_GRID_Y_VALUE
EditText Enabled
120 150 152 210

*I MO_GRID_OK
BtnItem Enabled
DEFAULTBUTTON
165 130 190 200
Proceed

*I MO_GRID_CANCEL
BtnItem Enabled
165 20 190 90
Cancel

*UM
*L ALL
*T MENU
,311
*N MO_MENU
*S PR 0 PU 1
Model Control
8 D M
*I MO_SELECT_TYPE
Select Model Type
*I MO_DATA_ENTER
Enter velocity and density data
*I MO_DIGITIZE
Edit layers
*I GENERATE_MODEL
Generate Model
*I MO_DRAW_TIEPLOT
Draw tieplot
*I GENERATE_TRACES
Generate traces
*I MO_TRACES
Draw traces
*I MO_QUIT
Quit




*T CDLG
,130
*N MO_DIALOG
*S PR 0 PU 1
100 100 250 400
Strat Modeling
1
*I MO_TEXT
StatText Enabled
8 10 115 288
Finder Strat Modelling Version 0.5






,131
*N MO_DATA_ENTRY
*S PR 0 PU 1
39 89 335 481
Empirical Modeling Data Entry
17

*I MO_DE_DENTEXT
StatText Enabled
55 30 77 158
Density (g/cc)

*I MO_DE_OK
BtnItem Enabled
DEFAULTBUTTON
254 272 286 353
OK

*I MO_DE_CANCEL
BtnItem Enabled
250 33 282 113
CANCEL


*I MO_DE_VELTEXT
StatText Enabled
23 31 50 162
Velocity (ft-m/sec)

*I MO_DE_VELOCITY
EditText Enabled
14 185 44 292
v

*I MO_DE_DENSITY
EditText Enabled
54 185 84 292
d

*I MO_DE_MESSAGE
StatText Enabled
200 31 247 379
msg

*I MO_DE_B1
StatText Disabled
91 31 108 173
Button 1 - Add Point

*I MO_DE_B2
StatText Disabled
107 31 124 174
Button 2 - Pick Point

*I MO_DE_B3
StatText Disabled
124 31 141 173
Button 3 - Edit Point

*I MO_DE_B4
StatText Disabled
141 31 158 193
Button 4 - Delete Point

*I MO_DE_B8
StatText Disabled
91 210 108 371
Button 8 - Zoom in

*I MO_DE_BC
StatText Disabled
107 210 124 373
Button C - Cancel

*I MO_DE_BD
StatText Disabled
124 210 140 390
Button D - Done

*I MO_DE_BF
StatText Disabled
141 210 158 390
Button F - Reset Window

*I MO_DE_B5
StatText Disabled
158 31 174 216
Button 5 - Move point

*I MO_DE_B7
StatText Disabled
174 31 190 216
Button 7 - Redraw model



,133
*N MO_CHANGE_TYPE
*S PR 0 PU 1
62 59 209 405
Set Model Type
4

SETMODAL

SETGROUP 1

*I MO_CT_PURE_EMP
RadioItem Enabled
23 35 39 220
Empirical

*I MO_CT_WELL_TIE
RadioItem Enabled
50 35 65 248
Well Tie

*I MO_CT_OK
BtnItem Enabled
105 245 137 326
OK

*I MO_CT_CANCEL
BtnItem Enabled
105 32 137 112
CANCEL

*T CDLG
,134
*N MO_PARM_EMP
*S PR 0 PU 1
44 24 327 491 
Emp Model Parameters
15

*I MO_PARM_STAT_MESSAGE
StatText Enabled
35 20 55 450

*I MO_PARM_TITLE_LBL
StatText Enabled
75 15 94 138 
Title-comment

*I MO_PARM_MIN_DEPTH_LBL
StatText Enabled
120 10 138 120 
Minimum Depth

*I MO_PARM_MAX_DEPTH_LBL
StatText Enabled
120 235 138 347 
Maximum Depth

*I MO_PARM_MAX_X_LBL
StatText Enabled
160 235 178 347 
Maximum X

*I MO_PARM_MIN_X_LBL
StatText Enabled
160 10 178 120 
Minimum X

*I MO_PARM_DEPTH_UNITS_LBL
StatText Enabled
200 15 219 200 
UOM (FEET/METERS)

*I MO_PARM_TITLE
EditText Disabled
75 130 94 433 

*I MO_PARM_MIN_DEPTH
EditText Disabled
120 121 139 208 

*I MO_PARM_MAX_DEPTH
EditText Disabled
120 355 139 443 

*I MO_PARM_MIN_X
EditText Disabled
160 121 179 208 

*I MO_PARM_MAX_X
EditText Disabled
160 355 179 443 

*I MO_PARM_DEPTH_UNITS
EditText Disabled
200 210 219 300 

*I MO_PARM_BTN_CANCEL
BtnItem Enabled
240 50 270 122
Cancel

*I MO_PARM_BTN_OK
BtnItem Enabled
DEFAULTBUTTON
240 360 270 431
Proceed






*T CDLG
,135
*N MO_REG_MOD_SEGMENT
*S PR 0 PU 1
52 30 331 473
Register Model Segment
14


*I MO_REG_STAT_MESSAGE
StatText Enabled
5 3 21 438


*I MO_REG_LL_CP_LBL
StatText Enabled
100 20 116 192
Lower Left Control Point

*I MO_REG_X_COL_LBL
StatText Enabled
70 251 86 270
X

*I MO_REG_Y_COL_LBL
StatText Enabled
70 340 86 390
Depth

*I MO_REG_UL_CP_LBL
StatText Enabled
130 20 146 212
Upper Left Control Point

*I MO_REG_LR_CP_LBL
StatText Enabled
160 20 176 201
Lower Right Control Point

*I MO_REG_LL_CP_X
EditText Enabled
97 217 119 293


*I MO_REG_LL_CP_Y
EditText Enabled
97 327 119 403


*I MO_REG_UL_CP_X
EditText Enabled
127 218 149 294

*I MO_REG_UL_CP_Y
EditText Enabled
127 327 149 403

*I MO_REG_LR_CP_X
EditText Enabled
157 217 179 293


*I MO_REG_LR_CP_Y
EditText Enabled
157 327 179 403

*I MO_REG_BTN_OK
BtnItem Enabled
DEFAULTBUTTON
230 340 260 410
Proceed

*I MO_REG_BTN_CANCEL
BtnItem Enabled
230 30 260 102
Cancel


,136
*N MO_GET_SED_TYPE
*S PR 0 PU 1
52 60 261 390
Set Sedimentation Type
6

*I MO_SED_STAT_MESSAGE
StatText Enabled
10 5 31 300
Select Method of Sub-Layer Computation

SETGROUP 1

*I MO_SED_NORMAL
RadioItem Disabled
45 40 66 192
Normal

*I MO_SED_UNCONFORM
RadioItem Enabled
80 40 101 192
Unconformity

*I MO_SED_ONLAP
RadioItem Enabled
115 40 136 192
Onlap

*I MO_SED_OK
BtnItem Enabled
DEFAULTBUTTON
160 240 190 310
Go

*I MO_SED_CANCEL
BtnItem Enabled
160 20 190 90
Cancel

,137
*N MO_GRID_INFO
*S PR 0 PU 1
52 60 261 300
Set Model Grid Options
8

*I MO_GRID_STAT_MESSAGE
StatText Enabled
10 5 31 230

*I MO_GRID_ON_OFF
ChkItem Enabled
45 40 64 220
Grid ON/OFF

*I MO_GRID_X_TEXT
StatText Enabled
80 20 96 170
X grid interval

*I MO_GRID_X_VALUE
EditText Enabled
80 175 100 235

*I MO_GRID_Y_TEXT
StatText Enabled
120 20 137 170
Y grid interval

*I MO_GRID_Y_VALUE
EditText Enabled
120 175 140 235

*I MO_GRID_OK
BtnItem Enabled
DEFAULTBUTTON
165 150 195 220
Proceed

*I MO_GRID_CANCEL
BtnItem Enabled
165 20 195 90
Cancel

