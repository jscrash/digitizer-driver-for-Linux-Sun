/* DEC/CMS REPLACEMENT HISTORY, Element AG_SHFT_DBL.C */
/* *2    18-MAR-1991 18:07:41 JULIAN "(SPR 0) Lease Mapping Init Checkin" */
/* *1    18-MAR-1991 17:50:18 JULIAN "Lease Mapping" */
/* DEC/CMS REPLACEMENT HISTORY, Element AG_SHFT_DBL.C */
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
/*Translated by FOR_C++, v0.2, on 09/18/90 at 15:13:35 */
/*FOR_C++ Options SET: cio op=a c */
#include "esi_c_lib.h"
#include "esi_math.h"
/* list   ag_shift_DOUBLE 
 *
 *     ** Shifts a circular buffer of DOUBLE precision values **
 *
 *     Calling sequence:
 *
 *     ag_shift_double(buf,n,nshift); 
 *
 *     Arguments:
 *
 *     Name   Input/   Type      Description
 *            output
 *
 *     buf     I,O    double[]   Circular buffer containing values 
 *                               to be ag_shift_doubleed.
 *
 *     n       I        INT      Number of values in buffer.
 *
 *     nshift  I        INT      Number of positions to shift buffer
 *
 *     Comments:
 *
 *     This routine is used to ag_shift_DOUBLE values within a circular buffer.
 *     If nshift=1 all n values in the buffer will be ag_shift_doubleed one
 *     position left.  The first value will become the last value.  
 *     If nshift=-1 all values will be ag_shift_doubleed one position to the right
 *     and the last value will become the first value. 
 *
 * nolist ag_shift_DOUBLE 
 */
void /*FUNCTION*/ ag_shift_double(buf, n, nshift)
DOUBLE buf[];
INT n, nshift;
{
INT from, i, to;
DOUBLE tmpbuf, tstart;

	tstart = 1;
	to = 1;
	from = nshift + 1;
	if( from < 1 )
		from += n;
	tmpbuf = buf[0];
	for( i = 1; i <= n; i++ ){

		/*     the next transfer would overwrite a buffer element that  
		 *     has already been transferred.
		 */
		if( from != tstart )
			goto L_10;
		buf[to - 1] = tmpbuf;
		tstart += 1;
		to = tstart;
		from = to + nshift;
		if( from > n )
			from -= n;
		if( from < 1 )
			from += n;
		tmpbuf = buf[to - 1];
		goto L_20;
L_10:
		buf[to - 1] = buf[from - 1];
		to = from;
		from = to + nshift;
		if( from > n )
			from -= n;
		if( from < 1 )
			from += n;
L_20:
		;
		}
L_200:
	return;
} /* end of function */
