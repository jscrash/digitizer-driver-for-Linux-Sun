/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_NAME_ID.SQL */
/* *3    14-JUN-1991 13:08:30 ERIC "(SPR 1) Changed select list variable to project variable." */
/* *2     6-DEC-1990 14:13:18 ERIC "(SPR -1) removed unneeded sort parameter." */
/* *1     6-DEC-1990 13:21:45 ERIC "Displays line_id equivalent to survey and line_name." */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_NAME_ID.SQL */
break on survey skip 2
set space 2
set newpage 0
set pagesize 30
set linesize 45
column line_id format 99999 heading 'Line Id #' justify left
column survey format a15 heading 'Seismic Survey'
column line_name format a15 heading 'Line Name'
ttitle center 'Seismic Line Identifiers For Project &1' skip - 
center 'Sorted by Survey and Line Name' skip 2
select
survey, line_name, line_id
from &1..seis_line_hdr
order by survey, line_name
/
