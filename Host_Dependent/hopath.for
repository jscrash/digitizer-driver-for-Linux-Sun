C DEC/CMS REPLACEMENT HISTORY, Element HOPATH.FOR
C *2     1-MAY-1991 12:37:28 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:10:42 KEE "dls/nts"
C DEC/CMS REPLACEMENT HISTORY, Element HOPATH.FOR
c********************************************************************
c
c
c                 Copyright Gulf Canada Resources, 1990
c                  Unpublished -- All rights reserved
c
c THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
c IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
c WITHOUT PRIOR WRITTEN CONSENT OF
c
c                       Gulf Canada Resources, Inc.
c                       401 9th Avenue SW
c                       Calgary, Alberta
c                       Canada T2P 2H7
c                       (403) 233-4000
c
c (The above notice does not constitute an admission of publication or
c  unrestricted dissemination of the work)
c*********************************************************************
c list HOPATH
c   
c     *** Get full path name for generic file ***
c
c     Calling sequence:
c      
c     call HOPATH(genam,path,irtn)
c     
c     Arguments:
c     
c     Name   Input/       Type         Description   
c            output
c
c     genam    I      character*(*)    Generic file name
c   
c     path     O      character*(*)    Full pathname including extension
c
c     irtn     O         Integer       Return code
c
c     Comments:
c
c     This routine examins a system file looking for the full pathname
c     for the input generic file name.
c
c nolist HOPATH
c
      subroutine HOPATH(genam,path,irtn)
      parameter (maxpth=200)
      character*(*) genam
      character*16 gnames(maxpth),gtest
      character*255 string,paths(maxpth)
      character*240 gpaths,path
      integer CHTRIM
      data npath/0/,iounit/2/
      data string/
     &'fgs_ats_paths.dat'/
      gpaths=' '
      call HOG_ADD_PATH('ESI$BIN',string,LEN(string),gpaths)
      irtn=0
      if(npath.eq.0)then
c     
c        Read in path names from pathname file
c
         OPEN(unit=iounit,file=gpaths,status='OLD',iostat=jstat)
         if(jstat.ne.0)go to 180
         do50 i=1,maxpth
         read(iounit,10,end=60,err=60)gnames(i),paths(i)
 10      format(a,1x,a)
         npath=i
 50      continue
 60      if(npath.eq.0)go to 180
      end if
      namlen=CHTRIM(genam)
c
c     Search entries looking for generic file name
c
      do100 i=1,npath
      call HOG_ADD_PATH('ESI$BIN',paths(i),LEN(paths(i)),path)
      gtest=gnames(i)
      if(gtest(1:namlen).eq.genam(1:namlen))go to 200
 100  continue
 180  irtn=-1
 200  return
      end
