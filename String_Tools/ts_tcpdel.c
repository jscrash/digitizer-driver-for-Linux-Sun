/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCPDEL.C*/
/*  *4    17-AUG-1990 22:30:30 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3     6-FEB-1990 10:15:50 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *2    11-AUG-1989 16:07:38 JULIAN "(SPR -1) tcp evaluation order undefined"*/
/*  *1    19-JUN-1989 13:34:05 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCPDEL.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_tc.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Remove a string from a Table of Character Pointers and compress the
    TCP.

Prototype:
    publicdef INT  ts_tcp_delete ( TCP tcp, INT idx );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) tcp         -(CHAR **) Table of Character Pointers.
    (I) idx         -(INT) index to string that is to be removed from TCP.

Return Value/Status:
    SUCCESS - Successfull completion of freeing TCP.
    FAIL    - Idx is greater than last index in TCP.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_delete (TCP tcp, INT idx) 
#else
publicdef INT ts_tcp_delete (tcp, idx)
TCP tcp;
INT idx;
#endif
    {
    if (idx > ts_tcp_len (tcp))
        {
        return FAIL;
        }
    tcp += idx - 1;
    
    tc_free ((BYTE *) * tcp);       /*  free the storage for this one           */
    
    while (*tcp)
        {
        *tcp = *(tcp + 1);          /*  move all others up one place            */
        tcp++;
        }
    return SUCCESS;
    }
/* END:     */
