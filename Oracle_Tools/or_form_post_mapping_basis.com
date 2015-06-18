$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MAPPING_BASIS.COM
$! *1    13-NOV-1990 11:23:47 VINCE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MAPPING_BASIS.COM
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MAPPING_BASIS.COM
$! *3     4-OCT-1990 11:23:41 VINCE "(SPR 1) changed SAY to MSG to avoid conflict with OR_FORM's SAY"
$! *2     4-OCT-1990 11:15:19 VINCE "(SPR 1) added action menu and reworked setting of CMS library"
$! *1     6-MAR-1990 08:50:16 GILLESPIE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element OR_FORM_POST_MAPPING_BASIS.COM
$!************************************************************************
$!
$! FILE = OFORM_POST_MAPPING_BASIS.COM
$!
$!	This command file is run after the oform command file utility
$!	is used to activate the mapping_basis form.
$!
$!	This utility will checkout, create new versions, and checkin
$!	the include files "ESI_MD_DBDEFS.H" and "ESI_MD_BASIS.AH".
$!	The checkout/checkin (CMS) facility is used in order to trigger
$!	recompilation of source code via the ESI$MAKE procedure.
$!	The SPR number 5043 is reserved for this process.
$!
$!	**** NOTE ****
$!	When moved to a customer development site this procedure may
$!	need to be modified depending upon there recompilation and
$!	relinking procedures.
$!
$!************************************************************************
$!
$!
$   SET NOON
$   ! note: can't use SAY because it conflicts with our caller OR_FORM
$   MSG :== "WRITE SYS$OUTPUT"
$   CMS_LIBRARY = F$TRNLNM("CMS$LIB","LNM$PROCESS_TABLE")
$!
$!				Check if need to do anything
$ ASK_OPT:
$   MSG ""
$   MSG "Actions :"
$   MSG "	1.  Generate the updated include files."
$   MSG "	2.  Checkout current include files and Generate updated set."
$   MSG "	3.  Checkout, Generate and Checkin."
$   MSG "	4.  Exit; take no acton.
$   MSG ""
$   INQUIRE ACTION "Enter Action Number : (1,2,3,4) ? [1]"
$   IF (ACTION .EQS. "") THEN ACTION = 1
$   MSG ""
$!
$   IF (ACTION .LT. 1) .OR. (ACTION .GT. 4)
$   THEN
$	MSG "Invalid action; Please choose from the list."
$	GOTO ASK_OPT
$   ENDIF
$!
$   IF (ACTION .EQ. 1) THEN GOTO GENERATE
$   IF (ACTION .EQ. 4) THEN GOTO DONE
$!
$! Check that CMS$LIB is pointing at the include library
$!
$   MSG "-- Checking setting of CMS library."
$!
$   DEFINE/USER SYS$ERROR   NL:
$   DEFINE/USER SYS$OUTPUT  NL:
$   CMS SHOW ELEMENT/BRIEF/OUTPUT=NL: ESI_MD_BASIS.AH
$   IF ($SEVERITY .NE. 1)
$   THEN
$	IF (F$GETSYI("NODENAME") .EQS. "ERNIE")
$	    THEN DEFINE/PROCESS/NOLOG CMS$LIB DKA400:[CMS.INC]
$	    ELSE DEFINE/PROCESS/NOLOG CMS$LIB SRCDISK:[CMSINCLIB]
$	ENDIF
$!
$!      check again.
$	DEFINE/USER SYS$ERROR   NL:
$	DEFINE/USER SYS$OUTPUT  NL:
$	CMS SHOW ELEMENT/BRIEF/OUTPUT=NL: ESI_MD_BASIS.AH
$	IF ($SEVERITY .NE. 1)
$	THEN
$	    MSG ""
$	    MSG "!!!  Can't find the CMS library for include files."
$	    MSG "!!!  aborting."
$	    MSG ""
$	    GOTO FINISHUP
$	ENDIF
$   ENDIF
$!
$!
$   MSG "-- Checking out Include files ESI_MD_BASIS.AH and ESI_MD_DBDEFS.H"
$!
$   CHECKOUT ESI_MD_DBDEFS.H
$   CHECKOUT ESI_MD_BASIS.AH
$   DELETE/NOLOG ESI_MD_DBDEFS.H;*
$   DELETE/NOLOG ESI_MD_BASIS.AH;*
$!
$ GENERATE:
$   MSG "-- Creating updated versions of include files."
$!
$   LIBRARIAN_ACCOUNT = F$TRNLNM("ESI$LIBRARIAN_ACCOUNT")
$   BASIS = "$ESI$MAINT:MP_CREATE_MAP_BASIS"
$   ASSIGN/USER SYS$COMMAND: SYS$INPUT:
$   BASIS 'LIBRARIAN_ACCOUNT'
$   IF (ACTION .LT. 3) THEN GOTO FINISHUP
$!
$   MSG "-- Checking include files back in."
$!
$   CHECKIN ESI_MD_DBDEFS.H "MAPPING USER INTERFACE MODIFIED" SPR=5043
$   CHECKIN ESI_MD_BASIS.AH "MAPPING USER INTERFACE MODIFIED" SPR=5043
$!
$!
$ FINISHUP:
$! reset CMS$LIB to the previous value
$   DEASSIGN/PROCESS CMS$LIB
$   IF (F$LENGTH("''CMS_LIBRARY'") .GT. 0) THEN -
	    DEFINE/PROCESS/NOLOG CMS$LIB 'CMS_LIBRARY'
$!
$!
$ DONE:
$   MSG "-- Done."
$   EXIT
