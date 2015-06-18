/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLR_CRD.C */
/* *2    18-MAR-1991 18:06:41 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:46:15 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLR_CRD.C */
/************************************************************************


                  Copyright Gulf Canada Resources, 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
WITHOUT PRIOR WRITTEN CONSENT OF

			Gulf Canada Resources, Inc.
			401 9th Avenue SW
			Calgary, Alberta
                        Canada T2P 2H7
			(403) 233-4000

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)
************************************************************************/
/*Translated by FOR_C++, v0.2, on 09/14/90 at 14:34:21 */
/*FOR_C++ Options SET: cio op=a c */
#include "esi_c_lib.h"
#include "esi_math.h"
/*********************************************************************
 *
 *                 Copyright Gulf Canada Resources, 1990
 *                  Unpublished -- All rights reserved
 *
 * THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
 * IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
 * WITHOUT PRIOR WRITTEN CONSENT OF
 *
 *                       Gulf Canada Resources, Inc.
 *                       401 9th Avenue SW
 *                       Calgary, Alberta
 *                       Canada T2P 2H7
 *                       (403) 233-4000
 *
 * (The above notice does not constitute an admission of publication or
 *  unrestricted dissemination of the work)
 **********************************************************************
 * list                ******ag_polar_coord******
 *
 *     this subroutine calculates the position of a point p (x,y) in
 *     terms of a polar coordinate system having origin (xorign,yorign) 
 *     and measuring 'angle' counter-clockwise from the positive x  
 *     axis.
 *
 *     ag_polar_coord(x,y,xorign,yorign,&angle,&r) 
 *
 *     Arguments:
 *
 *     Name       Input/     Type        Description
 *                output
 *
 *     x            I       DOUBLE       x coordinate of point.  
 *
 *     y            I       DOUBLE       y coordinate of point.  
 *
 *     xorign       I       DOUBLE       x origin 
 *
 *     yorign       I       DOUBLE       y origin
 *
 *     *angle       O       DOUBLE       Calculated angle in the range 0-2 pi.
 *
 *     *r           O       DOUBLE       Distance from origin  
 *
 *     Revision history:
 *
 *     This routine was written for the Geological Survey of Canada 
 *     by Davis Swan in 1976.
 *
 * nolist
 */
void /*FUNCTION*/ ag_polar_coord(x, y, xorign, yorign, angle, r)
DOUBLE x, y, xorign, yorign, *angle, *r;
{
DOUBLE pi, signy, xprime, yprime;

	pi = 3.1415927;
	*angle = 0.0;
	xprime = x - xorign;
	yprime = y - yorign;
	*r = sqrt( xprime*xprime + yprime*yprime );
	if( xprime == 0.0 && yprime == 0.0 )
		goto L_200;
	if( yprime != 0.0 )
		signy = ABS( yprime )/yprime;
	if( xprime == 0.0 )
		goto L_2;
	*angle = atan( yprime/xprime );
	if( xprime > 0.0 )
		goto L_1;
	*angle += pi;
	goto L_200;
L_1:
	if( yprime < 0.0 )
		*angle += 2.0*pi;
	goto L_200;
L_2:
	*angle = pi - 0.5*pi*signy;
	goto L_200;
L_200:
	return;
} /* end of function */
