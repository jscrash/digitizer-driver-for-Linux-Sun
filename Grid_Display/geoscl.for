C DEC/CMS REPLACEMENT HISTORY, Element GEOSCL.FOR
C *2     1-MAY-1991 12:37:22 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:10:16 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element GEOSCL.FOR
c list GEOSCL  
c  
c     *** Convert GEOFILE X-Y coordinates to plot inches ***
c  
c     Calling sequence:
c  
c     call GEOSCL(x,y,xp,yp)
c  
c     Arguments:
c  
c     Name     Input/     Type      Description
c              Output  
c  
c     x,y        I       Real*8     GEOFILE X-Y coordinates.
c  
c     xp,yp      O        Real      Plot coordinates
c  
c     Comments:
c  
c     This subroutine tranforms input coordinates to plot inches
c     given a scale, angle, and X-Y minimums.  These parameters are
c     passed through the shared data routines.  Note that as of June,
c     1987 the GEOFILE minimums are expressed as ROTATED coordinates.  
c  
c nolist GEOSCL
c  
      subroutine GEOSCL(x,y,xp,yp) 
      double precision xy(6),x,y,sfact,xrot,yrot
      data sfact/0.0d0/
      if(sfact.eq.0.0d0)then
c
c        Get data values from storage area
c
	 nvalue=6
         call DSTORE('GET XYFRAME',xy,nvalue,irtn)
	 nvalue=1
         call DSTORE('GET SCALE FACTOR',sfact,nvalue,irtn)
         call RSTORE('GET ANGLE',angle,nval,irtn)
         call RSTORE('GET X0',x0,nval,irtn)
         call RSTORE('GET Y0',y0,nval,irtn)
      end if
c  
c     Rotate coordinates
c  
      call ROTATE(x,y,angle,xrot,yrot) 
c  
c     Translate coordinates
c  
      xp=(xrot-xy(2))*sfact+x0 
      yp=(yrot-xy(4))*sfact+y0 
      return
      end  
