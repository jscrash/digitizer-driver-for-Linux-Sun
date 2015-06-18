/* DEC/CMS REPLACEMENT HISTORY, Element AG_AUTO_SCLD.C */
/* *2    18-MAR-1991 18:06:10 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:44:05 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_AUTO_SCLD.C */
/*Translated by FOR_C++, v0.2, on 09/27/90 at 13:08:14 */
/*FOR_C++ Options SET: cio op=a c */
#include "esi_c_lib.h"
#include "esi_math.h"
/*********************************************************************
 *
 *
 *                 Copyright Gulf Canada Resources, 1990
 *                  Unpublished -- All rights reserved
 *
 * THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Gulf Canada Resources.
 * IT MAY NOT BE USED, MODIFIED, DUPLICATED OR COPIED IN ANY FORM
 * WITHOUT PRIOR WRITTEN CONSENT OF
 *
 *                       Gulf Canada Resources, Inc.
 *                       401 9th Avenue SW
 *                       Calgary, Alberta
 *                       Canada T2P 2H7
 *                       (403) 233-4000
 *
 * (The above notice does not constitute an admission of publication or
 *  unrestricted dissemination of the work)
 **********************************************************************
 * list ag_auto_scaled 
 *
 *     *** Routine to calculate scaled max/min values from exact values ***
 *
 *     Calling sequence:
 *
 *     ag_auto_scaled(exact_max,exact_min,&scaled_max,&scaled_min,&scaled_intrv,
 *                 min_intrvs,&n_intrvs,&ndec_places)  
 *
 *     Arguments:
 *
 *     Type    Name          Input/  Description
 *                           output
 *
 *     DOUBLE   exact_max      I     Exact maximum
 *
 *     DOUBLE   exact_min      I     Exact minimum
 *
 *     DOUBLE  *scaled_max     O     Scaled maximum
 *
 *     DOUBLE  *scaled_min     O     Scaled minimum 
 *
 *     DOUBLE  *scaled_intrv  I,O    Scaled interval
 *
 *     INT      min_intrvs     I     Minimum number of intervals to generate
 *
 *     INT     *n_intrvs       O     Actual number of intervals generated
 *
 *     INT     *ndec_places    O     Number of decimal places to format scaled_intrv
 *
 *     Comments:
 *
 *     This subroutine calculates a scaled maximum, minumum, and interval
 *     value from the input exact maximum and minium.  At least min_intrvs
 *     intervals will be generated.
 *
 *     Revision history:
 *
 *     This routine was written for the Geological Survey of Canada 
 *     by Davis Swan in 1977.  
 *
 * nolist ag_auto_scaled
 */
#define TRUE 1
#define FALSE 0
void /*FUNCTION*/ ag_auto_scaled(exact_max, exact_min, scaled_max, scaled_min, 
	                      scaled_intrv, min_intrvs, n_intrvs, ndec_places)
DOUBLE exact_max, exact_min, *scaled_max, *scaled_min, *scaled_intrv;
INT min_intrvs, *n_intrvs, *ndec_places;
{
INT i, iexact_min, getdx, j, magx;
DOUBLE pi, xrange;
static DOUBLE dx[3]={1.0,0.5,0.2};

	pi = 3.14159;
	xrange = exact_max - exact_min;
	if( xrange == 0.0 ) return;
        /*
	 *     Choose the interval to be used for x values unless min_intrvs was  
	 *     set to zero and a non-zero interval was input.
	 */
	getdx = FALSE;
	if( (min_intrvs > 0 || *scaled_intrv <= 0.0) || *scaled_intrv > xrange )
		getdx = TRUE;
	magx = log10( xrange ) + 1.0e0;
	for( i = 0; i < 5; i++ )
        {
		for( j = 0; j < 3; j++ )
		{
			if( getdx )
				*scaled_intrv = dx[j]*pow(10.0e0,(double)magx);
			*n_intrvs = xrange/ *scaled_intrv;
			if( *n_intrvs**scaled_intrv < xrange )
				*n_intrvs += 1;
			if( *n_intrvs >= min_intrvs ) break;
			if( j == 2 )
				magx -= 1;
		}
	}
	iexact_min = ABS( exact_min )/ *scaled_intrv;
        /*
	 *     Correct for the fact that negative numbers are rounded up.
	 */
	if( exact_min < 0.0 )
		iexact_min += 1;
	*scaled_min = iexact_min**scaled_intrv*( exact_min / ABS(exact_min) );
	if( *scaled_min + *n_intrvs**scaled_intrv < exact_max )
		*n_intrvs += 1;
	*scaled_max = *scaled_min + *n_intrvs**scaled_intrv;
	*ndec_places = -1;
	if( *scaled_intrv >= 1.0 ) return;
	*ndec_places = -log10( *scaled_intrv );
	if( *ndec_places < -log10( *scaled_intrv ) )
		*ndec_places += 1;
	return;
} /* end of function */
