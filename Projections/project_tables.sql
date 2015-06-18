/* DEC/CMS REPLACEMENT HISTORY, Element PROJECT_TABLES.SQL */
/* *3     8-JUN-1991 16:14:17 ERIC "(SPR 1) Fixed error in set newpage 0 option." */
/* *2    29-SEP-1989 12:36:55 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:20:00 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element PROJECT_TABLES.SQL */
TTITLE '<----- Finder Project Tables ----->'
set newpage 0
set space 3
set lines 80
set pages 55
COL DESCRIPTION FORMAT A50 WORD_WRAPPED HEADING Description
COL TNAME FORMAT A25 HEADING 'Table Name'
SELECT
TNAME,
DESCRIPTION
FROM ESI.FINDER_TABLES
WHERE SCOPE = 'PROJECT'
order by tname
/
