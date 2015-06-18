C  DEC/CMS REPLACEMENT HISTORY, Element CT_GTP0.FOR
C  *3     7-JUN-1991 14:19:56 MARK "(SPR 000) Fix UTM for S. hemisphere, make it do more transformations"
C  *2    19-SEP-1989 10:10:40 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:47:18 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element CT_GTP0.FOR
C
      SUBROUTINE GTP$0(XIN,YIN,INSYS,INZONE,TPARIN,INUNIT,INSPH,
     .                 XOUT,YOUT,IOSYS,IOZONE,TPARIO,IOUNIT,IOSPH,IFLG)
C
C GENERAL PROGRAM FOR TRANSFORMATION BETWEEN VARIOUS REFERENCE SYSTEMS
C     MODIFIED VERSION OF GTRN$0 BY J.F. WAANANEN

C 7 JUNE, 1991   M. PASSOLT
C 	1. COMMENTED OUT CODE THAT MADE INAPPROPRIATE DETERMINATION AS TO
C	WHETHER OR NOT A TRANSFORMATION IS REQUIRED. (SEE AROUND STMT 120)
C	THE ROUTINE WILL NOW DO EXTRA WORK IN THE CASE THAT TWO PROJECTIONS
C	DIFFER ONLY IN THE UNIT OF MEASURE. HOWEVER, IT WILL NOW DO A
C	TRANSFORMATION IN THE CASE WHERE TWO PROJECTIONS DIFFER IN THEIR
C	PARAMETERS (TPARIN, TPARIO). AN EXAMPLE OF SUCH WOULD BE CHANGING
C	THE CENTRAL MERIDIAN IN A TM PROJECTION.
C
C	2. COMMENTED OUT A LINE THAT INCORRECTLY EVALUATED WHETHER OR NOT
C	A UTM PROJECTION WAS FOR THE SOUTHERN HEMISPHERE. IN THE S. HEMI.,
C	A FALSE NORTHING OF 10,000,000 METERS NEEDS TO BE ACCOUNTED FOR.
C
C	THE EFFECT OF THIS CHANGE IS THAT UTM PROJECTION DEFINITIONS *MUST*
C	INCLUDE THE UTM ZONE (1,...,60). FURTHERMORE, IF THE PROJECTION IS 
C	BE USED IN THE SOUTHERN HEMISPHERE, A REPRESENTATIVE POINT *MUST*
C	ALSO BE INCLUDED. IT IS THE SIGN OF THAT POINT THAT CLUES THE
C	PROGRAM THAT IT IS A SOUTHERN HEMISPHERE PROJECTION.

C INPUT ****                                                       ****
C CRDIN  : COORDINATES IN INPUT SYSTEM (2 DP WORDS ARRAY).
C INSYS  : CODE NUMBER OF INPUT COORDINATE SYSTEM (INTEGER).
C            =  0 , GEOGRAPHIC
C            =  1 , U T M
C            =  2 , STATE PLANE
C            =  3 , ALBERS CONICAL EQUAL-AREA
C            =  4 , LAMBERT CONFORMAL CONIC
C            =  5 , MERCATOR
C            =  6 , POLAR STEREOGRAPHIC
C            =  7 , POLYCONIC
C            =  8 , EQUIDISTANT CONIC
C            =  9 , TRANSVERSE MERCATOR
C            = 10 , STEREOGRAPHIC
C            = 11 , LAMBERT AZIMUTHAL EQUAL-AREA
C            = 12 , AZIMUTHAL EQUIDISTANT
C            = 13 , GNOMONIC
C            = 14 , ORTHOGRAPHIC
C            = 15 , GENERAL VERTICAL NEAR-SIDE PERSPECTIVE
C            = 16 , SINUSOIDAL
C            = 17 , EQUIRECTANGULAR (PLATE CARREE)
C            = 18 , MILLER CYLINDRICAL
C            = 19 , VAN DER GRINTEN I
C            = 20 , OBLIQUE MERCATOR (HOTINE)
C INZONE : CODE NUMBER OF INPUT COORDINATE ZONE (INTEGER).
C TPARIN : PARAMETERS OF INPUT REFERENCE SYSTEM (15 DP WORDS ARRAY).
C INUNIT : CODE NUMBER OF UNITS OF MEASURE FOR INPUT COORDS (INTEGER).
C            = 0 , RADIANS.
C            = 1 , FEET.
C            = 2 , METERS.
C            = 3 , SECONDS OF ARC.
C            = 4 , DEGREES OF ARC.
C INSPH  : INPUT SPHEROID CODE.  SEE SPHD$0 FOR PROPER CODES.
C OUTPUT ***                                                       ****
C IOSYS  : CODE NUMBER OF OUTPUT COORDINATE SYSTEM (INTEGER).
C IOZONE : CODE NUMBER OF OUTPUT COORDINATE ZONE (INTEGER).
C TPARIO : PARAMETERS OF OUTPUT REFERENCE SYSTEM (15 DP WORDS ARRAY).
C IOUNIT : CODE NUMBER OF UNITS OF MEASURE FOR OUTPUT COORDS (INTEGER).
C IOSPH  : OUTPUT SPHEROID CODE.  SEE SPHD$0 FOR PROPER CODES.
C CRDIO  : COORDINATES IN OUTPUT REFERENCE SYSTEM (2 DP WORDS ARRAY).
C IFLG   : RETURN FLAG (INTEGER).
C            = 0 , SUCCESSFUL TRANSFORMATION.
C            = 1 , ILLEGAL INPUT SYSTEM CODE.
C            = 2 , ILLEGAL OUTPUT SYSTEM CODE.
C            = 3 , ILLEGAL INPUT UNIT CODE.
C            = 4 , ILLEGAL OUTPUT UNIT CODE.
C            = 5 , INCONSISTANT UNIT AND SYSTEM CODES FOR INPUT.
C            = 6 , INCONSISTANT UNIT AND SYSTEM CODES FOR OUTPUT.
C            = 7 , ILLEGAL INPUT ZONE CODE.
C            = 8 , ILLEGAL OUTPUT ZONE CODE.
C      OTHERWISE , ERROR CODE FROM PROJECTION COMPUTATIONAL MODULE.
C
      IMPLICIT REAL*8 (A-H,O-Z)
      IMPLICIT INTEGER*4 (I-N)

      INTEGER*4 SYSUNT(21),IFLG
      REAL*8 CRDIN(2),CRDIO(2),TPARIN(15),TPARIO(15),COORD(2)
      REAL*8 DUMMY(2)
      COMMON/ERRM$0/ IERROR
      COMMON/PROJ$0/ IPROJ
C
C 12/20/88 SC - BLOCK DATA IS NOT PORTABLE, SO INIT ELLIPSOID TO CLARK'S 1866
C		PARAMETERS HERE.
C               
      COMMON/SPHR$0/ A$$
      COMMON/ELLP$0/ A$,E$,ES$,E0$,E1$,E2$,E3$

      DATA A$  /0.6378206400000000D+07/
      DATA E$  /0.8227185422300323D-01/
      DATA ES$ /0.6768657997291094D-02/
      DATA E0$ /0.9983056818784341D+00/
      DATA E1$ /0.2542555507651308D-02/
      DATA E2$ /0.2698084527466011D-05/
      DATA E3$ /0.1003393903560134D+01/

      DATA A$$ /0.6370997000000000D+07/

      DATA SYSUNT / 0 , 2 , 1 , 18*2 /
      DATA MAXUNT,MAXSYS / 4 , 20 /

C
C	PRIME-ONLY INITIALIZATIONS COMMENTED OUT...JGG
C
C      DATA JFLAG/0/

C      IF (JFLAG.NE.0) GO TO 10
C      E$ = 0.0D0
C      ES$ = 0.0D0
C      CALL SPHD$0(0,DUMMY)
C      JFLAG = 1
C
C	END PRIME-ONLY EXORCISM

      CRDIN(1)=XIN
      CRDIN(2)=YIN
      IPROJ=INSYS
   10 INZONE = IABS(INZONE)
      IOSPH  = IABS(IOSPH)
      INSPH  = IABS(INSPH)
C
C CHECK VALIDITY OF CODES FOR UNITS OF MEASURE AND REFERENCE SYSTEMS.
      IF (INSYS.GE.0 .AND. INSYS.LE.MAXSYS) GO TO 020
      IFLG = 1
      RETURN
  020 IF (IOSYS.GE.0 .AND. IOSYS.LE.MAXSYS) GO TO 040
      IFLG = 2
      RETURN
  040 IF (INUNIT.GE.0 .AND. INUNIT.LE.MAXUNT) GO TO 060
      IFLG = 3
      RETURN
  060 IF (IOUNIT.GE.0 .AND. IOUNIT.LE.MAXUNT) GO TO 080
      IFLG = 4
      RETURN
C
C CHECK CONSISTANCY BETEEN UNITS OF MEASURE AND REFERENCE SYSTEM.
  080 IUNIT = SYSUNT(INSYS + 1)
      CALL UNTF$0 (INUNIT,IUNIT,FACTOR,IFLG)
      IF (IFLG .EQ. 0) GO TO 100
      IFLG = 5
      RETURN
  100 COORD(1) = FACTOR * CRDIN(1)
      COORD(2) = FACTOR * CRDIN(2)
      IUNIT = SYSUNT(IOSYS + 1)
      CALL UNTF$0 (IUNIT,IOUNIT,FACTOR,IFLG)
      IF (IFLG .EQ. 0) GO TO 120
      IFLG = 6
      RETURN
C
CC    THE FOLLOWING TEST WAS IMPROPERLY IMPLEMENTED - IT REJECTED
C     WHERE PARAMETERS IN TPARIN OR TPARIO DISTINGUISHED BETWEEN THE
C     PROJECTIONS. M. PASSOLT 7 JUNE, 1991
  120 CONTINUE
CC	SEE IF INPUT/OUTPUT SYSTEMS MATCH
CC
C  120 IF (INSYS.NE.IOSYS.OR.INZONE.NE.IOZONE) GO TO 140
C      IF (IOSPH.NE.INSPH) GO TO 140
C      CRDIO(1) = FACTOR * COORD(1)
C      CRDIO(2) = FACTOR * COORD(2)
C      XOUT=CRDIO(1)
C      YOUT=CRDIO(2)
C      RETURN
C
C COMPUTE TRANSFORMED COORDINATES AND ADJUST THEIR UNITS.
C
  140 IF (INSYS .EQ. 0) GO TO 540

C
C INVERSE TRANSFORMATION.
  200 IPROJ=INSYS
      IF (INSYS.GT.2) CALL SPHD$0(INSPH,TPARIN)
      GO TO (210,220,230,240,250,260,270,280,290,300,
     .       310,320,330,340,350,360,370,380,390,400) , INSYS
C
C	Make sure UTM zone is reasonable
C
  210 IF (INZONE .GT. 60) THEN
        IFLG = 7
        RETURN
      END IF
CC      IF (INZONE.EQ.0.AND.TPARIN(1).NE.0.0D0) GO TO 215
C
C	CONVERT INPUT COORD TO 'PACKED' FMT - ONLY CARE ABOUT DEGREES
C	(THIS SELECTS UTM ZONE OF INPUT POINT, IF ZONE NOT SPECIFIED)
C
CC      M. PASSOLT  : NOTE THAT COORD() IS IN *METERS* AND SO PUTTING
CC      7 JUNE, 1991:            IT IN PACKED FORMAT MAKES NO SENSE!
CC      TPARIN(1) = COORD(1) * 1.0D6
CC      TPARIN(2) = COORD(2) * 1.0D6

CC    INITIALIZE DUMMY
        DUMMY(1) = 0.0D0
        DUMMY(2) = 0.0D0
  215 CALL SPHD$0(INSPH,DUMMY)
      TPARIN(14) = DUMMY(1)
      TPARIN(15) = DUMMY(2)
      CALL IS1A$0(INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI1A$0 (COORD,CRDIO)
      GO TO 500
  220 CALL IS02$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI02$0 (COORD,CRDIO)
      GO TO 500
  230 CALL IS03$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI03$0 (COORD,CRDIO)
      GO TO 500
  240 CALL IS04$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI04$0 (COORD,CRDIO)
      GO TO 500
  250 CALL IS05$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI05$0 (COORD,CRDIO)
      GO TO 500
  260 CALL IS06$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI06$0 (COORD,CRDIO)
      GO TO 500
  270 CALL IS07$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI07$0 (COORD,CRDIO)
      GO TO 500
  280 CALL IS08$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI08$0 (COORD,CRDIO)
      GO TO 500
  290 CALL IS09$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI09$0 (COORD,CRDIO)
      GO TO 500
  300 CALL IS10$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI10$0 (COORD,CRDIO)
      GO TO 500
  310 CALL IS11$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI11$0 (COORD,CRDIO)
      GO TO 500
  320 CALL IS12$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI12$0 (COORD,CRDIO)
      GO TO 500
  330 CALL IS13$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI13$0 (COORD,CRDIO)
      GO TO 500
  340 CALL IS14$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI14$0 (COORD,CRDIO)
      GO TO 500
  350 CALL IS15$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI15$0 (COORD,CRDIO)
      GO TO 500
  360 CALL IS16$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI16$0 (COORD,CRDIO)
      GO TO 500
  370 CALL IS17$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI17$0 (COORD,CRDIO)
      GO TO 500
  380 CALL IS18$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI18$0 (COORD,CRDIO)
      GO TO 500
  390 CALL IS19$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI19$0 (COORD,CRDIO)
      GO TO 500
  400 CALL IS20$0 (INZONE,TPARIN)
      IF (IERROR .NE. 0) GO TO 500
      CALL PI20$0 (COORD,CRDIO)
  500 IFLG = IERROR
      IF (IFLG .NE. 0) RETURN
      IF (IOSYS .EQ. 0) GO TO 920
      COORD(1) = CRDIO(1)
      COORD(2) = CRDIO(2)
C
C FORWARD TRANSFORMATION.
  540 IPROJ=IOSYS
      IF (IOSYS.GT.2) CALL SPHD$0(IOSPH,TPARIO)
      GO TO (610,620,630,640,650,660,670,680,690,700,
     .       710,720,730,740,750,760,770,780,790,800) , IOSYS
C
C	CALC. A POINT IN THE UTM ZONE, IF SUPPLIED ZONE IS ZERO
C
  610 IF (IOZONE .GT. 0) GO TO 615
      TPARIN(1) = COORD(1)
      TPARIN(2) = COORD(2)
  615 CALL SPHD$0(IOSPH,DUMMY)
      TPARIO(14) = DUMMY(1)
      TPARIO(15) = DUMMY(2)
      CALL IS1A$0(IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF1A$0 (COORD,CRDIO)
      GO TO 900
  620 CALL IS02$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF02$0 (COORD,CRDIO)
      GO TO 900
  630 CALL IS03$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF03$0 (COORD,CRDIO)
      GO TO 900
  640 CALL IS04$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF04$0 (COORD,CRDIO)
      GO TO 900
  650 CALL IS05$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF05$0 (COORD,CRDIO)
      GO TO 900
  660 CALL IS06$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF06$0 (COORD,CRDIO)
      GO TO 900
  670 CALL IS07$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF07$0 (COORD,CRDIO)
      GO TO 900
  680 CALL IS08$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF08$0 (COORD,CRDIO)
      GO TO 900
  690 CALL IS09$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF09$0 (COORD,CRDIO)
      GO TO 900
  700 CALL IS10$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF10$0 (COORD,CRDIO)
      GO TO 900
  710 CALL IS11$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF11$0 (COORD,CRDIO)
      GO TO 900
  720 CALL IS12$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF12$0 (COORD,CRDIO)
      GO TO 900
  730 CALL IS13$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF13$0 (COORD,CRDIO)
      GO TO 900
  740 CALL IS14$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF14$0 (COORD,CRDIO)
      GO TO 900
  750 CALL IS15$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF15$0 (COORD,CRDIO)
      GO TO 900
  760 CALL IS16$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF16$0 (COORD,CRDIO)
      GO TO 900
  770 CALL IS17$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF17$0 (COORD,CRDIO)
      GO TO 900
  780 CALL IS18$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF18$0 (COORD,CRDIO)
      GO TO 900
  790 CALL IS19$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF19$0 (COORD,CRDIO)
      GO TO 900
  800 CALL IS20$0 (IOZONE,TPARIO)
      IF (IERROR .NE. 0) GO TO 900
      CALL PF20$0 (COORD,CRDIO)
  900 IFLG = IERROR
  920 CRDIO(1) = FACTOR * CRDIO(1)
      CRDIO(2) = FACTOR * CRDIO(2)
      XOUT=CRDIO(1)
      YOUT=CRDIO(2)
      RETURN
      END
