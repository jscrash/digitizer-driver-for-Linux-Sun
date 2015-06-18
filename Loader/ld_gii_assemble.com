$! DEC/CMS REPLACEMENT HISTORY, Element LD_GII_ASSEMBLE.COM
$! *4    12-APR-1990 17:39:42 VINCE "(SPR 1) fixed vflag usage"
$! *3    26-MAR-1990 12:53:32 VINCE "(SPR 1) added set [no]verify"
$! *2    25-MAR-1990 20:54:26 VINCE "(SPR 1) Initial Checkin"
$! *1    25-MAR-1990 19:38:25 VINCE "Run all the parts of the GII loader."
$! DEC/CMS REPLACEMENT HISTORY, Element LD_GII_ASSEMBLE.COM
$
$ ! p1 = account/password@node
$ ! p2 = project name
$ ! p3 = GII data file
$
$ set noon
$
$ ! set up symbols
$ ld_gii_topo	    :== $esi$bin:ld_gii_topo
$ mk_section_lbl    :== $esi$bin:mk_section_lbl
$ ly_g_twsh_rec	    :== $esi$bin:ly_g_twsh_rec
$ ld_compile_land   :== $esi$bin:ld_compile_land
$
$ vflag = f$verify(1)
$
$! load the data
$ ld_gii_topo	    "''p1'" 'p2 'p3
$
$! create the section labels
$ mk_section_lbl    "''p1'" 'p2
$
$! build the township boundry records
$ ly_g_twsh_rec	    "''p1'" 'p2
$
$! build the compiled land grid objects
$ ld_compile_land   "''p1'" 'p2 SECTION
$ ld_compile_land   "''p1'" 'p2 TOWNSHIP
$
$ if .not. vflag then set noverify
$ exit
