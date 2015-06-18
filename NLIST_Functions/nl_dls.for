C DEC/CMS REPLACEMENT HISTORY, Element NL_DLS.FOR
C *2     1-MAY-1991 12:38:18 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:18:00 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element NL_DLS.FOR
c list NL_DLS
c
c     ***Plot a Dominion Land Survey grid ***
c
c     Calling sequence:
c
c     call NL_DLS(trm,latlng,area,lsecn)
c
c     Arguments:
c
c     Name    Input/   Type       Description
c             Output
c
c     trm       I    Integer(6)   Township/range/meridian limits
c
c     latlng    I     Real*8(4)   Latitude/longitude limits
c
c     area      I   Character*(*) Area specification type (LL,NTS,DLS,LSD)
c
c     lsecn     I     Integer     Section lines:0=none,1=solid,2=dashed
c
c     Comments:
c
c     This routine draws a township range grid for an area.  The grid
c     will be clipped at the latitude/longitude area limits unless
c     area='DLS'.
c
c nolist NL_DLS
c
      subroutine NL_DLS(trm,latlng,area,lsecn,nlfile)
c
c*****This routine was written for Gulf Canada Resources by Davis Swan
c     in April, 1982.  Optimized for Calcomp plotter in September, 1983.
c     Lat/long windowing added September, 1984.  Re-written to use 
c     new clipping fence routines in November, 1989.
c
      parameter (maxpts=5000)
      double precision latlng(4),sfact,xw,yw
      double precision xlab(25000),ylab(25000)
      integer jsecs(25000)
      real xy(2*maxpts),xyp(2*maxpts)
      real angle,hx
      integer trm(6),CHTRIM,lcols(2)
      character area*(*)
      character label*12
      logical diag,top,left,big,lab
      common/diags/diag
      common/xyplcb/xy
      data maxrng/35/,nfont/0/,lcols/2*0/,angle/0.0/,lwx/3/,lwy/3/,
     &hx/0.0/
      character*(*) nlfile
c	(CP) new option suppress sections
      real supprs
      call RSTORE('GET SPRS',supprs, nvals, irtn)
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

c
c     Check input parameters
c
      itest=itwpnw*irngnw*mernw*itwpse*irngse*merse
      if(itest.eq.0)go to 180
      if(itwpnw.lt.1.or.itwpnw.gt.130.or.itwpse.lt.1.or.itwpse.gt.
     &130)go to 180
      if(irngnw.lt.1.or.irngnw.gt.maxrng.or.irngse.lt.1.or.irngse.gt.
     &maxrng)go to 180
      if(mernw.lt.1.or.mernw.gt.6.or.merse.lt.1.or.merse.gt.6)
     &go to 180
      if(itwpnw.ge.itwpse)go to 10
      isave=itwpnw
      itwpnw=itwpse
      itwpse=isave
 10   if(mernw*100+irngnw.ge.merse*100+irngse)go to 15
      isave=mernw
      mernw=merse
      merse=isave
      isave=irngnw
      irngnw=irngse
      irngse=isave
 15   irtn=0
      call PJREVD('LLXY')
      irng=irngse
      nrng=irngnw
c
c     Set the letter height so that range labels will fit properly.
c
c     (fix undefined variable by getting it from store)
      nvalue=1
      call DSTORE('GET SCALE FACTOR',sfact,nvalue,irtn)
c     (end fix)

      ht=20000.0d0*sfact/6.0d0
      ht=AMAX1(0.05,amin1(0.25,ht))
     
      i = index(nlfile,' ')
      nlfile(i:i) = char(0)
      call DLS_NLIST_INIT()
c	(CP) we try branching around section stuff if supprs.eq. 1.0
      if(supprs .eq. 1.0) goto 989

c
c     (CP) unclear what is right here. There is no variable called
c     Map boundary but there is one called MAP_FRAME
c     also, about 10 lines down xy is loaded (again) with B.C. Border
c     this is probably a noop, but since nvals=0 on return, left it alone
      call RSTORE('GET Map boundary',xy,nvals,jrtn)
      if (nvals.gt.0)then
          nxy=nvals/2
          call PRXY(xy,nxy,0,0,0)
      end if
c
c     Activate  B.C. Border if in Map area
c
      call CLCMND('ACTIVATE B.C. Border',jrtn)
      if(jrtn.eq.0)then
c
c     Retrieve B.C. border
c
         call RSTORE('GET B.C. Border',xy,nvals,jrtn)
         call CLCMND('SELECT data within B.C. Border',jrtn)
         nxy=nvals/2
         call PRXY(xy,nxy,0,0,0)
         call CLCMND('DEACTIVATE Map boundary',jrtn)
      end if
c
c     call DLS_NLIST_INIT()

c
c     SECTION LINES
c
      top=.false.
      ntlab=0
      do75 jtwp=itwpse,itwpnw
      do75 irow=2,6
      call DLSROW(trm,irow,jtwp,top,xy,nxy)
      nxyp=maxpts	
      call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      if (nxyp.ne.0) call PRXY(xyp,nxyp,jtwp,irow,0)
 75   continue
      left=.false.
c
c     It may be necessary to search for the next meridian if the
c     township/range specifications for this grid use different prime
c     meridians.  This requires an additional loop.
c
      do100 merid=merse,mernw
      if(merse.eq.mernw)go to 80
      irng=1
      nrng=maxrng
      if(merid.eq.merse)irng=irngse
      if(merid.eq.mernw)nrng=irngnw
 80   do90 jrng=irng,nrng
      do90 jcol=2,6
      call DLSCOL(trm,jcol,jrng,merid,left,xy,nxy)
      nxyp=maxpts
      call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      if (nxyp.ne.0) call PRXY(xyp,nxyp,jrng,jcol,merid)
 90   continue
 100  continue

c
c     Writing section lines Nlist 
c
      i = 0
      call DLS_NLIST_TERM(nlfile,'section_lines',i)
c	(CP) we think we just skipped section nlists
989   continue
C
C     TOWNSHIP LINES
C
      call DLSTEXT_NLIST_INIT()
 110  lt=1
      left=.false.
      do140 jtwp=itwpse,itwpnw
      ktwp=jtwp
      top=.false.
      irow=1
      call DLSROW(trm,4,ktwp,top,xy,nxy) 
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp = 0
      endif
      lab=.true.
      if(nxyp.lt.2)lab=.false.
      if(.not.lab)go to 120
      ntlab=ntlab+1
      ifirst=0
      if(ntlab.eq.1) ifirst=1
      label=' '
      write(label,115)jtwp
 115  format('T',i3)
      if(big)write(label,117)jtwp
 117  format(i3)
      nchar=CHTRIM(label)
      xlab1=xyp(1)-(nchar+1)*ht
      if(xyp(1).gt.xyp(2*nxyp-1)) xlab1=xyp(1)+1.5*ht
      ylab1=xyp(2)-0.5*ht
      xlab2=xyp(2*nxyp-1)-(nchar+1)*ht
      if(xyp(2*nxyp-1).gt.xyp(1)) xlab2=xyp(2*nxyp-1)+1.5*ht
      ylab2=xyp(2*nxyp)-0.5*ht
 120  if(jtwp.ne.89.or.itwpse.ge.89)go to 125
c
c     In this special case we draw a line across the top of township
c     88 rather than across the bottom of township 89 in order to handle
c     the Peace River Block properly.
c
      ktwp=88
      top=.true.
      irow=6
 125  call DLSROW(trm,irow,ktwp,top,xy,nxy)
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp = 0
      endif
      if(lab)call PXYLAB(xlab1,ylab1,xw,yw)
      if(lab)call DLSTEXT_NLIST_POINT(xw,yw,nchar,label,ifirst)
      if(nxyp.gt.0) call PRXY(xyp,nxyp,ktwp,irow,0)
      if(lab)call PXYLAB(xlab2,ylab2,xw,yw)
      if(lab)call DLSTEXT_NLIST_POINT(xw,yw,nchar,label,ifirst)
      ktwp=jtwp
      top=.false.
      left=.not.left
 140  continue
c
c     Plot the line across the top of the grid.
c
      irow=6
      top=.true.
      call DLSROW(trm,irow,ktwp,top,xy,nxy)
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp = 0
      endif
      if(nxyp.gt.0) call PRXY(xyp,nxyp,ktwp,irow,0)
c
c     Plot the north-south grid lines
c
c     It may be necessary to search for the next meridian if the
c     township/range specifications for this grid use different prime
c     meridians.  This requires an additional loop.
c
      jcol=1
      lt=2
      top=.false.
      left=.false.
      nrlab=0
      do170 merid=merse,mernw
      if(merse.eq.mernw)go to 145
      irng=1
      nrng=maxrng
      if(merid.eq.merse)irng=irngse
      if(merid.eq.mernw)nrng=irngnw
c
c     Do loop for range lines
c
 145  do165 jrng=irng,nrng
      krng = jrng
c
c     Annotate ranges by finding the intersection of column 4 with map
c     borders.
c
      label=' '
      write(label,147)jrng,merid
 147  format('R ',i2,'W',i1)
      if(big)write(label,150)jrng,merid
 150  format(i2,'W',i1)
      nchar=CHTRIM(label)
      call DLSCOL(trm,4,krng,merid,left,xy,nxy)
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp = 0
      endif
      lab=.true.
      if(nxyp.lt.2)lab=.false.
      if(.not.lab)go to 155
      nrlab=nrlab+1 
      xlab1=xyp(1)-0.5*nchar*ht
      ylab1=xyp(2)+1.0*ht
      if(xyp(2).lt.xyp(2*nxyp)) ylab1=xyp(2)-2.0*ht
      xlab2=xyp(2*nxyp-1)-0.5*nchar*ht
      ylab2=xyp(2*nxyp)+1.0*ht
      if(xyp(2*nxyp).lt.xyp(2)) ylab2=xyp(2*nxyp)-2.0*ht 
      call PXYLAB(xlab1,ylab1,xw,yw)
      call DLSTEXT_NLIST_POINT(xw,yw,nchar,label,ifirst)

 155  call DLSCOL(trm,jcol,krng,merid,left,xy,nxy)
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp=0
      endif
      if(nxyp.gt.0) call PRXY(xyp,nxyp,jrng,jcol,merid)
      if(lab)call PXYLAB(xlab2,ylab2,xw,yw)
      if(lab)call DLSTEXT_NLIST_POINT(xw,yw,nchar,label,ifirst)     
      top=.not.top
 165  continue
 170  continue
c
c     Start writing into the text nlist 
c
      i=0
      call DLSTEXT_NLIST_TERM(nlfile,'dls_text',i)

c
c     Plot the line along the western edge of the grid.
c
      left=.true.
      jcol=6
      call DLSCOL(trm,jcol,krng,mernw,left,xy,nxy)
      if(nxy.gt.0) then
        nxyp=maxpts
	call CLPXYL(xy,nxy,nxy,xyp,nxyp)
      else
	nxyp=0
      endif
      if(nxyp.gt.0) call PRXY(xyp,nxyp,krng,jcol,mernw)
      call CLCMND('DEACTIVATE B.C. Border',jrtn)
      call CLCMND('ACTIVATE Map boundary',jrtn)
      go to 200
 180  irtn=-1

 200  i = 1
      call DLS_NLIST_TERM(nlfile,'twp_rng_lines',i)
      
      ifirst=1
      lab=.true.
      if (supprs.eq. 0.0)then
      call SECLAB(trm,xlab,ylab,jsecs,nlabel)
      if(nlabel.eq.0)goto 500
      label=' '
      do210 j=1,nlabel
      write(label,'(i2)')jsecs(j)
      nchar=CHTRIM(label)
      if(lab)
     +call DLSTEXT_NLIST_POINT(xlab(j),ylab(j),nchar,label,ifirst)
      ifirst=0
210   continue
      i=1
      call DLSTEXT_NLIST_TERM(nlfile,'section_labels',i)
      end if
500   return
      end


