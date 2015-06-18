C DEC/CMS REPLACEMENT HISTORY, Element NP_NEUTPLT.FOR
C *1    10-AUG-1989 18:51:02 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NP_NEUTPLT.FOR
C NP.NEUTRAL_PLOTLIB.F77,  PHIL>FWORK,  PHIL BURK,  04/02/86
C Provide plotter independant PLOT calls to file.
C Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA.
C
C TITLE : NP - Provide plotter independant PLOT calls to file.
C
C START-HISTORY :
C 04/02/86 PHIL     Read output filename from user.
C 04/01/86 PHIL     Created.
C 04/01/86 PHIL BURK Template constructed
C END-HISTORY
C
C START-DESCRIPTION :
C  description to be inserted
C END-DESCRIPTION
C
C START-DESIGN :
C NP:
C BEGIN
C  design to be inserted here
C END
C END-DESIGN
C
C START-CODE :
C
C  This library provides CALCOMP like calls that result in x,y,npen
C  Information being written to a file.  This file can then be post-
C  processesed to any specific plotter like a ZETA.  This was written to
C  avoid having to run the Houston Instruments Library under an INFO
C  process.  It will also provide, however, the ability to view
C  plotter output on Terminals.
C
C  The format of the file is:
C    FORMATTED ASCII CHARACTERS for guaranteed operation at the expense
C    of speed and disk preservation.
C
C  Line 1:  NP Version Number starting at 100
C  Line 2:  Comment.
C  Line 3:  Comment, perhaps a date?
C  Line 4:  XSIZE, YSIZE   (2F9.3)
C  Line 4:  First Data record.
C  Line 5:  Second Data record.
C    and so on...
C
C  Each data record contains XPOS, YPOS, NPEN   (2F9.3,I6)
C    NPEN can be 2 for DRAW
C                 3 for MOVE
C                 -2,-3 for reorigin.
C                 999 for QUIT
C                 1000 + pen for NEWPEN calls, 1004 -> NEWPEN(4)
C
C Author: Phil Burk, ESI, 3/12/86
C
C %M MOD: PLB 4/2/86 Read filename from user instead of generating
C                    internally.
C
C -----------------------------------------------
      SUBROUTINE HIINIT(IUNIT,XSIZE,YSIZE)
C
C %S Original stub for Houston Instruments Implementation.
C
      CALL  NP_INIT(IUNIT, XSIZE, YSIZE)
      RETURN
      END
C
C -----------------------------------------------
      SUBROUTINE RSTR(IVAL)
C
C %S Stub for original Houston Instruments Library.
C
      CALL NP_TERM
      RETURN
      END
C
C -------------------------------------------------------
      SUBROUTINE NP_INIT(IUNIT, XSIZE, YSIZE)
C
C Initialize the Plotter System, returns Plotter size.
C
      COMMON /ZZZNEUT/ JUNIT
      CHARACTER NPFILE*40
C
C Print welcome.
      PRINT *,' '
      PRINT *,' --=< NEUTRAL PLOT LIBRARY by ESI >=--'
      PRINT *,' '
C
C Ask for plot file name.
      PRINT *,' Enter plot file name, (eg. NEUTRAL_PLOT.USER#)'
      READ 1500, NPFILE
1500  FORMAT (A)
C
C Open file for storing plot calls.
      CALL HOGFNT(JUNIT)
      OPEN(UNIT=JUNIT,FILE=NPFILE)
      PRINT 2000,NPFILE
2000  FORMAT ('Neutral plot in file = ',A)
C
C  Ask for size of plotter that GKS can use.
      PRINT *,' Enter XSIZE and YSIZE'
      READ *, XSIZE, YSIZE
C
C Write Header
      IVERS = 100
      WRITE(JUNIT,1000) IVERS
1000  FORMAT (I6)
      WRITE(JUNIT, 1010)
1010  FORMAT('NEUTRAL PLOT FILE from FINDER by ESI')
      PRINT *,1020
      WRITE(JUNIT, 1020)
1020  FORMAT('Process using NI.NEUTRAL_INTERP')
      WRITE(JUNIT, 1030) XSIZE, YSIZE
1030  FORMAT(2F9.3)
C
      IUNIT = JUNIT
      RETURN
      END
C
C ----------------------------------------------
      SUBROUTINE NEWPEN(IPEN)
      COMMON /ZZZNEUT/ JUNIT
      INTEGER IPEN,NPEN
      REAL XPOS,YPOS
C
C Save NEWPEN info.
      XPOS = 0.0
      YPOS = 0.0
      NPEN = IPEN + 1000
      WRITE(JUNIT,1100) XPOS, YPOS, NPEN
1100  FORMAT (2F9.3,I6)
      RETURN
      END
C
C ----------------------------------------------
      SUBROUTINE PLOT(XPOS, YPOS, NPEN)
      COMMON /ZZZNEUT/ JUNIT
      INTEGER IPEN,NPEN
      REAL XPOS,YPOS
C
      WRITE(JUNIT,1100) XPOS, YPOS, NPEN
1100  FORMAT (2F9.3,I6)
      RETURN
      END
C
C -----------------------------------------------
      SUBROUTINE NP_TERM
C
C %S Terminate Neutral PLot File
C
      COMMON /ZZZNEUT/ JUNIT
      INTEGER IPEN,NPEN
      REAL XPOS,YPOS
C
C Store Termination Command
      XPOS = 0.0
      YPOS = 0.0
      NPEN = 999
      WRITE(JUNIT,1100) XPOS, YPOS, NPEN
1100  FORMAT (2F9.3,I6)
C
      RETURN
      END
C
C END-CODE
