/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_AREA.C */
/* *2    18-MAR-1991 18:06:47 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:46:40 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_AREA.C */
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
/* list ag_poly_area
 *
 *     *** Calculate the area and centre for a convex polygon ***
 *
 *     Calling sequence:
 *
 *     ag_poly_area(xpoly,ypoly,np,wratio,&area,&xcentr,&ycentr,&wmax)
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I    double[np]     X coordinates for polygon.
 *
 *     ypoly     I    double[np]     Y coordinates for polygon.
 *
 *     np        I      INT          Number of points in polygon.
 *
 *     wratio    I     DOUBLE        Width to height ratio for rectangle
 *                                   to be fit inside polygon.
 *
 *     *area     O     DOUBLE        Area of the polygon.
 *
 *     *xcentr   O     DOUBLE        X coordinate of polygon centre.
 *
 *     *ycentr   O     DOUBLE        Y coordinate of polygon centre.
 *
 *     *wmax     O     DOUBLE        Width of rectangle that will fit
 *                                   inside the polygon.
 *
 *     Comments:
 *
 *     This routine calculates the area and center of gravity of the
 *     input convex polygon.  If the user inputs a non-zero value for
 *     wratio the routine will then examin the convex polygon to
 *     determine the largest rectangle with the width/heigth ratio as
 *     specified, that will fit inside the polygon.  Note that the
 *     values xcentr and ycentr will be moved to the center of the
 *     rectangle in this case.
 *
 *     Revision History:
 *
 *     Written for Gulf Canada Resources by Davis Swan in November, 1986.
 *     Revised to keep sign of apex angle so that concave polygons are
 *     handled correctly in February, 1988.
 *
 * nolist ag_poly_area
 */
void /*FUNCTION*/ ag_poly_area(xpoly, ypoly, np, wratio, area, xcentr, ycentr, 
	 wmax)
DOUBLE xpoly[], ypoly[];
INT np;
DOUBLE wratio, *area, *xcentr, *ycentr, *wmax;
{
INT i, isense, jp, jquad, jquad_, nang, ncross, 
	 nquad;
DOUBLE absang, acentr, adiag, angle1, angle2, angmax, apex, arc, atest[4], 
	 r1, r2, rmax, suma, sumx, sumy, x0, x1, x2, xa, xc, xcornr[4], 
	 xmax, xmin, xp, y0, y1, y2, ya, yc, ycornr[4], ymax, ymin, yp;
DOUBLE ag_arc_angle();
void ag_intersxn(), ag_polar_coord();
static DOUBLE pi = 3.14159;


	/*     Calculate area of polygon and length of the side of the largest
	 *     square that can be placed at the center of the polygon.
	 */
	if( np < 3 )
		goto L_200;
	sumx = 0.0;
	sumy = 0.0;
	suma = 0.0;
	*wmax = 0.0;
	*xcentr = 0.0;
	*ycentr = 0.0;
	x0 = xpoly[0];
	y0 = ypoly[0];
	nang = 0;
	for( jp = 2; jp <= np; jp++ ){
		x2 = xpoly[jp-1];
		y2 = ypoly[jp-1];

		/*     Ignore duplicate points
		 */
		if( x2 == x0 && y2 == y0 )
			goto L_45;
		nang += 1;
		if( nang == 1 ){
			ag_polar_coord( x2, y2, x0, y0, &angle1, &r1 );
			goto L_40;
			}
		else{
			ag_polar_coord( x2, y2, x0, y0, &angle2, &r2 );

			/*        Calculate the apex angle and area of this triangle
			 */
			apex = ag_arc_angle( angle1, angle2 );
			*area = 0.5*r2*r1*sin( apex );
			xa = (x0 + x1 + x2)/3.0;
			ya = (y0 + y1 + y2)/3.0;
			sumx += xa*ABS( *area );
			sumy += ya*ABS( *area );
			suma += *area;
			}
		angle1 = angle2;
		r1 = r2;
L_40:
		x1 = x2;
		y1 = y2;
		goto L_50;
L_45:
		nang = 0;
L_50:
		;
		}

	/*     Calculate centroid of polygon.
	 */
	suma = ABS( suma );
	*area = suma;
	if( *area < 0.001 )
		goto L_200;
	*xcentr = sumx/suma;
	*ycentr = sumy/suma;

	/*     Calculate the largest rectangle with a height/width ratio
	 *     as specified by hratio that can be plotted inside this convex
	 *     polygon.  Some adjustment of the xcentr,ycentre values may be
	 *     required.
	 */
	if( wratio <= 0.01 )
		goto L_200;

	/*     Calculate the angle of the rectangle diagonals.
	 */
	adiag = atan( 1.0/ wratio );
	atest[0] = adiag;
	atest[1] = pi - adiag;
	atest[2] = pi + adiag;
	atest[3] = 2.0*pi - adiag;
	x0 = *xcentr;
	y0 = *ycentr;
	nang = 0;
	nquad = 0;
	for( i = 1; i <= np; i++ ){
		x2 = xpoly[i-1];
		y2 = ypoly[i-1];
		nang += 1;
		ag_polar_coord( x2, y2, x0, y0, &angle2, &r2 );
		if( nang == 1 )
			goto L_90;

		/*     Check if any of the rectangle diagonals cut this line segment.
		 */
		if( angle2 == angle1 )
			goto L_100;
		arc = 0.5*ABS( ag_arc_angle( angle1, angle2 ) );
		acentr = MIN( angle1, angle2 ) + arc;
		angmax = MAX( angle1, angle2 );
		if( angmax - acentr > pi/2.0 )
			acentr += 2.0*pi - 2.0*arc;
		for( jquad = 1; jquad <= 4; jquad++ ){
			jquad_ = jquad - 1;
			absang = ABS( ag_arc_angle( atest[jquad_], acentr ) );
			if( absang > arc )
				goto L_80;

			/*     Find intersection with line segment
			 */
			rmax = MAX( r1, r2 );
			xp = x0 + rmax*cos( atest[jquad_] );
			yp = y0 + rmax*sin( atest[jquad_] );
			ag_intersxn( x1, y1, x2, y2, x0, y0, xp, yp, &xc, &yc, 
			 &ncross, &isense );
			if( ncross == 0 )
				goto L_80;
			xcornr[jquad_] = xc;
			ycornr[jquad_] = yc;
			nquad += 1;
L_80:
			;
			}
L_90:
		x1 = x2;
		y1 = y2;
		angle1 = angle2;
		r1 = r2;
L_100:
		;
		}

	/*     Determine dimensions of rectangle
	 */
	xmin = MAX( xcornr[1], xcornr[2] );
	xmax = MIN( xcornr[0], xcornr[3] );
	ymin = MAX( ycornr[2], ycornr[3] );
	ymax = MIN( ycornr[0], ycornr[1] );

	/*     Adjust xcentr,ycentr to centre of defined rectangle.
	 */
	*xcentr = (xmax + xmin)/2.0;
	*ycentr = (ymax + ymin)/2.0;
	*wmax = xmax - xmin;
L_200:
	return;
} /* end of function */
