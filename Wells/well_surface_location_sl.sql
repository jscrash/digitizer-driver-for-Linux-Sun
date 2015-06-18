/* DEC/CMS REPLACEMENT HISTORY, Element WELL_SURFACE_LOCATION_SL.SQL */
/* *3    26-MAR-1990 11:42:04 DOROTHY "(SPR -1) Schema fix." */
/* *2    29-SEP-1989 12:56:12 ALLAWAY "(SPR 5011) SQL SCRIPT FOR FINDER REPORTS - NEW SCHEMA" */
/* *1    29-SEP-1989 11:42:28 ALLAWAY "SQL SCRIPT FOR FINDER REPORTS" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_SURFACE_LOCATION_SL.SQL */
DOC
	ESI Well Latlong Report
	p1 = Project Name
	p2 = name of control list table
        p3 = name of original select list
#

CLEAR 	BREAKS;

BREAK ON WELLNAME SKIP 1

SET NEWPAGE 0
SET LINESIZE 132
SET PAGESIZE 55
COLUMN UWI FORMAT A20 HEADING 'Unique Identifier'
COLUMN WELLNAME FORMAT A30 HEADING 'Well Name'
COL NODEX FORMAT 99,999,999.9 HEADING 'Node X'
COL NODEY FORMAT 99,999,999.9 HEADING 'Node Y'
COL XOFFSET FORMAT 999,999.9 HEADING 'X Offset'
COL YOFFSET FORMAT 999,999.9 HEADING 'Y Offset'
COL WELLX FORMAT 99,999,999.9 HEADING 'Well X'
COL WELLY FORMAT 99,999,999.9 HEADING 'Well Y'
COL LATITUDE FORMAT 999.99999 HEADING 'Latitude'
COL LONGITUDE FORMAT 9999.99999 HEADING 'Longitude'
TTITLE 	CENTER ' -->  --> &1 Well Locations <--  <--' SKIP CENTER 'Sorted by Well Name';
TTITLE  CENTER ' -->  --> Using Control List &3 <--  <--'
SELECT
A.UWI,
B.NODE_X NODEX,
B.NODE_Y NODEY,
A.TOP_DELTA_X XOFFSET,
A.TOP_DELTA_Y YOFFSET,
B.NODE_X + A.TOP_DELTA_X WELLX,
B.NODE_Y + A.TOP_DELTA_Y WELLY,
B.LATITUDE,
B.LONGITUDE
FROM 	&1..WELL_HDR A, &1..NODES B, &2 C
WHERE	A.NODE_ID = B.NODE_ID
AND
A.UWI = C.UWI
ORDER BY A.WELL_NAME, A.WELL_NUMBER;
