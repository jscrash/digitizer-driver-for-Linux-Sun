C DEC/CMS REPLACEMENT HISTORY, Element RTRANS.FOR
C *2     1-MAY-1991 12:39:33 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:25:36 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element RTRANS.FOR
c list RTRANS(valin,nvals,valout)
c
c     *** Routine to transfer data from one array to another ***
c
c nolist RTRANS
c
      subroutine RTRANS(valin,nvals,valout)
      real valin(1),valout(1)
      do50 i=1,nvals
      valout(i)=valin(i)
 50   continue
      return
      end
