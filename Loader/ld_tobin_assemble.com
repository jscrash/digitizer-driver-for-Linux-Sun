$! DEC/CMS REPLACEMENT HISTORY, Element LD_TOBIN_ASSEMBLE.COM
$! *3    17-SEP-1991 14:03:12 LOEBL "(SPR 3951) Add sqlplus processing"
$! *2    27-APR-1990 17:56:53 VINCE "(SPR 1) Initial Checkin"
$! *1    27-APR-1990 17:52:22 VINCE "ss"
$! DEC/CMS REPLACEMENT HISTORY, Element LD_TOBIN_ASSEMBLE.COM
$
$ ! p1 = account/password@node
$ ! p2 = project name
$ ! p3 = TOBIN data file
$ ! p4... = other options
$
$ if p3 .eqs. "" 
$ then
$     type sys$input

Usage: @LD_TOBIN_ASSEMBLE  Username/Password  Project  Datafile

$     exit
$ endif
$ set noon
$!
$! set up symbols
$!
$ db_ver_tabs       := $esi$bin:db_ver_tabs
$ ld_tobin_topo	    := $esi$bin:ld_tobin_topo
$ ld_compile_land   := $esi$bin:ld_compile_land
$!
$! First, get rid of some indexes, which will enhance load performance.
$! Note: This script assumes the project password is the same as the
$!        Project name.
$!
$ sqlplus 'p2'/'p2' @ESI$ROOT:[SCRIPTS]ld_tobin_assemble.sql 'p2'
$!
$! Now load the data
$!
$ ld_tobin_topo    'p1' 'p2' 'p3'  'p4'
$!
$! Restore the indexes
$!
$ db_ver_tabs -i 'p1'
$!
$! build the compiled land grid objects
$!
$ ld_compile_land  'p1' 'p2' SECTION   'p4' 
$ ld_compile_land  'p1' 'p2' TOWNSHIP  'p4'
$
$ exit
