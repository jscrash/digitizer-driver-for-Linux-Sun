*UX
*T CDLG
,1
*N MD_BASE_DIALOG
*S PR 0 PU 1
100 100 200 400
Mapping
2

*I MD_BASE_LINE_1
StatText Enabled
20 70 53 230
Finder Mapping Module

*I MD_BASE_LINE_2
StatText Enabled
80 38 96 245

*T CDLG
,2
*N MD_OVERLAYS
*S PR 0 PU 1
40 5 505 640
Map Definition
25

*I MD_OVER_MSG
StatText Enabled
0 2 30 595


*I MD_MAP_NUMBER
StatText Enabled
30 2 55 70
MAP #

*I MD_NAME_TITLE
StatText Enabled
30 100 55 200
MAP NAME:

*I MD_MAP_NAME
EditText Enabled
27 185 60 500


*I MD_OVER_TEXT
StatText Enabled
65 2 90 80
Overlays:

*I MD_OVER_ADD_BTN
BtnItem Enabled
65 110 90 150
Add

*I MD_KEY_TEXT
StatText Enabled
65 176 90 250
Options:

*I MD_KEY_ADD_BTN
BtnItem Enabled
65 294 90 334
Add

*I MD_PARM_TEXT
StatText Enabled
65 360 90 451
Parameters:

*I MD_PARM_ADD_BTN
BtnItem Enabled
65 555 90 595
Add

*I MD_PARM_CANCEL
BtnItem Enabled
210 545 235 595 
Cancel

SetGroup 1

*I MD_OVER_SELECT
Selector Enabled
95 5 207 145
<OVER_1>
<OVER_2>
<OVER_3>
<OVER_4>
<OVER_5>
<OVER_6>
<OVER_7>
<OVER_8>

*I MD_OVER_SCROLL
VScroll Enabled
95 148 207 171

SetGroup 2

*I MD_KEY_SELECT
Selector Enabled
95 176 207 334

*I MD_KEY_SCROLL
VScroll Enabled
95 335 207 355 

SetGroup 3

*I MD_PARM_SELECT
Selector Enabled
95 360 207 595 

*I MD_PARM_SCROLL
VScroll Enabled
95 598 207 622 

SetGroup 4

*I MD_STAT_TEXT_2
StatText Enabled
210 4 240 160
Overlay Preferences:

*I MD_OVER_PREFS
Selector Enabled
240 5 425 170 

*I MD_KEY_PREFS
Selector Enabled
240 172 425 355 

*I MD_PARM_PREFS
Selector Enabled
240 357 425 595 

*I MD_PREFS_SCROLL
VScroll Enabled
241 598 425 622 

SetGroup 5

*I MD_OVER_DELETE_BTN
BtnItem Enabled
431 21 453 142
Delete Overlay

*I MD_KEY_DELETE_BTN
BtnItem Enabled
431 242 453 363
Delete Option

*I MD_PARM_EXPAND_BTN
BtnItem Enabled
431 494 453 622
Edit Parameter

*T CDLG
,3
*N MD_MAP_SETUP
*S PR 0 PU 1
44 5 490 540
Map Setup
29

*I MD_SETUP_MSG
StatText Enabled
0 10 28 510


*I MD_SETUP_CANCEL
BtnItem Enabled
400 30 430 105
Cancel

*I MD_SETUP_OK
BtnItem Enabled
DEFAULTBUTTON
400 435 430 510
OK

*I MD_PROJ_SELECT_LBL
StatText Enabled
30 10 58 150
Map Projection

*I MD_MAP_CORN_LBL
StatText Enabled
30 340 58 500
Area of Interest

*I MD_SIZE_SCALE
ChkItem Enabled
370 310 400 510
Scale Sizes for Plotting

SetGroup 1

*I MD_PROJ_SELECT
Selector Enabled
60 10 165 170

*I MD_PROJ_SCROLL
VScroll Enabled
60 175 165 195


SetGroup 2

*I MD_MAP_SCALE_LBL
StatText Enabled
170 10 198 100
Plot Scale

*I MD_MAP_SCALE
StatText Enabled
200 10 228 200


*I MD_EDIT_SCALE
BtnItem Enabled
230 10 258 180
Edit Scale

*I MD_MAP_SIZE_STAT
StatText Enabled
274 10 302 180
Plot Size

*I MD_MAP_SIZE
StatText Enabled
304 10 332 280

SetGroup 

*I MD_BOARDERS_RADIO
RadioItem Enabled
334 10 362 150
Size w/ borders

*I MD_WO_BOARDERS_RADIO
RadioItem Enabled
364 10 392 150
Size w/o borders

SetGroup 3

*I MD_INCHES_RADIO
RadioItem Enabled
334 150 362 265
inches

*I MD_CENTI_RADIO
RadioItem Enabled
364 150 392 265
centimeters

SetGroup 4

*I MD_XY_RADIO
RadioItem Enabled
85 215 113 310
XY 

*I MD_USPLS_RADIO
RadioItem Enabled
115 215 143 310
STR Grid

*I MD_MAP_CORNERS_LBL
StatText Enabled
180 235 208 325
Map Corners

*I MD_COORD_FORMAT
StatText Enabled
180 340 210 510


*I MD_LL_CORNER_LBL
StatText Enabled
206 270 234 308
LL

*I MD_LL_CORNER
StatText Enabled
206 310 234 520


*I MD_UR_CORNER_LBL
StatText Enabled
245 270 273 308
UR

*I MD_UR_CORNER
StatText Enabled
245 310 273 520


*I MD_EDIT_CORNERS
BtnItem Enabled
275 310 303 485
Edit Corners

*I MD_COORD_LBL
StatText Enabled
55 310 82 520
Select Coordinate System

SetGroup 5

*I MD_COORD_SELECT
Selector Enabled
79 310 178 485

*I MD_COORD_SCROLL
VScroll Enabled
79 490 178 505


*T CDLG
,4
*N MD_DELETE_MAP
*S PR 0 PU 1
42 106 340 596
Delete Maps
6

*I MD_DELETE_MAP_MSG
StatText Enabled
0 15 30 280


*I MD_DELETE_MAP_LBL
StatText Enabled
30 50 60 233
Select Maps for Deletion

SetGroup 1

*I MD_DELETE_MAP_SELECT
Selector Enabled
65 17 240 443


*I MD_DELETE_MAP_SCROLL
VScroll Enabled
65 443 240 450


SetGroup 2

*I MD_DELETE_MAP_OK
BtnItem Enabled
DEFAULTBUTTON
255 340 280 410
OK

*I MD_DELETE_MAP_CANCEL
BtnItem Enabled
255 45 280 115
Cancel

*T CDLG
,5
*N MD_ACTIVATE_MAP
*S PR 0 PU 1
45 80 315 445
Activate a Map
6

*I MD_ACTIVATE_TEXT
StatText Enabled
8 17 38 170
Please Select a Map:

SetGroup 1

*I MD_ACTIVATE_SELECT
Selector Enabled
40 20 210 320


*I MD_ACTIVATE_SCROLL
VScroll Enabled
39 320 211 340


SetGroup 2

*I MD_ACTIVATE_CANCEL
BtnItem Enabled
225 15 250 75
CANCEL

*I MD_ACTIVATE_NEW
BtnItem Enabled
225 95 250 215
Activate as New

*I MD_REACTIVATE
BtnItem Enabled
DEFAULTBUTTON
225 235 250 335
Re-Activate
 
*T CDLG
,6
*N MD_TITLE_BLOCK_DLG
*S PR 0 PU 1
39 1 365 315
Title Block
15

*I MD_TITLE_TEXT_PROMPT
StatText Enabled
1 125 30 230
EDIT TEXT

*I MD_TITLE_TEXT
EditText Enabled
35 15 85 295


*I MD_REPLACE_TITLE
BtnItem Enabled
DEFAULTBUTTON
283 235 303 295
Refresh

*I MD_TITLE_PROMPT
StatText Enabled
85 110 120 240
PLEASE SELECT

*I MD_TITLE_BOX_SEP
RectItem Enabled
85 0 87 315


SetGroup 1

*I MD_TITLE_FIELD_PROMPT
StatText Enabled
105 15 130 200
Title Field:

*I MD_TITLE_FIELD_SELECT
Selector
126 15 305 190

*I MD_TITLE_SCROLL
VScroll
125 191 306 200


SetGroup 2

*I MD_TITLE_SIZE
StatText Enabled
105 235 130 292
Size:

*I MD_TITLE_SIZE_BOX
RectItem Enabled
125 235 265 295


*I MD_TITLE_SIZE_1
RadioItem Enabled
135 243 153 285
1

*I MD_TITLE_SIZE_2
RadioItem Enabled
159 243 179 285
2

*I MD_TITLE_SIZE_3
RadioItem Enabled
185 243 205 285
3

*I MD_TITLE_SIZE_4
RadioItem Enabled
211 243 231 285
4

*I MD_TITLE_SIZE_5
RadioItem Enabled
237 243 267 285
5

*T CDLG
,7
*N MD_DELETE_TTLBLK
*S PR 0 PU 1
42 106 340 390
Delete Title Block
6

*I MD_DELETE_TTLBLK_MSG
StatText Enabled
0 15 30 280


*I MD_DELETE_TTLBLK_LBL
StatText Enabled
30 40 60 243
Select Title Blocks for Deletion

SetGroup 1

*I MD_DELETE_TTLBLK_SELECT
Selector Enabled
65 27 240 229


*I MD_DELETE_TTLBLK_SCROLL
VScroll Enabled
65 229 240 250


SetGroup 2

*I MD_DELETE_TTLBLK_OK
BtnItem Enabled
DEFAULTBUTTON
255 190 280 250
OK

*I MD_DELETE_TTLBLK_CANCEL
BtnItem Enabled
255 35 280 95
Cancel

*T CDLG
,8
*N MD_INPUT_TEXT
*S PR 0 PU 1
100 100 315 415
Parameter Entry
5

*I MD_INPUT_TEXT_MESSAGE
StatText Enabled
0 5 30 295


SetGroup 1

*I MD_INPUT_TEXT_FIELD
EditText Enabled
35 12 155 269


*I MD_INPUT_TEXT_SCROLL
VScroll Enabled
35 269 155 290


SetGroup 2

*I MD_INPUT_TEXT_OK
BtnItem Enabled
DEFAULTBUTTON
170 230 195 290
OK

*I MD_INPUT_TEXT_CANCEL
BtnItem Enabled
170 15 195 75
Cancel

*T CDLG
,9
*N MP_GET_FILE
*S PR 0 PU 1
40 65 465 445
File Selection
13

*I MP_FILE_ERR
StatText Enabled
0 2 30 300


*I MP_TYPE_HEAD
StatText Enabled
30 30 60 120
File Type:

*I MP_FILE_TYPE
StatText Enabled
30 121 60 310


*I MP_FILE_PROMPT
StatText Enabled
305 7 330 130
Current Choice:

*I MP_FILE_CHOICE
EditText Enabled
335 10 367 340


*I MP_FILE_CANCEL
BtnItem Enabled
380 50 405 110
CANCEL

*I MP_FILE_OK
BtnItem Enabled
DEFAULTBUTTON
380 240 405 300
OK
 
SetGroup 1

*I MP_EXIST_PROMPT
StatText Enabled
70 7 95 130
Existing Files:

*I MP_EXIST_SELECT
Selector Enabled
100 10 180 340


*I MP_EXIST_SCROLL
VScroll Enabled
100 340 180 360


SetGroup 2

*I MP_DEFINED_PROMPT
StatText Enabled
180 7 205 130
Defined Files:

*I MP_DEFINED_SELECT
Selector Enabled
210 10 295 340


*I MP_DEFINED_SCROLL
VScroll Enabled
210 340 295 360

*T CDLG
,10
*N MP_CORNER_EDIT_DLG
*S PR 0 PU 1
380 40 640 420
Map Corners
9

*I MP_CORNER_MSG
StatText Enabled
0 0 60 360

*I MP_ENTER_STAT
StatText Enabled
65 10 90 150
Enter Map Limits

*I MP_COORD_FORMAT
StatText Enabled
65 160 90 350


*I MP_LL_STAT
StatText Enabled
100 5 130 40
LL

*I MP_LL_EDIT
EditText Enabled
100 50 130 370


*I MP_UR_STAT
StatText Enabled
150 5 180 40
UR

*I MP_UR_EDIT
EditText Enabled
150 50 180 370

*I MP_CORNER_CANCEL
BtnItem Enabled
210 30 240 100
Cancel

*I MP_CORNER_OK
BtnItem Enabled
DEFAULTBUTTON
210 280 240 350
OK

*T CDLG
,11
*N MP_SCALE_EDIT_DLG
*S PR 0 PU 1
340 40 560 400
Map Scale
5

*I MP_SCALE_MSG
StatText Enabled
0 10 30 350

*I MP_ENTER_STAT_2
StatText Enabled
40 20 70 150
Enter Map Scale

*I MP_SCALE_EDIT
EditText Enabled
80 20 110 350

*I MP_SCALE_CANCEL
BtnItem Enabled
175 20 200 90
Cancel

*I MP_SCALE_OK
BtnItem Enabled
DEFAULTBUTTON
175 270 200 340
OK


