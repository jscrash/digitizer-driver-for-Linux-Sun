/* DEC/CMS REPLACEMENT HISTORY, Element CT_DMS2DEC.C*/
/* *2     8-JUL-1991 11:39:14 CHEW "(SPR 7344) Correct conversion error for negative zero degree."*/
/* *1    12-JUL-1990 15:08:23 GILLESPIE "Convert dms to decimal and back again"*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_DMS2DEC.C*/
#include "esi_ct.h"

/* Function to convert degrees, minutes,seconds to decimal degrees 
Input:
	LATLONG *latlong	degrees, minutes,seconds fields only
	
Output:
	LATLONG *latlong:	decimal field filled in
	
*/

#if USE_PROTOTYPES
publicdef VOID ct_dms_to_decimal(LATLONG *latlong)
#else
publicdef VOID ct_dms_to_decimal(latlong)
LATLONG *latlong;
#endif
{
    INT td, tm;
    DOUBLE ts, ts_abs;
    
    td = latlong->dms.degrees;
    tm = latlong->dms.minutes;

    /* determine the absolute value of latlong->dms.seconds */
    if ((ts = latlong->dms.seconds) < 0.0)
        ts_abs = ts * (-1.0);
    else
        ts_abs = ts;

    latlong->decimal = abs(td) + abs(tm) / 60.0 + ts_abs / 3600.0;
    if ((td < 0) || ((td == 0)&&(tm < 0)) || ((td == 0)&&(tm == 0)&&(ts < 0.0)))
        latlong->decimal *= -1.0;
}
