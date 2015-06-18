*UX
*L ALL
*T CDLG 
,100
*N SYN_INIT
*S PR 0 PU 1
41 41 320 421
Well Parameters
11

SETGROUP 1

*I SYN_INIT_GL_UWI_BUTT
RadioItem Enabled
15 10 35 190
Use Current Global UWI:


*I SYN_INIT_GL_UWI_TEXT
StatText Enabled
15 200 40 370


*I SYN_INIT_USER_UWI_BUTT
RadioItem Enabled
35 10 60 190
Enter a UWI:


*I SYN_INIT_USER_UWI_TEXT
EditText Enabled
45 195 77 365
<User_Uwi_Text>

*I SYN_INIT_LINE1
StatText Enabled
85 2 110 370
---------------------------------------------


SETGROUP 2

*I SYN_INIT_METERS
RadioItem Enabled
115 10 140 280
Synthetics Output to be in meters

 
*I SYN_INIT_FEET
RadioItem Enabled
140 10 170 275
Synthetics Output to be in feet

 
*I SYN_INIT_LINE2
StatText Enabled
180 2 205 370
---------------------------------------------

 
*I SYN_INIT_SAVE
ChkItem Enabled
200 10 220 316
Save all output time curves in database
 

 
*I SYN_INIT_CANCEL
BtnItem Enabled
235 32 260 121
Cancel

*I SYN_INIT_OK
BtnItem Enabled
DefaultButton
235 230 260 320
Proceed






,101
*N SYN_WAVLET
*S PR 0 PU 1
40 58 325 454
Trace Generation
11

*I SYN_WAVLET_TEXT
StatText Enabled
0 80 30 250
Select wavelet type:

SETGROUP 1

*I SYN_WAVLET_RICKER
RadioItem Disabled
40 100 65 300
Ricker Wavelet

*I SYN_WAVLET_BUTTER
RadioItem Disabled
65 100 90 300
Butterworth Wavelet

*I SYN_WAVLET_ORMSBY
RadioItem Disabled
90 100 115 300
Ormsby Wavelet(Bandpass)

*I SYN_WAVLET_USERDEF
RadioItem Disabled
115 100 150 300
User Defined Wavelet

*I SYN_WAVLET_LINE
StatText Enabled
155 14 180 389
----------------------------------------------


*I SYN_WAVLET_PHASE_TEXT
StatText Enabled
175 37 205 240
Phase Rotation (Degrees)

*I SYN_WAVLET_PHASE
EditText Enabled
175 259 207 341
0.0

*I SYN_WAVLET_DISPLAY
ChkItem Enabled
210 37 235 300
Display Wavelet and Spectrum

*I SYN_WAVLET_CANCEL
BtnItem Disabled
240 28 265 127
Cancel

*I SYN_WAVLET_OK
BtnItem Disabled
DEFAULTBUTTON
240 269 265 368
Proceed








 
,102 
*N SYN_ORMSBY
*S PR 0 PU 1 
40 30 375 430
Ormsby Bandpass Filter Definition
29 

 
*I SYN_ORMSBY_F1_TEXT
StatText Enabled 
5 10 50 80
Low Cut Freq. 1

*I SYN_ORMSBY_F1_A 
EditText Enabled 
55 10 87 75
<1_A>

*I SYN_ORMSBY_F1_B 
EditText Enabled 
90 10 122 75
<1_B>

*I SYN_ORMSBY_F1_C 
EditText Enabled 
125 10 157 75
<1_C>



*I SYN_ORMSBY_F2_TEXT
StatText Enabled 
5 83 50 160
Low Pass Freq. 2

*I SYN_ORMSBY_F2_A 
EditText Enabled 
55 85 87 150
<2_A>

*I SYN_ORMSBY_F2_B 
EditText Enabled 
90 85 122 150
<2_B>

*I SYN_ORMSBY_F2_C 
EditText Enabled 
125 85 157 150
<2_C>



*I SYN_ORMSBY_F3_TEXT
StatText Enabled 
5 158 50 235
High Pass Freq. 3

*I SYN_ORMSBY_F3_A 
EditText Enabled 
55 160 87 225
<3_A>

*I SYN_ORMSBY_F3_B 
EditText Enabled 
90 160 122 225
<3_B>

*I SYN_ORMSBY_F3_C 
EditText Enabled 
125 160 157 225
<3_C>




*I SYN_ORMSBY_F4_TEXT
StatText Enabled 
5 235 50 305
High Cut Freq. 4

*I SYN_ORMSBY_F4_A 
EditText Enabled 
55 235 87 300
<4_A>

*I SYN_ORMSBY_F4_B 
EditText Enabled 
90 235 122 300
<4_B>

*I SYN_ORMSBY_F4_C 
EditText Enabled 
125 235 157 300
<4_C>



*I SYN_ORMSBY_TIME_TEXT
StatText Enabled 
5 320 50 390
End Time (msec)

*I SYN_ORMSBY_TIME_A 
EditText Enabled 
55 320 87 385
<5_A>

*I SYN_ORMSBY_TIME_B 
EditText Enabled 
90 320 122 385
<5_B>

*I SYN_ORMSBY_TIME_C 
EditText Enabled 
125 320 157 385
<5_C>



*I SYN_ORMSBY_BPASS
RadioItem Disabled
180 15 200 205
Bandpass (F1,F2,F3,F4)
 
*I SYN_ORMSBY_LPASS
RadioItem Disabled
200 15 220 205
Lowpass (-,-,F3,F4)

*I SYN_ORMSBY_HPASS
RadioItem Disabled
220 15 240 205
Highpass (F1,F2,-,-)



*I SYN_ORMSBY_TAPER_TEXT
StatText Enabled 
160 220 190 395 
Length Taper Zone (msec) 


*I SYN_ORMSBY_TAPER 
EditText Enabled 
185 260 217 340
<_TZ_>
 
*I SYN_ORMSBY_LENGTH_TEXT
StatText Enabled 
220 230 250 390 
Filter Length (msec)


*I SYN_ORMSBY_LENGTH 
EditText Enabled 
245 260 277 340
<_FL_>
 
*I SYN_ORMSBY_CANCEL 
BtnItem Enabled
290 30 315 120
Cancel 

*I SYN_ORMSBY_OK 
BtnItem Enabled
DefaultButton
290 280 315 370 
Proceed 









,103 
*N SYN_BUTTWRTH
*S PR 0 PU 1 
40 30 325 430
Butterworth Filter Definition
26 

 
*I SYN_BUTTWRTH_F1_TEXT
StatText Enabled 
5 10 50 85
Low Pass Freq.

*I SYN_BUTTWRTH_F1_A 
EditText Enabled 
55 10 87 75
<1_A>

*I SYN_BUTTWRTH_F1_B 
EditText Enabled 
90 10 122 75
<1_B>

*I SYN_BUTTWRTH_F1_C 
EditText Enabled 
125 10 157 75
<1_C>



*I SYN_BUTTWRTH_S1_TEXT
StatText Enabled 
5 83 50 160
Roll Off (DB/Oct.)

*I SYN_BUTTWRTH_S1_A 
EditText Enabled 
55 85 87 150
<2_A>

*I SYN_BUTTWRTH_S1_B 
EditText Enabled 
90 85 122 150
<2_B>

*I SYN_BUTTWRTH_S1_C 
EditText Enabled 
125 85 157 150
<2_C>



*I SYN_BUTTWRTH_F2_TEXT 
StatText Enabled 
5 158 50 235
High Pass Freq.

*I SYN_BUTTWRTH_F2_A 
EditText Enabled 
55 160 87 225
<3_A>

*I SYN_BUTTWRTH_F2_B 
EditText Enabled 
90 160 122 225
<3_B>

*I SYN_BUTTWRTH_F2_C 
EditText Enabled 
125 160 157 225
<3_C>



*I SYN_BUTTWRTH_S2_TEXT
StatText Enabled 
5 235 50 310
Roll Off (DB/Oct.)

*I SYN_BUTTWRTH_S2_A
EditText Enabled 
55 235 87 300
<4_A>

*I SYN_BUTTWRTH_S2_B
EditText Enabled 
90 235 122 300
<4_B>

*I SYN_BUTTWRTH_S2_C
EditText Enabled 
125 235 157 300
<4_C>



*I SYN_BUTTWRTH_TIME_TEXT
StatText Enabled 
5 320 50 390
End Time (msec)

*I SYN_BUTTWRTH_TIME_A 
EditText Enabled 
55 320 87 385
<5_A>

*I SYN_BUTTWRTH_TIME_B 
EditText Enabled 
90 320 122 385
<5_B>

*I SYN_BUTTWRTH_TIME_C 
EditText Enabled 
125 320 157 385
<5_C>



*I SYN_BUTTWRTH_TAPER_TEXT
StatText Enabled 
165 20 195 210 
Length Taper Zone (msec) 


*I SYN_BUTTWRTH_TAPER 
EditText Enabled 
165 235 197 310
<_TZ_>

 
*I SYN_BUTTWRTH_LENGTH_TEXT
StatText Enabled 
200 20 230 210 
Filter Length (msec)


*I SYN_BUTTWRTH_LENGTH 
EditText Enabled 
200 235 232 310
<_FL_>
 

 
*I SYN_BUTTWRTH_CANCEL 
BtnItem Enabled
240 30 265 120
Cancel 

*I SYN_BUTTWRTH_OK 
BtnItem Enabled
DefaultButton
240 280 265 370 
Proceed 








 
,104 
*N SYN_RICKER
40 50 330 350
Ricker Wavelet Definition
14 

 
*I SYN_RICKER_FREQ_TEXT
StatText Enabled 
5 50 50 140 
Predominant Frequencies
 
*I SYN_RICKER_FREQ_1 
EditText Enabled 
55 60 87 130
<F_1>
 
*I SYN_RICKER_FREQ_2 
EditText Enabled 
90 60 122 130
<F_2>
 
*I SYN_RICKER_FREQ_3 
EditText Enabled 
125 60 157 130
<F_3>



 
*I SYN_RICKER_TIME_TEXT
StatText Enabled 
5 180 50 260
End Time (msec)

*I SYN_RICKER_TIME_A 
EditText Enabled 
55 175 85 270
<TIME_1>

*I SYN_RICKER_TIME_B 
EditText Enabled 
90 175 122 270
<TIME_2>

*I SYN_RICKER_TIME_C 
EditText Enabled 
125 175 157 270
<TIME_3>



*I SYN_RICKER_TAPER_PROMPT
StatText Enabled 
165 20 195 200 
Length Taper Zone (msec)
 
*I SYN_RICKER_TAPER 
EditText Enabled 
165 195 197 270
<_TZ_>


*I SYN_RICKER_LENGTH_PROMPT
StatText Enabled 
200 20 230 200 
Filter Length (msec)
 
*I SYN_RICKER_LENGTH 
EditText Enabled 
200 195 232 270
<_FL_>



*I SYN_RICKER_CANCEL 
BtnItem Enabled
240 20 265 100
Cancel 

*I SYN_RICKER_OK 
BtnItem Enabled
DefaultButton
240 190 265 270 
Proceed 
 




*UM
*L ALL
*T CDLG 

,100
*N SYN_INIT
*S PR 0 PU 1
41 41 291 421
Well Parameters
11


SETGROUP 1

*I SYN_INIT_GL_UWI_BUTT
RadioItem Enabled
15 10 31 200
Use Current Global UWI:


*I SYN_INIT_GL_UWI_TEXT
StatText Enabled
15 200 31 370


*I SYN_INIT_USER_UWI_BUTT
RadioItem Enabled
41 10 57 190
Enter a UWI:


*I SYN_INIT_USER_UWI_TEXT
EditText Enabled
39 195 60 365


*I SYN_INIT_LINE1
StatText Enabled
63 10 78 360
--------------------------------------------------


SETGROUP 2

*I SYN_INIT_METERS
RadioItem Enabled
90 10 108 265
Synthetics Output to be in meters

 
*I SYN_INIT_FEET
RadioItem Enabled
118 10 136 265
Synthetics Output to be in feet

 
*I SYN_INIT_LINE2
StatText Enabled
140 10 158 360
--------------------------------------------------

 
*I SYN_INIT_SAVE
ChkItem Enabled
165 10 183 316
Save all output time curves in database
 

 
*I SYN_INIT_CANCEL
BtnItem Enabled
200 32 231 121
Cancel

*I SYN_INIT_OK
BtnItem Enabled
DefaultButton
200 230 231 320
Proceed






,101
*N SYN_WAVLET
*S PR 0 PU 1
40 58 280 454
Trace Generation
11

*I SYN_WAVLET_TEXT
StatText Enabled
8 31 23 179
Select wavelet type:

SETGROUP 1

*I SYN_WAVLET_RICKER
RadioItem Disabled
32 10 47 135
Ricker Wavelet

*I SYN_WAVLET_BUTTER
RadioItem Disabled
54 10 69 170
Butterworth Wavelet

*I SYN_WAVLET_ORMSBY
RadioItem Disabled
32 195 47 394
Ormsby Wavelet(Bandpass)

*I SYN_WAVLET_USERDEF
RadioItem Disabled
54 195 69 390
User Defined Wavelet

*I SYN_WAVLET_LINE
StatText Enabled
74 14 83 389
----------------------------------------------------


*I SYN_WAVLET_PHASE_TEXT
StatText Enabled
120 37 140 240
Phase Rotation (Degrees)

*I SYN_WAVLET_PHASE
EditText Enabled
120 259 140 341
0.0

*I SYN_WAVLET_DISPLAY
ChkItem Enabled
155 37 175 300
Display Wavelet and Spectrum

*I SYN_WAVLET_CANCEL
BtnItem Disabled
200 28 230 127
Cancel

*I SYN_WAVLET_OK
BtnItem Disabled
DEFAULTBUTTON
200 269 230 368
Proceed








 
,102 
*N SYN_ORMSBY
*S PR 0 PU 1 
40 30 320 430
Ormsby Bandpass Filter Definition
29 

 
*I SYN_ORMSBY_F1_TEXT
StatText Enabled 
5 10 36 70
Low Cut Freq. 1

*I SYN_ORMSBY_F1_A 
EditText Enabled 
40 10 62 70

*I SYN_ORMSBY_F1_B 
EditText Enabled 
64 10 86 70

*I SYN_ORMSBY_F1_C 
EditText Enabled 
88 10 110 70



*I SYN_ORMSBY_F2_TEXT
StatText Enabled 
5 83 36 147
Low Pass Freq. 2

*I SYN_ORMSBY_F2_A 
EditText Enabled 
40 85 62 145

*I SYN_ORMSBY_F2_B 
EditText Enabled 
64 85 86 145

*I SYN_ORMSBY_F2_C 
EditText Enabled 
88 85 110 145



*I SYN_ORMSBY_F3_TEXT
StatText Enabled 
5 158 36 222
High Pass Freq. 3

*I SYN_ORMSBY_F3_A 
EditText Enabled 
40 160 62 220

*I SYN_ORMSBY_F3_B 
EditText Enabled 
64 160 86 220

*I SYN_ORMSBY_F3_C 
EditText Enabled 
88 160 110 220



*I SYN_ORMSBY_F4_TEXT
StatText Enabled 
5 235 36 295
High Cut Freq. 4

*I SYN_ORMSBY_F4_A 
EditText Enabled 
40 235 62 295

*I SYN_ORMSBY_F4_B 
EditText Enabled 
64 235 86 295

*I SYN_ORMSBY_F4_C 
EditText Enabled 
88 235 110 295



*I SYN_ORMSBY_TIME_TEXT
StatText Enabled 
5 320 36 390
End Time (msec)

*I SYN_ORMSBY_TIME_A 
EditText Enabled 
40 320 62 390

*I SYN_ORMSBY_TIME_B 
EditText Enabled 
64 320 86 390

*I SYN_ORMSBY_TIME_C 
EditText Enabled 
88 320 110 390



*I SYN_ORMSBY_BPASS
RadioItem Disabled
140 15 155 205
Bandpass (F1,F2,F3,F4)
 
*I SYN_ORMSBY_LPASS
RadioItem Disabled
170 15 185 205
Lowpass (-,-,F3,F4)

*I SYN_ORMSBY_HPASS
RadioItem Disabled
200 15 215 205
Highpass (F1,F2,-,-)



*I SYN_ORMSBY_TAPER_TEXT
StatText Enabled 
130 220 151 395 
Length Taper Zone (msec) 


*I SYN_ORMSBY_TAPER 
EditText Enabled 
152 260 172 320

 
*I SYN_ORMSBY_LENGTH_TEXT
StatText Enabled 
180 230 201 390 
Filter Length (msec)


*I SYN_ORMSBY_LENGTH 
EditText Enabled 
202 260 222 320
 

 
*I SYN_ORMSBY_CANCEL 
BtnItem Enabled
240 30 275 120
Cancel 

*I SYN_ORMSBY_OK 
BtnItem Enabled
DefaultButton
240 280 275 370 
Proceed 









,103 
*N SYN_BUTTWRTH
*S PR 0 PU 1 
40 30 320 430
Butterworth Filter Definition
26 

 
*I SYN_BUTTWRTH_F1_TEXT
StatText Enabled 
5 8 36 72
Low Pass Freq.

*I SYN_BUTTWRTH_F1_A 
EditText Enabled 
40 10 62 70

*I SYN_BUTTWRTH_F1_B 
EditText Enabled 
64 10 86 70

*I SYN_BUTTWRTH_F1_C 
EditText Enabled 
88 10 110 70



*I SYN_BUTTWRTH_S1_TEXT
StatText Enabled 
5 83 36 147
Roll Off (DB/Oct.)

*I SYN_BUTTWRTH_S1_A 
EditText Enabled 
40 85 62 145

*I SYN_BUTTWRTH_S1_B 
EditText Enabled 
64 85 86 145

*I SYN_BUTTWRTH_S1_C 
EditText Enabled 
88 85 110 145



*I SYN_BUTTWRTH_F2_TEXT 
StatText Enabled 
5 158 36 222
High Pass Freq.

*I SYN_BUTTWRTH_F2_A 
EditText Enabled 
40 160 62 220

*I SYN_BUTTWRTH_F2_B 
EditText Enabled 
64 160 86 220

*I SYN_BUTTWRTH_F2_C 
EditText Enabled 
88 160 110 220



*I SYN_BUTTWRTH_S2_TEXT
StatText Enabled 
5 235 36 295
Roll Off (DB/Oct.)

*I SYN_BUTTWRTH_S2_A
EditText Enabled 
40 235 62 295

*I SYN_BUTTWRTH_S2_B
EditText Enabled 
64 235 86 295

*I SYN_BUTTWRTH_S2_C
EditText Enabled 
88 235 110 295



*I SYN_BUTTWRTH_TIME_TEXT
StatText Enabled 
5 320 36 390
End Time (msec)

*I SYN_BUTTWRTH_TIME_A 
EditText Enabled 
40 320 62 390

*I SYN_BUTTWRTH_TIME_B 
EditText Enabled 
64 320 86 390

*I SYN_BUTTWRTH_TIME_C 
EditText Enabled 
88 320 110 390



*I SYN_BUTTWRTH_TAPER_TEXT
StatText Enabled 
140 20 162 210 
Length Taper Zone (msec) 


*I SYN_BUTTWRTH_TAPER 
EditText Enabled 
140 240 162 310

 
*I SYN_BUTTWRTH_LENGTH_TEXT
StatText Enabled 
180 20 202 210 
Filter Length (msec)


*I SYN_BUTTWRTH_LENGTH 
EditText Enabled 
180 240 202 310
 

 
*I SYN_BUTTWRTH_CANCEL 
BtnItem Enabled
240 30 275 120
Cancel 

*I SYN_BUTTWRTH_OK 
BtnItem Enabled
DefaultButton
240 280 275 370 
Proceed 








 
,104 
*N SYN_RICKER
40 50 320 350
Ricker Wavelet Definition
14 

 
*I SYN_RICKER_FREQ_TEXT
StatText Enabled 
5 50 45 140 
Predominant Frequencies
 
*I SYN_RICKER_FREQ_1 
EditText Enabled 
50 70 72 120
 
*I SYN_RICKER_FREQ_2 
EditText Enabled 
74 70 96 120
 
*I SYN_RICKER_FREQ_3 
EditText Enabled 
98 70 120 120



 
*I SYN_RICKER_TIME_TEXT
StatText Enabled 
5 180 45 260
End Time (msec)

*I SYN_RICKER_TIME_A 
EditText Enabled 
50 175 72 250

*I SYN_RICKER_TIME_B 
EditText Enabled 
74 175 96 250

*I SYN_RICKER_TIME_C 
EditText Enabled 
98 175 120 250



*I SYN_RICKER_TAPER_PROMPT
StatText Enabled 
135 20 157 200 
Length Taper Zone (msec)
 
*I SYN_RICKER_TAPER 
EditText Enabled 
134 210 158 270



*I SYN_RICKER_LENGTH_PROMPT
StatText Enabled 
170 20 192 200 
Filter Length (msec)
 
*I SYN_RICKER_LENGTH 
EditText Enabled 
169 210 193 270



*I SYN_RICKER_CANCEL 
BtnItem Enabled
235 20 270 100
Cancel 

*I SYN_RICKER_OK 
BtnItem Enabled
DefaultButton
235 200 270 280 
Proceed 
 




