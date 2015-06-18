/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOCS_BY_SRVY_SL.SQL */
/* *3    28-NOV-1990 17:42:51 ERIC "(SPR -1) Modified to use select list." */
/* *2     4-AUG-1990 15:41:57 DOROTHY "(SPR 5738) seismic report script" */
/* *1     4-AUG-1990 12:20:54 DOROTHY "seismic location by survey report script with select list" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOCS_BY_SRVY_SL.SQL */
ttitle 'Seismic Line Summary for Project &1 Using Control List &3'

set newpage 0
set lines 80
set pages 24
set space 1

break on survey skip 1

col line_name format a12 heading 'Line Name'
col survey format a20 heading 'Survey Name'

select
	survey,
	min(lower_left_x) min_x,
	min(lower_left_y) min_y,
	max(upper_right_x) max_x,
	max(upper_right_y) max_y
from
	&1..seis_line_hdr h, &2 b
where
	h.line_id = b.line_id
group by survey
order by survey
/
