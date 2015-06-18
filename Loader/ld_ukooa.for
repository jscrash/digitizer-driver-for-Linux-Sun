C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA.FOR
C  *5    27-SEP-1989 09:49:27 GORDON "(SPR 5002) Change from SUBROUTINE back to PROGRAM"
C  *4    19-SEP-1989 12:15:52 GORDON "(SPR 100) latest Gulf version"
C  *3    19-SEP-1989 12:13:07 GORDON "(SPR 100) latest Gulf version"
C  *2     4-AUG-1989 11:42:03 ADEY "(SPR -1) UPDATE TO THE NEW UKOOA LOADER VERSION 8/4/89 KPA"
C  *1     3-AUG-1989 15:02:25 ADEY "Entry point for the UKOOA loader"
C  DEC/CMS REPLACEMENT HISTORY, Element LD_UKOOA.FOR
C     
C     $Header: /j/finder/main/RCS/ld_ukooa.for,v 1.1 2008/02/09 09:23:35 julian Exp julian $ ld_ukooa.f 
C     
***********************************************************
*                 UKOOA LOADER MAIN ROUTINE               *
***********************************************************
*    
C  06/29/87  KARL  MOD FOR LAT/LONG DDDMMSS.S RE:CHOIX INITIALIZATION
C  07/08/89  KARL  MOD NO LONGER COMPUTE INTERSECTIONS - FR SCD
C  07/31/89  KARL  MOD ROUTINE NAMES FOR THE 'UK_' PREFIX
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
* FILES USED      : NO
*
* FILES CREATED   : NO
*
* TYPE:
* SOURCE FILES USED   :
*       
*             ===> LD_UKUTIL.FOR
*		   LD_UKSTORLIN.PFO
*		   LD_UKOPAREA.PFO
*                  LD_UKDECOD.FOR
*                  LD_UKTOOLS.FOR    
*
* TASK IMAGE: NO
*
*---------------------------------------
        PROGRAM LD_UKOOA
*---------------------------------------
*
	INCLUDE 'esi$include:ld_ukooa2.i'
*
*    COMMON PARAM used in the data base interface
*    giving the type of the survey
*
        COMMON /PROGRAM/ NPARAM(2)
        INTEGER*4 NPARAM
        CHARACTER*4 CNPARAM
        EQUIVALENCE ( CNPARAM , NPARAM )
*
	REAL*8 CPU1 , CPU2 , DIF_CPU
*
*  declaration of function
*
        REAL*8 HO_CPU_TM_GC
*
	CHARACTER*100 DAT_TIM
*
************************************
*
*   initialization of logical units
*
************************************

        DATA NBENT, IFG / 0, 0 /
C
c
c       initialise common program ( for 2d survey )
C
        CNPARAM = '2D' 
C
C     initialize the sql cursor logical ( in common ) 
C
	FIRST_SQL = .TRUE.
C
c ... initialize  logical units
C
      CALL  UK_INITLU ( ISTAT)

      IF(ISTAT .NE. 0 ) THEN
         WRITE (LIST, 1000)
1000     FORMAT(///' ************************************',/,
     -             ' * ERROR INITIALIZING LOGICAL UNITS *',/,
     -             ' *                                  *',/,
     -             ' *   TERMINATING THE UKOOA LOADER   *',/,
     -             ' ************************************',//)
         STOP
      ENDIF

c ... initialization  of variables
c
C

	CPU1 = HO_CPU_TM_GC() 

	CALL HOG_CTIME ( DAT_TIM )

	WRITE ( LIST , 5000 ) DAT_TIM  
5000    FORMAT ( /,' *****************************',
     -             '*****************************',
     2/,' ****    BEGINNING OF LOADING  : ',A50,/,
     3            '* *****************************',
     -             '*****************************')



      DO 3 KN = 1, L_TAB   
3     ITABL(KN) = 0
*
c
c ... read parameter file filled in the interactive part or by the
c       command  file
c
      CALL UK_RDPAR (HSIZE, LRECL, ISTAT)
C
      IF ( ISTAT .NE. 0  ) THEN
         CALL UK_LOGMES ( C49 , TIME , I1,I2,I3,I4 )
         CALL UK_LOGMES ( C25 , TIME , I1, I2,I3,I4 )
         GO TO  9999
      ENDIF
C
c ... login to the data base
c         fill the CLIGNE array with line names already
c         loaded in the data base for this survey
C
      CALL UK_OPAREA (ISTAT)
      IF (ISTAT .NE. 0) GO TO 9999
C
c
c ... operation on the data file : read - transfer of data in
c
      CALL UK_READMAG ( LRECL, HSIZE, ISTAT)
c
C
C      
*
 8000 CONTINUE
C
C
	CALL HOG_CTIME ( DAT_TIM )

	WRITE ( LIST , 5001 ) DAT_TIM
5001    FORMAT ( /,'*******************************************',
     2/,'****    END  OF LOADING  : ',A50 ,/ )

	CPU2 = HO_CPU_TM_GC() 
	DIF_CPU = CPU2 - CPU1


	WRITE ( LIST , 5002 ) DIF_CPU
5002    FORMAT (/,'****    CPU TIME  : ',F15.3 ,' seconds ' ,/,
     3'******************************************************')

c
C
        CPU1 = 0

	CPU1 = HO_CPU_TM_GC() 

	CALL HOG_CTIME ( DAT_TIM )

	WRITE ( LIST , 5005) DAT_TIM  
5005    FORMAT (/,/, /,'*****************************************',
     2/,'****    BEGINNING OF INTERSECTION COMPUTATION  : ',A50,/,
     3'******************************************************')






c    call the line intersection programm - this programm will compute 
c    the intersections between the new lines loaded and the rest
c    of the lines already in the survey -  this program is a written
c    in C   so we first call a fortran routine to prepare the parameters
c    then a c glue routine which will cal the intersection program itself
c
c COMMENTED OUT KPA 7/8/89
C         CALL LD_INTER ( ISTAT ) 
C
        WRITE (LIST, 5010)
5010    FORMAT(//,' ******* NO INTERSECTION PROCESSING *******',//)
C
	CALL HOG_CTIME ( DAT_TIM )
	CPU2 = HO_CPU_TM_GC() 
	DIF_CPU = CPU2 - CPU1
C
C     print a report of the intersections computed
c COMMENTED OUT KPA 7/8/89 NO INTERSECTION COMPUTED ANY LONGER
C            CALL INREPORT ( LIST ) 
C
	WRITE ( LIST, 5006 ) DAT_TIM
5006    FORMAT (/,/, /,'*******************************************',
     2/,'****    END  OF INTERSECTION COMPUTATION : ',A50 ,/ )
C
	WRITE ( LIST, 5002 ) DIF_CPU
C
C
        CLOSE (LIST)
	IF (ISTAT .NE. 0) THEN
            PRINT*, ' ********************************************'
            PRINT*, ' ERROR OCCURED DURING UKOOA RUN,--'
            PRINT*, ' CHECK THE JOURNAL FILE'
            PRINT*, ' ********************************************'
        ENDIF
C
9999    CONTINUE
C
C
        STOP
        END

