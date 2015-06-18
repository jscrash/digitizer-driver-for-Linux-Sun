C  DEC/CMS REPLACEMENT HISTORY, Element TF_DELFIL.FOR
C  *3    20-NOV-1990 09:24:44 MING "(SPR 6173) add closefil"
C  *2    19-SEP-1989 10:19:01 GORDON "(PURNA) GULF MODS UNDER SPR 100"
C  *1    10-AUG-1989 18:53:47 VINCE "Fortran code after UNIX mods"
C  DEC/CMS REPLACEMENT HISTORY, Element TF_DELFIL.FOR
C
C ****************************************************************************
C
      SUBROUTINE DELFIL(FILE,IERR)
C
       CHARACTER*(*) FILE
       CALL HOGFNT(IUNIT)
       IERR=0
       OPEN(FILE=FILE,UNIT=IUNIT,STATUS='OLD',ERR=99)
       CLOSE(IUNIT,STATUS='DELETE')
       RETURN
99     IERR=1
       RETURN
       END


C ------------------------------------------------------------

      SUBROUTINE CLOSEFIL(FUNIT)
C
C %S  Close a file open by fortran
C
C %P  IN:
C %P    STRIN1  = file unit
C
      INTEGER FUNIT
C
      CLOSE (FUNIT)

      RETURN
      END
C ------------------------------------------------------------
