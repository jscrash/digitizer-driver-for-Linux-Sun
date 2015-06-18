/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PRLL.C */
/* *3     1-MAY-1991 11:16:47 KEE "(SPR -1) Fix bug for vms version " */
/* *2    18-MAR-1991 18:07:22 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:48:59 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_PLY_PRLL.C */
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
/*Translated by FOR_C++, v0.2, on 10/03/90 at 09:39:23 */
/*FOR_C++ Options SET: cio op=a c */
#include <stdio.h>
#include <math.h>
/* list poly_parall
 *
 *     *** Create a line string parallel to that input ***
 *
 *     Calling sequence:
 *
 *     call poly_parall(xpin,ypin,npin,isense,pdist,maxout,xpout,ypout,npout)
 *
 *     Arguments:
 *
 *     Name   Input/     Type         Description
 *            output
 *
 *     xpin     I      double[npin]     X coordinates for input line.
 *
 *     ypin     I      double[npin]     Y coordinates for input line.
 *
 *     npin     I         int           Number of input points.
 *
 *     isense   I         int           Sense of direction of output line.
 *                                      -1=clockwise, 1=counterclockwise
 *
 *     pdist    I        double         Perpendicular distance between lines.  
 *
 *     maxout   I         int           Maximum output points allowed.
 *
 *     xpout    O      double[npout]    X coordinates for output line. 
 *
 *     ypout    O      double[npout]    Y coordinates for output line.
 *
 *     *npout   O         int           Number of output points.
 *
 *     Comments:
 *
 *     This routine creates a line parallel to the input line, either
 *     on the clockwise (isense=-1) or the counter-clockwise (isense=1)
 *     side of the input line.
 *
 * nolist poly_parall
 */
#define 	MAX(a,b)	( (a) > (b) ? (a) : (b) )
#define		MIN(a,b)	( (a) < (b) ? (a) : (b) )
#define ABS(v)	( (v) < 0 ? -(v) : (v) )

void /*FUNCTION*/ poly_cparal(xpin, ypin, npin, isense, pdist, maxout, 
	 xpout, ypout, npout)
double xpin[], ypin[];
int npin, isense;
double pdist;
int maxout;
double xpout[], ypout[];
int *npout;
{
int jp,jstart,nangle;
double angle, anglst, diff, poff, r, wfact, x, x1, x2, xoff, y, y1, 
	 y2, yoff;
void ag_polar_coord();
static double pi = 3.14159;
static double angnow = 0.0;

	if( npin <= 2 ) return;
        nangle = 0;
/*      
 *      If this is a closed polygon, start with the second last point 
 *      rather than the first
 */
        if(xpin[0] == xpin[npin - 1] && ypin[0] == ypin[npin - 1])
        {
           jstart = 0;
           x1 = xpin[npin - 2];
           y1 = ypin[npin - 2];
        }
        else
        {
           jstart = 1;
           x1 = xpin[0];
           y1 = ypin[0];
        }
	poff = pdist*isense;
	for( jp = jstart; jp < npin; jp++ )
        {
           x2 = xpin[jp];
           y2 = ypin[jp];
           if(x2 != x1 || y2 != y1 )
           {
                /*
		 *     Calculate angle along line          
		 */
		anglst = angnow;
		wfact = 1.0;
		ag_polar_coord( x2, y2, x1, y1, &angnow, &r );
                nangle++;
		if( nangle == 1 && jstart == 1)
		{

			/*        Use first angle to plot first point.
			 */
			angle = angnow;
			}
		else{

			/*        Use the average angle for points between the first and last.
			 */
                        if( nangle == 1) break;
			diff = angnow - anglst;
			if( diff > pi )
				diff += -2.0*pi;
			if( diff < -pi )
				diff += 2.0*pi;

			/*        Calculate factor for line offset
			 */
			wfact = MAX( 0.2, cos( 0.5*(ABS( diff )) ) );
			angle = anglst + diff/2.0;
			if( angle > 2.0*pi )
				angle += -2.0*pi;
			if( angle < 0.0 )
				angle += 2.0*pi;
			if( diff*isense > pi/1.9 ){

				/*        If the line bends more than pi/2 round out the corner.  This is
				 *        done by plotting points at the ends of the line segments as well
				 *        as at the intersection of the line segments as shown below.
				 *
				 *                  \
				 *                   \     *
				 *                    \    
				 *            ----------
				 *                          *
				 *
				 *                     *
				 */
				x = x1 + poff*sin( anglst );
				y = y1 - poff*cos( anglst );
				*npout = MIN( maxout, *npout + 1 );
				xpout[*npout - 1] = x;
				ypout[*npout - 1] = y;
				x = x1 + poff*sin( angle );
				y = y1 - poff*cos( angle );
				*npout = MIN( maxout, *npout + 1 );
				xpout[*npout - 1] = x;
				ypout[*npout - 1] = y;
				angle = angnow;
				wfact = 1.0;
				}
			}
		xoff = (poff/wfact)*sin( angle );
		yoff = (poff/wfact)*cos( angle );
		x = x1 + xoff;
		y = y1 - yoff;
		*npout = MIN( maxout, *npout + 1 );
		xpout[*npout - 1] = x;
		ypout[*npout - 1] = y;
            }
            x1 = x2;
            y1 = y2;
	}
        /*
	 *     Plot the last point
	 */
	x = x2 + poff*sin( angnow );
	y = y2 - poff*cos( angnow );
	*npout = MIN( maxout, *npout + 1 );
	xpout[*npout - 1] = x;
	ypout[*npout - 1] = y;
	return;
} /* end of function */
