/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_GAP.C */
/* *2    18-MAR-1991 18:07:09 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:48:09 JULIAN "0" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_GAP.C */
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
/* list ag_poly_gap
 *
 *     *** Find "convex" x,y within a gap in a polygon ***  
 *
 *     Calling sequence:
 *
 *     ag_poly_gap(xpoly,ypoly,np,&iconcv,&iconvx,xout,yout,&nout)  
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I     double[np]    X coordinates for polygon. 
 *
 *     ypoly     I     double[np]    X coordinates for polygon. 
 *
 *     np        I         INT       Number of points in polygon.
 *
 *     iconcv    I         INT       Concav point at start of gap.  
 *
 *     iconvx    I         INT       Convex point at end of gap.
 *
 *     xout     I,O    double[nout]  X coordinates of points in gap.
 *
 *     yout     I,O    double[nout]  Y coordinates of points in gap.
 *
 *     *nout    I,O         INT      Number of points in gap.  
 *
 *     Comments:
 *
 *     This routine will determine the points required to bridge a gap  
 *     in a polygon.
 *
 *     Revision history:
 *
 *     Written for Gulf Canada Resources by Davis Swan in July, 1987.
 *
 * nolist ag_poly_gap 
 */
#define TRUE 1
#define FALSE 0
#define	MAXOUT	200

void /*FUNCTION*/ ag_poly_gap(xpoly, ypoly, np, iconcv, iconvx, xout, yout, 
	 nout)
DOUBLE xpoly[], ypoly[];
INT np, iconcv, iconvx;
DOUBLE xout[], yout[];
INT *nout;
{
INT i, jend, jp, nc, ncheck, nseg;
DOUBLE _f0, rseg, rtest, rx, seg1, seg2, x1, x2, x3, x4, 
	 xc, xg1, xg2, y1, y2, y3, y4, yc, yg1, yg2;
void ag_ext_ag_intersxn();
static DOUBLE tolrnc = 1.0e-3,TWO = 2;
static INT debug = FALSE;


	/*     Routine to find the convex polygon beginning at point nstart.
	 */

	/*     Check to see if input points are the same.
	 */
	rtest = hypot(xpoly[iconcv - 1] - xpoly[iconvx - 1], 
	              ypoly[iconcv - 1] - ypoly[iconvx - 1] );
	if( rtest < tolrnc )
		goto L_150;
	x1 = xout[*nout - 2];
	y1 = yout[*nout - 2];
	x2 = xout[*nout - 1];
	y2 = yout[*nout - 1];
	x3 = xpoly[iconvx - 1];
	y3 = ypoly[iconvx - 1];

	/*     Search ahead through polygon looking for next point. 
	 */
	jp = iconvx;
	for( i = 1; i <= np; i++ ){
		jp += 1;
		if( jp > np )
			jp = 1;
		if( xpoly[jp - 1] == x3 && ypoly[jp - 1] == y3 )
			goto L_30;
		x4 = xpoly[jp - 1];
		y4 = ypoly[jp - 1];
		goto L_35;
L_30:
		;
		}

	/*     Find intersection of end segments with the gap segments. 
	 *
	 *           x4,y4 o                              o x1,y1
	 *                  *                           *
	 *                   *                        * 
	 *                    *                     *
	 *               x3,y3 o                  o x2,y2
	 *                    * \               /
	 *                   *   \            /  *  
	 *                   *    \         /
	 *                     *   \      /   * 
	 *                         *o * *o *
	 *
	 *
	 */
L_35:
	jp = iconcv;
	ncheck = iconvx - iconcv;
	if( ncheck < 0 )
		ncheck += np;
	ncheck += 1;
	seg1 = 0.0;
	seg2 = 0.0;
	xg1 = xpoly[jp - 1];
	yg1 = ypoly[jp - 1];
	for( i = 1; i <= ncheck; i++ ){
		jp += 1;
		if( jp > np )
			jp = 1;

		/*     Ignore duplicate points  
		 */
		if( xpoly[jp - 1] == xg1 && ypoly[jp - 1] == yg1 )
			goto L_100;
		xg2 = xpoly[jp - 1];
		yg2 = ypoly[jp - 1];
		if( i == 1 )
			goto L_40;

		/*     Locate intersection with 1st end segment.
		 */
		ag_ext_ag_intersxn( x1, y1, x2, y2, xg1, yg1, xg2, yg2, 1.5, 
		 &nc, &xc, &yc, &nseg );
		if( (nc == 1 && nseg == 2) && seg1 == 0.0 ){
			*nout += 1;
			xout[*nout - 1] = xc;
			yout[*nout - 1] = yc;
			jend = 1;
			rx = hypot(xc - xg1,yc - yg1);
			rseg = hypot(xg2 - xg1, yg2 - yg1 );
			if( rseg == 0.0 ){
				goto L_200;
				}
			seg1 = i - 1 + rx/rseg;
			if( debug == TRUE )
				{
				printf("0*** DIAGNOSTIC ***ag_poly_gap  -End segment %2d intersects gap segment %2d\n", 
				 jend, jp );
				}
			if( *nout >= MAXOUT )
				goto L_180;
			}

		/*     Locate intersection with 2nd end segment.
		 */
L_40:
		ag_ext_ag_intersxn( xg1, yg1, xg2, yg2, x3, y3, x4, y4, 1.5, 
		 &nc, &xc, &yc, &nseg );
		if( (nc == 1 && nseg == 1) && seg2 == 0.0 ){
			*nout += 1;
			if( *nout > MAXOUT )
				goto L_180;
			xout[*nout - 1] = xc;
			yout[*nout - 1] = yc;
			jend = 2;
			rx = hypot(xc - xg1, yc - yg1 );
			rseg = hypot(xg2 - xg1, yg2 - yg1 );
			if( rseg == 0.0 ){
				goto L_200;
				}
			seg1 = i - 1 + rx/rseg;
			seg2 = i - 1 + rx/rseg;
			if( debug == TRUE )
				{
				printf("0*** DIAGNOSTIC ***ag_poly_gap  -End segment %2d intersects gap segment %2d\n", 
				 jend, jp );
				}
			}
L_50:
		xg1 = xpoly[jp - 1];
		yg1 = ypoly[jp - 1];
L_100:
		;
		}

	/*     The second end segment does not intersect any of the segments
	 *     in the gap.  An example of this situation is shown below:
	 *
	 *                                    o x1,y1
	 *     x4,y4    x3,y3               *            In this situation the  
	 *      o * * * o                o x2,y2     largest convex polygon will
	 *               *             /  *          be found by keeping the 1st
	 *                *          /    *          intersection point and 
	 *                 *       /     *           treating the point x3,y3 as
	 *                  * * * * * *              a new concave point.
	 */
	if( seg2 >= seg1 || seg2 == 0.0 )
		goto L_150;

	/*     Check for the intersection of the line segments in the gap
	 *
	 *     x4,y4 o                  o x1,y1 
	 *             *              * 
	 *         x3,y3 o          o x2,y2 
	 *                 \     /  
	 *                   \ /
	 *                    xc,yc 
	 *
	 */
	ag_ext_ag_intersxn( x1, y1, x2, y2, x3, y3, x4, y4, 1.5, 
	 &nc, &xc, &yc, &nseg );

	/*     Remove intersection points found previously. 
	 */
	*nout -= 2;
	if( nc == 0 || nseg == 2 )
		goto L_150;
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_gap  -End segments intersected.\n" );
		}
	*nout += 1;
	xout[*nout - 1] = xc;
	yout[*nout - 1] = yc;
L_150:
	rtest = hypot(xout[*nout - 1] - xpoly[iconvx - 1], 
	              yout[*nout - 1] - ypoly[iconvx - 1]);
	if( rtest <= tolrnc )
		goto L_160;
	*nout += 1;
	xout[*nout - 1] = xpoly[iconvx - 1];
	yout[*nout - 1] = ypoly[iconvx - 1];
L_160:
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_gap -Set point %5d to (%7.2f,%7.2f)\n", 
		 *nout, xpoly[*nout - 1], ypoly[*nout - 1] );
		}
	goto L_200;
L_180:
	printf("ag_poly_gap - too many points\n" );
L_200:
	return;
} /* end of function */
