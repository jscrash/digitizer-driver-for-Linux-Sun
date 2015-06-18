/* DEC/CMS REPLACEMENT HISTORY, Element TOPS_BY_NUMBER_SL.SQL */
/* *3    26-MAR-1990 11:39:04 DOROTHY "(SPR -1) Schema fix." */
/* *2    29-SEP-1989 12:44:23 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:37:18 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element TOPS_BY_NUMBER_SL.SQL */
SET NEWPAGE 0
SET LINES 80
SET PAGES 132

TTITLE 'Number of &1 Project Wells Having Formation Picks Using Control List &2'
 
COLUMN WELLS HEADING '# of Wells'

SELECT 
   FORM_CODE "Formation",  
   COUNT(DISTINCT A.UWI) WELLS
FROM &1..WELL_TOPS A, &2 B
WHERE A.UWI = B.UWI
GROUP BY FORM_CODE
ORDER BY COUNT(DISTINCT A.UWI) DESC
/
