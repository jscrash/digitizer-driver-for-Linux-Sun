C  DEC/CMS REPLACEMENT HISTORY, Element UGRECT.FOR
C  *4    18-DEC-1989 10:47:32 GILLESPIE "(SPR 5077) Use WMG_GPL"
C  *3    19-SEP-1989 10:22:03 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:04:40 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:55:39 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGRECT.FOR
C ----------------------------------------------------------------
      SUBROUTINE UGRECT (X1,Y1,X2,Y2, IFFILL)
C
C %S Draw rectangle filled or unfilled.
C
C %P Input parameters:
C     X1,Y1 = (REAL) lower left corner.
C     X2,Y2 = (REAL) upper right corner.
C     IFFILL = (INT) flag for filling or not.
C              0 -> Use GPL.
C              1 -> Use GFA.
      INTEGER IFFILL
      REAL X1,Y1,X2,Y2
      REAL XA(5), YA(5)

C Fill array.
      XA(1) = X1
      YA(1) = Y1

      XA(2) = X1
      YA(2) = Y2

      XA(3) = X2
      YA(3) = Y2

      XA(4) = X2
      YA(4) = Y1

C Draw primitive.
       IF (IFFILL .EQ. 0) THEN
           XA(5) = X1
           YA(5) = Y1
           CALL WMG_GPL(5,XA,YA)
       ELSE
           CALL GFA(4,XA,YA)
       ENDIF

      RETURN
      END

