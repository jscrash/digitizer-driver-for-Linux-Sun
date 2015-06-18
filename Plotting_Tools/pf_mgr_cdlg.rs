*T CDLG
,128
*N PF_MAP_INFO_CDLG
*S PR 0 PU 1
30 50 535 530 
Map Plot File Information
31

*I PF_MAP_INFO_TEXT1
StatText Disabled
6 10 33 140 
Plot File Name:

*I PF_MAP_INFO_PLOTFILE_NAME
EditText Enabled
2 116 32 353 

*I PF_MAP_INFO_TEXT2
StatText Disabled
33 10 60 95 
Created By: 

*I PF_MAP_INFO_CREATOR
StatText Disabled
33 93 60 193 
BILLA

*I PF_MAP_INFO_TEXT3
StatText Disabled
33 195 60 285 
Created On: 

*I PF_MAP_INFO_CREATION_DATE
StatText Disabled
33 279 60 379 
Feb 12, 1991

*I PF_MAP_INFO_TEXT4
StatText Disabled
60 10 87 100 
Map Name:

*I PF_MAP_INFO_MAP_NAME
StatText Disabled
60 102 87 400
Do I care

*I PF_MAP_INFO_TEXT5
StatText Disabled
87 10 114 140
Plot Layout Name:

*I PF_MAP_INFO_LAYOUT_NAME
StatText Disabled
87 132 114 400
infiniti

*I PF_MAP_INFO_TEXT6
StatText Disabled
110 10 137 98 
Map Scale:

*I PF_MAP_INFO_MAP_SCALE
StatText Disabled
110 90 137 400 
20000 M/M

*I PF_MAP_INFO_TEXT9
StatText Disabled
137 10 163 123 
Map Projection: 

*I PF_MAP_INFO_MAP_PROJECTION
StatText Disabled
137 125 163 190 
UTM-12

*I PF_MAP_INFO_TEXT10
StatText Disabled
137 230 163 330 
Map Size:

*I PF_MAP_INFO_MAP_SIZE
StatText Disabled
137 302 163 500 
100 X 100 inches

*I PF_MAP_INFO_TEXT11
StatText Disabled
163 10 190 300
Lower Left  XY-Coordinate:

*I PF_MAP_INFO_TEXT12
StatText Disabled
190 10 217 300
Upper Right XY-Coordinate:

*I PF_MAP_INFO_LL_XY
StatText Disabled
163 188 190 349 
487880 , 5555555 M

*I PF_MAP_INFO_UR_XY
StatText Disabled
190 188 217 351 
5050000 , 6666666 M

*I PF_MAP_INFO_TEXT13
StatText Disabled
217 10 244 109 
Deletion Date:

*I PF_MAP_INFO_EXPIRED_DATE
EditText Enabled
217 115 248 255 
Dec 31, 2099

*I PF_MAP_INFO_TEXT14
StatText Disabled
244 10 271 145 
Overlay Definitions:

*I PF_MAP_INFO_TEXT15
StatText Disabled
360 15 387 100 
Description:

*I PF_MAP_INFO_REMARK
EditText Eabled
387 45 440 390 

*I PF_MAP_INFO_CANCEL
BtnItem Enabled
445 75 490 155 
Cancel

*I PF_MAP_INFO_SAVE
BtnItem Enabled
445 265 490 340 
SAVE

SETGROUP 1

*I PF_MAP_INFO_OVERLAY
Selector Enabled
271 25 356 114 

*I PF_MAP_INFO_OPTION
Selector Enabled
271 120 356 260 

*I PF_MAP_INFO_PARAMETER
Selector Enabled
271 266 356 398 

*I PF_MAP_INFO_VSCROLL
Vscroll Enabled
271 400 356 410 


*T CDLG
,129
*N PF_MGR_CDLG
*S PR 0 PU 1
30 50 500 530
Plot File Manager
26

*I PF_MGR_TEXT1
StatText Disabled
6 10 33 430 
Select a Plot File, or Relist the Plot Files Using Different Criteria.

*I PF_MGR_TEXT2
StatText Disabled
205 6 232 300
Select the Function You Wish to Perform.

*I PF_MGR_PLOT
BtnItem Enabled
236 26 261 86
Plot

*I PF_MGR_TEXT3
StatText Disabled
240 90 270 220 
Plot the Plot File.

*I PF_MGR_TEXT4
StatText Disabled
238 290 265 350
Copies: 

*I PF_MGR_COPIES
EditText Enabled
VerifyInteger
235 352 268 390
1

*I PF_MGR_TEXT5
StatText Disabled
268 230 295 350
Choose a Plotter:

*I PF_MGR_PLOTTER
OptionMenu Enabled
272 347 298 535

*I PF_MGR_DISPLAY 
BtnItem Enabled
264 26 290 86
Display

*I PF_MGR_TEXT6
StatText Disabled
268 90 296 230
Display the Plot File.

*I PF_MGR_ERASE
BtnItem Enabled
294 26 319 86
Erase

*I PF_MGR_TEXT7
StatText Disabled
298 90 324 350 
Erase the Plot File Display.

*I PF_MGR_INFO
BtnItem Enabled
323 26 348 86  
Inform

*I PF_MGR_TEXT8
StatText Disabled
327 90 354 350
Get or Modify Plot File Information.

*I PF_MGR_DELETE
BtnItem Enabled
352 26 377 86
Delete

*I PF_MGR_TEXT9
StatText Disabled
356 90 382 300
Delete Plot File Now.

*I PF_MGR_DONE
BtnItem Enabled
DefaultButton
385 190 425 250
DONE

*I PF_MGR_TEXT10
StatText Disabled
92 26 119 300
Plot File Name

*I PF_MGR_TEXT11
StatText Disabled
92 258 119 342
Application

*I PF_MGR_TEXT12
StatText Disabled
92 342 119 400
Source

SetGroup 21
*I PF_MGR_MINE
RadioItem Enabled
33 30 50 140
Current Analyst

*I PF_MGR_ALL
RadioItem Enabled
50 30 83 140
All Analysts

Setgroup 22
*I PF_MGR_FNAME_SELECTOR
Selector Enabled
115 26 200 250

*I PF_MGR_APP_SELECTOR
Selector Enabled
115 256 200 334

*I PF_MGR_SOURCE_SELECTOR
Selector Enabled
115 340 200 400

*I PF_MGR_VSCROLL
Vscroll Enabled
115 402 200 452


*T CDLG
,131
*N PF_ERS_CDLG
*S PR 0 PU 1
30 50 300 550
Erase Display Window
10

*I PF_ERS_TEXT1
StatText Disabled
10 10 37 400
Select a Plot File From the list to erase the display window.

*I PF_ERS_TEXT2
StatText Disabled
36 26 63 300
Plot File Name

*I PF_ERS_TEXT3
StatText Disabled
36 306 63 402
Application

*I PF_ERS_TEXT4
StatText Disabled
36 390 63 470
Source

*I PF_ERS_ERASE
BtnItem Enabled
210 130 250 190
ERASE

*I PF_ERS_DONE
BtnItem Enabled
DefaultButton
210 300 250 360
OK

Setgroup 1
*I PF_ERS_FNAME_SELECTOR
Selector Enabled
60 26 200 300

*I PF_ERS_APP_SELECTOR
Selector Enabled
60 306 200 384

*I PF_ERS_SOURCE_SELECTOR
Selector Enabled
60 390 200 450

*I PF_ERS_VSCROLL
Vscroll Enabled
60 452 200 462



*T CDLG
,132
*N PF_XS_INFO_CDLG
*S PR 0 PU 1
30 50 340 510 
Cross Section Plot File Information
18

*I PF_XS_INFO_TEXT1
StatText Disabled
6 10 33 140 
Plot File Name:

*I PF_XS_INFO_PLOTFILE_NAME
EditText Enabled
4 130 33 433 

*I PF_XS_INFO_TEXT2
StatText Disabled
38 10 65 95 
Created By: 

*I PF_XS_INFO_CREATOR
StatText Disabled
38 130 65 193 
BILLA

*I PF_XS_INFO_TEXT3
StatText Disabled
38 260 65 350 
Created On: 

*I PF_XS_INFO_CREATION_DATE
StatText Disabled
38 340 65 440 
Feb 12, 1991

*I PF_XS_INFO_TEXT4
StatText Disabled
65 10 92 130
Layout Name:

*I PF_XS_INFO_LAYOUT_NAME
StatText Enabled
65 130 92 433
ABC

*I PF_XS_INFO_TEXT5
StatText Disabled
92 10 119 140
Plot Layout Name:

*I PF_XS_INFO_PLOT_LAYOUT
StatText Enabled
92 130 119 433
ABC

*I PF_XS_INFO_TEXT6
StatText Disabled
119 10 146 93
Plot Size:

*I PF_XS_INFO_PLOT_SIZE
StatText Disabled
119 130 146 433
40 X 40 inches

*I PF_XS_INFO_TEXT7
StatText Disabled
146 10 173 100 
Deletion Date:

*I PF_XS_INFO_EXPIRED_DATE
EditText Enabled
146 130 178 250 
Dec 31, 2099

*I PF_XS_INFO_TEXT8
StatText Disabled
173 10 200 100 
Description:

*I PF_XS_INFO_REMARK
EditText Eabled
200 45 254 433 

*I PF_XS_INFO_CANCEL
BtnItem Enabled
263 75 295 155 
Cancel

*I PF_XS_INFO_SAVE
BtnItem Enabled
263 265 295 340 
SAVE
