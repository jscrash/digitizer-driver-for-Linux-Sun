C  DEC/CMS REPLACEMENT HISTORY, Element UGOPERR.FOR
C  *4    10-OCT-1989 12:40:33 GORDON "(SPR 4999) Get filename from argument, fix error message"
C  *3    19-SEP-1989 10:21:11 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:04:07 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:55:04 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGOPERR.FOR
C -----------------------------------------------------------
      SUBROUTINE UGOPERR(ERRFIL,FNAME)
C
C %S Open a file for the error messages to appear on.
C %S Allocate the error file logical unit number.
C %S
C %S Author: Phil Burk
C %S
C
      CHARACTER*(*) FNAME
      INTEGER ERRFIL
      LOGICAL ISOPEN

C
C ... Check to see if its already open
C
      INQUIRE(FILE=FNAME,OPENED=ISOPEN,NUMBER=ISFILE)
C
C ... If it is, return
C
      IF(ISOPEN) THEN
          ERRFIL = ISFILE
          PRINT '(A,I3)',' GKS error log already open on:',ISFILE
          RETURN
      ENDIF

C ...Allocate unit number.
      CALL HOGFNT(ERRFIL)

C ...OPEN it.
      IF (ERRFIL .LT. 0) GOTO 88888
      OPEN (UNIT=ERRFIL, FILE=FNAME, IOSTAT = IOS,
     1         STATUS = 'UNKNOWN',ERR=88888)
      PRINT 6000, FNAME
6000  FORMAT(' GKS errors will be logged to ',A)
      RETURN

C ...Error trap.
C ...Report using PRINT since other parts of system not yet operational.
88888 CONTINUE
      PRINT *,'Error opening error log file'
      ERRFIL = 0
      RETURN
      END
