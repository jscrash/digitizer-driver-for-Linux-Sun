/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOCS_BY_SRVY.SQL */
/* *4    14-JUN-1991 13:10:48 ERIC "(SPR 1) Removed control list variable." */
/* *3     3-DEC-1990 14:58:39 ERIC "(SPR -1) added select list capability." */
/* *2     4-AUG-1990 15:42:55 DOROTHY "(SPR 5738) seismic report script" */
/* *1     4-AUG-1990 12:20:15 DOROTHY "seismic locations by survey report script" */
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LOCS_BY_SRVY.SQL */
ttitle 'Seismic Line Summary for Project &1'

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
	&1..seis_line_hdr h
group by survey
order by survey
/
