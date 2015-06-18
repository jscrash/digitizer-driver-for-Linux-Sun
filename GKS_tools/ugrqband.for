C  DEC/CMS REPLACEMENT HISTORY, Element UGRQBAND.FOR
C  *3    19-SEP-1989 10:22:12 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:04:48 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:55:43 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGRQBAND.FOR
C -----------------------------------------------------
      SUBROUTINE UGRQBAND(PETYPE, X1, Y1,
     1                     STATUS, TNR, XPOS, YPOS, BUTTON)
C
C %S Perform a locator operation with a special prompt echo type.
C
C %S Phil Burk
C
C %P INPUTS:
C       PETYPE = (INT) Prompt echo type.
C            -4 = Band vector.
C            -5 = Band Rectangle.
C       X1,Y1 = (REAL) Initial position in current WC coordinates.
C    OUTPUTS:
C       STATUS = (INT) Completion status.
C             0 = SUCCESS.
C            -1 = FAIL.
C       TNR    = (INT) Transformation number hit.
C       XPOS, YPOS = Input location.
C       BUTTON = (INT) Button hit on puck.
C

      REAL XPOS,YPOS, X1,Y1
      INTEGER TNR, STATUS, PETYPE, BUTTON, PETEMP

      INTEGER SUCCESS, FAIL, GKS_OK
      INTEGER WKID, LCDNR
      INTEGER NTNBR, RECLEN, ERRIND
      REAL XMIN,XMAX, YMIN,YMAX
      CHARACTER RECORD(1)*80
C
C ...Set to Finder values.  %Q
      WKID = 1
      LCDNR = 1
      SUCCESS = 0
      FAIL = -1
      GKS_OK = 1

C Get current transformation number.
       CALL GQCNTN(ERRIND, NTNBR)
       IF (ERRIND .NE. 0) NTNBR = 0

C Initialize Locator.
       XMIN = 0.0
       XMAX = 1.0
       YMIN = 0.0
       YMAX = 1.0
       RECLEN = 1
       RECORD(1) = ' '
       CALL GINLC (WKID, LCDNR, NTNBR, X1,Y1, PETYPE,
     1             XMIN, XMAX, YMIN, YMAX, RECLEN, RECORD)
       CALL GRQLC(WKID, LCDNR, STATUS, TNR, XPOS, YPOS)

C
C  Convert from GKS status to Finder Status,i.e. SUCCESS = 0.
C
       IF (STATUS .EQ. GKS_OK) THEN 
          STATUS = SUCCESS
       ELSE
          STATUS = FAIL
       ENDIF

C %Q Illegal call to get button value for compound Input.
       CALL GKZQBTN(BUTTON)

C Reset locator.
      PETEMP = 1
       CALL GINLC (WKID, LCDNR, NTNBR, XPOS,YPOS, PETEMP,
     1             XMIN, XMAX, YMIN, YMAX, RECLEN, RECORD)

      RETURN
      END
