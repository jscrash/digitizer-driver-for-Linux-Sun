/* DEC/CMS REPLACEMENT HISTORY, Element WELL_HDR_BY_NAME_SL.SQL */
/* *2    26-MAR-1990 11:54:20 DOROTHY "(SPR 1) New SQL scripts" */
/* *1    26-MAR-1990 11:50:18 DOROTHY "SQL script" */
/* DEC/CMS REPLACEMENT HISTORY, Element WELL_HDR_BY_NAME_SL.SQL */
DOC
	ESI Well Header Report
	p1 = Project Name
	p2 = name of control list table
        p3 = name of original select list
#

CLEAR 	BREAKS;

TTITLE 	CENTER ' -->  --> &1 Well Header Using Control List "&3" <--  <--' SKIP CENTER 'Sorted by Well Name';

SELECT  A.WELL_NAME||' #'||A.WELL_NUMBER WELLNAME, A.OPERATOR,
B.SECTION||'-'||B.TOWNSHIP||B.NORTH_SOUTH||'-'||B.RANGE||B.EAST_WEST STR,
A.ELEVATION, A.ELEVATION_REF, A.DRILLERS_TD,A.UWI
FROM 	&1..WELL_HDR A, &1..WELL_LOCATIONS B, &2 C
WHERE	A.NODE_ID = B.NODE_ID(+) AND A.UWI = C.UWI
ORDER BY WELL_NAME, WELL_NUMBER;
