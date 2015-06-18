#!/bin/csh
if "$2" == "" then
   echo Usage: ld_tobin_assemble username/password project datafile 
   exit 1
endif
#
# First, get rid of some indexes, which will enhance load performance.
# Note: This script assumes the project password is the same as the
#       Project name.
#
sqlplus "$2/$2"  @$FINDER_HOME/scripts/ld_tobin_assemble.sql "$2"
#
# Now load the data
#
$FINDER_BINARY/ld_tobin_topo    "$1" $2 $3 $4
#
# Restore the indexes
#
$FINDER_BINARY/db_ver_tabs -i $1
#
# build the compiled land grid objects
#
$FINDER_BINARY/ld_compile_land  "$1" $2 SECTION $4
$FINDER_BINARY/ld_compile_land  "$1" $2 TOWNSHIP $4
