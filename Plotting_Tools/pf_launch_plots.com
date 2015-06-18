$! DEC/CMS REPLACEMENT HISTORY, Element PF_LAUNCH_PLOTS.COM
$! *2    20-JUN-1991 10:35:22 MING "(SPR 0) new checkin"
$! *1    13-JUN-1991 15:13:13 MING "launch plot file to plotter"
$! DEC/CMS REPLACEMENT HISTORY, Element PF_LAUNCH_PLOTS.COM
$!* ------------------------------------------------------------
$!*
$!* SEND PLOT FILE TO PLOTTER
$!*
$!* ------------------------------------------------------------
$ SET VERIFY
$       ON WARNING THEN CONTINUE
$!       ON SEVERE_ERROR THEN GOTO ANY_ERROR
$!
$!* Get the command line arguments
$!
$       COPIES   = 'P1'
$       PLOTFILE = "''P2'"
$       DRIVER   = "''P3'"
$       QUEUE    = "''P4'"
$
$!*
$!* Get rid of the version number
$!*
$       PLOTFILE = F$PARSE("''PLOTFILE'",,,"DEVICE") -
		+ F$PARSE("''PLOTFILE'",,,"DIRECTORY") -
                + F$PARSE("''PLOTFILE'",,,"NAME") + "."
$!* Get the parameter path
$       PARAMFILE = "''PLOTFILE'" + "PAR"
$!*
$!* Loop for sending the plot file
$!*
$ LOOP:
$       @PLOT_BIN_DIR:'DRIVER' 'PARAMFILE' 'PLOTFILE' 'QUEUE'
$       COPIES = COPIES - 1
$       IF COPIES .GT. 0 THEN GOTO LOOP
$
$       EXIT
$ 
$ ANY_ERROR:
$       WRITE SYS$OUTPUT "UNRESOLVABLE ERROR, MUST EXIT"
$       EXIT
$!
