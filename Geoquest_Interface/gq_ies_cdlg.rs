*T CDLG
,102
*N IES_CDLG
*S PR 0 PU 1
39 5 450 465 
Load IES Project Setup
20

*I IES_MSG
StatText Enabled
1 3 41 78
Message:

*I IES_MSG_TXT
StatText Enabled
1 80 41 458 
Select Wells and Seismic Select Lists, Enter IES project,

*I IES_MSG_TXT_2
StatText Enabled
28 80 55 231 
Then Click Export.

*I IES_WELLS_TITLE
StatText Enabled
50 30 75 198 
Wells Select List

SetGroup 1
*I IES_WELLS_LIST
Selector Disabled
71 30 175 194


*I IES_WELLS_SCROLL
VScroll Disabled
71 197 175 211


SetGroup 2
*I IES_WELLS_CUR
EditText Disabled
175 32 207 200 

SetGroup 3
*I IES_SEIS_TITLE
StatText Disabled
50 219 75 383
Seismic Select List

SetGroup 4
*I IES_SEIS_LIST
Selector Disabled
71 219 175 383 


*I IES_SEIS_SCROLL
VScroll Disabled
71 386 175 399


SetGroup 5
*I IES_SEIS_CUR
EditText Disabled
175 221 207 390 


SetGroup 6
*I IES_USER_PROMPT
StatText Disabled
215 81 245 210 
IES User Name:

*I IES_PASSWD_PROMPT
StatText Disabled
255 81 285 210 
IES Password:

*I IES_PROJ_PROMPT
StatText Disabled
295 81 325 210 
IES Project Name:

*I IES_USER
EditText Enabled
215 221 247 390 

*I IES_PASSWORD
EditText Enabled
255 221 287 390 

*I IES_PROJECT
EditText Enabled
295 221 327 390

*I IES_NOTE
StatText Disabled
335 51 363 420 
Note: The IES Project must already exist before exporting Finder data.

*I IES_EXPORT
BtnItem Enabled
375 270 400 348 
Export

*I IES_QUIT
BtnItem Enabled
375 94 400 170 
Quit

