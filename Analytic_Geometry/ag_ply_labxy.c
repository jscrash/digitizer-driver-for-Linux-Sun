/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_LABXY.C */
/* *3     1-MAY-1991 11:17:15 KEE "(SPR -1) Fix bug for vms version" */
/* *2    18-MAR-1991 18:07:16 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:48:33 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_LABXY.C */
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
/* list ag_poly_labelxy
 *
 *     *** Find optimal label position within an irregular polygon ***
 *
 *     Calling Sequence:
 *
 *     ag_poly_labelxy(xpoly,ypoly,np,isense,wratio,&xlab,&ylab,&wmax)
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
 *     np        I       INT         Number of points in polygon.
 *
 *     isense    I       INT         Sense of direction (-1=clockwise,
 *                                   1=counter-clockwise)
 *
 *     wratio    I      DOUBLE       Width to height ratio for rectangle
 *                                   containing label.
 *
 *     *xlab     O      DOUBLE       X coordinate for label position.
 *
 *     *ylab     O      DOUBLE       Y coordinate for label position.
 *
 *     *wmax     O      DOUBLE       Maximum width of label that can be
 *                                   plotted inside the polygon.
 *
 *     Comments:
 *
 *     This routine examins the input polygon, defines all the convex
 *     polygons contained by the input polygon, and locates the
 *     optimal label position.  Polygons containing more than 100
 *     points cannot be processed.
 *
 * nolist ag_poly_labelxy
 */
#define TRUE 1
#define FALSE 0
#define	MAXPTS	200

void /*FUNCTION*/ ag_poly_labelxy(xpoly, ypoly, np, isense, wratio, xlab, ylab, 
	 wmax)
DOUBLE xpoly[], ypoly[];
INT np, isense;
DOUBLE wratio, *xlab, *ylab, *wmax;
{
INT _do0, i, i_, jpoly, jpoly_, nout;
DOUBLE area, armax, wnow, xcentr, xmax, xmin, xout[MAXPTS], ycentr, 
	 ymax, ymin, yout[MAXPTS], xtemp[MAXPTS], ytemp[MAXPTS];
/*
 *  The following decalartions are for debugging only
 */
CHAR dummy[20];
INT icolor,ibord,j;
void ag_poly_area(), ag_poly_convex();
static INT debug = FALSE;


	/*     Initialize variables for this polygon
	 */
	*wmax = 0.0;
	if( np < 3 || np > MAXPTS )
		goto L_100;
	armax = 0.0;
	xmax = xpoly[0];
	xmin = xpoly[0];
	ymax = ypoly[0];
	ymin = ypoly[0];
	for( i = 2; i <= np; i++ ){
		i_ = i - 1;
		xmax = MAX( xmax, xpoly[i_] );
		xmin = MIN( xmin, xpoly[i_] );
		ymax = MAX( ymax, ypoly[i_] );
		ymin = MIN( ymin, ypoly[i_] );
		}

	/*     Start polygons at concave points
	 */
	for( jpoly = 1; jpoly <= 50; jpoly++ ){
		jpoly_ = jpoly - 1;

		/*     Get next convex polygon.
		 */
		ag_poly_convex( xpoly, ypoly, np, isense, jpoly, xout, yout, &nout );
		if( nout == 0 )
			goto L_100;

		/*     Calculate area and center of convex polygon.
		 */
		ag_poly_area( xout, yout, nout, wratio, &area, &xcentr, &ycentr, 
		 &wnow );
		if( ((xcentr > xmax || xcentr < xmin) || ycentr > ymax) || 
		 ycentr < ymin )
			goto L_50;
		/*-----
		 *-----Graphx commands used for debugging -- These commands 
                 *-----should be replaced by GKS commands for debugging in FINDER.
                 *-----The call to geoscl is only there because the Gulf system
                 *-----expects coordinates in plot inches.
		 *-----  
#ifdef vms
		       getclr(&jpoly,&icolor);
                       ibord = 1;
                       for(j = 0; j < nout ; j++)
                       {
                          geoscl(&xout[j],&yout[j],&xtemp[j],&ytemp[j]);
                       }
		       ipoly(xtemp,ytemp,&nout,&icolor,&ibord);
#endif

#if UNIX
		       getclr_(&jpoly,&icolor);
                       ibord = 1;
                       for(j = 0; j < nout ; j++)
                       {
                          geoscl_(&xout[j],&yout[j],&xtemp[j],&ytemp[j]);
                       }
		       ipoly_(xtemp,ytemp,&nout,&icolor,&ibord);
#endif

#ifdef primos
		       getclr(jpoly,icolor);
                       ibord = 1;
                       for(j = 0; j < nout ; j++)
                       {
                          geoscl(xout[j],yout[j],xtemp[j],ytemp[j]);
                       }
		       ipoly(*xtemp,*ytemp,nout,icolor,ibord);
#endif

                       printf("Enter anything to contine >\n");
                       scanf("%s",dummy);
		 *-----
		 *-----Update the maximum rectangle size
		 */
		if( *wmax - wnow > 0.001 )
			goto L_50;
		if( wnow - *wmax < 0.001 && area <= armax )
			goto L_50;
		*wmax = wnow;
		armax = area;
		*xlab = xcentr;
		*ylab = ycentr;
L_50:
		;
		}
L_100:
	if( debug == TRUE )
		{
		printf("0*** DIAGNOSTIC ***ag_poly_labelxy - x,y,wmax =%7.2f%7.2f%7.2f\n", 
		 *xlab, *ylab, *wmax );
		}
	return;
} /* end of function */
