*T CDLG
,128
*N VI_DATASEL
*S PR 0 PU 1
45 5 340 440
Velocity Interpolation Data Selection
8

*I VI_SEIS_LIST
StatText Enabled
56 15 76 200
Seismic List :

*I VI_HORIZ
StatText Enabled
56 246 76 330
Horizons :

*I VI_DS_OK
BtnItem Enabled
255 305 281 375 
O K

*I VI_DS_CANCEL
BtnItem Enabled
255 35 281 105 
Cancel

Setgroup 1
*I VI_SEIS_SELECTOR
Selector Enabled
85 5 230 205

*I VI_SEIS_VSCROLL
Vscroll Enabled
86 208 231 221

Setgroup 2
*I VI_HOR_SELECTOR
Selector Enabled
85 236 230 388

*I VI_HOR_VSCROLL
Vscroll Enabled
86 390 231 410 

