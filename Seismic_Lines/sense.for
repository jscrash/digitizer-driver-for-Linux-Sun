C DEC/CMS REPLACEMENT HISTORY, Element SENSE.FOR
C *2     1-MAY-1991 12:39:42 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:27:53 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element SENSE.FOR
c list SENSE
c
c     **Routine to determine the sense of direction of a polygon**
c
c     Calling sequence:
c
c     call SENSE(xy,n,isense)
c
c     Arguments:
c
c     Name    Input/      Type       Description
c             output
c
c     xy        I        Real(n*2)   Array containing x/y values
c                                    defining polygon.
c
c     n         I        Integer     Number of points in polygon.
c
c     isense    O        Integer     Sense of direction (1=clockwise,
c                                    -1=counter-clockwise)
c     Comments:
c
c     This subroutine scans the input polygon to determine the
c     sense of direction.  ISENSE will be set to 0 if there
c     are less than three unique points in the polygon.  ISENSE=1
c     indicates that the polygon is defined in a clockwise sense
c     while isense=-1 indicates that the polygon is defined in
c     a counter-clockwise sense.
c
c nolist SENSE
c
c*****This routine was written for Gulf Canada Resources by Davis Swan
c     in May, 1985.
c
      subroutine sense(xy,n,isense)
      real xy(2*n)
      logical diag
      common/diags/diag
      data pi/3.14159/,tolrnc/0.0001/
      isense=0
      if(n.lt.3)go to 60
      nloop=n+2
      ipnt=0
      ncnt=1
      sumdif=0.0
      x2=0.0
      y2=0.0
      do50 i=1,nloop
      ipnt=ipnt+1
      if(ipnt.gt.n)ipnt=1
      x3=xy(2*ipnt-1)
      y3=xy(2*ipnt)
      if(i.eq.1)go to 40
c
c     Check for unique point
c
      if(x3.eq.x2.and.y3.eq.y2)go to 50
      ncnt=ncnt+1
c
c     Calculate angle of this line segment
c
      call POLRCO(x3,y3,x2,y2,ang,r)
c
c     Sum angles
c
      if(ncnt.lt.3)go to 30
      diff=ANGDIF(anglst,ang)
      if(ABS(diff-pi).lt.tolrnc)then
c
c        A side of the polygon doubles back on itself.  Reset the
c        last point and go on to the next.
c        
         x2=x3
         y2=y3
         go to 50
      end if
      sumdif=sumdif+diff
 30   anglst=ang
 40   x1=x2
      y1=y2
      x2=x3
      y2=y3
 50   continue
      if(ncnt.lt.3)go to 60
      isense=1
      if(sumdif.lt.0.0)isense=-1
 60   if(diag)write(6,65)sumdif,isense
 65   format('0*** DIAGNOSTIC ***SENSE -Accumulated angle is ',f8.3,
     &', sense is ',i2)
      return
      end
