/* DEC/CMS REPLACEMENT HISTORY, Element WELL_EXTENTS.SQL */
/* *6    26-NOV-1990 11:57:33 ERIC "(SPR 1) no change" */
/* *5    26-NOV-1990 11:54:39 ERIC "(SPR 1) Changed 'set spaces' to 'set space' to correct sysntax error." */
/* *4    24-MAY-1990 20:34:22 DOROTHY "(SPR 5182) NO CHANGES MADE" */
/* *3    26-MAR-1990 11:40:16 DOROTHY "(SPR -1) Schema fix." */
/* *2    29-SEP-1989 12:48:15 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:39:08 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_EXTENTS.SQL */
set newpage 0
set linesize 70
set pages 55
set space 4

ttitle 'Project &1 Well Location Extents'

select
   min(node_x) Xmin, 
   max(node_X) Xmax, 
   min(node_Y) Ymin, 
   max(node_y) Ymax
from
&1..well_hdr a, &1..nodes b
where
a.NODE_ID = b.node_id
/
