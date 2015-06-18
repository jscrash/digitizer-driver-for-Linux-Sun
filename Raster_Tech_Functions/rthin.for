C DEC/CMS REPLACEMENT HISTORY, Element RTHIN.FOR
C *2     1-MAY-1991 12:39:28 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:24:17 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element RTHIN.FOR
c********************************************************************
c
c
c                 Copyright Gulf Canada Resources, 1990
c                  Unpublished -- All rights reserved
c
c THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
c IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
c WITHOUT PRIOR WRITTEN CONSENT OF
c
c                       Gulf Canada Resources, Inc.
c                       401 9th Avenue SW
c                       Calgary, Alberta
c                       Canada T2P 2H7
c                       (403) 233-4000
c
c (The above notice does not constitute an admission of publication or
c  unrestricted dissemination of the work)
c*********************************************************************
c list RTHIN
c
c     *** Eliminate unnecessary points along a plotted curve ***
c
c     Calling sequence:
c
c     call RTHIN(reset,x,y,angmax,dmin,r,bend,skip)
c
c     Arguments:
c
c     Name    Input/    Type         Description
c             output
c
c     reset     I      Logical       Reset for start of line.
c
c     x,y       I       Real         Coordinates for next point.
c
c     angmax    I       Real         Maximum angle allowed between 3
c
c     dmin      I       Real         Minimum distance between points.
c
c     r         O       Real         Actual distance between points.
c
c     bend      O       Real         Actual angle between last 3 points.
c
c     skip      O      Logical       Skip flag.
c
c     Comments:
c
c     This subroutine accepts line x-y data in plot inches and indicates
c     whether or not the current data point is required to produce
c     a smooth line based upon the following criteria; 1)the angle
c     between any two line segments will not exceed angmax, or
c     2) the distance between any two plotted data points will not
c     be less than dmin.  The logical variable skip will be true unless
c     the input data point is required.  Note that the routine
c     should be initialized on the first call for a given line by
c     specifying an angmax value of 0.0 .  The actual distance r and
c     bend are also returned to the calling routine.
c
c nolist RTHIN
c*****This routine was written for Gulf Canada Resources by Davis Swan
c     in November, 1982.  Re-written in June, 1987.
c
      subroutine RTHIN(reset,x,y,angmax,dmin,r,bend,skip)
      real angle(2),null
      logical skip,diag,shiftp,reset
      common/diags/diag
      data null/-1.0e-15/,pi/3.14159/,shiftp/.false./
      skip=.true.
c
c     Reset 1st point
c
      if(reset)then
         x1=x
         y1=y
         x2=null
         y2=null
         skip=.false.
         shiftp=.false.
         reset=.false.
         go to 200
      end if
c
c     Reset 2nd point
c
      if(x2.eq.null)then
         if(x.eq.x1.and.y.eq.y1)go to 200
         skip=.false.
         x2=x
         y2=y
         go to 200
      end if
      if(x.eq.x2.and.y.eq.y2)go to 200
c
c     Convert cartesian to polar coordinates to calculate curvature.
c
      call POLRCO(x,y,x2,y2,angle(2),r)
      if(r.lt.dmin)go to 200
      call POLRCO(x2,y2,x1,y1,angle(1),r)
      bend=ANGDIF(angle(1),angle(2))*180.0/pi
      if(ABS(bend).le.angmax)then
         if(ABS(bend).le.angmax/2.0.or.shiftp)go to 200
c
c        If this is the first arc and the angle is greater than 1/2 the
c        maximum bend allowed, reset x2,y2.  Changing this "pivot" point
c        will produce better sampling of smoothly varying curves.
c
         shiftp=.true.
         go to 150
      end if
      skip=.false.
c
c     Reset reference points for curvature calculation.
c
      x1=x2
      y1=y2
 150  x2=x
      y2=y
 200  return
      end
