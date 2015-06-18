C DEC/CMS REPLACEMENT HISTORY, Element RSTORE.FOR
C *2     1-MAY-1991 12:39:24 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:23:46 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element RSTORE.FOR
c list RSTORE
c
c     *** Subroutine to store SAVE and GET floating point values ***
c
c     Calling sequence:
c
c     call RSTORE(comnd,value,nvalue,irtn)
c
c     Arguments:
c
c     Name    Input/        Type       Description
c             outout
c
c     comnd     I       Character*(*)  Command string
c     
c     value    I,O         Real        Value or array
c   
c     nvalue   I,O        Integer      Number of elements in array
c
c     irtn      O         Integer      Return code: -1=failed
c
c     Comments:
c
c     This routine can be used to save constants in a data area
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
c nolist RSTORE
c
      subroutine RSTORE(comnd,value,nvalue,irtn)
      parameter (maxsav=100,maxval=5000)
      character*(*) comnd
      character*20 name,func
      real vals(maxval),value(1)
      integer sepfld(2,5),sepnam(2,maxsav),noff(maxsav),
     &numval(maxsav),lcols(2),to,from,CHTRIM
      character*200 names
c
c     The NULL and PI values are predefined
c
      data names/'NULL|PI'/,
     &sepnam/1,4,6,7,196*0/,nsave/2/,nvals/2/,noff/0,1,98*0/,
     &numval/1,1,98*0/,lcols/2*0/,
     &vals/-999.0,3.141593,4998*0.0/
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
c
c     Check for stored name
c
      call CHCOMP(names,sepnam,nsave,name,match)
      if(func(1:4).eq.'SAVE'.and.match.gt.0)func='UPDATE'
      if(func(1:4).eq.'SAVE')then
         if(nvalue.le.0)go to 180
c
c        Save double precision variable or array
c  
         call CHADD(names,name,'|',irtn)
         if(irtn.ne.0)go to 200
         nsave=nsave+1
         if(nsave.gt.maxsav)go to 180
         sepnam(1,nsave)=sepnam(2,nsave-1)+2
         sepnam(2,nsave)=sepnam(1,nsave)+lenam-1
         numval(nsave)=nvalue
         noff(nsave)=nvals
         do10 i=1,nvalue
         nvals=nvals+1
         if(nvals.gt.maxval)go to 180
         vals(nvals)=value(i)
 10      continue
      else if(func(1:3).eq.'GET')then
         if(match.eq.0)go to 180
         ioff=noff(match)
         nvalue=numval(match)
         do20 i=1,nvalue
         value(i)=vals(ioff+i)
 20      continue
      else if(func(1:6).eq.'UPDATE')then
         if(nvalue.le.0)go to 180
         if(match.eq.0)go to 180
         ioff=noff(match)
         do30 i=1,nvalue
         vals(ioff+i)=value(i)
 30      continue
      else if(func(1:6).eq.'DELETE')then
         if(match.eq.0)go to 180
         ioff=noff(match)
         if(match.eq.nsave)then
            if(nsave.eq.1)then
               nvals=0
            else
               nvals=noff(nsave-1)
            end if
            nsave=nsave-1
            go to 200
         end if
c
c        Delete data name and redefine column markers
c
         name=names(sepnam(1,match):sepnam(2,match))
         if(match.gt.1)
     &   name='|'//names(sepnam(1,match):sepnam(2,match))
         nrep=1
         call CHANGE(names,name,' ',' ',lcols,nrep)
	 nrep=maxsav
         call CHSEP(names,'1|',sepnam,nsep)
c
c        Move data values
c
         to=noff(match)
         from=noff(match+1)
         move=noff(nsave)+numval(nsave)-from
         do40 i=1,move
         vals(to+i)=vals(from+i)
 40      continue
c
c        Move data descriptions
c
         do50 i=match,nsave
         numval(i)=numval(i+1)
         if(i.eq.match)go to 50
         noff(i)=noff(i-1)+numval(i-1)
 50      continue
         nsave=nsave-1
         nvals=noff(nsave)+numval(nsave)
      else if(func(1:5).eq.'PRINT')then
         if(match.eq.0)go to 180
         ioff=noff(match)
         nprint=numval(match)
         write(6,60)names(sepnam(1,match):sepnam(2,match))
 60      format(1x,10('--------')/1x,'Floating point array: ',a/
     &          1x,10('--------'))
         write(6,*)(vals(j),j=ioff+1,ioff+nprint)
      else if(func(1:6).eq.'REPORT')then
         if(nsave.le.0)then
            write(6,*)'*** No Floating point arrays stored ***'
         else
            write(6,70)
 70         format(1x,10('--------')/' Current Floating point arrays'/
     &             1x,'Name',18x,'Offset  Number of values'/
     &             1x,10('--------'))
            do100 i=1,nsave
            if(match.gt.0.and.i.ne.match)go to 100
            j1=noff(i)+1
            j2=noff(i)+numval(i)
            name=names(sepnam(1,i):sepnam(2,i))
            write(6,80)name,noff(i),numval(i),vals(j1),vals(j2)
 80         format(1x,a,2i8,2e15.7)
 100        continue
         end if
      end if
      go to 200
 180  irtn=-1
      nvalue=0
 200  return
      end
