/* DEC/CMS REPLACEMENT HISTORY, Element WELL_VEL_SL.SQL */
/* *4    26-NOV-1990 16:19:09 ERIC "(SPR 1) Added 'Heading' to UWI column." */
/* *3    26-MAR-1990 11:43:34 DOROTHY "(SPR -1) Schema fix." */
/* *2    29-SEP-1989 12:58:11 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:48:15 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_VEL_SL.SQL */
DOC
	ESI Well Velocity Report
	p1 = Project Name
	p2 = name of control list table
        p3 = name of original select list
#

CLEAR 	BREAKS;

BREAK ON WELLNAME SKIP 1
SET NEWPAGE 0
SET LINES 132
SET PAGES 55
COLUMN UWI FORMAT A20 HEADING 'Unique Identifier'
COLUMN WELLNAME FORMAT A30 HEADING 'Well Name'
COLUMN SOURCE FORMAT A10 HEADING 'Source'
COLUMN BEST_PICK HEADING '*' FORMAT A1
COLUMN LAST_UPDATE HEADING 'Last|Update'
COLUMN DATUM HEADING 'Datum|Elev.' FORMAT 99,999.9
COLUMN METHOD HEADING 'Method'
COLUMN VELOCITY HEADING 'Velocity' FORMAT 99,999.9
COLUMN VELOCITY_TYPE HEADING 'Type'
COLUMN TOP_FORM HEADING 'Top|Code'
COLUMN BASE_FORM HEADING 'Base|Code'
TTITLE 	CENTER ' -->  --> &1 Well Velocities Using Control List &3 <--  <--' SKIP CENTER 'Sorted by Well Name';

SELECT
  A.WELL_NAME||' #'||A.WELL_NUMBER WELLNAME,
  A.UWI,
  B.TOP_FORM, 
  B.BASE_FORM, 
  B.VELOCITY, 
  B.VELOCITY_TYPE, 
  B.SOURCE,
  B.METHOD, 
  B.DATUM, 
  B.BEST_PICK, 
  B.LAST_UPDATE
FROM 	&1..WELL_HDR A, &1..WELL_VELOCITY B, &2 C
WHERE	A.UWI = B.UWI 
AND 
A.UWI = C.UWI
ORDER BY A.WELL_NAME, A.WELL_NUMBER, VELOCITY;
