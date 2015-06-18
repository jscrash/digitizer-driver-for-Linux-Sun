$! DEC/CMS REPLACEMENT HISTORY, Element LD_RUN_WHCS_LOAD.COM
$! *11   12-JUN-1991 14:52:40 LOEBL "(SPR 0) Fix a misplaced $set default"
$! *10   13-MAY-1991 17:33:36 LOEBL "(SPR 5528) Restructured for V5 and added new function calls."
$! *9    21-DEC-1990 14:22:35 ALLAWAY "(SPR 5528) PERFORMANCE OVERHAUL"
$! *8    24-SEP-1990 16:43:43 GILLESPIE "Merge: Beta Deltas"
$!  6B1  24-SEP-1990 16:43:05 GILLESPIE "Beta Deltas"
$! *7    31-AUG-1990 17:27:25 ALLAWAY "(SPR 5890) Fix executable symbols to not reference Dorothy's directory"
$! *6    23-APR-1990 14:24:17 PURNA "(SPR 0) Dorothy's enron mods"
$! *5     6-APR-1990 16:53:15 ALLAWAY "(SPR -1) enhancements in tops handling, bug fixes"
$! *4    25-MAR-1990 20:11:29 DOROTHY "(SPR -1) Corrected an error in a logical name definition"
$! *3    25-MAR-1990 12:39:51 ALLAWAY "(SPR -1) Make the p6 prompt more intuitive and add support for updateing and moving loacation
$!tables."
$! *2    23-MAR-1990 21:50:00 DOROTHY "(SPR -1) INITIAL CHECKIN"
$! *1    19-JUN-1989 14:57:20 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element LD_RUN_WHCS_LOAD.COM
!  DEC/CMS REPLACEMENT HISTORY, Element LD_RUN_WHCS_LOAD.COM
!  *2    31-AUG-1990 17:31:53 ALLAWAY "(SPR 5890) Changed executable symbols to not reference Dorothy's directory"
!  *1    13-AUG-1990 17:44:39 SYSTEM "Finder 6.1"
!  DEC/CMS REPLACEMENT HISTORY, Element LD_RUN_WHCS_LOAD.COM
$!This command procedure was created 2/25/90 by Bill Allaway to reflect the
$!division of the control file into multiple seperate files, requiring many
$! runs of the SQL*Loader instead of only 1 and to provide for the new node
$! loader and use of loader tables serviced by sql scripts
$!
$ ! p1 = ORACLE name/password@database of the PROJECT to be loaded
$ ! p2 = Name of the Finder Project to be loaded
$!  p3 = Full pathname to the data file you wish to load.
$!  p4 = Tolerance in project projection units for sharing nodes between 2 wells
$!  p5 = Y if you want to execute the script to create the loader tables.
$!  p6 = B if you want to execute both parts of the node loader.  
$!  p6 = 2 if you want to execute only the second part of the node loader
$!	on this run. The procedure will ignore all the parameters except
$!	Project username/password@node and Project Name, but something must
$!	be entered for all parameters or the procedure will fail. This allows
$!	you to verify the results of the first pass of the node loader in the
$!	WELL_HDR_SECOND AND NODES_SECOND tables before moving the data into
$!	WELL_HDR AND NODES. 
$!	Note that both the above options requires that a table 
$!	SOURCE_SEARCH_LIST be created in the project account with the 
$!	sources of the nodes loaded in the first part indicated with a priority
$!
$!  p6 = 1 if you want to only execute pass 1 of the node loader. This is the
$!	default.
$ !
$!  p7 = P for primary data load. This will load only the well header
$!	 data and locations according to how p6 (described above) is set.
$!	 This is the minimum load for maximum performance on the loader.
$!       S for Secondary load only. This run assumes that the data for the 
$!	 Primary load has been already loaded.  This will load the data of 
$!       secondary importance or data that needs to be massaged in the loader 
$!       tables before being copied to their final destination.  Some of this 
$!       massaging can be very time consuming, especially on large data sets 
$!       where the whole transaction can not be done in physical memory. Note 
$!       that if Pass 2 of the node loader has not been done previously, it 
$!       should be done at the time the secondary 
$!       data are loaded.
$!	 B for both passes.  If this option is selected most of the data should
$!       be mappable and usable when the node loader finishes assuming Pass 2 
$!       of the node loader has been executed.
$!
$! p8=   Flag to tell the loader whether to use the Permian Basin version of 
$!       certain scripts.
$!
$ ! To run this procedure interactively, enter
$ !  @ESI$BIN:LD_RUN_WHCS_LOAD NAME/PASSWORD@NODE project filename tol Y B P N
$ !    procedure will prompt you for values if they are omitted from
$ !    command line.
$ !  
$ ! To submit this procedure for a batch run, enter
$ !  SUBMIT ESI$BIN:LD_RUN_WHCS_LOAD/PARAMETER=("NAME/PASSWORD@NODE",-
$!	project,filename,tolerance,Y,B,P,N)
$ !    NOTE: procedure cannot prompt in batch mode.  Parameters must be included
$ !
$ ! the output files will be placed in the same directory as the input file
$ ! It is suggested that this input file be placed in its own directory.
$ !
$ ! All created files take their name from the input file name and add
$ !    an appropriate extension in the directory where the input file resides
$ !
$ ! It is up to the operator to delete the files created by this procedure,
$ ! since they may be reused by other procedures.
$ !
$ userpass = f$edit(p1 ,"UPCASE")
$ project  = f$edit(p2,"UPCASE")
$ datafile = f$edit(p3,"UPCASE")
$ tolerance = f$edit(p4,"UPCASE")
$ create_flag = f$edit(p5,"UPCASE")
$ nodeload_flag = f$edit(p6,"UPCASE")
$ primary_flag = f$edit(p7,"UPCASE")
$ permian_flag = f$edit(p8,"UPCASE")
$ 
$!  p1 = Oracle Username/Password 
$!  p2 = Name of the Finder Project to be loaded
$!  p3 = Full pathname to the data file you wish to load.
$!  p4 = Tolerance in project projection units for sharing nodes between 2 wells
$!  p5 = Y if you want to execute the script to create the loader tables.
$!  p6 = B if you want to execute both parts of the node loader.  
$!  p6 = 2 if you want to execute only the second part of the node loader
$ say = "write sys$output"
$ mode = f$mode()
$ say "           Welcome to the Finder WHCS Loader"
$ say ""
$ say "If you are doing an update install, don't forget to preserve any data"
$ say "in the loader tables you wish to keep and delete all the data in them"
$ say "before proceeding.  It is also recommended that you export and "
$ say "backup your project before running this loader."
$ say ""
$
$ if userpass .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE USERPASS -
   "Enter ORACLE username/password@node for the PROJECT to be loaded"
$ if userpass .eqs. "" 
$ then 
$	say "No Project account specified - aborting"
$	exit
$ endif
$
$ if project .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE PROJECT "Enter the name of the PROJECT to be loaded"
$ if PROJECT .eqs. "" 
$ then 
$	say "No Project account specified - aborting"
$	exit
$ endif
$
$ if  project .nes. f$element(0,"/",userpass)
$ then
$      say "The procedure requires USERNAME and PROJECT to be the same."
$      exit
$ endif
$
$ if datafile .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE DATAFILE "Enter name of input file"
$ if datafile .eqs. "" 
$ then
$	say "No input file specified - aborting"
$	exit
$ endif
$ name = f$parse(datafile,,,"NAME")
$ 
$ if tolerance .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
INQUIRE TOLERANCE "Enter the tolerence in project projection units for shared nodes [Ignore]"
$ if tolerance .eqs. "" 
$ then 
$    say ""
$    say "No tolerence value specified. Node tolerances are ignored... "
$    tolerance = "I"
$ endif
$
$ if create_flag .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE CREATE_FLAG "Do you need to create the loader tables in this project [N]"
$ if create_flag .eqs. "" 
$ then 
$	say ""
$       say "Default selected, tables will NOT be created..."
$	create_flag = "N"
$ endif
$!
$ 
$ get_nodeload:
$ if nodeload_flag .eqs. "" .AND. MODE .EQS. "INTERACTIVE" 
$ THEN 
$ type sys$input  

    Enter 1 to stop after first NODE LOADER pass
          2 to begin again at second NODE LOADER pass
          B for both 

$   INQUIRE NODELOAD_FLAG  "Enter Choice [1] "
$ endif
$ if nodeload_flag .eqs. "" 
$ then 
$	say ""
$	say "No node loader option specified, locations will only be"
$	say "loaded into the WELL_HDR_SECOND and NODES_SECOND tables..."
$	nodeload_flag = "1"
$ endif 
$ if nodeload_flag .eqs. "B" .or.  nodeload_flag .eqs. "b" 
$ then 
$	say ""
$	say "Doing both node passes requires that a table named"
$	say "SOURCE_SEARCH_LIST be present in the project account"
$	say "and that it contains the sources of the nodes to be loaded "
$	say "on this run. If you know which locations you have purchased"
$	say "you can make the list ahead of time by constructing the"
$	say "sources that will be present as follows:
$	say "                       WHCS-MU 1927"
$	say "where the WHCS- is constant the MU represents the two digit"
$	say "base map code and the 1927 represents the datum used(1927 or 1983)."
$	say ""
$       PROCEED := Y                         !set default for batch , interactive is N
$       if MODE .EQS. "INTERACTIVE" THEN -
        INQUIRE PROCEED "Do you want to proceed with both node loading runs? [N]"
$       if PROCEED .eqs. "Y" .or. PROCEED .eqs. "y" then goto get_primary_flag
$       nodeload_flag = ""
$ endif 
$ if nodeload_flag .eqs. "" then goto get_nodeload
$!
$ get_primary_flag:
$ if primary_flag .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE PRIMARY_FLAG "Enter (P)rimary data load,(S)econdary load, or (B)oth [P]"
$ if primary_flag .eqs. "" 
$ then 
$	say ""
$       say "Default selected, only primary data load will be done."
$	primary_flag = "P"
$ endif 
$!
$ if permian_flag .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE PERMIAN_FLAG "Are you loading a Permian Basin project [N]"
$ if permian_flag .eqs. "" 
$ then 
$	say ""
$	say "Default requested; no Permian Basin data types will be loaded..."
$	permian_flag = "N"
$ endif
$!
$! we have all the inputs, start execution...
$!
$ if f$trnlnm("whcs_exe") .eqs. "" then  define whcs_exe ESI$BIN
$ if f$trnlnm("sql_exe")  .eqs. "" then  define sql_exe ESI$ROOT:[SCRIPTS]
$ device = f$parse(datafile,,,"DEVICE")
$ directory = f$parse(datafile,,,"DIRECTORY")
$ set default 'device''directory'
$ set noon
$!
$!
$ if nodeload_flag .eqs. "2" then goto move_locations
$ if primary_flag .eqs. "S" .or. primary_flag .eqs. "s" then goto pass2
$!
$ sqlplus 'userpass'
     start sql_exe:ld_whcs_fix_tables.sql
$!
$ if create_flag .eqs. "Y" .or. create_flag .eqs. "y" 
$ then 
$   sqlplus 'userpass'
    start sql_exe:ld_create_loader_tables.sql
$ endif
$!
$!
$ preload := $whcs_exe:ld_whcs_preload
$ preload 'datafile'
$!
$ pass1:
$! Load the output of the preloader:
$!
$ sqlplus 'userpass' @sql_exe:ld_whcs_alter_schema
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_well_hdr.ctl,-
discard='name'_11wh.dis,-
bad='name'_11wh.bad,-
log='name'_11wh.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_11wh.log to verify the 
$ say "success of this loading run."
$!
$ if nodeload_flag .eqs. "S" .or. nodeload_flag .eqs. "s" then goto skip_node_load
$!Load nodes into secondary tables:
$ say ""
$ say "Loading well location data into WELL_HDR_SECOND AND NODES_SECOND."
$ SAY ""
$ node_loader := $whcs_exe:LD_WHCS_NOD.exe
$ node_loader 'userpass' 'PROJECT' 'datafile' 'tolerance'
$!
$ if nodeload_flag .eqs. "B" then goto move_locations
$ if nodeload_flag .eqs. "b" then goto move_locations
$ goto exit
$!
$ move_locations:
$ SAY "Moving well locations into WELL_HDR AND NODES based on SOURCE_SEARCH_LIST..."
$ SAY ""
$ move_locs := $whcs_exe:LD_NODE_SEQ.exe
$ move_locs 'userpass' 'PROJECT'
$!
$ skip_node_load:
$ if primary_flag .eqs. "S" .or. primary_flag .eqs. "s" .or.  -
     primary_flag .eqs. "B" .or. primary_flag .eqs. "b" -
 THEN goto pass2
$ goto exit
$!
$pass2:
$!
$! Load the rest of the data from the original data file:
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_load_2.ctl,-
discard='name'_21PD.dis,-
bad='name'_21PD.bad,-
log='name'_21PD.log,-
silent=discards
$ !
$ say "The log files for this run have the same root name as the"
$ say "output files from the pre-processor.  Be sure to check "
$ say "these files to verify the success of the loading run."
$ say "Summary of the run can be found in ''name'_21PD.log."
$!
$! Load the rest of the data from the original data file:
$!
$!!!      $ sqlload userid='userpass',-
$!!!      control=whcs_exe:ld_whcs_case_perfs.ctl,-
$!!!      data='datafile',-
$!!!      discard='name'_22CP.dis,-
$!!!      bad='name'_22CP.bad,-
$!!!      log='name'_22CP.log,-
$!!!      silent=discards
$!!!      $ !
$!!!      $ say "Be sure to check the log file named ''name'_22CP.log to verify the 
$!!!      $ say "success of this loading run."
$!
$
$ ld_whcs_case_perfs := $whcs_exe:ld_whcs_case_perfs
$ ld_whcs_case_perfs 'userpass'  'project'  'datafile'
$
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_press_anal.ctl,-
data=ld_whcs_case_perfs.dis,-
discard='name'_23PA.dis,-
bad='name'_23PA.bad,-
log='name'_23PA.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_23PA.log to verify the"
$ say "success of this loading run."
$!
$ delete ld_whcs_case_perfs.dis;*
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_top_fault.ctl,-
data='name'_23PA.dis,-
discard='name'_24TF.dis,-
bad='name'_24TF.bad,-
log='name'_24TF.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_24TF.log to verify the"
$ say "success of this loading run."
$!
$ delete 'name'_23PA.dis;*
$!
$ if permian_flag .eqs. "Y" .or. permian_flag .eqs. "y" 
$then
$   sqlload userid='userpass',-
    control=whcs_exe:ld_whcs_core_restpb.ctl,-
    data='name'_24TF.dis,-
    discard='name'_25CR.dis,-
    bad='name'_25CR.bad,-
    log='name'_25CR.log,-
    silent=discards
$else
$   sqlload userid='userpass',-
    control=whcs_exe:ld_whcs_core_rest.ctl,-
    data='name'_24TF.dis,-
    discard='name'_25CR.dis,-
    bad='name'_25CR.bad,-
    log='name'_25CR.log,-
    silent=discards
$endif
$ !
$ say "Be sure to check the log file named ''name'_25CR.log to verify the" 
$ say "success of this loading run."
$!
$ delete 'name'_24TF.dis;*
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_locs.ctl,-
data='name'_25CR.dis,-
discard='name'_26LO.dis,-
bad='name'_26LO.bad,-
log='name'_26LO.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_26LO.log to verify the" 
$ say "success of this loading run."
$!
$ delete 'name'_25CR.dis;*
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_misc.ctl,-
data='name'_26LO.dis,-
discard='name'_27MI.dis,-
bad='name'_27MI.bad,-
log='name'_27MI.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_27MI.log to verify the" 
$ say "success of this loading run."
$!
$ delete 'name'_26LO.dis;*
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_bad_tops.ctl,-
data='name'_24TF.bad,-
discard='name'_28TFbt.dis,-
bad='name'_28TFbt.bad,-
log='name'_28TFbt.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_28TFbt.log to verify the"
$ say "success of this loading run."
$!
$ sqlplus 'userpass'
start sql_exe:ld_whcs_dump_tops.sql
$!
$ sqlload userid='userpass',-
control=whcs_exe:ld_whcs_loader_tops.ctl,-
data=loader_well_tops.dat,-
discard='name'_29TFlt.dis,-
bad='name'_29TFlt.bad,-
log='name'_29TFlt.log,-
silent=discards
$ !
$ say "Be sure to check the log file named ''name'_29TFlt.log to verify the"
$ say "success of this loading run."
$!
$ date_check := $whcs_exe:LD_well_hdr_dates.exe
$ date_check  'userpass' 'PROJECT' 
$ 
$ if permian_flag .eqs. "Y" .or. permian_flag .eqs. "y" 
$then 
$    sqlplus 'userpass'
     start sql_exe:ld_whcs_move_tabs.sql PERMIAN
$else
$    sqlplus 'userpass'
     start sql_exe:ld_whcs_move_tabs.sql NOPERMIAN
$endif
$!
$!
$ exit:
$say "Running DB_VER_TABS to recreate dropped indexes"
$
$
$ @esi$bin:db_ver_tabs -i 'userpass'
$ say " "
$ say "loading complete - have a nice day"
$ exit

