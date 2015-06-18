ttitle 'Project &1 Graphic Object Inventory'
set newpage 0
set pagesize 55
set linesize 120
set space 5
col object_name format a26 heading 'Object Type' word_wrap
col attribute_1 format a10 heading 'Horizon'
col source format a10 heading 'Source'
col lower_left_x format 99,999,999 heading 'X-min'
col lower_left_y format 99,999,999 heading 'Y-min'
col upper_right_x format 99,999,999 heading 'X-max'
col upper_right_y format 99,999,999 heading 'Y-max'
select
	object_name, 	
	source,
	attribute_1,
	lower_left_x, 
	lower_left_y,
	upper_right_x, 	
	upper_right_y
from 
	&1..graphic_objects
order by 
	object_name, 
	source, 
	attribute_1
/
/* DEC/CMS REPLACEMENT HISTORY, Element GRAPHIC_OBJECT_INFO.SQL*/
/* *2    20-AUG-1991 08:31:25 ERIC "(SPR 1) Initial checkin"*/
/* *1    20-AUG-1991 08:30:22 ERIC "SQL query which lists info about graphic objects."*/
/* DEC/CMS REPLACEMENT HISTORY, Element GRAPHIC_OBJECT_INFO.SQL*/
