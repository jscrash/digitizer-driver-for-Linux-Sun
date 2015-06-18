C  DEC/CMS REPLACEMENT HISTORY, Element TSUPPER.FOR
C  *2    19-SEP-1989 10:20:23 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:54:36 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TSUPPER.FOR
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
* DATE : 20/7/1987
C
C             ROUTINE USED TO CONVERT LOWER CASE IN UPPER CASE
C
C             IN THE STRING      STRING
C
C
C *******************************************************************

             SUBROUTINE TSUPPER ( STRING )

           
             CHARACTER*(*) STRING
             INTEGER   ITRANS

             DO 10 I = 1 , LEN  ( STRING )
              ITRANS = ICHAR ( STRING(I:I))

              IF ( ITRANS .LT. 123 .AND . ITRANS .GT. 96 )THEN
              STRING (I:I) =CHAR ( ITRANS - 32 )
              ENDIF

10           CONTINUE

             END

