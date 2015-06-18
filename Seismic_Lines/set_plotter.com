$! DEC/CMS REPLACEMENT HISTORY, Element SET_PLOTTER.COM
$! *1    19-JUN-1989 14:58:00 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element SET_PLOTTER.COM
$ !************************************************************
$ !** 
$ !** Set the logical symbols ESI$PLOTTER_DEVICE
$ !** and ESI$PLOTTER_QUEUE for Finder plotter output.
$ !** 
$ !************************************************************
$ !** 
$     SET NOON
$     SAY = "WRITE SYS$OUTPUT "
$     GOSUB SHOW_PLOTTER
$     GOSUB SHOW_QUEUE
$ !** 
$ !** 
$ SELECT:
$     SAY " " 
$     SAY " (1)  Nicolet ZETA Plotter"
$     SAY " (2)  Calcomp Type Plotter"
$     SAY " " 
$     INQUIRE/NOPUNC TYPE "Select a plotter: "
$     IF F$TYPE(TYPE) .NES. "INTEGER" THEN GOTO SELECT
$     IF TYPE .LT. 1 .OR. TYPE .GE. 3 THEN GOTO SELECT
$     IF TYPE .EQ. 1 THEN PLOTTER = "ZETA"
$     IF TYPE .EQ. 2 THEN PLOTTER = "CALCOMP"
$ !** 
$ SELECT2:
$     SAY " " 
$     INQUIRE/NOPUNC QUEUE "Enter the queue name for device ''PLOTTER': "
$     IF QUEUE .EQS. "" THEN GOTO NONE_SELECTED
$     ASS/NOLOG/PROC "''PLOTTER'" ESI$PLOTTER_DEVICE
$     ASS/NOLOG/PROC "''QUEUE'" ESI$PLOTTER_QUEUE
$ !** 
$     SAY " " 
$     GOSUB SHOW_PLOTTER
$     GOSUB SHOW_QUEUE
$ !** 
$     EXIT
$ !** 
$ !** 
$ !** 
$ SHOW_PLOTTER:
$     PLOTTER = "''F$TRN("ESI$PLOTTER_DEVICE")'"
$     IF PLOTTER .EQS. "" THEN GOTO NOPL
$     SAY "Current plotter selected is ''PLOTTER'"
$     RETURN
$ !** 
$ NOPL:
$     SAY "There is no plotter selected. "
$     RETURN
$ !** 
$ !** 
$ !** 
$ SHOW_QUEUE:
$     QUEUE = "''F$TRN("ESI$PLOTTER_QUEUE")'"
$     IF QUEUE .EQS. "" THEN GOTO NOQUE
$     SAY "The current plotting queue is ''QUEUE'"
$     RETURN
$ !** 
$ NOQUE:
$     SAY "There is no current plotting queue selected. "
$     RETURN
$     
$     

