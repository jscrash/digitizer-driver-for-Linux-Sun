C DEC/CMS REPLACEMENT HISTORY, Element LLANGL.FOR
C *2     1-MAY-1991 12:37:58 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:14:38 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element LLANGL.FOR
c list LLANGL
c
c     **Calculate number of conversion points to plot a line for a**
c       latitude/longitude grid
c
c     Calling sequence:
c
c     call LLANGL(lntype,llfram,lat,long,bndmax,delta,ncon)
c
c     Arguments:
c
c     Name     Input/    Type        Decription
c              output
c
c     lntype    I      Character*3   Line type ('LAT' or 'LNG')
c
c     llfram    I       Real*8 (6)   Latitude/longitude frame limits and
c                                    annotation intervals in the order
c                                    latmax,latmin,dlat,lngmax,lngmin,
c                                    dlng
c
c     lat       I        Real*8      Latitude (set if lntype='LAT')
c
c     long      I        Real*8      Longitude (set if lntype='LNG')
c
c     bndmax    I         Real       Maximum bend allowed (degrees)
c
c     delta     O        Real*8      Increment for conversions
c
c     ncon      O        Integer     Number of conversions required
c
c     Comments:
c
c     Calculate lat/long increment required to produce lines which bend
c     no more than bndmax degrees.
c
c nolist LLANGL
c
      subroutine LLANGL(lntype,llfram,lat,long,bndmax,delta,ncon)
      double precision llfram(6),lat,long,latest,lntest,x,y,degree,
     &delta
      character*3 lntype
      logical diag,skip
      common/diags/diag
      angmax=0.0
      if(lntype.ne.'LAT'.and.lntype.ne.'LNG')go to 200
      do30 i=1,3
      if(lntype.eq.'LAT')then
c
c        Convert latitude/longitude to x/y.
c
         lntest=llfram(4)+(i-1)*llfram(6)
         call PJGET(lat,lntest,x,y)
      else
         latest=llfram(2)+(i-1)*llfram(5)
         call PJGET(latest,long,x,y)
      end if
c
c     Convert x/y to plotter coordinates
c
      call GEOSCL(x,y,xp,yp)
c
c     Pass point to REDUCE to calculate angle.
c
      if(i.gt.1)angmax=90.0
      call REDUCE(xp,yp,angmax,0.0,r,bend,skip)
 30   continue
c
c     Calculate the number of lat/long conversions that have to be
c     made in order to draw a line having bends less than bndmax degrees.
c     The resultant interval is adjusted to be a multiple of the
c     grid interval so that grid crossover points are precise.
c
      if(lntype.eq.'LNG')then
         range=llfram(1)-llfram(2)
         totbnd=bend*range/llfram(5)
         dlat=range/(totbnd/bndmax+1)
         nlat=max0(1,int(llfram(5)/dlat+0.99))
         delta=llfram(5)/nlat
         ncon=range/delta+1.01d0
         degree=long
      else
         range=llfram(3)-llfram(4)
         totbnd=bend*range/llfram(6)
         dlng=range/(totbnd/bndmax+1)
         nlng=max0(1,int(llfram(6)/dlng+0.99))
         delta=llfram(6)/nlng
         ncon=range/delta+1.01d0
         degree=lat
      end if
      if(diag)write(6,55)ncon,degree,totbnd
 55   format('0*** DIAGNOSTIC ***LLANGL -Converting ',i6,' points ',
     &'along line ',f12.3,'. Total curvature is ',f7.3,' degrees.')
 200  return
      end
