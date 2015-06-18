$! DEC/CMS REPLACEMENT HISTORY, Element LD_SGC_BATCH.COM
$! *1    19-JUN-1989 14:57:22 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element LD_SGC_BATCH.COM
$!* ------------------------------------------------------------     
$!* LOAD SCATTER/GRID/CONTOUR/PERSPECTIVE FILES
$!* 
$!* John Conroy       Dec. 20, 1988
$!*      
$!* ------------------------------------------------------------     
$        ON WARNING THEN CONTINUE
$        ON SEVERE_ERROR THEN GOTO ANY_ERROR
$!* 
$!*      
$!*      
$        ORASTRING	= "''P1'"		  ! ORACLE LOGIN STRING
$        PROJECT	= "''P2'"		  ! FINDER PROJECT NAME
$        COMMFILE	= "''P3'"		  ! MAP DEF NAME 
$!*      
$!*      
$!*      
$ CHKPARM:
$        IF (ORASTRING .EQS. "" ) THEN GOTO PARAM_ERROR
$        IF (PROJECT   .EQS. "" ) THEN GOTO PARAM_ERROR
$        IF (COMMFILE  .EQS. "" ) THEN GOTO PARAM_ERROR
$!*      
$!* 
$!* 
$!* 
$!* 
$        USER := 'F$EDIT(F$GETJPI("","USERNAME"),"TRIM,COMPRESS")'
$        TOD = "''F$TIME()'"
$        TMP_FILE = "''USER'S_MAPTMP_''F$EXT(F$LEN(TOD)-2,999,TOD)'.COM"
$!
$!	Open and Write the Command file
$!
$        OPEN/WRITE/ERROR=OPEN_ERROR TMP 'TMP_FILE'
$        WRITE TMP "$LOAD_FILES := $ESI$BIN:LD_BATCH_SGC_LOAD"
$        WRITE TMP "$LOAD_FILES ''ORASTRING' ''PROJECT' ''COMMFILE'"
$        CLOSE TMP
$        @'TMP_FILE'
$!
$        PURGE/NOLOG 'TMP_FILE'
$!
$!
$       EXIT
$!
$!
$! ERROR CONDITION HANDLERS
$!
$ OPEN_ERROR:
$        WRITE SYS$OUTPUT "CANNOT OPEN INPUT COMMAND FILE"
$        EXIT
$!
$ CLOSE_ERROR:
$        WRITE SYS$OUTPUT "CANNOT CLOSE INPUT COMMAND FILE"
$        EXIT
$!
$ PARAM_ERROR:
$        WRITE SYS$OUTPUT "INSUFFICIANT PARAMETER LIST"
$        EXIT
$!
$ ANY_ERROR:
$        WRITE SYS$OUTPUT "UNRESOLVABLE ERROR, MUST EXIT"
$        EXIT
$!
$!
