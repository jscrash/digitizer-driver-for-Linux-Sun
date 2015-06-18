*UX
*L FORTRAN
*T MENU
,10
*N ARRAY_MENU
*S PR 0 PU 1
Geophone_Response
4 D M
*I ARRAY_GENERATE
Generate Array Response
*I ARRAY_SUMMED
Display Summed Response
*I ARRAY_RESET
Reset Response Sum
*I ARRAY_QUIT
Quit Array Response Program

*T CDLG
,11
*N MO_ARRAY1
*S PR 0 PU 1
42 31 265 450
Seismic Array Response
13
*I MO_ARRAY1_HELP
BtnItem Enabled
185 173 210 254
HELP

*I MO_ARRAY1_OK
BtnItem Enabled
185 325 210 406
OK

SETGROUP 1

*I MO_ARRAY1_CONSTANT
RadioItem Enabled
63 24 83 175
Constant Distances

*I MO_ARRAY1_CANCEL
BtnItem Enabled
185 23 210 104
Cancel

*I MO_ARRAY1_RESPONSE_NUMBER
StatText Enabled
0 165 30 228
rn

*I MO_ARRAY1_RESPONSE_NUMBERTEXT
StatText Enabled
0 24 30 155
Response Number:

*I MO_ARRAY1_NELEMENTSTEXT
StatText Enabled
30 24 58 216
Number of Array Elements:

*I MO_ARRAY1_TITLE_TEXT
StatText Enabled
140 24 165 77
TITLE:

*I MO_ARRAY1_TITLE
EditText Enabled
138 77 170 406
tit

*I MO_ARRAY1_NELEMENTS
EditText Enabled
27 212 59 280
nel

*I MO_ARRAY1_VARYING
RadioItem Enabled
84 24 104 175
Varying Distances

*I MO_ARRAY1_CONSTANT_DISTANCETEXT
StatText Enabled
72 200 97 327
Constant Distance:

*I MO_ARRAY1_CONSTANT_DISTANCE
EditText Disabled
70 328 102 406
cd

,12
*N MO_ARRAY2
*S PR 0 PU 1
47 12 160 330
Seismic Array Response Weights
6
*I MO_ARRAY2_OK
BtnItem Enabled
70 219 95 299
OK

*I MO_ARRAY2_HELP
BtnItem Enabled
70 113 95 193
HELP

*I MO_ARRAY2_CANCEL
BtnItem Enabled
70 5 94 95
Cancel

*I MO_ARRAY2_WEIGHTTEXT
StatText Disabled
19 2 38 102
Array Weight

*I MO_ARRAY2_NOF_TEXT
StatText Disabled
19 112 35 195
01 of 48 =

*I MO_ARRAY2_WEIGHT
EditText Disabled
15 209 47 299
wt

,13
*N MO_ARRAY3
*S PR 0 PU 1
47 14 195 390
Seismic Array Response Weights and Distances
9
*I MO_ARRAY_OK
BtnItem Enabled
105 276 130 356
OK

*I MO_ARRAY3_HELP
BtnItem Enabled
105 143 130 223
HELP

*I MO_ARRAY3_CANCEL
BtnItem Enabled
105 13 130 93
Cancel

*I MO_ARRAY3_WEIGHTTEXT
StatText Disabled
7 9 36 109
Array Weight

*I MO_ARRAY3_NOF_TEXT
StatText Disabled
7 143 33 230
01 of 48 =

*I MO_ARRAY3_WEIGHT
EditText Disabled
4 266 36 356
wt

*I MO_ARRAY3_DISTANCE
EditText Disabled
51 265 83 356
dis

*I MO_ARRAY3_NOFD_TEXT
StatText Disabled
54 143 79 230
01 of 48 =

*I MO_ARRAY3_DISTANCETEXT
StatText Disabled
53 9 80 140
Array Distance

,14
*N MO_ARRAY_DIALOG
*S PR 0 PU 1
100 100 183 368
Array Response
1

*I MO_ARRAY_TEXT
StatText Enabled
10 20 75 265
Array Response Program

*UM
*L FORTRAN
*T MENU
,10
*N ARRAY_MENU
*S PR 0 PU 1
Geophone_Response
4 D M
*I ARRAY_GENERATE
Generate Array Response
*I ARRAY_SUMMED
Display Summed Response
*I ARRAY_RESET
Reset Response Sum
*I ARRAY_QUIT
Quit Array Response Program

*T CDLG
,11
*N MO_ARRAY1
*S PR 0 PU 1
42 31 222 445
Seismic Array Response
13
*I MO_ARRAY1_HELP
BtnItem Enabled
133 129 161 210
HELP

*I MO_ARRAY1_OK
BtnItem Enabled
133 237 161 318
OK

SETGROUP 1

*I MO_ARRAY1_CONSTANT
RadioItem Enabled
43 24 63 175
Constant Distances

*I MO_ARRAY1_CANCEL
BtnItem Enabled
133 23 161 104
Cancel

*I MO_ARRAY1_RESPONSE_NUMBER
StatText Enabled
5 165 20 228
rn

*I MO_ARRAY1_RESPONSE_NUMBERTEXT
StatText Enabled
5 24 22 155
Response Number:

*I MO_ARRAY1_NELEMENTSTEXT
StatText Enabled
22 24 40 216
Number of Array Elements:

*I MO_ARRAY1_TITLE_TEXT
StatText Enabled
95 24 112 70
TITLE:

*I MO_ARRAY1_TITLE
EditText Enabled
92 77 115 406
tit

*I MO_ARRAY1_NELEMENTS
EditText Enabled
21 212 39 280
nel

*I MO_ARRAY1_VARYING
RadioItem Enabled
64 24 84 175
Varying Distances

*I MO_ARRAY1_CONSTANT_DISTANCETEXT
StatText Enabled
47 261 84 327
Constant Distance:

*I MO_ARRAY1_CONSTANT_DISTANCE
EditText Disabled
52 328 73 406
cd

,12
*N MO_ARRAY2
*S PR 0 PU 1
47 12 160 338
Seismic Array Response Weights
6
*I MO_ARRAY2_OK
BtnItem Enabled
62 219 94 299
OK

*I MO_ARRAY2_HELP
BtnItem Enabled
62 113 94 193
HELP

*I MO_ARRAY2_CANCEL
BtnItem Enabled
62 5 94 85
Cancel

*I MO_ARRAY2_WEIGHTTEXT
StatText Disabled
19 2 35 102
Array Weight

*I MO_ARRAY2_NOF_TEXT
StatText Disabled
19 112 35 195
01 of 48 =

*I MO_ARRAY2_WEIGHT
EditText Disabled
16 209 38 287
wt

,13
*N MO_ARRAY3
*S PR 0 PU 1
47 14 191 363
Seismic Array Response Weights and Distances
9
*I MO_ARRAY_OK
BtnItem Enabled
94 226 126 306
OK

*I MO_ARRAY3_HELP
BtnItem Enabled
94 118 126 198
HELP

*I MO_ARRAY3_CANCEL
BtnItem Enabled
94 13 126 93
Cancel

*I MO_ARRAY3_WEIGHTTEXT
StatText Disabled
17 9 33 109
Array Weight

*I MO_ARRAY3_NOF_TEXT
StatText Disabled
17 117 33 200
01 of 48 =

*I MO_ARRAY3_WEIGHT
EditText Disabled
14 216 36 294
wt

*I MO_ARRAY3_DISTANCE
EditText Disabled
51 215 73 293
dis

*I MO_ARRAY3_NOFD_TEXT
StatText Disabled
54 115 70 198
01 of 48 =

*I MO_ARRAY3_DISTANCETEXT
StatText Disabled
53 9 69 109
Array Distance

,14
*N MO_ARRAY_DIALOG
*S PR 0 PU 1
100 100 183 368
Array Response
1

*I MO_ARRAY_TEXT
StatText Enabled
10 20 75 265
Array Response Program

