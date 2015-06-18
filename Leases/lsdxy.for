C DEC/CMS REPLACEMENT HISTORY, Element LSDXY.FOR
C *2     1-MAY-1991 12:38:12 KEE "(SPR -1) Fix bug and modify Gulf Canada DLS/NTS for vms and sun version"
C *1     1-MAY-1991 12:17:10 KEE "DLS/NTS"
C DEC/CMS REPLACEMENT HISTORY, Element LSDXY.FOR
c list LSDXY
c
c     *** Generate LSD corner coordinates from section corners ***
c
c     Calling sequence:
c
c     call LSDXY(sx,sy,sra,wra,lx,ly)
c
c     Arguments:
c
c     Name      Input/       Type        Description
c               output
c
c     sx          I        Real*8(4)     Section x coordinates.
c
c     sy          I        Real*8(4)     Section y coordinates.
c
c     sra         I         Real*8       South road allowance.
c
c     wra         I         Real*8       West road allowance.
c
c     lx          O       Real*8(25)     LSD x coordinates.
c
c     ly          O       Real*8(25)     LSD y coordinates.
c
c     Comments:
c
c     Given 4 corners of a section sx,sy generate LSD corners in
c     lx,ly by division as shown below:
c
c          lx,ly                  sx,sy
c     25 13 14 15 16        4  .  .  .  .  1
c     24 12 11 10  9        .  .  .  .  .  .
c     23  5  6  7  8        .  .  .  .  .  .
c     22  4  3  2  1        .  .  .  .  .  .
c     21 20 19 18 17        3  .  .  .  .  2
c
c     Revision history:
c
c     Written by Steve Cox for Gulf Canada Resources.
c
c nolist LSDXY
c
      subroutine LSDXY(sx,sy,sra,wra,lx,ly)
c
      common/diags/diag
      logical diag
c
c
      double precision sx(4),sy(4),sra,wra
      double precision lx(25),ly(25)
c
c     Corners
c
      lx(16)=sx(1)
      ly(16)=sy(1)
      lx(17)=sx(2)
      ly(17)=sy(2)+sra
      lx(21)=sx(3)+wra
      ly(21)=sy(3)+sra
      lx(25)=sx(4)+wra
      ly(25)=sy(4)
c
c     North
c
      lx(14)=(lx(25)+lx(16))/2.0d0
      ly(14)=(ly(25)+ly(16))/2.0d0
      lx(13)=(lx(25)+lx(14))/2.0d0
      ly(13)=(ly(25)+ly(14))/2.0d0
      lx(15)=(lx(14)+lx(16))/2.0d0
      ly(15)=(ly(14)+ly(16))/2.0d0
c
c     South
c
      lx(19)=(lx(21)+lx(17))/2.0d0
      ly(19)=(ly(21)+ly(17))/2.0d0
      lx(20)=(lx(21)+lx(19))/2.0d0
      ly(20)=(ly(21)+ly(19))/2.0d0
      lx(18)=(lx(19)+lx(17))/2.0d0
      ly(18)=(ly(19)+ly(17))/2.0d0
c
c     East
c
      lx( 8)=(lx(16)+lx(17))/2.0d0
      ly( 8)=(ly(16)+ly(17))/2.0d0
      lx( 1)=(lx( 8)+lx(17))/2.0d0
      ly( 1)=(ly( 8)+ly(17))/2.0d0
      lx( 9)=(lx( 8)+lx(16))/2.0d0
      ly( 9)=(ly( 8)+ly(16))/2.0d0
c
c     West
c
      lx(23)=(lx(25)+lx(21))/2.0d0
      ly(23)=(ly(25)+ly(21))/2.0d0
      lx(22)=(lx(21)+lx(23))/2.0d0
      ly(22)=(ly(21)+ly(23))/2.0d0
      lx(24)=(lx(23)+lx(25))/2.0d0
      ly(24)=(ly(23)+ly(25))/2.0d0
c
c     Center
c
      lx( 6)=(lx(14)+lx(19))/2.0d0
      ly( 6)=(ly(14)+ly(19))/2.0d0
c
      lx( 7)=(lx(18)+lx(15))/2.0d0
      ly( 7)=(ly(18)+ly(15))/2.0d0
      lx( 5)=(lx(13)+lx(20))/2.0d0
      ly( 5)=(ly(13)+ly(20))/2.0d0
      lx(11)=(lx( 6)+lx(14))/2.0d0
      ly(11)=(ly( 6)+ly(14))/2.0d0
      lx( 3)=(lx( 6)+lx(19))/2.0d0
      ly( 3)=(ly( 6)+ly(19))/2.0d0
c
      lx( 2)=(lx(18)+lx( 7))/2.0d0
      ly( 2)=(ly(18)+ly( 7))/2.0d0
      lx( 4)=(lx( 5)+lx(20))/2.0d0
      ly( 4)=(ly( 5)+ly(20))/2.0d0
      lx(10)=(lx(15)+lx( 7))/2.0d0
      ly(10)=(ly(15)+ly( 7))/2.0d0
      lx(12)=(lx( 5)+lx(13))/2.0d0
      ly(12)=(ly( 5)+ly(13))/2.0d0
      return
      end
