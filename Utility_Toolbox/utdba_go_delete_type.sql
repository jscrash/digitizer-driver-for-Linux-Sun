DOC
This script takes one parameter, namely the graphic object type name to be
deleted. It purposely does not delete any nodes, as they might be shared
by other database objects.

Usage: Within SQLPLUS type:

START GO_DELETE_TYPE "OBJECT NAME"

where OBJECT NAME is the name of an object that is present in the database.

And weeeere off!...

...Deleting from the TEXT table.;
#
DELETE FROM TEXT WHERE TEXT_ID IN
   (SELECT GOC.ITEM_ID
      FROM
	 GRAPHIC_OBJECT_CORRELATIONS GOC,
	 GRAPHIC_OBJECTS GO
      WHERE
	 GO.OBJECT_ID = GOC.OBJECT_ID  AND
	 GOC.ITEM_CLASS = 'TEXT'       AND
	 GO.OBJECT_NAME LIKE '&1');

DOC
...Deleting from the LINES table.;
#
DELETE FROM LINES WHERE LINE_ID IN
   (SELECT GOC.ITEM_ID
      FROM
	 GRAPHIC_OBJECT_CORRELATIONS GOC,
	 GRAPHIC_OBJECTS GO
      WHERE
	 GO.OBJECT_ID = GOC.OBJECT_ID  AND
	 GOC.ITEM_CLASS = 'LINE'       AND
	 GO.OBJECT_NAME LIKE '&1');

DOC
...Deleting from the GRAPHIC_OBJECT_CORRELATIONS table.;
#
DELETE FROM GRAPHIC_OBJECT_CORRELATIONS WHERE OBJECT_ID IN
   (SELECT OBJECT_ID
      FROM  GRAPHIC_OBJECTS
      WHERE OBJECT_NAME LIKE '&1');

DOC
...Deleting from the GRAPHIC_OBJECTS table.;
#
DELETE FROM GRAPHIC_OBJECTS WHERE OBJECT_NAME LIKE '&1';

DOC
...Deleting from the GRAPHIC_OBJECT_CODES table.;
#
DELETE FROM GRAPHIC_OBJECT_CODES WHERE NAME LIKE '&1';
DOC
Note that this script also does not delete the nlist file, which will have
a filename the same as the object name and can be found in the 
ESI$PROJECT:[CULTURE] directory
#
EXIT

/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_GO_DELETE_TYPE.SQL*/
/* *2    16-AUG-1991 14:30:16 ALLAWAY "(SPR 8648) initial checkin"*/
/* *1    16-AUG-1991 14:27:30 ALLAWAY "Delete a graphic object type from the graphic object database"*/
/* DEC/CMS REPLACEMENT HISTORY, Element UTDBA_GO_DELETE_TYPE.SQL*/
