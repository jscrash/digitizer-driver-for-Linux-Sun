*L ALL
*T CDLG
,12
*N MODPLOT
*S PR 0 PU 1
37 17 370 486
Trace Plot Definition
20

*I MODPLOT_CANCEL
BtnItem Enabled
285 35 310 116
Cancel

*I MODPLOT_OK
BtnItem Disabled
DEFAULTBUTTON
285 358 310 440
Proceed

*I MODPLOT_TITLE1TEXT
StatText Enabled
5 3 35 79
Title 1

*I MODPLOT_TITLE2TEXT
StatText Enabled
45 4 75 75
Title 2

*I MODPLOT_TITLE1
EditText Disabled
5 90 37 430
t1

*I MODPLOT_TITLE2
EditText Disabled
45 90 77 430
t2

*I MODPLOT_TPITEXT
StatText Enabled
125 6 155 159
Traces per (Inch-cm)

*I MODPLOT_PLOTSPTEXT
StatText Enabled
165 6 195 155
(Inch-cm) per Second

*I MODPLOT_PLOTSPD
EditText Enabled
165 160 197 210
IPS

SETGROUP 1

*I MODPLOT_NORMAL
RadioItem Disabled
210 6 225 145
Normal Polarity

*I MODPLOT_REVERSE
RadioItem Disabled
230 6 255 145
Reverse Polarity

SETGROUP 2

*I MODPLOT_ENGLISH
RadioItem Disabled
210 155 225 300
English UOM Scales

*I MODPLOT_METRIC
RadioItem Disabled
230 155 255 300
Metric UOM Scales


SETGROUP 3

*I MODPLOT_VARIABLE
RadioItem Disabled
210 310 225 461
Variable Area Trace

*I MODPLOT_WIGGLE
RadioItem Disabled
230 310 255 419
Wiggle Trace

*I MODPLOT_TPI
EditText Enabled
125 160 157 210
tpi

*I MODPLOT_COMMENT
EditText Disabled
85 90 117 430
com

*I MODPLOT_COMMENTTEXT
StatText Enabled
85 2 115 89
Comments:


*I MODPLOT_SCALE
EditText Enabled
125 350 157 405
scal

*I MODPLOT_SCALETEXT
StatText Enabled
125 225 155 345
Amplitude Scalar




,13
*N TIEPLOT
*S PR 0 PU 1
38 28 420 460
Model Display Parameters
19

*I TIEPLOT_RIGHT
EditText Enabled
80 282 112 364
rs

*I TIEPLOT_LEFT
EditText Enabled
80 72 112 154
ls

*I TIEPLOT_LEFTTEXT
StatText Enabled
50 62 76 181
Left Sonic Scale

*I TIEPLOT_RIGHTTEXT
StatText Disabled
50 263 76 399
Right Sonic Scale

*I TIEPLOT_TITLETEXT
StatText Disabled
5 14 35 63
Title:

*I TIEPLOT_TITLE
EditText Enabled
5 64 37 392
ti

*I TIEPLOT_CANCEL
BtnItem Disabled
335 50 360 143
Cancel

*I TIEPLOT_OK
BtnItem Disabled
DEFAULTBUTTON
335 281 360 374
Proceed

*I TIEPLOT_FPITEXT
StatText Enabled
130 40 157 235
Depth Scale Units

*I TIEPLOT_SCTEXT
StatText Enabled
175 205 205 295
Scale Value

*I TIEPLOT_SCVALUE
EditText Enabled
175 302 207 384
fpi

*I TIEPLOT_DECIM_TEXT
StatText Enabled
255 220 283 355
Log Plot Dec. Factor

*I TIEPLOT_DECIM_TEXT2
StatText Enabled
280 220 310 365
(Draw every n pts)

*I TIEPLOT_DECIM_VALUE
EditText Enabled
265 362 297 400

SETGROUP 1

*I TIEPLOT_ENGLISH
RadioItem Enabled
160 46 185 150
Feet/Inch

*I TIEPLOT_METRIC
RadioItem Disabled
190 46 215 150
Metres/Cm

SETGROUP 2

*I TIEPLOT_SON_ONLY
RadioItem Disabled
235 20 260 190
Draw Sonic Only

*I TIEPLOT_DENSITY
RadioItem Enabled
265 20 290 190
Draw Sonic and Density

*I TIEPLOT_AC_IMP
RadioItem Disabled
295 20 320 190
Draw Sonic and Ac. Imp.



,14
*N MODSEGY
*S PR 0 PU 1
63 63 257 423
Create Model SIS File
7

*I MODSEGY_LINETEXT
StatText Enabled
93 23 113 111
Line name

*I MODSEGY_LINE
EditText Enabled
90 112 122 339
line

*I MODSEGY_CREATE
ChkItem Enabled
51 23 75 355
Create a SIS format trace file of this model

*I MODSEGY_OK
BtnItem Enabled
DEFAULTBUTTON
150 258 175 339
Proceed

*I MODSEGY_CANCEL
BtnItem Enabled
150 27 175 107
Cancel

*I MODSEGY_MODELTEXT
StatText Enabled
16 22 46 128
Model name:

*I MODSEGY_MODEL
StatText Enabled
16 125 46 355
model




,15
*N MODELT
*S PR 0 PU 1
40 58 425 454
Trace Generation
14

*I MODELT_WAVETEXT
StatText Enabled
1 31 29 179
Select wavelet type:

SETGROUP 1

*I MODELT_RICKER
RadioItem Disabled
37 90 57 280
Ricker Wavelet

*I MODELT_BUTTER
RadioItem Disabled
59 90 79 280
Butterworth Wavelet

*I MODELT_ORMSBY
RadioItem Disabled
81 90 101 280
Ormsby Wavelet(Bandpass)

*I MODELT_USERDEF
RadioItem Disabled
103 90 153 280
User Defined Wavelet

*I MODELT_LINE
StatText Enabled
160 14 190 389
----------------------------------------------------

*I MODELT_USENOISE
ChkItem Enabled
180 37 210 247
Use Signal to Noise ratio of:

*I MODELT_NOISE
EditText Enabled
180 259 212 341
10.0

*I MODELT_NT1
StatText Enabled
220 37 250 392
(Avg. noise amplitude = trace RMS / SN ratio)


*I MODELT_PHASE_TEXT
StatText Enabled
260 37 290 240
Phase Rotation (Degrees)

*I MODELT_PHASE
EditText Enabled
260 259 292 341
0.0

*I MODELT_DISPLAY
ChkItem Enabled
300 37 330 300
Display Wavelet and Spectrum

*I MODELT_CANCEL
BtnItem Disabled
340 28 365 127
Cancel

*I MODELT_OK
BtnItem Disabled
DEFAULTBUTTON
340 269 365 368
Proceed





,16
*N SISCOSINE
*S PR 0 PU 1
34 57 205 400
Cosine Wavelet Definition
9

*I SISCOSINE_OK
BtnItem Enabled
DEFAULTBUTTON
125 220 150 301
Proceed

*I SISCOSINE_CANCEL
BtnItem Enabled
125 18 150 99
Cancel

*I SISCOSINE_DISPLAY
ChkItem Enabled
92 21 112 197
Display Filter Spectrum


*I SISCOSINE_MINFR
EditText Enabled
50 26 82 120
MIN FR 1

*I SISCOSINE_MINFR_TEXT
StatText Enabled
5 30 50 110
Minimum Frequency

*I SISCOSINE_MAXFR_TEXT
StatText Enabled
4 140 50 220
Maximum Frequency

*I SISCOSINE_PHASE_TEXT
StatText Enabled
21 253 50 335
Phase

*I SISCOSINE_PHASE
EditText Enabled
50 248 82 310
0

*I SISCOSINE_MAXFR
EditText Enabled
50 138 82 230
MAX FR 1



,17
*N SISRICKER
*S PR 0 PU 1
52 60 205 335
Ricker Wavelet Definition
8

*I SISRICKER_OK
BtnItem Enabled
DEFAULTBUTTON
110 170 135 250
Proceed


SETGROUP 1

*I SISRICKER_TYPE1
RadioItem Disabled
60 10 80 60
Type 1

*I SISRICKER_TYPE2
RadioItem Disabled
60 62 80 110
Type 2

*I SISRICKER_TYPE3
RadioItem Disabled
60 112 80 162
Type 3

*I SISRICKER_TYPE4
RadioItem Disabled
60 164 80 220
Type 4

*I SISRICKER_PREDF_TEXT
StatText Enabled
10 14 40 191
Predominant Frequency

*I SISRICKER_PREDF
EditText Enabled
6 179 40 260
FREQ 1

*I SISRICKER_CANCEL
BtnItem Enabled
110 10 135 91
Cancel




,18
*N MO_ORMSBY
*S PR 0 PU 1
34 40 280 450
Ormsby Wavelet Definition
15

*I MO_ORMSBY_LCUT
EditText Enabled
50 10 82 100
MIN FR 1

*I MO_ORMSBY_LCUT_TEXT
StatText Enabled
5 15 50 100
Low Cut Freq. 1

*I MO_ORMSBY_LPASS
EditText Enabled
50 110 82 200
MIN FR 2

*I MO_ORMSBY_LPASS_TEXT
StatText Enabled
5 120 50 205
Low Pass Freq. 2

*I MO_ORMSBY_HPASS
EditText Enabled
50 210 82 300
MAX FR 1

*I MO_ORMSBY_HPASS_TEXT
StatText Enabled
5 215 50 300
High Pass Freq. 3

*I MO_ORMSBY_HCUT
EditText Enabled
50 310 82 400
MAX FR 2

*I MO_ORMSBY_HCUT_TEXT
StatText Enabled
5 320 50 393
High Cut Freq. 4


SETGROUP 1

*I MO_ORMSBY_BANDPASS
RadioItem Disabled
100 25 120 203
Bandpass (F1, F2, F3, F4)

*I MO_ORMSBY_LOWPASS
RadioItem Disabled
130 25 150 195
Lowpass  (-, -, F3, F4)

*I MO_ORMSBY_HIGHPASS
RadioItem Disabled
160 25 180 195
Highpass (F1, F2, -, -)

*I MO_ORMSBY_LENGTH_TEXT
StatText Enabled
95 240 130 350
Length (msec)

*I MO_ORMSBY_LENGTH
EditText Enabled
130 255 162 320
300

*I MO_ORMSBY_OK
BtnItem Enabled
DEFAULTBUTTON
200 293 225 374
Proceed

*I MO_ORMSBY_CANCEL
BtnItem Enabled
200 18 225 99
Cancel





,19
*N MO_BUTTERWORTH
*S PR 0 PU 1
34 40 210 370
Butterworth Wavelet Definition
12

*I MO_BUTTWRTH_LPASS
EditText Enabled
52 15 84 75
15

*I MO_BUTTWRTH_LPASS_TEXT
StatText Enabled
5 13 50 79
Low Pass Freq.

*I MO_BUTTWRTH_LROFF
EditText Enabled
52 90 84 150
18

*I MO_BUTTWRTH_LROFF_TEXT
StatText Enabled
5 87 50 165
Roll Off (DB/Oct.)

*I MO_BUTTWRTH_HPASS
EditText Enabled
52 165 84 225
60

*I MO_BUTTWRTH_HPASS_TEXT
StatText Enabled
5 163 50 235
High Pass Freq.

*I MO_BUTTWRTH_HROFF
EditText Enabled
52 240 84 300
18

*I MO_BUTTWRTH_HROFF_TEXT
StatText Enabled
5 237 50 315
Roll Off (DB/Oct.)

*I MO_BUTTWRTH_LENGTH_TEXT
StatText Enabled
95 40 125 193
Filter Length (msec)

*I MO_BUTTWRTH_LENGTH
EditText Enabled
90 200 122 270
300

*I MO_BUTTWRTH_OK
BtnItem Enabled
DEFAULTBUTTON
130 223 155 304
Proceed

*I MO_BUTTWRTH_CANCEL
BtnItem Enabled
130 18 155 99
Cancel



,20
*N MO_RICKER2
*S PR 0 PU 1
52 60 200 363
Ricker Wavelet Definition
6

*I MO_RICKER2_PREDF_TEXT
StatText Enabled
15 14 45 175
Predominant Frequency

*I MO_RICKER2_PREDF
EditText Enabled
15 189 47 254
30

*I MO_RICKER2_LENGTH_TEXT
StatText Enabled
60 50 90 180
Length (msec)

*I MO_RICKER2_LENGTH
EditText Enabled
60 189 92 254
90

*I MO_RICKER2_OK
BtnItem Enabled
DEFAULTBUTTON
105 202 130 283
Proceed

*I MO_RICKER2_CANCEL
BtnItem Enabled
105 10 130 91
Cancel




,21
*N MO_USER_DEF
*S PR 0 PU 1
50 40 330 435
User Wavelet Definition
5

*I MO_USER_DEF_TEXT1
StatText Enabled
1 10 30 380
Enter Integer Wavelet Values separated by commas

*I MO_USER_DEF_TEXT2
StatText Enabled
30 10 60 380
Example:  0,1,4,1,0,etc,............

*I MO_USER_DEF_DATA
EditText Enabled
60 10 228 380

*I MO_USER_DEF_OK
BtnItem Enabled
DEFAULTBUTTON
235 290 260 370
Proceed

*I MO_USER_DEF_CANCEL
BtnItem Enabled
235 10 260 90
Cancel




,22
*N MO_DENSITY
*S PR 0 PR 1
52 60 300 363
Density Definition
8

*I MO_DEN_TEXT
StatText Enabled
15 10 40 280
Select Density Log/Calculation Method

*I MO_DEN_LOG
ChkItem Enabled
50 25 80 280
Retrieve Density Log from Database

SETGROUP 1

*I MO_DEN_GARDNER
RadioItem Disabled
85 25 105 230
Use Gardner Formula

*I MO_DEN_CONSTANT
RadioItem Enabled
110 25 130 230
Use Constant Density Value

*I MO_DEN_CONSTANT_TEXT
StatText Enabled
160 30 190 220
Constant Density (gr/cc)

*I MO_DEN_CONST_VALUE
EditText Enabled
160 220 192 280
2.0

*I MO_DEN_OK
BtnItem Enabled
DEFAULTBUTTON
205 200 230 270
Proceed

*I MO_DEN_CANCEL
BtnItem Enabled
205 20 230 90
CANCEL
