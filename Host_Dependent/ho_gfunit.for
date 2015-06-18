C  DEC/CMS REPLACEMENT HISTORY, Element HO_GFUNIT.FOR
C  *2    19-SEP-1989 10:11:00 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:47:33 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element HO_GFUNIT.FOR
C
      SUBROUTINE HOGETFUNIT(FILUNIT)
C
C%S  Allocate an unused file unit.
C%S  Author: Jeff Fried.
C%M  MOD: PLB 7/19/85  Converted calling sequence.
C%M  MOD: PLB 7/22/85  Corrected logic error.
C
      INTEGER I, FILUNIT
      LOGICAL ISOPEN
C
      DO 10 I = 50, 90
         INQUIRE ( UNIT = I, OPENED = ISOPEN )
         IF ( .NOT. ISOPEN ) THEN
            FILUNIT = I
            RETURN
         ENDIF
10    CONTINUE
C
C...No unit found.
      FILUNIT = -1
C
      RETURN
C
      END
