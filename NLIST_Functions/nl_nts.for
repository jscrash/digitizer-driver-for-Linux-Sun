C DEC/CMS REPLACEMENT HISTORY, Element NL_NTS.FOR
C *2     1-MAY-1991 12:38:36 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:18:34 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element NL_NTS.FOR
c list NL_NTS
c
c     ***Draw a latitude/longitude grid***
c
c     Calling sequence:
c
c     call NL_NTS(llfram,lnunit,clip,nlfile)
c
c     Arguments:
c
c     Name   Input/  Type        Decription
c            Output
c
c     llfram   I   Real*8(6)     Latitude/longituie frame description
c
c     lnunit   I    Logical      Unit line flag.
c
c     clip     I    Logical      Clip NTS in township/range area
c
c     nlfile   I  Character*(*)  NLIST path name.
c
c     Comments:
c
c     This subroutine draws a latitude/longitude grid onto the
c     map at the specified intervals.
c
c nolist NL_NTS
c
      subroutine NL_NTS(llfram,lnunit,clip,nlfile)
c
c*****This routine was modified for Gulf Canada Resources by Davis
c     Swan in March, 1983 so that lat/long to x/y calculations
c     were made only as needed based upon line curvature.  LL point grid
c     added in June, 1986.  Detailed NTS grid added in October, 1989.
c
      double precision xx,yy,lat,long,sfact,
     &rnglat,rnglng,dlat,dlong,delta,llfram(6),latlng(4),
     &latmax,latmin,lngmax,lngmin,degree
      double precision xl,yl
      real xy(5000),xyclip(5000)
      character lntype*3,messag*28,label*12,UPCASE*8,sheets*12
      character sheet*1
      character*(*)nlfile
      logical backwd,sublab,letlab,ulines,break,inside,
     &skip,reset,lnunit
      logical diag,clip
      integer CHTRIM,lcols(2)
      common/diags/diag
c	(CP) new option to suppress units
      real supprs

      data sheets/'ABCDHGFEIJKL'/
      data lcols/2*0/

      call NTS_NLIST_INIT()
c
c     Get scale factor and origin
c
      nvalue=1
      call DSTORE('GET SCALE FACTOR',sfact,nvalue,irtn)
      call RSTORE('GET NULL',rnull,nvalue,irtn)
      call RSTORE('GET X0',x0,nvalue,irtn)
      call RSTORE('GET Y0',y0,nvalue,irtn)
c	(CP) new option
      call RSTORE('GET SPRS',supprs,nvalue,irtn)
c     (CP) not clear what to do here, see also nl_dls
c     Map boundary does not exist although there is a MAP_FRAME variable
c     however, it appears as if the array xy get used elsewhere but not for this
      call RSTORE('GET Map boundary',xy,nvals,irtn)
      if (nvals.gt.0)then
          nxy=nvals/2
          call PXYNTS(xy,nxy,0.0)
      end if
      if(clip)then
c
c        Activate the B.C. border as a blanking fence
c
         call CLCMND('ACTIVATE B.C. Border',jrtn)
         call CLCMND('REJECT data within B.C. Border',jrtn)
      end if
c
c     call NTS_NLIST_INIT()
c
c     Assign values to variable names to make it easier to follow
c     the code, adjusting the lat/long frame if necessary.
c
      mxloop = 2
      do 150 nloop=1,mxloop
      if (nloop.eq.1) then
          lnunit=.false.
          dlat=5.0d0/60.0d0
          dlong=7.5d0/60.0d0
      else
          lnunit=.true.
          dlat=0.5d0/60.0d0
          dlong=0.75d0/60.0d0
      end if
      call DSCALD(llfram(1),llfram(2),latmax,latmin,dlat,nmin,nll,npll)
      call DSCALD(llfram(3),llfram(4),lngmax,lngmin,dlong,nmin,nll,npll)
c
c     Check for valid latitude and longitude tic intervals.
c
      if(dlat.le.0.0d0.or.dlong.le.0.0d0)go to 180
      sublab=.false.
      letlab=.false.
      nlabs=0
c
c     Set up parameters for NTS grid
c
      htsub=2500.0*sfact
      htsub=AMAX1(0.1,AMIN1(0.25,htsub))
      htuln=0.6*htsub
      wdnts=3.0*htsub
c
c(cp)
c
c     mscale=50000
      mscale=1.0/sfact*3.048
      if(lngmin.gt.-112.0d0)go to 200
      if(lngmax.gt.-112.0d0)lngmax=-112.0d0
      ulines=.false. 
      if(dlong.eq.0.0125d0)ulines=.true.
      rnglat=latmax-latmin
      rnglng=lngmax-lngmin
      if(rnglat.lt.dlat.or.rnglng.lt.dlong)go to 180
c
c     Determine the smallest unit (degrees, minutes, or seconds)
c     required to annotate the lat/long grid properly.
c
      ltdeg=dlat
      ltmin=(dlat-ltdeg)*60.0d0+0.0005
      altsec=(dlat-ltdeg-ltmin/60.0d0)*3600.0d0
      ltchar=5
      if(ltmin.gt.0)ltchar=8
      if(altsec.gt.0.0)ltchar=11
      lndeg=dlong
      lnmin=(dlong-lndeg)*60.0d0+0.0005
      alnsec=(dlong-lndeg-lnmin/60.0d0)*3600.0d0
      lnchar=5
      if(lnmin.gt.0)lnchar=8
      if(alnsec.gt.0.0)lnchar=11
c
c     Set the maximum bend allowed between line segments (in degrees)
c
      bndmax=1.0
      angmax=0.1
      dmin=0.005
      ht=.14
c
c     Draw a frame around the latitude/longitude grid
c
      nlats=(latmax-latmin)/dlat+1
      nlongs=(lngmax-lngmin)/dlong+1
c
c     Calculate the character height required to plot the lat/long
c     labels for each axis and choose the minimum.
c
      height=AMAX1(height,0.05)
      width=height*0.90
      lntype='LAT'
c
      if(diag)write(6,17)nlats,latmin,latmax
 17   format('0*** DIAGNOSTIC ***NTSGRD -Drawing in ',i6,
     &' latitude grid lines (',f12.5,' to ',f12.5,').')
c
      call PJREVD('LLXY')
      nchar=ltchar
      backwd=.true.
      lposn=1
      do60 il=1,nlats
      backwd=.not.backwd
      lat=latmin+(il-1)*dlat
c
c     Set bold lines and colors for different blocks
c
      call DEGSEC(lat,ideg,minute,second,2)
      rminut=minute+second/60.0
      sublab=.false.
      letlab=.false.
      if(AMOD(rminut+7.5,15.0).eq.0.0)sublab=.true.
      if(ulines.and.AMOD(rminut+2.5,5.0).eq.0.0)letlab=.true.
      if(dabs(lat).ge.90.0d0)go to 60
      degree=lat
      delta=dlong
      ncon=nlongs
c
c     Find the longitude increment that will produce a latitude line
c     which bends no more than bndmax degrees.
c
      call LLANGL(lntype,llfram,lat,long,bndmax,delta,ncon)
      reset=.true.
      nxy=0
      do40 j=1,ncon
      k=j
      if(backwd)k=ncon+1-j
      long=lngmin+(k-1)*delta
      if(long.gt.180.0d0)long=long-360.0d0
      call PJGET(lat,long,xx,yy)
      call GEOSCL(xx,yy,xp,yp)
      break=.false.
      if(sublab.or.letlab)then
         call INANY(xp,yp,inside)
         if(.not.inside)go to 20
c
c        Break grid lines at label positions
c
         call DEGSEC(long,ideg,minute,second,2)
         rminut=minute+second/60.0
         if(AMOD(rminut+3.75,7.5).eq.0.0.and.letlab)then
            xend=xp-0.75*htuln
            if(backwd)xend=xp+0.75*htuln
            call RTHIN(reset,xend,yp,angmax,dmin,r,bend,skip)
            if(.not.skip)nxy=nxy+1
            xy(2*nxy-1)=xend
            xy(2*nxy)=yp
            nxy=nxy+1
            xy(2*nxy-1)=rnull
            xy(2*nxy)=rnull
            xp=xend+1.5*htuln
            reset=.true.
            if(backwd)xp=xend-1.5*htuln
         else if(sublab.and.(AMOD(rminut+15.0,30.0).eq.0.75.or.
     &                       AMOD(rminut+15.0,30.0).eq.28.5))then
            break=.not.break
            reset=.true.
            if(.not.break)then
               nxy=nxy+1
               xy(2*nxy-1)=rnull
               xy(2*nxy)=rnull
            end if
         end if
      end if
 20   if(break)go to 40
      call RTHIN(reset,xp,yp,angmax,dmin,r,bend,skip)
      if(.not.skip)nxy=nxy+1
      xy(2*nxy-1)=xp
      xy(2*nxy)=yp
 40   continue
      if(nxy.eq.0)go to 60
c
c     Clip grid line against map boundary
c
c (CP)
c
      if (ulines.and.supprs.eq.1.0) goto 60
      nclip=5000/2
      call CLPXYL(xy,nxy,nxy,xyclip,nclip)
      if(nclip.gt.0) call PXYNTS(xyclip,nclip,lat)
 60   continue
      if(diag)write(6,65)
 65   format('0*** DIAGNOSTIC ***NTSGRD -finished plotting lat lines.')

      if (.not.lnunit) then
          call NTSTEXT_NLIST_INIT()
      end if
c
c     Draw in the longitude lines.
c
      lntype='LNG'
      nchar=lnchar
      backwd=.true.
      lposn=2
      call PJREVD('LLXY')
c
      if(diag)write(6,67)nlongs,lngmin,lngmax
 67   format('0*** DIAGNOSTIC ***NTSGRD -drawing in ',i6,' longitude ',
     &'grid lines (',f12.5,' to ',f12.5,').')
 
      do100 il=1,nlongs
      backwd=.not.backwd
      long=lngmin+(il-1)*dlong
      if(long.gt.180.0d0)long=long-360.0d0
      sublab=.false.
c
c     Set bold lines and colors for different blocks
c
      lrow=0
      lcol=0
      call DEGSEC(long,ideg,minute,second,2)
      rminut=minute+second/60.0
      rtest=AMOD(rminut+15.0,30.0)
      if(rtest.le.1.5.or.rtest.ge.28.5)sublab=.true.
c
c     Build label position for map sheets
c        
      rminut=minute+second/60.0+3.75
      if(ulines.and.AMOD(rminut,7.5).eq.0.0)then
         lcol=rminut/7.5  
         if(lcol.gt.4)lcol=lcol-4
      end if
      degree=long
      delta=dlat
c
c     Calculate the latitude increment that will produce a longitude
c     line which bends no more than bndmax degrees.
c
      call LLANGL(lntype,llfram,lat,long,bndmax,delta,ncon)
      if(sublab)delta=DMIN1(dlat,2.5d0/60.0d0)
      ncon=(latmax-latmin)/delta+1
      nxy=0
      reset=.true.
      do90 j=1,ncon
      k=j
      if(backwd)k=ncon+1-j
      lat=latmin+(k-1)*delta
      if(dabs(lat).gt.90.0d0)go to 90
      call PJGET(lat,long,xx,yy)
      call GEOSCL(xx,yy,xp,yp)
      call DEGSEC(lat,ideg,minute,second,2)
      if(sublab)then
         call INANY(xp,yp,inside)
         if(.not.inside)go to 70 
c
c        Label quadrangles
c   
         rminut=minute+second/60.0
         if(AMOD(rminut+7.5,15.0).eq.0.0)then
c
c           Break lines around label
c        
            yend=yp-0.75*htsub
            if(backwd)yend=yp+0.75*htsub
            call RTHIN(reset,xp,yend,angmax,dmin,r,bend,skip)
            if(.not.skip)nxy=nxy+1
            xy(2*nxy-1)=xp
            xy(2*nxy)=yend
            nxy=nxy+1
            xy(2*nxy-1)=rnull
            xy(2*nxy)=rnull
            reset=.true.
c
c           Plot label
c
            if(rtest.le.0.1.or.rtest.ge.29.9)then
               nlabs=nlabs+1
               ifirst = 0
               if (nlabs.eq.1) ifirst = 1
               call LLNTS(lat,long,mscale,label,irtn)
               call CHNOT(label,' ',l1)
               label=UPCASE(label(l1:))
               ntschr=CHTRIM(label)
               xlab=xp-0.5*wdnts
               ylab=yp-0.5*htsub
               if (.not.lnunit) then
                   call PXYLAB(xlab,ylab,xl,yl)
                   call NTSTEXT_NLIST_POINT(xl,yl,label,ntschr,ifirst)
               end if
            end if
            yp=yend+1.5*htsub
            if(backwd)yp=yend-1.5*htsub
         end if
      end if
 70   rminut=minute+second/60.0+2.5
      if(ulines.and.lcol.gt.0.and.AMOD(rminut,5.0).eq.0.0)then
         call INANY(xp,yp,inside)
         if(.not.inside)go to 80
         lrow=rminut/5.0
         if(lrow.gt.6)lrow=lrow-6
         if(lrow.gt.3)lrow=lrow-3
         yend=yp-0.75*htuln
         if(backwd)yend=yp+0.75*htuln
c
c        Break lines for label
c       
         call RTHIN(reset,xp,yend,angmax,dmin,r,bend,skip)
         if(.not.skip)nxy=nxy+1
         xy(2*nxy-1)=xp
         xy(2*nxy)=yend  
         nxy=nxy+1
         xy(2*nxy-1)=rnull
         xy(2*nxy)=rnull
         reset=.true. 
         ndx=(lrow-1)*4+lcol
         sheet = sheets(ndx:ndx)
         xlab=xp-0.4*htuln
         ylab=yp-0.5*htuln
         call PXYLAB(xlab,ylab,xl,yl)
         call NTSTEXT_NLIST_POINT(xl,yl,sheet,1,ifirst)
         yp=yend+1.5*htuln
         if(backwd)yp=yend-1.5*htuln
      end if
 80   call RTHIN(reset,xp,yp,angmax,dmin,r,bend,skip)
      if(.not.skip)nxy=nxy+1
      xy(2*nxy-1)=xp
      xy(2*nxy)=yp
 90   continue
c
c     Clip grid line against map boundary
c
c
c (CP)
c
      if(ulines.and.supprs.eq.1.0) goto 100
      if(nxy.eq.0) goto 100
      nclip=5000/2
      call CLPXYL(xy,nxy,nxy,xyclip,nclip)
      if (nclip.gt.0) call PXYNTS(xyclip,nclip,long)
 100  continue
      if(diag)write(6,105)
 105  format('0*** DIAGNOSTIC *** -finished plotting long lines.')
c
c     Write unit lines NLIST and lat_lng_lines NLIST accordingly
c 
      if (.not.lnunit) then
          i = index(nlfile,' ')
          nlfile(i:i) = char(0)
          i = 0
          call NTS_NLIST_TERM(nlfile,'lat_lng_lines',i)
      else 
          if(supprs.eq.0.0) then
          i = 0
          call NTS_NLIST_TERM(nlfile,'unit_lines',i)
          endif
      end if
 150  continue

      if(nlabs.eq.0)then
c
c        Label lower right corner of map if no labels have been plotted
c
	 nval=4
         call DSTORE('GET LATLNG',latlng,nval,irtn)
         lat=latlng(2)+0.001d0
         long=latlng(4)+0.001d0
         call LLNTS(lat,long,10000,label,irtn)
         call CHSUB(label,1,12,' ','-')
         nrep=12
         call CHANGE(label,'-',' ',' ',lcols,nrep)
         label=UPCASE(label)
         ntschr=CHTRIM(label)
         htlab=0.7*htsub
         xlab=x0
         ylab=y0-2.0*htlab
         ifirst=1
         call PXYLAB(xlab,ylab,xl,yl)
         call NTSTEXT_NLIST_POINT(xl,yl,label,ntschr,ifirst)
      end if
c
c     Write nts label NLIST
c
      i = 1
      call NTSTEXT_NLIST_TERM(nlfile, 'nts_labels',i)

      call CLCMND('DEACTIVATE B.C. Border',jrtn)
      go to 200
 180  messag='LL Grid parameters incorrect'
      print*,messag
 200  return
      end



