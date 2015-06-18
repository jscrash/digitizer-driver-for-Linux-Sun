C DEC/CMS REPLACEMENT HISTORY, Element LLDLS.FOR
C *2     1-MAY-1991 12:38:02 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:15:16 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element LLDLS.FOR
c list LLDLS
c
c     *** Convert lat/long to Township/range/meridian ***
c
c     Calling sequence:
c
c     call LLDLS(llat,llong,ntwp,nrng,nsecn,lsd,merid,irtn)
c
c     Arguments:
c
c     Name    Input/   Type      Description
c             Output
c
c     llat      I     Real*8     Latitude of point.
c
c     llong     I     Real*8     Longitude of point
c
c     ntwp      O     Integer    Township that point is in.
c
c     nrng      O     Integer    Range that point is in.
c
c     nsecn     0     Integer    Section that point is in.
c
c     lsd       O     Integer    Legal sub-division that point is in.
c
c     merid     O     Integer    Meridian that the point is west of.
c
c     irtn      O     Integer    Return code: 0=successful
c
c     Comments:
c
c     This subroutine uses an input latitude and longitude to
c     calculate the township, range, section, and legal subdivision
c     for a point.  If the input parameters are not valid irtn will
c     have a negative value.
c
c     Revision history:
c
c     Written for Gulf Canada Resources by Davis Swan in 1982.
c     Converted to ASCII in 1983.
c
c
c nolist LLDLS
c
      subroutine LLDLS(llat,llong,ntwp,nrng,nsecn,lsd,merid,irtn)
c
c*****This subroutine was written for Gulf Canada Resources by Davis
c     Swan in February, 1983.
c
      double precision lat,long,llat,llong,pmlong(6),lat1,long1
      logical diag
      common/diags/diag
      data pmlong/97.45833333d0,102.0064236d0,106.0d0,110.0d0,114.0d0,
     &118.0d0/,diag/.false./
      ntwp=0
      nrng=0
      nsecn=0
      lsd=0
      merid=0
      lat=llat
      long=-llong
      if(diag)write(6,5)lat,long
 5    format('0*** DIAGNOSTIC ***LLDLS -LAT/LONG ARE ',2f12.5)
      if(lat.lt.45.0d0.or.lat.gt.65.0d0.or.long.lt.90.0d0.or.
     &long.gt.130.0d0)go to 200
c
c     Find the prime meridian for this point.
c
      merid=1
      do10 i=1,6
      if(pmlong(i).gt.long)go to 15
      merid=i
 10   continue
c
c     Find the township which contains this latitude.
c
 15   ltwp=(lat-49.0d0)/0.09d0
      ltwp=MIN0(126,MAX0(ltwp,1))
      do20 itwp=ltwp,126
      ntwp=itwp
      call DLSLL(2,itwp,1,31,13,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(lat1.gt.lat)go to 30
 20   continue
c
c     Find the range which contains this longitude.
c
 30   maxrng=35
      if(merid.eq.6)maxrng=26
      lrng=(long-pmlong(merid))/0.18d0
      lrng=MIN0(maxrng,MAX0(lrng,1))
      do40 irng=lrng,maxrng
      nrng=irng
      call DLSLL(2,ntwp,irng,31,13,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(dabs(long1).gt.long)go to 45
      if(irtn.eq.99)go to 45
 40   continue
c
c     Find the section row for the input point.
c
 45   do50 irow=1,6
      isecn=(irow-1)*6+1
      nrow=irow
      call DLSLL(2,ntwp,nrng,isecn,13,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(lat1.gt.lat)go to 55
      if(irtn.eq.99)go to 55
 50   continue
c
c     Find the section column for the input point.
c
 55   do60 icol=1,6
      ncol=icol
      call DLSLL(2,ntwp,nrng,icol,13,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(dabs(long1).gt.long)go to 65
      if(irtn.eq.99)go to 65
 60   continue
 65   nsecn=(nrow-1)*6+ncol
      if(mod(nrow,2).eq.0)nsecn=nrow*6-ncol+1
c
c     Find the LSD row for the input point.
c
      do70 irow=1,4
      ilsd=irow*4
      nrow=irow
      call DLSLL(2,ntwp,nrng,nsecn,ilsd,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(lat1.gt.lat)go to 75
 70   continue
c
c     Find the LSD column for the input point.
c
 75   do80 icol=1,4
      ncol=icol
      call DLSLL(2,ntwp,nrng,nsecn,icol,merid,lat1,long1,irtn)
      if(irtn.lt.0)go to 180
      if(dabs(long1).ge.long)go to 85
 80   continue
 85   lsd=(nrow-1)*4+ncol
      if(mod(nrow,2).eq.0)lsd=nrow*4-ncol+1
      if(diag)write(6,87)lsd,nsecn,ntwp,nrng,merid
 87   format('0*** DIAGNOSTIC ***LLDLS -Point is in ',2(i2,'-'),i3,
     &'-',i2,'W',i1)
      go to 200
 180  irtn=-1
 200  return
      end
