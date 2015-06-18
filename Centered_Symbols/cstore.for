C DEC/CMS REPLACEMENT HISTORY, Element CSTORE.FOR
C *2     1-MAY-1991 12:36:44 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:05:50 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element CSTORE.FOR
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
c list CSTORE
c
c     *** Subroutine to store SAVE and GET character strings ***
c
c     Calling sequence:
c
c     call CSTORE(comnd,string,irtn)
c
c     Arguments:
c
c     Name    Input/        Type       Description
c             outout
c
c     comnd     I       Character*(*)  Command string
c     
c     string   I,O      Character*(*)  Character string
c   
c     irtn      O         Integer      Return code: -1=failed
c
c     Comments:
c
c     This routine can be used to save strings in a data area
c     accessable by other subroutines without using hidden common
c     blocks.  This improves the portability of the Fortran code.
c
c     Valid commands are:
c
c     SAVE    - save data array
c     GET     - get data array
c     UPDATE  - update data array
c     DELETE  - delete data array
c     PRINT   - print an array
c     REPORT  - report on any or all arrays
c
c nolist CSTORE
c
      subroutine CSTORE(comnd,string,irtn)
      parameter (maxsav=100,maxval=5000)
      character*(*) comnd,string
      character*20 name,func
      character*200 save
      character*2000 cbuf
      integer sepfld(2,5),sepstr(2,maxsav),sepnam(2,maxsav),
     &CHTRIM,lcols(2)
      character*200 names
      data names/'NULL'/,cbuf/'-NULL-'/,
     &sepnam/1,4,198*0/,sepstr/1,6,198*0/,nsave/1/
      irtn=0
c
c     Separate input command into words
c
      nfld=10
      call CHWORD(comnd,sepfld,nfld)
      if(nfld.lt.2)go to 180
      func=comnd(sepfld(1,1):sepfld(2,1))
      name=comnd(sepfld(1,2):sepfld(2,2))
      lenam=CHTRIM(name)
      lenstr=CHTRIM(string)
      if(lenam.eq.0)go to 200
c
c     Check for stored name
c
      call CHCOMP(names,sepnam,nsave,name,match)
      if(func(1:4).eq.'SAVE'.and.match.gt.0)func='UPDATE'
      if(func(1:4).eq.'SAVE')then
         if(lenstr.eq.0)go to 200
c
c        Save character string
c  
         call CHADD(names,name,'|',irtn)
         if(irtn.ne.0)go to 200
         call CHADD(cbuf,string,'|',irtn)
         if(irtn.ne.0)go to 200
         nsave=nsave+1
         if(nsave.gt.maxsav)go to 180
         sepnam(1,nsave)=sepnam(2,nsave-1)+2
         sepnam(2,nsave)=sepnam(1,nsave)+lenam-1
         sepstr(1,nsave)=sepstr(2,nsave-1)+2
         sepstr(2,nsave)=sepstr(1,nsave)+lenstr-1
      else if(func(1:3).eq.'GET')then
         if(match.eq.0)go to 180
         string=cbuf(sepstr(1,match):sepstr(2,match))
      else if(func(1:6).eq.'UPDATE')then
c
c        Update data string and redefine column markers
c
         if(match.eq.0)go to 180
         save=cbuf(sepstr(1,match):sepstr(2,match))
         nrep=1
         call CHANGE(cbuf,save,string,' ',lcols,nrep)
	 nsep=maxsav
         call CHSEP(cbuf,'1|',sepstr,nsep)
      else if(func(1:6).eq.'DELETE')then
         if(match.eq.0)go to 180
c
c        Delete data name and redefine column markers
c
         name=names(sepnam(1,match):sepnam(2,match))
         if(match.gt.1)
     &   name='|'//names(sepnam(1,match):sepnam(2,match))
         nrep=1
         call CHANGE(names,name,' ',' ',lcols,nrep)
	 nsep=maxsav
         call CHSEP(names,'1|',sepnam,nsep)
c
c        Delete data string and redefine column markers
c
         save=cbuf(sepstr(1,match):sepstr(2,match))
         if(match.gt.1)
     &   save='|'//cbuf(sepstr(1,match):sepstr(2,match))
         nrep=1
         call CHANGE(cbuf,save,' ',' ',lcols,nrep)
	 nsep=maxsav
         call CHSEP(cbuf,'1|',sepstr,nsep)
         nsave=nsave-1
      else if(func(1:5).eq.'PRINT')then
         if(match.eq.0)go to 180
         write(6,60)names(sepnam(1,match):sepnam(2,match))
 60      format(1x,10('--------')/1x,'Character string: ',a/
     &          1x,10('--------'))
         write(6,*)cbuf(sepstr(1,match):sepstr(2,match))
      else if(func(1:6).eq.'REPORT')then
         if(nsave.le.0)then
            write(6,*)'*** No strings stored ***'
         else
            write(6,70)
 70         format(1x,10('--------')/' Current strings'/
     &             1x,'Name',18x,'String'/
     &             1x,10('--------'))
            do100 i=1,nsave
            if(match.gt.0.and.i.ne.match)go to 100
            name=names(sepnam(1,i):sepnam(2,i))
            write(6,80)name,cbuf(sepstr(1,i):sepstr(2,i))
 80         format(1x,a,5x,a)
 100        continue
         end if
      end if
      go to 200
 180  irtn=-1
 200  return
      end
