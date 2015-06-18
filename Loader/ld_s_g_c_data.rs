*T CDLG
,1
*N LD_S_G_C_HEAD
*S PR 0 PU 1
40 5 360 230
Data Loader
10

SetGroup 1

*I LD_TITLE_1
StatText Enabled
5 10 37 200
Data Type

*I LD_DUMMY_BOX_1
RectItem Enabled
43 10 157 200


*I LD_SCATTER_DATA
RadioItem Enabled
45 20 65 200
Scatter (XYZ) Data

*I LD_GRID_DATA
RadioItem Enabled
70 20 90 200
Grid Data

*I LD_CONTOUR_DATA
RadioItem Enabled
95 20 115 200
Contour Data

*I LD_PERSPEC_DATA
RadioItem Enabled
120 20 155 200
Perspective Data

SetGroup 2

*I LD_TITLE_2
StatText Enabled
165 10 195 200
Directory

*I LD_DIRECTORY
EditText Enabled
196 16 275 206


SetGroup 3

*I LD_QUIT_1
BtnItem Enabled
285 10 305 60
Quit

*I LD_OK_1
BtnItem Enabled
DEFAULTBUTTON
280 150 300 200
OK

*T CDLG
,2
*N LD_SCATTER
*S PR 0 PU 1
40 223 350 515
Scatter Data Loader
10

SetGroup 1

*I LD_TITLE_3
StatText Enabled
1 10 29 200
Select File(s)

*I LD_XYZ_SELECT
Selector Enabled
35 10 164 250


*I LD_XYZ_SCROLL
VScroll Enabled
34 250 165 270


SetGroup 2

*I LD_TITLE_4
StatText Enabled
170 10 197 200
Number of Records To Skip:

*I LD_NUM_RECORDS
EditText Enabled
168 210 200 265


SetGroup 3

*I LD_TITLE_5
StatText Enabled
203 10 230 200
Projection

*I LD_PROJECTION
EditText Enabled
230 10 262 265


SetGroup 4

*I LD_QUIT_2
BtnItem Enabled
270 10 290 60
Quit

*I LD_RUN_1
BtnItem Enabled
270 110 290 160
Run

*I LD_SUBMIT_1
BtnItem Enabled
DEFAULTBUTTON
270 210 290 260
Submit

*T CDLG
,3
*N LD_GRID
*S PR 0 PU 1
40 223 345 515
Grid Data Loader
6

SetGroup 1

*I LD_TITLE_6
StatText Enabled
1 10 28 200
Select File(s)

*I LD_GRID_SELECT
Selector Enabled
32 10 261 250


*I LD_GRID_SCROLL
VScroll Enabled
31 250 262 270


SetGroup 2

*I LD_QUIT_3
BtnItem Enabled
265 10 285 60
Quit

*I LD_RUN_2
BtnItem Enabled
265 110 285 160
Run

*I LD_SUBMIT_2
BtnItem Enabled
DEFAULTBUTTON
265 210 285 260
Submit

*T CDLG
,4
*N LD_CONTOUR
*S PR 0 PU 1
40 223 345 515
Contour Data Loader
6

SetGroup 2

*I LD_TITLE_7
StatText Enabled
1 10 28 200
Select File(s)

*I LD_CONTOUR_SELECT
Selector Enabled
32 10 261 250


*I LD_CONTOUR_SCROLL
VScroll Enabled
31 250 262 270


SetGroup 4

*I LD_QUIT_4
BtnItem Enabled
265 10 285 60
Quit

*I LD_RUN_3
BtnItem Enabled
265 110 285 160
Run

*I LD_SUBMIT_3
BtnItem Enabled
DEFAULTBUTTON
265 210 285 260
Submit

*T CDLG
,5
*N LD_PERSPECTIVE
*S PR 0 PU 1
40 223 345 515
Perspective Data Loader
6

SetGroup 2

*I LD_TITLE_8
StatText Enabled
1 10 28 200
Select File(s)

*I LD_PERSPECTIVE_SELECT
Selector Enabled
32 10 261 250


*I LD_PERSPECTIVE_SCROLL
VScroll Enabled
31 250 262 270


SetGroup 4

*I LD_QUIT_5
BtnItem Enabled
265 10 285 60
Quit

*I LD_RUN_4
BtnItem Enabled
265 110 285 160
Run

*I LD_SUBMIT_4
BtnItem Enabled
DEFAULTBUTTON
265 210 285 260
Submit

