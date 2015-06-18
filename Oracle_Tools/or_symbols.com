$! DEC/CMS REPLACEMENT HISTORY, Element OR_SYMBOLS.COM
$! *1    13-NOV-1990 11:23:50 VINCE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_SYMBOLS.COM
$! DEC/CMS REPLACEMENT HISTORY, Element OR_SYMBOLS.COM
$! *1     6-MAR-1990 08:50:24 GILLESPIE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_SYMBOLS.COM
$ !**********************************************************************
$ !
$ ! NAME = OR_SYMBOLS.COM
$ !
$ ! AUTHOR:	W. Jaeck, 12-July-1987
$ !
$ ! DESCRIPTION:
$ !
$ !	Define DCL symbols for OR unit utilities.
$ !
$ !**********************************************************************
$ !
$	OFORM :== @ESI$BIN:OR_FORM
$ !
$	EXIT
