/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_FLLR.C */
/* *2    18-MAR-1991 18:07:02 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:47:32 JULIAN "Lease Ma" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_FLLR.C */
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
/*Translated by FOR_C++, v0.2, on 09/27/90 at 13:47:06 */
/*FOR_C++ Options SET: cio op=a c */
/* list ag_poly_filler  
 *
 *     *** Fill in polygon with line strokes to emulate colour fill ***
 *
 *     Calling sequence:
 *
 *     ag_poly_filler(xpoly,ypoly,np,degrees,rdist,xout,yout,&nout)  
 *
 *     Arguments:
 *
 *     Name    Input/     Type       Description
 *             output
 *
 *     xpoly     I     double[]      X coordinates for polygon. 
 *
 *     ypoly     I     double[]      Y coordinates for polygon. 
 *
 *     np        I      INT          Number of points in polygon.
 *
 *     degrees   I     DOUBLE        Rotation angle for filler strokes.
 *
 *     rdist     I     DOUBLE        Distance between filler strokes.
 *
 *     xout      O    double[]       X coordinates of output polygon.
 *
 *     yout      O    double[]       Y coordinates of output polygon.
 *
 *     *nout     O      INT          Number of points in output polygon.
 *
 *     Comments:
 *
 *     This routine will emulate panel fill.  It is useful for building
 *     fonts or for colour fill on pen plotters.
 *
 * nolist ag_poly_filler
 *
 */
#include "esi_c_lib.h"
#include "esi_math.h"
#define TRUE 1
#define FALSE 0
#define	ISLISZ	2500

void /*FUNCTION*/ ag_poly_filler( xpoly, ypoly, np, degrees, rdist, 
                               xout, yout, nout)
INT np;
DOUBLE xpoly[], ypoly[], degrees, rdist;
DOUBLE xout[], yout[];
INT *nout;
{
INT  i, np1, islice, j, k, kpts, nplacx, nslice, nx, qrevrs = FALSE;
static INT min_intrvs = 0;
CHAR dummy[10];
DOUBLE costh, radians, exact_ymax, exact_ymin, sinth, xintcp, xslice[ISLISZ], 
	 ypos, yend, ystart, x1, x2, y1, y2;
void ag_auto_scaled();


	/*     Routine does panel fill emulation for devices not able to.
	 *
	 *     np   -> number of points defining closed polygon
	 *     xpoly,ypoly  -> real points in inches of the perimeter
	 *     degrees -> angle for hatch pattern
	 *     rdist  -> distance separating the lines
	 *
	 *     errors
	 *     1) ran out of room in the work array
	 *
	 *     ------------------------------------------------------------------------
	 */
	*nout = 0;
	radians = degrees*0.017453293;
	sinth = sin( radians );
	costh = cos( radians );
	exact_ymin = 10.0e10;
	exact_ymax = -exact_ymin;

	/*     Rotate the polygon vertices by radians about the origin and determine
	 *     the min and max values. NOTE: input data is DESTROYED.
	 */
	for( i = 0; i < np; i++ )
	{
		ypos = -xpoly[i]*sinth + ypoly[i]*costh;
		xpoly[i] = xpoly[i]*costh + ypoly[i]*sinth;
		ypoly[i] = ypos;
		if( ypoly[i] < exact_ymin )
			exact_ymin = ypoly[i];
		if( ypoly[i] > exact_ymax )
			exact_ymax = ypoly[i];
	}

	/*     Find ystart, starting y, which is the first value of y over exact_ymin,
	 *     such that the family of rulings has one which passes through the origin.
	 *     This guarantees that patches panels will have matching rulings.
	 */
	ag_auto_scaled( exact_ymax, exact_ymin, &yend, &ystart, &rdist, min_intrvs, &nx, &nplacx );

	/*     Scan up the panel drawing horizontal slices. This makes the mathematics
	 *     of calculating intersections between the current slice and the defining
	 *     segments easier. After the intersection calculations are completed, the
	 *     slice get rotated back to the correct position.
	 */
	nslice = nx - 2;
	qrevrs = FALSE;
	for( islice = 0; islice < nslice; islice++ )
	{
		ypos = ystart + (islice+1)* rdist;

		/*       Find the intersection between lines on the panel perimeter and the
		 *       current slice. Sort these points in ascending order.
		 */
		kpts = 0;
                x1 = xpoly[np -1];
                y1 = ypoly[np -1];
		for( i = 0; i < np; i++ )
		{
                        x2 = xpoly[i];
                        y2 = ypoly[i];
			if( (y1 < ypos && y2 >= ypos) || (y1 >=  ypos && y2 < ypos) )
			{

				/*           Calculate the intersection of two lines defined by y=ypos and two
				 *           points (xpoly(i),ypoly(i)) and (xpoly(i+1),ypoly(i+1))
				 */
				if( ABS( y1 - y2) < 0.0001 )
					xintcp = (x1 + x2)*0.5;
				else
					xintcp = x1 - (y1 - ypos)*(x1 - x2)/(y1 -y2);

				/*           Use insertion sort to place the x coordinate of the point of
				 *           intersection in ascending order in xslice. The y values are all
				 *           on the current slice(by definition) so we don't have to store
				 *           them.
				 */
				kpts += 1;
				if( kpts > ISLISZ ) return;
				for( k = kpts - 1; k >= 0; k-- )
				{
                                        if( k == 0 ) break;
					if( xslice[k - 1] >= xintcp ) break ;
					xslice[k] = xslice[k - 1];
				}
				xslice[k] = xintcp;
			}
                        x1 = x2;
                        y1 = y2;

		}
		/*
		 *       We have all the points we need to draw this slice.
		 */
		for( i = 0; i < kpts; i++ )
		{
			/*         if(qrevrs == TRUE) then
			 *           j=kpts-i+1
			 *         else */
			j = i;
			/*         endif */
			*nout += 1;
			xout[*nout - 1] = xslice[j]*costh - ypos*sinth;
			yout[*nout - 1] = xslice[j]*sinth + ypos*costh;
		}
		qrevrs = !qrevrs;
	}

	return;
} /* end of function */
