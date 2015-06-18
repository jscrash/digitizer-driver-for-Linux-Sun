/*  DEC/CMS REPLACEMENT HISTORY, Element TS_UNIQ_NUM.C*/
/*  *3    17-AUG-1990 22:31:07 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    27-OCT-1989 09:00:52 GILLESPIE "(SPR 1) Added esi_time.h include file"*/
/*  *1    19-JUN-1989 13:34:33 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_UNIQ_NUM.C*/
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

#include "esi_ts.h"
#include "esi_time.h"

#define WRITTING_DATE 610650821

/* Function Description -----------------------------------------------------
Description:
    Compute a number which is unique to all of FINDER. This routine attempts to 
    generate a reasonably unique id number for the caller, based on the number
    of second since this routine was written, adjusted accordingly.  The date of 
    this routine is 8 May 1989 11:19 am (610650821 sec.).

Prototype:
    publicdef UINT ts_uniq_num ();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Completion of number generation.
    The function returns the new id number.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef UINT ts_uniq_num () 
#else
publicdef UINT ts_uniq_num ()
#endif
    {
    static UINT old_id = 0, count = 0;
    UINT result;
    time_t xid;
    
    /* this routine attempts to generate a reasonably unique id number
     * by first getting the number of seconds since this routine was 
     * written and multipling it by 100.  If this number is not the same
     * as the last call to this routine it is return.  If it is the same
     * as the last call an index number will be added to it.  If the
     * index number becomes greater than 99 the routine will enter a loop
     * waiting for a new second.
     */
    
    time (&xid);
    result = (xid - WRITTING_DATE) * 100;
    
    if (result != old_id)
        {
        old_id = result;
        count = 0;
        }
    else
        {
        if (count < 99)
            result += ++count;
        else
            {
            while (result == old_id)
                {                   /* wait for a new second to click over */
                time (&xid);
                result = (xid - WRITTING_DATE) * 100;
                }
            old_id = result;        /* this is a new second; reset the statics */
            count = 0;
            }
        }
    return result;
    }
/* END:     */
