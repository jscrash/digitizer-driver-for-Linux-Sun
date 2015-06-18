C DEC/CMS REPLACEMENT HISTORY, Element LLNTS.FOR
C *2     1-MAY-1991 12:38:05 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:16:06 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element LLNTS.FOR
c list LLNTS
c
c     *** Convert latitude/longitude to NTS Sheet numbers ***
c
c     Calling sequence
c
c     call LLNTS(latin,longin,mscale,sheet,irtn)
c
c     Arguments:
c
c     Name    Input/   Type        Description
c
c
c     latin     I    Real*8        Latitude.
c
c     longin    I    Real*8        Longitude.
c
c     mscale    I    Integer       Map series scale:
c                                  10000,20000,50000 or 250000
c
c     sheet     O   Character*(*)  NTS Sheet number.
c
c     irtn      O     Integer      Return code: 0=successful
c
c     Comments:
c
c     The routine will determine the National Topographic sheet which
c     contains the latitude/longitude input.
c
c nolist LLNTS
c
      subroutine LLNTS(latin,longin,mscale,sheet,irtn)
      character*(*) sheet
      character letter*16,lquad*1,quads*8,quad16*2,block*1
      integer pquad,unit
      double precision latin,longin,lat,long,latse,longse,dlat,dlong
      data letter/'abcdefghijklmnop'/,quads/'seswnenw'/
      irtn=0
      lat=latin
      long=-longin
      sheet=' '
c
c     Calculate Primary Quadrangle.  Handle only "normal" NTS
c
      if(lat.lt.40.0d0.or.lat.gt.80.0d0.or.long.lt.48.0d0.or.long.gt.
     &176.0d0)go to 180
      nrow=(lat-40.0d0)/4.0d0
      ncol=(long-48.0d0)/8.0d0
      pquad=ncol*10+nrow
      latse=nrow*4.0d0+40.0d0
      longse=ncol*8.0d0+48.0d0
c
c     Determine lettered quadrangle.
c
      nrow=lat-latse+1
      nrow=MIN0(nrow,4)
      ncol=(long-longse)/2.0d0+1
      ncol=MIN0(ncol,4)
      jlet=(nrow-1)*4+ncol
      if(mod(nrow,2).eq.0)jlet=nrow*4-ncol+1
      lquad=letter(jlet:jlet)
c
c     Lettered quadrangle is smallest unit required for 1:250,000 series
c
      write(sheet,10)pquad,lquad
 10   format(i3,a1,i2,a,i3)
      if(mscale.ge.250000)go to 200
      latse=latse+nrow-1
      longse=longse+(ncol-1)*2.0d0
c
c     Determine 16th of lettered quadrangle.
c
      nrow=(lat-latse)/0.25d0+1.0d0
      nrow=MIN0(nrow,4)
      ncol=(long-longse)/0.5d0+1.0d0
      ncol=MIN0(ncol,4)
      jsix=(nrow-1)*4+ncol
      if(MOD(nrow,2).eq.0)jsix=nrow*4+1-ncol
c
c     16th is smallest unit required for 1:50,000 series
c
      write(sheet,10)pquad,lquad,jsix
      if(mscale.ge.50000)go to 200
      latse=latse+(nrow-1)*0.25d0
      longse=longse+(ncol-1)*0.5d0
c
c     Determine quadrant of 16th.
c
      nrow=(lat-latse)/0.125d0+1.0d0
      nrow=MIN0(nrow,2)
      ncol=(long-longse)/0.25d0+1.0d0
      ncol=MIN0(ncol,2)
      jchar=2*((nrow-1)*2+ncol)-1
      quad16=quads(jchar:jchar+1)
      write(sheet,10)pquad,lquad,jsix,quad16
      if(mscale.ge.20000)go to 200
c
c     Determine block and unit
c
      dlat=5.0d0/60.0d0
      dlong=7.5d0/60.0d0
      nrow=(lat-latse)/dlat+1.0d0
      nrow=MIN0(nrow,3)
      ncol=(long-longse)/dlong+1.0d0
      ncol=MIN0(ncol,4)
      jchar=(nrow-1)*4+ncol
      if(MOD(nrow,2).eq.0)jchar=nrow*4+1-ncol
      block=letter(jchar:jchar)
      latse=latse+(nrow-1)*dlat
      longse=longse+(ncol-1)*dlong
      dlat=dlat/10.0d0
      dlong=dlong/10.0d0
      nrow=(lat-latse)/dlat+1.0d0
      nrow=MIN0(nrow,10)
      ncol=(long-longse)/dlong+1.0d0
      ncol=MIN0(ncol,10)
      unit=(nrow-1)*10+ncol
      write(sheet,10)pquad,lquad,jsix,block,unit
      go to 200
 180  irtn=-1
 200  return
      end
