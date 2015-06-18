C DEC/CMS REPLACEMENT HISTORY, Element NP_CALCOMP.FOR
C *3    28-NOV-1989 14:41:27 GORDON "(SPR 0) lowercase include files"
C *2    11-AUG-1989 17:05:20 VINCE "(SPR -1) last fixes from UNIX port."
C *1    10-AUG-1989 18:50:41 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NP_CALCOMP.FOR
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
      INTEGER     INLUN,                ! LUN for input file.  (FOR010)
     1            OUTLUN,               ! LUN for output file. (FOR009)
     2            STATUS,               ! Status of Driver operation
     3            IPEN                  ! Pen #/command code for PLOT()
      
      LOGICAL*1   REPORT/.TRUE./        ! TRUE=VERBOSE OUTPUT 
      LOGICAL*1   OVERFLOW/.FALSE./     ! TRUE IF PLOT TO BIG
      
      REAL        XPOS,YPOS,            ! Current vector coord.
     2            SCFAC            ! UOM Xform. (2.54 or 1.0) 
           
      ASSIGN   990  TO   EXIT_VEC       ! Set label to jump to so
      ASSIGN   100  TO   ERROR_VEC      ! the code is easier to read.
C 
C ------------------------------------------------------------------
C 
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
      OVERFLOW = .FALSE.
      CALL NP_REPORT(INLUN,REPORT,OVERFLOW,STATUS)
      
      IF (OVERFLOW) THEN
            IF (NPF_XMAX .GT. DEV_XMAX) THEN 
                  SCFAC = (DEV_XMAX / NPF_XMAX)
            ELSE
                  SCFAC = 1.0
            ENDIF
      ENDIF        
      
      IF (STATUS .NE. SUCCESS) THEN
            IF (.NOT. REPORT)  PRINT 50, NPF_FNAME (1:NPF_FNLEN)            
50          FORMAT(A, ' Failed ....')
            GOTO ERROR_VEC
C Call vector processing routine if all is well
C             
      ELSE
            REWIND(UNIT=INLUN)
            CALL NP_PROCESS(INLUN,OUTLUN,SCFAC,STATUS)
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
      SUBROUTINE  NP_PROCESS(INLUN,OUTLUN,SCFAC,STATUS)
C     
      INCLUDE      'npdefs.i'
      
      INTEGER     INLUN,             ! Input lun = FOR010
     1            OUTLUN             ! Output lun = FOR009
       
      REAL        SCFAC,            ! INPUT UOM VALUE
     1            XPOS,YPOS,
     2            XMAX_SOFAR         ! TO TRACK THE LARGEST X RECIEVED
      
      INTEGER     STATUS,             ! Status of this run
     1            IPEN,
     2            TPEN,
     3            FPEN
      
      ASSIGN   9990  TO   EXIT_VEC   ! Set label to jump to so
      ASSIGN   8000  TO   ERROR_VEC  ! code is easier to read.
      
C 
C --------------------------------------------------------------    
C 
C Assume at entry that the file for inlun has
C been opened, and is positioned at the first 
C record....
C 
      
C     
C Advance to the first of the vector lines
      CALL NP_RDHDR(INLUN,STATUS)
C
C Open the output file and init the plotter driver             
      OPEN(UNIT=OUTLUN,FORM='FORMATTED',STATUS='UNKNOWN')
C 
C INIT THE PLOTTER - CALCOMP STYLE CALL TO PLOTS 
      CALL PLOTS(0,0,OUTLUN)
      CALL FACTOR (SCFAC)
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
      PRINT *,'Could not read version # ! Is this a neutral file?'
      GOTO EXIT_VEC
C
8010  CONTINUE
      PRINT *,'NEUTRAL - Premature end of plot file!!!'
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
