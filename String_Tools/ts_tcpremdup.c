/* DEC/CMS REPLACEMENT HISTORY, Element TS_TCPREMDUP.C */
/* *2    21-AUG-1990 12:27:28 TOM "(SPR 1) Initial coding." */
/* *1    21-AUG-1990 12:24:30 TOM "This function removes duplicates in a tcp." */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_TCPREMDUP.C */
/*****************************************************************************/
/******************************************************************************

				Copyright Finder Graphics Systems, Inc. 1990
		   			Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

							Finder Graphics Systems
							201 Tamal Vista Blvd.
							Corte Madera, CA  94925
							(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_ts.h"
#include "esi_ts_err.h"

/* Function Description -----------------------------------------------------
Description:
	This function removes duplicate strings in a tcp.  It first
	sorts the tcp and then removes all the duplicates.
	
Prototype:
    publicdef INT ts_tcp_remove_duplicates (TCP tcp);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) tcp			-(TCP) Modified tcp.

Return Value/Status:
    SUCCESS	- Successful completion
    TS_TCP_EMPTY - Returned if tcp is NULL.
	
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_remove_duplicates (TCP tcp)
#else
publicdef INT ts_tcp_remove_duplicates (tcp)
TCP tcp;
#endif
{
TCP temp;
INT status = SUCCESS;
INT index = 1;

ts_tcp_sort (tcp);

if (tcp == (TCP)NULL)
    {
    status = TS_TCP_EMPTY;
    return status;
    }

for (temp = tcp; *temp; temp++)
    {
    while ((*(temp+1)) && (strcmp (*temp, *(temp+1)) == 0))
	ts_tcp_delete (temp, 1);
    }

return status;
}
/* END:	    */


