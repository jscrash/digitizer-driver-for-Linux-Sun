C  DEC/CMS REPLACEMENT HISTORY, Element UGESCAPE.FOR
C  *4     1-MAR-1990 16:44:14 WALTERS "(SPR 0) Fix errors and warnings as flagged by SUN Fortran compiler"
C  *3    19-SEP-1989 10:21:03 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:03:55 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:54:55 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGESCAPE.FOR
      SUBROUTINE UGESCAPE(WKID, FCTID, NUMINT, INTARR, NUMREL, RELARR)
C
C %S Easy way to call an escape function in GKS
C %P Parameters:
      INTEGER WKID, FCTID, NUMINT, INTARR(1), NUMREL
      REAL RELARR(1)
      INTEGER*4 UGG_IF_GPX
      INTEGER IL, IA(130)
      INTEGER LSTR, NSTR, MLDR
      INTEGER ERRIND, LDR
      INTEGER MLODR, LODR
      CHARACTER STR*80,  DATREC(128)*80
      CHARACTER ODR(16)*80
      INTEGER*4 IFGPX

      IFGPX = UGG_IF_GPX()
C
C ...Check for too large a buffer.
      IF (NUMINT .GT. 128) NUMINT = 128

C Arrange data properly in IA, NOVA GKS specific.
      IA(1) = WKID
      DO 100 J = 1,NUMINT
         IA(J+1) = INTARR(J)
100   CONTINUE
      IL = NUMINT + 1

C ...Pack data into data record for escape call.
      LSTR = 80
      NSTR = 0
      MLDR = 128
      CALL GPREC (IL,IA,NUMREL,RELARR, NSTR,LSTR,STR,
     1            MLDR, ERRIND,LDR,DATREC)
      IF (ERRIND .NE. 0) THEN
            PRINT *,'BAD PACK, ERRIND = ',ERRIND
      ENDIF
C
C Send packed data record to device.
      MLODR = 16
      CALL GESC (FCTID, LDR, DATREC, MLODR, LODR, ODR)
C
C Unpack data to test NOVA escape failure.
C %%%      IIL = 64
C %%%      IRL = 64
C %%%      ISTR = 80
C %%%      CALL GUREC (MLDR, DATREC, IIL, IRL, ISTR, ERRIND,
C %%%     1            IL, IA, NUMREL,  RELARR, NSTR, LSTR, STR)
C %%%      IF(ERRIND .NE. 0) THEN
C %%%          PRINT *,' Bad unpack = ',ERRIND
C %%%      ENDIF
      RETURN
      END

