*T CDLG
,1
*N WIYESNO_CDLG
*S PR 0 PU 1
100 100 235 410
Please Answer the Question
3
*I YESNO_QUESTION
StatText Enabled
5 8 85 296


*I YESNO_YES
BtnItem Disabled
DefaultButton
90 31 115 131
Yes

*I YESNO_NO
BtnItem Disabled
90 168 115 268
No


*T CDLG
,2
*N YES_NO_CANCEL_CDLG
*S PR 0 PU 1
100 100 235 410
Please Answer the Question
4
SETMODAL
*I YES_NO_CANCEL_QUESTION
StatText Enabled
5 8 85 296


*I YES_NO_CANCEL_YES
BtnItem Disabled
DefaultButton
90 31 115 97
Yes

*I YES_NO_CANCEL_NO
BtnItem Disabled
90 117 115 182
No

*I YES_NO_CANCEL_CANCEL
BtnItem Disabled
90 202 115 268
Cancel

