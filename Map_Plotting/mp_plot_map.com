$! DEC/CMS REPLACEMENT HISTORY, Element MP_PLOT_MAP.COM
$! *13   20-JUN-1991 10:33:15 MING "(SPR 0) new plot file managment"
$! *12   12-OCT-1990 17:22:24 GILLESPIE "(SPR 100) Merge Ernie Deltas"
$! *11   24-MAY-1990 20:59:30 MING "(SPR 0) ADD GKS LOGICAL SYMBOLS"
$! *10   24-MAY-1990 18:56:38 MING "(SPR 0) ADD PARAMETER FILE AS THE SECOND ARGUMENT OF THE DRIVER"
$! *9    22-MAY-1990 18:03:14 VINCE "(SPR 1) cleaned up parameter display"
$! *8     3-MAY-1990 08:52:30 MING "(SPR 0) fix bug "
$! *7    30-APR-1990 09:00:58 MING "(SPR 0) produce metafile"
$! *6    24-APR-1990 10:24:35 WALTERS "(SPR 0) ADD SET VERIFY"
$! *5    19-APR-1990 13:39:26 VINCE "(SPR 1) new from Enron"
$! *4    18-APR-1990 12:18:34 MING "(SPR 0) test metafile"
$! *3    18-APR-1990 10:49:25 MING "(SPR 1) test for metafile"
$! *2     6-OCT-1989 10:02:25 GORDON "(SPR 4999) Only produce neutral plot file, don't send to driver for now"
$! *1    19-JUN-1989 14:57:30 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element MP_PLOT_MAP.COM
!  DEC/CMS REPLACEMENT HISTORY, Element MP_PLOT_MAP.COM
!  *2    12-OCT-1990 00:31:14 SCD "(SPR 6075) plot composer checkin"
!  *1    13-AUG-1990 17:44:55 SYSTEM "Finder 6.1"
!  DEC/CMS REPLACEMENT HISTORY, Element MP_PLOT_MAP.COM
$!* ------------------------------------------------------------     
$!* PLOT MAPS....     
$!* 
$!* Julian Carlisle     May 21, 1988     
$!*
$!* 10/02/89 G Shannon - Modified to produce neutral file only (for BHP)      
$!* ------------------------------------------------------------     
$! SET VERIFY
$	ON WARNING THEN CONTINUE
$	ON SEVERE_ERROR THEN GOTO ANY_ERROR
$!* Get the command line arguments
$	ORASTRING    = "''P1'"	! ORACLE LOGIN STRING
$	PROJECT	     = "''P2'"	! FINDER PROJECT NAME
$	NAME         = "''P3'"	! INFO ABOUT MPDEFNAME, PLOT FILE FORMAT,DATA FILE
$	MAX_SIZE     = "''P4'"	! SIZE OF THE PLOT FILE FOR NEUTRAL PLOT FILE
$	LAYOUT       = "''P5'"	! PLOT LAYOUT
$	DRIVER_QUEUE = "''P6'"	! INFO ABOUT PLOTTER .COM SCRIPT & PLOT QUEUE
$	MISC         = "''P7'"	! INFO ABOUT # OF COPIES & SAVING FLAG
$	FM_INFO      = "''P8'"	! FILE MANAGMENT INFOMATION
$
$! SET LOGICAL FOR GKS 
$
$	ASSIGN ESI$ROOT:[GKS.INCLUDE]   GKSLIB
$	ASSIGN SYS$SCRATCH:GKWISS.DAT   GKWISS
$	ASSIGN ESI$ROOT:[BIN]GKERR.TXT  GKERR
$	ASSIGN ESI$ROOT:[BIN]GKNAME.TXT GKNAME
$	ASSIGN ESI$ROOT:[BIN]GKFONT.DAT GKFONT
$
$ CHKPARM:
$       MAPDEF_NAME = F$ELEMENT (0,"@",NAME)
$       PLOT_FILE_FORMAT = F$ELEMENT (1,"@",NAME)
$       DATA_FILE = F$ELEMENT (2,"@",NAME)
$       DRIVER = F$ELEMENT (0,"@",DRIVER_QUEUE)
$       QUEUE  = F$ELEMENT (1,"@",DRIVER_QUEUE)
$       COPIES = F$ELEMENT (0,"@",MISC)
$       SAVED  = F$ELEMENT (1,"@",MISC)
$
$       DUMMY_FILE = F$PARSE("''DATA_FILE'",,,"DEVICE") - 
                     + F$PARSE("''DATA_FILE'",,,"DIRECTORY") -
                     + F$PARSE("''DATA_FILE'",,,"NAME") + ".DUMMY"
$	IF (ORASTRING .EQS. "" ) THEN GOTO PARAM_ERROR
$	IF (PROJECT   .EQS. "" ) THEN GOTO PARAM_ERROR
$
$!* To spot parm problems in the error log printout...
$	WRITE SYS$OUTPUT "*************  Parameter Display  *************"
$	WRITE SYS$OUTPUT "ORASTRING        = ''ORASTRING'"
$	WRITE SYS$OUTPUT "PROJECT          = ''PROJECT'"
$	WRITE SYS$OUTPUT "MAP NAME         = ''MAPDEF_NAME'"
$	WRITE SYS$OUTPUT "PLOT FILE FORMAT = ''PLOT_FILE_FORMAT'"
$	WRITE SYS$OUTPUT "DATA FILE        = ''DATA_FILE'"
$	WRITE SYS$OUTPUT "PLOT FILE SIZE   = ''MAX_SIZE'"
$	WRITE SYS$OUTPUT "DRIVER           = ''DRIVER'"
$	WRITE SYS$OUTPUT "PLOT QUEUE       = ''QUEUE'"
$	WRITE SYS$OUTPUT "FM_INFO          = ''FM_INFO'"
$	WRITE SYS$OUTPUT "*************  ********* *******  *************"
$	WRITE SYS$OUTPUT " "
$!
$!
$	ASSIGN/NOLOG ESI$BIN: PLOT_BIN_DIR:
$!
$! check if netural file used
$   IF "''PLOT_FILE_FORMAT'" .EQS. "NPF"
$   THEN
$	IS_NEUTRAL = "1"    
$       IS_XSECT   = "0"
$	USER := 'F$EDIT(F$GETJPI("","USERNAME"),"TRIM,COMPRESS")'
$	TOD = "''F$TIME()'"
$	TMP_FILE = "''USER'S_MAPTMP_''F$EXT(F$LEN(TOD)-2,999,TOD)'.COM"
$   TMP_NPF_FILE = F$TRNLNM ("SYS$LOGIN")  -
                   + F$PARSE("''DATA_FILE'",,,"NAME") + ".NPF"
$!
$!	Open and Write the Command file
$!
$	OPEN/WRITE/ERROR=OPEN_ERROR TMP 'TMP_FILE'
$	WRITE TMP "$PLOT_MAP := $PLOT_BIN_DIR:PL_BATCH_PLOT.EXE"
$	WRITE TMP "$PLOT_MAP ''ORASTRING' ''PROJECT' ""''LAYOUT'"" ''IS_XSECT' ""''MAPDEF_NAME'"" ''PLOT_FILE_FORMAT' ''DATA_FILE' ""''SAVED@ ''FM_INFO' """
$       
$
$	WRITE TMP "''TMP_NPF_FILE'"
$	WRITE TMP "''MAX_SIZE'  ''MAX_SIZE'"
$	CLOSE TMP
$	@'TMP_FILE'
$
$!
$!      GET PLOT FILE NAME FROM DUMMY FILE CREATED BY IN THE PL_BATCH_PLOT.EXE
$!
$   OPEN/READ/ERROR=OPEN_ERROR DUMMY 'DUMMY_FILE'
$   READ DUMMY PLOTFILE
$   CLOSE DUMMY
$!
$!   COPY "''TMP_NPF_FILE'" "''PLOTFILE'"
$!   DELETE "''TMP_NPF_FILE"
$!
$	PURGE/NOLOG 'TMP_FILE'
$!
$
$   ELSE    ! METAFILE
$   IS_NEUTRAL = "0"
$   IS_XSECT   = "0"
$	PLOT_MAP := $PLOT_BIN_DIR:PL_PLOT_META.EXE
$	PLOT_MAP 'ORASTRING' 'PROJECT' "''LAYOUT'" 'IS_XSECT' "''MAPDEF_NAME'" -
		 'PLOT_FILE_FORMAT' 'DATA_FILE' "''SAVED@ ''FM_INFO' "
$
$!
$!      GET PLOT FILE NAME FROM DUMMY FILE CREATED BY IN THE PL_BATCH_PLOT.EXE
$!
$   OPEN/READ/ERROR=OPEN_ERROR DUMMY 'DUMMY_FILE'
$   READ DUMMY PLOTFILE
$   CLOSE DUMMY
$   ENDIF
$
$   IF COPIES .EQ. 0 THEN EXIT
$
$   @PLOT_BIN_DIR:PF_LAUNCH_PLOTS 'COPIES' "''PLOTFILE'" "''DRIVER'" "''QUEUE'"
$
$   EXIT
$!
$!
$! ERROR CONDITION HANDLERS
$!
$ OPEN_ERROR:
$	WRITE SYS$OUTPUT "CANNOT OPEN INPUT COMMAND FILE"
$	EXIT
$!
$ CLOSE_ERROR:
$	WRITE SYS$OUTPUT "CANNOT CLOSE INPUT COMMAND FILE"
$	EXIT
$!
$ PARAM_ERROR:
$	WRITE SYS$OUTPUT "INSUFFICIANT PARAMETER LIST"
$	EXIT
$!
$ ANY_ERROR:
$	WRITE SYS$OUTPUT "UNRESOLVABLE ERROR, MUST EXIT"
$	EXIT
$!
