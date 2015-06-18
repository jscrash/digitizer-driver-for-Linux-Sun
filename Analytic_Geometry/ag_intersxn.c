/* DEC/CMS REPLACEMENT HISTORY, Element AG_INTERSXN.C */
/* *2    18-MAR-1991 18:06:34 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:45:49 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_INTERSXN.C */
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
/* list  ag_intersxn 
 *
 *     **Find intersection point between two line segments **
 *
 *     Calling Sequence:
 *
 *     ag_intersxn(x1,y1,x2,y2,xx1,yy1,xx2,yy2,&xc,&yc,&ncross,&isense) 
 *
 *     Arguments:
 *
 *     Name            Input/  Type     Description 
 *                     output
 *
 *     x1,y1,x2,y2       I    DOUBLE    Coordinates for endpoints of the
 *                                      first line segment. 
 *
 *     xx1,yy1,xx2,yy2   I    DOUBLE    Coordinates for endpoints of the
 *                                      second line segment.
 *
 *     *xc               O    DOUBLE    X coordinate of intersection.
 *
 *     *yc               O    DOUBLE    Y coordinate of intersection.
 *
 *     *ncross           O      INT     Number of intersections (1 or 0)
 *
 *     *isense           O      INT     Sense of the intersection (1,-1)
 *
 *     Comments:
 *
 *     This subroutine locates the intersection of the line segment 
 *     defined by the points (x1,y1),(x2,y2) with the segment defined
 *     by the points (xx1,yy1),(xx2,yy2).  The routine returns the x and
 *     y coordinates of the intersection point if one exists.  The  
 *     returned variable ncross is 1 if the line segments intersected,  
 *     zero if they did not.  The sense of line xx,yy with respect  
 *     to line x,y is also returned;  isense=1 if the sense is  
 *     counter-clockwise, isense=-1 if sense is clockwise.  
 *
 *     Revision History:
 *
 *     Written by Davis Swan in July, 1986.  Tolerance added to handle  
 *     "near" vertical slopes in December, 1986.  Slopes and intercepts 
 *     changed to DOUBLE precision in August, 1987. 
 *
 * nolist ag_intersxn
 */
#define TRUE 1
#define FALSE 0

void /*FUNCTION*/ ag_intersxn(x1, y1, x2, y2, xx1, yy1, xx2, yy2, xc, yc, 
	 ncross, isense)
DOUBLE x1, y1, x2, y2, xx1, yy1, xx2, yy2, *xc, *yc;
INT *ncross, *isense;
{
DOUBLE angx, angxx, diff, r, xmax, xmin, ymax, ymin;
DOUBLE bx, bxx, dx, dxx, dy, dyy, mx, mxx, testm, testx, testxx, xxc, 
	 yx, yxx;
void ag_polar_coord();
static DOUBLE pi = 3.14159;
static DOUBLE tolrnc = 1.0e-5;
static INT debug = FALSE;


	/*     Calculate x and y maximums and minimums for this intersection.
	 *     Add constants to allow for precision errors. 
	 */
	xmax = MIN( MAX( x1, x2 ), MAX( xx1, xx2 ) ) + tolrnc;
	xmin = MAX( MIN( x1, x2 ), MIN( xx1, xx2 ) ) - tolrnc;
	ymax = MIN( MAX( y1, y2 ), MAX( yy1, yy2 ) ) + tolrnc;
	ymin = MAX( MIN( y1, y2 ), MIN( yy1, yy2 ) ) - tolrnc;
	*ncross = 0;
	*isense = 0;
	if( xmax < xmin || ymax < ymin )
		goto L_200;
	testx = ABS( x1 - x2 );
	testxx = ABS( xx1 - xx2 );
	if( testx < tolrnc ){

		/*        Line x,y is vertical.  Lines will not cross if line xx,yy 
		 *        is also vertical. 
		 */
		if( testxx < tolrnc )
			goto L_200;
		dxx = xx2 - xx1;
		dyy = yy2 - yy1;
		mxx = dyy/dxx;
		bxx = yy2 - mxx*xx2;
		xxc = (x1 + x2)/2.0e0;
		*xc = xxc;
		*yc = mxx*xxc + bxx;
		}
	else{

		/*     Calculate slope and intercept of line x,y.
		 */
		dx = x2 - x1;
		dy = y2 - y1;
		mx = dy/dx;
		bx = y2 - mx*x2;
		if( testxx < tolrnc ){

			/*           Line xx,yy is vertical. Calculate intersection point
			 */
			xxc = (xx1 + xx2)/2.0;
			*xc = xxc;
			*yc = mx*xxc + bx;
			}
		else{

			/*           Neither line is vertical. Calculate intersection point.
			 */
			dxx = xx2 - xx1;
			dyy = yy2 - yy1;
			mxx = dyy/dxx;

			/*           If lines are parallel there is no intersection 
			 */
			testm = ABS( mx - mxx );
			if( testm < tolrnc )
				goto L_200;
			bxx = yy2 - mxx*xx2;
			xxc = -(bxx - bx)/(mxx - mx);

			/*           Use both line equations to get most accurate results
			 */
			yx = mx*xxc + bx;
			yxx = mxx*xxc + bxx;
			*xc = xxc;
			*yc = (yx + yxx)/2.0e0;
			}
		}

	/*     Check if intersection is within the limits of the input segments,
	 */
	if( ((*xc > xmax || *xc < xmin) || *yc > ymax) || *yc < ymin )
		goto L_200;
	*ncross = 1;

	/*     Calculate the sense of the line xx,yy with respect to the line
	 *     x,y.  If the sense is counter-clockwise isense will be set to 1. 
	 *     if it is clockwise sense will be set to -1.  
	 */
	ag_polar_coord( x2, y2, x1, y1, &angx, &r );
	ag_polar_coord( xx2, yy2, xx1, yy1, &angxx, &r );
	diff = angxx - angx;
	if( diff > pi )
		diff += -2.0*pi;
	if( diff < -pi )
		diff += 2.0*pi;
	*isense = 1;
	if( diff < 0.0 )
		*isense = -1;
L_100:
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_intersxn -Intersection at (%7.2f,%7.2f), isense=%2d\n", 
		 *xc, *yc, *isense );
		}
L_200:
	return;
} /* end of function */
