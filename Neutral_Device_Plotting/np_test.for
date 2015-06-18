C DEC/CMS REPLACEMENT HISTORY, Element NP_TEST.FOR
C *3    28-NOV-1989 14:42:04 GORDON "(SPR 0) lowercase include files"
C *2    11-AUG-1989 17:05:43 VINCE "(SPR -1) last fixes from UNIX port."
C *1    10-AUG-1989 18:51:07 VINCE "Fortran code after UNIX mods"
C DEC/CMS REPLACEMENT HISTORY, Element NP_TEST.FOR
C---------------------------------------------------------
C
C TEST DRIVER FOR NP_SIZE SUBROUTINE
C
      PROGRAM     NP_TEST
      INCLUDE      'npdefs.i'
C
      INTEGER      LUN,STATUS
      LOGICAL      REPORT,OVERFLOW

      ASSIGN 990 TO EXIT_VEC
      ASSIGN 100 TO ERROR_VEC
C
C Julian Carlisle   April  27, 1988
C---------------------------------------------------------
C
      LUN = INPUT_UNIT
      STATUS = SUCCESS
      REPORT = .TRUE.
C
      OPEN(UNIT = LUN, ERR = 110, STATUS = 'OLD')
      CALL NP_REPORT(LUN,REPORT,OVERFLOW,STATUS)
C
      CLOSE(UNIT=LUN)
      IF (STATUS .EQ. SUCCESS) THEN
	    STATUS = 1
	    GOTO EXIT_VEC
      ENDIF
C
C ---- ERROR VECTOR -----
C
C DECODE & DISPLAY ERROR MESSAGE ASSOC. W/STATUS
C
100   IF (STATUS .EQ. FILE_ERROR)
     1      PRINT *, 'Unexpected end of file reached....'
      GO TO EXIT_VEC

110   IF (STATUS .EQ. CORRUPT_FILE)
     1      PRINT *,'Bad file type or format...'
      GO TO EXIT_VEC

120   IF (STATUS .EQ. BAD_VERSION)
     1      PRINT *, 'Bad Version number...'
      GO TO EXIT_VEC

130   IF (STATUS .EQ. READ_ERROR)
     1      PRINT *,'Error reading file '
      GO TO EXIT_VEC
C
C---- EXIT VECTOR ----
C
990   CALL EXIT(STATUS)
      END
C
                                                  
