$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_DIRECTORY.COM
$! *1    13-NOV-1990 11:23:46 VINCE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_DIRECTORY.COM
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_DIRECTORY.COM
$! *1     6-MAR-1990 08:50:14 GILLESPIE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_DIRECTORY.COM
$ !**********************************************************************
$ !
$ ! FILE:	OR_FORM_DIRECTORY.COM
$ !
$ ! AUTHOR:	W. Jaeck, 25-August-1987
$ !
$ ! DESCRIPTION:
$ !
$ !	OR_FORM_DIRECTORY.COM gives the user a list of form files
$ !	whose names would be valid to specify as parameters to the
$ !	OFORM command.
$ !
$ !**********************************************************************
$ !
$	SAY = "WRITE SYS$OUTPUT"
$	SAY "List of forms:"
$ LOOP:
$	FILE = F$SEARCH("ESI$FORMS:*.FRM")
$	IF FILE .EQS. "" THEN EXIT
$	FILENAME = F$PARSE(FILE,,,"NAME","SYNTAX_ONLY")
$	SAY "  ''FILENAME'"
$	GOTO LOOP
