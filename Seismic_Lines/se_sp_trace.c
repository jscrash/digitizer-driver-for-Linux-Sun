/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_TRACE.C */
/* *3    17-AUG-1990 22:18:14 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:11:40 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:57:10 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_SP_TRACE.C */
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

#include "esi_c_lib.h"
#include "esi_se.h"

/* Function Description -----------------------------------------------------
Description:
    For the trace file described by line_str
    this function returns the sp number
    corresponding to a given trace.

Prototype:
    publicdef INT se_sp_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace, SP *sp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_str        -(TRACE_ATTRIBUTE *)
    trace           -(TRACE)
    sp              -(SP *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_sp_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace, SP *sp)
#else
publicdef INT se_sp_from_trace (line_str, trace, sp)
TRACE_ATTRIBUTE *line_str;
TRACE trace;
SP *sp;
#endif
    {
    CDP cdp;
    INT status;
    
    status = se_cdp_from_trace (line_str, trace, &cdp);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    return se_sp_from_cdp (&line_str->line, cdp, sp);
    }
/* END:     */
