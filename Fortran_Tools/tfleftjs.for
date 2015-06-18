C  DEC/CMS REPLACEMENT HISTORY, Element TFLEFTJS.FOR
C  *2    19-SEP-1989 10:18:20 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:53:21 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TFLEFTJS.FOR
      SUBROUTINE TFLEFTJS (STRIN, STROUT,LENNBL)
C
C %S  Left justify character string.
C
C %A  Author: Phil Burk, 9/11/85
C
C %P  IN:
C %P    STRIN  = Input character string.
C %P  OUT:
C %P    STROUT = Left justified string.
C %P    LENNBL = Length of non blenk portion of STROUT.
C %P
C %C Copyright ESI 9/11/85
C
      CHARACTER STRIN*(*), STROUT*(*)
      INTEGER TFNBLEN
C
C ...Determine length of strings.
      LIN = LEN (STRIN)
      LOUT = LEN (STROUT)
C
C ...Scan for first nonblank.
      DO 100 J = 1,LIN
         LB = J
         IF (STRIN(LB:LB) .NE. ' ') GOTO 200
100   CONTINUE
C
C ...Input string blank!!!
      STROUT = ' '
      LENNBL = 0
      RETURN
C
C ...Move strin to left part of strout.
200   CONTINUE
      LINNB = TFNBLEN(STRIN)
      LENNBL = LINNB - LB + 1
      IF (LENNBL .GT. LOUT) LENNBL = LOUT
      STROUT = STRIN(LB:LB+LENNBL-1)
C
      RETURN
      END
