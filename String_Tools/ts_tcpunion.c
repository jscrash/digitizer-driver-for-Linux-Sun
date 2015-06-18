/* DEC/CMS REPLACEMENT HISTORY, Element TS_TCPUNION.C */
/*  3     2-MAR-1992 15:20:53 JILL "(SPR 0) removed flag from non-proto param list" */
/* *2    21-AUG-1990 12:27:35 TOM "(SPR 1) Initial coding." */
/* *1    21-AUG-1990 12:23:36 TOM "This function joins two tcps into a third tcp." */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_TCPUNION.C */
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
	This function takes two tcps and combines them into
	a third tcp.
	
Prototype:
    publicdef INT ts_tcp_union (TCP tcp1, TCP tcp2, TCP *tcp3);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) tcp1		-(TCP) Input tcp.
	(I) tcp2		-(TCP) Input tcp.
	(O) tcp3		-(TCP *) Output combination of tcp1 + tcp2.

Return Value/Status:
    SUCCESS	- Successfull completion
    TS_TCP_NOT_EMPTY - Returned if destination tcp in not NULL.
	
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_union (TCP tcp1, TCP tcp2, TCP *tcp3)
#else
publicdef INT ts_tcp_union (tcp1, tcp2, tcp3, flag)
TCP tcp1, tcp2, *tcp3;
#endif
{
TCP temp;
INT status = SUCCESS;

if (*tcp3 != (TCP)NULL)
    {
    status = TS_TCP_NOT_EMPTY;
    return status;
    }

if ((tcp1 == (TCP)NULL) && (tcp2 == (TCP)NULL))
    return status;

if (tcp1)
    for (temp = tcp1; *temp; temp++)
	ts_tcp_append (tcp3, *temp);

if (tcp2)
    for (temp = tcp2; *temp; temp++)
	ts_tcp_append (tcp3, *temp);

return status;
}
/* END:	    */








