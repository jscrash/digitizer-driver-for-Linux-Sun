*T CDLG
,1
*N SS_DIAL
*S PR 1 PU 0
47 60 320 370 
Select SURVEY
6

*I SS_PROMPT
StatText Enabled
0 30 30 300 
Select a SURVEY and press " DONE ".

*I SS_SELECT_PROMPT
StatText Enabled
35 90 62 300 
List of surveys

*I SS_IGNORE
BtnItem Enabled
230 40 255 110 
CANCEL

*I SS_ACCEPT
BtnItem Enabled
DefaultButton
230 190 255 250 
DONE


SETGROUP 1
*I SS_SELECTOR
Selector Enabled
65 30 212 253 
<Survey_1>
<Survey_2>
<Survey_3>
<Survey_4>
<Survey_5>
<Survey_6>
<Survey_7>
<Survey_8>


*I SS_SCROLL
VScroll Enabled
65 253 212 270 

