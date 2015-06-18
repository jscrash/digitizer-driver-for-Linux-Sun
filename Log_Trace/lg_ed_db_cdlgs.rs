*T CDLG
,200
*N LG_ED_AVAIL_TRACES
*S PR 0 PU 1
40 6 340 503 
Available Log Traces
5

*I LG_ED_AT_PROMPT_1
StatText Enabled
5 20 35 460 
For additional Information, select a trace and press "Get Info".

*I LG_ED_AT_INFO
BtnItem Enabled
250 395 275 475 
Get Info

*I LG_ED_AT_DONE
BtnItem Enabled
DefaultButton
250 20 275 100 
Done

SETGROUP 1
*I LG_ED_AT_SELECTOR
Selector Enabled
40 25 240 450 


*I LG_ED_AT_VSCROLL
VScroll Enabled
40 453 240 470 




*T CDLG
,300
*N LG_ED_LOG_HDR_CDLG
*S PR 0 PU 1
40 6 370 503
Well Log Trace Hdr
21

*I LG_ED_LOG_HDR_DONE
BtnItem Enabled
DefaultButton
275 395 300 475 
Done

*I LG_ED_LOG_HDR_LABEL_1
StatText Enabled
5 20 35 120 
Trace Name:

*I LG_ED_LOG_HDR_TRACE_NAME
StatText Enabled
5 120 35 420 
SOURCE:TRACE.SERVICE;VER

*I LG_ED_LOG_HDR_LABEL_2
StatText Enabled
50 20 80 120 
Top of Data:

*I LG_ED_LOG_HDR_TOP
StatText Enabled
50 120 80 220 
321.1524

*I LG_ED_LOG_HDR_LABEL_3
StatText Enabled
50 235 80 335 
Base of Data:

*I LG_ED_LOG_HDR_BASE
StatText Enabled
50 375 80 475 
1543.00

*I LG_ED_LOG_HDR_LABEL_4
StatText Enabled
90 20 120 120 
Depth Units:

*I LG_ED_LOG_HDR_DEPTH_UOM
StatText Enabled
90 120 120 220 
METERS

*I LG_ED_LOG_HDR_LABEL_5
StatText Enabled
90 235 120 365 
Sample Increment:

*I LG_ED_LOG_HDR_DINC
StatText Enabled
90 375 120 475 
0.1524

*I LG_ED_LOG_HDR_LABEL_6
StatText Enabled
140 20 170 135 
Trace Minimum:

*I LG_ED_LOG_HDR_MIN
StatText Enabled
140 135 170 235 
12345.0000

*I LG_ED_LOG_HDR_LABEL_7
StatText Enabled
140 235 170 350 
Trace Maximum:

*I LG_ED_LOG_HDR_MAX
StatText Enabled
140 375 170 475 
12345.0000

*I LG_ED_LOG_HDR_LABEL_8
StatText Enabled
180 20 210 120 
Trace Units:

*I LG_ED_LOG_HDR_TRACE_UOM
StatText Enabled
180 135 210 235 
API

*I LG_ED_LOG_HDR_LABEL_9
StatText Enabled
230 20 260 120 
Last Updated:

*I LG_ED_LOG_HDR_LAST_UPDATE
StatText Enabled
230 125 260 225 
26-Oct-1987

*I LG_ED_LOG_HDR_LABEL_10
StatText Enabled
230 235 260 285 
By:

*I LG_ED_LOG_HDR_UPDATED_BY
StatText Enabled
230 285 260 470 
FINDER_ACCOUNT



*T CDLG
,400
*N LG_ED_RENAME_TRACE_CDLG
*S PR 0 PU 1
40 6 345 503 
Rename a Log Trace
10

*I LG_ED_REN_TRACE_PROMPT_1
StatText Enabled
0 20 30 460 
Select an existing trace to rename.

*I LG_ED_REN_TRACE_PROMPT_2
StatText Enabled
180 25 210 470 
Enter the new Trace and (optional) Service for the trace.

*I LG_ED_REN_TRACE_PROMPT_3
StatText Enabled
215 25 245 80 
Trace:

*I LG_ED_REN_TRACE_TRACE
EditText Enabled
VerifyAlphaNumeric 10
215 90 247 215 


*I LG_ED_REN_TRACE_PROMPT_4
StatText Enabled
215 245 245 315 
Service:

*I LG_ED_REN_TRACE_SERVICE
EditText Enabled
VerifyAlphaNumeric 10
215 325 247 450 


*I LG_ED_REN_TRACE_RENAME
BtnItem Enabled
265 395 290 475 
Rename

*I LG_ED_REN_TRACE_CANCEL
BtnItem Enabled
265 20 290 100 
Cancel

SETGROUP 1
*I LG_ED_REN_TRACE_SELECTOR
Selector Enabled
35 25 176 450 


*I LG_ED_REN_TRACE_VSCROLL
VScroll Enabled
35 453 176 465 




*T CDLG
,410
*N LG_ED_REN_CON_CDLG
*S PR 0 PU 1
100 40 270 475 
Confirm Rename
7

*I LG_ED_REN_CON_PROMPT_1
StatText Enabled
5 10 35 135 
Old Trace Name:

*I LG_ED_REN_CON_OLD_NAME
StatText Enabled
5 135 35 430 
THIRTY_CHARACTERS+THREE_MORE_____

*I LG_ED_REN_CON_PROMPT_2
StatText Enabled
40 10 70 135 
New Trace Name:

*I LG_ED_REN_CON_NEW_NAME
StatText Enabled
40 135 70 430 
THIRTY_CHARACTERS+THREE_MORE_____

*I LG_ED_REN_CON_PROMPT_3
StatText Enabled
80 10 110 145 
Please verify...

*I LG_ED_REN_CON_RENAME
BtnItem Enabled
DefaultButton
125 305 150 410 
Rename

*I LG_ED_REN_CON_CANCEL
BtnItem Enabled
125 10 150 115 
Do Not Rename




*T CDLG
,600
*N LG_ED_COPY_TRACE_CDLG
*S PR 0 PU 1
40 6 345 503 
Copy a Log Trace
10

*I LG_ED_COPY_PROMPT_1
StatText Enabled
0 20 30 460 
Select an existing trace to copy.

*I LG_ED_COPY_PROMPT_2
StatText Enabled
180 25 210 470 
Enter the Trace name and (optional) Service for the new trace.

*I LG_ED_COPY_OK
BtnItem Enabled
265 395 290 475 
Copy

*I LG_ED_COPY_CANCEL
BtnItem Enabled
265 20 290 100 
Cancel

*I LG_ED_COPY_PROMPT_3
StatText Enabled
215 25 245 80 
Trace:

*I LG_ED_COPY_TRACE
EditText Enabled
VerifyAlphaNumeric 10
215 90 247 215 


*I LG_ED_COPY_PROMPT_4
StatText Enabled
215 245 245 315 
Service:

*I LG_ED_COPY_SERVICE
EditText Enabled
VerifyAlphaNumeric 10
215 325 247 450 


SETGROUP 1
*I LG_ED_COPY_SELECTOR
Selector Enabled
35 25 176 450 


*I LG_ED_COPY_VSCROLL
VScroll Enabled
35 453 176 465 




*T CDLG
,700
*N LG_ED_DEL_TRACES
*S PR 0 PU 1
40 6 331 503 
Delete Log Traces
5

*I LG_ED_DEL_TRACES_DELETE
BtnItem Enabled
DefaultButton
245 395 270 475 
Delete

*I LG_ED_DEL_TRACES_CANCEL
BtnItem Enabled
245 20 270 100 
Cancel

*I LG_ED_DEL_TRACES_PROMPT
StatText Enabled
0 20 30 460 
Select the traces to delete:

SETGROUP 1
*I LG_ED_DEL_TRACES_SELECTOR
Selector Enabled
35 25 231 450 


*I LG_ED_DEL_TRACES_VSCROLL
VScroll Enabled
35 453 231 470 


*T CDLG
,710
*N LG_ED_DEL_CONFIRM
*S PR 0 PU 1
100 100 255 415 
Confirm Delete
4

*I LG_ED_DEL_CONFIRM_MSG
StatText Enabled
5 20 60 280 
You have selected 132 log traces to delete.

*I LG_ED_DEL_CONFIRM_PROMPT
StatText Enabled
70 20 100 155 
Please verify...

*I LG_ED_DEL_CONFIRM_DELETE
BtnItem Enabled
DefaultButton
110 185 135 285 
Delete

*I LG_ED_DEL_CONFIRM_CANCEL
BtnItem Enabled
110 15 135 115 
Do Not Delete


