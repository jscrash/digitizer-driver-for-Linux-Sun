/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_HDR.SQL */
/* *2     4-AUG-1990 15:27:34 DOROTHY "(SPR 5738) New seismic report" */
/* *1     4-AUG-1990 12:18:48 DOROTHY "seismic line header report script" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_HDR.SQL */
TTITLE 'Seismic Line Summary for Project &1'

set newpage 0
set lines 80
set pages 24
set space 2

break on survey skip 2

col line_name format a12 heading 'Line Name'
col survey format a20 heading 'Survey Name'
col initial_cdp format 99999 heading 'First|CDP #'
col final_cdp format 99999 heading 'Last|CDP #'
col initial_shot format 99999 heading 'First|Shot'
col final_shot format 99999 heading 'Last|Shot'
col cdps_per_shot_point format 99999 heading 'CDP/|Shot'

select
	survey,
	line_name,
	initial_cdp,
	final_cdp,
	initial_shot,
	final_shot,
	cdps_per_shot_point
from
	&1..seis_line_hdr h
order by
	survey,
	line_name
/
