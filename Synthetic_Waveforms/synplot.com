$! DEC/CMS REPLACEMENT HISTORY, Element SYNPLOT.COM
$! *1    19-JUN-1989 14:58:06 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element SYNPLOT.COM
$!* ------------------------------------------------------------     
$!*			 PLOT SYNTHETICS 
$!* Command procedure to call the plotting routine in Synthetics
$!* 
$!* Julian Carlisle     May 21, 1988     
$!*   01/05/89 GS - put plot file in ESI$PLOTS & rename, pass PARAMFILE as arg
$!*      
$!* ------------------------------------------------------------     
$!*      
$        ON WARNING THEN CONTINUE
$        ON SEVERE_ERROR THEN GOTO ANY_ERROR
$!* 
$!* To spot parm problems in the error log printout...     
$        DBG = "''P1',''P2',''P3',''P4',''P5',''P6',''P7',''P8'"
$        WRITE SYS$OUTPUT DBG
$!*           
$        ORASTRING	= "''P1'"		  ! ORACLE LOGIN STRING
$        NFILE		= "''P2'"		  ! NAME FOR NEUT FILE OUTPUT
$        PROJECT	= "''P3'"		  ! FINDER PROJECT NAME
$        PARAMFILE	= "''P4'"		  ! SYNTHETIC'S DATA FILE
$        PDRIVER	= "''P5'"		  ! NAME OF DRIVER TO USE
$        PQUEUE	= "''P6'"		  ! NAME OF PLOTTER QUEUE
$        PXSIZE	= "''P7'"		  ! MAX X SIZE OF DEVICE
$        PYSIZE	= "''P8'"		  ! MAX Y SIZE
$!*      
$        IF F$PARS(NFILE,,,"TYPE","SYNTAX_ONLY") .NES. "." THEN GOTO CHKPARM
$        NFILE = "''NFILE'.NPF"
$!*      
$ CHKPARM:
$        IF (PXSIZE    .EQS. "" ) THEN PXSIZE = "35.0"
$        IF (PYSIZE    .EQS. "" ) THEN PYSIZE = "''PXSIZE'"
$!*      
$        IF (ORASTRING .EQS. "" ) THEN GOTO PARAM_ERROR
$        IF (PROJECT   .EQS. "" ) THEN GOTO PARAM_ERROR
$        IF (NFILE     .EQS. "" ) THEN GOTO PARAM_ERROR
$!*      
$!*  Name of the plotter specific output file.     
$        DFILE = "ESI$PLOTS:''F$PARS(NFILE,,,"NAME","SYNTAX_ONLY")'.''PDRIVER'"
$!* 
$!* Create an command input file and execute it.
$!* 
$!* We only do this in Development for ease of maintanance.    
$        ASSIGN/NOLOG ESI$BIN: PLOT_BIN_DIR:
$!*      
$!* 
$        TOD = "''F$TIME()'"
$        USER = "''F$EDIT(F$GETJPI("","USERNAME"),"TRIM,COMPRESS")'"
$        TMP_FILE = "SYNPLOT_''F$EXT(F$LEN(TOD)-2,999,TOD)'.COM"
$        OPEN/WRITE/ERROR=OPEN_ERROR TMP 'TMP_FILE'
$!*      
$        WRITE TMP "$SYN_PLOT := $PLOT_BIN_DIR:SYN_PLOTTER.EXE"
$        WRITE TMP "$SYN_PLOT ''ORASTRING' ''PARAMFILE' ''PROJECT'"
$        WRITE TMP "''NFILE'"
$        WRITE TMP "''PXSIZE' ''PYSIZE'"
$        CLOSE  TMP
$        @'TMP_FILE'
$!* 
$        IF F$SEARCH(TMP_FILE) .NES. "" THEN DELETE/NOLOG 'TMP_FILE';*
$!*                     
$! Call neutral interpreter to create plot file from neutral file.
$!
$        IF F$TRNLNM("FOR009") .NES. "" THEN DEASS FOR009
$        IF DFILE .NES. "" THEN ASSIGN/NOLOG "''DFILE'" FOR009
$        IF DFILE .EQS. "" THEN DFILE = "FOR009.DAT"
$!*      
$        ASSIGN/NOLOG "''NFILE'" FOR010
$        R/NODEBUG ESI$BIN:'PDRIVER'
$        PRINT/QUE='PQUEUE' 'DFILE'
$        EXIT
$!* 
$!* -------------------------------------------------------     
$!* 
$!* ERROR CONDITION HANDLERS
$!* 
$ OPEN_ERROR:
$        WRITE SYS$OUTPUT "CANNOT OPEN INPUT COMMAND FILE"
$        EXIT
$!* 
$ CLOSE_ERROR:
$        WRITE SYS$OUTPUT "CANNOT CLOSE INPUT COMMAND FILE"
$        EXIT
$!* 
$ PARAM_ERROR:
$        WRITE SYS$OUTPUT "INSUFFICIANT PARAMETER LIST"
$        EXIT
$!* 
$ ANY_ERROR:
$        WRITE SYS$OUTPUT "UNRESOLVABLE ERROR, MUST EXIT"
$        EXIT
