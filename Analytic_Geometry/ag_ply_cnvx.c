/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_CNVX.C */
/* *3    21-JUN-1991 16:51:29 JULIAN "(SPR 0) add static" */
/* *2    18-MAR-1991 18:06:54 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:47:06 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_CNVX.C */
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
/* list ag_poly_convex  
 *
 *     *** Find a "convex" polygon within the input irregular polygon ***
 *
 *     Calling sequence:
 *
 *     ag_poly_convex(xpoly,ypoly,np,isense,jpoly,xout,yout,&nout)  
 *
 *     Arguments:
 *
 *     Name    Input/   Type       Description
 *             output
 *
 *     xpoly     I    double[]     X coordinates for polygon. 
 *
 *     ypoly     I    double[]     Y coordinates for polygon. 
 *
 *     np        I      INT        Number of points in polygon.
 *
 *     isense    I      INT        Sense of direction of polygon  
 *                                 (1=counterclockwise, -1=clockwise) 
 *
 *     jpoly     I      INT        Polygon number being asked for.
 *
 *     xout      O    double[]     X coordinates of output polygon.
 *
 *     yout      O    double[]     Y coordinates of output polygon.
 *
 *     *nout     O      INT        Number of points in output polygon.
 *
 *     Comments:
 *
 *     This routine will find convex polygon number jpoly within the
 *     input irregular polygon.  The maximum size of output polygons
 *     is 50 points.
 *
 *     Revision History:
 *
 *     Written by Davis Swan in December, 1986.  Major changes made in  
 *     July, 1987 to correct bugs.  
 *
 * nolist ag_poly_convex
 */
#define TRUE 1
#define FALSE 0
#define	MAXOUT	200
#define	MAXPT	100 

void /*FUNCTION*/ ag_poly_convex(xpoly, ypoly, np, isense, jpoly, xout, yout, 
	 nout)
DOUBLE xpoly[], ypoly[];
INT np, isense, jpoly;
DOUBLE xout[], yout[];
INT *nout;
{
static INT debug = FALSE;
INT _do0, _do1, _do2, _do3, _do4, _do5, _do6, _do7, _do8, i, i_, iconcv, 
	 iconvx, icount, iout[MAXOUT], iseg, iseg_, itest, itest_, itry, 
	 itry_, jadd, jadd_, jcheck, jcheck_, jgap, jgap_, jout, jp, jsense, 
	 jx, jx_,  nadd, nc, ndone, nextpt, ngap, 
	 nsave, nseg, ntest; 
static DOUBLE a, a1, angbas, angnow, atest, r, rtest, x1, x2, x3, x4, xc, 
	 xcjp, y1, y2, y3, y4, yc, ycjp;
DOUBLE ag_arc_angle();
void ag_poly_pt_cncv(), ag_poly_pt_cnvx(), ag_ext_ag_intersxn(), ag_intersxn(), ag_poly_gap(), ag_polar_coord();
static DOUBLE tolrnc = 1.0e-5;
static DOUBLE extfct = 1.5;
static INT npoly,kpoly,nstart,nreman,nrem2,maxply,nconcv,point[MAXPT];
	if( jpoly == 1 ){

		/*        Each concave point generates two convex polygons. 
		 *            * 
		 *            *                   The concave point (2) will generate a 
		 *            1 * * * * * * 2     convex polygon containing the line
		 *                           *    segment (1-2) and another containing  
		 *                            *   the line segment (2-3).
		 *                             3 */
		nreman = np;
		nstart = 1;
		maxply = MAXPT/2;
		nconcv = 0;
		for( i = 1, _do0 = maxply; i <= _do0; i++ ){
			i_ = i - 1;
			ag_poly_pt_cncv( xpoly, ypoly, np, isense, nstart, &nreman, &ndone, 
			 &iconcv, &a );
			if( iconcv == 0 )
				goto L_15;
			nconcv = i;
			point[nconcv - 1] = iconcv;
			nstart = iconcv;
			}
L_15:
		npoly = 2*nconcv;
		if( nconcv == 0 ){
			nconcv = 1;
			npoly = 1;
			point[0] = 1;
			}
		else{
			for( i = 1, _do1 = nconcv; i <= _do1; i++ ){
				i_ = i - 1;
				point[nconcv + i_] = point[i_];
				}
			}
		kpoly = 0;
		}
	for( itry = 1, _do2 = npoly; itry <= _do2; itry++ ){
		itry_ = itry - 1;

		/*     The variable kpoly is a count of the possible convex polygons
		 *     (maximum 2*nconcv) as they are processed.  Duplicates are
		 *     avoided by checking for a zero starting point.
		 */
		*nout = 0;
		kpoly += 1;
		if( kpoly > npoly )
			goto L_200;
		nstart = point[kpoly - 1];
		if( nstart == 0 )
			goto L_150;
		if( kpoly > nconcv )
			goto L_30;

		/*     Start 1st set of polygons before concave points. 
		 */
		jp = nstart;
		for( i = 1, _do3 = np; i <= _do3; i++ ){
			i_ = i - 1;
			jp -= 1;
			if( jp < 1 )
				jp = np;
			if( xpoly[jp - 1] == xpoly[nstart - 1] && ypoly[jp - 1] == 
			 ypoly[nstart - 1] )
				goto L_25;
			nstart = jp;
			goto L_30;
L_25:
			;
			}
L_30:
		nreman = np;
		if( debug == TRUE )
			{
			printf("0*** DIAGNOSTIC ***ag_poly_convex -Starting polygon %4d at point %4d\n", 
			 jpoly, nstart );
			}

		/*     Initialize first point of polygon.
		 */
		*nout = 1;
		xout[0] = xpoly[nstart - 1];
		yout[0] = ypoly[nstart - 1];
		iout[0] = nstart;
		for( iseg = 1; iseg <= 100; iseg++ ){
			iseg_ = iseg - 1;

			/*     Look for next concave point along current polygon
			 */
			ag_poly_pt_cncv( xpoly, ypoly, np, isense, nstart, &nreman, &ndone, 
			 &iconcv, &a1 );
			nadd = ndone;
			jp = nstart;

			/*     Store points up to the next concave point, checking for  
			 *     intersections with the first line segment.
			 */
			for( jadd = 1, _do4 = nadd; jadd <= _do4; jadd++ ){
				jadd_ = jadd - 1;
				jp += 1;
				if( jp > np )
					jp = 1;

				/*     Ignore duplicate points  
				 */
				if( xpoly[jp - 1] == xout[*nout - 1] && ypoly[jp - 1] == 
				 yout[*nout - 1] )
					goto L_50;
				*nout += 1;
				xout[*nout - 1] = xpoly[jp - 1];
				yout[*nout - 1] = ypoly[jp - 1];
				iout[*nout - 1] = jp;

				/*     Calculate base angle using 1st 2 points. 
				 */
				if( *nout == 2 ){
					ag_polar_coord( xout[1], yout[1], xout[0], yout[0], 
					 &angbas, &r );
					goto L_50;
					}

				/*     Check for angle greater than the base angle. 
				 */
				if( xout[*nout - 1] == xout[0] && yout[*nout - 1] == 
				 yout[0] )
					goto L_90;
				ag_polar_coord( xout[*nout - 1], yout[*nout - 1], xout[0], 
				yout[0], &angnow, &r );
				atest = ag_arc_angle( angbas, angnow )*isense;
				if( atest >= -tolrnc )
					goto L_50;

				/*     Find the intersection with first line segment of this polygon
				 */
				ag_ext_ag_intersxn( xout[*nout - 2], yout[*nout - 2], xout[*nout - 1], 
				 yout[*nout - 1], xout[0], yout[0], xout[1], yout[1], 
				 extfct, &nc, &xc, &yc, &nseg );

				/*     This point is behind the baseline.  Check to see if any points
				 *     further along the polygon are in front of the baseline by
				 *     looking for intersections between the baseline and other line
				 *     segments.
				 */
				x1 = xout[0];
				y1 = yout[0];
				x2 = xc;
				y2 = yc;
				icount = 0;
				ntest = nreman + nadd - jadd - 2;
				if( ntest < 2 )
					goto L_45;
				for( itest = 1, _do5 = ntest; itest <= _do5; itest++ ){
					itest_ = itest - 1;
					jp += 1;
					if( jp > np )
						jp -= np;
					if( x2 == xpoly[jp - 1] && y2 == ypoly[jp - 1] )
						goto L_40;
					icount += 1;
					x4 = xpoly[jp - 1];
					y4 = ypoly[jp - 1];
					if( icount >= 2 ){
						if( x3 == x4 && y3 == y4 )
							goto L_40;
						ag_intersxn( x1, y1, x2, y2, x3, y3, x4, 
						 y4, &xcjp, &ycjp, &nc, &jsense );
						if( nc == 1 )
							goto L_150;
						}
					x3 = x4;
					y3 = y4;
L_40:
					;
					}
L_45:
				if( debug == TRUE )
					{
					printf("0*** DIAGNOSTIC ***ag_poly_convex -Intersection with 1st segment\n" );
					}
				xout[*nout - 1] = xc;
				yout[*nout - 1] = yc;
				rtest = hypot(xout[*nout - 1] - xc, yout[*nout - 1] - yc);
				if( rtest > tolrnc && nseg == 1 )
					iout[*nout - 1] = 0;
				rtest = hypot(xout[0] - xc, yout[0] - yc);
				if( rtest <= tolrnc )
					goto L_90;
				xout[0] = xc;
				yout[0] = yc;
				if( nseg == 2 )
					iout[0] = 0;
				goto L_90;
L_50:
				;
				}

			/*     Look for next convex point to continue polygon.  
			 */
			if( iconcv == 0 )
				goto L_90;
			for( jx = 1; jx <= 100; jx++ ){
				jx_ = jx - 1;
				nstart = iconcv;
				if( nreman <= 0 )
					goto L_150;
				nrem2 = nreman;
				ag_poly_pt_cnvx( xpoly, ypoly, np, isense, nstart, a1, &nrem2, 
				 &ndone, &iconvx );
				if( iconvx == 0 )
					goto L_150;
				if( ndone == 1 )
					goto L_80;
				nreman = nrem2;
				nstart = iconvx;

				/*     Get convex points from gap
				 */
				nsave = *nout;
				ag_poly_gap( xpoly, ypoly, np, iconcv, iconvx, xout, yout, 
				 nout );
				ngap = *nout - nsave;
				if( ngap == 0 )
					goto L_80;
				*nout = nsave;
				for( jgap = 1, _do6 = ngap; jgap <= _do6; jgap++ ){
					jgap_ = jgap - 1;
					jout = nsave + jgap;

					/*     Ignore duplicate points  
					 */
					if( xout[jout - 1] == xout[*nout - 1] && yout[jout - 1] == 
					 yout[*nout - 1] )
						goto L_70;
					*nout += 1;
					xout[*nout - 1] = xout[jout - 1];
					yout[*nout - 1] = yout[jout - 1];
					iout[*nout - 1] = 0;

					/*     Check for angle greater than the base angle. 
					 */
					if( xout[*nout - 1] == xout[0] && yout[*nout - 1] == 
					 yout[0] )
						goto L_90;
					ag_polar_coord( xout[*nout - 1], yout[*nout - 1], xout[0], 
					yout[0], &angnow, &r );
					atest = ag_arc_angle( angbas, angnow )*isense;
					if( atest >= -tolrnc )
						goto L_70;
					ag_ext_ag_intersxn( xout[*nout - 2], yout[*nout - 2], xout[*nout - 1], 
					 yout[*nout - 1], xout[0], yout[0], xout[1], 
					 yout[1], extfct, &nc, &xc, &yc, &nseg );
					if( nc == 0 || nseg == 0 )
						goto L_70;
					if( debug == TRUE )
						{
						printf("0*** DIAGNOSTIC ***ag_poly_convex -Intersection with 1st segment\n" );
						}
					xout[*nout - 1] = xc;
					yout[*nout - 1] = yc;
					rtest = hypot(xout[*nout - 1] - xc, yout[*nout - 1] - yc);
					if( rtest > tolrnc && nseg == 1 )
						iout[*nout - 1] = 0;
					rtest = hypot(xout[0] - xc, yout[0] - yc);
					if( rtest <= tolrnc )
						goto L_90;
					xout[0] = xc;
					yout[0] = yc;
					if( nseg == 2 )
						iout[0] = 0;
					goto L_90;
L_70:
					;
					}
				ag_polar_coord( xout[*nout - 1], yout[*nout - 1], xout[*nout - 2], 
				yout[*nout - 2], &a1, &r );
				if( nreman <= 0 )
					goto L_90;
				iconcv = iconvx;
				}
L_80:
			;
			}

		/*     Close convex polygon.
		 */
L_90:
		if( xout[*nout - 1] != xout[0] || yout[*nout - 1] != yout[0] ){
			*nout += 1;
			xout[*nout - 1] = xout[0];
			yout[*nout - 1] = yout[0];
			iout[*nout - 1] = iout[0];
			}

		/*     The array iout contains all the convex points from the original  
		 *     polygon.  A check is now made to avoid duplication of existing
		 *     polygons.  This duplication is the result of the fact that
		 *     although there are 2*nconcv polygons generated, there are at most
		 *     nconcv+1 UNIQUE polygons.
		 */
		for( i = 1, _do7 = *nout; i <= _do7; i++ ){
			i_ = i - 1;

			/*     Set points flag to zero to avoid duplication of polygons.
			 */
			jp = iout[i_];
			for( jcheck = 1, _do8 = npoly; jcheck <= _do8; jcheck++ ){
				jcheck_ = jcheck - 1;
				if( jp == 0 )
					goto L_95;
				if( jcheck <= nconcv ){
					if( jp == point[jcheck_] )
						point[jcheck_] = 0;
					}
				else{
					nextpt = point[jcheck_] + 1;
					if( nextpt > np )
						nextpt = 1;
					if( jp == nextpt )
						point[jcheck_] = 0;
					}
L_95:
				;
				}
			}
		goto L_200;
L_150:
		;
		}
L_200:
	return;
} /* end of function */
