C  DEC/CMS REPLACEMENT HISTORY, Element TF_STRTOOL.FOR
C  *2    19-SEP-1989 10:19:53 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:54:17 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TF_STRTOOL.FOR
C
C ****************************************************************************
C   FUNCTION:   ISTRLEN
C
C   PURPOSE:    RETURN THE LENGTH OF THE NON - BLANK PORTION OF A STRING
C
C ****************************************************************************

      INTEGER FUNCTION ISTRLEN(STRING)
      CHARACTER*(*) STRING
      DO 100 ISTRLEN = LEN(STRING), 1, -1
100   IF(  STRING(ISTRLEN:ISTRLEN).NE.' ')RETURN
      ISTRLEN = 0
      RETURN
      END
C
C ****************************************************************************
C
      SUBROUTINE CHARDEC(CHAR, DEC)
      LOGICAL*1 CHAR, G(4)
      INTEGER*4 DEC,GG
      EQUIVALENCE(GG,G)
      GG=0
      IF(DEC .LT. -127 .OR. DEC .GT. 127)THEN
            CHAR = G(4)
            RETURN
      ENDIF
      GG= DEC
      IF(DEC.LT. 0)GG=256 -IABS(DEC)
      CHAR=G(4)
      RETURN
      END 
C
C ****************************************************************************
C
      SUBROUTINE ISTRJUS(STRING)
C *********************************************************************
C   RIGHT JUSTIFY A STRING, BLANK PADDING THE FRONT
C *********************************************************************
      CHARACTER*(*)  STRING

      ILENGTH = LEN(STRING)
      JLENGTH = ISTRLEN(STRING)
      IF (ILENGTH.EQ.JLENGTH) RETURN

      STRING(ILENGTH - JLENGTH + 1: ILENGTH) = STRING(1: JLENGTH) 

      DO 100 I = 1, JLENGTH
100   STRING(I: I) = ' '

      RETURN
      END
C
C ****************************************************************************
C
      SUBROUTINE STRLJ(STRING)
      CHARACTER*(*)STRING
      L=LEN(STRING)
      IF(L.EQ.0)RETURN
      DO 100 JK = 1 , L
100   IF(STRING(JK:JK).NE.' ')GOTO 200
      RETURN
200   IF(JK.EQ.1)RETURN
      ID=JK-1
      DO 300 IK = JK,L
300   STRING(IK-ID:IK-ID)=STRING(IK:IK)
      DO 400 JK = L-ID+1,L
400   STRING(JK:JK)=' '
      RETURN
      END
C
C ****************************************************************************
C
      SUBROUTINE STRN2B(STRING)
      CHARACTER*(*) STRING,TEMP*255,TEMP2*255
      TEMP=STRING
100   TEMP2=TEMP
      TEMP=' '
      L=ISTRLEN(TEMP2)
      I2=0
      IC=0
      DO 200 JK = 1 , L-1
      IF( TEMP2(JK:JK).NE.' '.OR. TEMP2(JK+1:JK+1).NE.' ')THEN
          I2=I2+1
          TEMP(I2:I2)=TEMP2(JK:JK)
      ELSE
          IC=IC+1
      ENDIF
200   CONTINUE
      I2=I2+1
      TEMP(I2:I2)=TEMP2(L:L)
      IF(IC.NE.0)GOTO 100
      STRING=TEMP
      RETURN
      END
C
C ****************************************************************************
C
      INTEGER FUNCTION TFNBLEN(STRIN0)
C
C %S  Return length of non blank portion of string.
C
C %A  Author: Phil Burk, 9/11/85
C
C %C Copyright ESI 9/11/85
C %P
C %P  IN:
C %P    STRING    = character string
C %P
      CHARACTER STRIN0*(*)
C
C ...Scan backwards.
      L = LEN (STRIN0) 
      N = L
      DO 100 J = 1,L
         IF (STRIN0(N:N) .NE. ' ') GOTO 200
         N = N-1
100   CONTINUE
C
200   CONTINUE
      TFNBLEN = N
C
      RETURN
      END
C ---------------------------------------------------------------

      SUBROUTINE TFSTRCAT (STRIN1, STRIN2, STROUT)
C
C %S  Concatenate two character strings.
C
C %A  Author: Phil Burk, 9/11/85
C
C %P  IN:
C %P    STRIN1  = Input character string.
C %P    STRIN2  = Input character string.
C %P  OUT:
C %P    STROUT = Concatenated string.
C %P
C %C Copyright ESI 9/11/85
C
      CHARACTER STRIN1*(*), STRIN2*(*), STROUT*(*)
      INTEGER TFNBLEN
      INTEGER LOUT, LNB1, LNB2, MAXL2
C
C ...Determine length of strings.
      LOUT = LEN (STROUT)
C
C ...Determine nonblank lengths.
      LNB1 = TFNBLEN(STRIN1)
      LNB2 = TFNBLEN(STRIN2)
C
C ...Determine how much of second string will fit.
      MAXL2 = LOUT - LNB1
      IF (LNB2 .GT. MAXL2) LNB2 = MAXL2

C ...Perform concatenation.
      STROUT = STRIN1(1:LNB1) // STRIN2(1:LNB2)
      if (LNB1+LNB2 .LT. LOUT) strout(LNB1+LNB2+1:) = ' '
C
      RETURN
      END
