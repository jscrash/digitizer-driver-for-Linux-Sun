C  DEC/CMS REPLACEMENT HISTORY, Element LDEXTRFN.FOR
C  *2    19-SEP-1989 10:11:28 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:47:49 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element LDEXTRFN.FOR

***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 14/7/87

C
C
C
C
C       LDEXTRFN   :   routine used to extract the filename
c                     from a character string composed of the 
c                     logical + the filename
c        input INSTR
c        output OUTSTR
c
*******************************************************************/

          SUBROUTINE LDEXTRFN ( INSTR , OUTSTR )

           CHARACTER*(*)  INSTR , OUTSTR
C
c       function TFNBLEN  use to look for the last non blank character
c            in the string
c
           INTEGER TFNBLEN , IL , I
C
C        look for the character :  in the input string
c
          IL = TFNBLEN ( INSTR )
C
             DO 10 I = IL , 1 , -1  
             IF ( INSTR (I:I) .EQ. ':' ) GOTO 20
             IF ( INSTR (I:I) .EQ. ']' ) GOTO 20
             IF ( INSTR (I:I) .EQ. '/' ) GOTO 20
10           CONTINUE
             I = 0
20           CONTINUE

                IMINC = MIN0 (LEN(OUTSTR) , IL - I +1 )         
                OUTSTR (1:) = INSTR (I+1 : I+IMINC)

	   END
