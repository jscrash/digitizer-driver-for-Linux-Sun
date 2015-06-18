$! DEC/CMS REPLACEMENT HISTORY, Element LD_SUBMIT_WHCS_LOAD.COM
$! *5    21-DEC-1990 14:23:40 ALLAWAY "(SPR 5528) PERFORMANCE OVERHAUL"
$! *4    23-APR-1990 14:24:24 PURNA "(SPR 0) Dorothy's enron mods"
$! *3     6-APR-1990 16:55:14 ALLAWAY "(SPR -1) enhancements to allow user to specify que name"
$! *2    23-MAR-1990 21:45:51 DOROTHY "(SPR -1) INITIAL CHECKIN"
$! *1    23-MAR-1990 21:44:28 DOROTHY "BATCH VERSION OF THE WHCS LOADER DRIVER SHELL."
$! DEC/CMS REPLACEMENT HISTORY, Element LD_SUBMIT_WHCS_LOAD.COM
$ ! p1 = ORACLE name/password@database of the PROJECT to be loaded$!  p3 = Full pathname to the data file you wish to load.
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
$!  p6 = 1 if you want to only execute pass 1 of the node loader. this is the
$!	default.
$ !
$!  p7 = P for primary data load. This will load all the data, including header
$!	 data that will go directly into the primary tables without massaging.
$!	 This is the minimum load for maximum performance on the loader.
$!       S for Secondary load only. This run assumes that the data for the Primary
$!       load has been already loaded.  This will load the data of secondary importance
$!	 or data that needs to be massaged in the loader tables before being
$!	 copied to their final destination.  Some of this massaging can be very
$!	 time consuming, especially on large data sets where the whole transaction 
$!	 can not be done in physical memory. Note that if Pass 2 of the node loader 
$!       has not been done previously, it should be done at the time the secondary 
$!       data are loaded.
$!	 B for both passes.  If this option is selected most of the data should be
$!	 mappable and usable when the node loader finishes assuming Pass 2 of the 
$!	 node loader has been executed.
$!
$! p8=   Flag to tell the loader whether to use the Permian Basin version of 
$!       certain scripts.
$!
$ ! To run this procedure interactively, enter
$ !  @ESI$BIN:LD_RUN_WHCS_LOAD NAME/PASSWORD@NODE project filename tol Y B P N
$ !    procedure will prompt you for values if they are omitted from
$ !    command line.
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
$ say = "write sys$output"
$ mode = f$mode()
$ say "           Welcome to the Finder WHCS Loader"
$ say ""
$ say "If you are doing an update install, don't forget to preserve any data"
$ say "in the loader tables you wish to keep and delete all the data in them"
$ say "before proceeding.  It is also recommended that you export and "
$ say "backup your project before running this loader."
$ say ""
$ if p1 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE P1 "Enter ORACLE name/password@node for the PROJECT to be loaded"
$ if p1 .eqs. "" then goto no_name
$ if p2 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE P2 "Enter the name of the PROJECT to be loaded"
$ if p2 .eqs. "" then goto no_name
$ if p3 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE P3 "Enter name of input file"
$ if p3 .eqs. "" then goto no_input_file
$ get_p4:
$ if p4 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
INQUIRE P4 "Enter the tolerence in project projection units for shared nodes (Ignore)"
$ if p4 .eqs. "" then goto no_tolerence_value
$ get_p5:
$ if p5 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
   INQUIRE P5 "Do you need to create the loader tables in this project(N)"
$ if p5 .eqs. "" then goto no_loader_table_flag
$ get_p6:
$ if p6 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE P6 "Enter B to do nodes pass 1 & 2,2 for only pass 2, 1 for 1 only (1)"
$ if p6 .eqs. "" then goto no_search_list
$ if p6 .eqs. "B" .or.  p6 .eqs. "b" then goto verify_nodes2 
$ get_p7:
$ if p7 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE P7 "Enter (P)rimary data load,(S)econdary load, or (B)oth (P)"
$ if p7 .eqs. "" then goto no_pass2_flag
$ get_p8:
$ if p8 .eqs. "" .AND. MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE P8 "Are you loading a Permian Basin project (N)"
$ if p8 .eqs. "" then goto no_permian
$ if MODE .eqs. "INTERACTIVE" THEN -
   INQUIRE QUE_NAME "To which que should this be submitted(SYS$BATCH)"
$ if QUE_NAME .eqs. "" then QUE_NAME = "sys$batch"
$!
$ load_it:
$!
$ submit/keep/noprint/log=sys$login:run_whcs_load.log/que='QUE_NAME'-
	/PARAMETERS=("''p1'",'p2','p3','p4','p5','p6','p7','p8') -
	esi$root:[bin]ld_run_whcs_load.com
$ exit:
$ say "Submit complete - have a nice day"
$ exit
$ no_input_file:
$	say "No input file specified - aborting"
$	exit
$ no_name:
$	say "No Project account specified - aborting"
$	exit
$ no_loader_table_flag:
$ say ""
$ say "Default selected, tables will NOT be created..."
$	p5 = "N"
$	goto get_p6
$ no_tolerence_value:
$ say ""
$ say "No tolerence value specified. Every node will be unique... "
$	p4 = "I"
$	goto get_p5
$ no_search_list:
$	say""
$	say "No node loader pass2 option specified, locations will only be"
$	say "loaded into the WELL_HDR_SECOND and NODES_SECOND tables..."
$	p6 = "1"
$ goto get_p7
$ verify_nodes2:
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
$ if MODE .EQS. "INTERACTIVE" THEN -
  INQUIRE PROCEED "Do you want to proceed with both node loading runs?"
$ if PROCEED .eqs. "Y" .or. PROCEED .eqs. "y" then goto get_p7
$ p6 = ""
$ goto get_p6
$ no_pass2_flag:
$ say ""
$ say "Default selected, only primary data load will be done."
$	p7 = "P"
$	goto get_p8
$ no_permian:
$	say "Default requested, no Permian Basin data types will be loaded..."
$	p8 = "N"
$	goto load_it
