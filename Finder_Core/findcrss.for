C  DEC/CMS REPLACEMENT HISTORY, Element FINDCRSS.FOR
C  *2    19-SEP-1989 10:10:50 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:47:24 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element FINDCRSS.FOR

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
*
* FUNCTION : BUILD A CROSSING VECTOR IN INTERPRET FORMAT
*
*  LINEN :     LINENAME 
*  IBUF  :     BUFFER CONTAINING NAME AND COORDINATES OF THE INTERSECTIONS
*  ICNTR :    COMPATIBILITY WITH INTERP CALLING SEQUENCE   
*  IOPT  :    COMPATIBILITY WITH INTERP CALLING SEQUENCE   
*  INFO  :    COMPATIBILITY WITH INTERP RETURN STATUS   
*
* LANGUAGE: FORTRAN
*
*
* TASK IMAGE: NO
*
*---------------------------------------
              SUBROUTINE  FINDCRSS ( LINEN ,IBUF ,ICNTR ,IOPT, INFO )
*---------------------------------------
C
C
              INTEGER*4 LINEN(*),IBUF(*),ICNTR(*),IOPT(*),INFO(*)
C
              INTEGER*4 IFIRST , ILAST , ICOEF , LNBUF 
CC
              CHARACTER*20 LINENAM
              INTEGER*4  LIN(5)
              EQUIVALENCE ( LIN , LINENAM )
C
C
              ICOEF = 1000
C
C
              LNBUF = ICNTR(3)
C
              IFIRST = -2000000
              ILAST  = -2000000
C
              DO 10 I = 1 , 5
10            LIN(I) = LINEN(I)
C
              CALL GFINDCRS ( LINENAM , IFIRST , ILAST, IBUF , LNBUF ,
     -                       ICOEF , INFO )
C
              INFO(4) = ICOEF
C
C
C
              END
