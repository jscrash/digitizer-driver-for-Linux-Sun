$! DEC/CMS REPLACEMENT HISTORY, Element DROPDB.COM
$! *1    13-NOV-1990 11:23:30 VINCE "initial release"
$! DEC/CMS REPLACEMENT HISTORY, Element DROPDB.COM
$    set noon
$!*************************************************************************
$!  Copyright 1990, Finder Graphics Systems Inc.   Corta Madera CA. 94925
$!
$! PROGRAM:
$!
$! DESCRIPTION:
$!
$! ARGUMENTS:
$!
$! RETURNS:
$!
$!   Julian Carlisle
$!*************************************************************************
$! don't let user symbols get in the way of progress...
$    set          = "set"
$    say          = "write sys$output "
$    sayit        = "call sayit "
$    mkdir        = "call makedir"
$    md           = "call makedir"
$    getdir       = "call get_dirname "
$    getfile      = "call get_filename "
$    ask          = "inquire/nopunct"
$    library      = "library/extract=*/output="
$    copy         = "copy/nolog"
$    define       = "define/nolog"
$    purge        = "purge/nolog"
$    backup       = "backup/noassist"
$    type         = "type"
$! 
$    bell[0,7]    == 7
$    esc[0,7]     == 27
$    cls          == esc+"[H"+esc+"[J"
$    off          == esc+"[0m"+esc+"(B"
$    bold         == esc+"[1m"
$    ul           == esc+"[4m"
$    blink        == esc+"[5m"
$    rev          == esc+"[7m"
$    wide         == esc+"#6"
$    chome        == esc+"[H"
$    ceos         == esc+"[J"
$    ceol         == esc+"[K"
$! 
$    true         = 1
$    false        = 0
$! 
$    ss$_fail     = %X0000
$    ss$_normal   = %X0001
$    ss$_badparam = %X0010
$! 
$    $rstring     == ""		! global return string from subroutines.
$! 		
$    account = ""
$    passwd  = ""
$    dbnode  = ""
$    status  = ss$_normal
$! 
$    curnode = f$getsy("NODENAME")
$    curdev  = f$parse("",f$environ("DEFAULT"),,"DEVICE")
$    homedir = f$trnlnm("SYS$LOGIN")
$    homedev = f$parse(homedir,,,"DEVICE")
$    tmpdbnode  = f$trnlnm("ESI$DATABASE_NODE") - "@"
$    orasid  = f$edit(f$trnlnm("ORA_SID"),"TRIM,UPCASE,COLLAPSE")
$!
$begin:
$    account = p1
$    if f$type(sqlplus) .eqs. "" then goto err_symbols
$!
$    say off+cls+wide+"	Delete Finder Accounts"+off+esc+"[3;1H"
$    say "You are currently on node		''ul'''curnode'''off'"
$    say "Your ORACLE SID is logical is set to	''bold'''orasid'''off'"
$    say ""
$    if orasid .nes. "W" then goto askins
$    say bell+bell+bell+" "
$    say "You may not delete from instance ''bold'''orasid'''off'!!!"
$    say "Aborting without changes....."
$    goto exit
$askins:
$    ask yesno "Are you sure this is the correct instance to delete from? [''bold'NO''off'] "
$    yesno = f$extract(0,2,yesno)
$    if yesno .eqs. "YE" then goto deletedb
$    if .not. yesno then goto exit
$    goto askins
$deletedb:
$    say "Dropping ''bold'DEFAULT_PROJECT''off' account...."
$    sqlplus default_project/default_project
set termout off
set feed off
set head off
spool drop_dflt.sql
select 'DROP '||OBJECT_TYPE||' '||OBJECT_NAME||';' from user_objects;
spool off
set termout on
set feed on
start drop_dflt.sql
$! 
$    say "Dropping ''bold'CODES''off' account...."
$    sqlplus codes/codes
set termout off
set feed off
set head off
spool drop_codes.sql
select 'DROP '||OBJECT_TYPE||' '||OBJECT_NAME||';' from user_objects;
spool off
set termout on
set feed on
start drop_codes.sql
$! 
$    say "Dropping ''bold'ESI''off' account...."
$    sqlplus esi/esi
set termout off
set feed off
set head off
spool drop_esi.sql
select 'DROP '||OBJECT_TYPE||' '||OBJECT_NAME||';' from user_objects;
spool off
set termout on
set feed on
start drop_esi.sql
$! 
$exit:
$    exit 1
$! 
$err_node:
$    say "Can't locate the database node...."
$    exit 1
$! 
$err_symbols:
$    say "The oracle symbols are not defined. Please run ORAUSER first."
$    exit 1
$! 
$! 
$! ----------------------------------------------------------------------
$MKPROFILE:		!generate the login.sql user profile script in sys$login:
$    subroutine
$!    p1 = profile name.	default=SYS$LOGIN:LOGIN.SQL
$! 
$    on warning then goto proferr
$    pname = f$parse(p1,"SYS$LOGIN:LOGIN.SQL",,,"SYNTAX_ONLY") - ".;"
$    write sys$output "Creating ''pname'...."
$    create 'pname'
	SET		VERIFY	OFF
	SET		CRT		VT100
	DEFINE	_EDITOR = "LSED"
	DEFINE	EC		= "CODES.EXPRESSION_CODES"
	DEFINE	WC		= "ESI.WORKSTATION_CONFIGURATION"
	DEFINE	SF		= "LIBRARIAN.SOURCE_FILES"
	DEFINE	DP		= "DEFAULT_PROJECT"
	DEFINE  LN		= "LOGICAL_NAMES"
	DEFINE	AC		= "ANALYST_CONFIGURATION"
	COL		TIMESTAMP		FOR DATE HEADING TIME
	COL		DESCRIPTION 	FOR A10 WORD HEADING DESC
	COL		STATUS 			FOR A10 
	COL		CNAME  			FOR A16 WRAP 
	COL		TNAME  			FOR A20 WRAP 
	COL		CREATOR 		FOR A8	 
	COL		TABLETYPE  		FOR A8	HEAD TTYPE
	COL		REMARKS  		FOR A1 	HEAD R
	COL		CLUSTERID  		FOR A6	HEAD CLUS_ID
	COL		LOGBLK  		FOR B9 	HEAD LB
	COL		REQBLK 			FOR B9  HEAD RB 
	COL		IXCOMP 			FOR A1  HEAD IX 
	COL		DEFAULTVAL 		FOR A1 HEAD DV
	COL		MODULE 			FOR A3 HEAD MO
	COL		FILE_NAME		FOR A20 HEAD FNAME
	COL		EXTENSION 		FOR A4 HEADING EXT
	COL		CHECKIN_TIME 	FOR DATE HEADING CHECKIN
	COL		LINE_COUNT 		FOR 99999	HEADING LINES
	COL		COMPILE_TIME 	FOR DATE HEADING COMPTIME
	COL		MAIN_PROGRAM 	FOR A7 HEADING MAIN 
	COL		COMPILE_STATUS 	FOR A7 HEADING STATUS
	COL		COMPILE_OPTIONS FOR A10 HEADING OPTIONS
	COL		LIBRARY 		FOR A4 HEAD LIB
	COL		TABLE_TYPE 		FOR A10 HEADING TYPE
	COL		TABLE_NAME 		FOR A30 HEADING TNAME
	COL		TABLESPACE_NAME FOR A12 HEADING TABLESPACE
	COL		CLUSTER_NAME 	FOR A10 HEADING CLUSTER
	COL		BACKED_UP 		FOR A1 HEADING B
	COL		COLTYPE 		FOR A9
	COL		NULLS 			FOR A4 HEADING NULL
	COL		KEYWORD 		FOR A14
	COL		CODE 			FOR 99999
	COL		NAME 			FOR A16
	COL		REMARKS 		FOR A1 HEADING R
	COL		ENVIRONMENT 	FOR A8 HEADING ENV
	COL		LOGICAL_NAME 	FOR A18 HEADING LNAME
	COL		SCOPE 			FOR A8
	COL		TRANSLATION 	FOR A30
	COL		SEQ 			FOR 999
	COL		CONCEAL 		FOR A1 HEADING C
	COL		SHARED			FOR A1 HEADING S
	COL		OWNER			FOR A15
	COL		OBJECT_NAME		FOR A30
	COL		OBJECT_ID		FOR 999999 HEADING ID
	COL		OBJECT_TYPE		FOR A11 HEADING TYPE
	COL		CREATED			FOR DATE
	COL		MODIFIED		FOR DATE
	COL		WORKSTATION_NAME FOR A18 HEADING WS_NAME
	COL		GRAPHICS_TERMINAL_TYPE FOR A4 HEADING GTRM
	COL		GRAPHICS_DEVICE FOR A4 HEADING GDEV
	COL		GRAPHICS_TABLET_TYPE FOR A18 HEADING TABLET
	COL		GRAPHICS_HEADS FOR 9 HEADING HD
	COL		GRAPHICS_PLANES FOR 9 HEADING PL
	COL		NODE FOR A8
	COL		GRAPHICS_CHANNEL_TYPE FOR A1 HEADING CH
	COL		OPTIONS FOR 9 HEADING O
	COL		MAC_DEVICE FOR A6 HEADING MAC
	COL		GRANTOR FOR A8
	COL		GRANTEE FOR A8
$!
$endprof:
$    set noon
$    exit 1
$proferr:
$    if f$trnlnm("ORA_PATH") .nes. "" then deass ora_path
$    exit 0
$    endsubroutine
$! ---------------------------------------------------------------------

