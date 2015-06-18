/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_DUMP_TOPS.SQL */
/* *4    21-DEC-1990 14:31:47 ALLAWAY "(SPR 5528) PERFORMANCE OVERHAUL" */
/* *3    23-APR-1990 14:26:50 PURNA "(SPR 0) Dorothy's enron mods" */
/* *2     6-APR-1990 17:03:21 ALLAWAY "(SPR -1) new file for tops loading" */
/* *1     6-APR-1990 17:02:12 ALLAWAY "script to make a loadable file of the tops loaded from the bad file" */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_WHCS_DUMP_TOPS.SQL */
SET TERMOUT OFF;
SET PAGESIZE 10000;
SET LINESIZE 132;
SET HEAD OFF;
SET FEED OFF;
COLUMN UWI FORMAT A20;
COLUMN FORM_CODE FORMAT A12;
COLUMN SOURCE FORMAT A12;
COLUMN AGE_CODE FORMAT A6;
COLUMN LAST_UPDATE FORMAT A12;
COLUMN MD FORMAT 99999.99;
COLUMN TVD FORMAT 99999.99;
COLUMN DESC_CODE FORMAT A4;
SPOOL LOADER_WELL_TOPS.DAT;
SELECT UWI,SOURCE,FORM_CODE,AGE_CODE,MD,TVD,DESC_CODE,LAST_UPDATE
FROM LOADER_WELL_TOPS ORDER BY UWI,MD;
SPOOL OFF;
EXIT;
