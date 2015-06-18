*T CDLG 
,128
*N PL_META_CDLG
*S PR 0 PU 1
43 30 280 440 
Plot Preview
5

*I PL_TEXT1
StatText Enabled
2 9 32 350 
Select plot file under ESI$PLOTS to preview  

*I PL_QUIT
BtnItem Enabled
200 90 225 174 
QUIT

*I PL_PREVIEW
BtnItem Enabled
200 217 225 296 
PREVIEW

Setgroup 1
*I PL_META_SELECTOR
Selector Enabled
35 8 182 360 
<Plot_File_1>
<Plot_File_2>
<Plot_File_3>
<Plot_File_4>
<Plot_File_5>
<Plot_File_6>
<Plot_File_7>

*I PL_META_VSCROLL
Vscroll Enabled
35 360 182 389 

