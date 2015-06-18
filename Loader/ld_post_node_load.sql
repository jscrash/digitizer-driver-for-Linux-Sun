/* DEC/CMS REPLACEMENT HISTORY, Element LD_POST_NODE_LOAD.SQL */
/* *5    16-JUL-1990 18:36:50 ALLAWAY "(SPR -1) CHECKIN FOR 7-15-90 SCHEMA UPDATE" */
/* *4    23-APR-1990 14:26:39 PURNA "(SPR 0) Dorothy's enron mods" */
/* *3     6-APR-1990 16:51:50 ALLAWAY "(SPR -1) Bug fixes" */
/* *2    23-MAR-1990 21:22:16 DOROTHY "(SPR -1) CHECKIN" */
/* *1    23-MAR-1990 21:18:01 DOROTHY "WHCS NODE LOADER POST SCRIPT." */
/* DEC/CMS REPLACEMENT HISTORY, Element LD_POST_NODE_LOAD.SQL */
DOC;
                      LD_POST_NODE_LOAD.SQL
		      BILL ALLAWAY 3/16/90

Updating of loaded data that requires that the nodes data be loaded before
executing it will do any good.


LOADER_WELL_LOCATIONS - GET NODE_IDs then insert data into WELL_LOCATIONS...
#
	UPDATE LOADER_WELL_LOCATIONS L SET
		NODE_ID = (SELECT NODE_ID FROM WELL_HDR H
				WHERE H.UWI = L.UWI);
 COMMIT;

	INSERT INTO WELL_LOCATIONS(
         NODE_ID,
         SOURCE,
         TOWNSHIP,
         NORTH_SOUTH,
         RANGE,
         EAST_WEST,
         SECTION,
         SECTION_TYPE,
         SPOT_CODE,
         PRINCIPAL_MERIDIAN,
         NS_FOOTAGE,
         NS,
         EW_FOOTAGE,
         EW,
         DESCRIPTION)
	SELECT
         DISTINCT NODE_ID,
         SOURCE,
         TOWNSHIP,
         NORTH_SOUTH,
         RANGE,
         EAST_WEST,
         SECTION,
         SECTION_TYPE,
         SPOT_CODE,
         PRINCIPAL_MERIDIAN,
         NS_FOOTAGE,
         NS,
         EW_FOOTAGE,
         EW,
         DESCRIPTION
	FROM LOADER_WELL_LOCATIONS
	WHERE NODE_ID IS NOT NULL
	AND SOURCE IS NOT NULL;



DOC;
LOADER_TEXAS_LOC - GET NODE_IDs then insert data into WELL_TEXAS_LOC...
#
	UPDATE LOADER_TEXAS_LOC L SET
		NODE_ID = (SELECT NODE_ID FROM WELL_HDR H
				WHERE H.UWI = L.UWI);
 COMMIT;
	INSERT INTO WELL_TEXAS_LOC(
         NODE_ID,
         SOURCE,
         SURVEY,
         BLOCK,
         ABSTRACT_NUMBER,
         LOCATION_DESC)
	SELECT
         NODE_ID,
         SOURCE,
         SURVEY,
         BLOCK,
         ABSTRACT_NUMBER,
         LOCATION_DESC
	FROM LOADER_TEXAS_LOC;
 COMMIT;
EXIT;

