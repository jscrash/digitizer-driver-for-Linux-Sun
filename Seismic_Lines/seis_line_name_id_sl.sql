set termout off
remark	This report is dynamically executed from finder
remark	It shows the relationship between seismic line ids and
remark	the equivalent survey and line names
remark	Arguments:
remark		1 - project name
remark		2 - Seismic Select List name
remark		3 - Temporary table that contains seismic line ids

set space 2
set newpage 0
set pagesize 30
set linesize 45
set verify off

column line_id format 99999 heading 'Line Id #' justify left
column survey format a15 heading 'Seismic Survey'
column line_name format a15 heading 'Line Name'

ttitle center 'Seismic Line Identifiers For Project &1' skip -
center  'Using Control List &2' skip - 
center 'Sorted by Survey and Line Name' skip 2

break on survey skip 3

set termout on
select
survey, line_name, slh.line_id
from &1..seis_line_hdr slh, &3 temp_table
where slh.line_id = temp_table.line_id
order by survey, line_name
/
exit
