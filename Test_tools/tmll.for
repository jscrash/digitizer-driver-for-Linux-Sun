C DEC/CMS REPLACEMENT HISTORY, Element TMLL.FOR
C *2     1-MAY-1991 12:40:01 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:29:43 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element TMLL.FOR
c list TMLL
c
c     *** Project Tranverse Mercator X/Y to latitude/longitude ***
c
c     Calling sequence:
c
c     call TMLL(xeast,ynorth,lamdac,k0,ec,lat,long)
c
c     Arguments:
c
c     Name    Input/       Type       Description
c             output
c
c     xeast     I         Real*8      Tranverse Mercator X coordinate.
c
c     ynorth    I         Real*8      Tranverse Mercator Y coordinate.
c
c     lamdac    I         Real*8      Central meridian for projection.
c
c     k0        I         Real*8      Scale factor at (0,0).
c
c     ec        I         Real*8      False Easting (x value).
c
c     lat       O         Real*8      Latitude.
c
c     long      O         Real*8      Longitude.
c
c     Comments:
c
c     This subroutine is designed to calculate the latitude and
c     longitude of a point having Tranverse Mercator coordinates
c     (X,Y).  In this routine the variable esq is the square of the
c     earth's first eccentricity.
c
c     To generate latitude/longitude from UTM coordinates set k0=0.9996
c     and ec=500000.0.  Otherwise set k0=1.000 and ec=0.0
c
c     Revision History:
c
c     Original version (UTMGEO) written by Davis Swan for the Geological
c     Survey in 1977.  Modified for decimal lat/long in January, 1988.
c
c nolist TMLL
c
      subroutine TMLL(xeast,ynorth,lamdac,k0,ec,lat,long)
      implicit double precision(a-z)
      double precision arclen(84)
      integer phi0,flag,xsign
      data flag/0/
      sa(phi)=(1.0d0+(3.0d0/4.0d0)*esq+(45.0d0/64.0d0)*esq**2+
     &(175.0d0/256.0d0)*esq**3)*phi
      sb(phi)=(((3.0d0/4.0d0)*esq+(15.0d0/16.0d0)*esq**2+(525.0d0/512.0d
     &0)*esq**3)*dsin(2.0d0*phi))/2.0d0
      sc(phi)=(((15.0d0/64.0d0)*esq**2+(105.0d0/256.0d0)*esq**3)*
     &dsin(4.0d0*phi))/4.0d0
      sd(phi)=((35.0d0/512.0d0)*esq**3*dsin(6.0d0*phi))/6.0d0
      s(phi)=ra*(1.0d0-esq)*(sa(phi)-sb(phi)+sc(phi)-sd(phi))
      i(phi)=s(phi)*k0
c
c     Calculate the arclengths from the equator to each whole degree
c     of latitude and set constants on the first call to TMLL. A flag
c     is then set so that further calls can skip these calculations.
c
      if(flag.eq.1)go to 20
      flag=1
      ra=6.3782064d6
      rb=6.3565838d6
      esq=6.768657997291d-3
      eprmsq=esq/(1.0d0-esq)
      sinsec=3.1415926535d0/180.0d0
c
c     Note that the conversion factor (pi radians/ 180 degrees) is
c     equal to sin(1").
c
      do10 phi0=1,84
      phi=phi0*sinsec
 10   arclen(phi0)=i(phi)
c
c     By dividing the y value by the arclength of 1 degree an
c     approximate value for phi prime is obtained. This value is then
c     improved using successive approximations.
c
 20   phi0=ynorth/arclen(1)
 30   if(ynorth.ge.arclen(phi0))go to 40
      phi0=phi0-1
      if(phi0.le.0)go to 200
      go to 30
c
c     Calculate the arclength of the degree of latitude in which (X,Y)
c     is located.  Using this value we can improve the estimation of
c     phi prime.
c
 40   arcdeg=arclen(phi0+1)-arclen(phi0)
      phiprm=phi0+(ynorth-arclen(phi0))/arcdeg
      ncal=i(phiprm*sinsec)
      phiprm=phi0+(ncal-arclen(phi0))/arcdeg
c
c     Convert phiprm to radian measure so that Fortran trignometric
c     functions can be used.
c
      if(xeast.eq.ec)then
         lat=phiprm
         long=lamdac
         go to 200
      end if
      phiprm=phiprm*sinsec
      eprime=dabs(ec-xeast)
      xsign=(ec-xeast)/eprime
      nu=ra/(dsqrt(1.0d0-esq*dsin(phiprm)**2))
      vii=(dtan(phiprm)*(1.0d0+eprmsq*dcos(phiprm)**2))/(2.0d0*nu**2*
     &sinsec*k0**2)
      viiia=dtan(phiprm)/(24.0d0*nu**4*sinsec*k0**4)
      viiib=5.0d0+3.0d0*dtan(phiprm)**2+6.0d0*eprmsq*dcos(phiprm)**2-
     &6.0d0*eprmsq*dsin(phiprm)**2-3.0d0*eprmsq**2*dcos(phiprm)**4
     &9.0d0*eprmsq**2*dcos(phiprm)**2*dsin(phiprm)**2
      viii=viiia*viiib
      ix=1.0d0/(dcos(phiprm)*nu*sinsec*k0)
      x=(1.0d0+2.0d0*dtan(phiprm)**2+eprmsq*dcos(phiprm)**2)/(6.0d0*nu
     &**3*sinsec*k0**3*dcos(phiprm))
      d6a=(prime/(nu*k0))**6*dtan(phiprm)/(7.2d2*sinsec)
      d6b=6.1d1+9.0d1*dtan(phiprm)**2+4.5d1*dtan(phiprm)**4+1.07d2*
     &eprmsq*dcos(phiprm)**2-162.0d0*eprmsq*dsin(phiprm)**2-4.5d1*
     &eprmsq*dtan(phiprm)**2*dsin(phiprm)**2
      d6=d6a*d6b
      e5a=((eprime/(nu*k0))**5)/(1.2d2*sinsec*dcos(phiprm))
      e5b=5.0d0+2.8d1*dtan(phiprm)**2+2.4d1*dtan(phiprm)**4+6.0d0*eprmsq
     &*dcos(phiprm)**2+8.0d0*eprmsq*dsin(phiprm)**2
      e5=e5a*e5b
      phi=phiprm/sinsec-vii*eprime**2+viii*eprime**4-d6
      dlamda=ix*eprime-x*eprime**3+e5
      lambda=lamdac+xsign*dlamda
      lat=phi
      long=lambda
 200  return
      end
