C  DEC/CMS REPLACEMENT HISTORY, Element UGSSCAXF.FOR
C  *3    19-SEP-1989 10:22:29 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:05:07 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:55:53 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGSSCAXF.FOR
C --------------------------------------------------------
      SUBROUTINE UGSSCAXF(WKID, IFRESET, XFMATR)
C
C %S Download transform matrix to the device.
C %P Parameters:
C %P   WKID = (INT) Workstation ID
C %P   IFRESET = (INT) 0 -> Set transform.
C %P                   1 -> Reset device to NO transform
C %P   XFMATR  = (REAL ARRAY) Transform matrix in following order.
C %P             1  2  _
C %P             3  4  _
C %P             5  6  _

      INTEGER WKID, IFRESET
      INTEGER FCTID, NUMINT, INTARR(1)
      REAL XFMATR(6)

      FCTID = 1102
      NUMINT = 0
      IF (IFRESET .NE. 0) THEN
            NUMREL = 0
      ELSE
            NUMREL = 6
      ENDIF
      CALL UGESCAPE(WKID,FCTID, NUMINT, INTARR, NUMREL, XFMATR)
      RETURN
      END
