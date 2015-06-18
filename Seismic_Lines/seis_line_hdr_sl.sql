/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_HDR_SL.SQL */
/* *5     3-DEC-1990 14:55:03 ERIC "(SPR -1) no changes" */
/* *4    28-NOV-1990 17:45:38 ERIC "(SPR -1) no changes." */
/* *3    24-SEP-1990 17:15:44 GILLESPIE "Beta Deltas" */
/* *2     4-AUG-1990 15:43:55 DOROTHY "(SPR 5738) seismic report script" */
/* *1     4-AUG-1990 12:19:31 DOROTHY "seismic line header report script with select list" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_HDR_SL.SQL */
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
	&1..seis_line_hdr h,&2 T
where
	h.line_id = t.line_id
order by
	survey,
	line_name
/
