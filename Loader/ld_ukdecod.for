C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKDECOD.FOR
C  *4    19-SEP-1989 10:11:38 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *3    10-AUG-1989 18:57:00 VINCE "(SPR -1) Fortran code after UNIX mods"
C  *2     4-AUG-1989 12:09:58 ADEY "(SPR -1) UPDATE THE VERSION FOR LD_UKOOA"
C  *1     3-AUG-1989 15:14:33 ADEY "support routine for LD_UKOOA"
C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKDECOD.FOR
***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*              FINDER GRAPHICS SYSTEMS, INC               *
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
* DATE : 12/7/87
*
* FUNCTION : to decode integer or floating format from 
*              an ascii represention
*            this routine doesn't decode  e format
*
* LANGUAGE: FORTRAN
*
* PARAMETERS:  INPUT PARAMETERS =
*              string  : character string
*              ideb    :  index in string of the first character to decode
*              ilast   :  index in string of the last character to decode
*              iresult :   integer result of the decoding
*              xresult :   real result of the decoding
*              istat   :   = 0    OK
*                          # 0    problem   cannot decode 
*                                 for example ascii string is not a number
*              *
* FILES USED      : NO
*
* FILES CREATED   : NO
*
* ROUTINES USED   :
*             *
*             *             *
*
*
*
       SUBROUTINE UK_DECOD ( STRING , IDEB , IFIN , IRESULT, 
     -                        XRESULT, ISTAT )
C
       CHARACTER*(*) STRING 
       INTEGER*4 IDEB,IFIN,IRESULT,ISTATUS , LFIN , I
       REAL   XRESULT 
      CHARACTER*200   CDUM
      CHARACTER BLANC , POINT , PLUS , MOINS 
      CHARACTER*20 FORM 
      DATA BLANC /' '/
      DATA POINT /'.'/
      DATA PLUS  /'+'/
      DATA MOINS /'-'/


             ISTAT = 0
           
c
c   look for the first non blank character comming from the right
C
          DO  10  I = IFIN , IDEB , - 1 
           IF ( STRING (I:I) .NE. BLANC )  THEN
                 LFIN = I
                 GOTO 15
           ENDIF
10         CONTINUE
c
c    we come here  if the string is blank
C
         IRESULT = 0 
         XRESULT = 0 
         GOTO 999
C
C
15       CONTINUE
c
c  parsing the ascii string to look for  decimal point , sign
c    and non digit characters
C
        IPOINT = LFIN + 1
C      
         DO 20  I = LFIN , IDEB , - 1
C
c             first look if we are on a digit character
C
         IF( STRING (I:I) .GT. '9' .OR. STRING (I:I) .LT. '0') THEN
c
c              we are not on a digit so we look if it is a decimal point
C
             IF ( STRING (I:I) .EQ. POINT ) THEN
                IPOINT = I
                                            ELSE
C
c      we are not on a digit or a dec point so we look if it a sign
C
              IF( .NOT. (STRING(I:I) .EQ. MOINS  .OR.
     -            STRING(I:I) .EQ. PLUS   .OR.
     -            STRING(I:I) .EQ. BLANC )  )  THEN
c
c    a character in the ascii string is not a digit or a sign 
c    we cannot decode
C
                ISTAT = 1 
                GOTO 999
             ENDIF
          ENDIF
       ENDIF
20     CONTINUE
c
c     create the format to decode the float number
C
          ILEN = LFIN - IDEB + 1
          IDEC = LFIN - IPOINT + 1
C
          WRITE ( UNIT = FORM , FMT = '(A,I2,A,I2,A,I2,A)')
     -       '(A',IDEB - 1    , ',F', ILEN , '.' , IDEC ,')'

C
C     INTERNAL READ TO DECODE THE NUMBER
C
           READ ( UNIT = STRING , FMT = FORM , ERR = 900 ) 
     -            CDUM , XRESULT

C
          IRESULT = XRESULT
C
          GO TO 999
C
900       CONTINUE
          ISTAT = 2
          IRESULT = 0
          XRESULT = 0                        
C
999       CONTINUE
C
          END
