*T CDLG
,101
*N DIGITIZE_CONTROL_POINT
*S PR 0 PU 1
95 120 295 395
Digitize Control Points
9

*I DIG_PNT_STAT_MESSAGE
StatText Disabled
115 5 160 260


*I DIG_PNT_BTN_OK
BtnItem Enabled
155 205 180 255
OK

*I DIG_PNT_BTN_BACKUP
BtnItem Enabled
155 108 180 158
Backup

*I DIG_PNT_BTN_CANCEL
BtnItem Enabled
155 10 180 60
Cancel

*I DIG_PNT_STAT_ENTER
StatText Enabled
0 5 30 260
Enter point number ##

*I DIG_PNT_STAT_X
StatText Disabled
35 1 65 70
X or Lat:

*I DIG_PNT_STAT_Y
StatText Disabled
75 1 105 70
Y or Long:

*I DIG_PNT_EDIT_X
EditText Enabled
35 90 67 241

*I DIG_PNT_EDIT_Y
EditText Enabled
75 90 107 241

*T CDLG
,102
*N REG_GET_PROJECTION
*S PR 0 PU 1
45 50 320 460
Register Map
12

*I REG_GET_STAT_MESSAGE
StatText Disabled
0 15 30 400


*I REG_GET_STAT_SELECT
StatText Disabled
39 20 67 160
Select a Projection:

SetGroup 1

*I REG_GET_SEL_BOX
Selector Enabled
70 17 180 163


*I REG_GET_SCR_BAR
VScroll Enabled
70 167 180 192


SetGroup 2

*I REG_GET_STAT_POINTS
StatText Disabled
190 15 220 135
Number of points:

*I REG_GET_EDIT_POINTS
EditText Enabled
190 137 222 198


SetGroup 3

*I REG_COORD_TYPE_BOX
Rectitem Enabled
70 203 160 380


*I REG_XY_COORD
RadioItem Enabled
75 208 100 370
XY Coordinates

*I REG_USPLS_COORD
RadioItem Enabled
100 208 125 370
USPLS Coordinates

*I REG_ATS_COORD
RadioItem Enabled
125 208 160 370
ATS Coordinates

SetGroup 4

*I REG_GET_BTN_OK
BtnItem Enabled
DefaultButton
230 320 255 385
OK

*I REG_GET_BTN_CANCEL
BtnItem Enabled
230 15 255 80
Cancel


*T CDLG
,103
*N DIGITIZE_STR_CONTROL_POINT
*S PR 0 PU 1
95 96 275 399
Register Map
7

*I DIG_PNT_TITLE
StatText Enabled
0 62 30 238
Map Registration Corners

*I DIG_PNT_STAT_LL
StatText Enabled
45 32 75 160
Lower Left STR

*I DIG_PNT_STAT_UR
StatText Enabled
85 30 115 160
Upper Right STR

*I DIG_PNT_EDIT_LL
EditText Enabled
45 161 77 283
<LL_STR>

*I DIG_PNT_EDIT_UR
EditText Enabled
85 161 117 283
<UR_STR>

*I DIG_STR_BTN_OK
BtnItem Enabled
130 174 155 239
OK

*I DIG_STR_BTN_CANCEL
BtnItem Enabled
130 57 155 122
Cancel

*T CDLG
,104
*N CHECKPOINT_CDLG 
*S PR 0 PU 1 
72 72 265 353
Checkpoint
6
*I CHK_PNT_X_LABEL
StatText Enabled
20 26 50 95
X or Lat:

*I CHK_PNT_Y_LABEL
StatText Enabled
60 17 90 95
Y or Long:

*I CHK_PNT_X
EditText Enabled
20 100 52 250

*I CHK_PNT_Y
EditText Enabled
60 100 92 250

*I CHK_YESNO_BOX
RectItem
110 51 190 231


*I CHK_YESNO_LABEL
StatText Enabled
120 69 175 250
Digitize a checkpoint.  Press C or D button to exit.
