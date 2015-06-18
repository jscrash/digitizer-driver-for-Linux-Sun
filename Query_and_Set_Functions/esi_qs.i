C  DEC/CMS REPLACEMENT HISTORY, Element ESI_QS.I
C  *1    30-JUN-1989 15:17:13 GILLESPIE ""
C  DEC/CMS REPLACEMENT HISTORY, Element ESI_QS.I
C     
C     $Header: /j/finder/inc/RCS/esi_qs.i,v 1.1 2008/02/09 09:16:37 julian Exp $ esi_qs.i 
C     
C ESI_QS.I                  Doug Canfield    April 9, 1987
C query/set constants for FORTRAN77
C Copyright (c) 1985, Exploration Systems Inc, Sausalito, CA.
C
C TITLE : ESI - query/set constants for FORTRAN77
C
C START-HISTORY :
C 11/19/85 JOHN     INITIAL CODING
C 11/19/85 J GILLESPIE Template constructed
C 04/09/87 D Canfield converted to FINDER Version 5.
C END-HISTORY
C
C
C START-CODE :
C EQUATES FOR CHARACTER STATE VARIABLES
C
      PARAMETER ( IQS_FINDER_ACCOUNT=0)
      PARAMETER ( IQS_FINDER_PASSWORD=1)
      PARAMETER ( IQS_PROJECT_NAME=2)
      PARAMETER ( IQS_PROJECT_PROJECTION=3)
      PARAMETER ( IQS_CUR_WELL=4)
      PARAMETER ( IQS_CUR_SEISMIC=5)
      PARAMETER ( IQS_CUR_LEASE=6 )
      PARAMETER ( IQS_CUR_MAP=7)
      PARAMETER ( IQS_CUR_GRAPHIC_OBJECT=8)
      PARAMETER ( IQS_LAST_WELLS_RETRIEVED=9 )
      PARAMETER ( IQS_LAST_SEISMIC_RETRIEVED=10 )
      PARAMETER ( IQS_LAST_LEASES_RETRIEVE=11 )
      PARAMETER ( IQS_LAST_MAPS_RETRIEVED=12 )
      PARAMETER ( IQS_LAST_GRAPHICS_RETRIEVED=13 )
      PARAMETER ( IQS_CUR_WELL_LIST=14 )
      PARAMETER ( IQS_CUR_SEISMIC_LIST=15 )
      PARAMETER ( IQS_CUR_LEASE_LIST=16 )
      PARAMETER ( IQS_CUR_MAP_LIST=17 )
      PARAMETER ( IQS_CUR_GRAPHIC_LIST=18 )
      PARAMETER ( IQS_CURR_MAP_NAME=19 )
      PARAMETER ( IQS_CURR_MAP_UOM=20 )
      PARAMETER ( IQS_MAP_PROJECTION=21 )
      PARAMETER ( IQS_WORKSTATION_NAME=22 )
      PARAMETER ( IQS_TERMINAL_TYPE=23 )
      PARAMETER ( IQS_UOM_INIT=24 )
      PARAMETER ( IQS_DFLT_MAP_DUOM=25 )
      PARAMETER ( IQS_DATABASE=26 )
      PARAMETER ( IQS_DGI_FILE=27 )
      PARAMETER ( IQS_MAP_DEVICE=28 )
      PARAMETER ( IQS_DGI_ROOTNAME=29 )
      PARAMETER ( IQS_WORKSTATION_TYPE=30)
C
C EQUATES FOR FLOAT STATE VARIABLES
C
      PARAMETER ( IQS_DGI_XMIN=0)
      PARAMETER ( IQS_DGI_YMIN=1)
      PARAMETER ( IQS_DGI_SCALE_FACTOR=2)
C
C
C EQUATES FOR INTEGER STATE VARIABLES
C
      PARAMETER ( IQS_IF_BATCH=0)
      PARAMETER ( IQS_WELL_LIST_EXISTS=1 )
      PARAMETER ( IQS_SEISMIC_LIST_EXISTS=2 )
      PARAMETER ( IQS_LEASE_LIST_EXISTS=3 )
      PARAMETER ( IQS_MAP_LIST_EXISTS=4 )
      PARAMETER ( IQS_GRAPHIC_LIST_EXISTS=5 )
      PARAMETER ( IQS_OVERLAY_LIST_SIZE=6 )
      PARAMETER ( IQS_TABLET_TYPE=7 )
      PARAMETER ( IQS_GRAPHIC_TERM=8 )
      PARAMETER ( IQS_USER_SKILL=9 )
      PARAMETER ( GLOBAL_MENU_ID=10 )
      PARAMETER ( IQS_CUR_MAP_NUMBER=11 )
      PARAMETER ( IQS_DIVIDE_BY_2=12)
      PARAMETER ( IQS_ADD_1K=13)
C
C PROGRAMMERS: IF ANY OF THE FOLLOWING LIMITS IS EXCEEDED, YOU
C              WILL NEED TO EDIT ESI.QS.H TO INCREASE ARRAY LIMITS
C              AND RECOMPILE QS_ROUTINES.  NO OTHER CHANGES SHOULD
C              BE REQUIRED...
C
C CURRENT ARRAY LIMITS:
C CHARACTER: 50
C FLOAT    : 50
C INTEGER  : 50
C POINTERS : 25

C END-CODE
