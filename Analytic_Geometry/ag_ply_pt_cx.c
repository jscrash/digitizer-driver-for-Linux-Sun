/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PT_CX.C */
/* *2    18-MAR-1991 18:07:35 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:49:52 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PT_CX.C */
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
/* list ag_poly_pt_cnvx  
 *
 *     *** Find next "convex" point along a polygon *** 
 *
 *     Calling sequence:
 *
 *     ag_poly_pt_cnvx(xpoly,ypoly,np,isense,nstart,angle1,&nreman,&ndone,
 *    &iconvx)  
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I     double[np]    X coordinates for polygon. 
 *
 *     ypoly     I     double[np]    Y coordinates for polygon. 
 *
 *     np        I        INT        Number of points in polygon.
 *
 *     isense    I        INT        Sense of direction of polygon  
 *                                   (1=counterclockwise, -1=clockwise) 
 *
 *     nstart    I        INT        PoINT at which search is to start. 
 *
 *     angle1    I      DOUBLE       Base angle to use in determining
 *                                   next convex point. 
 *
 *     *nreman  I,O       INT        Points remaining to be examined.
 *                                   This value will be updated.
 *
 *     *ndone    O        INT        Number of points examined in order 
 *                                   to find next convex point. 
 *
 *     *iconvx   O        INT        Next convex point. 
 *
 *     Comments:
 *
 *     This routine finds the next convex point a polygon.  An initial  
 *     point is located by checking the relative angles of points after 
 *     nstart.  Having found a convex value the routine then searches
 *     ahead looking for "concave" points that may be closer to the 
 *     original point than the initial convex point.
 *
 * nolist ag_poly_pt_cnvx
 */
#define TRUE 1 
#define FALSE 0
void /*FUNCTION*/ ag_poly_pt_cnvx(xpoly, ypoly, np, isense, nstart, angle1, 
	 nreman, ndone, iconvx)
DOUBLE xpoly[], ypoly[];
INT np, isense, nstart;
DOUBLE angle1;
INT *nreman, *ndone, *iconvx;
{
INT iconcv, j,  jconcv, jp, ndone2, 
	 nrem1, nrem2, nstrt2, nsum2;
DOUBLE angc, angle2, angle3, anglst, angtst, angxx, asum, r2, r3, rmin, 
	 x1, x2, x3, y1, y2, y3;
DOUBLE ag_arc_angle();
void ag_poly_pt_cncv(), ag_polar_coord();
static INT debug = FALSE;
static DOUBLE pi = 3.14159;
static DOUBLE tolrnc = 1.0e-5;

        /*
	 *     Find the nearest convex point along the polygon. 
	 *
	 */
	rmin = 1.0e15;
	*iconvx = 0;
	jp = nstart;
	nrem1 = *nreman;
	x1 = xpoly[nstart - 1];
	y1 = ypoly[nstart - 1];
	*ndone = 0;
	asum = 0.0;
	anglst = angle1;
	for( j = 1; j <= nrem1; j++ )
	{
		jp += 1;
		if( jp > np )
			jp = 1;
		x2 = xpoly[jp - 1];
		y2 = ypoly[jp - 1];
		*ndone += 1;

		/*     Ignore duplicate points  
		 */
		if( x2 == x1 && y2 == y1 )
			goto L_50;

		/*     Calculate current angle and distance 
		 */
		ag_polar_coord( x2, y2, x1, y1, &angle2, &r2 );

		/*     Sum angles to account for "curls" in the polygon.
		 */
		asum += ag_arc_angle( anglst, angle2 )*isense;
		if( asum >= -tolrnc )
			goto L_100;
		anglst = angle2;
L_50:
		;
		}

	/*     No convex point found.
	 */
	*nreman -= *ndone;
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_pt_cnvx -no convex point found\n" );
		}
	goto L_200;

	/*     A convex point has been found.  Check all concave points 
	 *     further along the polygon to make sure that there isn't one  
	 *     closer to the original point.
	 *
	 *              *  1                  In the plot to the left
	 *           *       *                the original concave point is 
	 *          *        2  * *    *      labelled 0.  The initial convex
	 *           *                        point located is labelled 1.  
	 *              0                     By searching ahead looking for
	 *              *                     concave points we can find point  
	 *              *                     2, which will produce a "larger"  
	 *              *                     convex polygon than point 1.  
	 */
L_100:
	*iconvx = jp;
	rmin = r2;

	/*     The check counters are as follows:
	 *
	 *     ndone - number of points examined to find the first convex point 
	 *     nrem2 - number of points remaining to find next concave point
	 */
	*nreman -= *ndone;
	nstrt2 = jp;
	nrem2 = *nreman;
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_pt_cnvx -Initial convex point %5d found after %6d searches\n", 
		 *iconvx, *ndone );
		}
	nsum2 = *ndone;
	for( j = 1; j <= np; j++ )
		{
		ag_poly_pt_cncv( xpoly, ypoly, np, isense, nstrt2, &nrem2, &ndone2, 
		 &iconcv, &angc );
		if( iconcv == 0 )
			goto L_200;
		nsum2 += ndone2;

		/*     Concave point found  
		 */
		x2 = xpoly[iconcv - 1];
		y2 = ypoly[iconcv - 1];
		if( x2 == x1 && y2 == y1 )
			goto L_120;
		ag_polar_coord( x2, y2, x1, y1, &angle2, &r2 );
		angtst = ag_arc_angle( angle1, angle2 )*isense;

		/*     Check that angle will be convex from the original point. 
		 *     Ignore angles greater than 120 degrees.  
		 *     Note that this routine is very sensitive to the angle limit. 
		 *     Changes to this value will dramatically alter the polygons
		 *     generated by PLCNVX. 
		 */
		if( angtst < -tolrnc || angtst > 5.0*pi/9.0 )
			goto L_120;

		/*     Check to see if this concave point "cuts off" the initial line
		 *     segment.  An example is shown below. 
		 *
		 *                                          o x0,y0 
		 *                                       *  
		 *      x3,y3    x2,y2                * 
		 *       o * * * o-----------------*
		 *                *             o x1,y1 
		 *                 *          * 
		 *                  * * * * * */
		jp = iconcv;
		for( jconcv = 1; jconcv <= np; jconcv++ )
		{
			jp += 1;
			if( jp > np )
				jp = 1;
			if( xpoly[jp - 1] == x2 && ypoly[jp - 1] == y2 )
				goto L_110;
			x3 = xpoly[jp - 1];
			y3 = ypoly[jp - 1];
			goto L_115;
L_110:
			;
			}
L_115:
		ag_polar_coord( x3, y3, x2, y2, &angle3, &r3 );
		angxx = ag_arc_angle( angle2, angle3 )*isense;
		if( angxx < 0.0 )
			goto L_120;

		/*     Check distance
		 */
		if( r2 >= rmin )
			goto L_120;
		rmin = r2;
		*iconvx = iconcv;
		*ndone = nsum2;
		*nreman = nrem2;
		if( debug == TRUE )
			{
			printf("0*** DIAGNOSTIC ***ag_poly_pt_cnvx -Resetting next convex point  to %6d after %6d searches.\n", 
			 *iconvx, *ndone );
			}
L_120:
		if( nrem2 <= 0 )
			goto L_200;
		nstrt2 = iconcv;
		}
L_200:
	return;
} /* end of function */
