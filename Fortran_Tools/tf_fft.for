C  DEC/CMS REPLACEMENT HISTORY, Element TF_FFT.FOR
C  *2    19-SEP-1989 10:19:09 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:53:52 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TF_FFT.FOR
C
C ****************************************************************************
C
      SUBROUTINE FFT(N,PPP,SINN)
C
C PERFORMS A FORWARD OR REVERSE FOURIER TRANSFORM ON A COMPLEX ARRAY
C
C N     = BINARY EXPONENT OF NUMBER OF POINTS IN ARRAY X
C PPP   = COMPLEX ARRAY TO TRANSFORM
C SINN  = 1. = FORWARD TRANSFORM, -1. = REVERSE TRANSFORM
C
      COMPLEX *16 A,WK,HOLD,Q,X(8192)
      REAL*8 ZTEMP
      COMMON/HNGLIBCOM/X
      COMPLEX PPP(1)
      DIMENSION M(15)
C
      LX=2**N
      IF(LX.GT.8192)THEN
        PPP(1)=-999.
        RETURN
      ENDIF
C
      DO 1005 JK = 1 , LX
         X(JK)=PPP(JK)
 1005 CONTINUE
C
      DO 100 I=1,N
         M(I)=2**(N-I)
  100 CONTINUE
C
      DO 101 L=1,N
         NBLOCK=2**(L-1)
         LBLOCK=LX/NBLOCK
         LBHALF=LBLOCK/2
         K=0
         DO 102 IBLOCK=1,NBLOCK
            FK=K
            FLX=LX
            V=SINN*6.2832*FK/FLX
            WK=DCMPLX(COS(V),SIN(V))
            ISTART=LBLOCK*(IBLOCK-1)
            DO 103 I=1,LBHALF
               J=ISTART+I
               JH=J+LBHALF
               ZTEMP=DREAL(X(JH))
               IF(ZTEMP .LT. 1.0E-35 .AND. ZTEMP. GT. -1.0E-35)
     X             X(JH)=0.0
               Q=X(JH)*WK
               X(JH)=X(J)-Q
               X(J)=X(J)+Q
  103       CONTINUE
            DO 104 I=2,N
               II=I
               IF (K.LT.M(I)) GOTO 200
               K=K-M(I)
  104       CONTINUE
  200       K=K+M(II)
  102    CONTINUE
  101 CONTINUE
      K=0
      DO 105 J=1,LX
           IF (K.LT.J) GOTO 201
           HOLD=X(J)
           X(J)=X(K+1)
           X(K+1)=HOLD
           A=X(K+1)
  201      DO 106 I=1,N
                II=I
                IF (K.LT.M(I)) GOTO 202
                K=K-M(I)
  106      CONTINUE
  202      K=K+M(II)
  105 CONTINUE
      IF (SINN.LT.0.) FLX=1
C
C fix up funny range values
C
      DO 107 I=1,LX
           X(I)=X(I)/FLX
CC           IF(REAL(X(I)).GT. 1.0E38)X(I)=1.E38
CC           IF(REAL(X(I)).LT.-1.0E38)X(I)=-1.E38
CC           IF(REAL(X(I)).GT. -1.0E-38 .AND. REAL(X(I)).LT. 1.0E-38)
CC     X        X(I)=0.
           PPP(I)=X(I)
  107 CONTINUE
      RETURN
      END
