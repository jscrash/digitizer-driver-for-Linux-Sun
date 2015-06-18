*T CDLG
,1
*N MAKE_OR_HIGHLIGHT
*S PR 0 PU 1
64 9 400 520
Make or Highlight List
21

helpkey GLOBAL/MAKE_OR_HIGHLIGHT_LIST

*I MHI_RT2
Rectitem Enabled
35 144 255 300
make current list rect

*I MHI_RT1
Rectitem Enabled
80 8 205 106
data class rect

*I MHI_MESSAGE
StatText Enabled
5 8 35 488
message window

SETGROUP 1

*I MHI_ST1
StatText Disabled
40 12 70 100
Data Class:

*I MHI_WELLS
RadioItem Enabled
90 20 115 96
Wells

*I MHI_SEISMIC
RadioItem Enabled
115 20 140 96
Seismic

*I MHI_LYNX
RadioItem Enabled
140 20 165 96
Culture

*I MHI_LEASE
RadioItem Enabled
165 20 200 96
Lease

SETGROUP 2

*I MHI_ST2
StatText Disabled
40 160 70 288
Make Current List:

*I MHI_RELATIONAL
BtnItem Enabled
65 156 90 288
Relational

*I MHI_POINT
BtnItem Enabled
105 156 130 288
Point

*I MHI_BAND_RECTANGLE
BtnItem Enabled
145 156 170 288
Rectangle

*I MHI_POLYGON
BtnItem Enabled
185 156 210 288
Polygon

*I MHI_GET_LIST
BtnItem Enabled
225 156 250 288
Get List

*I MHI_HIGHLIGHT_CURRENT_LIST
BtnItem Enabled
65 332 90 488
Highlight Current List

*I MHI_ERASE_HIGHLIGHT
BtnItem Enabled
105 332 130 488
Erase Highlighting

*I MHI_SAVE_CURRENT_LIST
BtnItem Enabled
145 332 170 488
Save Current List

*I MHI_CLEAR_CURRENT
BtnItem Enabled
185 332 210 488
Clear Current List

*I MHI_DELETE
BtnItem Enabled
225 332 250 488
Delete List(s)

*I MHI_DISPLAY_STATUS
BtnItem Disabled
285 4 315 116
Display Status

*I MHI_DONE
BtnItem Disabled
DefaultButton
285 376 315 488
Done
