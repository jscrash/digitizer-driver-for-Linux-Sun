/* DEC/CMS REPLACEMENT HISTORY, Element TS_VALID_DATE.C */
/* *3    17-MAY-1991 14:42:58 LOEBL "(SPR 0) Insert missing include file" */
/* *2    13-MAY-1991 17:18:14 LOEBL "(SPR 0) Initial version of routine" */
/* *1    13-MAY-1991 17:13:22 LOEBL "Function which validates a date in user-defined format." */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_VALID_DATE.C */
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
    
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
    

/* Function Description -----------------------------------------------------
Description:
    Valid a date string/date format 

Prototype:
    publicdef INT ts_valid_date ( char *date, char *format)

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) date    - string containing formatted date
    (i) format  - string which decodes the date string.

Return Value/Status:

   returns { e  |  i + j + k + l + m + n }
   where e = -1 if format mask is invalid 
         i = 1 if year is invalid, otherwise 0
         j = 2 if month is invalid, otherwise 0
         k = 4 if day is invalid, otherwise 0
         l = 8   if hours is invalid, otherwise 0
         m = 16  if minutes is invalid, otherwise 0
         n = 32  if seconds is invalid, otherwise 0

Scope:
    PUBLIC

Limitations/Assumptions:

This routine will be grown as needed. 
As/of 5/13/91 the only date format being considered is 
"mm/dd/yy" which actually represents strings of the form mm_/_dd_/_yyyy
where _ represents optional whitespace.
        

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
    publicdef INT ts_valid_date ( char *date, char *format)
#else
    publicdef INT ts_valid_date(date, format)
    char *date, *format;
#endif
{
    char yr[12], mo[12], dy[12];
    char *next;
    INT ier;
    INT iyr, imo, idy;
    


   ier = -1;
   if ( strcmp(format,"MM/DD/YYYY") == 0 )
   {
      ier = 0;
      if ( !(next = ts_next_token( date, mo, '/') ) ) return (2);
      if ( !(next = ts_next_token( next, dy, '/') ) ) return (4);
      if (strlen (next) == 0 ) return (1);
      strcpy (yr, next);
      ier = ts_valid_ymd(yr, mo, dy);
   }

    return (ier);
}
/* END:	    */
