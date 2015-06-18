C DEC/CMS REPLACEMENT HISTORY, Element NP_ZETA.FOR
C *3    28-NOV-1989 14:42:52 GORDON "(SPR 0) lowercase include files"
C *2    11-AUG-1989 17:06:09 VINCE "(SPR -1) last fixes from UNIX port."
C *1    10-AUG-1989 18:51:20 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NP_ZETA.FOR
C --------------------------------------------------------------
C                   - DESCRIPTION -
C
C Generalized neutral-to-device format processor.  This program
C will implement a Geoquest/Disco interface through standard
C calls i.e. PLOTS(0,0,LUN) will actually:
C
C       * Select Metric/English UOM for plot.
C       * Establish MIN/MAX xy extents for the plot.
C       * call PLT_IMAGE with the information & return
C         a unique Plot Job ID.
C
C The goal is to transparently-to-Finder glue our current method
C of plotting to whatever technology we come desire.
C
C   Julian Carlisle    28, April  1988
C --------------------------------------------------------------
      PROGRAM      NP_GENPLOT
      INCLUDE      'npdefs.i'
C
      INTEGER     INLUN,
C   LUN for input file.  (FOR010)
     1            OUTLUN,
C   LUN for output file. (FOR009)
     2            STATUS,
C   Status of Driver operation
     3            IPEN
C   Pen #/command code for PLOT()

      LOGICAL*1   REPORT/.TRUE./
C   TRUE=VERBOSE OUTPUT
      LOGICAL*1   OVERFLOW/.FALSE./
C   TRUE IF PLOT TO BIG

      REAL        XPOS,YPOS,
C   Current vector coord.
     2            UOM_FACTOR
C   UOM Xform. (2.54 or 1.0)

      ASSIGN   990  TO   EXIT_VEC
C   Set label to jump to so
      ASSIGN   100  TO   ERROR_VEC
C   the code is easier to read.
C
C ------------------------------------------------------------------
C
C
C     MODIFICATION BY JRB 7/9/88 TO SET SELECTED PLOTTER TYPE TO "ZETA"
C     THIS USES THE DEVSEL VARIABLE AND DEVICE CODE FROM NPDEFS.I
C
      DEVSEL = ZETA_ID

      INLUN = INPUT_UNIT
      OUTLUN = OUTPUT_UNIT
      STATUS = SUCCESS
C
C
      OPEN (UNIT=INLUN, ERR=130, STATUS='OLD')
C
C Get the filename, size of plotter, extents of npfile
C and at the same time validate it, checking format etc.
C
C
C Set Units of Measure factor to inches (DEF_UOM) and
C Validate the neutral file.
C
      UOM_FACTOR = DEF_UOM
      CALL NP_REPORT(INLUN,REPORT,OVERFLOW,STATUS)
      IF (STATUS .NE. SUCCESS) THEN
	    IF (.NOT. REPORT)  PRINT 50, NPF_FNAME (1:NPF_FNLEN)
50          FORMAT(A, ' Failed ....')
	    GOTO ERROR_VEC
      ELSE
C
C Call vector processing routine if all is well
C
	    REWIND(UNIT=INLUN)
	    CALL NP_PROCESS(INLUN,OUTLUN,UOM_FACTOR,STATUS)
	    CLOSE(INLUN)
       ENDIF
       GOTO EXIT_VEC
C
C ------DECODE & DISPLAY ERROR MESSAGE ASSOC. W/STATUS------
C
100   IF (STATUS .EQ. FILE_ERROR)
     1      PRINT *, 'Unexpected end of file reached....'
      GO TO EXIT_VEC
C
C
110   IF (STATUS .EQ. CORRUPT_FILE)
     1      PRINT *,'Bad file type or format...'
      GO TO EXIT_VEC
C
C
120   IF (STATUS .EQ. BAD_VERSION)
     1      PRINT *, 'Bad Version number...'
      GO TO EXIT_VEC
C
C
130   IF (STATUS .EQ. READ_ERROR)
     1      PRINT *,'Error reading file '
      GO TO EXIT_VEC
C
C---- EXIT VECTOR ----
C
990   CONTINUE
      CALL EXIT(STATUS)
      END
C
C
C------------------------------------------------------------------
C                   - DESCRIPTION -
C PROCESS THE INPUT FILE WITH ZETA CALLS TO GENERATE THE OUTPUT
C ZETA-SPECIFIC FILE.
C
C   Julian Carlisle    28, April  1988
C------------------------------------------------------------------
C
      SUBROUTINE  NP_PROCESS(INLUN,OUTLUN,UOM_FACTOR,STATUS)
C
      INCLUDE      'npdefs.i'

      INTEGER     INLUN,
C   Input lun = FOR010
     1            OUTLUN
C   Output lun = FOR009

      REAL        UOM_FACTOR,
C   INPUT UOM VALUE
     1            XPOS,YPOS,
     2            XMAX_SOFAR
C   TO TRACK THE LARGEST X RECIEVED

      INTEGER     STATUS,
C   Status of this run
     1            IPEN,
     2            TPEN,
     3            FPEN

      ASSIGN   9990  TO   EXIT_VEC
C   Set label to jump to so
      ASSIGN   8000  TO   ERROR_VEC
C   code is easier to read.

C
C --------------------------------------------------------------
C
C Assume at entry that the file for inlun has
C been opened, and is positioned at the first
C record....
C
      XMAX_SOFAR = 0.0
      IF (UOM_FACTOR .EQ. 0.0) THEN
	    UOM_FACTOR = UOM_ENGLISH
C   DEFAULT TO ENGLISH UNITS
      ENDIF

C
C Advance to the first of the vector lines
      CALL NP_RDHDR(INLUN,STATUS)
C
C Open the output file and init the plotter driver
      OPEN(UNIT=OUTLUN,FORM='FORMATTED',STATUS='UNKNOWN')
C
C THIS NEXT LINE IS THE ZETA SPECIFIC INIT. 53 IS THE MAGIC #
      CALL PLOTS(53,0,OUTLUN)
C
C
1000  CONTINUE
      READ(INLUN,FMT='(2F9.3, I6)', END=8010) XPOS, YPOS, IPEN
C
C Control is directed by the current pen value
C

      IF (IPEN .GE. 1000) THEN
	    FPEN = IPEN - 1000
C
C Map pen from FPEN to TPEN
C
	    CALL NP_MAPPEN(FPEN, TPEN)
	    CALL NEWPEN(TPEN)
C
C
      ELSE IF (IPEN .EQ. 999) THEN
	    GOTO  9000
C
C Nothing special, just log next vector
C
      ELSE
C
C Simply lock wandering coords to the border limit
C
	    IF (XPOS .GT. XMAX_SOFAR) THEN
		  XMAX_SOFAR = XPOS
	    ENDIF
	    CALL PLOT(XPOS,YPOS,IPEN)
      ENDIF

      GOTO 1000
C
C ---------------------------------------------------------------
C
C Premature end.
C
8000  CONTINUE
      PRINT *,'Could not read version # . Is this a neutral file?'
      GOTO EXIT_VEC
C
8010  CONTINUE
      PRINT *,'NEUTRAL - Premature end of plot file.'
C
C Terminate PLOT, but first Move pen past plot to home position.
C
9000  CONTINUE

      CALL PLOT(XMAX_SOFAR + 2.0, 0.0, 3)
      CALL PLOT(0.0, 0.0, 999)
C
9990  CONTINUE
      CLOSE(OUTLUN)
C
      RETURN
      END
C
C
                                                
