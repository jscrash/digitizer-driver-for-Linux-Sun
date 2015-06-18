/* DEC/CMS REPLACEMENT HISTORY, Element TS_STRN_TRM.C */
/*  3     2-MAR-1992 15:23:45 JILL "(SPR 0) added proto" */
/* *2    18-MAR-1991 18:10:09 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 18:00:23 JULIAN "0" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_STRN_TRM.C */
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
/*
 *   ts_str_trim  - function to trim off trailing blanks
 */
#include "esi_c_lib.h"
#include "esi_ctype.h"
int	ts_strn_trim(string,n)
char	*string;
INT  n;
{
   INT length;
   for (length = n ;length > 0; length--)
   {
      if(isgraph(string[length-1]))break;
   }
   return(length);
}

