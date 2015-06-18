ttitle 'Project &1 Plot Layout Descriptions'
set newpage 0
set pagesize 40
set linesize 80
break on layout_type skip page
break on layout_name skip 3
col layout_type format a10 heading 'Layout|Type'
col layout_name format a20 heading 'Layout Name'
col window_name format a20 heading 'Window Name' word_wrap
col window_type format a15 heading 'Window Type'
select layout_type, layout_name, window_name, window_type
from &1..plot_layout_win
order by layout_type, layout_name
/
/* DEC/CMS REPLACEMENT HISTORY, Element PLOT_LAYOUT.SQL*/
/* *2    20-AUG-1991 08:55:25 ERIC "(SPR 1) Initial checkin"*/
/* *1    20-AUG-1991 08:54:31 ERIC "SQL query listing project plot layouts."*/
/* DEC/CMS REPLACEMENT HISTORY, Element PLOT_LAYOUT.SQL*/
