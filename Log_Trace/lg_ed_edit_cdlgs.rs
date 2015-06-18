*T CDLG
,520
*N LG_ED_LIN_TRANS_MIN_MAX
*S PR 0 PU 1
40 6 345 503 
Linear Transform 
19

*I LG_ED_LIN_MM_PROMPT_1
StatText Enabled
0 20 30 460 
Select a trace to transform

*I LG_ED_LIN_MM_EXECUTE
BtnItem Enabled
DefaultButton
268 395 291 475 
Execute

*I LG_ED_LIN_MM_CANCEL
BtnItem Enabled
268 20 291 100 
Cancel

*I LG_ED_LIN_MM_PROMPT_2
StatText Enabled
145 20 175 175 
Current minimum:

*I LG_ED_LIN_MM_OLDMIN
EditText Enabled
VerifyFloat
145 180 175 250 


*I LG_ED_LIN_MM_PROMPT_3
StatText Enabled
145 260 175 405 
Current maximum:

*I LG_ED_LIN_MM_OLDMAX
EditText Enabled
VerifyFloat
145 410 175 480 


*I LG_ED_LIN_MM_PROMPT_7
StatText Enabled
175 20 205 175
New minimum:

*I LG_ED_LIN_MM_NEWMIN
EditText Enabled
VerifyFloat
175 180 205 250


*I LG_ED_LIN_MM_PROMPT_8
StatText Enabled
175 260 205 405
New maximum:

*I LG_ED_LIN_MM_NEWMAX
EditText Enabled
VerifyFloat
175 410 205 480


*I LG_ED_LIN_MM_PROMPT_4
StatText Enabled
206 20 236 135 
Output Trace:

*I LG_ED_LIN_MM_TRACE
EditText Enabled
VerifyAlphaNumeric 10
206 140 236 250


*I LG_ED_LIN_MM_PROMPT_5
StatText Enabled
206 260 236 355 
Service:

*I LG_ED_LIN_MM_SERVICE
EditText Enabled
VerifyAlphaNumeric 10
206 370 236 480 


*I LG_ED_LIN_MM_PROMPT_6
StatText Enabled
237 260 267 355 
Units:

*I LG_ED_LIN_MM_UNITS
EditText Enabled
VerifyAlphaNumeric 20
237 370 267 480 


SETGROUP 1
*I LG_ED_LIN_MM_SELECTOR
Selector Enabled
35 25 140 450 


*I LG_ED_LIN_MM_VSCROLL
VScroll Enabled
35 453 140 465 


*T CDLG
,500
*N LG_ED_LIN_TRANS
*S PR 0 PU 1
40 6 360 503 
Linear Transform (Y=mX+b)
15

*I LG_ED_LIN_TRANS_PROMPT_1
StatText Enabled
1 10 30 460 
Select a trace to transform (X).

*I LG_ED_LIN_TRANS_EXECUTE
BtnItem Enabled
DefaultButton
270 395 295 475 
Execute

*I LG_ED_LIN_TRANS_CANCEL
BtnItem Enabled
270 10 295 90 
Cancel

*I LG_ED_LIN_TRANS_PROMPT_2
StatText Enabled
150 10 180 110 
Slope (m):

*I LG_ED_LIN_TRANS_SLOPE
EditText Enabled
VerifyFloat
150 140 182 250 


*I LG_ED_LIN_TRANS_PROMPT_3
StatText Enabled
150 260 180 365 
Intercept (b):

*I LG_ED_LIN_TRANS_INTERCEPT
EditText Enabled
VerifyFloat
150 370 182 480 


*I LG_ED_LIN_TRANS_PROMPT_4
StatText Enabled
185 10 215 140 
Output Trace (Y):

*I LG_ED_LIN_TRANS_TRACE
EditText Enabled
VerifyAlphaNumeric 10
185 140 217 250


*I LG_ED_LIN_TRANS_PROMPT_5
StatText Enabled
185 260 215 355 
Service:

*I LG_ED_LIN_TRANS_SERVICE
EditText Enabled
VerifyAlphaNumeric 10
185 370 217 480 


*I LG_ED_LIN_TRANS_PROMPT_6
StatText Enabled
220 260 250 355 
Units:

*I LG_ED_LIN_TRANS_UNITS
EditText Enabled
VerifyAlphaNumeric 20
220 370 252 480 


SETGROUP 1
*I LG_ED_LIN_TRANS_SELECTOR
Selector Enabled
34 10 140 450 


*I LG_ED_LIN_TRANS_VSCROLL
VScroll Enabled
34 453 140 465 




*T CDLG
,800
*N LG_ED_SPLICE
*S PR 0 PU 1
40 6 310 503
Splice Curves
16

*I LG_ED_MSG
StatText Enabled
0 10 28 450
Enter trace type and optional service for output trace

*I LG_ED_CANCEL
BtnItem Enabled
230 10 255 90
Cancel

*I LG_ED_PROCEED
BtnItem Enabled
DefaultButton
230 370 255 450
Proceed

*I LG_ED_OTRACE_PR1
StatText Enabled
35 10 66 120
Trace Type:

*I LG_ED_TRACE_OUT
EditText Enabled
VerifyAlphaNumeric 10
35 125 66 230


*I LG_ED_OSERVC_PR1
StatText Enabled
35 250 65 350
Service:

*I LG_ED_SERVICE_OUT
EditText Enabled
VerifyAlphaNumeric 10
35 355 66 460

*I LG_ED_ITRACE_PR1
StatText Enabled
75 10 91 120
Input Trace 1:

*I LG_ED_TRACE1_IN
EditText Enabled
75 125 106 460


*I LG_ED_ITRACE_PR2
StatText Enabled
115 10 131 120
Input Trace 2:

*I LG_ED_TRACE2_IN
EditText Enabled
115 125 146 460


*I LG_ED_INTVL_TXT
StatText Enabled
150 50 166 480
Enter depth interval of Trace 2 that is to be spliced into Trace 1

*I LG_ED_LOW_INTVL_PR
StatText Enabled
180 10 196 120
Interval Top:

*I LG_ED_LOW_INTVL
EditText Enabled
VerifyFloat
180 125 211 230


*I LG_ED_HI_INTVL_PR
StatText Enabled
180 250 196 350
Interval Base:

*I LG_ED_HI_INTVL
EditText Enabled
VerifyFloat
180 355 211 460



*T CDLG
,710
*N LG_ED_SPLICE_CONFIRM
*S PR 0 PU 1
100 100 255 415
Confirm Splice
3

*I LG_ED_SPLICE_CONFIRM_MSG
StatText Enabled
5 20 75 285


*I LG_ED_SPLICE_CONFIRM_PROCEED
BtnItem Enabled
DefaultButton
110 185 135 285
Proceed

*I LG_ED_SPLICE_CONFIRM_CANCEL
BtnItem Enabled
110 15 135 115
Cancel

