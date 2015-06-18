C DEC/CMS REPLACEMENT HISTORY, Element NP_TOOLS.FOR
C *3    28-NOV-1989 14:42:30 GORDON "(SPR 0) lowercase include files"
C *2    11-AUG-1989 17:05:54 VINCE "(SPR -1) last fixes from UNIX port."
C *1    10-AUG-1989 18:51:12 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NP_TOOLS.FOR
C------------------------------------------------------------------
C                 - DESCRIPTION -
C     
C Read a neutral plot file and determines its absolute
C maximum drawing extents.  Fill out the common_block vars
C that describe the current plot file & the context of its
C instanciation:
C     
C         VARIABLE
C #         NAME                       DESCRIPTION   
C --------------------------------------------------------------    
C 1      IFNAMED_OUT             - BOOL: TRUE IF OUTLUN HAS FILENAME ASSOC. (OPEN)    
C 2      IFNAMED_IN              - BOOL: DITTO EXCEPT FOR INLUN
C 3      IFOPEN_OUT              - BOOL: TRUE IF OUTLUN IS CURR. OPEN
C 4      IFOPEN_IN               - BOOL: SAME EXCEPT FOR INLUN
C 5      NPVERS                  - VERSION # OF NEUT FILE. MUST BE 100
C 6      NPF_FNAME               - NAME OF FILE OPENED WITH INLUN (FOR010)
C 7      NPF_FNLEN               - LENGTH IN BYTES (CHAR) OF THE NPF_FNAME                    
C 8      NPF_XMIN,NPF_YMIN,      - EXTENTS OF NPF
C 9      NPF_XMAX,NPF_YMAX       - EXTENTS OF NPF
C 10     DEV_XMAX,DEV_YMAX       - X MAX (UNITS),Y SIZE OF PLOTTER DEVICE
C 11     NPSTATUS                - STATUS OF FILE (LAST CHECKED)
C 12     UOM_FACTOR              - EITHER 1.00 FOR ENGLISH OR 2.54 FOR METRIC 
C 13     DEV_NPENS               - NUMBER OF PENS AVAIL ON PLOTTER.
C 
C-----------------------------------------------------------------------  
C               
C     
      SUBROUTINE  NP_GETINFO (INLUN,STATUS)
C
C      
C-----------------------------------------------------------------------  
C                    -  USAGE -      
C     
C Pass by ref. the inlun for input (INPUT = 10, OUTPUT = 9)
C and get back the size limits of the device - xsize,ysize
C as well as the actual size limits of the plot nxmax,NPF_YMax
C and also the filename associated with the inlun, & then length
C of the filename in characters.  
C
C
C                  - STATUS RETURN -
C     
C Status will be 0 if all was well.....otherwise:
C      1 = FILE_ERROR        I.E. COULD'NT OPEN, OR BAD INLUN
C      2 = READ_ERROR
C      3 = BAD_VERSION       I.E. NOT EQUAL TO 100
C      4 = CORRUPT_FILE      I.E. NP FORMAT INCORRECT OR NOT NPFILE!
C
C To display a status message, call NP_STATMSG() with STATUS 
C immediatly after the call that generated the status.
C     
C                     - NOTES -
C     
C If the inlun has not been opened, it is opened locally and 
C then closed again before returning.  
C If the inlun is alreayc opened, (normal usage) then the file 
C is just re-wound to the beginning before use and also
C after use/before returning.
C
C     
C  BY Julian Carlisle    ON  April 27, 1988
C-------------------------------------------------------------
C     
      INCLUDE      'npdefs.i'
      
      REAL        XPOS,YPOS         ! X,Y LOC READ FROM NPFILE
      INTEGER     STATUS,           ! 1=SUCCESS,0=FAIL,2,3
     1            IPEN,             ! PEN CODE READ FROM NPFILE
     1            INLUN             ! LOG. UNIT OF INPUT FILE

      ASSIGN 990 TO EXIT_VEC        ! PSEUDO LABEL FOR EASY READING
C     
C-------------------------------------------------------------
C
      NPF_XMIN = 0.0
      NPF_XMAX = 0.0
      NPF_YMIN = 0.0
      NPF_YMAX = 0.0      
      NPLEN = 0
C
C Assign the npfile name to logical  for010   before calling
C file is opened/closed locally if it arrives unopened                   
C       
      INQUIRE(UNIT=INLUN, OPENED=IFOPEN_IN)
      IF (.NOT. IFOPEN_IN ) THEN
            OPEN(UNIT = INLUN, ERR = 910, STATUS = 'OLD')
      ENDIF      
C     
C     
C Read the version #, xsize,ysize etc from npfile into common
C     
      CALL NP_RDHDR(INLUN,STATUS)
      IF (STATUS .NE. SUCCESS) GO TO EXIT_VEC
C
C     
C Get the name of the file assoc. with inlun and return it.
C
      INQUIRE(UNIT=INLUN, NAME=NPF_FNAME,NAMED=IFNAMED_IN)
      IF (IFNAMED_IN) THEN
            NPF_FNLEN = INDEX(NPF_FNAME, ' ') -1
      ELSE
            NPF_FNLEN = 0
      ENDIF
C            
C            
C Main loop starts here....            
C            
100   CONTINUE
      READ (INLUN,FMT='(2F9.3, I6)',ERR=950,END=900) XPOS,YPOS,IPEN
C 
C     
C Bump the line counter ahead
C     
      NPLEN = NPLEN + 1
      IF (IPEN .EQ. 999) GO TO EXIT_VEC
C     
      IF (XPOS .GT. NPF_XMAX) NPF_XMAX = XPOS
      IF (YPOS .GT. NPF_YMAX) NPF_YMAX = YPOS
      GO TO 100
C     
C            
C---ERROR VECTORS ----
C
900   CONTINUE
      STATUS = CORRUPT_FILE
      GO TO EXIT_VEC
                   
910   CONTINUE
      IF (NPLEN .EQ. 0) THEN
            STATUS = FILE_ERROR
      ELSE
            STATUS = READ_ERROR
      ENDIF
      GO TO EXIT_VEC
            
920   CONTINUE
      STATUS = BAD_VERSION
      GO TO EXIT_VEC

950   CONTINUE
      STATUS = READ_ERROR
      GO TO EXIT_VEC
                   
C            
C----- EXIT VECTOR -----
C            
990   CONTINUE
      IF (.NOT. IFOPEN_IN) THEN
            CLOSE(UNIT=INLUN)
      ELSE
            REWIND(UNIT=INLUN)
      ENDIF
      
      RETURN
      END
C
C------------------------------------------------------------------
C      
C Read the version, xsize,ysize from np file header      
C and leave the file positioned at the first of the
C vector lines.
C      
      SUBROUTINE NP_RDHDR(INLUN,STATUS)
C     
C------------------------------------------------------------------
      INCLUDE      'npdefs.i'
C     
      INTEGER     INLUN,             ! IN  - INPUT LOGICAL UNIT #      
     1            STATUS             ! OUT - STATUS OF QUERY
C     
      ASSIGN      990 TO EXIT_VEC
      
C Julian Carlisle   April  27th,   1988
C------------------------------------------------------------------
C
      STATUS = SUCCESS      
      INQUIRE(UNIT=INLUN, OPENED=IFOPEN_IN)
      IF (IFOPEN_IN) THEN
            REWIND(UNIT=INLUN)
            READ (INLUN,FMT='(I6)',ERR = 910) NPVERS
            IF (NPVERS .NE. 100) GO TO 920
C            
C Next two lines are for comments so skip
C
            READ (INLUN,FMT='(A)',ERR = 950) LINEIN
            READ (INLUN,FMT='(A)',ERR = 950) LINEIN
C 
C Read the size limits for this plot from the file
C
            READ (INLUN,FMT='(2F9.3)',ERR=950) DEV_XMAX,DEV_YMAX
            STATUS = SUCCESS
      ELSE
            STATUS = FILE_ERROR
      ENDIF
C
C All done, now exit
C
      GOTO      EXIT_VEC
C            
C---ERROR & EXIT VECTORS ----
C
900   CONTINUE
      STATUS = CORRUPT_FILE
      GO TO EXIT_VEC
C                   
910   CONTINUE
      STATUS = FILE_ERROR
      GO TO EXIT_VEC
C            
920   CONTINUE
      STATUS = BAD_VERSION
      GO TO EXIT_VEC
C
950   CONTINUE
      STATUS = READ_ERROR
      GO TO EXIT_VEC
C            
990   CONTINUE
      IF (.NOT. IFOPEN_IN) THEN
            STATUS = FILE_ERROR
      ENDIF
      
      RETURN
      END
C
C 
C--------------------------------------------------------------
C                    
C Display on SYS$OUTPUT the results of NP_GETINFO for the
C given file.   This routine is a  diagnostic tool for
C analyzing neutral plot files.  Traps Overflow, bad format etc.           
C 
C For interactive output i.e. Terminal, set OUTLUN to 1.
C A OUTLUN of 0 results in the output being placed in a File.
C                
C--------------------------------------------------------------
      SUBROUTINE  NP_REPORT(INLUN,REPORT,OVERFLOW,STATUS)
C
      INCLUDE      'npdefs.i'
C
      LOGICAL*1    REPORT
      LOGICAL*1    OVERFLOW
      INTEGER      STATUS,INLUN
C     
      ASSIGN 990 TO EXIT_VEC
      ASSIGN 100 TO ERROR_VEC
C          
C Julian Carlisle   April  29, 1988
C--------------------------------------------------------------
C
      STATUS = SUCCESS     
      INQUIRE(UNIT=INLUN,OPENED=IFOPEN_IN)      
      IF (.NOT. IFOPEN_IN) THEN 
            OPEN(UNIT=INLUN,ERR=130,STATUS='OLD')
            CALL NP_GETINFO(INLUN,STATUS)
            CLOSE(INLUN)
      ELSE
            CALL NP_GETINFO(INLUN,STATUS)
      ENDIF
      IF (REPORT) THEN
            IF (NPF_FNLEN .GT. 0) THEN
                  PRINT 30,NPF_FNAME (1:NPF_FNLEN)
            ENDIF
            IF (STATUS .EQ. SUCCESS) THEN
                  IF (NPLEN .GT. 0) THEN
                        PRINT *,NPLEN,' Vectors Processed'
                        PRINT 40,'    Plotter Size: ',
     1                        DEV_XMAX,',',DEV_YMAX
                        PRINT 40,'    Max File X/Y: ',
     2                        NPF_XMAX ,',',NPF_YMAX
                        IF ((NPF_XMAX .GT. DEV_XMAX).OR. 
     1                        (NPF_YMAX .GT. DEV_YMAX)) THEN
                              PRINT *,'    **  X/Y Overflow!  ** '
                              OVERFLOW = .TRUE.
                        ELSE
                              PRINT *,'    ** Good X/Y Range. **'
                              OVERFLOW = .FALSE.
                        ENDIF
                        PRINT *,' '
                  ELSE
                        PRINT *, '   ** Empty Neut File **'
                        GOTO 130
                  ENDIF
            ELSE 
                  PRINT *, ' An error was found near line ',NPLEN 
                  IF (STATUS .EQ. SUCCESS) THEN 
                        GO TO EXIT_VEC
                  ELSE
                        GOTO ERROR_VEC
                  ENDIF
            ENDIF
      ELSE
            PRINT *,' No filename associated with input unit.'
            STATUS = FILE_ERROR
      ENDIF
C 
C         
      IF (STATUS .EQ. SUCCESS) THEN
            GO TO EXIT_VEC
      ELSE 
            GOTO ERROR_VEC
      ENDIF
      
30    FORMAT(' --> ',A,' <--')
40    FORMAT(A,F9.3,A,F9.3)
C      
C ---- ERROR VECTOR -----    
C
C DECODE & DISPLAY ERROR MESSAGE ASSOC. W/STATUS
C
100   IF (STATUS .EQ. FILE_ERROR) 
     1      PRINT *, 'Unexpected end of file reached....'
      GO TO EXIT_VEC
          
110   IF (STATUS .EQ. CORRUPT_FILE) 
     1      PRINT *,'Bad file type or format...'
      GO TO EXIT_VEC
          
120   IF (STATUS .EQ. BAD_VERSION) 
     1      PRINT *, 'Bad Version number...'
      GO TO EXIT_VEC
          
130   IF (STATUS .EQ. READ_ERROR) 
     1      PRINT *,'Error reading file '
      GO TO EXIT_VEC
C     
C---- EXIT VECTOR ---- 
C 
990   RETURN
      END
        
C
C ---------------------------------------------------------------------
C
      SUBROUTINE NP_MAPPEN(PIN, POUT )
      INCLUDE 'npdefs.i'
C
C Map 'all pens' to 'pens supported by device' (eg. '0-15')
C
      INTEGER  PIN, POUT
      INTEGER NONE, BLACK, RED
      INTEGER GREEN, BLUE, CYAN
      INTEGER MAGENTA, YELLOW, RED_YELLOW
      INTEGER GREEN_YELLOW, GREEN_CYAN, BLUE_CYAN
      INTEGER BLUE_MAGENTA, RED_MAGENTA, DARK_GREY
      INTEGER LIGHT_GREY
C
      PARAMETER ( NONE = 00, BLACK = 01, RED = 02 )
      PARAMETER ( GREEN = 03, BLUE = 04, CYAN = 05 )
      PARAMETER ( MAGENTA = 06, YELLOW = 07, RED_YELLOW = 08 )
      PARAMETER ( GREEN_YELLOW = 09, GREEN_CYAN = 10, BLUE_CYAN = 11 )
      PARAMETER ( BLUE_MAGENTA = 12, RED_MAGENTA = 13, DARK_GREY = 14 )
      PARAMETER ( LIGHT_GREY = 15 )
C
      IF (DEVSEL .NE. CALCOMP_ID) THEN
C         
C Zeta 8 pen plotters
C     
         POUT = BLACK
         IF (PIN .EQ. NONE)         POUT = BLACK
         IF (PIN .EQ. BLACK)        POUT = BLACK
         IF (PIN .EQ. RED)          POUT = RED
         IF (PIN .EQ. GREEN)        POUT = GREEN
         IF (PIN .EQ. BLUE)         POUT = BLUE
         IF (PIN .EQ. CYAN)         POUT = CYAN
         IF (PIN .EQ. MAGENTA)      POUT = MAGENTA
         IF (PIN .EQ. YELLOW)       POUT = YELLOW
         IF (PIN .EQ. RED_YELLOW)   POUT = RED_YELLOW
         IF (PIN .EQ. GREEN_YELLOW) POUT = GREEN
         IF (PIN .EQ. GREEN_CYAN)   POUT = GREEN
         IF (PIN .EQ. BLUE_CYAN)    POUT = BLUE
         IF (PIN .EQ. BLUE_MAGENTA) POUT = BLUE
         IF (PIN .EQ. RED_MAGENTA)  POUT = RED
         IF (PIN .EQ. DARK_GREY)    POUT = BLACK
         IF (PIN .EQ. LIGHT_GREY)   POUT = BLACK
C 
C         
C ADDED CALCOMP PEN ASSIGNMENTS USING 5800 PLOTTER DEFAULTS
C     
      ELSE 
C     
         POUT = BLACK
         IF (PIN .EQ. NONE)         POUT = 2
         IF (PIN .EQ. BLACK)        POUT = 2
         IF (PIN .EQ. RED)          POUT = 226
         IF (PIN .EQ. GREEN)        POUT = 338
         IF (PIN .EQ. BLUE)         POUT = 268
         IF (PIN .EQ. CYAN)         POUT = 289
         IF (PIN .EQ. MAGENTA)      POUT = 233
         IF (PIN .EQ. YELLOW)       POUT = 206
         IF (PIN .EQ. RED_YELLOW)   POUT = 214
         IF (PIN .EQ. GREEN_YELLOW) POUT = 334
         IF (PIN .EQ. GREEN_CYAN)   POUT = 307
         IF (PIN .EQ. BLUE_CYAN)    POUT = 247
         IF (PIN .EQ. BLUE_MAGENTA) POUT = 282
         IF (PIN .EQ. RED_MAGENTA)  POUT = 198
         IF (PIN .EQ. DARK_GREY)    POUT = 101
         IF (PIN .EQ. LIGHT_GREY)   POUT = 88
      ENDIF
      RETURN
      END
