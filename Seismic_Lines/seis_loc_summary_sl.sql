/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOC_SUMMARY_SL.SQL */
/* *2     4-AUG-1990 15:40:16 DOROTHY "(SPR 5738) seismic report script" */
/* *1     4-AUG-1990 12:22:19 DOROTHY "seismic line location summary report with select list" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOC_SUMMARY_SL.SQL */
ttitle 'Seismic Line Summary for Project &1 Using List &3'

set newpage 0
set lines 80
set pages 24
set space 2

break on survey skip 2

col line_name format a12 heading 'Line Name'
col survey format a20 heading 'Survey Name'

select
	min(lower_left_x) min_x,
	min(lower_left_y) min_y,
	max(upper_right_x) max_x,
	max(upper_right_y) max_y
from
	&1..seis_line_hdr h, &2 a
where 
	h.line_id = a.line_id
/
