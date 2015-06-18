/* DEC/CMS REPLACEMENT HISTORY, Element CODES_TABLES.SQL */
/* *3     8-JUN-1991 16:09:55 ERIC "(SPR 1) Fixed error in newpage 0 option." */
/* *2    29-SEP-1989 12:01:47 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 10:58:03 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element CODES_TABLES.SQL */
TTITLE '<----- Finder Validation Code Tables ----->'
set newpage 0
set space 3
set lines 80
set pages 55
COL DESCRIPTION FORMAT A50 WORD_WRAPPED HEADING 'Description'
COL TNAME FORMAT A25 Heading 'Validation Table Name'
SELECT
TNAME,
DESCRIPTION
FROM ESI.FINDER_TABLES
WHERE SCOPE = 'CODE'
order by tname
/
