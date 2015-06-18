ttitle 'Project &&1 Seismic Interpretation Locations'

set newpage 0
set linesize 132
set pagesize 55
set space 5

col surface_code format a10 heading 'Surface|Code'
col source format a10 heading 'Source'
col nlines format 999 heading '# Lines'
col minx format 9,999,999 heading 'X-min.'
col miny format 99,999,999 heading 'Y-min.'
col maxx format 9,999,999 heading 'X-max.'
col maxy format 99,999,999 heading 'Y-max.'
col display_name format a10 heading 'Data|Type'
col surface format a10 heading 'Surface|Type'

SELECT
	decode(surface_type,1,'Horizon',2,'Fault') surface,
	display_name,
	surface_code, 
	ss.source,
	count(ss.line_id) nlines,
	MIN(LOWER_LEFT_X) minx, 
	MIN(LOWER_LEFT_Y) miny,
	MAX(UPPER_RIGHT_X) maxx, 	
	MAX(UPPER_RIGHT_Y) maxy
FROM 
	&&1..SEIS_LINE_HDR slh, 
	&&1..seis_surface ss,
	&&1..seis_surface_codes ssc
where 
	ss.line_id = slh.line_id
and
	ss.data_type = ssc.data_type
group by 
	surface_type,
	display_name,
	surface_code,
	ss.source	
/
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_LOC_BY_HORIZ_INT.SQL*/
/* *3    20-AUG-1991 16:09:52 ERIC "(SPR 1) Fix"*/
/* *2    20-AUG-1991 12:30:34 ERIC "(SPR 1) Initial checkin."*/
/* *1    20-AUG-1991 12:29:32 ERIC "SQL query that displays geographic extents of seis surface data."*/
/* DEC/CMS REPLACEMENT HISTORY, Element SEIS_LINE_LOC_BY_HORIZ_INT.SQL*/
