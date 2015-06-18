C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKUTIL.FOR
C  *6     1-MAR-1990 15:59:36 GILLESPIE "(SPR 1) Remove unused varaibles"
C  *5    19-SEP-1989 10:11:58 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *4    10-AUG-1989 18:57:17 VINCE "(SPR -1) Fortran code after UNIX mods"
C  *3    10-AUG-1989 14:48:38 ADEY "(SPR -1) UPDATE AND INSERT UK_LOGMES MESSAGES AND SURVEY MESSAGE"
C  *2     4-AUG-1989 12:10:30 ADEY "(SPR -1) UPDATE THE VERSION FOR LD_UKOOA"
C  *1     3-AUG-1989 15:13:17 ADEY "support routine for LD_UKOOA"
C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKUTIL.FOR
***********************************************************
C   06/28/89 KARL MOD TO SUBR RDPAR FOR DDDMMSS.S LAT/LONG DATA FORMAT*
C   07/01/89 KARL MOD TO SUBR READMAG FOR ABOVE
C   07/01/89 KARL MOD TO SUBR UK_LOGMES FOR BAD FORMAT STATEMENTS
C   07/06/89 KARL MOD TO SUBR UK_LOGMES FOR NEW SEISMIC ERROR MESSAGES
C   07/31/89 KARL MOD TO SUBR NAMES FOR 'UK_' PREFIX
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
*
* FUNCTION : TO READ AN UKOOA FORMAT TAPE
*
* LANGUAGE: FORTRAN
*
* PARAMETERS:  INPUT PARAMETERS =
*              *
*              LRECL  = LENGTH OF A RECORD (IN BYTES)
*              HSIZE  = NUMBER OF RECORDS IN THE HEADER
*              *
* FILES USED      : NO
*
* FILES CREATED   : NO
*
* ROUTINES USED   :
*             *
*             *             *
*
*
* TYPE:
*
* TASK IMAGE: NO
*
*
*------------------------------------------------------------
        SUBROUTINE UK_READMAG ( LRECL, HSIZE, ISTAT)
*-----------------------------------------------------------
      INCLUDE 'ld_ukooa2.i'
*
*
*
      REAL*4          XLAT, XLONG, WATDEP, XSEC
      LOGICAL         PROCESS, FSELEC, IFLPRO, IFLSEL
      INTEGER*4       HSIZE, ICOUNT, NBSEL
      INTEGER*4       JBLAN, TOTSEL, NUMSEL
      INTEGER*4       SELECT( LSELECT )
      INTEGER*4       SPNUM, NDEG, NMIN
      REAL*4          EAST, NORTH
      INTEGER*4       IN( L_REC ), OUT( NB_SP * MAXFIELD )
      REAL*4          XOUT ( NB_SP * MAXFIELD )
      EQUIVALENCE     ( OUT, XOUT )
      CHARACTER*4     IBLAN, CSELEC
      CHARACTER*20    LINE, LSTLIN
      CHARACTER*512   CIN
C
C     INT_AR is an array for intermediate storage of the fields decoded
c     on each record
C     INT_AR ( 1 )  IS ALWAYS THE SP NUMBER
C     INT_AR ( 2 )  IS ALWAYS THE X_COORD, IF LAT_LONG IS SELECTED THEN
C                        THE X_COORD IS DERIVED FROM LONGITUDE CONVERTED
C                        TO THE SPECIFIED DEFAULT PROJECTION
C     INT_AR ( 3 )  IS ALWAYS THE Y_COORD, IF LAT_LONG IS SELECTED THEN
C                        THE Y_COORD IS DERIVED FROM LATITUDE CONVERTED
C                        TO THE SPECIFIED DEFAULT PROJECTION
C     INT_AR ( 4 )  IS WATER_DEPTH IF SELECTED
C
      INTEGER*4   INT_AR  ( NB_CHOIC )
      REAL*4   XINT_AR (  NB_CHOIC )
      EQUIVALENCE ( INT_AR, XINT_AR )
      EQUIVALENCE ( INT_AR ( 1 ), SPNUM )
      EQUIVALENCE ( INT_AR ( 2 ), EAST )
      EQUIVALENCE ( INT_AR ( 3 ), NORTH ) 
      EQUIVALENCE ( INT_AR ( 4 ), WATDEP )
C
c .........indx is the index we use to fill int_ar
      INTEGER*4 INDX
C
      INTEGER*4  NBSPSTOR,DSP,DSP_OLD
C
C
      EQUIVALENCE     ( CIN,       IN(1) )
      EQUIVALENCE     ( CSELEC,    SELECT)
      EQUIVALENCE     ( JBLAN,     IBLAN )
C ......
c ......    logical TRUEREC  FALSE when we begin the process before
c ..........                  reading any record
c ........                   TRUE  once we read a record
C
      LOGICAL*4 TRUEREC,MONOTONIC
C
      INTEGER*4  ILAST_SP
C
      CHARACTER*12 FIELNAME (7)
      DATA FIELNAME/'LINE NAME','SP NUMBER','EASTING ','NORTHING',
     +'LATITUDE','LONGITUDE','WATER DEPTH'/
      DATA IBLAN      /'    '/
      DATA LSTLIN   /'99999999990000000000'/
C
C
c -----------------------------------------------------------
c      beginning of instructions
c
c ------------------------------------------------------------
c
c     compute number of fields to be decoded and stored in buffer buf
c      in order to be processed and/or stored in the data base
C
      NBFIELD = 0
C
      MONOTONIC = .TRUE.
C
C
C
      DO 5 I = 2, NB_CHOIC
5     IF ( CHOIX (I) .NE. 0 )  NBFIELD = NBFIELD + 1
C
C
C    write the parameters for decoding to the log file 
c
c
      WRITE (UNIT=LIST, FMT='(//A/)') ' DATA FIELDS DEFINITIONS'
      WRITE (UNIT=LIST, FMT='(12X,3(5X,A))') ' STARTING COLUMN',
     +'ENDING COLUMN', 'DECODE (IF NON-ZERO)'
      WRITE (UNIT=LIST, FMT='(12X,3(5X,A))') '---------------',
     +'-------------', '--------------------'
      DO 6 M =1, NB_CHOIC
      WRITE (UNIT=LIST, FMT='(X,A12,3(12X,I5))')
     + FIELNAME(M),START(M), END(M), CHOIX(M)
6     CONTINUE
C
C
C
C   NBSPSTOR  = NUMBER OF SP STORED IN BUFFER BUF
C
      NBSPSTOR = 0
C
C        MAXLINE is the maximum of lines which can be stored
C     IN BUFFER ITABL - L_TAb length of the buffer
C                       NBWLI  number of words per line name
C
C
      MAXLINE =  L_TAB / NBWLI
C
      LIN  = L_REC
      LOUT = NBFIELD * NB_SP
      ISTAT = 0
      NBW_SP = NBFIELD
      TRUEREC = .FALSE.
C
C  read the lines selected 
c
      CALL UK_READDCD (ISTAT, SELECT, FSELEC, NBSEL)
C
C    ISTAT = 2   means that the buffer containing the selection  is
c                 not big enough
c
      IF ( ISTAT . NE. 0 )   THEN
         CALL UK_LOGMES ( C41,LINE, ISTAT, I2,I3,I4 )
         IF(ISTAT.EQ.2) CALL UK_LOGMES ( C42, LINE,NBSEL,I2,I3,I4)
         CALL UK_LOGMES ( C25, LINE, I1,I2,I3,I4 )
         ISTAT = 4
         RETURN
      ENDIF
c
c     display selection of lines on the log file
c
      IF ( FSELEC )    THEN
C
         WRITE (UNIT=LIST, FMT='(/,/,A/A/(5A4,3I10))') 'LINE SELECTION',
     + '-------------', (SELECT(I), I = 1, NBSEL*8)
C
         WRITE (UNIT=LIST, FMT='(A,I4)') 'NUMBER OF LINES SELECTED  = '
     +     , NBSEL
C
      ELSE
         CALL UK_LOGMES ( C35 ,LINE, I1, I2,I3,I4 )
C
      ENDIF
c
c ... skip the header records already processed in the interactive part
C
      DO 10  I = 1, HSIZE
10    READ (UNIT=MAG, FMT='(A1)') JUNK
C
      IFLAGE = 0
      ICOUNT = 0
      ILAST_SP  = -99999
C
C ......   ERRCOUNT is a counter of errors encountered when
c .......    decoding a line .  for each error the record is dropped
c ......    when ERCOUNT reacheS MAXERR defined as a parameter
c ........     the line is dropped
C
      ERRCOUNT = 0
C
C ......    NUMBOGUS is the number of bogus lines already found in the
c ......      session . bogus line means that there was more than
c ......   maxerr of decoding in this line
c
      NUMBOGUS = 0
C
C
c ... initialisations before loop
C
      PROCESS = .FALSE.
      TOTSEL = 0

      DO 16 II = 1, L_REC
16    IN ( II ) = JBLAN
C
      CALL UK_LOGMES ( C1, LINE, I1, I2 ,I3, I4 )
*
*
********  Loop through the DATA records
*
*
1000  CONTINUE
C
C ... DECODE LINE NAME
      LINE (1:20) = ' '
      LINE(1:20) = CIN(START(1):END(1))
c
c    throw out blanks at the beginning of line name
C
      CALL UK_SHIFBLK ( LINE, C20 )
*
*  testing if end of file - if yes we exit the loop and
*   process the last line we just stored in our buffer
*
c ......if we didn't read a record  TRUEREC = .FALSE.
C
      IF ( .NOT.TRUEREC ) THEN
         TRUEREC = .TRUE.
         GO TO 2000
      ENDIF
      IF (LINE(1:3) .EQ. 'EOF' ) THEN
           IFLAGE = 1
           GO TO 9000
      ENDIF
c
c ... if line name is blank we go to next record and skip this one
C
      IF (LINE(1:20) .EQ. '                    ') THEN
         CALL UK_LOGMES ( C2 ,CIN, I1 ,I2, I3 ,I4 )
         ERRCOUNT = ERRCOUNT + 1
         GO TO 2001
      ENDIF
c
*   we test if we are jumping to another line
*    if yes we have to first store the last line if it was
*    to be stored  logical PROCESS =  .TRUE.
*    after if we had a slection we verify that we have other lines to
*     to store . if not we end the program
*     if yes we verify that NEWLINE is to be processed and
*   set logicaL PROCESS
*
      IF (LINE(1:) .NE. LSTLIN(1:)) THEN
*         ~~~~~~~~~~~~~~~~~~~~~~~~~~~
C
C   For a new line we initialize the increment between SP's to zero
C   to check monotonic variation
C
	  DSP = 0
C
          IF (PROCESS) THEN
               CALL UK_WRITMAG (OUT, XOUT, ICOUNT, LSTLIN(1:), NBFIELD,
     +                       SELECT, FSELEC, CSELEC, NBSEL, ISTAT)
C
               IF ( TOTSEL .LT. MAXLINE ) THEN

C
c ............ increment total of lines processed
C
                  TOTSEL = TOTSEL 
               ELSE
                  CALL UK_LOGMES (C43,LINE,L_TAB,I1,I2,I3 )
                  ISTAT = 7
                  RETURN
               ENDIF
c
c ............ if we are on a selection of lines we test to see
c ............ if we still have lines to process
C
               IF (FSELEC .AND. (NBSEL .EQ. TOTSEL)) GO TO 9100
c
c ............ set variables for next line process
C
               IFG = 0
               CALL UK_LOGMES ( C4, LSTLIN, I1 ,I2 ,I3,I4 )
               LSTLIN(1:) = LINE(1:)
               ICOUNT = 0
               ERRCOUNT= 0
               ILAST_SP = -99999
C
               NBSPSTOR = 0
          ENDIF
C
C
          CALL UK_LOGMES ( C3, LINE, I1 ,I2, I3,I4 )
c
c ....... test if line has already be processed in this session
C
          CALL UK_TESTPRO (LINE, IFLPRO)
          IF (IFLPRO) THEN
*         ++          ++++
c ........... display an error message saying that we are on a line
c ........... having different parts - only first part has been
c ........... treated
c
              CALL UK_LOGMES ( C5, LINE, I1 ,I2 ,I3 ,I4 )
C
              PROCESS = .FALSE.
          ELSE
*         ++++
C
c ........... we test if line selected
C
              IF (FSELEC) THEN
*             **          ****
* ............... we test if current ligne is in the selection
* ............... IF YES  PROCESS = .TRUE.
* ...............     NO  PROCESS = .FALSE.
c
                  CALL UK_SISSEL (LINE, CSELEC, NBSEL, NUMSEL,IST)
C
                  IF ( NUMSEL .EQ. 0 ) THEN
                     CALL UK_LOGMES ( C6, LINE, I1 ,I2, I3, I4 )
                     PROCESS = .FALSE.
                  ELSE
                     PROCESS = .TRUE.
                  ENDIF
              ELSE
*             ****
c ............... no selection
c ............... we store all the lines: PROCESS alwayS TRUE
C
                  PROCESS = .TRUE.
              ENDIF
C             *****
c ........... if the line is to be processed, we test the option
c ........... for completely replacing the line already existing
c ........... in the database
C
              IF (PROCESS) THEN
c ............... test if line exists in db
C
                  CALL UK_SISLIN (LINE, IFLSEL)
                  IF ((IFLSEL) .AND.
     +                (.NOT. REP_FLAG) .AND.
     +                (.NOT. COMP_FLAG)) THEN
                      PROCESS = .FALSE.
                      CALL UK_LOGMES ( C7, LINE ,I1 ,I2, I3, I4 )
                   ENDIF
              ENDIF
          ENDIF
*         +++++
      ENDIF
*     ~~~~~
C
C ... if PROCESS is TRUE, we decode the whole record
c ... decode others parameters
C
      IF (PROCESS) THEN
c
c ........    first we verify that the count of errors didn't
c ........     reach the maximum of errors if yes we drop the line
C
         IF( ERRCOUNT .GE. MAXERR )  THEN
C
            CALL UK_LOGMES ( C8, LINE, MAXERR, I2 ,I3 ,I4 )
            PROCESS = .FALSE.
c
c      verify than we didn t reach the maximum of bogus lines
c       allowed in the session this maximum is maxbogus which is
c       define in the insert file .
c      reaching the maximum can mean that the format describing
c      fields is bogus  or that the data is very bad and that we
c       MAXERR ( maximum of errors per line ) is not big enough
C
            IF ( NUMBOGUS .GT.MAXBOGUS ) THEN
               CALL UK_LOGMES ( C46, LINE, NUMBOGUS,I2,I3,I4 )
               CALL UK_LOGMES ( C25, LINE, I1,I2,I3 ,I4 )
               ISTAT = 1
               RETURN
            ENDIF
C
C
            NUMBOGUS = NUMBOGUS + 1
C
C
            ERRCOUNT = 0
C
            GO TO 2000
         ENDIF
c
C     REPLACE CODE FOR THE LAT/LONG DDDMMSS.S FORMAT BASED UPON THE 
C      CHOIX(I) VALUE: IF = 1 THEN NORMAL DECIMAL DEGREES
C                      IF = 2 THEN DDDMMSS.S FORMAT DECODED FOR DEGREES
C                                LAT/LONG
         DO 290 I = 2, 7
            I1 = START(I)
            I2 = END(I)
            IF(CHOIX(I) .EQ. 1) THEN
C
C  DECODE DECIMAL/INTEGER FORMAT
               CALL UK_DECOD(CIN, I1, I2, NUM, XNUM, ISTAT)
               IF(ISTAT .NE. 0)  GOTO 8997
            ELSE IF(CHOIX(I) .EQ.2) THEN
C
C  DECODE DEGREES, MINUTES, SECONDS FORMAT
C
C  DECODE DEGREES
               I2 = I1 + 2
               CALL UK_DECOD(CIN, I1, I2, NDEG, XNUM, ISTAT)
               IF(ISTAT .NE. 0)  GOTO 8997
C  DECODE MINUTES
               I1 = I2 + 1
               I2 = I1 + 1
               CALL UK_DECOD(CIN, I1, I2, NMIN, XNUM, ISTAT)
               IF(ISTAT .NE. 0)  GOTO 8997
C  DECODE SECONDS
               I1 = I2 + 1
               I2 = END(I) - 1
               CALL UK_DECOD(CIN, I1, I2, NUM, XSEC, ISTAT)
               IF(ISTAT .NE. 0)  GOTO 8997
C
               XNUM = NDEG/1.0 + NMIN/60.0 + XSEC/3600.0
               II = END(I)
C  REVERSE SIGN IF HEMISPHERE IS WEST OR SOUTH
               IF((CIN(II:II) .EQ. 'W') .OR. 
     -		(CIN(II:II) .EQ. 'S')) 
     +		XNUM = -XNUM

               IF((CIN(II:II) .NE. 'W') .AND. 
     -		  (CIN(II:II) .NE. 'S') .AND.
     +            (CIN(II:II) .NE. 'E') .AND.
     -            (CIN(II:II) .NE. 'N') .AND.
     +            (CIN(II:II) .NE. ' ')) THEN
                  CALL UK_LOGMES(C60,LINE,I1,I2,I3,I4)
                  GOTO 8997
               ENDIF 

            ENDIF
C
            IF((CHOIX(I) .NE.1) .AND. 
     -		(CHOIX(I) .NE. 2)) GOTO 290
C  SHOT POINT NUMBER
            IF(I .EQ. 2) SPNUM = NUM
C  EASTING
            IF(I .EQ. 3) EAST = XNUM
C  NORTHING
            IF(I .EQ. 4) NORTH = XNUM
C  LAT
            IF(I .EQ. 5) THEN 
		XLAT = XNUM
                IF(ABS(XLAT) .LE. 360) GOTO 250
                   CALL UK_LOGMES(C58,LINE,XLAT,I2,I3,I4)
                   GOTO 8997
250             NORTH = XLAT
            ENDIF
C  LONG
            IF(I .EQ. 6) THEN
                XLONG = XNUM
                IF(ABS(XLONG) .LE. 360) GOTO 270
                   CALL UK_LOGMES(C59,LINE,XLONG,I2,I3,I4)
                   GOTO 8997
270             EAST = XLONG
            ENDIF
C  WELL DEPTH - TO BE NEGATIVE VALUE
            IF(I .EQ. 7) THEN
               IF(XNUM .GT. 0.) XNUM = -XNUM
               WATDEP = XNUM
            ENDIF
            INDX = INDX + 1
290      CONTINUE
         GO TO 7998
C
8997     CONTINUE
C
c ......     an error was encountered when decoding the record
C
         CALL UK_LOGMES ( C9, CIN, I1, I2 ,I3, I4 )
         ERRCOUNT = ERRCOUNT + 1
         GO TO  2000
C
7998     CONTINUE
c
C
C    check if the previous SP stored was the same than the
c    current one - if it is so we drop  the current record
c    with a warning
c
         IF ( ILAST_SP .EQ. SPNUM )  THEN
            ERRCOUNT  = ERRCOUNT + 1
            CALL UK_LOGMES (C51, CIN, I1 ,I2, I3,I4 )
            GOTO 2000
         ENDIF
C
C
c     perform some cheecking and controls on the fields
c    decoded in the current record
C
         CALL UK_CHECKFILE ( INT_AR, XINT_AR, ISTAT )
C
c ....... update of buffer out if the coord are not screwed up
C
         IF ( ISTAT .NE. 0 )  THEN
c   something bogus in the fields decoded
C
             CALL UK_LOGMES ( C10, CIN, SPNUM ,EAST, NORTH, I4 )
             ERRCOUNT = ERRCOUNT + 1
         ELSE
C
c     testing if  buffer out  is not full
c      if yes we display an error message and don't process the line
C
            IF ( NBSPSTOR .GE. NB_SP )  THEN
               CALL UK_LOGMES ( C44, LINE ,NB_SP, I2 ,I3,I4 )
               CALL UK_LOGMES ( C26, LINE ,I2,I3,I4 ,I4)
               PROCESS = .FALSE.
            ELSE
C
C   Check monotonic variation of SP's ;
C   if non-monotonic we check if duplicate SP's exist within the same line
C
               DSP_OLD = DSP
	       IF (DSP_OLD.EQ.0) THEN
C              %%                %%%%
  	          DSP = INT_AR (1)
	          MONOTONIC = .TRUE.
	       ELSE
C              %%%%
                  IF (MONOTONIC) THEN
                     DSP = INT_AR (1) - OUT ( ICOUNT-NBW_SP+1 )
                     IF ((DSP * DSP_OLD) .LT. 0 ) MONOTONIC = .FALSE.
                  ENDIF
C   
                  IF ( .NOT. MONOTONIC ) THEN
	             DO 350  II = 1, ICOUNT, NBW_SP
                        IF (INT_AR(1) .EQ. OUT(II)) THEN
                           CALL UK_LOGMES (C52,LINE,INT_AR(1),
     -	                       I2,I3,I4)
	                   DO 555 J = 1, NBW_SP
555                        OUT(II+J-1) = INT_AR(J)
      	                   ERRCOUNT = ERRCOUNT + 1
                           GO TO 2000
		        ENDIF
350   	             CONTINUE
	          ENDIF
	       ENDIF
C              %%%%%
C
               DO 320  I = 1 ,NBW_SP
320            OUT (ICOUNT + I ) =  INT_AR ( I)
               ICOUNT = ICOUNT + NBW_SP
               ILAST_SP = SPNUM
               NBSPSTOR = NBSPSTOR + 1
            ENDIF
C
C
         ENDIF
      ENDIF
C
2000  CONTINUE
c
c ... read current data  record
C
      LSTLIN = LINE
2001  CONTINUE
      READ (UNIT=MAG, FMT='(A80)', END=8999,
     +      ERR=8998, IOSTAT=IOS) CIN
C
C
c ... go to process the record read
C
      GO TO 1000
C
C
 8998 CONTINUE
      CALL UK_LOGMES ( C11, LINE, I1,I2,I3,I4 )
      ERRCOUNT = ERRCOUNT + 1
      GO TO  2000
C
 8999 CONTINUE
c ... process the last line
C
      IFLAGE = 1
 9000 CONTINUE
      IF (IFLAGE .EQ. 1) THEN
         CALL UK_WRITMAG (OUT, XOUT, ICOUNT, LSTLIN, NBFIELD, SELECT,
     +                   FSELEC, CSELEC, NBSEL,ISTAT)
         IF (ISTAT .NE. 2) IFG = 0
C
C
c ............ increment total of lines processed
C
         TOTSEL = TOTSEL + 1
      ENDIF
C
9100  CONTINUE
C
      CALL UK_LOGMES ( C47, LINE, NEWLINE,I2,I3,I4)
      CALL UK_LOGMES ( C40, LINE, NEWMIX, NEWMIY ,NEWMAX,NEWMAY)
      CALL UK_LOGMES ( C12, LINE, I1 ,I2 ,I3 ,I4 )
C
      RETURN
      END

***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR :  REMI DORET
*
* DATE : 20/10/1986
*
* FUNCTION: PROCESS SP SELECTION, DECIMATION AND STORE IN DB
C           THE DATA  FOR A GIVEN LINE
*
* LANGUAGE: FORTRAN
*
* PARAMETERS: INPUT PARAMETERS :
*                    BUF *  BUFFER CONTAINING  THE DECODED FIELDS FOR
*     C               THE LINE
*	              XBUF    REAL BUFFER EQUIVALENT TO BUF 
*                    LBUF * LENGTH OF BUF
*                    LINE * LINE NAME
C                    NBFIELD : NUMBER OF FIELD STORED IN BUF FOR EACH SP
*                    SELECT *INTEGER*4 ARRAY OF SELECTED LINES
C                    CSELECT : CHARACTER ARRAY EQUIVALENT TO SELECT
C                    NBSEL  :  NUMBER OF LINES IN THE SELECTION
*                    FSELECT * FLAG 0=no selection
*                                       1=with selection
*                    ISTAT * RETURNED STATUS
*
* FILES USED:  NO FILE USED
*
* FILES CREATED:  NO FILES CREATED
*
* TYPE:
*
* TASK IMAGE: NO
*
*-----------------------------------------------------------------------
        SUBROUTINE UK_WRITMAG (BUF, XBUF, LBUF, LINE,
     -  NBFIELD, SELECT, FSELECT, CSELEC, NBSEL, IST)
*-----------------------------------------------------------------------
*
        INCLUDE 'ld_ukooa2.i'
*
        INTEGER*4   BUF(*)
        INTEGER*4   SELECT(*)
	REAL*4      XBUF (*)
*
        INTEGER*4   FIRSTSP, LASTSP, INC_SP, INDEB, INFIN
        INTEGER*4   NBW_SP, NBFIELD, IST, LBUF, NBPOINT
        INTEGER*4   NBSEL
*
        CHARACTER*(*)    CSELEC
        CHARACTER*(*) LINE

        LOGICAL*4  FSELECT
*
C    IF LESS THAN TWO SP IN THE LINE WE DROPPED THE LINE
C
      IF ( LBUF .LE. NBFIELD ) THEN
         CALL UK_LOGMES ( C13, LINE, I1, I2, I3, I4 )
C
C         INCREMENT NUMBER OF BOGUS  LINES
C
         NUMBOGUS = NUMBOGUS + 1
         GOTO 1000
      ENDIF

*
*
*        Initialise variables
*
*       NBPOINT : total number of sp in buffer
*       FIRSTSP : first sp selected in the line
*       LASTSP  : last sp selected in the line
*       INC_SP  : increment between sp ( we keep 1 sp every INC_SP )
*       INDEB   : index in buffer of first sp selected
*       INFIN   : index in buffer of last sp selected
*       NBW_SP  :  number of word of information by sp in the buffer
*
*
        FIRSTSP = 0
        LASTSP = 0
        INC_SP = 0
        NBW_SP = NBFIELD
        INDEB = 0
        INFIN = 0
*
*
      IF (FSELECT) THEN
c ....... we are on a selection of lines so we fill first and last sp
c ....... and increment with values of the selection
c ....... if first sp selected = 0 means we take the first in file
c ....... if last sp selected = 0 means we take the last in file
c ....... if inc sp = 0  means that we take all the sp
c
c ....... first we look for the line selected
C
          CALL UK_SISSEL (LINE, CSELEC, NBSEL, NUMSEL, IST)
C
          IF( IST.NE. 0 )  THEN
          CALL UK_LOGMES (C14 ,LINE, IST, NUMSEL, I3,I4 )
          GO TO 1001
            ENDIF
C
C
C ....... NUMSEL = 0 means that we didn't find the line
c ....... which is strange because we already did this test when
c ....... reading the records
c

          IF (NUMSEL .EQ. 0)  THEN
              GO TO 1001
          ELSE
              IND1 = (NUMSEL-1) * NBWSEL
              FIRSTSP = SELECT (IND1 + 6)
              LASTSP  = SELECT (IND1 + 7)
              INC_SP  = SELECT (IND1 + 8)
          ENDIF
      ENDIF
      NBPOINT = LBUF / NBW_SP
C
      IF (FIRSTSP .EQ. 0 ) FIRSTSP = BUF (1)
      IF (LASTSP  .EQ. 0 ) LASTSP = BUF ( (NBPOINT-1) * NBW_SP + 1)
c
c
c ... we are now going to find the useful part of the buffer
c ... which is the part selected between FIRSTSP and LASTSP
c ... we are returning INDEB and INFIN which are the indexes in the
c ... array of first and last useful record
C
      CALL UK_SELTAB (BUF, NBW_SP, NBPOINT, FIRSTSP, LASTSP,
     +             INDEB ,INFIN, IST )
C
      IF ( IST .NE. 0 ) THEN
         CALL UK_LOGMES ( C15, LINE, FIRSTSP, LASTSP, I3, I4 )
         GO TO 1001
      ENDIF
c ... now we can decimate  the array using INC_SP
c ... the decimated  sp will be stored in the same buffeR BUF
c ... froM BUF (INDEB)  for the first sp
c ... we return the number of sp after decimation
C
      CALL UK_TDECIM (BUF, INDEB, INFIN, INC_SP, NUM_DEC, NBW_SP,IST)
c
c ... the buffer has been decimated so we can now store it in the data base
c
c      convert sp numbers to float
c
      IBEGIN = ( INDEB - 1 ) * NBW_SP + 1
C
      DO 220 K = 1, NUM_DEC
         XBUF ( IBEGIN ) = FLOAT ( BUF ( IBEGIN))
         IBEGIN = IBEGIN + NBW_SP
220   CONTINUE
C      
      CALL UK_STORLIN (BUF, XBUF, INDEB, NUM_DEC, NBW_SP, LINE, IST)
      IF(IST .NE. 0) THEN
         CALL UK_LOGMES(C56, LINE, IST, BUF(1), I3, I4)
C         GOTO 1001
      ENDIF
C
 1000 CONTINUE
      IST = 0
      RETURN
C
 1001 CONTINUE
      IST = 2
      RETURN
      END
C
C ********************************************************************
***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986

C
C   ROUTINE USED TO VERIFY THAT THE LINE  LINE WAS NOT PREVIOUSLY
C      STORED IN THE DATA BASE - WE SCAN THE ARRAY OF COMMON
C      SISLINE  WHERE WERE STORED ALL THE LINES PRESENT IN
C         THE DB  WHEN THE LOADER BEGAN
C
C      LINE   :  LINE NAME
C      OUTPUT  :  IFLG  = 0 LINE NOT IN DB
C                       = 1  LINE ALREADY  IN DB
C
C *************************************************************
C
      SUBROUTINE UK_SISLIN ( LINE ,IFLG )

      INCLUDE 'ld_ukooa2.i'
C
      CHARACTER*(*) LINE
      LOGICAL*4 IFLG

C
C
*      c  looking if ligne already existing in the db
C
C     IF YES  IFLG  = 1
C     IF NO   IFLG  = 0
C
      J = 1
      IAD1 = NBCARL - 1
C
      IFLG = .FALSE.
C
      DO 100 I = 1,NB_LIG
C
             IF ( LINE .EQ. CLIGNE ( J:J+IAD1 )) THEN
                IFLG = .TRUE.
                GO TO 110
             ENDIF
         J = J + IAD1
100   CONTINUE
110    CONTINUE
C
C
C
      RETURN
      END
C
C ======================  UK_TESTPRO  ========================
C
***********************************************************
*  07/31 KARL NAME CHANGE
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C      ROUTINE USED TO TEST HAS ALREADY BEEN PROCESSED
C      WE SCAN ARRAY ITABL IN COMMON  ENTREE
C
C
C      LINE   :  LINE NAME
C      OUTPUT  :  IFLG  = 0 LINE NOT PROCESSED
C                       = 1  LINE ALREADY  PROCESSED
C
C **********************************************************
C
      SUBROUTINE UK_TESTPRO ( LINE ,IFLG )

       INCLUDE 'ld_ukooa2.i'
C
      CHARACTER*(*) LINE
      LOGICAL*4 IFLG

C
C
*      c  looking if ligne has already been processed
C
C     IF YES  IFLG  = 1
C     IF NO   IFLG  = 0
C
      J = 1
      IAD1 = NBCARL - 1
C
      IFLG = .FALSE.
C
      DO 100 I = 1,NBENT
C
             IF ( LINE .EQ. CTABL ( J:J+IAD1 )) THEN
                IFLG = .TRUE.
                GO TO 110
             ENDIF
         J = J + IAD1
100   CONTINUE
110    CONTINUE
C
C
C
      RETURN
      END

C
C ===================  UK_SISSEL ==============================
C
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C    ROUTINE LOOKING IF A LINE IS ALREADY IN THE SELECTION
C     FOR THE PROCESSING   - WE SSCAN THE ARRAY SELECT WHICH
C     CONTAINS ALL THE LINES TO PROCESS
C
C   INPUT :
C    LINE :   LINE NAME
C     SELECT :  ARRAY CONTAINING LINE TO PROCESS
C   NBSEL :  NUMBER OF LINES TO SELECT IN THE ARRAY
C   IFLSEL :  = 0   IF LINE IS NOT IN THE ARRAY
C               #0  RANK OF THE LINE IN THE ARRAY
C
C *****************************************************************
C
      SUBROUTINE UK_SISSEL (LINE, SELECT, NBSEL, IFLSEL,IST)

      INCLUDE 'ld_ukooa2.i'
C
      INTEGER*4     IFLSEL, NBSEL, INC
      CHARACTER*(*)   LINE, SELECT
C
      J = 1
      IAD1 = NBCARL - 1
      IFLG = 0
      IST = 0
      IFLSEL = 0
C
C     IINC is the increment in bytes to jump from one line name
c     to the next line name in the buffer SELECT
c     we have a total of NBWSEL words for each selection
c     number of bytes IS NBWSEL * 4
c
      INC = NBWSEL * 4
      DO 100 I = 1, NBSEL
          IF ( LINE (1:NBCARL) .EQ. SELECT ( J:J+IAD1 )) THEN
              IFLSEL = I
              GO TO 110
          ENDIF
          J = J + INC
  100 CONTINUE
  110 CONTINUE
C
      RETURN
      END
C
C ===================== UK_SELTAB ====================================
C
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986

C
C
C    INPUT  :
C    BUF    :  INTEGER*4 BUFFER CONTAINING ALL THE INFORMATIONS
C             FOR EACH SP OF THE CURRENT LINE
C             THE FIRST INFORMATION IS THE SP NUMBER
C             THE SECOND IS X COORD OR CONVERTED LONG
C             THE THIRD  IS Y COORD OR CONVERTED LAT
C             THEN WE CAN HAVE ALTI WATER DEPTH
C             WE HAVE NBW_SP WORDS(4 BYTES) OF INFORMATION FOR EACH SP
C             BUF ( 1)  IS THE FIRST SP NUMBER
C             BUF ( 1 + NBW_SP ) IS THE SECOND SP NUMBER ....
C   NBW_SP  :  NUMBER OF WORDS ( 4 BYTES) OF INFO FOR EACH SP
C   NBPOINT :  TOTAL NUMBER OF SP IN THE BUFFER
C   FIRSTSP :  FIRSTSP SELECTED IN THEBUFFER
C   LASTSP  :  LASTSP SELECTED D8IN THE BUFFER
C
C   OUTPUT  :
C   INDEB   :  INDEX IN THE BUFFER OF THE RANK OF THE FIRST SP SELECTED
C              EXEMPLE : INDEB = 1 IF BUF(1) = FIRSTSP
C                        INDEB = 2 IF BUF (1 + NBW_SP ) + FIRSTSP
C   INFIN   : RANK IN THE BUFFER OF LAST SP SELECTED
C
C
C
C
C ******************************************************************
C
      SUBROUTINE UK_SELTAB ( BUF, NBW_SP, NBPOINT, FIRSTSP, LASTSP,
     +                       INDEB, INFIN, IST )
C
C
      INCLUDE 'ld_ukooa2.i'

      INTEGER*4 BUF ( * ) ,NBW_SP ,NBPOINT, FIRSTSP, LASTSP, INDEB
      INTEGER*4 INFIN, IND1, IND2 ,I ,J, IST
C
c
c
c       looking for rank of first sp selected
c       begin scanning from beginning of buffer
c
C      WRITE (LIST,2)NBPOINT, NBW_SP,FIRSTSP, LASTSP
2       FORMAT ( /,'INPUT SELTAB NBPOINT NBW_SP FIRSTSP LASTSP',
     +  4(1X,I6))
C
      J = 1
      IND1 = 0
      IST = 0
C
      DO 10  I = 1 ,NBPOINT
C
            IF ( BUF( J ) .EQ. FIRSTSP )  THEN
                IND1 = I
                GO TO 11
            ENDIF
        J = J + NBW_SP
10       CONTINUE
C
11    CONTINUE
C
c
c    looking for rank of last sp selected
c       begin scanning from  end of  the buffer
C
      J =  ( NBPOINT - 1) * NBW_SP + 1
      IND2 = 0
C
      DO 20 I = NBPOINT, 1 ,-1
            IF ( BUF ( J ) .EQ. LASTSP )  THEN
                  IND2 = I
                  GO TO 21
            ENDIF
            J = J - NBW_SP
20    CONTINUE
C
21    CONTINUE
C
C
C    RETURN INDEB AS MIN OF IND1, IND2
C
C     RETURN INFIN AS MAX OF IND1 IND2
C
      INDEB = MIN0 ( IND1 ,IND2)
      INFIN = MAX0 (IND1 ,IND2)
C
      IF(INDEB .EQ . 0 .OR. INFIN .EQ. 0) IST = 1
C
      RETURN
      END
C
C ========================= UK_TDECI===============================
C ***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986

C
C
C
C    INPUT  :
C    BUF    :  INTEGER*4 BUFFER CONTAINING ALL THE INFORMATIONS
C             FOR EACH SP OF THE CURRENT LINE
C             THE FIRST INFORMATION IS THE SP NUMBER
C             THE SECOND IS X COORD OR CONVERTED LONG
C             THE THIRD  IS Y COORD OR CONVERTED LAT
C             THEN WE CAN HAVE ALTI WATER DEPTH
C             WE HAVE NBW_SP WORDS(4 BYTES) OF INFORMATION FOR EACH SP
C             BUF ( 1)  IS THE FIRST SP NUMBER
C             BUF ( 1 + NBW_SP ) IS THE SECOND SP NUMBER ....
C   NBW_SP  :  NUMBER OF WORDS ( 4 BYTES) OF INFO FOR EACH SP
C   INDEB   :  INDEX IN THE BUFFER OF THE RANK OF THE FIRST SP SELECTED
C              EXEMPLE : INDEB = 1 IF BUF(1) = FIRSTSP
C                        INDEB = 2 IF BUF (1 + NBW_SP ) + FIRSTSP
C   INFIN   : RANK IN THE BUFFER OF LAST SP SELECTED
C
C   INC_SP  : INCREMENT FOR DECIMATION
C             IF ( INC_SP = 0 )  NO DECIMATION
C             IF ( INC_SP = 1    WE KEEP 1 SP EACH 2 SP
C             WE ALWAYS KEEP FIRST AND LAST SP SELECTED
C
C   OUTPUT  :
C
C   NUM_DEC :  NUMBER OF SP AFTER DECIMATION
C             THE FIRST SP AS ALWAYS RANK INDEB IN BUF
C              THE OTHES ARE STORED IN " SEQUENCE "
C
C
C *********************************************************************
C
      SUBROUTINE UK_TDECIM ( BUF, INDEB, INFIN, INC_SP, NUM_DEC,
     C                   NBW_SP, IST )

C
C
      INCLUDE 'ld_ukooa2.i'
C
      INTEGER*4 BUF (*) ,INDEB ,INFIN, INC_SP ,NUM_DEC ,NBW_SP
      INTEGER*4 J1, INCR ,K ,IK, IST

C
C
      IST = 0
C
      IF ( INC_SP .EQ. 0 )   THEN
C     --                     ----
C   NO DECIMATION
C
      NUM_DEC = INFIN - INDEB + 1
C
                              ELSE
C                             ----
C    DECIMATION
C
      INCR = INC_SP + 1
      J1 = INDEB * NBW_SP + 1
C
      DO 10 K = INDEB + INCR, INFIN ,INCR
C
            IK = ( K - 1) * NBW_SP + 1
c
c     move the nbw_sp words of data to its new position in the
c     buffer
            DO 5 JK = 0, NBW_SP -1
5            BUF ( J1+JK ) = BUF ( IK + JK )
C
            J1 = J1 + NBW_SP
10    CONTINUE
c
c
c  move last sp to next position ( even if this was already done )
c   in the loop
C
      INDD = 1 + ( INFIN-1)* NBW_SP
C
      DO  20  JJ = 0, NBW_SP -1
20    BUF(J1 + JJ ) = BUF ( INDD + JJ )
c
c     compute number of sp in decimated buffer
c
      NUM_DEC = ( 1 + INFIN - INDEB ) / INCR + 1
C
      ENDIF
C     -----
C
C      WRITE (LIST,1000) INDEB, INFIN, NUM_DEC
1000     FORMAT ( /,'END DECIM INDEB OINFIN NUM_DEC ',3 (1X,I6))
C
      RETURN
      END

C ============================  UK_LOGMES ===============================
C
***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C     routine used to display all messages to a LOG FILE
C
C
C
C ************************************************************
C
C
      SUBROUTINE UK_LOGMES ( ICOD ,STRING, I1 ,I2 ,I3 ,I4 )
C
      INCLUDE 'ld_ukooa2.i'
C
      INTEGER*4 ICOD, I1,I3,I4
      CHARACTER*(*) STRING
C
      REAL*4 XI1,XI2,XI3,XI4
      INTEGER*4 J1,J2,J3,J4
      EQUIVALENCE (J1,XI1),(J2,XI2),(J3,XI3),(J4,XI4)
C
C
      J1 = I1
      J2 = I2
      J3 = I3
      J4 = I4
C
      GOTO ( 10,20,30,40,50,60,70,80,90,100,110,120,130,
     +140,150,160,170,180,190,200,210,220,230,240,250,260,
     +270,280,290,300,310,320,330,340,350,360,370,380,390,
     +400,410,420,430,440,450,460,470,480,490,500,510,520,
     +530,540,550,560,570,580,590,600,610,620 ) ,ICOD
C
C
10      CONTINUE
        WRITE ( LIST, 1010 )
1010    FORMAT(/,'*************************************************',
     +/,'*****    BEGINNING OF DATA  PROCESSING      ******',/,
     +'**************************************************',/,/)
      GOTO  9999
C
20     CONTINUE
       WRITE (LIST, 1020 ) STRING ( 1:80)
1020    FORMAT(/,'----  LINE NAME IS BLANK IN THIS RECORD..  ',
     + /,A80, '  RECORD DROPPED .....')
       GOTO  9999
C
30    CONTINUE
      WRITE ( LIST, 1030) STRING ( 1:NBCARL )
1030  FORMAT (/,'=================================================',
     +/,'== BEGINNING PROCESSING OF LINE:',A20,'  ==',/
     +,'================================================')
      GO TO 9999
C
40    CONTINUE
      WRITE ( LIST, 1040) STRING ( 1:NBCARL )
1040  FORMAT (/,'=================================================',
     +/,'==== END PROCESSING OF LINE:',A20,'  ==',/
     +,'......................................................',/
     +,'......................................................')
      GO TO 9999
C
50     CONTINUE
       WRITE ( LIST, 1050 ) STRING (1:NBCARL )
1050  FORMAT (/,'--- LINE : ', A30 ,' IS BROKEN IN SEVERAL PARTS'
     +,/, '--- ONLY FIRST PART HAS BEEN PROCESSED ;THIS PART ',
     +/,'---   IS DROPPED ; GATHER THE DIFFERENT PARTS IF YOU',
     +/,'---  WANT  THE WHOLE LINE TO BE PROCESSED ' )
      GO TO 9999
C
60    CONTINUE
      WRITE  ( LIST ,1060 ) STRING ( 1:NBCARL)
1060   FORMAT ( /,'-- LINE  : ',A30 ,' IS NOT IN YOUR SELECTION',
     +/,'---   THIS LINE IS NOT PROCESSED  ' )
      GO TO 9999
C
70    CONTINUE
      WRITE ( LIST ,1070 )
1070  FORMAT ( /,'****   LINE ALREADY IN THE DATA BASE ; ',/,
     +'***  REPLACE FLAG IS FALSE SO THE LINE IS DROPPED ',/,
     +' ****  AND THE DATA BASE LINE NOT MODIFIED' )
      GO TO 9999
C
80    CONTINUE
      WRITE ( LIST , 1080 ) I1
1080  FORMAT (/,' >>>>>>>>>>> WARNING >>>>>>>>>>>>>>>>>>',/,
     +' >>>  ALREADY  ',I6,'  BOGUS RECORDS IN THIS LINE  >>',
     +'>>>>>>>   LINE IS DROPPED         >>>>>>>>>>',/,
     +' >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>')
      GO TO 9999
C
90    CONTINUE
      WRITE ( LIST, 1090 ) STRING (I1:I2), STRING ( 1:80)
1090  FORMAT ( /,' ----  ERROR WHEN DECODING FIELD :  ',A,
     +'    OF THE ',
     +'RECORD :',/,'   ',A80,/,'---- RECORD DROPPED--' )
      GO TO 9999
C
100   CONTINUE
      WRITE ( LIST, 1100) I1, XI2, XI3, STRING ( 1:80)
1100   FORMAT (/,' SOMETHING BOGUS IN SPNUM :',I10,/,
     +'EASTING :',F12.3,'  NORTHING : ',F12.3, ' OF RECORD: '
     +,/,A80,/,'     ****   THIS RECORD IS DROPPED  ****')
      GO TO 9999
C
110   CONTINUE
      WRITE ( LIST, 1110)
1110    FORMAT ( /,'.... ERROR WHEN READING A RECORD..',
     +'  RECORD DROPPED ********* ' )
      GO TO 9999
C
120   CONTINUE
      WRITE ( LIST , 1120 )
1120  FORMAT ( /,/,/,' ************************************',
     +'*****',/,' ******   END OF LINE PROCESSING   *******',
     +/,' *********************************************')
      GO TO 9999
C
130   CONTINUE
      WRITE ( LIST, 1130 ) STRING ( 1:NBCARL )
1130   FORMAT (/,' --- LESS THAN 2 SHOT POINTS SELECTED IN LINE :',
     +A30,/,' *****   THIS LINE IS DROPPED ******' )
      GO TO 9999
C
140    CONTINUE
       WRITE ( LIST ,1140 ) STRING(1:NBCARL), I1, I2
1140   FORMAT (/,' ERROR WHEN TRYING TO STORE LINE  : ',A20,
     + /, ' ERROR UK_SISSEL  IST = ', 2(1X,I6),/,
     + ' ****   THE LINE  IS DROPPED   *****' )
           GO TO  9999
C
150   CONTINUE
C
       WRITE ( LIST, 1150 ) I1, I2, STRING ( :NBCARL )
1150   FORMAT ( /,' FIRSTSP :',I6,'OR LASTSP : ',I6 ,
     + 'BOUNDS OF THE LINE: ', A20 ,'WERE NOT FOUND ',
     + /, ' IN THE VALID RECORDS DECODED .VERIFY YOUR INPUTS ',/,
     + ' BOUNDS OR  DATA RECORDS ',/,'*** THE LINE IS DROPPED')
       GO TO 9999
C
160   CONTINUE
      WRITE  ( LIST, 1160 ) STRING ( 1:NB_CHAR )
1160   FORMAT (/,' >>>>  ALL DATA WILL BE STORED IN AREA :',A20,
     +/,' ____________________________________________________')
      GO TO 9999
C
170    CONTINUE
       WRITE ( LIST, 1170 ) I1, STRING(1:80)
1170  FORMAT ( /,' AN ORACLE ERROR HAS OCCURRED ',
     +'  ISTAT =  ', I6,/' FOR LOGIN: ',A80 )
      GO TO 9999
C
180    CONTINUE
       WRITE ( LIST, 1180 ) STRING ( 1:100)
1180  FORMAT ( /,' NAME OF DATA BASE TABLE  :',A100 )
      GO TO 9999
C
190    CONTINUE
       WRITE ( LIST, 1190 ) I1
1190  FORMAT ( /,' ERROR WHEN OPENING LINE_TABLE ',
     +'  IST =  ', I6 )
      GO TO 9999
C
200    CONTINUE
       WRITE ( LIST, 1200 ) I1
1200  FORMAT ( /,' ERROR WHEN PARSING FIELDS ',
     +'  IST =  ', I6 )
      GO TO 9999
C
210    CONTINUE
       WRITE ( LIST, 1210 ) I1
1210  FORMAT ( /,' ERROR WHEN DEFINING VARIABLES FOR FIELDS ',
     +'  IST =  ', I6 )
      GO TO 9999
C
220    CONTINUE
       WRITE ( LIST, 1220 ) I1
1220  FORMAT ( /,' ERROR WHEN SELECTING ',
     +'  IST =  ', I6 )
      GO TO 9999
C
230    CONTINUE
       WRITE ( LIST, 1240 ) STRING( 1:20), I1
1230  FORMAT ( /,' ERROR WHEN LOOKING FOR PARAMETERS OF PROJECTION  ',
     +A20,/,' WHICH IS THE DEFAULT AREA PROJECTION   IST =  ', I6 )
      GO TO 9999
C
240    CONTINUE
       WRITE ( LIST, 1240 ) STRING( 1:20), I1
1240  FORMAT ( /,' ERROR WHEN LOOKING FOR PARAMETERS OF PROJECTION  ',
     +A20,/,' WHICH IS THE UKOOA DATA PROJECTION   IST =  ', I6 )
      GO TO 9999
C
250   CONTINUE
      WRITE  ( LIST ,1250 )
1250  FORMAT(/,/,/,' ************************************************',
     +/,' ****    BATCH  JOB HAS BEEN ABORTED   ****** ',/,
     +' **************************************************')
      GO TO 9999
C
260   CONTINUE
      WRITE ( LIST ,1260 )
1260  FORMAT(/,/,' ================================================',
     +/,' ====    PROCESSING OF THE LINE IS DROPPED ===='
     +/,' ====================================================')
      GO TO 9999
C
270   CONTINUE
      WRITE ( LIST, 1270 ) I1, STRING (1:40 )
1270  FORMAT (/,' +++  ERROR  :', I6,' WHEN WRITING THE NLIST :',A40,
     +/,'YOUR DATA ON THIS LINE CAN BE CORRUPTED  ++++',/,
     +' +++++++++++++++++++++++++++++++++++++++++++++++++')
      GO TO 9999
C
280   CONTINUE
      WRITE ( LIST, 1280 ) I1, STRING ( 1:40 )
1280  FORMAT (/,' +++ ERROR : ',I6,' WHEN INITIALIZING THE  NLIST ;',
     +A40,/,'  NO NEW DATA STORED FOR THIS LINE +++++')
      GO TO  9999
C
290   CONTINUE
      WRITE ( LIST, 1290 ) I1, STRING ( 1:40 )
1290  FORMAT (/,' +++ ERROR : ',I6,' WHEN STARTING STROKE ON NLIST ;',
     +A40,/,'  NO NEW DATA STORED FOR THIS LINE +++++')
      GO TO  9999
C
300   CONTINUE
      WRITE ( LIST, 1300 ) I1, STRING ( 1:40 )
1300  FORMAT (/,' +++ ERROR : ',I6,' WHEN ADDING POINT IN NLIST ;',
     +A40,/,'  NO NEW DATA STORED FOR THIS LINE +++++')
      GO TO  9999
C
310   CONTINUE
      WRITE ( LIST, 1310 ) I1, STRING ( 1:40 )
1310  FORMAT (/,' +++ ERROR : ',I6,' WHEN UPDATING LINE RECORD'
     +,'  IN THE DATA BASE ;',
     +/,' DATA CAN BE CORRUPTED FOR  LINE :',A30,'  +++++')
      GO TO  9999
C
320   CONTINUE
      WRITE ( LIST, 1320 ) I1, STRING ( 1:40 )
1320  FORMAT (/,' +++ ERROR : ',I6,' WHEN INSERTING NEW LINE RECORD'
     +,'  IN THE DATA BASE ;',
     +/,' DATA CAN BE CORRUPTED FOR  LINE :',A30,'  +++++')
      GO TO  9999
C
330   CONTINUE
      WRITE ( LIST, 1330) I1 ,I2 ,I3,STRING (1:NBCARL)
1330  FORMAT (' *********   SUCCESS ****************',
     +/,'** ',I6,' SHOT POINTS FROM SP :',I6,'  TO SP :',
     +I6,'  STORED FOR LINE :',A20 ,/,
     +' ______________________________________________')
      GO TO 9999
C
340   CONTINUE
      WRITE  ( LIST, 1340 ) I1
1340  FORMAT ( /,' ERROR WHEN LOOKING FOR THE DEFAULT PROJECTION',
     +'  IN THE AREA  STATUS = ',I6,/,'   CHECK YOUR AREA ',
     + '  PROJECTION  ' )
       GO TO 9999
C
350   CONTINUE
      WRITE ( LIST, 1350 )
1350  FORMAT (/,' <<<< ALL THE LINES IN THE UKOOA FILE HAVE BEEN ',
     +'SELECTED >>>>',/)
      GO TO 9999
C
360    CONTINUE
       WRITE (LIST, 1360 ) STRING ( 1:11)
1360  FORMAT ( /,' --- THE DEFAULT PROJECTION FOR THE AREA IS :',A11)
      GO TO 9999
C
370   CONTINUE
      WRITE ( LIST, 1370 ) STRING ( 1:11 )
1370  FORMAT (/,' ---THE DATA PROJECTION ON THE UKOOA FILE IS :',A11)
      GO TO 9999
C
380   CONTINUE
      WRITE ( LIST, 1380 ) I1
1380  FORMAT ( /,' ++++ ERROR :',I6,' WHEN CONVERTING COORDINATES')
      GO TO 9999
C
390   CONTINUE
      WRITE ( LIST, 1390 ) STRING(1:20),XI1,XI2,XI3,XI4
1390  FORMAT (/,' *     IN SURVEY :',A20,/,/
     +,' MINIMUM X_COORD ON THE LINE IS :',F15.2,/
     +,' MINIMUM Y_COORD ON THE LINE IS :',F15.2,/
     +,' MAXIMUM X_COORD ON THE LINE IS :',F15.2,/
     +,' MAXIMUM Y_COORD ON THE LINE IS :',F15.2,/)
      GOTO 9999
C
400   CONTINUE
      WRITE ( LIST, 1400 ) XI1,XI2,XI3,XI4
1400  FORMAT (/,/,' MINIMUM X_COORD ON THE LOADED LINES IS :',
     + F15.2,/
     +,' MINIMUM Y_COORD ON THE LOADED LINES IS :',F15.2,/
     +,' MAXIMUM X_COORD ON THE LOADED LINES IS :',F15.2,/
     +,' MAXIMUM Y_COORD ON THE LOADED LINES IS :',F15.2,/)
      GOTO 9999
C
410   CONTINUE
      WRITE ( LIST, 1410 ) I1
1410  FORMAT ( /,' ERROR WHEN READING SELECTION OF LINES :',I6)
      GO TO 9999
C
420   CONTINUE
      WRITE ( LIST, 1420 ) I1
1420  FORMAT (/,' NOT ENOUGH SPACE IN BUFFER SELECT TO STORE THE',
     +I6,'  LINES SELECTED ',/,'  THE PROGRAM NEEDS TO BE ',
     +' RECOMPILED INCREASING THE  PARAMETER LSELECT',/,
     +' WHICH IS THE SIZE OF THE BUFFER SELECT ' )
      GO TO 9999
C
430   CONTINUE
      WRITE ( LIST, 1430 ) I1
1430  FORMAT (/,' NOT ENOUGH SPACE IN BUFFER ITABL TO STORE THE',
     +I6,'  LINES PROCESSED ',/,'  THE PROGRAM NEEDS TO BE ',
     +' RECOMPILED INCREASING THE  PARAMETER L_TAB',/,
     +' WHICH IS THE SIZE OF THE BUFFER ITABL ' )
      GO TO 9999
C
440   CONTINUE
      WRITE ( LIST, 1440 ) I1
1440  FORMAT (/,' NOT ENOUGH SPACE IN BUFFER OUT TO STORE MORE THAN',
     +I6,'  SP ALREADY PROCESSED ',/,'  THE PROGRAM NEEDS TO BE ',
     +' RECOMPILED INCREASING THE  PARAMETER NB_SP',/,
     +' TO BE ABLE TO PROCESS ALL THE SP FOR THIS LINE ' )
      GO TO 9999
C
450   CONTINUE
      WRITE ( LIST, 1450 ) I1
1450  FORMAT (/,' NOT ENOUGH SPACE IN BUFFER LIGNE TO STORE MORE THAN',
     +I6,'LINES ALREADY FOUND IN THE DB',/,'  THE PROGRAM NEEDS TO BE ',
     +' RECOMPILED INCREASING THE  PARAMETER L_TAB',/,
     +' TO BE ABLE TO STORE IN COMMON SISLINE ALL THE LINES ',/,
     +'  PREASENT IN THE DB WHEN BEGINNING TO LOAD NEW LINES ' )
      GO TO 9999
C
460   CONTINUE
      WRITE ( LIST, 1460 ) I1
1460  FORMAT (/,' ***  ALREADY ',I6,'  BOGUS LINES ENCOUNTERED IN',
     +' THIS SESSION '/,'   VERIFY THAT YOUR COLUMN NUMBERS ',
     +' DESCRIBING THE FIELDS ARE OK  ',/,' OR VERIFY THAT ',
     +' THE MAXIMUM OF ERRORS YOU ALLOW PER LINE IS BIG ENOUGH')
      GO TO 9999
C
470   CONTINUE
      WRITE ( LIST, 1470 ) I1
1470  FORMAT (/,/,' <<<<<<<   ',I6,'  LINES HAVE BEEN PROCESSED',
     +'  AND STORED IN THE DATA BASE >>>>>>')
      GO TO 9999
C
480   CONTINUE
      WRITE ( LIST, 1480 )
1480  FORMAT ( /,' *****     ERROR WHEN INITIALISING LOGICAL UNITS**'
     +,/,' ***    VERIFY THE NAMES OF DATA, PARAM AND LOG FILE***')
      GO TO 9999
C
490   CONTINUE
      WRITE ( LIST, 1490)
1490  FORMAT (/,' **** ERROR WHEN READING PARAMETER FILE *****',
     +/,' ****   CHECK YOUR PARAMETERS IN THE COMMAND FILE **')
      GO TO 9999
C
500   CONTINUE
      WRITE ( LIST, 1500)STRING(1:20),I1,STRING(21:50),I2,I3
1500  FORMAT (/,' THE NUMBER OF CHARACTERS IN LINENAME : ',A20,
     -  '   IS ', I6,/,
     -     ' THE NUMBER OF CHARACTERS IN SURVEY NAME : ',A20,
     -  '   IS ', I6,/,
     - ' THE TOTAL OF THIS TWO NUMBERS IS GREATER THAN : ',I6,
     - /,' WHICH IS THE MAXIMUM FOR IDENTIFYING A LINE WITH ITS',
     - /, ' LINENAME + ITS SURVEY NAME' ,/,
     - ' YOU NEED TO SHORTEN EITHER THE SURVEY OR THE LINE NAME',
     - /,' *****    THIS LINE IS DROPPED  *******' )
C
      GOTO 9999
C
510    CONTINUE
      WRITE ( LIST, 1510 )  STRING ( 1:80)
1510  FORMAT ( /, '--- IN THE RECORD :  ',A, /,
     + '  THE SP IS DUPLICATED  ',/,
     +' ------ THE RECORD IS DROPPED  ---------  '  )
      GO TO 9999
C
520    CONTINUE
      WRITE ( LIST, 1520 )  STRING ( 1:20), I1
1520  FORMAT ( /,' --- FOR LINE NAME :  ',A20, /,
     + '  DUPLICATE SHOT POINTS ENCOUNTERED, SP = ',I6,/,
     + '  ONLY LAST OCCURENCE OF THIS SP IS VALIDATED ' )
      GO TO 9999
C
530    CONTINUE
      WRITE ( LIST, 1530 )  STRING ( 1:40),I1
1530  FORMAT ( /,' --- ERROR DETECTED IN NLIST: ',A40, /,
     +'  DUPLICATE CDP = ',I6,' FOR DIFFERENT X AND Y COORDINATES ',/,
     +'  WHEN TRYING TO OPTIMIZE LINE IN OPTM_XY: ',/,
     +'  CHECK FOR INCONSISTENT SP, X AND Y IN YOUR DATA FILE. ' )
      GO TO 9999
C KPA 7/7/89
540    CONTINUE
      WRITE ( LIST, 1540) STRING( 1:60), I1
1540  FORMAT (/,' ---ERROR - COULD NOT GENERATE NL_FILE NAME'
     +' FOR SURVEY:',/,A60,/,'HO_ADD_PATH STATUS = ',I6,/)
      GOTO 9999
C
550    CONTINUE
      WRITE ( LIST, 1550) STRING(1:20)
1550  FORMAT (//,' ****  A SURVEY NAME MUST BE SUPPLIED!!!',/,
     +'      SURVEY SUPPLIED IS BLANK: ',A20,/)
C
560    CONTINUE
      WRITE ( LIST, 1560) I1, STRING(1:20), I2
1560  FORMAT (//,' *** ',I6,' ERROR STATUS FROM UK_STORLIN',
     +' FOR LINE: ',A20,/' SHOTPOINT NUMBER ',I6,/)   
      GOTO 9999
C
570   CONTINUE
      WRITE ( LIST, 1570 ) I1, STRING ( 1:40 )
1570  FORMAT (/,' +++ ERROR : ',I6,' WHEN READING LINE RECORD'
     +,'  IN THE DATA BASE ;',
     +/,' DATA CAN BE CORRUPTED FOR LINE :',A40,'  +++++')
      GO TO  9999
C
580   CONTINUE
      WRITE ( LIST, 1580 ) STRING ( 1:40 ), XI1
1580  FORMAT (/,' +++ ERROR WHEN DECODING DDDMMSS.SH LATITUDE'
     +,' IN LINE:',A40,
     +/,' DATA DECODED FOR LAT.:',F15.3,'  +++++')
      GO TO  9999
C
590   CONTINUE
      WRITE ( LIST, 1590 ) STRING ( 1:40 ), XI1
1590  FORMAT (/,' +++ ERROR WHEN DECODING DDDMMSS.SH LONGITUDE'
     +,' IN LINE:',A40,
     +/,' DATA DECODED FOR LONG.:',F15.3,'  +++++')
      GO TO  9999
C
600   CONTINUE
      WRITE ( LIST, 1600 ) STRING ( 1:40 )
1600  FORMAT (/,' +++ ERROR WHEN DECODING DDDMMSS.SH FORMAT'
     +,' IN LINE:',A40,
     +/,' HEMISPHERE DECODED FOR LINE IS INCORRECT  +++++')
      GO TO  9999
C
610   CONTINUE
      WRITE ( LIST, 1610 ) 
1610  FORMAT (/,' UPDATE :')
      GOTO 9999
C
620   CONTINUE
      WRITE ( LIST, 1620 ) 
1620  FORMAT (/,' INSERT :')
      GOTO 9999
C
9999  CONTINUE
      RETURN
      END

C
C ========================   UK_CHECKFILE =============================
C
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C
C
C       ROUTINE USED TO CONTROL THAT THE FIELDS DECODED ARE
C      NO "PROBLEMS" IF YES AN ERROR STATUS IS RETURNED
C
C
C
C    INT_AR IS AN ARRAY FOR INTERMEDIATE STORAGE OF THE FIELDS DECODED
C     ON EACH RECORD
C     INT_AR (1 )   IS ALWAYS THE SP NUMBER
C     INT_AR ( 2 )  IS THE X_COORD OR THE CONVERTED LONG
C     INT_AR ( 3)   IS THE Y_COORD OR THE CONVERTED LAT
C     INT_AR ( 4 )  IS THE WATER_DEPTH IF SELECTED
C
C      XINT_AR IS  A REAL ARRAY EQUIVALENT TO  INT_AR
C
C    OUTPUT  :
C     ISTAT   = 0   IF ALL CONTROLS ARE OK
C             = 1   IF ERROR DETECTED IN THE FIELDS CONTROLS
C
C *****************************************************************

      SUBROUTINE UK_CHECKFILE ( INT_AR, XINT_AR, ISTAT )
C
      INTEGER*4 INT_AR (*)
      INTEGER*4 ISTAT
      REAL*4  XINT_AR (*)
C
C
      ISTAT = 0
C
C    TESTING THAT  X AND Y COORD ARE DIFFERENT
C       FROM ZERO
C      IF  ONE IS EQUAL 0 THE RECORD IS BOGUS
C
      ISTAT = 1
C   EASTING AND NORTHING CHECK
      IF (( XINT_AR(2) .NE. 0.) .AND.
     + ( XINT_AR(3) .NE. 0.))  ISTAT = 0
C
C     OTHERS CONTROLS AND CHECKING CAN BE ADDED IF OTHER FIELDS ARE
C        SELECTED
C
        RETURN
        END
C ***********************************************************
C

C
C PURPOSE:
C
C    READ PARAMETERS FILE AND INITIALIZE CERTAIN VARIABLES
C
C PARAMETERS:
C
C    HSIZE     I*4       NUMBER OF HEADER RECORDS
C    LRECL     I*4       DATA RECORD LENGTH
C
C    ISTAT     I*4     STATUS  = 0  NO ERROR
C                               #0   ERROR
C AUTHOR:
C
C    GEORGE S. DOBEK, 10/9/86
C
C NOTES:
C
C    IN ADDITION TO READING AND INITIALIZING THE ABOVE VARIABLES, THIS
C    ROUTINE INITIALIZES THE AREA NAME, SURVEY NAME AND PROJECTION CODE
C    IN COMMON BLOCK /C_AREA/.
C
C ******************************************************************
C
      SUBROUTINE UK_RDPAR (HSIZE, LRECL,ISTAT)
C
      INTEGER*4     HSIZE, LRECL
      INTEGER       TFNBLEN
      INTEGER*4     ISTAT
C
      INCLUDE 'ld_ukooa2.i'
C
      ISTAT = 0
c
c ... get project name and survey name.
C
      READ (UNIT=DISK, FMT='(A20)', REC=2,
     +      ERR=998, IOSTAT=IOS) AREANAME
      READ (UNIT=DISK, FMT='(A30)', REC=3,
     +      ERR=998, IOSTAT=IOS) SURV_NAM
c
c  look for number of characters in the survey name 
c
            ILSURV = TFNBLEN ( SURV_NAM)
C
C     pass survey name in upper character
c
            CALL TSUPPER ( SURV_NAM )
c
c     look for number of characters in the project name
c
            NB_CHAR = TFNBLEN ( AREANAME )
c
c      pass project name in upper character
c
c
           CALL TSUPPER ( AREANAME )      
C
c ... get number of header records and record length
c
      READ (UNIT=DISK, FMT='(I10)', REC=4,
     +      ERR=998, IOSTAT=IOS) HSIZE
      READ (UNIT=DISK, FMT='(I10)', REC=5,
     +      ERR=998, IOSTAT=IOS) LRECL
C
c ... get maximum allowable errors per seismic line
C
      READ (UNIT=DISK, FMT='(I10)', REC=6,
     +      ERR=998, IOSTAT=IOS) MAXERR
C
c ... get projection code
C
      DATAPROJ = ' '
      READ (UNIT=DISK, FMT='(A12)', REC=7,
     +      ERR=998, IOSTAT=IOS) DATAPROJ
c
c      pass projection code in upper character
c
c
           CALL TSUPPER ( DATAPROJ )      
C
c ... fill start and end arrays.
C
      READ (UNIT=DISK, FMT='(I10)', REC=1,
     +      ERR=998, IOSTAT=IOS) NB1
C
C   FILL ARRAY CHOIC START END WITH ZEROES
C
            DO 8 KK = 1,NB_CHOIC
            START(KK) = 0
            END (KK) = 0
            CHOIX(KK) = 0
8       CONTINUE
C
C
      J = 1
      DO 10 I = 8, NB1
          READ (UNIT=DISK, FMT='(I4,2X,I4,2X,I4)', REC=I,
     +          ERR=998, IOSTAT=IOS) START(J), END(J), CHOIX(J)
          J = J + 1
   10 CONTINUE
c
c ... get flag values: rep_flag, comp_flag
C
      READ (UNIT=DISK, FMT='(I10)', REC=NB1+3,
     +      ERR=998, IOSTAT=IOS) REP_FLAG
C
C ... TEMPORARILY DISABLED --- GSD 10/27/86
C     READ (UNIT=DISK, FMT='(I10)', REC=NB1+4,
C    +      ERR=998, IOSTAT=IOS) COMP_FLAG
C
C    CHECK THAT SURVEY IS FILLED OR SEPARATED OUT OF LINE
C
            IF(ILSURV .EQ. 0 .AND. CHOIX(1) .LE. 1) THEN
               CALL UK_LOGMES(C55, SURV_NAM, 0,0,0,0)
               GOTO 998
            ENDIF
C
      GO TO 999
C
  997 CONTINUE
      PRINT *, 'Premature End of File!'
      ISTAT = 2
      GO TO 999
C
  998 CONTINUE
      PRINT *, 'UK_RDPAR===> Error Code = ', IOS
C
      ISTAT = 1
  999 CONTINUE
      RETURN
      END
C
C ********************************************************
C
C


C
C PURPOSE:
C
C    INITIALIZE THE LOGICAL UNIT COMMON BLOCK
C
C PARAMETERS:
C     ISTAT  :  ERROR STATUS  = 0  NO ERROR
C                             #0   ERROR
C
C
C AUTHOR:
C
C    GEORGE S. DOBEK, 10/9/86
C
c     modif   R DORET   1/2/87
C NOTES:
C
C    THE 3 LOGICAL UNITS (MAG, DISK, LIST) ARE SET TO VALID LOGICAL
C    UNIT NUMBERS.
C
C *****************************************************************
C
      SUBROUTINE UK_INITLU( ISTAT)
C
      INCLUDE 'ld_ukooa2.i'
C
      INTEGER*4     POS
      CHARACTER*80  NAME, CHECK
      INTEGER*4   ISTAT
      INTEGER*4   STDIN, HO_GET_FORTRAN_STDIN
C
      ISTAT = 0
C 12/20/88 GS Don't use hard-coded terminal input unit number
      STDIN = HO_GET_FORTRAN_STDIN()
c
C ... read data file name and open iT on logical unit MAG
c
C
CCCCCCCCCCCCC       ATTENTION       CCCCCCCCCCCCCCCCCCCCCCC
C
C    IN THE VAX VERSION THE DATA FILE IS OPENED IN 
C    READONLY MODE TO SIMPLIFY PROTECTION  PROBLEM
C
C    THIS IS NOT PORTABLE
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C                                         
      MAG = 14
      NAME = ' '
   10 READ (UNIT=STDIN, FMT='(A80)') NAME
      POS = 1
      CALL FFASC (NAME, 80, POS, CHECK, 2)
      IF (CHECK(1:2) .EQ. '/*') GO TO 10
      OPEN (UNIT=MAG, FILE=NAME, ERR=998, STATUS= 'OLD',
     -   IOSTAT=IOS)
c
C ... read parameter file name and open iT on logical unit DISK
c
      DISK = 15
   20 READ (UNIT=STDIN, FMT='(A80)') PFNAM
      POS = 1
      CALL FFASC (PFNAM, 80, POS, CHECK, 2)
      IF (CHECK(1:2) .EQ. '/*') GO TO 20
      OPEN (UNIT=DISK, FILE=PFNAM, STATUS='OLD', ACCESS='DIRECT',
     +      FORM='FORMATTED', RECL=80, IOSTAT=IOS, ERR=998)
C
C ...  OPEN LIST ON UNIT 1 WHICH IS THE TERMINAL IF WE ARE
C       IN INTERACTIVE SESSION OR THE 'COMOFILE' IF WE ARE IN
C       A BATCH SESSION
C
C       LIST = 7
C
C **********    WARNING *********************
C    THIS PART CAN BE REACTIVATED  IF WE NEED TO HAVE A LOG LISTING
C      DIFFERENT FROM THE COMOFILE
C      IN THIS CASE THE LOG FILE WILL BE READ FROM THE COMMAND FILE
C      AND ASSIGN TO THE FORTRAN UNIT 16
C       CORRESPONDING TO PRIMOS FUNIT 12
C
CCCCCCCCCCCCC       ATTENTION       CCCCCCCCCCCCCCCCCCCCCCC
C
C    IN THE VAX VERSION THE DATA FILE IS OPENED IN 
C    SHARED MODE TO BE ABLE TO READ IT DURING EXECUTION OF THE
C    PROGRAM
C
C    THIS IS NOT PORTABLE
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
c
c       read log file name and open it on logical unit LIST
c
        LIST = 16
 30     READ (UNIT=STDIN, FMT='(A80)') NAME
       POS = 1
       CALL FFASC (NAME, 80, POS, CHECK, 2)
       IF (CHECK(1:2) .EQ. '/*') GO TO 30
       OPEN (UNIT=LIST, FILE=NAME, ERR=998, 
     - STATUS ='UNKNOWN' ,IOSTAT=IOS)
C
C ***********************************************************
C
C
c
c   read the ORACLE  login string and store it in the common LOG_STR
c
       READ (UNIT=STDIN, FMT='(A80)') LOG_STR (1:80)

C
      RETURN
C
  998 CONTINUE
      PRINT *, 'UK_INITLU===> Error Code = ', IOS
      PRINT *,' VERIFY DATA FILENAME, PARAMETER FILE NAME '
      PRINT *,'  BEFORE SUBMITTING THE COMMAND FILE   **** '
C
      ISTAT = 1
      RETURN
      END
C
C *******************************************************************
C
C


C
C PURPOSE:
C
C    READ PARAMETERS FILE AND INITIALIZE LINE SELECTION VARIABLES
C
C PARAMETERS:
C
C    ISTAT     I*4       ERROR STATUS: 0     NO ERROR
C                                      1     ERROR
C    SELECT    I*4       ARRAY OF LINE SELECTION INFORMATION. EACH GROUP
C                        OF 8 WORDS INCLUDES:
C                             LINE NAME                     (5 WORDS)
C                             FIRST SHOT POINT OF LINE      (1 WORD)
C                             LAST SHOT POINT OF LINE       (1 WORD)
C                             NUMBER OF SHOT POINTS TO SKIP (1 WORD)
C    ISEL      L*4       SELECTION FLAG: .FALSE.   NO SELECTION
C                                        .TRUE.    SELECTION
C    NBSEL     I*4       NUMBER OF LINES SELECTED
C
C AUTHOR:
C
C    GEORGE S. DOBEK, 10/10/86
C
C *****************************************************************
C
      SUBROUTINE UK_READDCD (ISTAT, SELECT, ISEL, NBSEL)
C
C
      INTEGER*4     ISTAT, NBSEL
      INTEGER*4     SELECT(*)
      LOGICAL*4     ISEL
C
      INCLUDE 'ld_ukooa2.i'
C
C ... get and set selection flag variable
C
      READ (UNIT=DISK, FMT='(I10)', REC=NB1+2,
     +      ERR=998, IOSTAT=IOS) I
      ISEL = .TRUE.
      IF (I .EQ. 0) ISEL = .FALSE.
c
c ... get number of selected lines
C
      READ (UNIT=DISK, FMT='(I10)', REC=NB1+4,
     +      ERR=998, IOSTAT=IOS) NBSEL
c
c      testing if buffer select big enough to store all
c      the lines selected by the user
C
            IF ( NBSEL * NBWLI .GT. LSELECT ) THEN
            ISTAT = 2
            RETURN
            ENDIF
C
C
c ... fill select array
C
      IREC = NB1+5
      DO 10 I = 1, NBSEL
          READ (UNIT=DISK, FMT='(5A4,3(2X,I10))', REC=IREC,
     +          ERR=998, IOSTAT=IOS) (SELECT(K), K = I*8-7, I*8)
          IREC = IREC + 1
   10 CONTINUE
      GO TO 999
C
  998 CONTINUE
      PRINT *, 'UK_READDCD===> Error Code = ', IOS
      ISTAT = 1
      RETURN
C
  999 CONTINUE
      ISTAT = 0
      RETURN
      END

C ========================   UK_SHIFBLK =============================
C
***********************************************************
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C
C
C       ROUTINE USED TO STRIP OUT BLANKS AT THE BEGINNING OF A
C          CHARACTER STRING
C
C     INPUT     :  STRING     CHARACTER STRING
C                   LEN       NB OF CHARACTERS IN THE STRING
C     OUTPUT    :
C                  THE CHARACTER STRING HAS BEING SHIFTED BY REMOVING
C                    THE BLANKS AT THE BEGINNING IF THERE WERE SOME
C
C *****************************************************************
C
       SUBROUTINE UK_SHIFBLK ( STRING, LEN )
C
       INTEGER*4 LEN
       CHARACTER*(*)    STRING

c   look for first non blank character
c
      DO 10 I=1,LEN
      IF ( STRING (I:I) .NE. ' ' ) GO TO 20
10      CONTINUE
C
c        the string is blank
C
      GOTO 99
20    CONTINUE
C
c      remove blanks from the beginning of the line name
c      if i = 1 nothing to do first character is not blank
C
      IF ( I .GT. 1) THEN
      INB = LEN - I + 1
      STRING ( 1 : INB ) = STRING ( I : LEN )
      STRING ( INB + 1:LEN) =' '
      ENDIF
C
99    CONTINUE
      RETURN
      END


C
C ***************************************************************
C ====================  UK_CONVPROJ ============================
***********************************************************
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C   ROUTINE USED TO CONVERT COORDINATES FROM THE DATA PROJECTION TO 
C     THE PROJECT DEFAULT PROJECTION
C
C
C    INPUT  :
C    BUF    :  INTEGER*4 BUFFER CONTAINING ALL THE INFORMATIONS
C              FOR EACH SP OF THE CURRENT LINE
C              THE FIRST INFORMATION IS THE SP NUMBER
C              THE SECOND IS X COORD OR THE CONVERTED LONG
C              THE THIRD  IS Y COORD OR THE CONVERTED LAT
C              THEN WE CAN HAVE ALTI WATER DEPTH
C              WE HAVE NBW_SP WORDS(4 BYTES) OF INFORMATION FOR EACH SP
C              BUF ( 1)  IS THE FIRST SP NUMBER
C              BUF ( 1 + NBW_SP ) IS THE SECOND SP NUMBER ....
c   XBUF    :  REAL BUFFER EQUIVALENT TO BUFFER BUF
C   NBW_SP  :  NUMBER OF WORDS ( 4 BYTES) OF INFO FOR EACH SP
C   NUM_DEC :  NUMBER OF SP TO STORE
C
C *******************************************************************

C
      SUBROUTINE UK_CONVPROJ  ( BUF, XBUF, NUMBER, NBW_SP, IST )
C
      INCLUDE 'ld_ukooa2.i'
C
      INTEGER*4 IST
      REAL*8 XY_IN(2), XY_OUT(2)
C
      REAL*4 XBUF (*)
      INTEGER*4 BUF (*)
       INTEGER*4 NUMBER, NBW_SP

C
C     IF LOGICAL PROJCONV is true we convert the xy coord in the
c       default projection for the project
C      IF PROJCONV IS FALSE NOTHING TO DO 
C
      IST = 0
C
C  ICUR is the current index in the array buf
c   it is initialised to point to the x coord of the first to process
C
	ICUR = 2

      IF (PROJCONV ) THEN
C
	DO  100 I = 1, NUMBER 

      XY_IN (1) = DBLE (XBUF ( ICUR ))
      XY_IN (2) = DBLE (XBUF ( ICUR + 1))


      CALL GTP$0(XY_IN(1),XY_IN(2),DA_TYPE,DA_ZONE,DA_PARAM,
     +DA_UNIT,DA_SPHE,
     +XY_OUT(1),XY_OUT(2),AR_TYPE,AR_ZONE,AR_PARAM,AR_UNIT,
     +AR_SPHE,IST)
C

      XBUF ( ICUR ) =SNGL( XY_OUT (1) )
      XBUF ( ICUR + 1) = SNGL (XY_OUT (2))
C
	ICUR = ICUR + NBW_SP
100      CONTINUE
C
        ENDIF
C
      RETURN
      END
C
C ====================  UK_ENVEL_F ============================
***********************************************************
*
* AUTHOR:REMI DORET
*
* DATE : 20/10/1986
C
C  ROUTINE USE TO COMPUTE THE ORIENTED RECTANGLE ENCLOSING 
C     THE LINE WHOSE XY COORD ARE STORED IN THE BUFFER 
C
C
C
C    INPUT  :
C    XBUF    :  REAL*4 BUFFER CONTAINING ALL THE INFORMATIONS
C             FOR EACH SP OF THE CURRENT LINE
C             THE FIRST INFORMATION IS THE SP NUMBER IF THERE ARE
C             MORE THAN 2 WORDS PER SP
C             THE SECOND IS X COORD OR CONVERTED LONG
C             THE THIRD  IS Y COORD OR CONVERTED LAT
C             THEN WE CAN HAVE ALTI WATER DEPTH
C             WE HAVE NBW_SP WORDS(4 BYTES) OF INFORMATION FOR EACH SP
C             BUF ( 1)  IS THE FIRST SP NUMBER
C             BUF ( 1 + NBW_SP ) IS THE SECOND SP NUMBER ....
C   NBW_SP  :  NUMBER OF WORDS ( 4 BYTES) OF INFO FOR EACH SP
C  NUMBER   :  NUMBER OF SP TO PROCESS
C  XD_LONG  :  DISTANCE TO ADD ALONG THE RECTANGLE AXIS
C               FOR COMPUTING COORDINATES OF THE ENVELOPPE
C  XD_LARG  :  DISTANCE TO ADD PERPENDICULAR TO THE RECTANGLE AXIS
C               FOR COMPUTING COORDINATES OF THE ENVELOPPE
C
C  OUTPUT 
C
C  RECT_ENV : BUFFER CONTAINING THE FOUR PAIRS OF (X,Y) COORD
C             OF THE CORNER OF THE RECTANGLE ENCLOSING THE LINE
C
C  XAXE_END : REAL ARRAY CONTAINING THE COORDINATES OF THE ENDS OF THE
C             AXIS OF THE RECTANGLE ENCLOSING THE LINE
C             XAXE_END (1)    X_COORD OF FIRST END
C             XAXE_END (2)    Y_COORD OF FIRST END
C             XAXE_END (3)    X_COORD OF SECOND END
C             XAXE_END (4)    Y_COORD OF SECOND END
C
C *******************************************************************

C
      SUBROUTINE UK_ENVEL_F( XBUF, NUMBER, NBW_SP, XD_LONG, XD_LARG,
     -                  RECT_ENV, XAXE_END  )
C
C
       INCLUDE 'ld_ukooa2.i'
C
       REAL*4 XBUF (*), XAXE_END (*)
       REAL*8 XD_LONG, XD_LARG
       REAL*8   RECT_ENV  (*)
       INTEGER*4 NUMBER, NBW_SP
       REAL*8  XDEB, YDEB, XFIN, YFIN, DIST	
       INTEGER*4 INDEX		     
       REAL*8 XPROJ, YPROJ, XT, YT ,DPROJ,XAL, XAL2, XAL3 
       REAL*8 XCOEF, XCOEF_ABS, X_ATAN, XINC1, XINC2, YINC1, YINC2
       INTEGER*4 I 	
       LOGICAL HORIZ
C
C     (XDEB, YDEB ) AND ( XFIN, YFIN ) are the coordinates of the two
c     points limiting the " axis" of the rectangle 
c     these two points are set to the first and last points of the line
c     at the beginning of the algorithm
c
c     then the slope of the axis is not going to change
c      
c      for each point of the line we compute the orthogonal projection
c      on the axis
c      if this projection is inside the axis segment we don't change the 
c      limit of the segment 
c      if this projection is outside the axis we change the limits to 
c      include the projection point 
c
c
C    NBW_SP  greater  than 2 means that we are on a buffer of the form
c    sp, x, y ,...
c    NBW_SP = 2   means that we just have  x,y couples
c
        IF ( NBW_SP .GT. 2 ) THEN
           INDX1 = 2
                             ELSE
           INDX1 = 1
        ENDIF
C
	XDEB = DBLE ( XBUF ( INDX1 ) )
	YDEB = DBLE ( XBUF ( INDX1 + 1 ) ) 
C
C    INDEX is the index in the array of the last point of the line
C
	INDEX = ( NUMBER - 1) * NBW_SP + INDX1

	XFIN = DBLE ( XBUF ( INDEX ))
	YFIN = DBLE ( XBUF ( INDEX + 1 ))


c
c
c       if the axis of the rectangle is horizontal we simplify the process
c       of projection      
c
c       logicaL HORIZ is TRUE if the line is horizontal
c
	IF ( DABS ( YDEB - YFIN) .LT. EPSILON ) THEN
	HORIZ = .TRUE.
	ELSE
	HORIZ = .FALSE.
	ENDIF
C
C
C
C     
C-----------------------------------------------------------------
C
C    INDEX is pointing to the x_coord of the current sp in the buffer
C-----------------------------------------------------------------
C

	INDEX = NBW_SP + INDX1
C
C-----------------------------------------------------------------
C    DIST is the current maximum distance between the point already
c    processed and th axis of the rectangle 
c-----------------------------------------------------------------
C
	DIST = 0

C
C
C
C




	IF ( HORIZ )   THEN
C       ++             +++++
c
c  reverse if necessay XDEB and XFIN for XDEB being lower than XFIN 
C

	IF ( XFIN .LT. XDEB )  THEN
	XT = XFIN
	YT = YFIN
	XFIN = XDEB
	YFIN = YDEB
	XDEB = XT
	YDEB = YT
	ENDIF


c
c       the line is horizontal so we just consider the x coord for 
c       projecting on the rectangle axis
c      loop on all the points of the buffer

	DO 100  I = 2, NUMBER - 1

	    YPROJ = YDEB
	    XPROJ = DBLE ( XBUF ( INDEX  ))
	    DPROJ = XBUF ( INDEX + 1 ) - YDEB
	    DPROJ = DPROJ * DPROJ
	    DIST  = DMAX1 ( DIST, DPROJ )
c
c     reset the bounds of the axis segment of the rectangle if necessary
c
	    IF ( XPROJ .LT. XDEB ) THEN
	    XDEB = XPROJ
				ELSE
	    IF ( XPROJ .GT. XFIN )  XFIN = XPROJ
      	      ENDIF

	    INDEX = INDEX + NBW_SP
100     CONTINUE

c
c     we have the axis of the rectangle which is horizontal between
C     (XDEB, YDEB) AND ( XFIN, YFIN )
C      we add  XD_LONG on each side of the segment
C            AND XD_LARG + DIST  perpendicular to the segment to
c      determine the 4 corners of the rectangle enclosing the line
c
	DIST = DSQRT ( DIST )
	DIST = DIST + XD_LARG
	RECT_ENV ( 1 )  = XDEB - XD_LONG
	RECT_ENV ( 2 )  = YDEB + DIST
	RECT_ENV ( 3 )  = XFIN + XD_LONG
	RECT_ENV ( 4 )  = RECT_ENV ( 2)
	RECT_ENV ( 5 )  = RECT_ENV ( 3 )
	RECT_ENV ( 6 )  = YFIN - DIST
	RECT_ENV ( 7 )  = RECT_ENV ( 1 )
	RECT_ENV ( 8 )  = RECT_ENV ( 6 )

	ELSE
C       +++++
C
c
c  reverse if necessay XDEB and XFIN for XDEB being lower than XFIN 
C

	IF ( XFIN .LT. XDEB )  THEN
	XT = XFIN
	YT = YFIN
	XFIN = XDEB
	YFIN = YDEB
	XDEB = XT
	YDEB = YT
	ENDIF

c
c      loop on all the point computing the projection on the segment axis
c      and the distance from the current point to the rectangle axis
c
c     XAL is the director coefficient of the perpendicular to 
c     the rectangle axis
C
	XAL = (  XDEB - XFIN ) / ( YFIN - YDEB )
	XAL2 = XAL * XAL 
	XAL3 = XAL2 + 1.

	DO 200 I = 2, NUMBER - 1

           XT = DBLE (XBUF ( INDEX ))
           YT = DBLE (XBUF ( INDEX + 1))
           XPROJ = ( XAL * ( YDEB - YT ) + XAL2 * XT + XDEB )/ XAL3
           YPROJ = XAL * ( XPROJ - XT) + YT
	
	   DPROJ =  ( XT - XPROJ ) **2 + ( YT - YPROJ )**2
	   DIST  = DMAX1 ( DIST, DPROJ )
c
c     reset the bounds of the axis segment of the rectangle if necessary
c
	   IF ( XPROJ .LT. XDEB ) THEN
	           XDEB = XPROJ
		   YDEB = YPROJ
		   			ELSE
		   IF ( XPROJ .GT. XFIN ) THEN
		   XFIN = XPROJ
		   YFIN = YPROJ          
	   	    ENDIF
	   ENDIF


		INDEX = INDEX + NBW_SP
200     CONTINUE


c
c     we have the axis of the rectangle which is not horizontal
C    BETWEEN    (XDEB, YDEB) AND ( XFIN, YFIN )
C      WE ADD  XD_LONG on each side of the segment
C            AND XD_LARG + DIST  perpendicular to the segment to
c      determine the 4 corners of the rectangle enclosing the line
c
	DIST = DSQRT ( DIST )
	DIST = DIST + XD_LARG
     
c      if the line is vertical we need not to compute 
c      tang

	IF  ( DABS (XAL)  .LT. EPSILON ) THEN
C       //                        ////
C
	RECT_ENV ( 1 )  = XFIN + DIST
	RECT_ENV ( 2 )  = YFIN + XD_LONG
	RECT_ENV ( 3 )  = RECT_ENV ( 1 )
	RECT_ENV ( 4 )  = YDEB - XD_LONG
	RECT_ENV ( 5 )  = XDEB - DIST
	RECT_ENV ( 6 )  = RECT_ENV ( 4 )
	RECT_ENV ( 7 )  = RECT_ENV ( 5 )
	RECT_ENV ( 8 )  = RECT_ENV ( 2 )

	ELSE
C       ////
c
c       general case  line is not vertical and not horizontal
C       
	XCOEF  = - 1 / XAL 
        XCOEF_ABS = DABS ( XCOEF )
        X_ATAN = DATAN ( XCOEF_ABS )
	XINC1 = XD_LONG * DCOS ( X_ATAN )
	YINC1 = XD_LONG * DSIN ( X_ATAN )
	XINC2 = DIST *  DSIN ( X_ATAN )
	YINC2 = DIST *  DCOS ( X_ATAN )

	IF ( XCOEF .GT. 0. )  THEN
C       --                   ----

                  RECT_ENV(1)= XDEB-XINC1+XINC2               
                  RECT_ENV(2)=YDEB-YINC1-YINC2 
                  RECT_ENV(3)=XDEB-XINC1-XINC2   
                  RECT_ENV(4)=YDEB-YINC1+YINC2 
                  RECT_ENV(5)=XFIN+XINC1-XINC2 
                  RECT_ENV(6)=YFIN+YINC1+YINC2 
                  RECT_ENV(7)=XFIN+XINC1+XINC2
                  RECT_ENV(8)=YFIN+YINC1-YINC2 

                  ELSE
C                 -----

                  RECT_ENV(1)=XDEB-XINC1-XINC2 
                  RECT_ENV(2)=YDEB+YINC1-YINC2 
                  RECT_ENV(3)=XDEB-XINC1+XINC2
                  RECT_ENV(4)=YDEB+YINC1+YINC2
                  RECT_ENV(5)=XFIN+XINC1+XINC2
                  RECT_ENV(6)=YFIN-YINC1+YINC2
                  RECT_ENV(7)=XFIN+XINC1-XINC2
                  RECT_ENV(8)=YFIN-YINC1-YINC2
*                   



	ENDIF
C       -----

	ENDIF
C       /////
C
	ENDIF
C       +++++
C
C
C     fill XAXE_END containing coordinates of the ends of the rectangle
c     axis
C
       XAXE_END (1) = SNGL (XDEB)
       XAXE_END (2) = SNGL (YDEB)
       XAXE_END (3) = SNGL (XFIN)
       XAXE_END (4) = SNGL (YFIN)


	RETURN
	END 
C
C ====================  UK_OPTM_XY ============================
***********************************************************
*
* AUTHOR:REMI DORET
*
* DATE : 2/03/1987
C
C
C  ROUTINE USED TO LINEARISED THE LINE DESCRIBED WITH XY COORD
C     USING THE METHOD OF ANGLE AND THE SPACING BETWEEN CDP
C
C
C    INPUT  :
C    IBUF    :  INTEGER*4 BUFFER EQUIVALENT TO XBUF
C    XBUF    :  REAL*4 BUFFER CONTAINING ALL THE INFORMATIONS
C             FOR EACH SP OF THE CURRENT LINE
C             THE FIRST INFORMATION IS THE SP NUMBER
C             THE SECOND IS X COORD OR CONVERTED LONG
C             THE THIRD  IS Y COORD OR CONVERTED LAT
C             THEN WE CAN HAVE ALTI WATER DEPTH
C             WE HAVE NBW_SP WORDS(4 BYTES) OF INFORMATION FOR EACH SP
C             BUF ( 1)  IS THE FIRST SP NUMBER
C             BUF ( 1 + NBW_SP ) IS THE SECOND SP NUMBER ....
C   NBW_SP  :  NUMBER OF WORDS ( 4 BYTES) OF INFO FOR EACH SP
C
C  NUMBER   : NUMBER OF SP TO PROCESS
C
C  ALIMIT   : ANGLE LIMIT FOR LINEARISATION ( IN RADIANS ) 
C              USUALLY EQUAL TO .04 
C  MINI_TRP : MINIMUM NUMBER OF SHOT POINT IN THE LINEARISED LINE
C
C  OUTPUT   :  NB_RET   NUMBER OF SP AFTER LINEARISATION 
C           :  XSPACIN      AVERAGE DISTANCE BETWEEN SP ON THE 
C                            LINE
C   	       ISTAT    ERROR STATUS : #0 ERROR, =0, OK
C *******************************************************************

C
      SUBROUTINE UK_OPTM_XY (IBUF, XBUF ,NUMBER ,NBW_SP,
     +             ALIMIT, MINI_TRP, NB_RET, XSPACIN, ISTAT)
C
      INCLUDE 'ld_ukooa2.i'
C
C
	INTEGER*4 IBUF (*)
       REAL*4 XBUF (*), XSPACIN
       INTEGER*4 NUMBER, NBW_SP, NB_RET

	INTEGER*4 CUR_IND1, CUR_IND2, LSTINDEX
	REAL*4 CUR_ANG, LST_ANG, DIF_ANG
	REAL*4 DX, DY
	LOGICAL KEEP, MULTIP 
	REAL*8 DIST, TOT_DIST, LST_DIST, CUR_DIST
	REAL*4 DROIT, ALIMIT
        INTEGER*4 INC ,INC_1, COMP, IDIF_CDP
        INTEGER*4 MINI_TRP
        
	DATA DROIT /1.570786/ 

        REAL*8 XDIF_SP, PERCENT
C
C       
         PERCENT = 0.05
C
C    if we have less than   MINI_TRP  points in the line we don't
c    linearise the line
c    if we have more than MINI_TRP  we can linearise the line
c    using the  ' angle method and distance' but we need to keep at least
c    MINI_TRP in the line 
c
c

	IF ( NUMBER .GT. MINI_TRP ) THEN
C       ++                          +++++
C     we compute an increment which will be used to keep at
c     least MINI_TRP  element in the linearisation
c
      	INC =  NUMBER/MINI_TRP + 1
C
	INC_1 = INC -1 
C
CC    LST_ANG  is the angle which is the reference with which we 
c     compare the current angle and determine if the difference
c      between the two is greater than the tolerance we have
c    CUR_ANG  is the current of the segment we are working
c    on 
c
c    LST_DIST is the average spacing between cdp on the last segment
c    it is compared with the current spacing CUR_DIST thedifference must be
c    lower than the limit for the point not to be kept in the linearised
c    buffer
C
C
C       initialise LST_ANG
c
	
	DX = XBUF (5) - XBUF (2)
	DY = XBUF (6) - XBUF (3)

	IF ( ABS (DX) .LT. EPSILON ) THEN
		    IF ( (DX * DY) .GT. 0 ) THEN
		    LST_ANG = DROIT 
	            ELSE
		    LST_ANG = - DROIT 
		    ENDIF

				ELSE
	LST_ANG = ATAN ( DY / DX )
	ENDIF

c  initialise LST_DIST

	LST_DIST = DBLE(DX)**2 + DBLE (DY)**2
	IDIF_CDP = IABS ( IBUF (4) - IBUF (1))
	IF (IDIF_CDP.EQ.0) THEN
	  ISTAT = IBUF(4)
	  GO TO 999
	ENDIF
	LST_DIST = LST_DIST / IDIF_CDP

	CUR_IND1  = NBW_SP + 2

        CUR_IND2 = CUR_IND1 + NBW_SP

	LSTINDEX = CUR_IND1

	COMP = 2 
c
C    initialise LS_STOR TO 2 because we always keep the first point

	LS_STOR = 2

10     CONTINUE

	IF ( COMP .LT. NUMBER ) THEN 
C       ==                      ====  

cc     if we are on a multiple of the increment we keep automatically
cc     the current point  
cc     else   we compute the difference of angles
cc
	KEEP = .FALSE. 
        MULTIP = .FALSE. 

       IF ( JMOD ( COMP , INC ) .EQ. INC_1 )  THEN
C      //                             ////  
	KEEP = .TRUE.
	MULTIP = .TRUE.

	ELSE
C       ////
c   compute the angle
c
	DX = XBUF ( CUR_IND2 )  -  XBUF ( CUR_IND1)
        DY = XBUF (CUR_IND2 + 1) - XBUF ( CUR_IND1 + 1)

	 IF ( ABS (DX)  .LT. EPSILON ) THEN
		    IF ( (DX * DY) .GT. 0 ) THEN
		    LST_ANG = DROIT 
	            ELSE
		    LST_ANG = - DROIT 
		    ENDIF
	 ELSE
	 CUR_ANG = ATAN ( DY / DX )
	 ENDIF
c
c   compute current average distance between cdp on this segment
C

	CUR_DIST = DBLE(DX)**2 + DBLE (DY)**2
	IDIF_CDP = IABS ( IBUF (CUR_IND2 - 1) - IBUF (CUR_IND1 - 1))
	IF (IDIF_CDP.EQ.0) THEN
	  ISTAT = IBUF(CUR_IND2-1)
	  GO TO 999
	ENDIF
	CUR_DIST = CUR_DIST / IDIF_CDP

c
c   test difference of angles
C
	DIF_ANG = CUR_ANG - LST_ANG
	IF ( ABS ( DIF_ANG ) .GT. ALIMIT )THEN 
	 KEEP = .TRUE.
C
	ELSE
c  the difference of angle is very little we now test the 
c  difference between average inter_cdp spacing
c  if this diff is lower than the limit we don't keep
c  the point - if it greater than the limit we keep the point
C
        IF ( CUR_DIST .GT. 0 ) THEN

        XDIF_SP = ( LST_DIST - CUR_DIST ) / CUR_DIST 
                               ELSE

	XDIF_SP = 1
        ENDIF



	IF ( DABS (  XDIF_SP ) .GT. PERCENT )KEEP = .TRUE.
	ENDIF
C
C
C

	ENDIF
C       /////
C
	IF ( KEEP )   THEN
C       --            -----

c      we need to keep some of this points

	IF ( LS_STOR .EQ. CUR_IND1 ) THEN
C       ...                            ....
C
c   we move only the last point of the current segment
c   the other one has already been moved
C
	INDEB = CUR_IND2
	NB_WORD = NBW_SP

	ELSE
C       ....
c   we  move the two ends of the segment 
C
	INDEB = CUR_IND1
	NB_WORD = 2 * NBW_SP

	ENDIF
C       .....
c
c   moving one or two points after the last point 
c   in the linearized buffer
C 
c   we begin the loop bY I = -1 because LSTINDEX and INDEB
c    are pointing on the x coord of the cdp which is the second word
c     in the block of information to move 
C    
	DO 15 I = -1, NB_WORD - 2

	XBUF ( LSTINDEX + I) = XBUF ( INDEB + I )
15      CONTINUE

	LS_STOR = CUR_IND2
	LSTINDEX = LSTINDEX + NB_WORD 

c
c      if we are on a multiple of the incr we need to recompute
c       the last angle  and the last average spacing between cdp
c
		IF ( MULTIP ) THEN
C               ;;            ;;;;
		I = LSTINDEX - NBW_SP
		J = I - NBW_SP 	
		DX = XBUF ( I )  -  XBUF ( J )
	        DY = XBUF (I + 1) - XBUF ( J + 1)


	LST_DIST = DBLE(DX)**2 + DBLE (DY)**2
	IDIF_CDP = IABS ( IBUF (I-1) - IBUF (J-1))
	IF (IDIF_CDP.EQ.0) THEN
	  ISTAT = IBUF(I-1)
	  GO TO 999
	ENDIF
	LST_DIST = LST_DIST / IDIF_CDP

		 IF ( ABS ( DX)  .LT. EPSILON ) THEN
C                <<                             <<<<
C  we need to test the sign oF DX * DY to know if the angle is -90 DEg or 
C  +90
			IF ( (DX * DY) .GT. 0 ) THEN
			LST_ANG = DROIT 
			ELSE
			LST_ANG = - DROIT 
			ENDIF
		
		    ELSE
C                   <<<<
		    LST_ANG = ATAN ( DY / DX )
		    ENDIF
C                   <<<<<
		
			    ELSE
C                           ;;;;
	
		LST_ANG = CUR_ANG
		LST_DIST = CUR_DIST
		ENDIF
C               ;;;;;


	ENDIF
C       ----

	CUR_IND1 = CUR_IND1 + NBW_SP
	CUR_IND2 = CUR_IND2 + NBW_SP


	COMP = COMP + 1


	GOTO   10
C
	ENDIF
C       =====
c
c    copy the last point after the last point stored
c  if it was not already stored
C    
	IF ( LS_STOR .LT. CUR_IND1 ) THEN
	DO 25 I = -1, NBW_SP - 2

	XBUF ( LSTINDEX + I) = XBUF ( CUR_IND1 + I )
25     CONTINUE

	LSTINDEX = LSTINDEX + NBW_SP
        ENDIF

	
C    NB_RET is the number of points in the linearized line
C
	NB_RET = ( LSTINDEX - 1 ) / NBW_SP

	
C
C
	ELSE
C       +++++
c  no linearisation because not enough points in the buffer
C
	NB_RET = NUMBER


	ENDIF
C       +++++

C
C
c
c	loop to compute the intersp the total length of the line
c        then we will divide the total length to find
c        the intersp distance
C
	CUR_IND1 = 2
	CUR_IND2 = CUR_IND1 + NBW_SP
	TOT_DIST = 0

	DO 50 I = 1, NB_RET - 1	   


	DIST = ( DBLE(XBUF( CUR_IND1)) - DBLE(XBUF( CUR_IND2)))**2
     C   + ( DBLE(XBUF(CUR_IND1 + 1)) - DBLE(XBUF(CUR_IND2+ 1)))**2
	TOT_DIST = TOT_DIST + DSQRT ( DIST )
	CUR_IND1 = CUR_IND1 + NBW_SP
	CUR_IND2 = CUR_IND2 + NBW_SP

50      CONTINUE
        IND = 2
C        DO 3 I = 1, NB_RET

C        WRITE ( LIST, 1200 ) IBUF (IND - 1) ,
C     C              XBUF ( IND), XBUF ( IND+ 1)
C	IND = IND + NBW_SP
C3        CONTINUE
C1200    FORMAT ( /,'CDP=',I6,' XCOORD = ',F15.3,' YCOORD =',F15.3)


        ILST = 1 + ( NB_RET -1 ) * NBW_SP
	DIST = TOT_DIST / IABS ( IBUF ( 1)- IBUF( ILST))
	XSPACIN = SNGL ( DIST )
C
999 	CONTINUE
	END

***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 5/5/87
*
* FUNCTION : TO UPDATE SURVEY BOUNDS + ENVELLOPPES AROUND PROFILES
*
* LANGUAGE: FORTRAN
*
* PARAMETERS:  
*             ISTAT :  STATUS = 0  SUCCESS
*                             # 0  FAIL
*
*
***************************************************************
C
C	    SUBROUTINE UPD_ENV ( ISTAT )
C
C       INCLUDE 'ld_ukooa2.i'
*
*
C	REAL*8 XDIST, RECT_ENV (8)
C
C       INTEGER*4  INDEX  ,IND_L, INC_V, ITOT_LIN, ITOT
C       INTEGER*4  INDINS
C       CHARACTER*20 LINE_NAM
C	REAL*4 XENDS (4)        
C
c
c          compute XDIST used for the enveloppe
c          computation 
c          this coeff are relative to the extent of the survey
C        we first compute the bounds in x y for the project 
c
C     CALL UPD_BNDS ( ISTAT )
C
C            XDIST = ABS  ( ( NEWMAX - NEWMIX ) / 30 )
c
c     read the vector containing all the enveloppes
c     we use the common sisline to store this information since it is not 
c     more used at this point  of the program
c
C      CALL FINDEVLP ( LIGNE, L_TAB, INC_V, ITOT_LIN, ISTAT )
C
C
C       ITOT = ITOT_LIN
c
c   loop on the loaded line to compute the envelopes
C
C	     INDEX = 1
C            IND_L = 1
C
C            DO  100   I = 1, NBENT
C            IND_L = ( I-1) * NBCARL + 1
c
c    compute the enveloppe
C
C            LINE_NAM (1:NBCARL) = CTABL ( IND_L:IND_L+NBCARL-1)
C
C            CALL UK_ENVEL_F ( EXTREM_C (INDEX ), C1, C2, XDIST,
C    -                         XDIST, RECT_ENV, XENDS ) 
c
c    update the vector containing all the envellopes 
c            scan first to see if the line is already in this vector
C
C              INDEX2 = 5
C
C              DO 50 J = 1, ITOT_LIN
C              
C              IF(LINE_NAM(1:NBCARL).EQ.
C    -             CLIGNE( INDEX2:INDEX2+NBCARL-1 )) GOTO 60
C
C              INDEX2 = INDEX2 +(4 *  INC_V )
C
C50            CONTINUE
c
c     we didn't found the line so we add a new line in the vector
c    INDINS is the index in LIGNE where we begin to insert the information
C
C           INDINS= ITOT * INC_V
C
C   ITOT total number of lines in this buffer is increased by 1
C
C           ITOT  = ITOT + 1
C
C          GOTO 70
C
C
C60       CONTINUE
c
c    the line is already there so we replace the envelloppe 
c     by the new one
C         INDINS = (J -1 ) * INC_V
C
C
C70       CONTINUE
C
C
C
C           LIGNE ( INDINS + 1) = INC_V
C           INDINS = INDINS + 1
c
c    store the number of words describing the enveloppe
C
C           IND_CAR = INDINS  * 4
c
c    store the line name 
C
C           CLIGNE(IND_CAR+1 :IND_CAR+NBCARL)=LINE_NAM(1:NBCARL)
C           INDINS = INDINS + NBWLI
c
c  store the envelope coordinates
C
C            DO 80 K = 1, 8
C            RLIGNE ( INDINS + K ) = SNGL ( RECT_ENV (K))
C80          CONTINUE
C
C
C           
C100        CONTINUE
C
C
c
c     store the envelloppes in the data base  AFTER PURGING THE OLD ONE
C
C      CALL PURGEVLP (ISTAT)
C
C      CALL ADDEVLP ( LIGNE, LTAB, INC_V, ITOT, ISTAT )
C
c
c      store the bounds of the survey in the data base after purging the 
c     old one
C
C       CALL PURGBNDS ( ISTAT )
C
C      CALL ADDBNDS ( NEWMAX,NEWMIX,NEWMAY,NEWMIY, ISTAT )
C
C
C           ISTAT = 0
C
C         RETURN 
C         END
C
C
***********************************************************
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*         COMPAGNIE GENERALE DE GEOPHYSIQUE               *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
*                                                         *
***********************************************************
*
*
*
* AUTHOR:REMI DORET
*
* DATE : 5/5/87
*
* FUNCTION : COMPUTE THE INTERSECTIONS BETWEEN THE NEW LINES LOADED
*               IN THE PROJECT AND THE LINES ALREADY THERE
*
* LANGUAGE: FORTRAN
*
* PARAMETERS:  
*             ISTAT :  STATUS = 0  SUCCESS
*                             # 0  FAIL
*
*
***************************************************************
C
C	    SUBROUTINE LD_INTER (ISTAT )
C
C       INCLUDE 'ld_ukooa2.i'
*
C        INTEGER*4 NBLINES
C
C      if there were no lines in the project before beginning or
c    if all the lines present were replaced we compute the intersections
c    for all the lines in the project ( NBLINES is set )
c    in  all the other cases we compute the lines between the lines added
c     and the rest of the lines in the project
c
C        IF ( (IETADB .EQ.0) .OR. (REPLINE .EQ. NB_LIG)) THEN
C             NBLINES = 0 
C                                                        ELSE
C             NBLINES = NBENT 
C        ENDIF
C
C         CALL LDGINTER ( ITABL, NBLINES, NBWLI, SURV_NAM )
C
C         RETURN
C         END
