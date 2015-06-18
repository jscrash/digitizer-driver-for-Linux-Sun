/* DEC/CMS REPLACEMENT HISTORY, Element AG_AUTO_SCLR.C */
/* *2    18-MAR-1991 18:06:16 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:44:32 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_AUTO_SCLR.C */
/*Translated by FOR_C++, v0.2, on 09/27/90 at 12:39:57 */
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
 * list ag_auto_scaler 
 *
 *     *** Routine to calculate scaled max/min valuess from an array ***
 *
 *     Calling sequence:
 *
 *     ag_auto_scaler(values,n,scaled_max,scaled_min,scaled_intrv,min_intrvs,
 *                 conf,n_intrvs,ndec_places);
 *
 *     Arguments:
 *
 *     Type    Name         Input/  Description
 *                          output
 *
 *     DOUBLE   values[]      I      Input array of valuess.
 *
 *     INT      n             I      Number of values.
 *
 *     DOUBLE  *scaled_max    O      Scaled maximum
 *
 *     DOUBLE  *scaled_min    O      Scaled minimum 
 *
 *     DOUBLE  *scaled_intrv  I,O    Scaled interval
 *
 *     INT      min_intrvs    I      Minimum number of intervals to generate
 *
 *     DOUBLE  *conf          I      Confidence level for eliminating bad data
 *
 *     INT     *n_intrvs      O      Actual number of intervals generated
 *
 *     INT     *ndec_places   O      Number of decimal places to format scaled_intrv
 *
 *     Comments:
 *
 *     This subroutine calculates scaled max/min valuess and intervals for
 *     the input array x.  At least min_intrvs intervals will be generated.
 *     The variable ndec_places indicates the number of decimal places needed
 *     to properly write scaled_intrv, and will be set to -1 if scaled_intrv > or = 1.0.
 *
 *           In a normally distributed group of numbers the probability 
 *     of a given values occurring can be calculated.  this subrourine will
 *     discard valuess which fall below the probability set in the variable
 *     conf.  For example, setting conf=0.001 would 
 *     insure that ag_auto_scaler would not consider valuess which have a
 *     probability of occurrence of less than one in a thousand.  Setting
 *     conf=0.0 will cause all x valuess to be considered.  Note that
 *     conf=0.0 should be specified if the data in the x array is highly
 *     non-normal.  
 *
 *     Revision history:
 *
 *     This routine was written for the Geological Survey of Canada 
 *     by Davis Swan in 1977.  
 *
 * nolist ag_auto_scaler
 */
void /*FUNCTION*/ ag_auto_scaler(values, n, scaled_max, scaled_min, scaled_intrv,
                              min_intrvs, conf, n_intrvs, ndec_places)
DOUBLE values[];
INT n;
DOUBLE *scaled_max, *scaled_min, *scaled_intrv;
INT min_intrvs;
DOUBLE *conf;
INT *n_intrvs, *ndec_places;
{
INT i;
DOUBLE pi, prob, sum, sumsq, test, exact_max, xmean, exact_min, xsd, z;
void ag_auto_scaled();
static DOUBLE dvalues[3]={1.0,0.5,0.2};

	exact_max = -1.0e20;
	exact_min = 1.0e20;
	pi = 3.14159;
	sum = 0.0;
	sumsq = 0.0;
/*
 *      Calculate mean and standard deviation for input values
 */
	if( *conf != 0.0 )
        {
	   for( i = 0; i < n; i++ )
           {
		sum += values[i];
		sumsq += values[i]*values[i];
           }
	   xmean = sum/ n;
	   xsd = sqrt( ABS( sumsq/ n - xmean*xmean ) );
	   test = *conf/xsd;
        }
	for( i = 0; i < n; i++ )
        {
		if( *conf != 0.0 )
                {
/*
 *                 Ignore for seemingly anomalous values
 */
		   z = (values[i] - xmean)/xsd;
		   if( z > 20.0 ) continue;
		   prob = exp( -z*z/2.0 )/(xsd*sqrt( 2.0*pi ));
		   if( prob < test ) continue;
                }
		if( values[i] < exact_min )
			exact_min = values[i];
		if( values[i] > exact_max )
			exact_max = values[i];
	}
	ag_auto_scaled( &exact_max, &exact_min, scaled_max, scaled_min, scaled_intrv,
                 min_intrvs, n_intrvs, ndec_places );
	return;
} /* end of function */
