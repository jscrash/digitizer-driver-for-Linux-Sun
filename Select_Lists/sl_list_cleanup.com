$! DEC/CMS REPLACEMENT HISTORY, Element SL_LIST_CLEANUP.COM
$! *2    26-NOV-1990 14:00:21 MING "(SPR 6193) sub job at the very end"
$! *1    19-JUN-1989 14:58:02 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element SL_LIST_CLEANUP.COM
$ !**********************************************************************
$ !
$ ! FILE:	SL_LIST_CLEANUP.COM
$ !
$ ! AUTHOR:	J. Chang, 21-August-1987
$ !             Extensively modified J Gillespie 6-Jan-88
$ !
$ ! DESCRIPTION:
$ !	   The purpose of this file is to clean up those temperary Selects lists
$ !	existing in all projects every night.  There is an potential security
$ !	problem, e.g. ACCOUNT.  In order to delete those Select Lists from
$ !	database, the account need have DBA privilege.  For the time being,
$ !	we hardcode it in this file.
$ ! 	   The password of account need to be consistent with the ORACLE 
$ ! 	account.
$ !	
$ ! ADDED 6-Jan-88 JGG
$ ! The following must be done BEFORE this procedure will run:
$ ! THE SITE VMS SYSTEM MANAGER MUST SET UP BATCH QUEUES
$ ! THE SITE VMS SYSTEM MANAGER MUST SET UP A CAPTIVE VMS ACCOUNT CALLED ESI
$ ! WHICH HAS /NOLOGIN SET IN AUTHORIZE
$ ! THE ORACLE DBA MUST SET UP AN ACCOUNT FOR THE USER OPS$ESI
$ ! THE FOLLOWING LINE SHOULD BE EXECUTED DURING SYSTEM REBOOT OR BY AN ACCOUNT
$ ! WITH CMKRNL PRIVILEGE:
$ ! SUBMIT/USER=ESI ESI$BIN:SL_LIST_CLEANUP
$ ! BE CAREFUL ABOUT GETTING MORE THAN ONE INSTANCE OF THE JOB IN THE QUEUES
$ !**********************************************************************
$ !
$ FULL_FILE_SPEC = F$ENVIRONMENT("PROCEDURE")
$ ! GET FILE SPEC SANS VERSION
$ THIS_DISK = F$PARSE(FULL_FILE_SPEC,,,"DEVICE")
$ THIS_DIRECTORY = F$PARSE(FULL_FILE_SPEC,,,"DIRECTORY")
$ THIS_NAME = F$PARSE(FULL_FILE_SPEC,,,"NAME")
$ THIS_FILE = THIS_DISK + THIS_DIRECTORY + THIS_NAME
$ !
$ !				SLASH INDICATES USE AUTOLOGIN FEATURE OF ORACLE
$	
$   ACCOUNT = "/"
$   DATABASE_NODE = F$TRNLNM("ESI$DATABASE_NODE")
$   IF DATABASE_NODE .NES. "" THEN ACCOUNT = ACCOUNT + DATABASE_NODE
$   SLC = "$ESI$BIN:SL_LIST_CLEANUP"
$   SLC 'ACCOUNT'
$ !				RESUBMIT THYSELF
$ SUBMIT 'THIS_FILE'/AFTER=TOMORROW
