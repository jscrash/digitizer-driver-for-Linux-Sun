C  DEC/CMS REPLACEMENT HISTORY, Element UGOPENIO.FOR
C  *5     1-MAR-1990 16:44:26 WALTERS "(SPR 0) Fix errors and warnings as flagged by SUN Fortran compiler"
C  *4    28-NOV-1989 14:43:09 GORDON "(SPR 0) lowercase include files"
C  *3    12-AUG-1989 12:04:01 GILLESPIE "(SPR 3) Fix up include file references"
C  *2    11-AUG-1989 17:07:24 VINCE "(SPR -1) last fixes from UNIX port."
C  *1    10-AUG-1989 18:54:59 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGOPENIO.FOR
C  ---------------------------------------------------------------
      SUBROUTINE UGOPENIO(CHANR)
C
C %S Open Host Independant I/O system.
C %S PHIL BURK , ESI , 8/2/85
C
      INCLUDE 'ho_dev.i'
      INTEGER CHANR
      INTEGER CHANNEL, DCHANL
      INTEGER*4 STATUS       
C ...INIT I/O
      PRINT *,' Enter device name (like TXA0)'

C ...Open I/O Channel.
      STATUS = HOOPENIO( %REF(CHANNEL), %REF(DCHANL))
      CHANR = CHANNEL
C
C %Q Tell GKS what the CHANNEL is.
      CALL GKZSCHN(CHANNEL)
      RETURN
      END
