/* DEC/CMS REPLACEMENT HISTORY, Element LYNX_DELETE_TYPE.SQL */
/* *5     2-AUG-1991 17:28:36 PURNA "(SPR 0) fixed missing keyword" */
/* *4    11-JUN-1991 13:15:20 PURNA "(SPR 8192) add missing tables" */
/* *3    12-NOV-1990 15:53:17 MING "(SPR 6154) add 4 lines at the end" */
/* *2     9-APR-1990 17:51:21 VINCE "(SPR 1) delete a lynx data type including all objects of that type." */
/* *1     9-APR-1990 17:50:24 VINCE "delete a lynx data type including all objects of that type." */
/* DEC/CMS REPLACEMENT HISTORY, Element LYNX_DELETE_TYPE.SQL */
/*
 * Delete a data type from the Lynx culture tables.
 *
 * Start this procedure with the name of the project and the data
 * type to be removed.
 */

DELETE	FROM 	&1..LYNX_TEXT_GC
	WHERE 	TEXT_ID IN (SELECT DISTINCT A.TEXT_ID FROM
		 	    &1..LYNX_TEXT A, &1..LYNX_CULTURE B
			    WHERE A.LYNX_ID = B.LYNX_ID AND
				  DATA_TYPE = '&2');
COMMIT;

DELETE	FROM	&1..LYNX_TEXT
	WHERE	LYNX_ID IN (SELECT  DISTINCT LYNX_ID
			    FROM    &1..LYNX_CULTURE
			    WHERE   DATA_TYPE = '&2');
COMMIT;

DELETE	FROM	&1..LYNX_OBJECTS
	WHERE	LYNX_ID IN (SELECT  DISTINCT LYNX_ID
			    FROM    &1..LYNX_CULTURE
			    WHERE   DATA_TYPE = '&2');
COMMIT;

DELETE	FROM	&1..LYNX_IFV
	WHERE	ARC_ID IN (SELECT DISTINCT A.LYNX_PART FROM 
			   &1..LYNX_LRFR A, &1..LYNX_CULTURE B
			    WHERE   A.LYNX_ID = B.LYNX_ID AND
				    B.DATA_TYPE = '&2');
COMMIT;

DELETE	FROM	&1..LYNX_LRFR
	WHERE	LYNX_ID IN (SELECT  DISTINCT LYNX_ID
			    FROM    &1..LYNX_CULTURE
			    WHERE   DATA_TYPE = '&2');
COMMIT;

DELETE	FROM	&1..SECTIONS
	WHERE	LYNX_ID IN (SELECT  DISTINCT LYNX_ID
			    FROM    &1..LYNX_CULTURE
			    WHERE   DATA_TYPE = '&2');
COMMIT;

DELETE  FROM 	&1..FM_HDR
	WHERE 	FINDER_FILENAME = '&2';
COMMIT;

DELETE	FROM 	&1..LYNX_DTYPE_TEXT_GC
	WHERE	DATA_TYPE = '&2';
COMMIT;

DELETE	FROM	&1..LYNX_CULTURE
	WHERE	DATA_TYPE = '&2';
COMMIT;

DELETE	FROM	&1..LYNX_DATA_TYPES
	WHERE	DATA_TYPE = '&2';
COMMIT;

DELETE	FROM	&1..LYNX_DTYPE_ARC_GC
	WHERE	DATA_TYPE = '&2';
COMMIT;

DELETE	FROM	&1..LYNX_DTYPE_NODE_GC
	WHERE	DATA_TYPE = '&2';
COMMIT;

DELETE	FROM	&1..LYNX_DTYPE_REGION_GC
	WHERE	DATA_TYPE = '&2';
COMMIT;

