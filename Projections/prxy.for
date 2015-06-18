C DEC/CMS REPLACEMENT HISTORY, Element PRXY.FOR
C *2     1-MAY-1991 12:38:45 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:19:47 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element PRXY.FOR
      subroutine PRXY(xy,nxy,itr,isec,imer)
      real xy(2*nxy)
      integer itr, isec, imer
      double precision xworld,yworld
      logical newseg

      newseg = .false.
      do10 i=1,nxy
      iflag = i
      if (newseg)  then
          iflag = 1
          newseg = .false.
      endif
      if (xy(2*i-1).eq.-999.0) then
          newseg = .true.
          go to 10
      endif
      call UNSCL(xy(2*i-1),xy(2*i),xworld,yworld)
      xworld=xworld*0.3048d0
      yworld=yworld*0.3048d0
      if (itr.gt.0) mer = 0
      call dls_nlist_point(xworld,yworld,isec,itr,imer,iflag)
 10   continue
      return
      end
