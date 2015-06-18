C  DEC/CMS REPLACEMENT HISTORY, Element MO_MODSUBS.FOR
C  *7     1-MAR-1990 17:19:07 GILLESPIE "(SPR 1) Remove unused variables"
C  *6     6-NOV-1989 16:48:59 GILLESPIE "(SPR 30) Get 'glue' entry points straightened out"
C  *5     6-NOV-1989 10:20:40 GILLESPIE "(SPR 30) Change mo_strlen to istrlen"
C  *4     6-NOV-1989 09:04:50 PURNA "(SPR 30) fix mo_ calls to mog_"
C  *3     3-NOV-1989 12:02:09 GILLESPIE "(SPR 30) Change entry points for new n-List nomenclature"
C  *2    19-SEP-1989 10:14:01 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:49:06 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element MO_MODSUBS.FOR
C******************************************************************************
C
C                Copyright Finder Graphics Systems, Inc. 1989
C		   Unpublished -- All rights reserved
C
C THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc AND MAY
C CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
C DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
C MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF
C
C			Finder Graphics Systems, Inc.
C			201 Tamal Vista Blvd
C			Corte Madera, California 94925
C			(415) 927-0100
C
C (The above notice does not constitute an admission of publication or
C unrestricted dissemination of the work)
C
C******************************************************************************
C

      SUBROUTINE MO_FIXLAY(TRINC,NTRCES,NLAY)
C ****************************************************************************
C
C
C THIS SUBROUTINE TAKES A DIGITIZED MODEL AND FIGURES OUT THE NUMBER OF LAYERS.
C FOR EACH TRACE, THE DEPTH OF EACH LAYER IS DETERMINED.  IF A LAYER IS UNDE-
C FINED FOR A TRACE, THE DEPTH IS SHOWN TO BE THAT OF THE PREVIOUS LAYER, THUS
C DEFINING ZERO THICKNESS.
C
C
C TRINC - TRACE SPACING INTERVAL
C NTRCES- NUMBER OF TRACES
C NLAY  - NUMBER OF LAYERS
C
C THE OUTPUT DEPTHS ARE STORED IN ARRAY YOUT.  THESE CAN BE RETRIVED BY USING
C THE FUNCTION GETDEP, WHICH WILL RETURN THE DEPTH OF A GIVEN TRACE AND LAYER.
C
C FIT ROWS TO THE TRACE SPACING INTERVAL
C
C THE FIRST AND LAST DIGITIZED LAYERS MUST BE THE MAXIMUM WIDTH OF THE MODEL.
C
C L      = COUNTER AND SUBSCRIPT FOR OUTPUT ROWS
C
C ******************************************************************************

      INCLUDE 'mo_modcom.i'
      REAL MO_CALC_SLOPE
      L    = 0
C
C CALCULATE X MAX.
C THIS IS ADDED IN A LOOP INSTEAD OF MULTIPLIED SO A SUM OF TRINCS
C CALCUALTED LATER CAN BE LEGALLY COMPARED TO XMAX. DO NOT CHANGE THIS
C TO MAKE IT MORE EFFICINET OR IT WON'T WORK ANY MORE.
C
      XMAX=0.0
      DO 10 JK = 1 , NTRCES-1
10    XMAX=XMAX+TRINC
C
      DO 10000 JK = 1 , NLAY
      IF(JK.EQ.1)THEN
            IPTR = 1
      ELSE
            IPTR=LAYERS(JK-1)+1
      ENDIF
C
C PROCESS EACH ROW IN LAYER
C
      XST = 0.0
      DO 190 J = IPTR,LAYERS(JK)
C
C IF FIRST ROW, SEE IF LINES UP ON LEFT EDGE OR NOT
C
      IF(J.NE.IPTR) GOTO 175
C
C DETERMINE STARTING TRACE OF THIS LAYER
C ROUND UP A TRACE (1.99) BUT NOT TO FAR
C
      ISTART = (XBUF(J)/TRINC) + 1.99
C
C IF TRACE 1 THEN SET IT UP
C
      IF(ISTART.EQ.1) THEN
          L=L+1
          YOUT(L) = YBUF(J)
          XST=XST+TRINC
          GOTO 190
      ENDIF
C
C STARTS AFTER TRACE 1, SO FILL UNUSED TRACE DATA
C
      DO 179 KK = 1,ISTART-1
          L = L + 1
          YOUT(L) = FICT
          XST = XST+TRINC
179   CONTINUE
      GOTO 190
C
C NOW FILL CURRENT TRACE
C
175   IF(XST.GT.XBUF(J)) GO TO 190
      IF(XBUF(J).GT.XMAX.AND.XST.GT.XMAX)GOTO 190
         L     = L + 1
      SLOPE=MO_CALC_SLOPE(XBUF(J),XBUF(J-1),YBUF(J),YBUF(J-1))
         CONST = YBUF(J) - SLOPE * XBUF(J)
         YOUT(L) = SLOPE * XST + CONST
         XST = XST+ TRINC
         GOTO 175
190   CONTINUE
C
C FINISH OUT TRACES AT END NOT DEFINED
C OR LAST ROW NOT DIGITIZED TO XMAX BUT CLOSE ENOUGHT TO INCLUDE IN LAST TRACE
C
155    IF(XST-TRINC.LT.XMAX.AND.MOD(L,NTRCES).NE.0) THEN
          L = L +1
          INDEX = LAYERS(JK)
C
C IF LAST TRACE, SEE IF LAST ROW IS CLOSE ENOUHT TO FILL IT
C
           IF(MOD(L,NTRCES).NE.0)THEN
              ISTART=0
           ELSE
              ISTART=(XBUF(INDEX)/TRINC)+1.01
           ENDIF
C
C FILL TRACE WITH FICT, UNLESS VALID LAST TRACE VALUE
C
           IF(ISTART.NE.NTRCES)THEN
               YOUT(L) = FICT
           ELSE
               SLOPE = MO_CALC_SLOPE( XBUF(INDEX), XBUF(INDEX-1),
     X                             YBUF(INDEX), YBUF(INDEX-1) )
               CONST = YBUF(INDEX) - SLOPE * XBUF(INDEX)
               YOUT(L)=SLOPE*XST+CONST
          ENDIF
C
C LOOP UNTIL FINISHED LAYER
C
          XST = XST + TRINC
          GOTO 155
      ENDIF
10000 CONTINUE
C
C DUMP CALCULATED LAYERS FOR DEBUG
C
C      CALL HOGFNT(IUNIT)
C      OPEN(IUNIT,FILE='MO_FIXPRINT',STATUS='UNKNOWN')
C      DO 212 JK = 1 , NLAY
C      II1 = (JK-1)*NTRCES+1
C      II2 = JK*NTRCES
C      WRITE(IUNIT,213)JK,(YOUT(I),I=II1,II2)
C 213   FORMAT(I5,12F10.2)
C 212   CONTINUE
C      CLOSE(IUNIT)
      RETURN
      END





C *****************************************************************************
      REAL*4 FUNCTION MO_CALC_SLOPE(X1, X2, Y1, Y2)
C CALCULATE THE SLOPE OF A LINE DETERMINED BY TWO ROWS
C RETURNS HUGE (1E31) IF X1 = X2
C
      IF (X1 .EQ. X2) THEN
         MO_CALC_SLOPE = 1E31
      ELSE
         MO_CALC_SLOPE = (Y2 - Y1) / (X2 - X1)
      ENDIF
      RETURN
      END




C *****************************************************************************
      REAL*4 FUNCTION MO_GETDEP(ITRACE,ILAYER,NTR)
C
C RETURN DEPTH FOR LAYER = ILAYER, TRACE = ITRACE
C NTR = TOTAL NUMBER OF TRACES
C
C
      INCLUDE 'mo_modcom.i'
C
      MO_GETDEP = YOUT( (ILAYER-1)*NTR+ITRACE )
      RETURN
      END




      SUBROUTINE MO_READ(NLIST,MO_PTR,NPTS,NLAYERS,STATUS)
C *****************************************************************************
C
C    READ ROWS AND MO_STRUCT STRUCTURE FROM N-LIST FILEX
C    RETURN NUMBER OF ROWS IN NPTS
C
C NLIST  - POINTER TO MODEL N-LIST
C MO_PTR - POINTER TO MODEL STRUCTURE
C NPTS   - RETURNED NUMBER OF DIGITIZED ROWS
C NLAYERS - NUMBER OF LAYERS FOUND
C STATUS  - 0 OK, 1 = FAIL
C
C *****************************************************************************

      INCLUDE 'mo_modcom.i'
      INCLUDE 'mo_struct.i'
      INCLUDE 'esi_nl.i'
C
      CHARACTER*130 STRING
      EQUIVALENCE(E_STRING,STRING)
      REAL*8 NLIST,MO_PTR
      REAL*8 VALUES(2)
      INTEGER*4 DIM_LIST(2)
      INTEGER STATUS
      REAL X,Y
      STATUS=0
C
C QUERY ROW LIST STRUCTURE
C
      ISIZE = MO_SIZE*4
      CALL MOG_HOBLOCKMOVE(MO_PTR,MO_STRUCT,ISIZE)
C
C HOW MANY LAYERS
C
      NLAYERS=0
      II = NLG_INQ_NLIST_INT(NLIST, NL_NLIST_NROWS, NPTS)
      IF(NPTS.EQ.0)THEN
          CALL WIGALERT(1,'HELP',
     X    'There are no layer points digitized.',
     X    'A layer model must be digitized to generate model.')
          STATUS=1
          RETURN
      ENDIF
C
C READ ROWS
C
      IPREV_TABLE = -9999
      NROWS = 0
C
      NDIM = 2
      DIM_LIST(1) = 1
      DIM_LIST(2) = 2
      VALUES(1) = HOPTR(X)
      VALUES(2) = HOPTR(Y)
      II = NLG_FIRST_ROW(NLIST, NDIM, DIM_LIST, VALUES)
C
      DO 100 JK = 1 , NPTS
C
C GET ROWS OUT AND BUILD LAYERS
C
            II = NLG_INQ_CURRENT_ROW(NLIST, IROW, ITABLE)
            IF(ITABLE.NE.IPREV_TABLE)THEN
                II = NLG_INQ_TABLE_INT(NLIST,ITABLE,
     +                             NL_TABLE_NROWS, NSTR)
                IF(NSTR.LE.1)GOTO 200
                NLAYERS=NLAYERS+1
                IPREV_TABLE=ITABLE
                LAYERS(NLAYERS)=NSTR
                IF(NLAYERS.GT.1)LAYERS(NLAYERS)=LAYERS(NLAYERS)
     X                         +LAYERS(NLAYERS-1)
            ENDIF
            NROWS=NROWS+1
            IF(NROWS.GT.MAX_ROWS)THEN
                  WRITE(STRING,11) MAX_ROWS
11    FORMAT('Maximum point limit exceeded. Limit = ',I5)
                  CALL WIGALERT(1,'MO_TIE_CALC',STRING,
     X            'Consult F.G.S. concerning this message.')
                  STATUS=1
                  RETURN
            ENDIF
            XBUF(NROWS)=X
            YBUF(NROWS)=Y
200         II = NLG_NEXT_ROW(NLIST, NDIM, DIM_LIST, VALUES)
100   CONTINUE
      NPTS=NROWS
      RETURN
      END




C *****************************************************************************
      SUBROUTINE MO_CLEANUP_STRING(STRING)
C
C  REMOVE NULL TERMINATOR AND BLANK PAD A CSTRING
C
      CHARACTER*(*) STRING
      CHARACTER*255 STRING2
      EQUIVALENCE(STRING2,ISTRING)
      I = LEN(STRING)
      STRING2=STRING
      J = ISTRLEN(STRING)
      STRING=STRING2(1:I)
      DO 100 JK = J+1,I
100   STRING(JK:JK)=' '
      RETURN
      END




      SUBROUTINE MO_SORT_LAYERS(NTRCES,NLAYERS,STATUS)
C *****************************************************************************
C
      INCLUDE 'mo_modcom.i'
C
C
      INTEGER NTRCES,NLAYERS,STATUS
      CHARACTER*128 STRING
C
C SORT LAYERS
      IF(NLAYERS.LT.2)THEN
            CALL WIGALERT(1,'HELP','There must be at least 2 layers',
     X      'Add more layers.')
            STATUS=1
            RETURN
      ENDIF
C
      STATUS = 0
C
C SET POSITION ARRAY
C
C
      DO 100 JK = 1 , NLAYERS
100   LAYERS_PTRS(JK)=JK
C
C FIND TOP AND BOTTOM LAYERS
C
      NODES = NTRCES*NLAYERS
      YMIN  = 1E31
      YMAX  = -1E31
      DO 110 JK = 1 , NODES
            IF(YOUT(JK).NE.FICT .AND. YOUT(JK).LT.YMIN)THEN
                ITOP = JK
                YMIN = YOUT(JK)
            ENDIF
            IF(YOUT(JK).NE.FICT .AND. YOUT(JK).GT.YMAX)THEN
                IBOT = JK
                YMAX = YOUT(JK)
            ENDIF
110   CONTINUE
C
C MAKE SURE TOP LAYER IS OK
C
      ITOP = (ITOP-1)/NTRCES + 1
C
C NO FICTS ALLOWED
C
      NFICT = 0
      JK1   = (ITOP-1)*NTRCES + 1
      JK2   = JK1+NTRCES-1
      DO 120 JK = JK1,JK2
120   IF(YOUT(JK).EQ.FICT)NFICT = NFICT + 1
      IF(NFICT.NE.0)THEN
            STATUS = 1
            CALL WIGALERT(1,'HELP',
     X 'The top most layer must go all the way across the model.',
     X ' ')
            RETURN
      ENDIF
C
C MAKE SURE BOTTOM LAYER IS OK
C
      IBOT = (IBOT-1)/NTRCES + 1
C
C NO FICTS ALLOWED
C
      NFICT = 0
      JK1   = (IBOT-1)*NTRCES + 1
      JK2   = JK1+NTRCES-1
      DO 130 JK = JK1,JK2
130   IF(YOUT(JK).EQ.FICT)NFICT = NFICT + 1
      IF(NFICT.NE.0)THEN
            STATUS = 1
            CALL WIGALERT(1,'HELP',
     X 'The bottom most layer must go all the way across the model.',
     X ' ')
            RETURN
      ENDIF
C
C MAKE THE TOP LAYER THE FIRST ONE AND THE BOTTOM LAYER THE LAST ONE
C
      CALL MO_SWITCH_LAYERS(ITOP,1,NTRCES)
      IF(IBOT.EQ.1)IBOT=ITOP
      CALL MO_SWITCH_LAYERS(IBOT,NLAYERS,NTRCES)
C
C MAKE SURE TOP IS REALLY TOP AND BOTTOM IS REALLY BOTTOM.
C IF YMIN OR YMAX WAS FOUND AND THE LAYERS WENT ALL THE WAY ACROSS
C WE ASSUMED TOP AND BOTTOM. CHECK TO SEE IF PARTIAL LAYERS ABOVE TOP
C OR BELOW BOTTOM.
C
      N_LESS_TOP = 0
      N_MORE_BOT = 0
      DO 150 I = 1,NTRCES
            DO 140 JK = 1,NLAYERS
C
C CHECK TOP
C
            IF(JK.EQ.1)GOTO 135
            ITOP = (JK-1)*NTRCES+I
            IF(YOUT(ITOP).NE.FICT.AND.YOUT(ITOP).LE.YOUT(I))
     X         N_LESS_TOP = N_LESS_TOP+1
135         IF(JK.EQ.NLAYERS)GOTO 140
            IBOT = (JK-1)*NTRCES+I
            LAST = (NLAYERS-1)*NTRCES+I
            IF(YOUT(IBOT).NE.FICT.AND.YOUT(IBOT).GE.YOUT(LAST))
     X         N_MORE_BOT = N_MORE_BOT + 1
140         CONTINUE
150   CONTINUE
C
C DANGLING LAYER ACROSS TOP
C
      IF(N_LESS_TOP.NE.0)THEN
            STATUS = 1
            CALL WIGALERT(1,'HELP',
     X 'The top most layer must go all the way across the model.',
     X ' ')
            RETURN
      ENDIF
C
C DANGLING LAYER ACROSS BOTTOM
C
      IF(N_MORE_BOT.NE.0)THEN
            STATUS = 1
            CALL WIGALERT(1,'HELP',
     X 'The bottom most layer must go all the way across the model.',
     X ' ')
            RETURN
      ENDIF
C
C SORT THEM
C
      DO 500 I = 2,NLAYERS-2
            DO 400 JK = I+1, NLAYERS - 1
                  DO 300 J = 1,NTRCES
                  ITOP = (I-1)*NTRCES+J
                  IBOT = (JK-1)*NTRCES+J
                  IF(YOUT(ITOP).EQ.FICT.OR.YOUT(IBOT).EQ.FICT)GOTO 300
                  IF(YOUT(ITOP).LT.YOUT(IBOT))GOTO 300
                  CALL MO_SWITCH_LAYERS(I,JK,NTRCES)
                  GOTO 400
300               CONTINUE
400         CONTINUE
500   CONTINUE

C
C
C NOW SEE IF ANY MORE ARE OUT OF ORDER. IF THEY ARE, WE HAVE A CROSS
C LINE PROBLEM
C
      N_LESS_TOP=0
      DO 600 I = 1 , NTRCES
            DO 550 JK = 1 , NLAYERS- 1
            ITOP = (JK-1)*NTRCES+I
            IBOT = ITOP + NTRCES
            IF(YOUT(ITOP).EQ.FICT.OR.YOUT(IBOT).EQ.FICT)GOTO 550
            IF(YOUT(ITOP).GE.YOUT(IBOT))THEN
                  N_LESS_TOP=1
                  GOTO 700
            ENDIF
550         CONTINUE
600   CONTINUE
C
700   IF(N_LESS_TOP.NE.0)THEN

            WRITE (STRING,701) I, YOUT(ITOP)  
701         FORMAT ('At Trace Number= ',I5,'       Y= ',F9.1)
            STATUS=1
            CALL WIGALERT(1,'HELP',
     X      'There are illegal intersecting layers in this model.',
     X      STRING)
      ENDIF
      RETURN
      END





      SUBROUTINE MO_SWITCH_LAYERS(FROM,TO,NTRCES)
C ****************************************************************************
C
C     SWITCH TWO LAYERS IN MATRIX
C     FROM AND TO LAYERS
C
C ****************************************************************************

      INCLUDE 'mo_modcom.i'
C
      INTEGER FROM,TO
C
C IF SAME, DO NOTHING
C
      IF(FROM.EQ.TO)RETURN
C
C SWITCH POINTERS
C
      ITEMP = LAYERS_PTRS(FROM)
      LAYERS_PTRS(FROM) = LAYERS_PTRS(TO)
      LAYERS_PTRS(TO)  =ITEMP

C
C SWITCH SEDIMENT TYPES
C
      ITEMP = LAYERS_SED(FROM)
      LAYERS_SED(FROM) = LAYERS_SED(TO)
      LAYERS_SED(TO) = ITEMP

C
C SWITCH ROWS
C
      DO 100 JK = 1 , NTRCES
       IFROM = (FROM-1)*NTRCES+JK
       ITO   = (TO-1)*NTRCES+JK
       TEMP = YOUT(IFROM)
       YOUT(IFROM) = YOUT(ITO)
       YOUT(ITO)   = TEMP
100   CONTINUE
      RETURN
      END




C ****************************************************************************
      SUBROUTINE MO_REMOVE_FICT(NTRCES,NLAYERS)
C
      INCLUDE 'mo_modcom.i'
C
C
C REMOVE FICT VALUES
C
C
      NODES = NLAYERS*NTRCES
      DO 200 JK = NTRCES +1,NODES
      IF(YOUT(JK).EQ.FICT)THEN
          YOUT(JK)=YOUT(JK-NTRCES)
      ENDIF
200   CONTINUE
      RETURN
      END




C *****************************************************************************
      SUBROUTINE MO_RESET_TEXT
      INCLUDE 'sun_gks77.i'
      CALL GSTXFP(SIMPLX,GCHARP)
      CALL GSTXAL(GAHNOR,GAVNOR)
      CALL GSCHUP(0.,1.)
      CALL GSCHXP(1.0)
      CALL GSCHSP(0.5)
      RETURN
      END




C *****************************************************************************
      SUBROUTINE MO_ZERO
C
C  ZERO OUT MODEL ARRAYS
C
C
      INCLUDE 'mo_modcom.i'
C
      DO 10 JK = 1 , 2500
      VEL(JK) = 0.0
      DEN(JK)=0.0
      DEPTH(JK)=0.0
      CONN(JK)=0.0
      VA(JK)=0.0
      XBUF(JK)=0.0
      YBUF(JK)=0.0
10    CONTINUE
C
      DO 20 JK = 2501,MAX_ROWS
      VEL(JK)=0.0
      DEN(JK)=0.0
      XBUF(JK)=0.0
      YBUF(JK)=0.0
20    CONTINUE

      RETURN
      END





C *****************************************************************************
C      SUBROUTINE MO_SEGY_DIALOG(MO_NAME,LINE_NAME,ICREATE,
C     X ICANCEL)
C
C MTIELOT DIALOG
C
C MO_NAME = NAME OF MODEL
C LINE_NAME IS NAME OF LINE TO ASSOCIATE MODEL WITH
C ICREATE = 1 CREATE = 0 DO NOT CREATE
C ICANCEL = WAS CANCEL HIT
C
C
C      INCLUDE 'MO_MODEL.RI'
C      INCLUDE 'MO_MODCOM.I'
C
C      CHARACTER*255 STRING,HELP/'MO_TIE'/
C      INTEGER CDLGID,CDITEM,CDVALUE,ID
C      CHARACTER *(*) MO_NAME,LINE_NAME
C      ICREATE=0
C      ICANCEL=0
C
C DEFINE DIALOG BOX
C
C      CALL MO_RESOURCE(MODSEGY,ID)
C      STRING=' '
C      CALL WIGSCDT(ID,MODSEGY_LINE,STRING)
C      STRING=MO_NAME
C      CALL WIGSCDT(ID,MODSEGY_MODEL,STRING)
C      CALL WIGSCDV(ID,MODSEGY_CREATE,ICREATE)
C      CALL WIGOPCD(ID)
C100   CALL WIGRQCDI(CDLGID,CDITEM,CDVALUE,STRING)
C
C      IF(CDITEM.EQ.MODSEGY_OK)GOTO 999
C      IF(CDITEM.EQ.MODSEGY_CANCEL) THEN
C          ICANCEL=1
CC          GOTO 9999
C      ENDIF
C      IF(CDITEM.EQ.MODSEGY_CREATE)THEN
C          IF(ICREATE.EQ.1)THEN
C            ICREATE=0
C            CALL WIGENCDI(ID,MODSEGY_LINE,0)
C          ELSE
C            ICREATE=1
CC            CALL WIGENCDI(ID,MODSEGY_LINE,1)
C          ENDIF
C          GOTO 100
C      ENDIF
C
C READ AND VERIFY LINE
C
C999   IF(ICREATE.EQ.0)GOTO 9999
C      CALL WIGQCCDI(CDLGID,MODSEGY_LINE,IDUM,STRING)
C      IF(STRING.EQ.' ')THEN
C            CALL WIGALERT(1,HELP,'The line name may not be blank.',
C     X              'Enter a line name or turn off create box.')
C            GOTO 100
C      ENDIF
C      LINE_NAME=STRING
C      STRING='F'//LINE_NAME
C      CALL OKFILE(STRING,IERR)
C      IF(IERR.EQ.2)THEN
C            CALL WIGALERT(1,HELP,'The line name is invalid.',
C     X 'Use letters, numbers, ".", "-", or "_".')
C            GOTO 100
C      ENDIF
C9999  CALL WIGCLCD(ID)
C      RETURN
C      END




C *****************************************************************************
      SUBROUTINE MO_READ_EMP_VELS(NLV_PTR,NVELPTS,STATUS)
C
C READ VELOCITY AND DENSITY ROWS AND STORE IN ARRAY
C RETURN NUMBER OF ROWS IN NVELPTS
C
C NLV_PTR - POINTER TO VELOCITY NLIST
C NVELPTS   - RETURNED NUMBER OF DIGITIZED ROWS
C STATUS  - 0 OK, 1 = FAIL
C
C
      INCLUDE 'mo_modcom.i'
      INCLUDE 'esi_nl.i'
C
      CHARACTER*130 STRING
      EQUIVALENCE(E_STRING,STRING)
      REAL*8 NLV_PTR
      INTEGER*4 NDIM
      INTEGER*4 DIM_LIST(4)
      REAL*8 VALUES(4)
      REAL*4 VEL_VALUE
      REAL*4 DEN_VALUE
      REAL*4 X
      REAL*4 Y
      INTEGER STATUS
C
      STATUS=0
C
C HOW MANY ROWS
C
      II = NLG_INQ_NLIST_INT(NLV_PTR, NL_NLIST_NROWS, NPTS)
C
C READ ROWS
C
      VALUES(1) = HOPTR(X)
      VALUES(2) = HOPTR(Y)
      VALUES(3) = HOPTR(VEL_VALUE)
      VALUES(4) = HOPTR(DEN_VALUE)
      DIM_LIST(1) = 1
      DIM_LIST(2) = 2
      DIM_LIST(3) = 3
      DIM_LIST(4) = 4
      NDIM = 4
C
      II = NLG_FIRST_ROW(NLV_PTR, NDIM, DIM_LIST, VALUES)
C
      DO 100 NROWS = 1 , NPTS
C
C GET ROWS OUT
C
         XVEL(NROWS)=X
         YVEL(NROWS)=Y
         HVEL(NROWS)=VEL_VALUE
         HDEN(NROWS)=DEN_VALUE
         VVEL(NROWS)=0.0
         VDEN(NROWS)=0.0
         VEL_LAYERS(NROWS)=0
         II = NLG_NEXT_ROW(NLV_PTR, NDIM, DIM_LIST, VALUES)
100   CONTINUE
      NVELPTS=NPTS
      RETURN
      END




C ******************************************************************
      SUBROUTINE MO_ASSIGN_VELS(NTRCES,NLAYERS,XMAX,TRINC,NVELPTS,
     X IMAX,IFDEN,STATUS)
C
C NTRCES = NUMBER OF TRACES
C NLAYERS = NUMBER OF LAYERS
C XMAX    = MAXIMUM X
C TRINC   = TRACE SPACING INCREMENT
C NVELPTS = NUMBER OF VELOCITY/DENSITY ROWS
C IFDEN   = RETURNED 0 IF NO OR 1 IF DENSITY USED
C IMAX    = MAX DEPTH FROM DATUM
C STATUS  = 0 SUCCESS, 1 = FAIL
C
C
      INCLUDE 'mo_modcom.i'
C
      REAL*4 MO_GETDEP
      STATUS = 0
      DO 100 JK = 1 , NVELPTS
C
C  CHECK IF OUT OF BOUNDS OF MODEL
C
C      IF (XVEL(JK).LT. -0.01 .OR. XVEL(JK).GT. XMAX+.01) THEN
C        CALL WIGALERT(1,'MODEL/EMP/VELOCITY',
C     X  'Detected Vel/Den points digitized outside model X bounds.',
C     X  'Please delete or move these points.')
C        STATUS = 1
C        RETURN
C      ENDIF

C      IF ( YVEL(JK).LT. 0.0 .OR. YVEL(JK).GT. IMAX) THEN
C        CALL WIGALERT(1,'MODEL/EMP/VELOCITY',
C     X  'Detected Vel/Den points digitized outside Y bounds of model.',
C     X  'Please delete or move these points.')
C        STATUS = 1
C        RETURN
C      ENDIF
C
C  WHICH TWO TRACES IT ROW ON/OR BETWEEN
C
      ITR = XVEL(JK)/TRINC + 1.001
      IF(ITR.EQ.0)ITR=1
      IF(ITR.EQ.NTRCES)ITR=NTRCES-1
      ITR2=ITR+1
C
C CALCULATE X LOCATIONS OF TRACES AND DX (FOR DY/DX)
C
      X1 = (ITR-1)*TRINC
      X2 = ITR * TRINC
      DX = X2-X1
C
C FIND FIRST LAYER THAT IT IS ABOVE
C
            DO 200 JJ = 1,NLAYERS
C
C CALCULATE DEPTH AT XVEL(JK)
C
            Y1 = MO_GETDEP(ITR,JJ,NTRCES)
            IF(XVEL(JK).EQ.X1.AND.Y1.NE.FICT)THEN
                  Y_RESULT=Y1
                  IF(Y_RESULT.GT.YVEL(JK))GOTO 250
            ENDIF
            Y2 = MO_GETDEP(ITR2,JJ,NTRCES)
            IF(XVEL(JK).EQ.X2.AND.Y2.NE.FICT)THEN
                  Y_RESULT=Y2
                  IF(Y_RESULT.GT.YVEL(JK))GOTO 250
            ENDIF
            IF(Y1.EQ.FICT.OR.Y2.EQ.FICT)GOTO 200
            SLOPE = (Y2-Y1)/DX
            B  = Y1 - SLOPE * X1
            Y_RESULT = SLOPE*XVEL(JK)+B
C
C CHECK IF IT BELONGS TO THIS LAYER
C
            IF(Y_RESULT.GT.YVEL(JK))GOTO 250
200         CONTINUE
C
C DOES NOT BELONG TO ANY LAYER (IMPOSSIBLE, BECAUSE ALL ROWS ARE
C WITHIN MODEL
C
      JJ= 0
C
C SAVE LAYER NUMBER
C
250   VEL_LAYERS(JK)=JJ
100   CONTINUE
C
C SORT ON LAYER
C
      DO 500 JK = 1 , NVELPTS - 1
         ISWITCH = 0
         LAST = NVELPTS - JK + 1
         DO 600 I = 1,LAST-1
         IF(VEL_LAYERS(I).GT.VEL_LAYERS(I+1))THEN
            ISWITCH = I+1
C
            TEMP=XVEL(ISWITCH)
            XVEL(ISWITCH)=XVEL(I)
            XVEL(I)=TEMP
C
            TEMP=YVEL(ISWITCH)
            YVEL(ISWITCH)=YVEL(I)
            YVEL(I)=TEMP
C
            TEMP=HVEL(ISWITCH)
            HVEL(ISWITCH)=HVEL(I)
            HVEL(I)=TEMP
C
            TEMP=HDEN(ISWITCH)
            HDEN(ISWITCH)=HDEN(I)
            HDEN(I)=TEMP
C
            TEMP=VVEL(ISWITCH)
            VVEL(ISWITCH)=VVEL(I)
            VVEL(I)=TEMP
C
            TEMP=VDEN(ISWITCH)
            VDEN(ISWITCH)=VDEN(I)
            VDEN(I)=TEMP
C
            ITEMP = VEL_LAYERS(ISWITCH)
            VEL_LAYERS(ISWITCH)=VEL_LAYERS(I)
            VEL_LAYERS(I)=ITEMP
C
         ENDIF
600      CONTINUE
         IF(ISWITCH .EQ. 0)GOTO 700
500   CONTINUE
C
C DEBUG
C
C 700   CALL HOGFNT(IUNIT)
C      OPEN(IUNIT,FILE='MO_PRINT',STATUS='UNKNOWN',IOSTAT=ISTAT)
C      DO 8090 JK = 1 , NVELPTS
C      WRITE(IUNIT,8091)JK,XVEL(JK),YVEL(JK),HVEL(JK),HDEN(JK),
C     X VEL_LAYERS(JK)
C 8091  FORMAT(I5,2F15.5,F10.2,F10.4,I5)
C 8090  CONTINUE
C      CLOSE(IUNIT)
C
C  ZERO OUT LAYER VEL AND DEN COUNTERS
C
700   CONTINUE
      DO 710 JK = 1 , MAX_LAYERS
      LAYERS_NVELS(JK)=0
      LAYERS_NDENS(JK)=0
      LAYERS_NVELPTS(JK)=0
710   CONTINUE
C
C COUNT VELS AND DENS FOR EACH LAYERS
C
      DO 720 JK = 1 , NVELPTS
      IF(VEL_LAYERS(JK) .NE. 0)THEN
          IF(HVEL(JK).NE.0.0)LAYERS_NVELS(VEL_LAYERS(JK))=
     X         LAYERS_NVELS(VEL_LAYERS(JK))+1
          IF(HDEN(JK).NE.0.0)LAYERS_NDENS(VEL_LAYERS(JK))=
     X         LAYERS_NDENS(VEL_LAYERS(JK))+1
      ENDIF
720   CONTINUE
C
C
C COUNT LAYERS WITH VELS AND LAYERS WITH DENS
C
      NVEL=0
      NDEN=0
      DO 730 JK = 1 , NLAYERS
      IF(LAYERS_NVELS(JK).NE.0)NVEL=NVEL+1
      IF(LAYERS_NDENS(JK).NE.0)NDEN=NDEN+1
730   CONTINUE
C
C ALL LAYERS MUST HAVE VELS
C
      IF(NVEL.NE.NLAYERS)THEN
            CALL WIGALERT(1,'MO_EMP_CALC',
     X      'All layers must be assigned velocities. ',
     X      'Please assign a velocity for all layers and move '//
     X      'ambiguous points away from the ends of pinched out layers.'
     X      )
            STATUS = 1
            RETURN
      ENDIF
C
C IF ANY DENSITIES, ALL MUST HAVE DENSITIES
C
      IF(NDEN.NE.NLAYERS)THEN
            IFDEN=0
            IF(NDEN.NE.0)THEN
              CALL WIGALERT(1,'MO_EMP_CALC',
     X        'Some layers have densities, others do not. '//
     X        'Densities will be ignored until present for all layers.',
     X      'Please assign a density for all layers and move '//
     X      'ambiguous points away from the ends of pinched out layers.'
     X      )
            ENDIF
      ELSE
            IFDEN = 1
      ENDIF
C
C BUILD A TABLE OF POINTERS FOR EACH LABEL FOR FASTER REFERENCING LATER
C
      IPREVL = -999
      IPREV_PTR = 0
      NPREV = 1
      DO 900 JK = 1 , NVELPTS
            IF(VEL_LAYERS(JK).NE.IPREVL)THEN
                  LAYERS_PTRS(VEL_LAYERS(JK))=IPREV_PTR+NPREV
                  IPREV_PTR=IPREV_PTR+NPREV
                  NPREV=1
                  LAYERS_NVELPTS(VEL_LAYERS(JK)) = 1
                  IPREVL = VEL_LAYERS(JK)
            ELSE
                  NPREV=NPREV+1
                  LAYERS_NVELPTS(VEL_LAYERS(JK))=NPREV
            ENDIF
900   CONTINUE
C
C SORT ON X FOR EACH LAYER
C
      DO 4000 MX = 1 , NLAYERS
C
        DO 5000 JK = 1 , LAYERS_NVELPTS(MX)
            ISWITCH = 0
            LAST = LAYERS_PTRS(MX) + LAYERS_NVELPTS(MX) - JK
            DO 6000 I = LAYERS_PTRS(MX),LAST-1
                IF(XVEL(I).GT.XVEL(I+1))THEN
                ISWITCH = I+1
C
                TEMP=XVEL(ISWITCH)
                XVEL(ISWITCH)=XVEL(I)
                XVEL(I)=TEMP
C
                TEMP=YVEL(ISWITCH)
                YVEL(ISWITCH)=YVEL(I)
                YVEL(I)=TEMP
C
                TEMP=HVEL(ISWITCH)
                HVEL(ISWITCH)=HVEL(I)
                HVEL(I)=TEMP
C
                TEMP=HDEN(ISWITCH)
                HDEN(ISWITCH)=HDEN(I)
                HDEN(I)=TEMP
C
                TEMP=VVEL(ISWITCH)
                VVEL(ISWITCH)=VVEL(I)
                VVEL(I)=TEMP
C
                TEMP=VDEN(ISWITCH)
                VDEN(ISWITCH)=VDEN(I)
                VDEN(I)=TEMP
C
                ITEMP = VEL_LAYERS(ISWITCH)
                VEL_LAYERS(ISWITCH)=VEL_LAYERS(I)
                VEL_LAYERS(I)=ITEMP
C
             ENDIF
6000         CONTINUE
             IF(ISWITCH .EQ. 0)GOTO 4000
5000      CONTINUE
4000  CONTINUE
C
C DEBUG
C
C      CALL HOGFNT(IUNIT)
C      OPEN(IUNIT,FILE='MO_PRINT2',STATUS='UNKNOWN',IOSTAT=ISTAT)
C      DO 9090 JK = 1 , NVELPTS
C      WRITE(IUNIT,9091)JK,XVEL(JK),YVEL(JK),HVEL(JK),HDEN(JK),
C     X VEL_LAYERS(JK),LAYERS_PTRS(VEL_LAYERS(JK)),
C     X LAYERS_NVELPTS(VEL_LAYERS(JK))
C 9091  FORMAT(I5,2F15.5,F10.2,F10.4,I5,I5,I5)
C 9090  CONTINUE
C      CLOSE(IUNIT)
C
999   RETURN
      END



C 
C  SET NOOVERFLOW CHECK FOR THE ROUTINE MO_NOISE - THIS IS TO PREVENT
C   BOMB DUE TO INTEGER OVERFLOW (ON VAX ONLY, DOES NOT OCCUR ON PRIME
C
C 12/20/88 GS - OPTIONS is non-portable - should be done in LINK
C      OPTIONS / CHECK = NOOVERFLOW


      SUBROUTINE MO_NOISE(SN,T,ILEN)
C ***************************************************************************
C
C  SUBROUTINE TO COMPUTE AND APPLY RANDOM NOISE TO REFLECTION
C  COEF. VECTOR.
C
C
C  SN=SIGNAL TO NOISE RATIO
C  T=INPUT SIGNAL VECTOR
C  ILEN=LENGTH OF INPUT VECTOR
C  THE AVERAGE NOISE AMPLITUDE = TRACE RMS/SIGNAL TO NOISE RATIO
C
C ***************************************************************************


      DOUBLE PRECISION TET
      DIMENSION T(1)
      DATA IX/1234567/
C
      IF(SN.EQ.0.0)RETURN
      TET = 0.0
      NET = 0
C
      DO 301 IS = 1,ILEN
      IF (T(IS) .EQ. 0.0) GO TO 301
      TET = TET + T(IS)*T(IS)
      NET = NET + 1
  301 CONTINUE
      IF (TET .EQ. 0.0) RETURN
      TET=TET/NET
      TRMS = DSQRT(TET)
      SD=TRMS/ABS(SN)
      DO 1 INOIS=1,ILEN
      A=0.0
      DO 50 I=1,12
      IY=IX*65539
      IF(IY)5,6,6
    5 IY=IY+2147483647+1
    6 YFL=IY
      YFL=YFL*.4656613E-9
      IX=IY
   50 A=A+YFL
      VN=(A-6.0)*SD
      T(INOIS)=T(INOIS)+VN
    1 CONTINUE
      RETURN
      END


