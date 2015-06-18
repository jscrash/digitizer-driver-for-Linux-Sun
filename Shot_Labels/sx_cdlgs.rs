*T CDLG
,128
*N DIALOG_SPO
*S PR 0 PU 1
50 25 355 490
Select Shotpoint Entry Options
19

helpkey FINDER/MAPPING/EDIT_SEISMIC_LINES/SHOTPOINTS/ADD...

*I ITEM_SPO_OK
BtnItem Enabled
DEFAULTBUTTON
235 355 260 435
OK

*I ITEM_SPO_CANCEL
BtnItem Enabled
235 260 260 330
CANCEL

SetGroup 1

*I ITEM_SPO_STATIC_1
StatText Disabled
25 10 50 130
Shot Label Entry:

*I ITEM_SPO_LABEL_ENTRY_MANUAL
RadioItem Enabled
123 10 148 105
Manual

*I ITEM_SPO_LABEL_ENTRY_AUTO
RadioItem Enabled
148 10 178 105
Automatic

*I ITEM_SPO_STATIC_2
StatText Disabled
60 45 90 160
Starting Number

*I ITEM_SPO_START_EDIT
EditText Enabled
60 162 92 253
(start)

*I ITEM_SPO_STATIC_3
StatText Disabled
60 255 90 330
Increment

*I ITEM_SPO_INCREMENT_EDIT
EditText Enabled
60 332 92 423
(incre)

SetGroup 2

*I ITEM_SPO_LABEL_ORDER_ASCEND
RadioItem Enabled
123 110 148 210
Ascending

*I ITEM_SPO_LABEL_ORDER_DESCEND
RadioItem Enabled
148 110 178 210
Descending

*I ITEM_SPO_STATIC_4
StatText Disabled
95 60 123 170
Label Ordering:

SetGroup 3

*I ITEM_SPO_STATIC_5
StatText Disabled
95 255 123 395
Shot Spacing:

*I ITEM_SPO_SHOT_SPAC_MANUAL
RadioItem Enabled
123 245 148 315
Manual

*I ITEM_SPO_SHOT_SPAC_AUTO
RadioItem Enabled
148 245 178 335
Automatic

*I ITEM_SPO_SHOT_SPAC_EDIT
EditText Enabled
137 355 169 445
(spaci)

SetGroup 4

*I ITEM_SPO_STATIC_6
StatText Disabled
200 65 225 190
Shot Labels:

*I ITEM_SPO_SHOT_LABELS_NUMERIC
RadioItem Enabled
225 90 245 195
Numeric

*I ITEM_SPO_SHOT_LABELS_ARBITRARY
RadioItem Enabled
245 90 280 195
Arbitrary

,130
*N DIALOG_SIO
*S PR 0 PU 1
105 40 300 350
Set DATA Label Posting Filter
13

helpkey FINDER/MAPPING/EDIT_SEISMIC_LINES/LABELS
*I ITEM_SIO_EVERY
RadioItem Enabled
10 10 26 65
Every

*I ITEM_SIO_NUMBERS
RadioItem Enabled
45 10 61 90
Numbers

*I ITEM_SIO_NUMBER2
RadioItem Enabled
80 10 96 90
Numbers

*I ITEM_SIO_ALL
RadioItem Enabled
115 10 131 110
All

*I ITEM_SIO_NONE
RadioItem Enabled
150 10 166 110
None

*I ITEM_SIO_STATIC_1
StatText Enabled
10 110 40 205
Nth with N =

*I ITEM_SIO_NTH_EDIT
EditText Enabled
7 220 39 290
(NTH)

*I ITEM_SIO_STATIC_3
StatText Enabled
45 110 75 205
Ending with

*I ITEM_SIO_ENDING_EDIT
EditText Enabled
42 220 74 290
(END)

*I ITEM_SIO_STATIC_4
StatText Enabled
80 110 110 205
Divisible by

*I ITEM_SIO_DIVIS_EDIT
EditText Enabled
77 220 109 290
(END)

*I ITEM_SIO_OK
BtnItem Enabled
DEFAULTBUTTON
145 200 170 280
OK

*I ITEM_SIO_CANCEL
BtnItem Enabled
145 110 170 180
CANCEL

,131
*N DIALOG_CLP
*S PR 0 PU 1
41 22 370 395
Select The Best Label Position 
15

helpkey FINDER/MAPPING/EDIT_SEISMIC_LINES/LABELS

*I ITEM_CLP_STATIC_1
StatText Enabled
121 14 141 294 
Choose Label Position Around Shot Point:

*I ITEM_CLP_ABOVE_LEFT
ChkItem Enabled
154 60 180 130 
Left

*I ITEM_CLP_CENTERED_LEFT
ChkItem Disabled
185 60 211 130 
Left

*I ITEM_CLP_BELOW_LEFT
ChkItem Disabled
216 60 242 130 
Left

*I ITEM_CLP_ABOVE_CENTERED
ChkItem Disabled
154 160 180 230
Centered

*I ITEM_CLP_BELOW_CENTERED
ChkItem Disabled
216 160 242 230 
Centered

*I ITEM_CLP_ABOVE_RIGHT
ChkItem Disabled
154 250 180 320 
Right

*I ITEM_CLP_CENTERED_RIGHT
ChkItem Disabled
185 250 211 320 
Right

*I ITEM_CLP_BELOW_RIGHT
ChkItem Disabled
216 250 242 320 
Right

*I ITEM_CLP_STATIC_5
StatText Enabled
18 14 43 194
Choose Label Orientation:

*I ITEM_CLP_SUPPRESS
ChkItem Enabled
90 14 110 174 
Suppress Labels

*I ITEM_CLP_CANCEL
BtnItem Enabled
280 160 305 230 
Cancel

SetGroup 1
*I ITEM_CLP_HORIZONTAL
RadioItem Enabled
45 59 75 159 
Horizontal

*I ITEM_CLP_VERTICAL
RadioItem Enabled
45 169 75 269 
Vertical

*I ITEM_CLP_ANGLED
RadioItem Enabled
45 269 75 310 
Angled

,132
*N DIALOG_SLE
*S PR 0 PU 1
90 95 225 410
Seismic Line Editor
8
*I ITEM_SLE_STATIC_1
StatText Disabled
60 15 90 55
Line:

*I ITEM_SLE_LINE_EDIT
StatText Disabled
60 102 90 298
(Line:)

*I ITEM_SLE_STATIC_2
StatText Disabled
33 15 60 55
Map:

*I ITEM_SLE_MAP_EDIT
StatText Disabled
33 102 60 298
(Map:)

*I ITEM_SLE_STATIC_3
StatText Disabled
0 15 30 90
Project:

*I ITEM_SLE_AREA_EDIT
StatText Disabled
0 102 30 298
(Project:)

*I ITEM_SLE_STATIC_4
StatText Disabled
90 15 120 100
Shotpoint:

*I ITEM_SLE_SHOTPOINT_EDIT
StatText Disabled
90 102 120 298
(Shotpoint:)

,133
*N GET_SURVEY_NAME
*S PR 0 PU 1
80 85 285 420
Line Name
7

*I ITEM_SURV_STATIC_1
StatText Disabled
0 17 30 285
Enter the Survey Code and Line Name

*I ITEM_SURV_STATIC_2
StatText Disabled
60 17 85 113
Survey Code:

*I ITEM_SURV_STATIC_3
StatText Disabled
119 17 149 113
Line Name:

*I SLE_SURVEY_CODE   
EditText Enabled 
60 121 92 297
<Survey_Code>

*I SLE_LINE_NAME      
EditText Enabled 
116 121 148 297
<Line_Name>

*I SLE_SURV_OK
BtnItem Enabled
DEFAULTBUTTON
160 230 185 305
OK

*I SLE_SURV_CANCEL
BtnItem Enabled
160 25 185 100
CANCEL

