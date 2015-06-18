C DEC/CMS REPLACEMENT HISTORY, Element RSET.FOR
C *2     1-MAY-1991 12:39:20 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:23:19 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element RSET.FOR
c list RSET
c  
c     *** Set the values of a real array to a constant *** 
c  
c     Calling sequence:
c  
c     call RSET(array,n,value)
c  
c     Arguments:
c  
c     Name   Input/      Type          Description 
c            output
c  
c     array    O        Real(n)        Real array.  
c  
c     n        I        Integer        Number of elements in iarray.
c  
c     value    I         Real          Constant
c  
c nolist RSET  
c  
      subroutine RSET(array,n,value) 
      real array(n)
      do50 i=1,n
      array(i)=value 
 50   continue 
      return
      end  
