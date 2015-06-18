*T CDLG
,130
*N PF_PLOT_CDLG
*S PR 0 PU 1
30 50 640 460
Send to Plotter
24

*I PF_PLOT_TEXT22
StatText Disabled
6 30 33 180
Plot File Size:

*I PF_PLOT_SIZE
StatText Disabled
6 120 33 430
00.00 X 00.00 inches

*I PF_PLOT_TEXT2
StatText Disabled
39 30 66 240
Select a Plot Layout

*I PF_PLOT_PLOT_MAP
ChkItem Enabled
184 30 204 100
Send to Plotter

*I PF_PLOT_TEXT3
StatText Disabled
204 30 231 200
Select a Plotter

*I PF_PLOT_TEXT4
StatText Disabled
226 290 253 350
Copies:

*I PF_PLOT_COPIES
EditText Enabled
VerifyInteger
223 352 258 385
1

*I PF_PLOT_SAVE
ChkItem Enabled
336 30 356 100
Save Plot File

*I PF_PLOT_TEXT5
StatText Disabled
356 30 384 200 
Plot File Name:

*I PF_PLOT_NAME
EditText Enabled
353 142 386 385

*I PF_PLOT_TEXT6
StatText Disabled
386 30 413 300
Plot File Expiration Status

*I PF_PLOT_TEXT7
StatText Disabled
469 30 498 200
Description:

*I PF_PLOT_REMARK
EditText Enabled
495 30 544 385

*I PF_PLOT_CANCEL
BtnItem Enabled
550 50 585 110
Cancel

*I PF_PLOT_OK
BtnItem Enabled
DefaultButton
550 280 585 340
OK

SetGroup 31
*I PF_PLOT_LAYOUT_SELECTOR
Selector Enabled
62 30 180 340 

*I PF_PLOT_LAYOUT_VSCROLL
Vscroll Enabled
62 342 180 352

SetGroup 32
*I PF_PLOT_PLOTTER_SELECTOR
Selector Enabled
226 30 326 260 

*I PF_PLOT_PLOTTER_VSCROLL
Vscroll Enabled
226 262 326 272

SetGroup 33
*I PF_PLOT_PERM
RadioItem Enabled
413 30 436 310
Permanent

*I PF_PLOT_DELETED
RadioItem Enabled
436 30 466 310
Deletion Date:

*I PF_PLOT_EXPIRED_DATE
EditText Enabled
436 190 465 308

SetGroup 34
*I PF_PLOT_INCH
RadioItem Enabled
6 300 26 390
inches

*I PF_PLOT_CM
RadioItem Enabled
26 300 56 390
centimeters
