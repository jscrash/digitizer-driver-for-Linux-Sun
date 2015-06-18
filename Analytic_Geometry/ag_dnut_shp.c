/* DEC/CMS REPLACEMENT HISTORY, Element AG_DNUT_SHP.C */
/* *2    18-MAR-1991 18:06:22 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:44:58 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_DNUT_SHP.C */
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
/*Translated by FOR_C++, v0.2, on 09/18/90 at 15:13:35 */
/*FOR_C++ Options SET: cio op=a c */
#include "esi_c_lib.h"
#include "esi_math.h"
/*
 * list ag_donut_shape
 *
 *     *** Set up a single polygon from inside/outside polygons ***
 *
 *     Calling sequence:
 *
 *     ag_donut_shape(xout,yout,nout,xin,yin,nin,xbuf,ybuf,&itotal)
 *
 *     Arguments:
 *
 *     Name     Input/  Type       Description
 *              output
 *
 *     xout,yout  I    double[]      Coordinates for outside polygon.
 *
 *     nout       I      INT         Number of points for outside polygon.
 *
 *     xin,yin    I    double[]      Coordinates for inside polygon.
 *
 *     nin       I,O     INT         Number of points for inside polygon.
 *
 *     xbuf,ybuf  O    double[]      Coordinates for combined polygon.
 *
 *     *itotal    O      INT         Number of points for combined polygon.
 *
 *
 *     Comments:
 *
 *     This subroutine "seams" the dough-nut shaped land.
 *
 *     **************                    **************
 *     *            *                    *            *
 *     *   ******   *       Becomes =>   *   ******   *
 *     *   *    *   *****                *****    *   *****
 *     *   *    *       *                *****    *       *
 *     *   ******       *                *   ******       *
 *     *                *                *                *
 *     *                *                *                *
 *     ******************                ******************
 */
void /*FUNCTION*/ ag_donut_shape(xout, yout, nout, xin, yin, nin, xbuf, ybuf, 
	 itotal)
DOUBLE xout[], yout[];
INT nout;
DOUBLE xin[], yin[];
INT nin;
DOUBLE xbuf[], ybuf[];
INT *itotal;
{
INT i, j, jsave, jseam, k, nshift;
DOUBLE dist, dmin;
void ag_shift_double();


	dmin = 10.0e15;

	/*     Find the closest points and save the counters.
	 */
	for( i = 1; i <= nout; i++ )
                {

		for( j = 1; j <= nin; j++ )
                        {
			dist = hypot(xin[j-1] - xout[i-1], yin[j-1] - yout[i-1]);
			if( dist < dmin )
                                {
				jseam = i;
				jsave = j;
				dmin = dist;
				}
			}

		}

	/*     Call shift routine to shift the "in's" to proper positions.
	 */
	nshift = (jsave - nin) - 1;
	ag_shift_double( xin, nin, nshift );
	ag_shift_double( yin, nin, nshift );

	/*     Get both arrays and put them into one buffer with the "out's" first.
	 *     The "out's" ends at where the "seams" begins;then the "re-arranged
	 *     in's " is added to the buffer and then the remainder "out's".
	 */
	for( i = 0; i < jseam; i++ ){
		xbuf[i] = xout[i];
		ybuf[i] = yout[i];
		}

	*itotal = jseam;
	for( j = 0; j < nin; j++ ){
		*itotal += 1;
		xbuf[*itotal - 1] = xin[j];
		ybuf[*itotal - 1] = yin[j];
		}

	/*     Make sure the last pair of "in's is equal the first pair.
	 */
	if( xbuf[*itotal - 1] == xin[0] && ybuf[*itotal - 1] == yin[0] )
		goto L_150;
	*itotal += 1;
	xbuf[*itotal - 1] = xin[0];
	ybuf[*itotal - 1] = yin[0];
	nin += 1;

	/*     Get the remainder of "out's array.
	 */
L_150:
	for( k = jseam; k <= nout; k++ )
                {
		*itotal += 1;
		xbuf[*itotal - 1] = xout[k-1];
		ybuf[*itotal - 1] = yout[k-1];
		}

	return;
} /* end of function */
