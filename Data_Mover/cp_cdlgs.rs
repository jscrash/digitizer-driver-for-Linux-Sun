*UX
*T CDLG
,901
*N CP_MAIN
*S PR 0 PU 1
40 40 340 440
Data Mover Definition Selector
7

*I CP_MAIN_MSG
StatText Enabled
5 10 50 400

*I CP_MAIN_CANCEL
BtnItem Enabled
240 32 280 92
Cancel

*I CP_MAIN_NEW
BtnItem Enabled
240 124 280 184
Create

*I CP_MAIN_MODIFY
BtnItem Enabled
240 216 280 276
Modify

*I CP_MAIN_OK
BtnItem Enabled
DefaultButton
240 308 280 368
OK

SetGroup 1

*I CP_MAIN_NAMES
Selector Enabled
60 50 220 347

*I CP_MAIN_NAMES_SCRL
VScroll Enabled
347 60 350 220

,902
*N CP_DEST
*S PR 0 PU 1
40 40 380 320
Data Mover Target Selector
9
 
*I CP_DEST_STAT_1
StatText Enabled
5 10 35 250
Please select the target type:

*I CP_DEST_STAT_2
StatText Enabled
100 10 130 250
Please select the target:

*I CP_DEST_CANCEL
BtnItem Enabled
290 33 325 123
Cancel

*I CP_DEST_OK
BtnItem Enabled
DefaultButton
290 166 325 246
OK

SetGroup 1

*I CP_DEST_F_PROJECT
RadioItem Enabled
40 20 60 170
Finder Project

*I CP_DEST_EXT_APPL
RadioItem Enabled
65 20 90 170
External Application

SetGroup 2

*I CP_DEST_NAMES
Selector Enabled
135 20 285 200

*I CP_DEST_NAMES_SCRL
VScroll Enabled
135 200 285 213

*I CP_DEST_RECT
RectItem Enabled
38 15 94 180

,903
*N CP_TRANS
*S PR 0 PU 1
25 25 435 400
Data Mover Execution
15

*I CP_TRANS_STAT_UPD
StatText Enabled
5 5 35 250
Please Select a Data Update Mode:

*I CP_TRANS_STAT_CONF
StatText Enabled
100 5 130 250
Please Confirm:

*I CP_TRANS_STAT_NAME
StatText Enabled
135 20 165 220
Data Mover Definition Name:

*I CP_TRANS_EDIT_NAME
StatText Enabled
135 225 165 355

*I CP_TRANS_STAT_FT
StatText Enabled
170 20 200 250
Data will be moved 

*I CP_TRANS_STAT_FROM
StatText Enabled
205 20 235 175
FROM project:

*I CP_TRANS_EDIT_FROM
StatText Enabled
205 180 235 355

*I CP_TRANS_STAT_TO
StatText Enabled
240 20 280 175
TO project:

*I CP_TRANS_EDIT_TO
StatText Enabled
240 180 280 355

*I CP_TRANS_STAT_LOGS1
StatText Enabled
285 20 315 250
Data Mover logs will be sent to:

*I CP_TRANS_EDIT_LOGS
StatText Enabled
320 40 355 355

*I CP_TRANS_CANCEL
BtnItem Enabled
365 30 395 90
Cancel

*I CP_TRANS_OK
BtnItem Enabled
DefaultButton
365 260 395 320
OK

SetGroup 2

*I CP_TRANS_NO_UPD
RadioItem Enabled
40 50 60 300
Insert New Records Only

*I CP_TRANS_YES_UPD
RadioItem Enabled
65 50 90 300
Replace or Modify Existing Records





,904
*N CP_CNTRL
*S PR 0 PU 1
20 20 550 500
Data Mover Data Selector
39

*I CP_CNTRL_STAT_NAME
StatText Enabled
0 20 30 208
Data Mover Definition Name:

*I CP_CNTRL_EDIT_NAME
StatText Enabled
0 209 30 420

*I CP_CNTRL_MSG
StatText Enabled
35 20 65 420

*I CP_CNTRL_STAT_DC
StatText Enabled
90 20 120 150
Data Class

*I CP_CNTRL_STAT_SC
StatText Enabled
90 120 120 350
Selection Criteria

*I CP_CNTRL_RECT
RectItem Enabled
80 10 290 460

*I CP_CNTRL_CANCEL
BtnItem Enabled
470 20 510 100
Cancel

*I CP_CNTRL_OK
BtnItem Enabled
DefaultButton
470 340 510 420
OK


SetGroup 1
*I CP_CNTRL_WELL
StatText Enabled
125 20 150 120
Wells

*I CP_CNTRL_WELL_NONE
RadioItem Enabled
125 120 145 170
None

*I CP_CNTRL_WELL_SEL
RadioItem Enabled
125 170 145 220
Get Select List

*I CP_CNTRL_WELL_ALL
RadioItem Enabled
125 220 145 365
All

*I CP_CNTRL_WELL_REF
ChkItem Enabled
125 375 145 425
Refine


SetGroup 2
*I CP_CNTRL_SEIS
StatText Enabled
155 20 180 120
Seismic

*I CP_CNTRL_SEIS_NONE
RadioItem Enabled
155 120 175 170
None

*I CP_CNTRL_SEIS_SEL
RadioItem Enabled
155 170 175 220
Get Select List

*I CP_CNTRL_SEIS_ALL
RadioItem Enabled
155 220 175 365
All

*I CP_CNTRL_SEIS_REF
ChkItem Enabled
155 375 175 425
Refine


SetGroup 3
*I CP_CNTRL_LEASE
StatText Enabled
190 20 220 120
Lease

*I CP_CNTRL_LEASE_NONE
RadioItem Enabled
190 120 210 170
None

*I CP_CNTRL_LEASE_SEL
RadioItem Enabled
190 170 210 220
Get Select List

*I CP_CNTRL_LEASE_ALL
RadioItem Enabled
190 220 210 365
All

*I CP_CNTRL_LEASE_REF
ChkItem Enabled
190 375 210 425
Refine


SetGroup 4
*I CP_CNTRL_CULT
StatText Enabled
225 20 255 120
Culture

*I CP_CNTRL_CULT_NONE
RadioItem Enabled
225 120 245 170
None

*I CP_CNTRL_CULT_SEL
RadioItem Enabled
225 170 245 220
Get Select List

*I CP_CNTRL_CULT_ALL
RadioItem Enabled
225 220 245 365
All

*I CP_CNTRL_CULT_REF
ChkItem Enabled
225 375 245 425
Refine


SetGroup 5
*I CP_CNTRL_GO
StatText Enabled
260 15 285 120
Graphic Object

*I CP_CNTRL_GO_NONE
RadioItem Enabled
260 120 280 170
None

*I CP_CNTRL_GO_SEL
RadioItem Enabled
260 170 280 220
Get Select List

*I CP_CNTRL_GO_ALL
RadioItem Enabled
260 220 280 365
All

*I CP_CNTRL_GO_REF
ChkItem Enabled
260 375 280 425
Refine


SetGroup 6
*I CP_CNTRL_GEO_CONST
StatText Enabled
310 50 340 300
Specify Geographic Constraints

*I CP_CNTRL_GEO_NO_CONST
RadioItem Enabled
345 60 363 310
No Geographic Constraints

*I CP_CNTRL_GEO_MAN_ENT
RadioItem Enabled
366 60 384 310
Manual Entry of boundaries

*I CP_CNTRL_GEO_BOUND
RadioItem Enabled
392 60 410 310
Boundaries Referenced to Maps

*I CP_CNTRL_GEO_SPEC
RadioItem Enabled
408 60 435 310
Graphical Specification of Boundaries

*I CP_CNTRL_RECT2
RectItem Enabled
305 420 342 430



,905
*N CP_SELECT
*S PR 0 PU 1
30 30 360 330 
Data Mover Select List
7

*I CP_SELECT_STAT_DTYPE
StatText Enabled
5 10 35 100
Data Type:

*I CP_SELECT_EDIT_DTYPE
StatText Enabled
15 100 45 290

*I CP_SELECT_STAT_NAME
StatText Enabled
55 30 75 270
Select List Name

*I CP_SELECT_NAME_SELS
Selector Enabled
85 30 235 257

*I CP_SELECT_NAME_SCRL
VScroll Enabled
85 257 235 270

*I CP_SELECT_CANCEL
BtnItem Enabled
255 30 295 130
Cancel

*I CP_SELECT_OK
BtnItem Enabled
DefaultButton
255 170 295 270
OK

,906
*N CP_GEO1
*S PR 0 PU 1
10 20 420 420
Data Mover Geographic Constraints - Manual Entry
14

*I CP_GEO1_STAT_PARAM
StatText Enabled
160 10 200 380

*I CP_GEO1_STAT_LLX
StatText Enabled
210 40 240 250
Minimum X or Longitude:

*I CP_GEO1_EDIT_LLX
EditText Enabled
210 250 240 380

*I CP_GEO1_STAT_LLY
StatText Enabled
245 40 275 250
Minimum Y or Latitude:

*I CP_GEO1_EDIT_LLY
EditText Enabled
245 250 275 380

*I CP_GEO1_STAT_URX
StatText Enabled
280 40 310 250
Maximum X or Longitude:

*I CP_GEO1_EDIT_URX
EditText Enabled
280 250 310 380

*I CP_GEO1_STAT_URY
StatText Enabled
315 40 345 250
Maximum Y or Latitude:

*I CP_GEO1_EDIT_URY
EditText Enabled
315 250 345 380

*I CP_GEO1_CANCEL
BtnItem Enabled
350 30 390 100
Cancel

*I CP_GEO1_OK
BtnItem Enabled
DefaultButton
350 300 390 370
OK

SetGroup 1

*I CP_GEO1_STAT_PROJ
StatText Enabled
10 10 30 300
Please select a coordinate system

*I CP_GEO1_PROJ_SELS
Selector Enabled
40 40 140 220

*I CP_GEO1_PROJ_SRCL
VScroll Enabled
40 220 140 233

,907
*N CP_GEO2
*S PR 0 PU 1
10 20 455 420
Data Mover Geographic Constraints - Map Reference
18

*I CP_GEO2_STAT_PARAM1
StatText Enabled
135 10 165 380
Data Mover Geographic Boundary Readout

*I CP_GEO2_STAT_PARAM2
StatText Enabled
166 40 196 380
Coordinate system

*I CP_GEO2_STAT_LLX
StatText Enabled
275 40 305 250
Minimum X or Longitude:

*I CP_GEO2_EDIT_LLX
StatText Enabled
275 250 305 380

*I CP_GEO2_STAT_LLY
StatText Enabled
305 40 335 250
Minimum Y or Latitude:

*I CP_GEO2_EDIT_LLY
StatText Enabled
305 250 335 380

*I CP_GEO2_STAT_URX
StatText Enabled
335 40 365 250
Maximum X or Longitude:

*I CP_GEO2_EDIT_URX
StatText Enabled
335 250 365 380

*I CP_GEO2_STAT_URY
StatText Enabled
365 40 395 250
Maximum Y or Latitude:

*I CP_GEO2_EDIT_URY
StatText Enabled
365 250 395 380

*I CP_GEO2_CANCEL
BtnItem Enabled
400 30 430 90
Cancel

*I CP_GEO2_OK
BtnItem Enabled
DefaultButton
400 310 430 370
OK


SetGroup 1
*I CP_GEO2_PROJ_SELS
Selector Enabled
200 40 268 220

*I CP_GEO2_PROJ_SRCL
VScroll Enabled
200 220 268 233


SetGroup 2
*I CP_GEO2_RAD_STAT
StatText Enabled
5 10 35 300
Please select the reference map.

*I CP_GEO2_TRGT_MAP
RadioItem Enabled
40 20 70 300
Use Target Project Default Map

*I CP_GEO2_SRC_MAP
RadioItem Enabled
75 20 105 300
Use Source Project Default Map

*I CP_GEO2_CUR_MAP
RadioItem Enabled
110 20 130 300
Use Currently Displayed Map




,908
*N CP_SAVE1
*S PR 0 PU 1
50 50 210 400
Please Note!
4

*I CP_SAVE1_STAT
StatText Enabled
10 10 100 340

*I CP_SAVE1_CANCEL
BtnItem Enabled
110 30 140 90
Cancel

*I CP_SAVE1_NO
BtnItem Enabled
110 145 140 205
No

*I CP_SAVE1_YES
BtnItem Enabled
DefaultButton
110 260 140 320
Yes

,909
*N CP_SAVE2
*S PR 0 PU 1
50 50 200 400
Save Data Mover Definition
4

*I CP_SAVE2_STAT
StatText Enabled
10 10 30 340
Please enter a Data Mover Definition name.

*I CP_SAVE2_EDIT
EditText Enabled
40 10 75 280

*I CP_SAVE2_CANCEL
BtnItem Enabled
90 30 130 90
Cancel

*I CP_SAVE2_SAVE
BtnItem Enabled
DefaultButton
90 260 130 320
Save

,910
*N CP_EXT
*S PR 0 PU 1
20 10 375 470
Data Mover External Application Params
16

*I CP_EXT_NAME_STAT
StatText Enabled
5 20 35 200
External Application Name :

*I CP_EXT_NAME_EDIT
StatText Enabled
5 200 35 400

*I CP_EXT_P1_STAT
StatText Enabled
40 10 75 280

*I CP_EXT_P1_EDIT
EditText Enabled
40 290 75 450

*I CP_EXT_P2_STAT
StatText Enabled
80 10 115 280 

*I CP_EXT_P2_EDIT
EditText Enabled
80 290 115 450 

*I CP_EXT_P3_STAT
StatText Enabled
120 10 155 280 

*I CP_EXT_P3_EDIT
EditText Enabled
120 290 155 450 

*I CP_EXT_P4_STAT
StatText Enabled
160 10 195 280 

*I CP_EXT_P4_EDIT
EditText Enabled
160 290 195 450 

*I CP_EXT_P5_STAT
StatText Enabled
200 10 235 280 

*I CP_EXT_P5_EDIT
EditText Enabled
200 290 235 450 

*I CP_EXT_P6_STAT
StatText Enabled
240 10 275 280 

*I CP_EXT_P6_EDIT
EditText Enabled
240 290 275 450 

*I CP_EXT_CANCEL
BtnItem Enabled
285 40 325 140
Cancel

*I CP_EXT_DONE
BtnItem Enabled
DefaultButton
285 320 325 420
Done

$

