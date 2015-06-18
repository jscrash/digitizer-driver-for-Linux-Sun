/* DEC/CMS REPLACEMENT HISTORY, Element WELL_EXTENTS_SL.SQL */
/* *6    26-NOV-1990 12:00:52 ERIC "(SPR 1) corrected syntax errors (space(s), a.uwi=c.uwi." */
/* *5    24-MAY-1990 20:35:23 DOROTHY "(SPR 5182) NO CHANGES MADE." */
/* *4    26-MAR-1990 11:40:23 DOROTHY "(SPR -1) Schema fix." */
/* *3    26-JAN-1990 14:39:46 MING "(SPR 0) change a.NODE_ID = c.uwi to a.NODE_ID = c.NODE_ID for unique node_id " */
/* *2    29-SEP-1989 12:50:01 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:39:33 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_EXTENTS_SL.SQL */
set newpage 0
set linesize 70
set pages 55
set space 4

ttitle 'Project &1 Well Location Extents Using Control List &3'

select
   min(node_x) Xmin, 
   max(node_X) Xmax, 
   min(node_Y) Ymin, 
   max(node_y) Ymax
from
   &1..well_hdr a, 
   &1..nodes b, 
   &2 c
where
   a.NODE_ID = b.node_id 
        and 
   a.uwi = c.uwi
/
