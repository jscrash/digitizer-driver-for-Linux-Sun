/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_TRACE.C */
/* *3    17-AUG-1990 22:16:01 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:04:48 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:44:44 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_CDP_TRACE.C */
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
    this function gives the cdp corresponding
    to the given trace.

Prototype:
    publicdef INT se_cdp_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace, CDP *cdp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_str        -(TRACE_ATTRIBUTE *)
    trace           -(TRACE)
    cdp             -(CDP *)

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns status from se_retrieve_trace.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_cdp_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace, CDP *cdp)
#else
publicdef INT se_cdp_from_trace (line_str, trace, cdp)
TRACE_ATTRIBUTE *line_str;
TRACE trace;
CDP *cdp;
#endif
    {
    INT status;
    TRACE_HDR *p_trace;
    
    /* look for the pointer to the structure
       describing the trace header                 */
    
    status = se_retrieve_trace (line_str, &p_trace);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* we now perform the computation using the
       structure informations                      */
    
    *cdp = p_trace->init_cdp + (trace - p_trace->init_trace)
    *p_trace->cdp_per_trace;
    
    return SUCCESS;
    }
/* END:     */
