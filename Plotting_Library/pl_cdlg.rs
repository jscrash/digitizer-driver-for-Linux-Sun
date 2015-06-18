*T CDLG 
,128
*N PL_CDLG
*S PR 0 PU 1
43 30 280 444 
Retrieve Layout and Plotter
8

*I PL_TEXT1
StatText Enabled
2 9 32 141 
Layout:

*I PL_TEXT2
StatText Disabled
2 200 32 300 
Plotter:

*I PL_CANCEL
BtnItem Enabled
195 90 220 174 
CANCEL

*I PL_OK
BtnItem Enabled
195 217 220 296 
OK

Setgroup 1
*I PL_LAYOUT_SELECTOR
Selector Enabled
35 8 182 160 
<Layout_1>
<Layout_2>
<Layout_3>
<Layout_4>
<Layout_5>
<Layout_6>
<Layout_7>

*I PL_LAYOUT_VSCROLL
Vscroll Enabled
35 171 182 189 

Setgroup 2
*I PL_PLOTTER_SELECTOR
Selector Enabled
35 202 182 371 
<Plotter_1>
<Plotter_2>
<Plotter_3>
<Plotter_4>
<Plotter_5>
<Plotter_6>
<Plotter_7>

*I PL_PLOTTER_VSCROLL
Vscroll Enabled
35 380 182 398 



