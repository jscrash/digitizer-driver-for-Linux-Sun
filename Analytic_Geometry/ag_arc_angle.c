/* DEC/CMS REPLACEMENT HISTORY, Element AG_ARC_ANGLE.C */
/* *2    18-MAR-1991 18:05:51 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:43:35 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_ARC_ANGLE.C */
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
/* list ag_arc_angle  
 *
 *     **Function to calculate difference between two angles**  
 *
 *     Calling sequence:
 *
 *     diff=ag_arc_angle(angle1,angle2)
 *
 *     Arguments:
 *
 *     Name    Input/      Type       Description
 *             Output
 *
 *     angle1    I       DOUBLE       First angle in radians.
 *
 *     angle2    I       DOUBLE       Second angle in radians.  
 *
 *     Comments:
 *
 *     This routine will not return differences greater than PI radians.
 *     Angles greater than 2*PI radians, and angles which straddle the  
 *     0 axis are handled correctly.
 *
 * nolist ag_arc_angle
 *
 ******This function was written for Gulf Canada Resources by Davis Swan
 *     in December, 1986.
 */
DOUBLE /*FUNCTION*/ ag_arc_angle(angle1, angle2)
DOUBLE angle1, angle2;
{
INT icycle;
DOUBLE ang1, ang2, ag_arc_angle_v;
static DOUBLE pi = 3.14159265358979323846;
static DOUBLE tolrnc = 0.00001;

	icycle = angle1/(2.0*pi);
	ang1 = angle1 - icycle*2.0*pi;
	icycle = angle2/(2.0*pi);
	ang2 = angle2 - icycle*2.0*pi;
	ag_arc_angle_v = ang2 - ang1;
	if( ABS( ag_arc_angle_v - pi ) < tolrnc )
		ag_arc_angle_v = pi;
	if( ag_arc_angle_v > pi )
		ag_arc_angle_v += -2.0*pi;
	if( ag_arc_angle_v < -pi )
		ag_arc_angle_v += 2.0*pi;
	return( ag_arc_angle_v );
} /* end of function */
