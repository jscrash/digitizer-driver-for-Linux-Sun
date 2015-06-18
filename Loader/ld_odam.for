C  DEC/CMS REPLACEMENT HISTORY, Element LD_ODAM.FOR
C  *1     3-AUG-1989 15:05:39 ADEY "support routines for UKOOA loader"
C  DEC/CMS REPLACEMENT HISTORY, Element LD_ODAM.FOR
C
C ODAM.F77,  CGG>DOBEK>F77>SOURCE,  GEORGE DOBEK (CGG),  12/19/86
C  PART OF UKOOA BATCH LOADER
C Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA.
C
C TITLE : ODAM -  PART OF UKOOA BATCH LOADER
C
C START-HISTORY :
c 06/28/89 KARL     FIX FOR DEGREES MINUTES SECONDS LAT/LONG DATA
C 12/20/88 GORDON   Replaced hard-coded '5' with STDIN & added function call
C 12/19/86 MARK     FIXED BUG WITH LEFT JUSTIFICATION OF STRINGS SUBROUTINE CALL
C 11/24/86 CGG      INITIAL CODING
C 11/24/86 GEORGE DOBEK (CGG) Template constructed
C END-HISTORY
C
      PROGRAM LD_ODAM
C
      INTEGER*4     IOS, POS, INT1, INT2, INT3
      INTEGER*4     NB1, NB2, NB3, NB4, NUMLIN
      REAL*4        REAL1
      LOGICAL*4     ALL
      CHARACTER*80  INPUT, OUTPUT, WORD
      INTEGER*4     STDIN, HO_GET_FORTRAN_STDIN
C
      STDIN = HO_GET_FORTRAN_STDIN()
C
C ... OPEN OUTPUT FOR DIRECT ACCESS
    1 READ (UNIT=STDIN, FMT='(A80)', END=997) INPUT
      POS = 1
      CALL FFASC (INPUT, 80, POS, WORD, 3)
      IF (WORD(1:2) .EQ. '/*') GO TO 1
      IF (WORD(1:3) .EQ. '+++') GO TO 1
      POS = 1
      CALL FFASC (INPUT, 80, POS, OUTPUT, 80)
      OPEN (UNIT=99, FILE=OUTPUT, STATUS='NEW', ACCESS='DIRECT',
     +      FORM='FORMATTED', RECL=80, IOSTAT=IOS)
C
C ... SKIP ### and /* CARDS
    2 READ (UNIT=STDIN, FMT='(A80)', END=997) INPUT
      POS = 1
      CALL FFASC (INPUT, 80, POS, WORD, 3)
      IF (WORD(1:2) .EQ. '/*') GO TO 2
      IF (WORD(1:3) .EQ. '###') GO TO 10
C
C ... TRANSFER FIRST GROUP OF RECORDS
   10 CONTINUE
      I = 1
   11 READ (UNIT=STDIN, FMT='(A80)', END=998) INPUT
      POS = 1
      CALL FFASC (INPUT, 80, POS, WORD, 3)
c
c    R DORET  august 10/87  suppression of a test
c    causing an error in the creation of the output
c    file
c   
          POS = 1
          IF ( (I .EQ. 1) .OR. (I .EQ. 2) .OR.
     +         (I .EQ. 6) ) THEN
C ............ SQUEEZE OUT LEADING BLANKS
               LS = TFNBLEN( INPUT )
               CALL TFLEFTJS(INPUT, OUTPUT, LS)
C ............ BLANK OUT COMMENTS
               DO 12 K = 1, 79
                    IF (OUTPUT(K:K+1) .EQ. '/*')  THEN
                         OUTPUT(K:80) = ' '
                         GO TO 13
                    ENDIF
   12          CONTINUE
   13          WRITE (UNIT=99, FMT='(A20)', REC=I+1) OUTPUT
          ELSE IF ((I .EQ. 3) .OR. (I .EQ. 4) .OR.
     +             (I .EQ. 5) ) THEN
               CALL FFINT (INPUT, 80, POS, INT1)
               WRITE (UNIT=99, FMT='(I10)', REC=I+1) INT1
          ELSE
               CALL FFASC (INPUT, 80, POS, WORD, 3)
               IF (WORD(1:3) .EQ. '###') GO TO 20
               POS = 1
               CALL FFINT (INPUT, 80, POS, INT1)
               CALL FFINT (INPUT, 80, POS, INT2)
C   MOD FOR CHOICE PARAMETER FOR DDDMMSS.S LAT/LONG DATA
               CALL FFINT (INPUT, 80, POS, INT3)
               WRITE (UNIT=99, FMT='(I4,2X,I4,2X,I4)', REC=I+1) INT1,
     -		  INT2,INT3
          ENDIF
          I = I + 1
          GO TO 11
C
   20 CONTINUE
      NB1 = I
      WRITE (UNIT=99, FMT='(I10)', REC=1) NB1
C
C ... TRANSFER SECOND GROUP OF RECORDS
      I = 1
      NUMLIN = 0
   21 READ (UNIT=STDIN, FMT='(A80)', END=998) INPUT
          POS = 1
          CALL FFASC (INPUT, 80, POS, WORD, 3)
          IF (WORD(1:2) .EQ. '/*') GO TO 21
          POS = 1
          IF ( (I .EQ. 1) .OR. (I .EQ. 2) ) THEN
               CALL FFINT (INPUT, 80, POS, INT1)
               WRITE (UNIT=99, FMT='(I10)', REC=NB1+I+1) INT1
               IF ( (I .EQ. 1) .AND. (INT1 .EQ. 0) ) ALL = .TRUE.
          ELSE
               CALL FFASC (INPUT, 80, POS, WORD, 3)
               IF (WORD(1:3) .EQ. '###') GO TO 30
               IF (WORD(1:3) .EQ. '+++') GO TO 30
               IF (ALL) GO TO 21
               POS = 1
               CALL FFTITL (INPUT, 80, POS, OUTPUT, 20)
               CALL FFINT (INPUT, 80, POS, INT1)
               CALL FFINT (INPUT, 80, POS, INT2)
               CALL FFINT (INPUT, 80, POS, INT3)
               WRITE (UNIT=99, FMT=25, REC=NB1+I+2)
     +              OUTPUT, INT1, INT2, INT3
   25          FORMAT (A20,2X,I10,2X,I10,2X,I10)
               NUMLIN = NUMLIN + 1
          ENDIF
          I = I + 1
          GO TO 21
C
   30 CONTINUE
      NB2 = NUMLIN + 4
      WRITE (UNIT=99, FMT='(I10)', REC=NB1+1) NB2
      WRITE (UNIT=99, FMT='(I10)', REC=NB1+4) NUMLIN
      IF (WORD(1:3) .EQ. '+++') GO TO 999
C
C ... TRANSFER THIRD GROUP OF RECORDS
      I = 1
   31 READ (UNIT=STDIN, FMT='(A80)', END=998) INPUT
          POS = 1
          CALL FFASC (INPUT, 80, POS, WORD, 3)
          IF (WORD(1:2) .EQ. '/*') GO TO 31
          IF (WORD(1:3) .EQ. '###') GO TO 40
          IF (WORD(1:3) .EQ. '+++') GO TO 40
          POS = 1
          IF (I .EQ. 1) THEN
               CALL FFINT (INPUT, 80, POS, INT1)
               WRITE (UNIT=99, FMT='(I10)', REC=NB1+NB2+I+1) INT1
          ELSE
               CALL FFREAL (INPUT, 80, POS, REAL1)
               WRITE (UNIT=99, FMT='(15X,F10.6)', REC=NB1+NB2+I+1) REAL1
          ENDIF
          I = I + 1
          GO TO 31
C
   40 CONTINUE
      NB3 = I
      WRITE (UNIT=99, FMT='(I10)', REC=NB1+NB2+1) NB3
      IF (WORD(1:3) .EQ. '+++') GO TO 999
C
C ... TRANSFER FOURTH GROUP OF RECORDS
      I = 1
   41 READ (UNIT=STDIN, FMT='(A80)', END=998) INPUT
          POS = 1
          CALL FFASC (INPUT, 80, POS, WORD, 3)
          IF (WORD(1:2) .EQ. '/*') GO TO 41
          POS = 1
          CALL FFTITL (INPUT, 80, POS, WORD, 20)
          CALL FFTITL (INPUT, 80, POS, OUTPUT, 40)
          WRITE (UNIT=99, FMT='(A20,2X,A40)', REC=NB1+NB2+NB3+I+1)
     +         WORD, OUTPUT
          I = I + 1
          GO TO 41
C
   50 CONTINUE
      NB4 = I
      WRITE (UNIT=99, FMT='(I10)', REC=NB1+NB2+NB3+1) NB4
      IF (WORD(1:3) .EQ. '+++') GO TO 999
C
C ... CLOSE FILES
  997 CONTINUE
      PRINT *, 'ODAM: Missing File Name Data!'
      STOP
  998 CONTINUE
      PRINT *, 'ODAM: Premature End of File Encountered on Input!'
      STOP
  999 CONTINUE
      PRINT *, 'ODAM: Normal Completion'
      CLOSE (UNIT=99, IOSTAT=IOS)
C
      END
