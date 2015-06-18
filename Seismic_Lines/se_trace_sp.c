/* DEC/CMS REPLACEMENT HISTORY, Element SE_TRACE_SP.C */
/* *3    17-AUG-1990 22:18:31 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:13:31 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 14:00:22 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_TRACE_SP.C */
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
    this function returns the trace number
    corresponding to a given sp.

Prototype:
    publicdef INT se_trace_from_sp (TRACE_ATTRIBUTE *line_str, SP sp, TRACE *trace);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_str        -(TRACE_ATTRIBUTE *)
    sp              -(SP)
    trace           -(TRACE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_trace_from_sp (TRACE_ATTRIBUTE *line_str, SP sp, TRACE *trace)
#else
publicdef INT se_trace_from_sp (line_str, sp, trace)
TRACE_ATTRIBUTE *line_str;
SP sp;
TRACE *trace;
#endif
    {
    CDP cdp;
    INT status;
    
    status = se_cdp_from_sp (&line_str->line, sp, &cdp);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    return se_trace_from_cdp (line_str, cdp, trace);
    }
/* END:     */
