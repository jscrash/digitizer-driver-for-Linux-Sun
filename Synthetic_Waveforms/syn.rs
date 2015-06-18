*UX
*L ALL
*T MENU
,1
*N SYNTH1
*S PR 0 PU 1
Syn.Def
7 D M
*I MENU_INTC
Select Curves to Display in Time
*I MENU_GENERAL
Synthetics Options Definition
*I MENU_ALTDEN
Define Alternate Density Source
*I MENU_ACOUS
Filtered Acoustic Impedance Logs
*I MENU_VELANOTHER
Velocity Survey of Another Well
*I MENU_RUNIT
Perform Integration Stage
*I MENU_QUIT
Quit Synthetics Program




,2
*N SYNTH2
*S PR 0 PU 1
Trace.Def
4 D M
*I MENU2_COSINE
Define Half Cycle Cosine Wavelet
*I MENU2_RICKER
Define Ricker Wavelet
*I MENU2_RUNIT
Generate Synthetic Traces
*I MENU2_QUIT
Quit Synthetics Program





*T CDLG
,1
*N GENERAL
*S PR 0 PU 1
43 34 380 507
General Setup Parameters
17

*I GENERAL_MULTI
ChkItem Enabled
25 22 40 246
Run Multiple Generator

*I GENERAL_MULTI_COEF_PROMPT
StatText Enabled
0 312 30 437
Surface Reflection

*I GENERAL_MULTI_COEF
EditText Enabled
35 344 67 403
1.

*I GENERAL_AGC
ChkItem Enabled
90 21 110 245
Run Automatic Gain Control

*I GENERAL_AGC_LENGTH_PROMPT
StatText Enabled
80 287 108 465
Length of AGC Window

*I GENERAL_AGC_LENGTH
EditText Enabled
105 344 137 403
50

*I GENERAL_PRINT_TD
ChkItem Enabled
150 21 170 290
Generate Printed Time/Depth Report

*I GENERAL_NFILT_PROMPT
StatText Enabled
190 23 215 277
Number of Filter Series to Generate

*I GENERAL_NFILT
EditText Enabled
185 284 217 310
1

*I GENERAL_DATUM_PROMPT
StatText Enabled
225 25 250 131
Seismic Datum

*I GENERAL_DATUM
EditText Enabled
220 137 252 206
<Dat>

*I GENERAL_VELOC_PROMPT
StatText Enabled
225 225 250 350
Surface Velocity

*I GENERAL_VELOC
EditText Enabled
220 360 252 425
<Vel>

*I GENERAL_AVGVEL_PROMPT
StatText Enabled
255 25 280 355
Average Velocity for First 50 Feet of Log:

*I GENERAL_AVGVEL
StatText Disabled
255 360 280 410


*I GENERAL_CANCEL
BtnItem Disabled
290 30 315 150
Cancel

*I GENERAL_OK
BtnItem Disabled
DefaultButton
290 320 315 440
Proceed





,2
*N COSINE
*S PR 0 PU 1
34 56 320 448
Half Cycle Cosine Wavelet  Definition
22
*I COSINE_OK
BtnItem Enabled
225 25 250 125
OK

*I COSINE_HELP
BtnItem Enabled
225 270 250 350
HELP

*I COSINE_DRAW
ChkItem Enabled
192 25 212 100
DRAW

*I COSINE_CANCEL
BtnItem Enabled
225 150 250 245
CANCEL

*I COSINE_LENGTH_TAPER_PROMPT
StatText Enabled
165 22 192 150
Length Taper Zone

*I COSINE_LENGTH_TAPER
EditText Enabled
162 150 194 230
100

*I COSINE_WAY_MESSAGE
StatText Enabled
255 25 283 280
All Times Two  Way Time, Milliseconds

*I COSINE_MINF_PROMPT
StatText Enabled
10 28 52 108
Minimum Frequency

*I COSINE_MAXF_PROMPT
StatText Enabled
10 160 52 240
Maximum Frequency

*I COSINE_MINF_1
EditText Enabled
55 25 87 125
MIN_FR_1

*I COSINE_HIGHTIME
StatText Disabled
24 285 55 325
High

*I COSINE_TIME_PROMPT
StatText Disabled
0 270 25 370
Time Gate

*I COSINE_PHASE_PROMPT
StatText Enabled
165 240 190 300
Phase

*I COSINE_PHASE
EditText Enabled
162 292 194 350
0

*I COSINE_MINF_2
EditText Enabled
90 25 122 125
MIN_FR_2

*I COSINE_MINF_3
EditText Enabled
125 25 157 125
MIN_FR_3

*I COSINE_TIME_2
EditText Enabled
90 270 122 350
TIME_2

*I COSINE_TIME_1
EditText Enabled
55 270 87 350
TIME_1

*I COSINE_MAXF_1
EditText Enabled
55 150 87 245
MAX_FR_1

*I COSINE_MAXF_2
EditText Enabled
90 150 122 245
MAX_FR_2

*I COSINE_MAXF_3
EditText Enabled
125 150 157 245
MAX_FR_3

*I COSINE_TIME_3
EditText Enabled
125 270 157 350
TIME_3






,3
*N ACOUS
*S PR 0 PU 1
100 100 255 452
Filtered Acoustic Impedence Logs
7

*I ACOUS_CALC
ChkItem Enabled
0 15 30 310
Run Filtered Acoustic Impedence Logs

*I ACOUS_MIN_FREQ_PROMPT
StatText Enabled
38 13 65 188
Minimum Frequency

*I ACOUS_MAX_FREQ_PROMPT
StatText Enabled
72 13 100 163
Maximum Frequency

*I ACOUS_MIN_FREQ
EditText Enabled
35 161 67 250
<Min_F>

*I ACOUS_MAX_FREQ
EditText Enabled
70 161 102 250
<Max_F>

*I ACOUS_OK
BtnItem Enabled
DefaultButton
110 231 135 321
Proceed

*I ACOUS_CANCEL
BtnItem Enabled
110 31 135 119
Cancel








,4
*N KELLY
*S PR 0 PU 1
65 57 250 443
Estimated Kelly Bushing Elevation
8

*I KELLY_NO_KELLY
StatText Disabled
4 17 34 306
Elevation of Kelly Bushing Not Available.

*I KELLY_ESTIMATED
ChkItem Disabled
41 19 61 322
Use Estimated Elevation of Kelly Bushing

*I KELLY_ELEVATION_PROMPT
StatText Enabled
83 19 113 188
Estimated Kelly Bushing

*I KELLY_ELEVATION
EditText Enabled
79 187 111 265
<Elev>

*I KELLY_FEET
RadioItem Enabled
93 288 109 388
Feet

*I KELLY_METERS
RadioItem Enabled
72 288 88 388
Meters

SetGroup 1

*I KELLY_OK
BtnItem Enabled
DefaultButton
140 275 165 355
Proceed

*I KELLY_CANCEL
BtnItem Enabled
140 51 165 131
Cancel







,5
*N ALTDEN
*S PR 0 PU 1
57 58 310 370
Alternate Density Log Sources
9


SETGROUP 1

*I ALTDEN_NO_CALC_DEN
RadioItem Enabled
28 23 44 322
Do Not Use an Alternate Density Source

*I ALTDEN_CALC_DEN_VEL
RadioItem Enabled
54 23 70 414
Run Approximate Density from Velocity

*I ALTDEN_CALC_DEN_FDCCNL
RadioItem Enabled
78 23 93 226
Run Density from FDC-CNL

*I ALTDEN_MATRIX_PROMPT
StatText Enabled
125 23 155 218
Density of Matrix Material

*I ALTDEN_MATRIX
EditText Enabled
125 208 157 284
2.68

*I ALTDEN_FLUID_PROMPT
StatText Enabled
160 23 190 215
Density of Drilling Fluid


*I ALTDEN_FLUID
EditText Enabled
160 209 192 284
1.0

*I ALTDEN_OK
BtnItem Enabled
DefaultButton
205 194 230 284
Proceed

*I ALTDEN_CANCEL
BtnItem Enabled
205 18 230 108
Cancel







,6
*N RICKER
52 60 380 410
Ricker Wavelet Definition
20
*I RICKER_OK
BtnItem Enabled
245 15 270 89
OK

*I RICKER_HELP
BtnItem Enabled
245 245 270 325
HELP

*I RICKER_DRAW
ChkItem Enabled
190 270 210 340
DRAW

SETGROUP 1

*I RICKER_TYPE1
RadioItem Disabled
35 10 57 90
Type 1

*I RICKER_TYPE2
RadioItem Disabled
60 10 82 85
Type 2

*I RICKER_TYPE3
RadioItem Disabled
85 10 107 90
Type 3

*I RICKER_TYPE4
RadioItem Disabled
113 9 135 89
Type 4

*I RICKER_PREDFREQ_PROMPT
StatText Enabled
0 110 50 200
Predominant Frequencies

*I RICKER_HIGHTIME
StatText Disabled
24 264 55 320
High

*I RICKER_FREQ_1
EditText Enabled
55 116 87 190
FREQ_1

*I RICKER_CANCEL
BtnItem Enabled
245 131 270 202
CANCEL

*I RICKER_LENGTH_TAPER_PROMPT
StatText Enabled
185 10 215 160
Length Taper Zone

*I RICKER_LENGTH_TAPER
EditText Enabled
185 137 217 218
100

*I RICKER_WAY_PROMPT
StatText Enabled
285 5 310 170
All Times Two  Way Time

*I RICKER_TIME_PROMPT
StatText Enabled
0 250 25 340
Time Gate

*I RICKER_TIME_1
EditText Enabled
55 249 87 325
TIME_1

*I RICKER_FREQ_2
EditText Enabled
90 116 122 190
FREQ_2

*I RICKER_TIME_2
EditText Enabled
90 249 122 325
TIME_2

*I RICKER_FREQ_3
EditText Enabled
125 116 157 190
FREQ_3

*I RICKER_TIME_3
EditText Enabled
125 249 157 325
TIME_3






,7
*N VELANOTHER
*S PR 0 PU 1
65 114 180 467
Velocity Survey from Another Well
4

*I VELANOTHER_NO_EXIST
StatText Enabled
0 21 28 351
Velocity Survey Does Not Exist for Current Well

*I VELANOTHER_USE
ChkItem Disabled
33 15 49 339
Use a Velocity Survey from Another Well

*I VELANOTHER_OK
BtnItem Enabled
Defaultbutton
59 245 91 325
Proceed

*I VELANOTHER_CANCEL
BtnItem Enabled
59 41 91 121
Cancel








,8
*N FAUST
*S PR 0 PU 1
65 62 210 370
Velocity Using Faust Method
5

*I FAUST_CONSTANT_PROMPT
StatText Enabled
64 24 82 131
Faust Constant

*I FAUST_CONSTANT
EditText Enabled
60 143 92 210
1948.

*I FAUST_CALC_VEL
ChkItem Enabled
16 25 33 360
Calculate Velocity Using Faust Method

*I FAUST_OK
BtnItem Enabled
105 205 130 285
Proceed

*I FAUST_CANCEL
BtnItem Enabled
105 27 130 108
Cancel








,9
*N WAVELET
*S PR 0 PU 1
61 58 217 360
Wavelet Types
6

*I WAVELET_PROMPT
StatText Disabled
6 80 36 300
Enter type of wavelet.

SETGROUP 1

*I WAVELET_RICKER
RadioItem Enabled
42 90 62 290
Ricker Wavelet

*I WAVELET_OK
BtnItem Enabled
120 14 145 94
OK

*I WAVELET_HELP
BtnItem Enabled
120 205 145 285
HELP

*I WAVELET_CANCEL
BtnItem Enabled
120 104 145 184
CANCEL

*I WAVELET_COSINE
RadioItem Enabled
69 90 89 290
Half Cycle Cosine







,10
*N STRAIGHT
*S PR 0 PU 1
52 12 255 396
No Velocity Curve Selected
7

*I STRAIGHT_EXPLAIN
StatText Enabled
0 11 30 370
A Velocity Curve Has Not Been Selected for Current Well

SETGROUP 1

*I STRAIGHT_EXIT
RadioItem Enabled
40 17 70 141
Exit Program!

*I STRAIGHT_USE
RadioItem Enabled
70 16 100 340
Use a Straight Line Velocity Function


*I STRAIGHT_VEL_PROMPT
StatText Enabled
115 14 145 89
Velocity =

*I STRAIGHT_VEL
EditText Disabled
115 85 147 200
<Velocity>

*I STRAIGHT_OK
BtnItem Enabled
DefaultButton
155 251 180 331
Proceed

*I STRAIGHT_CANCEL
BtnItem Enabled
155 44 180 124
Cancel






,11
*N HNGALERT
*S PR 0 PU 1
120 17 221 500
SIS ALERT
2
*I HNGALERT_ERROR
StatText Enabled
0 13 60 471


*I HNGALERT_OK
BtnItem Disabled
65 191 90 293
OK







,12
*N SYNPLOT
*S PR 0 PU 1
37 17 370 486
Synthetic Plot Definition
19


*I SYNPLOT_WELLTEXT
StatText Enabled
30 0 60 82
Well Name:

*I SYNPLOT_UWITEXT
StatText Enabled
70 0 100 75
U.W.I.:

*I SYNPLOT_WELL
EditText Disabled
28 90 60 430
well

*I SYNPLOT_UWI
EditText Disabled
68 90 100 430
uwi

*I SYNPLOT_TPITEXT
StatText Enabled
145 0 175 139
Traces per Inch

*I SYNPLOT_PLOTSPTEXT
StatText Enabled
180 0 210 150
Inches per / Second

*I SYNPLOT_NTRACESTEXT
StatText Enabled
230 0 260 218
Number of Traces per Synthetic

*I SYNPLOT_PLOTSPD
EditText Enabled
180 153 212 205
IPS

*I SYNPLOT_NTRACES
EditText Enabled
230 226 262 275
ntr

SETGROUP 1

*I SYNPLOT_VARIABLE
RadioItem Disabled
215 307 245 451
Variable Area Trace

*I SYNPLOT_WIGGLE
RadioItem Disabled
245 307 275 419
Wiggle Trace

*I SYNPLOT_TPI
EditText Enabled
145 152 177 205
tpi

*I SYNPLOT_COMMENT
EditText Disabled
105 90 142 430
com

*I SYNPLOT_COMMENTTEXT
StatText Enabled
110 0 140 85
Comments:


*I SYNPLOT_SCALE
EditText Enabled
145 338 177 430
scal

*I SYNPLOT_SCALETEXT
StatText Enabled
145 216 175 335
Amplitude Scalar


*I SYNPLOT_RESLOG
ChkItem Enabled
185 217 205 468
Plot Resistivity Logs on a Log Scale

*I SYNPLOT_CANCEL
BtnItem Enabled
285 30 310 130
Cancel

*I SYNPLOT_OK
BtnItem Enabled
DefaultButton
285 340 310 440
Proceed





,13
*N SYN_MAIN_DIALOG
*S PR 0 PU 1
100 100 183 368
Synthetics
1

*I SYN_MAIN_TEXT
StatText Enabled
10 20 75 265
Synthetics Program









                                                                   
*UM
*L ALL
*T MENU
,1
*N SYNTH1
*S PR 0 PU 1
Syn.Def
7 D M
*I MENU_INTC
Select Curves to Display in Time
*I MENU_GENERAL
Synthetics Options Definition
*I MENU_ALTDEN
Define Alternate Density Source
*I MENU_ACOUS
Filtered Acoustic Impedance Logs
*I MENU_VELANOTHER
Velocity Survey of Another Well
*I MENU_RUNIT
Perform Integration Stage
*I MENU_QUIT
Quit Synthetics Program




,2
*N SYNTH2
*S PR 0 PU 1
Trace.Def
4 D M
*I MENU2_COSINE
Define Half Cycle Cosine Wavelet
*I MENU2_RICKER
Define Ricker Wavelet
*I MENU2_RUNIT
Generate Synthetic Traces
*I MENU2_QUIT
Quit Synthetics Program





*T CDLG
,1
*N GENERAL
*S PR 0 PU 1
43 34 332 507
General Setup Parameters
17

*I GENERAL_MULTI
ChkItem Enabled
25 22 40 246
Run Multiple Generator

*I GENERAL_MULTI_COEF_PROMPT
StatText Enabled
10 312 26 437
Surface Reflection

*I GENERAL_MULTI_COEF
EditText Enabled
30 344 51 403
1.

*I GENERAL_AGC
ChkItem Enabled
67 21 82 245
Run Automatic Gain Control

*I GENERAL_AGC_LENGTH_PROMPT
StatText Enabled
57 287 75 479
Length of AGC Window

*I GENERAL_AGC_LENGTH
EditText Enabled
75 346 95 379
50

*I GENERAL_PRINT_TD
ChkItem Enabled
109 21 124 290
Generate Printed Time/Depth Report

*I GENERAL_NFILT_PROMPT
StatText Enabled
144 23 161 277
Number of Filter Series to Generate

*I GENERAL_NFILT
EditText Enabled
142 284 164 310
1

*I GENERAL_DATUM_PROMPT
StatText Enabled
176 25 194 131
Seismic Datum

*I GENERAL_DATUM
EditText Enabled
173 137 194 206

*I GENERAL_VELOC_PROMPT
StatText Enabled
176 225 194 350
Surface Velocity

*I GENERAL_VELOC
EditText Enabled
173 360 194 410


*I GENERAL_AVGVEL_PROMPT
StatText Enabled
205 25 222 355
Average Velocity for First 50 Feet of Log:

*I GENERAL_AVGVEL
StatText Disabled
205 360 222 410


*I GENERAL_CANCEL
BtnItem Disabled
245 30 280 150
Cancel

*I GENERAL_OK
BtnItem Disabled
DefaultButton
245 320 280 440
Proceed





,2
*N COSINE
*S PR 0 PU 1
34 56 315 448
Half Cycle Cosine Wavelet  Definition
22
*I COSINE_OK
BtnItem Enabled
205 7 233 88
OK

*I COSINE_HELP
BtnItem Enabled
205 208 233 289
HELP

*I COSINE_DRAW
ChkItem Enabled
172 012 192 081
DRAW

*I COSINE_CANCEL
BtnItem Enabled
205 107 233 188
CANCEL

*I COSINE_LENGTH_TAPER_PROMPT
StatText Enabled
145 10 167 160
Length Taper Zone

*I COSINE_LENGTH_TAPER
EditText Enabled
142 137 165 218
100

*I COSINE_WAY_MESSAGE
StatText Enabled
250 5 268 260
All Times Two  Way Time, Milliseconds

*I COSINE_MINF_PROMPT
StatText Enabled
5 25 36 105
Minimum Frequency

*I COSINE_MAXF_PROMPT
StatText Enabled
4 115 35 195
Maximum Frequency

*I COSINE_MINF_1
EditText Enabled
40 26 66 94
MIN FR 1

*I COSINE_HIGHTIME
StatText Disabled
19 242 40 277
High

*I COSINE_TIME_PROMPT
StatText Disabled
4 224 20 324
Time Gate

*I COSINE_PHASE_PROMPT
StatText Enabled
145 240 161 300
Phase

*I COSINE_PHASE
EditText Enabled
142 292 164 353
0

*I COSINE_MINF_2
EditText Enabled
67 26 93 94
MIN FR 2

*I COSINE_MINF_3
EditText Enabled
94 26 120 94
MIN FR 3

*I COSINE_TIME_2
EditText Enabled
67 219 93 287
TIME 2

*I COSINE_TIME_1
EditText Enabled
40 219 66 287
TIME 1

*I COSINE_MAXF_1
EditText Enabled
40 113 66 181
MAX FR 1

*I COSINE_MAXF_2
EditText Enabled
67 113 93 181
MAX FR 2

*I COSINE_MAXF_3
EditText Enabled
94 113 120 181
MAX FR 3

*I COSINE_TIME_3
EditText Enabled
94 219 120 287
TIME 3






,3
*N ACOUS
*S PR 0 PU 1
100 100 247 452
Filtered Acoustic Impedence Logs
7

*I ACOUS_CALC
ChkItem Enabled
10 15 26 310
Run Filtered Acoustic Impedence Logs

*I ACOUS_MIN_FREQ_PROMPT
StatText Enabled
38 13 54 188
Minimum Frequency

*I ACOUS_MAX_FREQ_PROMPT
StatText Enabled
67 13 83 163
Maximum Frequency

*I ACOUS_MIN_FREQ
EditText Enabled
33 161 55 237

*I ACOUS_MAX_FREQ
EditText Enabled
62 161 84 237

*I ACOUS_OK
BtnItem Enabled
DefaultButton
107 231 130 321
Proceed

*I ACOUS_CANCEL
BtnItem Enabled
107 31 130 119
Cancel








,4
*N KELLY
*S PR 0 PU 1
65 57 224 443
Estimated Kelly Bushing Elevation
8

*I KELLY_NO_KELLY
StatText Disabled
14 17 32 306
Elevation of Kelly Bushing Not Available.

*I KELLY_ESTIMATED
ChkItem Disabled
41 19 61 322
Use Estimated Elevation of Kelly Bushing

*I KELLY_ELEVATION_PROMPT
StatText Enabled
83 19 98 188
Estimated Kelly Bushing

*I KELLY_ELEVATION
EditText Enabled
79 187 101 265

*I KELLY_FEET
RadioItem Enabled
93 288 109 388
Feet

*I KELLY_METERS
RadioItem Enabled
72 288 88 388
Meters

SetGroup 1

*I KELLY_OK
BtnItem Enabled
DefaultButton
118 275 150 355
Proceed

*I KELLY_CANCEL
BtnItem Enabled
118 51 150 131
Cancel







,5
*N ALTDEN
*S PR 0 PU 1
57 58 279 410
Alternate Density Log Sources
9


SETGROUP 1

*I ALTDEN_NO_CALC_DEN
RadioItem Enabled
28 23 44 322
Do Not Use an Alternate Density Source

*I ALTDEN_CALC_DEN_VEL
RadioItem Enabled
54 23 70 414
Run Approximate Density from Velocity

*I ALTDEN_CALC_DEN_FDCCNL
RadioItem Enabled
78 23 93 226
Run Density from FDC-CNL

*I ALTDEN_MATRIX_PROMPT
StatText Enabled
111 23 130 218
Density of Matrix Material

*I ALTDEN_MATRIX
EditText Enabled
108 208 134 254
2.68

*I ALTDEN_FLUID_PROMPT
StatText Enabled
144 23 161 215
Density of Drilling Fluid


*I ALTDEN_FLUID
EditText Enabled
139 209 166 256
1.0

*I ALTDEN_OK
BtnItem Enabled
DefaultButton
183 244 215 334
Proceed

*I ALTDEN_CANCEL
BtnItem Enabled
183 18 215 108
Cancel







,6
*N RICKER
52 60 330 452
Ricker Wavelet Definition
20
*I RICKER_OK
BtnItem Enabled
202 8 230 89
OK

*I RICKER_HELP
BtnItem Enabled
202 209 230 290
HELP

*I RICKER_DRAW
ChkItem Enabled
142 270 160 340
DRAW

SETGROUP 1

*I RICKER_TYPE1
RadioItem Disabled
35 10 57 90
Type 1

*I RICKER_TYPE2
RadioItem Disabled
60 10 82 85
Type 2

*I RICKER_TYPE3
RadioItem Disabled
85 10 107 90
Type 3

*I RICKER_TYPE4
RadioItem Disabled
113 9 135 89
Type 4

*I RICKER_PREDFREQ_PROMPT
StatText Enabled
3 110 34 200
Predominant Frequencies

*I RICKER_HIGHTIME
StatText Disabled
22 264 43 299
High

*I RICKER_FREQ_1
EditText Enabled
39 116 65 181
FREQ 1

*I RICKER_CANCEL
BtnItem Enabled
202 104 230 185
CANCEL

*I RICKER_LENGTH_TAPER_PROMPT
StatText Enabled
145 10 167 160
Length Taper Zone

*I RICKER_LENGTH_TAPER
EditText Enabled
142 137 165 218
100

*I RICKER_WAY_PROMPT
StatText Enabled
250 5 267 170
All Times Two  Way Time

*I RICKER_TIME_PROMPT
StatText Enabled
7 250 23 322
Time Gate

*I RICKER_TIME_1
EditText Enabled
39 249 65 314
TIME 1

*I RICKER_FREQ_2
EditText Enabled
67 116 93 181
FREQ 2

*I RICKER_TIME_2
EditText Enabled
67 249 93 314
TIME 2

*I RICKER_FREQ_3
EditText Enabled
95 116 121 181
FREQ 3

*I RICKER_TIME_3
EditText Enabled
95 249 121 314
TIME 3






,7
*N VELANOTHER
*S PR 0 PU 1
97 114 214 467
Velocity Survey from Another Well
5

*I VELANOTHER_NO_EXIST
StatText Enabled
9 21 28 351
Velocity Survey Does Not Exist for Current Well

*I VELANOTHER_USE
ChkItem Disabled
33 15 49 339
Use a Velocity  Survey from Another Well

*I VELANOTHER_OK
BtnItem Enabled
Defaultbutton
59 245 91 325
Proceed

*I VELANOTHER_CANCEL
BtnItem Enabled
59 41 91 121
Cancel








,8
*N FAUST
*S PR 0 PU 1
65 62 217 420
Velocity Using Faust Method
5

*I FAUST_CONSTANT_PROMPT
StatText Enabled
64 24 82 131
Faust Constant

*I FAUST_CONSTANT
EditText Enabled
60 143 82 192
1948.

*I FAUST_CALC_VEL
ChkItem Enabled
16 25 33 357
Calculate Velocity Using Faust Method

*I FAUST_OK
BtnItem Enabled
99 253 131 334
Proceed

*I FAUST_CANCEL
BtnItem Enabled
99 27 131 108
Cancel








,9
*N WAVELET
*S PR 0 PU 1
61 58 217 360
Wavelet Types
6

*I WAVELET_PROMPT
StatText Disabled
16 17 34 306
Enter type of wavelet.

SETGROUP 1

*I WAVELET_RICKER
RadioItem Enabled
42 24 62 327
Ricker Wavelet

*I WAVELET_OK
BtnItem Enabled
117 14 149 94
OK

*I WAVELET_HELP
BtnItem Enabled
117 205 149 285
HELP

*I WAVELET_CANCEL
BtnItem Enabled
116 104 148 184
CANCEL

*I WAVELET_COSINE
RadioItem Enabled
69 24 89 327
Half Cycle Cosine







,10
*N STRAIGHT
*S PR 0 PU 1
52 12 207 396
No Velocity Curve Selected
7

*I STRAIGHT_EXPLAIN
StatText Enabled
5 11 24 405
A Velocity Curve Has Not Been Selected for Current Well

SETGROUP 1

*I STRAIGHT_EXIT
RadioItem Enabled
29 17 45 141
Exit Program!

*I STRAIGHT_USE
RadioItem Enabled
51 16 67 340
Use a Straight Line Velocity Function

*I STRAIGHT_VEL_PROMPT
StatText Enabled
77 14 94 89
Velocity =

*I STRAIGHT_VEL
EditText Disabled
75 85 97 191

*I STRAIGHT_OK
BtnItem Enabled
DefaultButton
111 251 143 331
Proceed

*I STRAIGHT_CANCEL
BtnItem Enabled
110 44 142 124
Cancel






,11
*N HNGALERT
*S PR 0 PU 1
120 17 221 500
SIS ALERT
2
*I HNGALERT_ERROR
StatText Enabled
10 13 32 471


*I HNGALERT_OK
BtnItem Disabled
69 191 97 293
OK







,12
*N SYNPLOT
*S PR 0 PU 1
37 17 297 486
Synthetic Plot Definition
19


*I SYNPLOT_WELLTEXT
StatText Enabled
4 3 21 79
Well Name:

*I SYNPLOT_UWITEXT
StatText Enabled
30 4 48 75
U.W.I.:

*I SYNPLOT_WELL
EditText Disabled
3 83 26 424
well

*I SYNPLOT_UWI
EditText Disabled
25 83 52 424
uwi

*I SYNPLOT_TPITEXT
StatText Enabled
103 6 120 139
Traces per Inch

*I SYNPLOT_PLOTSPTEXT
StatText Enabled
138 6 155 150
Inches per / Second

*I SYNPLOT_NTRACESTEXT
StatText Enabled
173 3 191 218
Number of  Traces per Synthetic

*I SYNPLOT_PLOTSPD
EditText Enabled
134 153 155 192
IPS

*I SYNPLOT_NTRACES
EditText Enabled
170 226 193 267
ntr

SETGROUP 1

*I SYNPLOT_VARIABLE
RadioItem Disabled
151 307 169 461
Variable Area Trace

*I SYNPLOT_WIGGLE
RadioItem Disabled
170 307 187 419
Wiggle Trace

*I SYNPLOT_TPI
EditText Enabled
100 152 123 192
tpi

*I SYNPLOT_COMMENT
EditText Disabled
51 83 78 424
com

*I SYNPLOT_COMMENTTEXT
StatText Enabled
57 2 75 80
Comments:


*I SYNPLOT_SCALE
EditText Enabled
98 338 121 378
scal

*I SYNPLOT_SCALETEXT
StatText Enabled
104 216 121 335
Amplitude Scalar


*I SYNPLOT_RESLOG
ChkItem Enabled
127 217 143 468
Plot Resistivity Logs on a Log Scale

*I SYNPLOT_CANCEL
BtnItem Enabled
208 30 245 130
Cancel

*I SYNPLOT_OK
BtnItem Enabled
DefaultButton
208 340 245 440
Proceed





,13
*N SYN_MAIN_DIALOG
*S PR 0 PU 1
100 100 183 368
Synthetics
1

*I SYN_MAIN_TEXT
StatText Enabled
10 20 75 265
Synthetics Program









                                                                   
