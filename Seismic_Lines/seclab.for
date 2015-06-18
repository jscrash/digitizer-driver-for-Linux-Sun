C DEC/CMS REPLACEMENT HISTORY, Element SECLAB.FOR
C *2     1-MAY-1991 12:39:38 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:26:54 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element SECLAB.FOR
c list NL_DLS
c
c     ***Generate section labels ***
c
c     Calling sequence:
c
c     call SECLAB(trm,xlabel,ylabel,labels,nlabel)
c
c     Arguments:
c
c     Name    Input/   Type       Description
c             Output
c
c     trm       I    Integer(6)     Township/range/meridian limits
c
c     xlabel    O   Real*8(25000)   X coordinates for labels 
c
c     ylabel    O   Real*8(25000)   Y coordinates for labels 
c
c     labels    O   Integer(25000)  Section numbers
c
c     nlabel    O      Integer      Number of labels
c
      subroutine SECLAB(trm,xlabel,ylabel,labels,nlabel)
      integer trm(6),labels(25000)
      double precision xlabel(25000),ylabel(25000),lat,long,
     &xx,yy,tolrnc
      logical inside
      data maxrng/35/,tolrnc/1.0d-6/
      nlabel=0
      lablmx=25000
c
c     Assign the elements of trm to individual names to make it
c     easier to follow the code.
c
      itwpnw=trm(1)
      irngnw=trm(2)
      mernw=trm(3)
      itwpse=trm(4)
      irngse=trm(5)
      merse=trm(6)
      irng=irngse
      nrng=irngnw
c
c     It may be necessary to search for the next meridian if the
c     township/range specifications for this grid use different prime
c     meridians.  This requires an additional loop.
c
      do100 merid=merse,mernw
      if(merse.eq.mernw)go to 60
      irng=1
      nrng=maxrng
      if(merid.eq.merse)irng=irngse
      if(merid.eq.mernw)nrng=irngnw
 60   do90 jrng=irng,nrng
      do80 itwp=itwpse,itwpnw
      do70 jsecn=1,36
      if(jsecn.eq.1.or.jsecn.eq.6.or.jsecn.eq.15.or.jsecn.eq.16.or.
     +jsecn.eq.21.or.jsecn.eq.22.or.jsecn.eq.31.or.jsecn.eq.36)then
      if(merid.eq.6.and.(itwp.lt.77.or.itwp.gt.88).and.
     +jrng.gt.13) goto 70
      if(merid.eq.6.and.jrng.gt.25) goto 70
      call DLSLL(1,itwp,jrng,jsecn,6,merid,lat,long,irtn)
      if (irtn .eq. 99) goto 70
      call PJGET(lat,long,xx,yy)
      if(DABS(xx*0.3048d0-xlabel(nlabel)).lt.tolrnc.and.
     +DABS(yy*0.3048d0-ylabel(nlabel)).lt.tolrnc)goto 70
      call GEOSCL(xx,yy,x,y)
      call INANY(x,y,inside)
      if(.not.inside)go to 70
      nlabel=nlabel+1
      if(nlabel.eq.lablmx)goto 200
      xlabel(nlabel)=xx*0.3048d0
      ylabel(nlabel)=yy*0.3048d0
      labels(nlabel)=jsecn
      end if
 70   continue
 80   continue
 90   continue
 100  continue
 200  return
      end
