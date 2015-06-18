C DEC/CMS REPLACEMENT HISTORY, Element NI_VERSATEC.F77
C *2    15-SEP-1989 11:35:34 VINCE "(SPR -1) moved from Enron"
C *1    15-SEP-1989 11:34:24 VINCE "a"
C DEC/CMS REPLACEMENT HISTORY, Element NI_VERSATEC.F77
C NI_NEW.F77,  FINDER>SOURCE,  VINCE CARDINALE,  03/06/89
C Interpret a NEUTRAL plot file and call PLOT, PLOTS and NEWPEN.
C Copyright (c) 1989, Exploration Systems Inc, San Francisco, CA.
C
C TITLE : NI - Interpret a NEUTRAL plot file and call PLOT, PLOTS and NEWPEN.
C
C %F NEUTRAL INTERPRETER
C Interpret a neutral file containing x,y,npen information.
C
      SUBROUTINE NINEW
C
C This can be called as a main program to process a file.
C
      CHARACTER*80 NPFILE
      INTEGER JUNIT
C
C Get filename.
      PRINT *,'--=< ESI Neutral Plot Processor (VERSATEC version) >=--'
      PRINT *,'Enter NEUTRAL Filename:'
      READ 1000,NPFILE
1000  FORMAT(A)

C Open File.
      CALL HOGFNT(JUNIT)
      OPEN(UNIT=JUNIT, FILE=NPFILE, ERR=8888, STATUS='OLD')

C Process file.
      CALL NI_PROCESS(JUNIT)
      CLOSE(JUNIT)
      GOTO 9999

C Error processing.
8888  CONTINUE
      PRINT *,' NEUTRAL -- INTERPRETER could not open plot file.'

9999  CONTINUE
      RETURN
      END

C -----------------------------------------------------------
      SUBROUTINE NI_PROCESS(JUNIT)
C
C %S Process file and call Plotter Library
C
      CHARACTER LINEIN*80, ZPFILE*80, COMPANY*60

      INTEGER*4 JUNIT
      INTEGER*4 I, VNPF_NAME(21), RETCODE, PENDEF(3)
      INTEGER*4 FPEN, TPEN, IPEN, ZUNIT, IDUMMY(2)
      INTEGER*4 V_COMPANY(16)
      REAL*4    DUMMY(2)
      REAL*4    XPOS, YPOS, XSIZE, YSIZE, PAPERSIZE(4)

      INTEGER*2 NLEN$A, XMIN, XMAX, YMIN, YMAX

      EQUIVALENCE (ZPFILE,  VNPF_NAME(2))
      EQUIVALENCE (COMPANY, V_COMPANY(2))

      PARAMETER (XMIN=1, XMAX=2, YMIN=3, YMAX=4)

C      when changing this string be sure to update the length.
C      maximum length is 60 characters.
       V_COMPANY(1) = 44
       COMPANY = 'ENRON Oil and Gas - Corpus Christi Division'

C      Process header.
       READ(JUNIT,1000, ERR=8000) IVERS
1000   FORMAT(I6)

C  Check Version.
      IF(IVERS .NE. 100) THEN
         PRINT *,'Bad version of NEUTRAL plot file = ',IVERS
         RETURN
      ENDIF

C Strip comments.
      READ(JUNIT,1010) LINEIN
      READ(JUNIT,1010) LINEIN
1010  FORMAT(A)

C Get size of plotter expected.
      READ(JUNIT,1020) XSIZE, YSIZE
1020  FORMAT (2F9.3)

C Get the plot file name
      PRINT *, 'Enter name for output file:'
      READ 1001, ZPFILE
 1001 FORMAT (A)
      VNPF_NAME(1) = NLEN$A (ZPFILE, INTS(80))

C Run thru the file and get the actual max paper size needed, ignore
C the size just read from the neutral file (its a fixed size).
      PAPERSIZE(XMIN) = 0.0
      PAPERSIZE(XMAX) = 0.0
      PAPERSIZE(YMIN) = 0.0
      PAPERSIZE(YMAX) = 0.0
5000  READ(JUNIT, 1030, END=6000) XPOS, YPOS, IPEN
      IF (IPEN.EQ.2 .OR. IPEN.EQ.3) THEN
         IF (XPOS.GT.PAPERSIZE(XMAX)) PAPERSIZE(XMAX) = XPOS
         IF (YPOS.GT.PAPERSIZE(YMAX)) PAPERSIZE(YMAX) = YPOS
      ENDIF
      GOTO 5000

C Rewind the file before starting the real vector processing
6000  REWIND (JUNIT)
C     read all the header stuff again
      READ (JUNIT, 1000, ERR=8000) IVERS
      READ (JUNIT, 1010) LINEIN
      READ (JUNIT, 1010) LINEIN
      READ (JUNIT, 1020) XSIZE, YSIZE

C     allow some space at the end of the plot.
      PAPERSIZE(XMAX) = PAPERSIZE(XMAX) + 2.0

C Send the setup commands to the Versatec library
C     set the output file name
      CALL VPOPT (22, VNPF_NAME, DUMMY, RETCODE)

C     turn on color plotting
      CALL VPOPT (101, 0, DUMMY, RETCODE)

C     set the pen colors to the same index as the pen number.
      DO 3 I=1,8
         PENDEF(1) = I - 1
         PENDEF(2) = I
         PENDEF(3) = 2
         CALL VPOPT (103, PENDEF, DUMMY, RETCODE)
    3 CONTINUE

C     turn on automatic paneling and set the paper size
      CALL VPOPT (33, -1, DUMMY, RETCODE)
      CALL VPOPT (2, IDUMMY, PAPERSIZE, RETCODE)

C     set the company name for the plot info line.
      CALL VPOPT (21, V_COMPANY, DUMMY, RETCODE)

C indicate that we are about to start sending vectors and
C set the initial pen color
      CALL PLOTS (0, 0, 0)
      TPEN = 0
      CALL NEWPEN (TPEN)

C Start processing vectors.
C ++++++++++++++++++++++++++++
10    CONTINUE
      READ(JUNIT, 1030, END=8010) XPOS, YPOS, IPEN
1030  FORMAT (2F9.3, I6)

C Control based on IPEN
C Select new pen.
      IF (IPEN .GE. 1000) THEN
         FPEN = IPEN - 1000

C Map pen from FPEN to TPEN
         CALL NI_MAPPEN(FPEN, TPEN)
         CALL NEWPEN(TPEN)

C Plot Termination
      ELSE IF (IPEN .EQ. 999) THEN
         GOTO 9999

C Normal plotting operation.
      ELSE
         CALL PLOT (XPOS, YPOS, IPEN)
      ENDIF

      GOTO 10
C ++++++++++++++++++++++++++++

C Premature end.
8000  CONTINUE
      PRINT *,'Could not read version # ! Is this a neutral file?'
      RETURN

8010  CONTINUE
      PRINT *,'NEUTRAL - Premature end of plot file!!!'

C Terminate PLOT
9999  CONTINUE

C Move pen past plot to a new home position.
      CALL PLOT(PAPERSIZE(XMAX), PAPERSIZE(YMIN), 3)

      CALL PLOT (0.0, 0.0, 999)

      RETURN
      END
C
C ---------------------------------------------------------------------
C
      SUBROUTINE NI_MAPPEN( PIN, POUT )
C
C Map the Finder pen colors to the Versatec color indexes.
C
C Note: This routine is to be readable not elegant...
C
      INTEGER*4 PIN, POUT
C
      INTEGER*4 NONE,         BLACK,       RED
      INTEGER*4 GREEN,        BLUE,        CYAN
      INTEGER*4 MAGENTA,      YELLOW,      RED_YELLOW
      INTEGER*4 GREEN_YELLOW, GREEN_CYAN,  BLUE_CYAN
      INTEGER*4 BLUE_MAGENTA, RED_MAGENTA, DARK_GREY
      INTEGER*4 LIGHT_GREY
C
      INTEGER*4 VBLACK, VCYAN, VMAGENTA, VYELLOW
      INTEGER*4 VBLUE,  VRED,  VGREEN,   VGRAY
C
C     Finder Pen Colors...
      PARAMETER ( NONE         = 00, BLACK       = 01, RED        = 02 )
      PARAMETER ( GREEN        = 03, BLUE        = 04, CYAN       = 05 )
      PARAMETER ( MAGENTA      = 06, YELLOW      = 07, RED_YELLOW = 08 )
      PARAMETER ( GREEN_YELLOW = 09, GREEN_CYAN  = 10, BLUE_CYAN  = 11 )
      PARAMETER ( BLUE_MAGENTA = 12, RED_MAGENTA = 13, DARK_GREY  = 14 )
      PARAMETER ( LIGHT_GREY   = 15 )
C
C     Versatec Color Indexes...
      PARAMETER ( VBLACK = 00, VCYAN = 01, VMAGENTA = 02, VYELLOW = 03 )
      PARAMETER ( VBLUE  = 04, VRED  = 05, VGREEN   = 06, VGRAY   = 07 )
C
C     Pen to index mapping...
      IF (PIN .EQ. NONE)         POUT = VBLACK
      IF (PIN .EQ. BLACK)        POUT = VBLACK
      IF (PIN .EQ. RED)          POUT = VRED
      IF (PIN .EQ. GREEN)        POUT = VGREEN
      IF (PIN .EQ. BLUE)         POUT = VBLUE
      IF (PIN .EQ. CYAN)         POUT = VCYAN
      IF (PIN .EQ. MAGENTA)      POUT = VMAGENTA
      IF (PIN .EQ. YELLOW)       POUT = VYELLOW
      IF (PIN .EQ. RED_YELLOW)   POUT = VRED
      IF (PIN .EQ. GREEN_YELLOW) POUT = VGREEN
      IF (PIN .EQ. GREEN_CYAN)   POUT = VGREEN
      IF (PIN .EQ. BLUE_CYAN)    POUT = VBLUE
      IF (PIN .EQ. BLUE_MAGENTA) POUT = VBLUE
      IF (PIN .EQ. RED_MAGENTA)  POUT = VRED
      IF (PIN .EQ. DARK_GREY)    POUT = VGRAY
      IF (PIN .EQ. LIGHT_GREY)   POUT = VGRAY
C
      RETURN
      END
C
C END-CODE
