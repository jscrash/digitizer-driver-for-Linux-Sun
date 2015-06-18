ttitle 'Project &1 Seismic Data Report'

set newpage 0
set pagesize 55
set linesize 130
set space 3

break on surface_type skip 4 nodup -
	on surface_code nodup -
	on display_name nodup

col stype format a10 heading 'Surface|Type'
col surface_code format a10 heading 'Surface|Code'
col display_name format a20 heaDING 'Data|Type'
col source format a14 heading 'Source'
col last_update format a10 heading  'Updated'
col first_cdp format 99999 heading 'First|CDP#'
col last_cdp format 99999 heading 'Last|CDP#'
col survey format a10 heading Survey
col line_name format a10 heading 'Line Name'

select
	decode(surface_type,1,'HORIZON',2,'FAULT','UNKNOWN') stype,
	display_name,
	surface_code,
	ss.source,
	line_name, 
	survey,
	last_update,
	first_cdp,
	last_cdp
from 
	&1..seis_surface  ss, 
	&1..seis_line_hdr slh,
	&1..seis_surface_codes ssc
where 
	ss.line_id = slh.line_id
and
	ssc.data_type = ss.data_type
order by
	surface_type,
	display_name,
	surface_code,
	ss.source
/
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_SURFACE_INFO.SQL*/
/* *2    20-AUG-1991 08:57:57 ERIC "(SPR 1) Replaces seis_surf_info.sql which should be deleted from cms."*/
/* *1    20-AUG-1991 08:56:40 ERIC "SQL query inventorying project seismic data."*/
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_SURFACE_INFO.SQL*/
