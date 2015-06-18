/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_FLL.C */
/*  3     2-MAR-1992 15:21:55 JILL "(SPR 0) added proto" */
/* *2    18-MAR-1991 18:10:34 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 18:01:34 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_STR_FLL.C */
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
/*list ts_str_fill
 *
 *    ts_str_fill - Routine to set characters in strings
 *
 *    Calling sequence:
 *
 *    ts_str_fill(image,nchars,fill_char)
 *
 *    Arguments:
 *
 *    Name         Input/  Type             Description
 *                 Output
 *
 *    image         I,O    *CHAR            Input character string.
 *
 *    nchars         I      INT             Number of caharcters to
 *                                          set.
 *
 *    fill_CHAR      I     *CHAR            Fill character to use.
 *  
 *    Comments:
 *
 *    This subroutine fills the input string with the fill
 *    character.  It is useful for initializing strings.  Note
 *    that the output string is NOT null-terminated.
 *
 *nolist ts_str_fill
 */
#include "esi_c_lib.h"
INT ts_str_fill(image,nchars,fill_char)
   CHAR *image,*fill_char;
   INT nchars;
{
   INT n;
   n=0;
   while(n++<=nchars)
   {
      image[n-1]=fill_char[0];
   }
}
