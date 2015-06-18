/* DEC/CMS REPLACEMENT HISTORY, Element CT_DEC2DMS.C*/
/* *7    15-AUG-1991 18:45:59 CHARLIE "(SPR -1) revert destructive mods"*/
/* *6     8-JUL-1991 11:38:04 CHEW "(SPR 7344) Correct conversion error for negative zero degree."*/
/* *5    13-MAY-1991 18:14:59 MING "(SPR -1) handle truncation errors in geodetic decimal coord from charlie"*/
/*  3B1   3-OCT-1990 11:36:55 GILLESPIE "Change file type to stream"*/
/* *4    20-AUG-1990 16:48:33 KEE "(SPR -1) Fixing Sun side compile error "*/
/* *3    26-JUL-1990 18:49:48 JESSIE "(SPR 0) recover to previous version"*/
/* *2    26-JUL-1990 18:31:56 JESSIE "(SPR 0) modify for map setup"*/
/* *1    12-JUL-1990 15:08:19 GILLESPIE "Convert dms to decimal and back again"*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_DEC2DMS.C*/

#include "esi_c_lib.h"
#include "esi_ct.h"

/***/

/* Function to convert decimal degrees to degrees, minutes,seconds
Input:
    LATLONG *latlongdecimal field only
    LL_T type
    
Output:
    LATLONG *latlong:    degrees, minutes,seconds fields filled in
    latlong_str          Character string containing ASCII representation of DMS
                                (caller is responsible for allocating sufficient space)
    
*/

#if USE_PROTOTYPES
publicdef VOID ct_decimal_to_dms(LATLONG *latlong, LL_T type, CHAR *latlong_str)
#else
publicdef VOID ct_decimal_to_dms(latlong, type, latlong_str)
LATLONG *latlong;
LL_T    type;
CHAR    *latlong_str;
#endif
{
    UINT degrees;
    char direction;
    double decimal = latlong -> decimal;
    double r;
    int dir;
    
    if (decimal < 0)
        {
        decimal *= -1;
        dir = -1;
        }
    else
        {
        dir = 1;
        }
    
    degrees = (unsigned int)decimal;
    latlong -> dms.degrees = degrees * dir;
    
    decimal -= degrees;
    latlong -> dms.minutes = 60 * decimal;    /* NOTE: degrees maintains sign of decimal*/
    
    r = latlong -> dms.minutes;
    latlong -> dms.seconds = (decimal - r / 60.0) * 3600.0;

/* (CP) this was commented out and incorrect. Trying a 'better' version */
/* check seconds and minutes are over 60 */
/*  if (latlong -> dms.seconds > 59.)	let's try it, but when VERY close to next
					minute */
    if (latlong -> dms.seconds > 59.95)
    {
    latlong -> dms.seconds = 0.0;
    latlong -> dms.minutes += 1;
    }
    /* 'cascade' roundoff from minutes to seconds */
    if (latlong -> dms.minutes > 59.)	/* we did a 'degree' round-off */
    {
	latlong -> dms.minutes = 0;	/* let's don't forget this */
        degrees += 1;
        if (latlong -> dms.degrees < 0)
        latlong -> dms.degrees -= 1;	/* round 'down' to negative */
        else
        latlong -> dms.degrees += 1;	/* round 'up' to next positive */
     }

    if (type == LATITUDE && dir == 1)
        direction = 'N';
    else if (type == LATITUDE && dir == -1)
        direction = 'S';
    else if (type == LONGITUDE && dir == -1)
        direction = 'W';
    else if (type == LONGITUDE && dir == 1)
        direction = 'E';
    
    sprintf(latlong_str, "%d %d' %.1f\" %c", degrees, latlong -> dms.minutes, latlong -> dms.seconds, direction);
    }
