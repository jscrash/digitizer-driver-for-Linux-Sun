/* DEC/CMS REPLACEMENT HISTORY, Element AG_TMLLXY.C */
/* *2    18-MAR-1991 18:07:47 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:50:46 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_TMLLXY.C */
/*************************************************************************


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
/* list ag_tm_ll_xy
 *
 *     *** Project latitude/longitude to Tranverse Mercator X/Y ***
 *
 *     Calling sequence:
 *
 *     ag_tm_ll_xy(lat,long,lamdac,k0,ec,&x,&y)
 *
 *     Arguments:
 *
 *     Name    Input/       Type       Description
 *             output
 *
 *     lat       O         DOUBLE      Latitude.
 *
 *     long      O         DOUBLE      Longitude.
 *
 *     lamdac    I         DOUBLE      Central meridian for projection.
 *
 *     k0        I         DOUBLE      Scale factor at lamdac.
 *
 *     ec        I         DOUBLE      False Easting (x value).
 *
 *     *x        I         DOUBLE      Tranverse Mercator X coordinate.
 *
 *     *y        I         DOUBLE      Tranverse Mercator Y coordinate.
 *
 *     Comments:
 *
 *     This subroutine is designed to calculate the Transverse Mercator
 *     X,Y coordinates from the latitide/longitude.  To generate UTM
 *     coordinates set k0=0.9996, ec=500000.0 and set lamdac
 *     appropriately.  Otherwise set k0=1.000, ec=0.0
 *
 *     Revision History:
 *
 *     Original UTM routine (GEOUTM) written by Davis Swan for the
 *     Geological Survey in 1977.  Converted to decimal degrees in
 *     January, 1988.
 *
 * nolist ag_tm_ll_xy
 */
#define SIGN(v)	( (v) < 0 ? -1 : 1 )
void /*FUNCTION*/ ag_tm_ll_xy(lat, long_, lamdac, k0, ec, x, y)
DOUBLE lat, long_, lamdac, k0, ec, *x, *y;
{
DOUBLE a6, a6a, a6b, b5, b5a, b5b, dlamda, eprmsq, esq, i, ii, iii, 
	 iiia, iiib, iv, lambda, nu, phi, ra, s, sa, sb, sc, sd, sinsec, 
	 v, xadd, xsign;
static DOUBLE flag = 0, TWO = 2.0, THREE = 3.0, FOUR = 4.0, FIVE = 5.0, SIX = 6.0;

	ra = 6.3782064e6;
	esq = 6.768657997291e-3;
	eprmsq = esq/(1.0e0 - esq);
	sinsec = 3.1415926535e0/180.0e0;

	/*     Convert latitude to radian measure so that Fortran trigonometric
	 *     functions can be used.  Note that the conversion factor (pi
	 *     radians/ 180 degrees) is equal to sin(1").
	 */
	phi = lat*sinsec;
	lambda = long_;
	dlamda = ABS( lambda - lamdac );
	if( dlamda == 0.0e0 ){
		xsign = 1.0e0;
		}
	else{
		xsign = (lambda - lamdac)/dlamda;
		}
	nu = ra/sqrt( 1.0e0 - esq*pow(sin( phi ), TWO) );
	sa = (1.0e0 + (3.0e0/4.0e0)*esq + (45.0e0/64.0e0)*pow(esq, TWO) + 
	 (175.0e0/256.0e0)*pow(esq, THREE))*phi;
	sb = (((3.0e0/4.0e0)*esq + (15.0e0/16.0e0)*pow(esq, TWO) + (525.0e0/
	 512.0e0)*pow(esq, THREE))*sin( 2.0e0*phi ))/2.0e0;
	sc = (((15.0e0/64.0e0)*pow(esq, TWO) + (105.0e0/256.0e0)*pow(esq, THREE))*
	 sin( 4.0e0*phi ))/4.0e0;
	sd = ((35.0e0/512.0e0)*pow(esq, THREE)*sin( 6.0e0*phi ))/6.0e0;
	s = ra*(1.0e0 - esq)*(sa - sb + sc - sc);
	i = s*k0;
	ii = (nu*sin( phi )*cos( phi )*pow(sinsec, TWO)*k0)/2.0e0;
	iiia = (pow(sinsec, FOUR)*nu*sin( phi )*pow(cos( phi ), THREE))/24.0e0;
	iiib = (5.0e0 - pow(tan( phi ), TWO) + 9.0e0*eprmsq*pow(cos( phi ), TWO) + 
	 4.0e0*pow(eprmsq, TWO)*pow(cos( phi ), FOUR))*k0;
	iii = iiia*iiib;
	iv = nu*cos( phi )*sinsec*k0;
	v = (pow(sinsec, THREE)*nu*pow(cos( phi ), THREE)*(1.0e0 - pow(tan( phi ), TWO) + 
	 eprmsq*pow(cos( phi ), TWO))*k0)/6.0e0;
	a6a = (pow(dlamda, SIX)*pow(sinsec, SIX)*nu*sin( phi )*pow(cos( phi ), FIVE))/
	 7.2e2;
	a6b = (61.0e0 - 58.0e0*pow(tan( phi ), TWO) + pow(tan( phi ), FOUR) + 
	 2.7e2*eprmsq*pow(cos( phi ), TWO) - 3.3e2*eprmsq*pow(sin( phi ), TWO))*
	 k0;
	a6 = a6a*a6b;
	b5a = (pow(dlamda, FIVE)*pow(sinsec, FIVE)*nu*pow(cos( phi ), FIVE))/
	 1.202;
	b5b = (5.0e0 - 18.0e0*pow(tan( phi ), TWO) + pow(tan( phi ), FOUR) + 
	 14.0e0*eprmsq*pow(cos( phi ), TWO) - 58.0e0*eprmsq*pow(sin( phi ), TWO))*
	 k0;
	b5 = b5a*b5b;
	*y = i + ii*pow(dlamda, TWO) + iii*pow(dlamda, FOUR) + a6;
	xadd = iv*dlamda + v*pow(dlamda, THREE) + b5;
	*x = ec + xsign*xadd;
	return;
} /* end of function */
