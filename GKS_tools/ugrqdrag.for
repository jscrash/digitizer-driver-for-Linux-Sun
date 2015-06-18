C  DEC/CMS REPLACEMENT HISTORY, Element UGRQDRAG.FOR
C  *3    19-SEP-1989 10:22:20 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *2    12-AUG-1989 12:04:57 GILLESPIE "(SPR 3) Fix up include file references"
C  *1    10-AUG-1989 18:55:48 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element UGRQDRAG.FOR
C --------------------------------------------------------
      SUBROUTINE UGRQDRAG( SEGMENT, WX1, WY1,
     +                     STATUS, TNR, WX2, WY2, BUTTON )
C
C %S Perform a locator operation with a segment as prompt echo
C
C %S Guy H Binns
C
C %P INPUTS:
C       SEGMENT = (INT) Prompt echo segment.
C       X1,Y1 = (REAL) Initial position in current WC coordinates.
C    OUTPUTS:
C       STATUS = (INT) Completion status.
C             0 = SUCCESS.
C            -1 = FAIL.
C       TNR    = (INT) Transformation number hit.
C       XPOS, YPOS = Input location.
C       BUTTON = (INT) Button hit on puck.
C
C
      INTEGER SEGMENT, STATUS, TNR, BUTTON
      REAL WX1, WY1, WX2, WY2
C
      INTEGER SUCCESS,FAIL,GKS_OK
      INTEGER WKID, LCDNR
      INTEGER NTNBR, RECLEN, ERRIND, PETYPE
      REAL XMIN, XMAX, YMIN, YMAX
C
C  CALL TO GINLC REQUIRES A CHARACTER DATA TYPE.
C
      INTEGER IREC(20) 
      CHARACTER CREC*80
      EQUIVALENCE( IREC,CREC )
C
C Set Finder variables.
C
      WKID = 1
      LCDNR = 1
      SUCCESS = 0
      FAIL = -1
      GKS_OK = 1
C
      CALL GQCNTN( ERRIND, NTNBR)
      IF (ERRIND .NE. 0) THEN
         NTNBR = 0
      ENDIF
C
      PETYPE = -7
      XMIN = 0.0
      XMAX = 1.0
      YMIN = 0.0
      YMAX = 1.0
      RECLEN = 1
      IREC(1) = SEGMENT
      CALL GINLC( WKID, LCDNR, NTNBR, WX1, WY1, PETYPE,
     +            XMIN, XMAX, YMIN, YMAX, RECLEN, CREC )
C
C Get Locator position and button number from GKS.
C
      CALL GRQLC( WKID, LCDNR, STATUS, TNR, WX2, WY2 )
      CALL GKZQBTN( BUTTON )
C
C Convert from GKS status to Finder Status.
C
      IF (STATUS .EQ. GKS_OK) THEN
         STATUS = SUCCESS
      ELSE
         STATUS = FAIL
      ENDIF

C
C Reset the Locator Promt Echo Type.
C
      PETYPE = 1
      CALL GINLC( WKID, LCDNR, NTNBR, WX1, WY1, PETYPE,
     +            XMIN, XMAX, YMIN, YMAX, RECLEN, CREC )
C
      RETURN
      END
