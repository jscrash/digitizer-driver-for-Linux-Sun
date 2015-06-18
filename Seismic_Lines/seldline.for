C  DEC/CMS REPLACEMENT HISTORY, Element SELDLINE.FOR
C  *3    19-NOV-1990 10:37:18 PURNA "(SPR 6166) remove extra char on line_name"
C  *2    19-SEP-1989 10:15:48 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:51:27 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element SELDLINE.FOR
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
* DATE : 01/04/1987
*
* FUNCTION : 
*             Routine used to sed the parameters in commons used by the
*             the fortran routine STORLIN used  to load the position
*
*             
* LANGUAGE: FORTRAN F77
*
* FILES USED      : NO
*
* FILES CREATED   : NO
*
* TYPE:
* ROUTINES USED   :
*             *
*            *
*
*/


	SUBROUTINE SELDLINE ( LINENAME , BUF , XBUF , NBPOINTS , ISTAT)

	INCLUDE 'ld_ukooa2.i'
        INCLUDE 'esi_qs.i'

	CHARACTER*(*)  LINENAME
        INTEGER*4 BUF (*) , NBPOINTS ,ISTAT
        REAL*4 XBUF(*)
	INTEGER*4  NBW_SP , INDEB
        CHARACTER*30  LINEN ,SURV
        INTEGER*4 HO_GET_FORTRAN_STDOUT
C
C
C     NBW_SP IS THE NUMBER OF WORDS ( 4 BYTES ) DESCRIBING A POINT
C      3 WORDS
C
C     INDEB IS THE INDEX IN THE BUFFER OF THE FIRST POINT WE LOAD
C
C
       NBW_SP  =  3
       INDEB   =  1
C
C
C     INITIALISE COMMONS USED 
C
C      SPLIT THE LONG LINE NAME
C
	    LINEN = ' '
	    SURV = ' '
            CALL SEGSPLIT ( LINENAME , LINEN , SURV )
C
C      COPY SURVEY NAME TO  SURV_NAM  
C
            SURV_NAM = SURV
C
C    LENGTH OF THE SURVEY NAME
C

            ILSURV =  ISTRLEN ( SURV_NAM )
         
C   NO CONVERSION OF PROJECTIONS

      PROJCONV = .FALSE.
C
C    FILL AREANAME WITH THE PROJECT NAME
C    NB_CHAR   NUMBER OF NON BLANK CHARACTERS IN THE PROJECT NAME
C
      NB_CHAR = 0 
      ISTAT = QSG_INQC ( IQS_PROJECT_NAME , AREANAME , NB_CHAR )
      NB_CHAR = ISTRLEN ( AREANAME )
C

C
C    INITIALISE LOGICAL UNIT LIST FOR THE DISPLAY OF MESSAGES
C
C 12/20/88 GS Don't use hard-coded output unit number
      LIST = HO_GET_FORTRAN_STDOUT()
C
C
       REPLINE = 0
C
        NEWLINE = 0
C
        IETADB = 1
C    
C
C      CALL TO THE ROUTINE PERFORMING THE LOADING OF THE INFORMATION
C
C
        CALL UK_STORLIN (  BUF , XBUF , INDEB , NBPOINTS ,NBW_SP , 
     +                 LINEN , ISTAT )


	RETURN
        END  


