$! DEC/CMS REPLACEMENT HISTORY, Element SET_INTERP_LOGICALS.COM
$! *1    19-JUN-1989 14:57:57 SYSTEM ""
$! DEC/CMS REPLACEMENT HISTORY, Element SET_INTERP_LOGICALS.COM
$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$!
$!   remi Doret  dec 87
$!
$!    SET_INTERP_LOGICALS
$!
$!    procedure  called in the login of processes wanting
$!    to run the integrated version INTERPFINDER
$!    this procedure is defining the logicals necessary to run
$!    there are job logicals  ( also used by subprocesses )
$!    and process logicals
$!
$!    job logicals       INTERP$EXE
$!                       INTERP_MUNXSHR
$!                       INTERP_PERKINSHR
$!                       INTERP_MASTERSHR
$!    process logicals   INTERP$MENU
$!                       INTERP$INI
$!
$!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
$!
$ SET NOON
$!
$! define job logical pointing to the directory of executables
$!
$ DEFINE/TABLE=LNM$JOB/NOLOG  INTERP$EXE  ESI$ROOT:[INTERPRET.BIN]
$ DEFINE/TABLE=LNM$JOB/NOLOG  INTERP$BUILD  ESI$ROOT:[INTERPRET.BUILD]
$!
$! define job logical pointing to
$!  the shareable image for MUNX
$!
$ DEFINE/TABLE=LNM$JOB/NOLOG INTERP_MUNXSHR  INTERP$EXE:INTERP_MUNXSHR
$!
$! define job logical pointing to
$!  the shareable image for PERKIN
$!
$ DEFINE/TABLE=LNM$JOB/NOLOG INTERP_PERKINSHR  INTERP$EXE:INTERP_PERKINSHR
$!
$! define job logical pointing to
$!  the shareable image for MASTERTASK
$!
$ DEFINE/TABLE=LNM$PROCESS/NOLOG INTERP_MASTERSHR  INTERP$EXE:INTERP_MASTERSHR
$!
$!
$!  define process logical INTERP$MENU which points to a directory
$!  containing all the files necessary for the raster menus
$!  for  FINDER-INTERPRET
$!
$!
$ DEFINE/NOLOG INTERP$MENU  esi$root:[interpret.MENU]
$!
$!
$!  define process logical INTERP$INI which points to a directory
$!  containing the initialisation files ( independant from
$!  a seismic survey or project ) which are used
$!  by FINDER-INTERPRET
$!
$!
$ DEFINE/NOLOG INTERP$INI  esi$root:[interpret.ini]
$!
$! define symbol to run interpret_finder
$!
$ INTERP :== @INTERP$EXE:RUN_FI_INTERP
$!
$! define symbol to run 2D  SEGY LOADER
$!
$ SEGY2D :== @INTERP$EXE:RUN_SEGY2D
$!
$!
$! define symbol to run 2D  SEGY LOADER
$!
$ SEGY3D :== @INTERP$EXE:RUN_SEGY3D
$!
$! define symbol to make job files for the SEGY loader
$!
$ MAKEJOB :== $INTERP$EXE:INLOAD
$!
$!
$ EXIT
