/* DEC/CMS REPLACEMENT HISTORY, Element AG_EXT_INTSXN.C */
/* *2    18-MAR-1991 18:06:28 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:45:22 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_EXT_INTSXN.C */
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
/* list ag_ext_ag_intersxn  
 *
 *     **Find intersection point between two extended line segments **  
 *
 *     Calling Sequence:
 *
 *     ag_ext_ag_intersxn(x1,y1,x2,y2,xx1,yy1,xx2,yy2,extfct,&xc,&yc,&ncross,&nseg)
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
 *     extfct            I    DOUBLE    Extension factor.
 *
 *     *xc               O    DOUBLE    X coordinate of intersection.
 *
 *     *yc               O    DOUBLE    Y coordinate of intersection.
 *
 *     *ncross           O    Integer   Number of intersections (1 or 0)
 *
 *     *nseg             O    Integer   A flag which indicates if the
 *                                      intersection found is on one
 *                                      of the original line segments.  
 *     Comments:
 *
 *     This routine extends the first line segment forward, and the 
 *     second line segment backward to find any intersection
 *     between the segments.  If an intersection is found the value of  
 *     nseg indicates where the intersection occured.  NSEG=0 indicates 
 *     that the intersection was between the endpoints of the segments. 
 *     NSEG=1 or 2 indicates the intersection was on the first or
 *     second input segments, and NSEG=3 indicates that the original
 *     (unextended) segments intersected.
 *
 * nolist ag_ext_ag_intersxn
 */
#define TRUE 1
#define FALSE 0
void /*FUNCTION*/ ag_ext_ag_intersxn(x1, y1, x2, y2, xx1, yy1, xx2, yy2, extfct, 
	 ncross, xc, yc, nseg)
DOUBLE x1, y1, x2, y2, xx1, yy1, xx2, yy2, extfct;
INT *ncross;
DOUBLE *xc, *yc;
INT *nseg;
{
INT isense;
DOUBLE angle, rext, rsep, x2ext, xmax, xmin, xx1ext, y2ext, ymax, ymin, 
	 yy1ext;
DOUBLE dr, dx, dxy, dy;
void ag_intersxn(), ag_polar_coord();
static DOUBLE tolrnc = 2.0e-3;
static INT debug = FALSE;


	/*     Find intersection between extensions of input line segments. 
	 */
	*ncross = 0;
	if( x2 == xx1 && y2 == yy1 ){
		*ncross = 1;
		*nseg = 3;
		*xc = x2;
		*yc = y2;
		goto L_150;
		}

	/*     Calculate distance between points
	 */
	ag_polar_coord( xx1, yy1, x2, y2, &angle, &rsep );

	/*     The extention distance is the separation multplied by the factor.
	 */
	rext = rsep*extfct;
	if( rext == 0.0 )
		rext = rsep;

	/*     Extend line forward  
	 */
	dx = x2 - x1;
	dy = y2 - y1;
	dxy = sqrt( dx * dx + dy * dy );
	if( dxy <= 0.0e0 )
		goto L_150;
	dr = rext/dxy;
	x2ext = x2 + dx*dr;
	y2ext = y2 + dy*dr;

	/*     Extend line backward 
	 */
	dx = xx2 - xx1;
	dy = yy2 - yy1;
	dxy = sqrt( dx * dx + dy * dy );
	if( dxy <= 0.0e0 )
		goto L_150;
	dr = rext/dxy;
	xx1ext = xx1 - dx*dr;
	yy1ext = yy1 - dy*dr;

	/*     Check for intersection
	 */
	*ncross = 0;
	*nseg = 0;
	ag_intersxn( x1, y1, x2ext, y2ext, xx1ext, yy1ext, xx2, yy2, xc, 
	 yc, ncross, &isense );
	if( *ncross == 0 )
		goto L_150;

	/*     Determine whether or not this intersection is on one of the  
	 *     original line segments.  
	 */
	xmax = MAX( x1, x2 ) + tolrnc;
	xmin = MIN( x1, x2 ) - tolrnc;
	ymax = MAX( y1, y2 ) + tolrnc;
	ymin = MIN( y1, y2 ) - tolrnc;
	if( ((*xc >= xmin && *xc <= xmax) && *yc >= ymin) && *yc <= ymax )
		*nseg += 1;
	xmax = MAX( xx1, xx2 ) + tolrnc;
	xmin = MIN( xx1, xx2 ) - tolrnc;
	ymax = MAX( yy1, yy2 ) + tolrnc;
	ymin = MIN( yy1, yy2 ) - tolrnc;
	if( ((*xc >= xmin && *xc <= xmax) && *yc >= ymin) && *yc <= ymax )
		*nseg += 2;
L_150:
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_ext_ag_intersxn -xc,yc,ncross =%7.2f%7.2f%6d\n", 
		 *xc, *yc, *ncross );
		}
	return;
} /* end of function */
