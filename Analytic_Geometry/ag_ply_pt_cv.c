/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PT_CV.C */
/* *2    18-MAR-1991 18:07:28 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:49:25 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PT_CV.C */
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
/* list ag_poly_pt_cncv  
 *
 *     *** Find next "concave" point along a polygon ***
 *
 *     Calling sequence:
 *
 *     ag_poly_pt_cncv(xpoly,ypoly,np,isense,nstart,&nreman,&ndone,
 *     &iconcv,&angle1)
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I     double[np]   X coordinates for polygon. 
 *
 *     ypoly     I     double[np]   Y coordinates for polygon. 
 *
 *     np        I       INT        Number of points in polygon.
 *
 *     isense    I       INT        Sense of direction of polygon  
 *                                  (1=counterclockwise, -1=clockwise) 
 *
 *     nstart    I       INT        PoINT at which search is to start. 
 *
 *     *nreman  I,O      INT        Points remaining to be examined.
 *                                  This value will be updated.
 *
 *     *ndone    O       INT        Number of points examined in order 
 *                                  to find next concave point.
 *
 *     *iconcv   O       INT        Next concave point.
 *
 *     *angle1   O      DOUBLE      Angle of line segment before concave
 *                                  point. 
 * nolist ag_poly_pt_cncv
 */
#define TRUE 1
#define FALSE 0

void /*FUNCTION*/ ag_poly_pt_cncv(xpoly, ypoly, np, isense, nstart, nreman, 
	 ndone, iconcv, angle1)
DOUBLE xpoly[], ypoly[];
INT np, isense, nstart, *nreman, *ndone, *iconcv;
DOUBLE *angle1;
{
INT j, jp, nang, ncheck;
static INT debug = FALSE;
DOUBLE angle2, r2, x1, x2, y1, y2;
DOUBLE ag_arc_angle();
void ag_polar_coord();


	/*     This routine locates the next concave point along the polygon.
	 *
	 */
	*iconcv = 0;
	if( *nreman == 1 )
		goto L_200;
	nang = 0;
	jp = nstart;
	x1 = xpoly[jp - 1];
	y1 = ypoly[jp - 1];
	ncheck = *nreman;
	*ndone = 0;
	for( j = 1; j <= ncheck; j++ ){
		jp += 1;
		if( jp > np )
			jp = 1;
		x2 = xpoly[jp - 1];
		y2 = ypoly[jp - 1];

		/*     Ignore duplicate points. 
		 */
		if( x1 == x2 && y1 == y2 )
			goto L_40;
		nang += 1;
		ag_polar_coord( x2, y2, x1, y1, &angle2, &r2 );
		if( nang == 1 )
			goto L_20;
		if( ag_arc_angle( *angle1, angle2 )*isense < 0 )
			goto L_100;
L_20:
		x1 = x2;
		y1 = y2;
		*angle1 = angle2;
L_40:
		*ndone += 1;
		}

	/*     No concave points found  
	 */
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_pt_cncv -No concave point found.\n" );
		}
	*nreman -= *ndone;
	goto L_200;
L_100:
	*iconcv = jp - 1;
	if( *iconcv < 1 )
		*iconcv = np;
	*nreman -= *ndone;
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_pt_cncv -Concave point %5d found after %6d searches.\n", 
		 *iconcv, *ndone );
		}
L_200:
	return;
} /* end of function */
