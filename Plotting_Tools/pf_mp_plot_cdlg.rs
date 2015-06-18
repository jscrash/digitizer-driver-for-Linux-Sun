*T CDLG
,130
*N PF_MAP_PLOT_CDLG
*S PR 0 PU 1
30 50 670 460
Send to plotter 
27

*I PF_PLOT_TEXT1
StatText Disabled
6 10 33 240 
Select the Map Contents

*I PF_PLOT_TEXT22
StatText Disabled
6 250 33 350
Map Size:

*I PF_PLOT_MAP_SIZE
StatText Disabled
33 250 61 430
00.00 * 00.00 inches

*I PF_PLOT_TEXT2
StatText Disabled
100 40 127 240
Select a Plot Layout

*I PF_PLOT_PLOT_MAP
ChkItem Enabled
224 20 251 100
Plot Map

*I PF_PLOT_TEXT3
StatText Disabled
244 40 270 200
Select a Plotter

*I PF_PLOT_TEXT4
StatText Disabled
266 290 293 350
Copies:

*I PF_PLOT_COPIES
EditText Enabled
VerifyInteger
266 352 298 383
1

*I PF_PLOT_SAVE
ChkItem Enabled
360 20 385 100
Save Plot File

*I PF_PLOT_TEXT5
StatText Disabled
378 40 406 160 
Plot File Name:

*I PF_PLOT_NAME
EditText Enabled
373 142 405 380

*I PF_PLOT_TEXT6
StatText Disabled
402 40 429 300
Plot File Expiration Status

*I PF_PLOT_TEXT7
StatText Disabled
490 40 517 200
Description:

*I PF_PLOT_REMARK
EditText Enabled
517 40 571 380

*I PF_PLOT_CANCEL
BtnItem Enabled
580 50 620 120
Cancel

*I PF_PLOT_OK
BtnItem Enabled
DefaultButton
580 275 620 345
OK

SetGroup 30
*I PF_PLOT_MAP_WITH_LAYOUT
RadioItem Enabled
31 30 50 240
Map Overlays and Layout Windows

*I PF_PLOT_MAP_ONLY
RadioItem Enabled
50 30 85 240
Map Overlays Only

SetGroup 31
*I PF_PLOT_LAYOUT_SELECTOR
Selector Enabled
122 40 220 358 

*I PF_PLOT_LAYOUT_VSCROLL
Vscroll Enabled
122 360 220 370

SetGroup 32
*I PF_PLOT_PLOTTER_SELECTOR
Selector Enabled
266 40 363 260 

*I PF_PLOT_PLOTTER_VSCROLL
Vscroll Enabled
266 262 362 272

SetGroup 33
*I PF_PLOT_PERM
RadioItem Enabled
425 40 455 298
Permanent

*I PF_PLOT_DELETED
RadioItem Enabled
455 40 487 298
Deletion Date:

*I PF_PLOT_EXPIRED_DATE
EditText Enabled
453 170 486 298

SetGroup 34
*I PF_PLOT_INCH
RadioItem Enabled
55 290 75 370
inches

*I PF_PLOT_CM
RadioItem Enabled
80 290 110 370
centimeters
