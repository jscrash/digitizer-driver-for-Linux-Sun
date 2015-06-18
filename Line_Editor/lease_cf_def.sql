ttitle 'Project &1 Lease Color Fill Definition Report'

set newpage 0
set pagesize 35
set linesize 90
set space 4

break on definition_name skip 4 nodup -
	on deftype nodup -
	on contype nodup -
	on dateform nodup


col definition_name format a15 heading 'Definition Name'
col deftype format a9 heading 'Data|Type'
col contype format a8 heading 'Control|Type'
col dateform format a8 heading 'Date|Format'
col range format a5 heading 'Range|Flag'
col starting_value format a20 heading 'Data Range'
col color_fill_code format a10 heading 'Color'

select
	lcdf.definition_name,
	decode(lcdf.definition_type,0,'Date',1,'Text',3,'Numeric') deftype,
	decode(lcdf.control_type,4,'Color',5,'Pattern') contype,
	decode(date_format,6,'MM/DD/YY',7,'DD/MM/YY',8,'MM/YY',9,'YY') dateform,
	starting_value, 
	color_fill_code
from
	&1..lease_color_fill_def lcdf,
	&1..lease_color_fill_def_entries lcdfe
where
	lcdf.definition_name = lcdfe.definition_name
order by
	lcdf.definition_name,
	starting_value
/
/* DEC/CMS REPLACEMENT HISTORY, Element LEASE_CF_DEF.SQL*/
/* *2    19-AUG-1991 15:09:49 ERIC "(SPR 1) Initial checkin."*/
/* *1    19-AUG-1991 15:09:01 ERIC "SQL query listing LEASE color/fill definitions."*/
/* DEC/CMS REPLACEMENT HISTORY, Element LEASE_CF_DEF.SQL*/
