/* DEC/CMS REPLACEMENT HISTORY, Element WELL_TOPS_STATS.SQL */
/* *3    26-MAR-1990 11:43:01 DOROTHY "(SPR -1) Schema fix." */
/* *2    29-SEP-1989 12:57:03 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:45:06 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_TOPS_STATS.SQL */
SET NEWPAGE 0
SET LINESIZE 80
SET PAGESIZE 55
TTITLE '>------ &1 Well Top Data Statistics ------<'
select
a.UWI "Unique Identifier",
count(distinct form_code) "Number of Picks" 
from &1..well_tops A 
group by A.uwi
order by count(distinct form_code) desc
/
