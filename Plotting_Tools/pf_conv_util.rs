*T CDLG
,128
*N PF_MP_CONV_UTIL
*S PR 0 PU 1
30 40 535 525 
Map Plot File Conversion Utility
31

*I PF_MP_PLOT_FILE_NAME_LBL
StatText Disabled
6 10 33 140 
Plot File Name:

*I PF_MP_PLOT_FILE_NAME
EditText Enabled
6 140 39 440 
abcdefghijklmnopqrstuvxyz1234567890

*I PF_MP_OS_PATHNAME_LBL
StatText Disabled
39 10 91 140 
Original 
OS Pathname :

*I PF_MP_OS_PATHNAME
EditText Enabled
49 140 82 440 
$FD/EUREKA/plots/...

*I PF_MP_PLOT_FILE_FORMAT_LBL
StatText Disabled
105  200 132 320
Plot File Format:

*I PF_MP_PLOT_FILE_FORMAT
OptionMenu Enabled
105 320 132 440

*I PF_MP_PLOT_LAYOUT_NAME_LBL
StatText Disabled
145 10 172 140
Plot Layout Name:

*I PF_MP_PLOT_LAYOUT_NAME
EditText Enabled
145 140 180 440
NA_SB_TB	

*I PF_MP_MAP_NAME_LBL
StatText Disabled
180 10 207 100
Map Name: 

*I PF_MP_MAP_NAME
EditText Enabled
180 140 213 440

*I PF_MP_MAP_SCALE_LBL
StatText Disabled
213 10 240 100
Map Scale: 

*I PF_MP_MAP_SCALE
EditText Enabled
213 140 246 360
2000 M/M

*I PF_MP_MAP_SCALE_FORMAT_LBL
StatText Disabled
213 360 246 500
Format: 2000 M/M 

*I PF_MP_MAP_CORNER_LL_LBL
StatText Disabled
246 10 273 140
Map Corners - LL:

*I PF_MP_MAP_CORNER_LL
EditText Enabled
246 140 280 360
123456,234567

*I PF_MP_MAP_MAP_CORNER_LL_FORMAT_LBL
StatText Disabled
246 360 273 495
Format: X,Y

*I PF_MP_MAP_CORNER_UR_LBL
StatText Disabled
280 100 307 140
UR:

*I PF_MP_MAP_CORNER_UR
EditText Enabled
280 140 316 360
123456,234567

*I PF_MP_MAP_CORNER_UR_FORMAT_LBL
StatText Disabled
280 360 307 495
Format: X,Y

*I PF_MP_MAP_PROJECTION_LBL
StatText Disabled
316 10 343 140
Map Projection:

*I PF_MP_MAP_PROJECTION
OptionMenu Enabled
321 140 348 350

*I PF_MP_PERMANENT
ChkItem Enabled
348 10 375 140
Permanent

*I PF_MP_EXPIRATION_DATE_LBL
StatText Disabled
348 120 375 230
Expiration Date:

*I PF_MP_EXPIRATION_DATE
EditText Enabled
348 230 381 350

*I PF_MP_EXPIRATION_DATE_FORMAT
StatText Disabled
348 353 380 480
(DD-MON-YYYY)

*I PF_MP_DESCRIPTION_LBL
StatText Disabled
382 10 409 140
Description:

*I PF_MP_DESCRIPTION
EditText Enabled
382 140 439 440

*I PF_MP_QUIT
BtnItem Enabled
450 100 490 180
QUIT

*I PF_MP_CONVERT
BtnItem Enabled
450 300 490 380
CONVERT

Setgroup 1

*I PF_MP_PLOT_FILE_WITH_LAYOUT
RadioItem Enabled
84 10 106 170
Plot File with Plot Layout

*I PF_MP_PLOT_FILE_WITH_NO_LAYOUT
RadioItem Enabled
111 10 140 180
Plot File without Plot Layout









*T CDLG
,129
*N PF_XS_CONV_UTIL
*S PR 0 PU 1
30 40 400 525 
Cross Section Plot File Conversion Utility
18

*I PF_XS_PLOT_FILE_NAME_LBL
StatText Disabled
6 10 33 140 
Plot File Name:

*I PF_XS_PLOT_FILE_NAME
EditText Enabled
6 140 39 440 
abcdefghijklmnopqrstuvxyz1234567890

*I PF_XS_OS_PATHNAME_LBL
StatText Disabled
39 10 91 140 
Original 
OS Pathname :

*I PF_XS_OS_PATHNAME
EditText Enabled
49 140 82 440 
$FD/EUREKA/plots/...

*I PF_XS_PLOT_LAYOUT_NAME_LBL
StatText Disabled
88 10 115 140
Plot Layout Name:

*I PF_XS_PLOT_LAYOUT_NAME
EditText Enabled
88 140 120 440
NA_SB_TB	

*I PF_XS_LAYOUT_NAME_LBL
StatText Disabled
124 10 151 160
Xsect Layout Name: 

*I PF_XS_LAYOUT_NAME
EditText Enabled
124 140 156 440

*I PF_XS_PERMANENT
ChkItem Enabled
160 10 187 140
Permanent

*I PF_XS_PLOT_FILE_FORMAT_LBL
StatText Disabled
160 200 187 320
Plot File Format:

*I PF_XS_PLOT_FILE_FORMAT
OptionMenu Enabled
160 320 187 440

*I PF_XS_EXPIRATION_DATE_LBL
StatText Enabled
190 10 217 120
Expiration Date:

*I PF_XS_EXPIRATION_DATE
EditText Enabled
190 140 222 270 

*I PF_XS_EXPIRATION_DATE_FORMAT
StatText Disabled
190 285 217 480
(DD-MON-YYYY)

*I PF_XS_DESCRIPTION_LBL
StatText Disabled
225 10 252 140
Description:

*I PF_XS_DESCRIPTION
EditText Enabled
225 140 279 440

*I PF_XS_QUIT
BtnItem Enabled
290 100 330 180
QUIT

*I PF_XS_CONVERT
BtnItem Enabled
290 300 330 380
CONVERT
