/* DEC/CMS REPLACEMENT HISTORY, Element TOPS_BY_MIN_MD.SQL */
/* *2    26-MAR-1990 11:54:06 DOROTHY "(SPR 1) New SQL scripts" */
/* *1    26-MAR-1990 11:49:19 DOROTHY "SQL script" */
/* DEC/CMS REPLACEMENT HISTORY, Element TOPS_BY_MIN_MD.SQL */
SET NEWPAGE 0
SET LINES 75
SET PAGES 55

TTITLE 'Number of &1 Project Wells Having Formation Picks'
 
COLUMN WELLS HEADING '# of Wells'
COLUMN MIN HEADING 'Min Depth'
COLUMN MAX HEADING 'Max Depth'

SELECT 
   FORM_CODE "Formation",  
   COUNT(DISTINCT UWI) WELLS,
   MIN(MD) MIN,
   MAX(MD) MAX
FROM &1..WELL_TOPS
GROUP BY FORM_CODE
ORDER BY MIN(MD)
/
