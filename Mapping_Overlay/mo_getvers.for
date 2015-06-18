C  DEC/CMS REPLACEMENT HISTORY, Element MO_GETVERS.FOR
C  *3     3-NOV-1989 12:01:54 GILLESPIE "(SPR 30) Change entry points for new n-List nomenclature"
C  *2    19-SEP-1989 10:13:36 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:48:45 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element MO_GETVERS.FOR
C
C SUBROUTINE TO MAKE A UNIQUE NAME BY PUTTING A VERSION NUMBER ON THE END.
C
      SUBROUTINE MO_GETVERS(FILE)
C
C FILE IS FILE NAME
C
      CHARACTER*(*)FILE
      CHARACTER*5 FMT
      LOGICAL EXIST
C
C MAKE SURE WE HAVE ENOUGH ROOM TO ADD _nn TO FILE
C
      L1 = LEN(FILE)
      L2 = ISTRLEN(FILE)
      IF(L2+3 .GT. L1)L2 = L1-3
C
C SET STARTING COLUMN (L2) AND ENDING COLUMN (L3)
C
      FILE(L2+1:L2+2)='_0'
      L2 = L2 + 3
      L3 = L2
C
C FORMAT FOR 0-9
C
      FMT = '(I1)'
      DO 100 JK = 0,99
C
C CHANGE FORMAT FOR DOUBLE DIGITS
C
      IF(JK.EQ.10) THEN
          L2 = L2 - 1
          FMT = '(I2)'
      ENDIF
C
C BUILD FILE NAME
C
      WRITE(FILE(L2:L3),FMT)JK
C
C SEE IF EXISTS
C
      INQUIRE(FILE = FILE, EXIST = EXIST)
C
C IF NOT WE'S FINE
C
      IF( .NOT. EXIST) RETURN
100   CONTINUE
C
C GETVERS FAILES SO REMOVE VERSION NUMBER
C
      FILE(L2-2:L1)=' '
      RETURN
      END


