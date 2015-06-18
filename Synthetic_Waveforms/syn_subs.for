C  DEC/CMS REPLACEMENT HISTORY, Element SYN_SUBS.FOR
C  *6    13-OCT-1989 09:57:26 GORDON "(SPR 5017) Avoid divide-by-zero error"
C  *5    26-SEP-1989 10:10:52 GORDON "(SPR 201) Remove again the redefinition of TINC"
C  *4    19-SEP-1989 10:17:28 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *3    12-AUG-1989 10:48:04 GILLESPIE "(SPR 2) remover duplicate definition of tinc"
C  *2    11-AUG-1989 17:06:45 VINCE "(SPR -1) last fixes from UNIX port."
C  *1    10-AUG-1989 18:52:42 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element SYN_SUBS.FOR
C******************************************************************************
C
C                Copyright Exploration Systems, Inc. 1989
C		   Unpublished -- All rights reserved
C
C THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc AND MAY
C CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
C DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
C MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF
C
C			Exploration Systems, Inc.
C			579 Market Street
C			San Francisco, CA  USA 94105
C			(415) 974-1600
C
C (The above notice does not constitute an admission of publication or
C unrestricted dissemination of the work)
C
C******************************************************************************
C
C SYNTH_SUBS.F77,  SYNTHETIC,  SPENCE,  12/19/86
C SYNTHETICS FORTRAN CALCULATION ROUTINES
C Copyright (c) 1986, Exploration Systems Inc, Sausalito, CA.
C
C TITLE : SYNTH_SUBS - SYNTHETICS FORTRAN CALCULATION ROUTINES
C
C START-HISTORY :
C 12/19/86 SPENCE   CHANGED COLOR OF REVERSE POLARITY TO WHITE SO IT WILL PLOT BLACK
C                     COULD NOT READ THE YELLOW
C 10/07/86 JULIAN   CHANGE NAME OF RESOURCE FILE FROM SYN TO ESI_RSRC_SYN
C 09/25/86 SPENCE   PUT GPL BUFS IN COMMON TO AVOID STACK_OVF$
C 09/22/86 SPENCE   MAKE GPL XBUF AND YBUF BIGGER IN SYNPLOT
C 07/15/86 SPENCE   FIXED PLOT RES LOG ON A LOG SCALE IN SYNPLOT DIALOG
C 07/02/86 JOHN     CHANGED NAME OF VAL COMMON BLOCK TO SYNVAL
C                   CHANGED NAME OF DATA COMMON BLOCK TO SYNDAT
C                   THESE CHANGES MADE TO RESOLVE CONFLICTING NAMES IN LINKER
C 06/05/86 SPENCE   FIXED NDCS FOR METAFILE AND OVERWRITING TEXT
C 05/15/86 SPENCE   RECORD WAVELET TYPE IN SYNTH STRUCTURE
C 05/13/86 SPENCE   NEW GKS FULL SCREEN
C 05/05/86 MARK     ADDED METAFILE CALLS (MARK)
C 03/19/86 SPENCE   INITIAL LOAD
C 03/19/86 SPENCE Template constructed
C END-HISTORY
C
C START-CODE :













      SUBROUTINE SYNOPFIL(IUNIT,TWELL)
C*************************************************************************
C                                                                         
C   OPEN A SYNTHETIC FILE
C
C***********************************************************************

      CHARACTER*(*) TWELL
      INCLUDE 'esi_syn_io.i'
C
C GET FILE UNIT
C
C 01/04/89 GS - Implement latest plot filename standards
      CALL HOG_GET_UNIQUE_FILENAME( SYNFILE )
      CALL TFSTRCAT( SYNFILE, '.DAT', SYNFILE )
      CALL HOG_ADD_PATH( 'ESI$SCRATCH', SYNFILE, LEN(SYNFILE),
     *			  SYNFILE )
      CALL HOG_TRANSLATE_FILENAME( SYNFILE, SYNFILE )

      CALL HOGFNT(IUNIT)
      OPEN(UNIT=IUNIT,FILE=SYNFILE,STATUS='NEW',FORM='UNFORMATTED')
      RETURN
      END







      SUBROUTINE SYNMULTIP(N,A,IORDER,ISHIF)
C***********************************************************************
C
C CALCULATE SURFACE MULTIPLES
C
C N     = NUMBER OF POINTS IN REFL COEF SERIES
C A     = REFL COEFF SERIES ARRAY
C IORDER = ORDER OF MULTIPLE
C ISHIF  = NUMBER OF MS SHIFT BETWEEN DATUM AND SURFACE
C
C AOUT = WORK ARRAY - SHARE COMMON AREA WITH AGC
C VA   = REFL COEF SERIES FROM DATA BETWEEN SURFACE AND DATUM TO BE ADDED
C        BACK INTO THE MULTIPLE CALCULATIONS
C
C***********************************************************************

      DIMENSION A(1),AOUT(4000)
      COMMON/AGCCOM/AOUT
      COMMON/EXMULT/VA(2000)
      DATA AOUT/4000*0.0/
C
C MOVE REFL COEF TO AOUT
C
      DO 1 I=1,N
    1 AOUT(I)=A(I)
C
C ADD IN MULTIPLE DATA FOR DATA BETWEEN SURFACE AND DATUM
C
      IF(ISHIF.NE.0)THEN
         DO 10 I=2,IORDER+1
C
C CALCULATE SIGN OF MULTIPLIER
C
         AM = (-1) ** I *(-1.)
           DO 30 J = 1,ISHIF
C
C I   IS ORDER OF MULTIPLE
C J   IS TIME
C
           IT = I*J-ISHIF
           IF(IT.LE.0)GOTO 30
           AOUT(IT) = AOUT(IT) + VA(J)* AM
30    CONTINUE
10    CONTINUE
      ENDIF
C
C REST OF TRACE
C
      DO 2 I=2,IORDER+1
C
C CALCULATE SIGN OF MULTIPLIER
C
      AM = (-1)**I  * (-1.)
      DO 3 J=1,N
C
C I IS ORDER
C J IS TIME
C
      IT = J + ISHIF
      IF(IT*I-ISHIF.GT. N)GOTO 2
      AOUT(IT*I-ISHIF)=AOUT(IT*I-ISHIF)+A(J)*AM
    3 CONTINUE
    2 CONTINUE
C
C PUT BACK IN INPUT VECTOR
C
      DO 4 I=1,N
    4 A(I)=AOUT(I)
      RETURN
      END








      SUBROUTINE SYNMTSHIF(IXM,IANS,NCKSHT,CKDEPT,CKTIME,IDEN,IDSMP)
C***********************************************************************
C
C THIS SUBROUTINE INTEGRATES THE PORTION OF THE SONIC LOG AND/OR VELOCITY
C FUNCTION WHICH COVER THE FOOTAGE BETWEEN THE SURFACE AND THE DATUM.
C THE REFLECTION COEFFICIENTS ARE THEN SAVED IN ARRAY VA IN COMMON BLOCK
C EXMULT. THIS DATA IS THEN USED IN THE MULTIPLE CALCULATIONS.
C
C      VEL   - VELOCITY ARRAY
C      DEN   - DENSITY ARRAY
C      IXM   - NUMBER OF SAMPLES TO INTEGRATE
C      IANS  - NUMBER OF MILLISECONDS OF MULTIPLE TIME SHIFT
C      NCKSHT- NUMBER OF CHECK SHOT PAIRS
C      CKDEPT- DEPTH ARRAY FOR TIME/DEPTH PAIRS
C      CKTIME- TIME ARRAY FOR TIME/DEPTH PAIRS
C      IDEN  - DENSITY EXISTENCE FLAG
C      IDSMP - DEPTH SAMPLING PERIOD OF LOGS
C
C***********************************************************************

      COMMON /VELDEN/VEL(12000), DEN(12000), DEPTH(2500)
      COMMON /LOGS12/AL1(12000), AL2(12000)
      COMMON /LOGS34/AL3(12000), AL4(12000)
      COMMON /LOGS5 /AL5(12000)
      INCLUDE 'esi_syn_io.i'
      COMMON /EXMULT/VA(2000)
C
C INTEGRATE NUMBER OF SAMPLES TO BE REMOVED BY DATUM CORRECTION
C THE ZERO CONSTANTS SAY DO NOT WORRY ABOUT ANY OTHER LOGS.
C ONE FILL VALUE REQUIRED.
C
      CALL SYNINTEG(IXM,NCKSHT,CKDEPT,CKTIME,IDEN,0,0,0,0,0,IDSMP,IANS,1
     X )
C
C CALCULATE REFLECTION COEFFICIENT SERIES
C
      CALL SYNVACALC(IANS,VA,1)
C     WRITE(ITERM,808)IANS
808   FORMAT(/'MULTIPLE TIME SHIFT OF ',I3,'MS 1 WAY TIME CALCULATED.')
      RETURN
      END









C
C***********************************************************************
C
      SUBROUTINE SYNINTEG(NPTS,NCKSHT,CKDEPT,CKTIME,IDEN,IL1,IL2,IL3,
     X IL4,IL5,IDSMP,NPTSO,NFILL)
C
C THIS PROGRAM INTEGRATES THE SONIC LOG INTO TIME FROM DEPTH USING VELOCITIES
C CALCULATED FROM THE SONIC LOG.  IF A CHECKSHOT SURVEY IS AVAILABLE, THE VEL-
C OCITIES USED ARE DERIVED FROM THE CHECKSHOT SURVEY.  WHILE INTEGRATING THE
C SONIC, THE DENSITY LOG AND UP TO 5 ADDITIONAL LOGS (IF AVAILABLE) WILL BE
C TRANSFORMED FROM DEPTH TO TIME.
C
C NPTS   - NUMBER OF SAMPLES IN INPUT LOGS
C NCKSHT - NUMBER OF CHECK SHOT PAIRS
C CKDEPT - CHECK SHOT DEPTH ARRAY (FIRST DEPTH ALWAYS 0)
C CKTIME - CHECK SHOT TIME ARRAY IN ONE WAY TIME SECONDS (1ST TIME ALWAYS 0)
C IDEN   - DENSITY AVAILABLE FLAG, = 0 IF DENSITY NOT AVAILABLE, DENSITY ARRAY
C          WILL BE SET EQUAL TO ALL 1.0 ' S IF NOT AVAILABLE.
C IL1    - LOG 1 AVAILABLE IN ARRAY AL1
C IL2    - LOG 2 AVAILABLE IN ARRAY AL2
C IL3    - LOG 3 AVAILABLE IN ARRAY AL3
C IL4    - LOG 4 AVAILABLE IN ARRAY AL4
C IL5    - LOG 5 AVAILABLE IN ARRAY AL5
C IDSMP  - DEPTH SAMPLING PERIOD
C NPTSO  - NUMBER OF SAMPLES IN ARRAYS IN TIME (MILLISECONDS ONE WAY TIME)
C NFILL  - NUMBER OF SAMPLES TO FILL AFTER ENDING TIME WITH VALUE AT ENDING
C          TIME.  THIS NEEDS TO BE AT 1 IF REFL COEF CALCS WILL BE MADE AND
C          MORE IF 50 OR 100 MILS OF STRAIGHT LINE ARE NEEDED TO BE PLOTTED
C          PAST CALCULATED ENDING TIME.
C
C COMMON BLOCK INFO:
C VEL    - INPUT VELOCITY LOG IN DEPTH, OUTPUT IN TIME
C DEN    - INPUT DENSITY LOG IN DEPTH, OUTPUT IN TIME
C DEPTH  - ARRAY CONTAINING DEPTH CORRESPONDING TO INTEGRATED TIME
C AL1    - FIRST OPTIONAL LOG TO BE TRANSFORMED FROM DEPTH TO TIME
C AL2    - SECOND OPTIONAL LOG TO BE TRANSFORMED FROM DEPTH TO TIME
C AL3    - THIRD OPTIONAL LOG TO BE TRANSFORMED FROM DEPTH TO TIME
C AL4    - FOURTH OPTIONAL LOG TO BE TRANSFORMED FROM DEPTH TO TIME
C AL5    - FIFTH OPTIONAL LOG TO BE TRANSFORMED FROM DEPTH TO TIME
C
      COMMON /VELDEN/VEL(12000),DEN(12000),DEPTH(2500)
      COMMON /LOGS12/AL1(12000),AL2(12000)
      COMMON /LOGS34/AL3(12000),AL4(12000)
      COMMON /LOGS5 /AL5(12000)
      REAL CKDEPT(1),CKTIME(1)
      REAL*4 SUM,TINC,V,VSUM,DSUM
      REAL TIME(12000)
      DATA   TINC/.001/
C
C ADD ONE MORE VELOCITY POINT IN ARRAY
C
      VEL(NPTS+1)=VEL(NPTS)
C
C IF CHECKSHOTS    1) TIME INTEGRATION LOOP
C
      IF ( NCKSHT .NE. 0 ) THEN
      	  TIME(1) = 0
      	  DO 33 L=2, (NPTS+1)
      	  TIME( L ) = TIME( L-1 ) + 2*IDSMP / ( VEL(L) + VEL(L-1) )
   33 	  CONTINUE
C
C		    2)  CALIBRATE THE SONIC ( 1/VELOCITY ) 
C
	DO 333 L=2,NCKSHT
	    ISTART = ( CKDEPT(L-1) / IDSMP ) + 1
	    IEND   = ( CKDEPT( L ) / IDSMP ) + 1
	    IF ( IEND .GT. (NPTS+1) ) THEN
		COEFF = ( (NPTS+1) - ISTART )
		COEFF = ( IEND - ISTART ) / COEFF
		IEND = (NPTS+1)
	    ELSE
		COEFF = 1.
	    ENDIF
	    COEFF  = COEFF * ( TIME( IEND ) - TIME (ISTART) ) /
     X	             ( CKTIME( L ) - CKTIME(L-1) ) 
	    IF ( L .EQ. NCKSHT )  IEND = NPTS+1
	    DO 334 LL = ISTART, IEND
		VEL(LL) = VEL(LL) * COEFF
334	    CONTINUE
	    IF ( IEND .EQ. (NPTS+1) ) GOTO 335
333	CONTINUE
      ENDIF
335   CONTINUE
C
C INITIALIZE SUMS AND COUNTERS
C
      DA=0.0
      SUM=0.0
      DSUM=0.0
      VSUM=0.0
      A1SUM=0.0
      A2SUM=0.0
      A3SUM=0.0
      A4SUM=0.0
      A5SUM=0.0
      K=0
      KONT=1
      NPTSO=1
C
      DO 38 L=1,NPTS
C
C GET VELOCITY FROM VELOCITY LOG OR CHECK SHOT SURVEY
          V=(VEL(L+1)+VEL(L))/2
C
C SUM TRAVEL TIME AND VELOCITY
C
      IF (V .NE. 0) SUM=SUM+IDSMP/V
      VSUM=VSUM+VEL(L)
C
C SUM OTHER LOGS IF AVAILABLE
C
      IF(IDEN.NE.0) DSUM=DSUM+DEN(L)
      IF(IL1.NE.0) A1SUM=A1SUM+AL1(L)
      IF(IL2.NE.0) A2SUM=A2SUM+AL2(L)
      IF(IL3.NE.0) A3SUM=A3SUM+AL3(L)
      IF(IL4.NE.0) A4SUM=A4SUM+AL4(L)
      IF(IL5.NE.0) A5SUM=A5SUM+AL5(L)
C
C INCREMENT COUNTER
C
      K=K+1
C
C INCREMENT DEPTH COUNTER
C
      DA=DA+IDSMP
C
C SEE IF SUM OF TRAVE TIME HAS PASS SAMPLING PERIOD TRAVEL TIME
C
      IF(SUM.LT.TINC) GO TO 38
C
C WE HAVE A NEW SAMPLE
C
      VEL(NPTSO)=VSUM/K
C
C CALCULATE DENSITY OF SET TO 1 IF NOT AVAILABLE
C
      IF(IDEN.EQ.0) THEN
           DEN(NPTSO)=1
      ELSE
           DEN(NPTSO)=DSUM/K
      ENDIF
      IF(IL1.NE.0)AL1(NPTSO)=A1SUM/K
      IF(IL2.NE.0)AL2(NPTSO)=A2SUM/K
      IF(IL3.NE.0)AL3(NPTSO)=A3SUM/K
      IF(IL4.NE.0)AL4(NPTSO)=A4SUM/K
      IF(IL5.NE.0)AL5(NPTSO)=A5SUM/K
C
C CALCULATE RESIDUAL TIME AND STORE IN SUM
C
      SUM=SUM-TINC
C
C CALCULATE DEPTH FOR THIS SAMPLE
C
      DEPTH(NPTSO)=DA-SUM*V
C
C RESET COUNTERS AND SUMS
C
      K=0
      VSUM=0.0
      DSUM=0.0
      A1SUM=0.0
      A2SUM=0.0
      A3SUM=0.0
      A4SUM=0.0
      A5SUM=0.0
C
C INCREMENT TIME SAMPLE COUNTER
C
      NPTSO=NPTSO+1
C
C IF AT END, SET IT BACK ONE
C
   38 CONTINUE
      NPTSO=NPTSO-1
C
C FILL OUT ARRAYS TO NPTSO + NFILL MILS
C
      DO 100 L = NPTSO+1,MIN0(NPTSO+NFILL,2500)
      VEL(L)=VEL(NPTSO)
      DEN(L)=DEN(NPTSO)
      AL1(L)=AL1(NPTSO)
      AL2(L)=AL2(NPTSO)
      AL3(L)=AL3(NPTSO)
      AL4(L)=AL4(NPTSO)
100   AL5(L)=AL5(NPTSO)
C
C THAT'S ALL FOLKS
C
      RETURN
      END







C
C***********************************************************************
C
      SUBROUTINE SYNGETRSRC(MENU_SYNTH1,INTC,IGEN,IALT,IACO,
     X  IVEL,IRUN,IQUIT,MS2,MS2C,MS2R,MS2RUN,MS2Q)

      INCLUDE 'syn.ri'

      MENU_SYNTH1=SYNTH1
      INTC=MENU_INTC
      IGEN=MENU_GENERAL
      IALT=MENU_ALTDEN
      IACO=MENU_ACOUS
      IVEL=MENU_VELANOTHER
      IRUN=MENU_RUNIT
      IQUIT=MENU_QUIT
      MS2=SYNTH2
      MS2C=MENU2_COSINE
      MS2R=MENU2_RICKER
      MS2RUN=MENU2_RUNIT
      MS2Q=MENU2_QUIT
      RETURN
      END






C
C***********************************************************************
C
      SUBROUTINE FIXCUR(ARRAY,IST,IEN,VNULL)
C
C SHIFT DATA TO SUBSCRIPT = 1/2 DEPTH
C
C ARRAY - ARRAY CONTAINING LOG CURVE, SAMPLING INTERVAL IS 2 FEET
C IST   - START DEPTH OF CURVE IN ARRAY
C IEN   - END DEPTH OF CURVE IN ARRAY
C VNULL - ARRAY NULL VALUE
C
      REAL ARRAY(12000)
C
C LET BE TOLERANT FOR THE NULL VALUES.....
C
      VNULL_MIN = .0001 * ABS(VNULL)
      VNULL_MAX = VNULL + VNULL_MIN
      VNULL_MIN = VNULL - VNULL_MIN
C
C NEWSP = SUBSCRIPT OF FIRST POINT (D/2) -1
C
      NEWSP = IST/2-1
C
C NPTS = NUMBER OF POINTS IN LOG
C
      NPTS =(IEN-IST)/2 + 1
C
C SHIFT EXISTING DATA TO SUBSCRIPT = 1/2 DEPTH
C
      DO 100 JK = NPTS,1,-1
100   ARRAY(JK+NEWSP)=ARRAY(JK)
C
C STUFF FRONT OF ARRAY WITH CONSTANT VALUE OF FIRST VALUE IN ARRAY
C
      DO 200 JK = 1, NEWSP
200   ARRAY(JK)=ARRAY(NEWSP+1)
C
C FIND FIRST NON NULL VALUE
C
      NEWSP=IEN/2
      IFOUND = 0
      DO 300 JK = 1 , NEWSP
      IF( ARRAY(JK).LT.VNULL_MIN .OR. ARRAY(JK).GT.VNULL_MAX )THEN
           IFOUND = JK
           GOTO 301
      ENDIF
300   CONTINUE
C
C IF ALL NULL JUST RETURN
C
301   IF(IFOUND.EQ.0)RETURN
C
C FILL FRONT NULLS WITH FIRST REAL VALUE
C
      DO 310 JK = 1,IFOUND-1
310   ARRAY(JK) = ARRAY(IFOUND)
C
C FILL BACK END NULLS WITH LAST REAL VALUE
C
      DO 320 JK = NEWSP,1,-1
      IF(ARRAY(JK).LT.VNULL_MIN .OR. ARRAY(JK).GT.VNULL_MAX)THEN
            IFOUND = JK
            GOTO 321
      ENDIF
320   CONTINUE
321   CONTINUE
      DO 330 JK = IFOUND+1,NEWSP
330   ARRAY(JK) = ARRAY(IFOUND)
C
C INTERPOLATE ALL NULL RANGES
C
      ISTART = 1
400   IFOUND = 0
C
C FIND FIRST NULL VALUE
C
      DO 420 JK = ISTART,NEWSP
      IF(ARRAY(JK).GT.VNULL_MIN .AND. ARRAY(JK).LT.VNULL_MAX )THEN
           IFOUND = JK
           GOTO 421
      ENDIF
420   CONTINUE
C
C IF NONE FOUND, ALL DONE
C
421   IF(IFOUND.EQ.0)GOTO 9999   
C
C FIND NEXT NON NULL VALUE
C
      DO 450 JK = IFOUND,NEWSP
      IF(ARRAY(JK).LT.VNULL_MIN .OR. ARRAY(JK).GT.VNULL_MAX)THEN
          IFOUND2 = JK
          GOTO 451
      ENDIF
450   CONTINUE
C
C INTERPOLATE IT
C
451   DX = (ARRAY(IFOUND2) - ARRAY(IFOUND-1)) / (IFOUND2-IFOUND+1)
      DO 460 JK = IFOUND,IFOUND2-1
460   ARRAY(JK)=ARRAY(JK-1) + DX
      ISTART = IFOUND2
      GOTO 400
C
9999  RETURN
      END





C
***********************************************************************
C
C BUILD A LOCATION DESCRIPTOR STRING
C
       SUBROUTINE GETLOC(OLOC,COUNTY,STATE,ITSHP,TSHPDR,
     X IRANGE,RANGDR,ISECT)

       INCLUDE 'esi_we_wells.i'
C
C BUILD OUTPUT LOCATION STRING
C
      CHARACTER*4 FMT(3)
      CHARACTER   OLOC*60
      CHARACTER   COUNTY*(SIZEOF_COUNTY), STATE*(SIZEOF_STATE)
      CHARACTER   TSHPDR*2,RANGDR*2
      DATA        FMT/'(I1)','(I2)','(I3)'/
      OLOC=' '
      IF(STATE.EQ.' ' .AND. COUNTY.EQ.' ')RETURN
      ILEN=ISTRLEN(STATE)
      IF(ILEN.NE.0)OLOC(1:ILEN)=STATE(1:ILEN)
      OLOC(ILEN+1:ILEN+2)= ','
      IST=ILEN+3
      ILEN = ISTRLEN(COUNTY)
      IF(ILEN.NE.0)OLOC(IST:IST+ILEN-1)=COUNTY(1:ILEN)
      IF(ITSHP.EQ.-999)RETURN
      OLOC(IST+ILEN:IST+ILEN+1)=', '
      IST=ILEN+IST+2
      OLOC(IST:IST+4)='SEC. '
      IST=IST+5
      ILEN = 1
      IF(ISECT.GT.9)ILEN=2
      IF(ISECT.GT.99)ILEN=3
      WRITE(OLOC(IST:IST+ILEN-1),FMT(ILEN))ISECT
      OLOC(IST+ILEN:IST+ILEN)='-'
      IST=IST+ILEN+1
      ILEN = 1
      IF(ITSHP.GT.9)ILEN=2
      IF(ITSHP.GT.99)ILEN=3
      WRITE(OLOC(IST:IST+ILEN-1),FMT(ILEN))ITSHP
      OLOC(IST+ILEN:IST+ILEN+1)=TSHPDR(1:1)//'-'
      IST=IST+ILEN+2
      ILEN = 1
      IF(IRANGE.GT.9)ILEN=2
      IF(IRANGE.GT.99)ILEN=3
      WRITE(OLOC(IST:IST+ILEN-1),FMT(ILEN))IRANGE
      OLOC(IST+ILEN:IST+ILEN)=RANGDR(1:1)
      RETURN
      END





C
C***********************************************************************
C
      SUBROUTINE CURCON(CURVE,NDEC,SYMBX,NPTSL)
      DIMENSION VALUES(5000),REFL(4000)
      COMMON/SYNVAL/VALUES
      COMMON/SYNDAT/DENSIT(4000),SP(4000),RES(4000),GAMMA(4000),REFL
      CHARACTER*10  CURVE
      RA = 6./7. * .1
        NDEC = 2
        SYMBX  = (  1. + 4. * RA -ISTRLEN(CURVE) * RA ) / 2.
      IF(CURVE.EQ.'SP'.OR.CURVE.EQ.'GAMMA RAY')NDEC=-1
      IF(CURVE.EQ.'REFL. COEF.') THEN
        NDEC = 3
        SYMBX  = (  1. + 5. * RA - 11. * RA ) / 2.
      ENDIF
      RETURN
      END




C
C***********************************************************************
C
      SUBROUTINE SINSYN(J)
      DIMENSION ACOR(6050),CORR(6050),FC(8),A(8000)
      COMMON/SYNTRA/A
      COMMON/BULLSH/ACOR,CORR
      DATA FC/-.090946,.127324,-.212206,.636621,.636621,-.212206,
     1.127324,-.090946/
      DO 200 I=1,6050
      CORR(I)=0.
  200 ACOR(I)=0.
      L=1
      K=1
      J=J+8
      DO 100 I=1,J
      SUM=0.
      DO 101 LM=1,8
      SUM=SUM+FC(LM)*A(K)
  101 K=K+1
      CORR(L)=SUM
      L=L+1
  100 K=L
      J=J-3
      K=5
      KK=8
      DO 102 I=1,J
      ACOR(KK)=CORR(I)
      KK=KK+1
      ACOR(KK)=A(K)
      KK=KK+1
  102 K=K+1
      J=KK-1
      DO 300 I=1,J
  300 A(I)=ACOR(I)
      RETURN
      END






C***********************************************************************
C
      SUBROUTINE SYNINIT
      INCLUDE 'esi_syn.i'
      FAUST_CONSTANT =  1948
      FAUST=0
      ID_FAUST=-9999
      FAUST_RERUN=0
      ALTDEN_CALC_DEN_VEL=0
      ALTDEN_CALC_DEN_FDCCNL=0
      ALTDEN_FLUID=1.
      ALTDEN_MATRIX=2.68
      ID_ALTDEN=-9999
      ALTDEN_RERUN=0
      ACOUS=0
      ACOUS_MIN_FREQ=8
      ACOUS_MAX_FREQ=40
      ID_ACOUS=-9999
      ACOUS_RERUN=0
      GENERAL_MULTI=0
      GENERAL_MULTI_COEF=1.
      GENERAL_AGC=0
      GENERAL_AGC_LENGTH=100
      GENERAL_TD_PRINT=0
      GENERAL_NFILT=1
      GENERAL_DATUM=-999
      GENERAL_VELOC=6500.
      ID_GENERAL=-9999
      GENERAL_RERUN=0
      KELLY_ESTIMATED=0
      KELLY_ELEVATION=-999
      ID_KELLY=-9999
      KELLY_RERUN=0
      VELANOTHER_USE=0
      ID_VELANOTHER=-9999
      VELANOTHER_RERUN=0
      RICKER_DRAW=1
      RICKER_TYPE=1
      RICKER_LENGTH_TAPER=100
      RICKER_TIME1=0
      RICKER_TIME2=0
      RICKER_TIME3=0
      RICKER_FREQ1=30
      RICKER_FREQ2=0
      RICKER_FREQ3=0
      ID_RICKER=-9999
      RICKER_RERUN=0
      COSINE_DRAW=01
      COSINE_LENGTH_TAPER=100
      COSINE_TIME1=0
      COSINE_TIME2=0
      COSINE_TIME3=0
      COSINE_MIN_FREQ1=8
      COSINE_MIN_FREQ2=0
      COSINE_MIN_FREQ3=0
      COSINE_MAX_FREQ1=40
      COSINE_MAX_FREQ2=0
      COSINE_MAX_FREQ3=0
      COSINE_PHASE=0
      ID_COSINE=-9999
      COSINE_RERUN=0
      IEKB=-999
      ITMAX=0
      STRAIGHT_USE=0
      STRAIGHT_VEL=6500
      ID_STRAIGHT=-9999
      STRAIGHT_RERUN=0
      WAVELET_TYPE=0
      ID_WAVELET=-9999
      WAVELET_RERUN=0
      RETURN
      END







      SUBROUTINE SYN_INIT_FILT_BUFFS
C**********************************************************************
C
C    ROUTINE:  SYN_INIT_FILT_BUFFS
C
C    FUNCTION: THIS ROUTINE INITIALIZES ALL THE FILTER PARAMETERS
C              AND BUFFERS.  THE INFO IS PASSED AROUND IN COMMON SO
C              WHEN THE USER RE-ENTERS A FILTER DIALOG HE WILL HAVE
C              THE PREVIOUS FILTER PARAMETERS SET UP BY DEFAULT.
C
C**********************************************************************

      INCLUDE 'sf_defs.i'

      INTEGER   HPASSFR, HCUTFR, HFREQ, HROFF

      COMMON/FILTPARAM/ IFTYPE, IDISP, IPHASE, ISAMP, IETIME

      COMMON/FILTORMS/  LCUTFR(3), LPASSFR(3), HPASSFR(3), HCUTFR(3),
     X                  ITIME_ORMS(3), NFILT_ORMS, IBTYPE,
     X                  ITAPER_ORMS, LENGTH_ORMS

      COMMON/FILTBUTT / LROFF(3), LFREQ(3), HFREQ(3), HROFF(3),
     X                  ITIME_BUTT(3), NFILT_BUTT, ITAPER_BUTT,
     X                  LENGTH_BUTT

      COMMON/FILTRICK / IPREDF(3), ITIME_RICK(3), NFILT_RICK, 
     X                  ITAPER_RICK, LENGTH_RICK


C
C  INITIALIZE FILTER PARAMETERS
C
      DO 7 I = 2, 3
        LCUTFR(I)     = 0
        LPASSFR(I)    = 0
        HCUTFR(I)     = 0
        HPASSFR(I)    = 0
        LROFF(I)      = 0
        LFREQ(I)      = 0
        HFREQ(I)      = 0
        HROFF(I)      = 0
        IPREDF(I)     = 0
        ITIME_ORMS(I) = 0
        ITIME_BUTT(I) = 0
7       ITIME_RICK(I) = 0


      IFTYPE    = ORMSBY_WAV
      IDISP     = 1
      IPHASE    = 0

      LCUTFR(1)     = 5
      LPASSFR(1)    = 15
      HCUTFR(1)     = 70
      HPASSFR(1)    = 55

      LROFF(1)      = 15
      LFREQ(1)      = 18
      HFREQ(1)      = 60
      HROFF(1)      = 36

      IPREDF(1)     = 30

      ITIME_ORMS(1) = 0
      ITIME_BUTT(1) = 0
      ITIME_RICK(1) = 0

      IBTYPE    = ORMSBY_BPASS

      ITAPER_ORMS = 100
      ITAPER_BUTT = 100
      ITAPER_RICK = 100
      LENGTH_ORMS = 400
      LENGTH_BUTT = 400
      LENGTH_RICK = 100
      ISAMP     = 2


      NFILT_ORMS = 1
      NFILT_BUTT = 1
      NFILT_RICK = 1


      RETURN
      END






      SUBROUTINE SYN_INIT_DIALOG(  IFEET, ISAVE, SYN_UWI, ICANCEL)
C***********************************************************************
C
C  ROUTINE:   SYN_INIT_DIALOG
C
C  FUNCTION:  BRING UP THE INITIALIZATION DIALOG FOR SYNTHETICS. ASK
C             THE USER IF HE WANTS TO USER THE CURRENT GLOBAL UWI (IF
C             EXISTING) OR ENTER ONE.  ALSO ASK IF METRIC OR ENGLISH
C             UNITS, AND IF SAVING SYNTHETICS IN DATABASE.
C
C  VARIABLES: 
C              
C             ICANCEL   = 0 IF NOT CANCEL HIT, 1 IF CANCEL IS HIT
C
C***********************************************************************

      INCLUDE 'synth2.ri'
      INCLUDE 'esi_am.i'
      INCLUDE 'esi_qs.i'
      INCLUDE 'esi_we_wells.i'

C
      CHARACTER*255 STRING, HELP
      CHARACTER*12  RES_FILE
      CHARACTER*(*) SYN_UWI
      CHARACTER     GLOBAL_UWI*(SIZEOF_UWI)
      DATA          HELP/'SYN_INIT'/

      INTEGER       CDLGID,CDITEM,CDVALUE,GLOBAL_UWI_FLAG

      NULL = 0
      ICANCEL=0
      ISAVE = 0
      GLOBAL_UWI_FLAG = 0
      RES_FILE = 'SYNTH2'

C
C  SEE IF THERE IS A CURRENT GLOBAL WELL
C
      CALL QSG_INQC( IQS_CUR_WELL, GLOBAL_UWI, LUWI)

C
C  OPEN THE DIALOG
C
      CALL AMG_DEFINE_RESOURCE(AM_DIALOG, RES_FILE, SYN_INIT,
     X                         NULL, ID,  NULL)
      CALL WIGOPCD(ID)

C
C  SET UP DEFAULTS
C

      SYN_UWI = ' '
      IF (GLOBAL_UWI.NE.' ') THEN
         GLOBAL_UWI_FLAG = 1
         SYN_UWI = GLOBAL_UWI
         CALL WIGSCDV(ID, SYN_INIT_GL_UWI_BUTT, 1)
         CALL WIGSCDT(ID, SYN_INIT_GL_UWI_TEXT, GLOBAL_UWI)         
      ELSE 
         CALL WIGSCDV(ID, SYN_INIT_USER_UWI_BUTT, 1)
         CALL WIGENCDI(ID, SYN_INIT_GL_UWI_BUTT, 0)
      ENDIF

      CALL WIGSCDV(ID, SYN_INIT_FEET, 1)
      IFEET = 1


100   CALL WIGRQCDI(CDLGID, CDITEM, CDVALUE, STRING)
C
C   CHECK PARAMETERS
C
C
C   CHECK UNITS  FIRST
C
      IF (CDITEM.EQ.SYN_INIT_FEET) THEN 
         IFEET = 1

      ELSEIF (CDITEM.EQ.SYN_INIT_METERS) THEN
         IFEET = 0
 
C
C  IF USING CURRENT GLOBAL UWI THEN COPY ELSE GET USER ENTERED UWI
C
      ELSEIF (CDITEM.EQ.SYN_INIT_GL_UWI_BUTT) THEN
          SYN_UWI = GLOBAL_UWI
      ELSEIF (CDITEM.EQ.SYN_INIT_USER_UWI_BUTT) THEN
C
C   THE WELL HAS TO BE DEFINED BY THE USER
C
         CALL WIGQCCDI(CDLGID, SYN_INIT_USER_UWI_TEXT, IDUM, STRING)
         IF(STRING.EQ.' ') THEN
            STRING = 'UWI Field is Blank. Enter a Valid UWI'
            CALL WIGALERT(1,HELP,STRING,' ')
            GOTO 100
         ELSE
            SYN_UWI = STRING                     
         ENDIF
C
C  CHECK IF SAVING CURVES
C
      ELSEIF (CDITEM.EQ.SYN_INIT_SAVE) THEN
          IF (ISAVE.EQ.1) THEN 
            ISAVE = 0
          ELSE
            ISAVE = 1
          ENDIF
C
C   CHECK OK OR CANCEL
C
      ELSEIF (CDITEM.EQ.SYN_INIT_OK) THEN 
C
C   CHECK THAT THERE IS REALLY A WELL DEFINED BY THE USER
C
         IF(GLOBAL_UWI_FLAG.EQ.1)    GOTO 9999
         CALL WIGQCCDI(CDLGID, SYN_INIT_USER_UWI_TEXT, IDUM, STRING)
         IF(STRING.EQ.' ') THEN
             STRING = 'UWI Field is Blank. Enter a Valid UWI'
             CALL WIGALERT(1,HELP,STRING,' ')
             GOTO 100
         ELSE
            SYN_UWI = STRING                     
            GOTO 9999
         ENDIF
      ELSEIF (CDITEM.EQ.SYN_INIT_CANCEL) THEN
            ICANCEL=1
            GOTO 9999
      ENDIF
C
C   STILL WAITING FOR OK OR CANCEL
C
      GOTO 100
   
9999  CALL AMG_RELEASE_RESOURCE(ID)
      RETURN
      END
C
C
      SUBROUTINE GOUPILLAUD( RR ,BB , NUSAMP, SURF_COEF, IERR )
C *********************************************************************
C
C   ROUTINE:   GOUPILLAUD
C
C   AUTHOR :   PIERO
C 
C   FUNCTION:  COMPUTE THE IMPULSION FILM WITH MULTIPLES BB
C		FROM THE REFLECTION COEFFICIENTS RR SAMPLED IN TIME
C		
C   VARIABLES: RR(INPUT) - INPUT BUFFER CONTAINING THE REFLECTION COEFFICIENTS
C	       BB(OUTPUT) - OUTPUT CONTAINING THE MULTIPLES
C	       NUSAMP (INPUT) - NUMBER OF REFLECTION COEFFICIENT
C	       SURF_COEF (INPUT) - FREE SURFACE REFLECTION COEFFICIENT
C	       IERR -  <0 IF PROBLEM 
C **********************************************************************
C
C
      INTEGER NUSAMP,I,J,NMAX
      DIMENSION RR(1), BB(1), DD(5000), UU(5000)
      REAL SURF_COEF
C
C
      SURF_COEF=ABS(SURF_COEF)
      IF (SURF_COEF.GT.1.) THEN
         IERR=-1
         RETURN
      ENDIF
      IF (NUSAMP.LE.2) THEN
         IERR=-2
         RETURN
      ENDIF
C
C	INITIALIZATION : PROPAGATE DOWNWARD THE ORIGINAL IMPULSE
C
      DD(1)=1.
      UU(1)=0.
C
      DO 100 I=2,NUSAMP
      UU(I) = 0.
100   DD(I)=(1.-RR(I-1))*DD(I-1)
      UU(NUSAMP+1)=0.
C
C	LET'S GO AN COMPUTE THE AMPLITUDES
C
      NMAX = NUSAMP
C
      DO 200 I=1,NUSAMP
C
C	UPGOING WAVES  I.E. REFLECTED  WAVES
C
      DO 300 J=1,NMAX
300   UU(J)=UU(J+1)+RR(J)*( DD(J)+UU(J+1) )
C
C       HERE ARE THE AMPLITUDES
C
      BB(I)=UU(1)
C
C	SHRINK THE LOOP SIZE TO COMPUTE ONLY THE USEFULL VALUES
C
      NMAX = NUSAMP - I
C
C	PROPAGATE THE DOWNGOING WAVES 
C
      DD(1)=-1*UU(1)*SURF_COEF
      DO 400 J=2,NMAX
400   DD(J)=DD(J-1) -RR(J-1)* ( UU(J)+DD(J-1) )
C
200   CONTINUE
C
      RETURN
      END
