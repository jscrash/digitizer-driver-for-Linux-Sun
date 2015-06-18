set newpage 0
set pagesize 55
set linesize 80
ttitle center 'Project &1 Graphic Object Inventory' skip 2
col object_name format a11 heading 'Object Type' word_wrap
col attribute_1 format a10 heading 'Horizon'
col source format a10 heading 'Source'
col lower_left_x format 9,999,999 heading 'X-min'
col lower_left_y format 99,999,999 heading 'Y-min'
col upper_right_x format 9,999,999 heading 'X-max'
col upper_right_y format 99,999,999 heading 'Y-max'
select
object_name, source,
attribute_1,
lower_left_x, lower_left_y,
upper_right_x, upper_right_y
from &1..graphic_objects
order by object_name, source, attribute_1
/
/* DEC/CMS REPLACEMENT HISTORY, Element GRAPHIC_OBJECT_INVENTORY.SQL*/
/* *4    14-JUN-1991 12:47:26 ERIC "(SPR 1) reformatted"*/
/* *3    18-DEC-1990 09:46:10 ERIC "(SPR -1) corrected script"*/
/* *2    18-DEC-1990 09:27:43 ERIC "(SPR -1) Report checked in."*/
/* *1    18-DEC-1990 09:26:27 ERIC "report to facilitate management and display of graphic objects."*/
/* DEC/CMS REPLACEMENT HISTORY, Element GRAPHIC_OBJECT_INVENTORY.SQL*/
