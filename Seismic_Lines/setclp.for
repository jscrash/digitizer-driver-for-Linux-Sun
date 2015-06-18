C DEC/CMS REPLACEMENT HISTORY, Element SETCLP.FOR
C *2     1-MAY-1991 12:39:50 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:28:23 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element SETCLP.FOR
c list SETCLP
c
c     **Returns clipping information for the current polygon**
c     ***THIS ROUTINE SHOULD BE ELIMINATED ASASP***
c
c     Calling sequence:
c
c     call SETCLP(lpoly,isense,nbord,itype,noff,xylims)
c
c     Arguments:
c
c     Name    Input/  Type    Range  Description
c             Output
c
c     lpoly   I,O    Integer         Polygon to set (0 initially)
c
c     isense   O     Integer  -1,1   Sense of direction of active
c                                    polygon (1=counter-clockwise)
c
c     nbord    O     Integer   n/a   Number of points defining polygon.
c
c     itype    O     Integer   0,1   Polygon type(0=clipping,1=blanking,
c                                    -1=inactive)
c
c     noff     O     Integer   n/a   Offset into the xybord array for
c                                    polygon coordinates.
c
c     xylims   O     Real(4)         Outer limits of border for
c                                    optimization.
c
c     Comments:
c
c        There are two common blocks used by the clipping software.
c     CLIPCB contains the number of clipping polygons defined,
c     the number of the polygon currently active, followed by 6 integer
c     numbers for each polygon, indicating sense of direction,
c     the number of points, and the polygon type (0=clipping,
c     1=blanking,-1=inactive). In most applications only one clipping
c     polygon will be used.  The CLIPXY common block contains the actual
c     x,y coordinates (in plot inches) for the clipping polygon(s).
c     Note that the sense of direction of the clipping polygon
c     is -1 if the polygon is defined clockwise, 1 if it is
c     defined counter-clockwise.
c
c        This subroutine examins the clipping common blocks to determine
c     the currently active clipping polygon.  It returns the sense
c     of direction of the current polygon, the number of points
c     defining the polygon, and the offset into the xybord array of the
c     first pair of x,y values for this polygon.
c
c nolist SETCLP
c
c*****This routine was written for Gulf Canada Resources by Davis Swan
c     in June, 1985.  Modified to handle polygon redefinition in Sept.,
c     1986.  Optimization improved by adding an inner rectangle in
c     January, 1988.
c
      subroutine SETCLP(lpoly,isense,nbord,itype,noff,xylims)
      parameter (maxpts=50)
      real xylims(8),xyopt(80)
      integer info(54)
      common/clipcb/info
      common/clipop/xyopt
c
c     Determine parameters for current clipping polygon
c
      nhead=info(1)
      ipoly=info(3)
      nflag=info(4)
      ioff=(ipoly-1)*nflag+nhead+1
      iactiv=info(ioff+4)
      if(iactiv.eq.-1)go to 200
      lpoly=ipoly
      isense=info(ioff)
      noff=info(ioff+1)*2-2
      nbord=info(ioff+2)
      itype=info(ioff+3)
c
c     Copy limits from clipping common
c
      joff=(ipoly-1)*8
      do10 i=1,8
      xylims(i)=xyopt(joff+i)
 10   continue
 200  return
      end
