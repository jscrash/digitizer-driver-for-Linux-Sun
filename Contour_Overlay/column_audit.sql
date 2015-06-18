/* DEC/CMS REPLACEMENT HISTORY, Element COLUMN_AUDIT.SQL */
/* *1    19-JUN-1989 14:53:15 SYSTEM "" */
/* DEC/CMS REPLACEMENT HISTORY, Element COLUMN_AUDIT.SQL */
DOC
    This procedure constructs a view which allows the viewing of all columns
    in the system by a DBA, whether or not specific privileges have been
    granted to the DBA.  A user will see only their own columns.
    Provided as part of ESI Finder, copyright 1987.  Exploration Systems, Inc
#
DROP VIEW COLUMN_AUDIT;
CREATE VIEW COLUMN_AUDIT
        (CNAME, TNAME, CREATOR, COLNO, COLTYPE,
         WIDTH, SCALE, NULLS, REMARKS, DEFAULTVAL) AS
SELECT  C.COL$NAME, T.TAB$NAME, U.USR$NAME, C.COL$ID, C.COL$DATATYPE,
        C.COL$LENGTH, C.COL$SCALE, C.COL$NULL_TYPE, C.COL$COMMENT, C.COL$DEFAULT
FROM	SYS.COLUMNS C, SYS.TABLES T, SYS.USERAUTH U
WHERE   T.TAB$PID=C.COL$PID AND
	T.TAB$RBA=C.COL$RBA AND
	T.TAB$TBL=C.COL$TBL AND
	U.USR$UID = T.TAB$OWNER AND
	T.TAB$TYPE = 'TABLE' AND
	T.TAB$OWNER NOT IN (0,2) AND
	(T.TAB$OWNER = UID OR EXISTS
	(SELECT USER FROM SYS.USERAUTH WHERE USR$DBA='Y' AND USR$NAME=USER));
GRANT SELECT ON COLUMN_AUDIT TO PUBLIC;
CREATE PUBLIC SYNONYM COLUMN_AUDIT FOR COLUMN_AUDIT;
