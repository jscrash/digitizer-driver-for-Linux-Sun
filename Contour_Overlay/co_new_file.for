C  DEC/CMS REPLACEMENT HISTORY, Element CO_NEW_FILE.FOR
C  *4    25-SEP-1991 12:04:58 MING "(SPR 4064) comment out INQUIRE function"
C  *3     5-AUG-1991 14:27:29 MING "(SPR 0) initialize LUN_OUT"
C  *2    19-SEP-1989 10:09:33 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:46:19 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element CO_NEW_FILE.FOR
C
C -----------------------------------------------------------
      SUBROUTINE CO_NEW_FILE(FILE_NAME,LUN_OUT,STATUS)
C
C--- Open a FORTRAN file
C
C --- Input:  FILE_NAME - A legal file name
C --- Output: LUN_OUT   - The file number assigned
C             STATUS    - O = SUCCESS, -1 = FAIL
C
      INTEGER*4 LUN_OUT
      CHARACTER*(*) FILE_NAME
C      LOGICAL OPN
      INTEGER*4 STATUS

      LUN_OUT = 0


C
C --- Check if already opened
C

C --- Comment out since this function does not work in FINDER
C     It returns random non-zero integer which means TRUE for the 
C     varibale OPN, even if the file dose not exist
C
C      INQUIRE(FILE=FILE_NAME,OPENED=OPN,NUMBER=LUN_OUT)
C

C
C --- If not already opened, get a file number and open
C

C      IF (.NOT. OPN) THEN

C        CALL HOGETFUNIT(LUN_OUT)                                 SC 1/23/89
	 CALL HOGFNT(LUN_OUT)
	 IF (LUN_OUT .LT. 0) GOTO 1000
	 OPEN(UNIT=LUN_OUT,FILE=FILE_NAME,STATUS='UNKNOWN',ERR=1000)

C      ENDIF

      STATUS = 0
      RETURN
C
C --- ERROR
C
1000  STATUS = -1
      LUN_OUT = 0
      RETURN
C
      END

                                     
