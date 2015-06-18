/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_COPY.C*/
/*   5     2-MAR-1992 15:24:20 JILL "(SPR 0) added proto"*/
/*  *4    23-JUL-1990 11:51:39 VINCE "(SPR 1) final fixes for recursive logical names on Unix"*/
/*  *3    19-JUL-1990 12:35:34 VINCE "(SPR 1) changed allocation of TO pointer to "*/
/*  *2     6-FEB-1990 15:39:58 WALTERS "(SPR 0) Fix ts_tcp_copy"*/
/*  *1    19-JUN-1989 13:34:18 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_COPY.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* **********************************************************************

   ROUTINE: TS_TCP_COPY

   DESCRIPTION:

    TS_TCP_COPY copies an array of character strings.

    Memory for the destination (to) array is allocated by TS_TCP_COPY.

    Each string in the source (from) array is duplicated into the 
    allocated memory.

    Calling TS_TCP_COPY with a null "from" parameter causes no action to
    be taken.

   ************************************************************************ */

#include "esi_c_lib.h"
#include "esi_tc.h"
#include "esi_ts.h"

/* ********************************************************************** */
publicdef TCP ts_tcp_copy(to,from)
/* ********************************************************************** */

TCP  *to;                       /* Destination */
TCP  from;                      /* Source */
{
    INT  tcp_length;		  /* Length of "from" tcp */
    int i;
/* ********************************************************************** */

    /* Initialize TO tcp */
    *to = (TCP)NULL;	  

    /* Do nothing if "from" is null    */
    if (from == (TCP)NULL) return *to;

    /* Get length of "from" tcp	     */
    tcp_length = ts_tcp_len (from);

    /* copy each string in the TCP */
    for(i=0; i<tcp_length; i++) {
	    ts_tcp_append(to, (CHAR *)from[i]);
    }
    
    return *to;
}
