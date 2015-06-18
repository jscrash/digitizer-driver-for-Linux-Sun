C  DEC/CMS REPLACEMENT HISTORY, Element TSZEROES.FOR
C  *2    19-SEP-1989 10:20:33 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:54:41 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TSZEROES.FOR
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
* AUTHOR:JAMES CHANG
*
* DATE : 25/3/1988
C
C             ROUTINE USED TO SET ALL CHAR. TO BE BLANK
C
C             IN THE STRING      STRING
C
C
C *******************************************************************

             SUBROUTINE TSZEROES ( STRING )

           
             CHARACTER*(*) STRING

             DO 10 I = 1 , LEN  ( STRING )
              STRING (I:I) = ' '

10           CONTINUE

             END

