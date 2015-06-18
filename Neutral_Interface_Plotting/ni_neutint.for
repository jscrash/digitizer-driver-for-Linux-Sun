C DEC/CMS REPLACEMENT HISTORY, Element NI_NEUTINT.FOR
C *1    10-AUG-1989 18:50:34 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NI_NEUTINT.FOR
C NI.NEUTRAL_INTERP.F77,  PHIL>FWORK,  PHIL BURK,  05/30/86
C Interpret a NEUTRAL plot file and call PLOT, PLOTS and NEWPEN.
C Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA.
C
C TITLE : NI - Interpret a NEUTRAL plot file and call PLOT, PLOTS and NEWPEN.
C
C START-HISTORY :
C 04/10/87 SY	    Added Calcomp 5800 support
C 05/30/86 GUY      Use PLOTTER_TERM and PLOTTER_INIT for zeta
C 05/29/86 GUY      Added NI_MAPPEN()
C 04/01/86 PHIL     GO LIB
C                   Created.
C 04/01/86 PHIL BURK Template constructed
C END-HISTORY
C
C START-DESCRIPTION :
C  Inquire from user the plotter type for output,
c  open and read a finder:neutral plot file, interpret
c  the commands with the plotters' driver and write
c  to a file
C END-DESCRIPTION
C
C %F NEUTRAL INTERPRETER
C Interpret a neutral file containing x,y,npen information.
C
C Author: Phil Burk
C
C %M MOD: PLB 4/8/86  Added return to home on termination.
C                 New device support for old HOUSTON driver.
C %M MOD: PLB 5/28/87 Fixed pen mappings for CALCOMP, OPEN
C                 and CLOSE output file, make NIINTERP a
C                 PROGRAM (not subroutine).
C ------------------------------------------------------
      PROGRAM NI_NEUTINT
C
C This can be called as a main program to process a file.
C
      CHARACTER NPFILE*80
      INTEGER JUNIT, DEVICE
C
C Get filename.
      PRINT *,' --=< NEUTRAL PLOT PROCESSOR - ESI >=--'
      PRINT *,'Enter NEUTRAL Filename:'
      READ 1000,NPFILE
1000  FORMAT(A)
C
C Open File.
      CALL HOGFNT(JUNIT)
      OPEN(UNIT = JUNIT, FILE=NPFILE, ERR=8888, STATUS = 'OLD')
C
C Select Device.
      PRINT *,'Select Device:'
      PRINT *,'(1 = Houston, 2 = ZETA, 3 = Old Houston,4 = Calcomp5800)'
      READ *,DEVICE
C
C Process file.
      CALL NI_PROCESS(JUNIT, DEVICE)
      CLOSE(JUNIT)
      GOTO 9999
C
C Error processing.
8888  CONTINUE
      PRINT *,' NEUTRAL -- INTERPRETER could not open plot file.'
C
9999  CONTINUE
      STOP
      END
C
C
C -----------------------------------------------------------
      SUBROUTINE NI_PROCESS(JUNIT, DEVICE)
C
C %S Process file and call Plotter Library
C
      CHARACTER LINEIN*80, ZPFILE*80
      INTEGER JUNIT, DEVICE
      INTEGER FPEN, TPEN, IPEN, ZUNIT
      REAL XPOS, YPOS, XMAX
C
C      Process header.
       READ(JUNIT,1000, ERR=8000) IVERS
1000   FORMAT(I6)
C
C  Check Version.
      IF(IVERS .NE. 100) THEN
         PRINT *,'Bad version of NEUTRAL plot file = ',IVERS
         RETURN
      ENDIF
C
C Strip comments.
      READ(JUNIT,1010) LINEIN
      READ(JUNIT,1010) LINEIN
1010  FORMAT(A)
C
C Get size of plotter expected.
      READ(JUNIT,1020) XSIZE, YSIZE
1020  FORMAT (2F9.3)
C
C %D Start plot process.
      XMAX = 0.0
      IF (DEVICE .EQ. 1) THEN
C Startup for HOUSTON Instruments.
          CALL HIINIT(IUNIT, XAVAIL, YAVAIL)
          IF ((XSIZE .GT. XAVAIL) .OR. (YSIZE .GT. YAVAIL)) THEN
             PRINT *,'NEUTRAL - Plot too big for plotter, need '
     1              ,XSIZE, YSIZE
             PRINT *,'But we only have ', XAVAIL, YAVAIL
          ENDIF
C
      ELSE IF (DEVICE .EQ. 2) THEN
C Startup for ZETA plotter.
          CALL HOGFNT(ZUNIT)
          CALL TFAPUSID('PLOT_FILE',ZPFILE)
C
C %M           OPEN(UNIT = ZUNIT, FILE = ZPFILE)
C %M           PRINT 2000,ZPFILE
C %M 2000      FORMAT ('Zeta plot in file = ',A)
C %M           CALL PLOTS (53,0,ZUNIT)
C
          CALL PLOTTER_INIT(ZPFILE, XAVAIL, YAVAIL, NPENS)
          IF ((XSIZE .GT. XAVAIL) .OR. (YSIZE .GT. YAVAIL)) THEN
             PRINT *,'NEUTRAL - Plot too big for plotter, need '
     1              ,XSIZE, YSIZE
             PRINT *,'But we only have ', XAVAIL, YAVAIL
          ENDIF
C
C Old HNG plot library for HI plotter.
      ELSE IF (DEVICE .EQ. 3) THEN
          CALL INITIAL
C Added Calcomp5800 support 4/11/87 [s.y.]
C 
      ELSE IF (DEVICE .EQ. 4) THEN
c On Vax, assume that FOR009 has been assigned to a plotfile
C c.f. ni_neutral.com for details.
C %M PLB Add OPEN -------------------------------------
C The device initialization and termination will probably
C need to be changed to integrate this into Finder.
C We may want to call PLOTTER_INIT like for the ZETA.
C I don't know if those routines exist for the CALCOMP yet.
          LDEV = 9
          OPEN(LDEV,FORM='FORMATTED',STATUS='UNKNOWN')
C
    	  CALL PLOTS(0,0,LDEV)
C
      ELSE
        PRINT *,'UNKNOWN DEVICE = ',DEVICE
        RETURN
      ENDIF
C
C Start processing vectors.
C ++++++++++++++++++++++++++++
10    CONTINUE
      READ(JUNIT, 1030, END=8010) XPOS, YPOS, IPEN
1030  FORMAT (2F9.3, I6)
C
C Control based on IPEN
C Select new pen.
      IF (IPEN .GE. 1000) THEN
         FPEN = IPEN - 1000
C
C Map pen from FPEN to TPEN for DEVICE
         CALL NI_MAPPEN(DEVICE, FPEN, TPEN)
C
         IF (DEVICE .EQ. 3) THEN
             CALL NEWPN(TPEN)
         ELSE
             CALL NEWPEN(TPEN)
         ENDIF
C
C Plot Termination
      ELSE IF (IPEN .EQ. 999) THEN
         GOTO 9999
C
C Normal plotting operation.
      ELSE
         CALL PLOT (XPOS, YPOS, IPEN)
         IF (XPOS .GT. XMAX) THEN
              XMAX = XPOS
         ENDIF
      ENDIF
C
      GOTO 10
C ++++++++++++++++++++++++++++
C
C Premature end.
8000  CONTINUE
      PRINT *,'Could not read version # ! Is this a neutral file?'
      RETURN
C
8010  CONTINUE
      PRINT *,'NEUTRAL - Premature end of plot file!!!'
C
C Terminate PLOT
9999  CONTINUE
C Move pen past plot to home position.
      CALL PLOT(XMAX + 2.0, 0.0, 3)
C
C %D HOUSTON
      IF (DEVICE .EQ. 1) THEN
         CALL RSTR(0)
C
C %D ZETA
       ELSE IF (DEVICE .EQ. 2) THEN
C
C %M          CALL PLOT(0.0, 0.0, 999)
C %M          CLOSE(ZUNIT)
C
         CALL PLOTTER_TERM
C
C %D %M Old HNG library.
      ELSE IF (DEVICE .EQ. 3) THEN
         CALL RSTR(0)
      ELSE IF (DEVICE .EQ. 4) THEN
C We may want to call PLOTTER_TERM like for the ZETA?!
      	 CALL PLOT(0.0, 0.0, 999)
      	 CLOSE(LDEV)
      ENDIF
C
      RETURN
      END
C
C ---------------------------------------------------------------------
C
      SUBROUTINE NI_MAPPEN( DEV, PIN, POUT )
C
C Map 'all pens' to 'pens supported by device' (eg. '0-15' to '1-4')
C
C Note: This routine is to be readable not elegant...
C
      INTEGER DEV, PIN, POUT
C
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
      IF ((DEV .EQ. 1) .OR. (DEV .EQ. 3)) THEN
C Houston 4 pen plotters
         POUT = BLACK
         IF (PIN .EQ. NONE)         POUT = BLACK
         IF (PIN .EQ. BLACK)        POUT = BLACK
         IF (PIN .EQ. RED)          POUT = RED
         IF (PIN .EQ. GREEN)        POUT = GREEN
         IF (PIN .EQ. BLUE)         POUT = BLUE
         IF (PIN .EQ. CYAN)         POUT = BLUE
         IF (PIN .EQ. MAGENTA)      POUT = RED
         IF (PIN .EQ. YELLOW)       POUT = GREEN
         IF (PIN .EQ. RED_YELLOW)   POUT = RED
         IF (PIN .EQ. GREEN_YELLOW) POUT = GREEN
         IF (PIN .EQ. GREEN_CYAN)   POUT = GREEN
         IF (PIN .EQ. BLUE_CYAN)    POUT = BLUE
         IF (PIN .EQ. BLUE_MAGENTA) POUT = BLUE
         IF (PIN .EQ. RED_MAGENTA)  POUT = RED
         IF (PIN .EQ. DARK_GREY)    POUT = BLACK
         IF (PIN .EQ. LIGHT_GREY)   POUT = BLACK
      ELSE IF (DEV .EQ. 2) THEN
C Zeta 8 pen plotters
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
C ADDED CALCOMP PEN ASSIGNMENTS USING 5800 PLOTTER DEFAULTS
      ELSE IF (DEV .EQ. 4) THEN
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
      ELSE
         POUT = PIN
      ENDIF
C
      RETURN
      END
C
C END-CODE
