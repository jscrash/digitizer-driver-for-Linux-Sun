*T CDLG
,128
*N DIALOG_WC
*S PR 0 PU 1
50 5 347 430
Window Display Control
12

helpkey GLOBAL/WINDOW_CONTROL
*I ITEM_WC_CURRENT
StatText Enabled
5 15 35 305
(Currently Open)

*I ITEM_WC_SELECTOR
Selector Enabled
40 12 186 278
<Selector_1>
<Selector_2>
<Selector_3>
<Selector_4>
<Selector_5>
<Selector_6>
<Selector_7>
<Selector_8>

*I ITEM_WC_VSCROLL
VScroll Enabled
40 285 186 305

*I ITEM_WC_OPEN
BtnItem Enabled
37 320 59 395
Open

*I ITEM_WC_CLOSE
BtnItem Enabled
71 320 93 395
Close

*I ITEM_WC_ON
BtnItem Enabled
105 320 127 395
On

*I ITEM_WC_OFF
BtnItem Enabled
139 320 161 395
Off

*I ITEM_WC_FRONT
BtnItem Enabled
173 320 195 395
Front

*I ITEM_WC_PLOT
BtnItem Enabled
207 320 229 395
Plot

*I ITEM_WC_PROMPT
StatText Enabled
190 15 220 319
(Prompt)

*I ITEM_WC_ERROR
StatText Enabled
225 15 255 319
(Error)

*I ITEM_WC_OK
BtnItem Enabled
DefaultButton
257 320 279 398
Done
