C DEC/CMS REPLACEMENT HISTORY, Element TMXY.FOR
C *2     1-MAY-1991 12:40:05 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:30:21 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element TMXY.FOR
c list TMXY
c
c     *** Project latitude/longitude to Tranverse Mercator X/Y ***
c
c     Calling sequence:
c
c     call TMXY(lat,long,lamdac,k0,ec,x,y)
c
c     Arguments:
c
c     Name    Input/       Type       Description
c             output
c
c     lat       O         Real*8      Latitude.
c
c     long      O         Real*8      Longitude.
c
c     lamdac    I         Real*8      Central meridian for projection.
c
c     k0        I         Real*8      Scale factor at lamdac.
c
c     ec        I         Real*8      False Easting (x value).
c
c     iflag     I         Real*8      Central meridian flag.
c
c     x         I         Real*8      Tranverse Mercator X coordinate.
c
c     y         I         Real*8      Tranverse Mercator Y coordinate.
c
c     Comments:
c
c     This subroutine is designed to calculate the Transverse Mercator
c     X,Y coordinates from the latitide/longitude.  To generate UTM
c     coordinates set k0=0.9996, ec=500000.0 and set lamdac
c     appropriately.  Otherwise set k0=1.000, ec=0.0
c
c     Revision History:
c
c     Original UTM routine (GEOUTM) written by Davis Swan for the
c     Geological Survey in 1977.  Converted to decimal degrees in
c     January, 1988.
c
c nolist TMXY
c
      subroutine TMXY(lat,long,lamdac,k0,ec,x,y)
      implicit double precision(a-z)
      integer flag
      data flag/0/
c
c     Constants are calculated on the first call to TMXY. A flag is
c     set so that further calls to TMXY can skip these
c     calculations.
c
      if(flag.gt.0)go to 10
      flag=1
      ra=6.3782064d6
      esq=6.768657997291d-3
      eprmsq=esq/(1.0d0-esq)
      sinsec=3.1415926535d0/180.0d0
c
c     Convert latitude to radian measure so that Fortran trigonometric
c     functions can be used.  Note that the conversion factor (pi
c     radians/ 180 degrees) is equal to sin(1").
c
 10   phi=lat*sinsec
      lambda=long
      dlamda=lamdac-lambda
      NU=ra/dsqrt(1.0d0-esq*dsin(phi)**2)
      sa=(1.0d0+(3.0d0/4.0d0)*esq+(45.0d0/64.0d0)*esq**2+
     &(175.0d0/256.0d0)*esq**3)*phi
      sb=(((3.0d0/4.0d0)*esq+(15.0d0/16.0d0)*esq**2+(525.0d0/512.0d0)*
     &esq**3)*dsin(2.0d0*phi))/2.0d0
      sc=(((15.0d0/64.0d0)*esq**2+(105.0d0/256.0d0)*esq**3)*
     &dsin(4.0d0*phi))/4.0d0
      sd=((35.0d0/512.0d0)*esq**3*dsin(6.0d0*phi))/6.0d0
      s=ra*(1.0d0-esq)*(sa-sb+sc-sc)
      I=s*k0
      II=(nu*dsin(phi)*dcos(phi)*sinsec**2*k0)/2.0d0
      IIIA=(sinsec**4*nu*dsin(phi)*dcos(phi)**3)/24.0d0
      IIIB=(5.0d0-dtan(phi)**2+9.0d0*eprmsq*dcos(phi)**2+4.0d0*eprmsq**2
     &*dcos(phi)**4)*k0
      III=IIIA*IIIB
      IV=NU*dcos(phi)*sinsec*k0
      V=(sinsec**3*nu*dcos(phi)**3*(1.0d0-dtan(phi)**2+eprmsq*dcos(phi)
     &**2)*k0)/6.0d0
      A6A=(dlamda**6*sinsec**6*nu*dsin(phi)*dcos(phi)**5)/7.2d2
      A6B=(61.0d0-58.0d0*dtan(phi)**2+dtan(phi)**4+2.7d2*eprmsq*dcos(phi
     &)**2-3.3d2*eprmsq*dsin(phi)**2)*k0
      A6=A6A*A6B
      B5A=(dlamda**5*sinsec**5*nu*dcos(phi)**5)/1.202
      B5B=(5.0d0-18.0d0*dtan(phi)**2+dtan(phi)**4+14.0d0*eprmsq*dcos(phi
     &)**2-58.0d0*eprmsq*dsin(phi)**2)*k0
      B5=B5A*B5B
      y=I+II*dlamda**2+III*dlamda**4+A6
      x=ec+IV*dlamda+V*dlamda**3+B5
      return
      end
