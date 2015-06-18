/* DEC/CMS REPLACEMENT HISTORY, Element TS_VALID_YMD.C */
/* *2    13-MAY-1991 17:17:23 LOEBL "(SPR 0) Initial version of routine" */
/* *1    13-MAY-1991 17:12:32 LOEBL "Function which validates a date in standard format." */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_VALID_YMD.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_gl_defs.h"
#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif


/* Function Description -----------------------------------------------------
Description:
Provide a basic logical date check routine for higher routines which provide
formatted date checks.


Prototype:
    publicdef INT ts_valid_ymd( char *year, char *mo, char *day );

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) year  - AD year
    (I) mo    - 1 - to 12
    (I) day   - day of month;

Return Value/Status:

 returns i + j + k 
   where i = 1 if year is invalid, otherwise 0
         j = 2 if month is invalid, otherwise 0
         k = 4 if day is invalid, otherwise 0

Scope:
    PUBLIC

Limitations/Assumptions:

Only tested for years A.D.  If BC (i.e. negative years) implemented, then
usage of ts_isfloat() will need to be revisited 
-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
  publicdef INT ts_valid_ymd( char *yr, char *mo, char *dy )
#else
  publicdef INT ts_valid_ymd(yr,mo,dy)
  char *yr,*mo,*dy;
#endif
{
    INT iyr, imo, idy;
    INT ier;
    static INT days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    ier = ( ts_isfloat(yr) > 0 ? 0 : 1) +
          ( ts_isfloat(mo) > 0 ? 0 : 2) +
          ( ts_isfloat(dy) > 0 ? 0 : 4);

    if (ier) return(ier);
    
    iyr=0;
    imo=0;
    idy=0;

    sscanf(yr, "%d", &iyr);
    sscanf(mo, "%d", &imo);
    sscanf(dy, "%d", &idy);
    
    if (imo < 1 || imo > 12 ) return (2);

    if (idy < 1 || idy > days[imo-1] + tc_isleap(iyr) ) return(4);

    return (0);
}
/* END:	    */
