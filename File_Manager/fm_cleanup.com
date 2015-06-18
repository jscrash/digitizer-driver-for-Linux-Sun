$! DEC/CMS REPLACEMENT HISTORY, Element FM_CLEANUP.COM
$! *2    17-DEC-1990 17:46:16 MING "(SPR 6244) fix syntax error on if statment"
$! *1    19-JUN-1989 14:57:12 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element FM_CLEANUP.COM
$ !**********************************************************************
$ !
$ ! FILE:	FM_CLEANUP.COM
$ !
$ ! AUTHOR:	John Conroy 29-Apr-889
$ !
$ ! DESCRIPTION:
$ !	    Command Procecure to execute the stand alone program "FM_CLEANUP"
$ !	    which deletes the files used by FINDER that are under the 
$ !	    control of the FINDER FILE MANAGEMENT SYSTEM.
$ !
$ !	    This procedure may be started either interactively or in batch.
$ !	    The input parameters for this procedure are:
$ !		    P1 - A flag to specify whether or not to delete the
$ !			 File Management data records for the file as
$ !			 well as the disk file itself.
$ !		    P2 - The FINDER account/password[@database] string
$ !			 to use when logging into FINDER.  This must be
$ !			 the FINDER system account.
$ !	    These parameters may be supplied to the procedure or omitted.
$ !
$ !	    In INTERACTIVE mode the user will be prompted for them, if they
$ !	    are omitted.
$ !
$ !	    If they are empty when the procedure gets ready to run the 
$ !	    program then defaults will be supplied. P1 will be set to "n"
$ !	    and P2 will be set to "/" (to use the ORACLE autologin feature).
$ !
$ !	    In order for the autologin feature to work, the user implementing
$ !	    the procedure must have the FINDER system account set up by
$ !	    the ORACLE DBA to be its autologin account.
$ !
$ !	    One way to make sure the cleanup gets implemented is to:
$ !		1)  Have the site VMS SYSTEM MANAGER set up a captive
$ !		    VMS ACCOUNT called ESI which has /NOLOGIN set in
$ !		    AUTHORIZE.
$ !		2)  Have the ORACLE DBA set up an account for the user OPS$ESI.
$ !		3)  Have either of the following executed during system reboot
$ !		    (or by an account with CMKRNL privilege):
$ !			SUBMIT/USER=ESI ESI$BIN:FM_CLEANUP
$ !		    or
$ !			SUBMIT/USER=ESI/PARAMETERS=(y) ESI$BIN:FM_CLEANUP
$ !
$ !	    NOTES:
$ !		1)  Due to the order of the parameters it is not possible
$ !		    to send in the FINDER account/password without sending
$ !		    the delete flag parameter.
$ !		2)  Be careful about getting more than one instance of the
$ !		    job in the queues.
$ !		3)  The following must be done BEFORE this procedure will run:
$ !		    THE SITE VMS SYSTEM MANAGER MUST SET UP BATCH QUEUES
$ !
$ !**********************************************************************
$
$
$ IF f$mode() .EQS. "BATCH"  THEN GOTO DOIT
$
$ IF P1 .EQS. "" THEN INQUIRE P1 "Do you wish to delete Data Base records ?"
$ IF P2 .EQS. "" THEN INQUIRE P2 "Enter FINDER Account/Password"
$
$ DOIT:
$
$ !	    Set delete flag parameter to upper Y or N
$ IF P1 .NES. "y"  THEN P1 = "n"
$
$ !	    Set Finder Login parameter (Either User Entered or AUTOLOGIN)
$ IF P2 .EQS. ""  THEN P2 = "/"
$ IF f$locate("@",P2) .EQ. f$length(P2)  THEN P2 = P2 + F$TRNLNM("ESI$DATABASE_NODE")
$
$ !	    Run Cleanup Program
$ RUN_PROG = "$ESI$BIN:FM_CLEANUP"
$ RUN_PROG 'P2' 'P1'
$
$ !	    Re-submit Procedure
$
$ !		First Get File Spec
$   FULL_FILE_SPEC = F$ENVIRONMENT("PROCEDURE")
$   THIS_DISK = F$PARSE(FULL_FILE_SPEC,,,"DEVICE")
$   THIS_DIRECTORY = F$PARSE(FULL_FILE_SPEC,,,"DIRECTORY")
$   THIS_NAME = F$PARSE(FULL_FILE_SPEC,,,"NAME")
$   THIS_FILE = THIS_DISK + THIS_DIRECTORY + THIS_NAME
$
$ !		Now re-submit
$ SUBMIT 'THIS_FILE'/AFTER=TOMORROW/PARAMETERS=('P1',"''P2'")
