/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_GEN_NODE_IDS.SQL */
/* *2    18-JUL-1991 08:23:38 ALLAWAY "(SPR 8648) INITIAL CHECKIN" */
/* *1    18-JUL-1991 08:22:31 ALLAWAY "SCRIPT TO GENERATE NODE_IDS FOR LOCATIONS LOADED WITH THE SQLLOADER" */
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_GEN_NODE_IDS.SQL */
UTDBA_GEN_NODE_IDS.SQL:

UPDATE WELL_HDR
SET NODE_ID = ESI.NODE_ID_SEQ.NEXTVAL;

UPDATE LOADER_NODES N
SET N.NODE_ID = (SELECT H.NODE_ID FROM WELL_HDR H WHERE H.UWI=N.UWI);

UPDATE LOADER_WELL_LOCATIONS N
SET N.NODE_ID = (SELECT H.NODE_ID FROM WELL_HDR H WHERE H.UWI=N.UWI);

INSERT INTO NODES (NODE_ID,SOURCE,LOC_QUAL,NODE_X,NODE_Y,LATITUDE,LONGITUDE)
SELECT NODE_ID,SOURCE,LOC_QUAL,NODE_X,NODE_Y,LATITUDE,LONGITUDE
FROM LOADER_NODES;

INSERT INTO WELL_LOCATIONS(NODE_ID,SOURCE,TOWNSHIP,NORTH_SOUTH,RANGE,EAST_WEST, SECTION)
SELECT NODE_ID,SOURCE,TOWNSHIP,NORTH_SOUTH,RANGE,EAST_WEST,SECTION
FROM LOADER_WELL_LOCATIONS;
