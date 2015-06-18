C DEC/CMS REPLACEMENT HISTORY, Element CPARXY.FOR
C *2     1-MAY-1991 12:36:41 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:05:33 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element CPARXY.FOR
c list CPARXY
c
c     *** Create a line string parallel to that input ***
c
c     Calling sequence:
c
c     call CPARXY(xypin,npin,isense,pdist,maxout,xypout,npout)
c
c     Arguments:
c
c     Name   Input/     Type         Description
c            output
c
c     xypin    I      Real(npin)     X,Y coordinates for input line.
c
c     npin     I       Integer       Number of input points.
c
c     isense   I       Integer       Sense of direction of output line.
c                                    -1=clockwise, 1=counterclockwise
c
c     pdist    I        Real         Perpendicular distance between lines.  
c
c     maxout   I       Integer       Maximum output points allowed.
c
c     xypout   O      Real(npout)    X,Y coordinates for output line. 
c
c     npout    O       Integer       Number of output points.
c
c     Comments:
c
c     This routine creates a line parallel to the input line, either
c     on the clockwise (isense=-1) or the counter-clockwise (isense=1)
c     side of the input line.  Identical to CPARAL except that the
c     coordinates are passed as paired arrays rather than vectors.
c
c nolist CPARXY
c
      subroutine CPARXY(xypin,npin,isense,pdist,maxout,xypout,npout)
      real xypin(2,npin)
      real xypout(2,maxout)
      logical diag
      common/diags/diag
      data pi/3.14159/,angnow/0.0/
c     (CP) npout didn't start at initial value. now it does.
      npout = 0
      if(npin.le.2)go to 200
      poff=pdist*isense
      do100 jp=1,npin
      j=jp
      x2=xypin(1,j)
      y2=xypin(2,j)
      if(jp.eq.1)go to 90
c
c     Check for duplicate points
c
      if(x2.eq.x1.and.y2.eq.y1)go to 100
c
c     Calculate angle along line          
c
      anglst=angnow
      wfact=1.0
      call POLRCO(x2,y2,x1,y1,angnow,r)
      if(jp.eq.2)then
c
c        Use first angle to plot first point.
c
         angle=angnow
      else
c
c        Use the average angle for points between the first and last.
c
         diff=angnow-anglst
         if(diff.gt.pi)diff=diff-2.0*pi
         if(diff.lt.-pi)diff=diff+2.0*pi
c
c        Calculate factor for line offset
c
         wfact=AMAX1(0.2,COS(0.5*(ABS(diff))))
         angle=anglst+diff/2.0
         if(angle.gt.2.0*pi)angle=angle-2.0*pi
         if(angle.lt.0.0)angle=angle+2.0*pi
         if(diff*isense.gt.pi/1.9)then
c
c        If the line bends more than pi/2 round out the corner.  This is
c        done by plotting points at the ends of the line segments as well
c        as at the intersection of the line segments as shown below.
c
c                  \
c                   \     *
c                    \    
c            ----------
c                          *
c                         
c                     *
c
            x=x1+poff*SIN(anglst)
            y=y1-poff*COS(anglst)
            npout=MIN0(maxout,npout+1)
            xypout(1,npout)=x
            xypout(2,npout)=y
            x=x1+poff*SIN(angle)
            y=y1-poff*COS(angle)
            npout=MIN0(maxout,npout+1)
            xypout(1,npout)=x
            xypout(2,npout)=y
            angle=angnow
            wfact=1.0
         end if
      end if
      xoff=(poff/wfact)*SIN(angle)
      yoff=(poff/wfact)*COS(angle)
      x=x1+xoff
      y=y1-yoff
      npout=MIN0(maxout,npout+1)
      xypout(1,npout)=x
      xypout(2,npout)=y
 90   x1=x2
      y1=y2
 100  continue
c
c     Plot the last point
c
      x=x2+poff*SIN(angnow)
      y=y2-poff*COS(angnow)
      npout=MIN0(maxout,npout+1)
      xypout(1,npout)=x
      xypout(2,npout)=y
 200  return
      end
