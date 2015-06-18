/* DEC/CMS REPLACEMENT HISTORY, Element SE_TRACE_CDP.C */
/* *3    17-AUG-1990 22:18:27 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:12:51 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 13:59:22 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_TRACE_CDP.C */
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

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    For the trace file described by line_str
    this function gives the trace number
    corresponding to the given cdp.

Prototype:
    publicdef INT se_trace_from_cdp (TRACE_ATTRIBUTE *line_str, CDP cdp, TRACE *trace);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_str        -(TRACE_ATTRIBUTE *)
    cdp             -(CDP)
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
publicdef INT se_trace_from_cdp (TRACE_ATTRIBUTE *line_str, CDP cdp, TRACE *trace)
#else
publicdef INT se_trace_from_cdp (line_str, cdp, trace)
TRACE_ATTRIBUTE *line_str;
CDP cdp;
TRACE *trace;
#endif
    {
    DOUBLE trac1;
    DOUBLE nb;
    INT status;
    TRACE_HDR *p_trace;
    
    /* loo for the pointer to the structure
       describing the trace header                 */
    
    status = se_retrieve_trace (line_str, &p_trace);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* we now perform the computation using the
       structure informations                      */
    
    trac1 = p_trace->init_trace + (cdp - p_trace->init_cdp) / p_trace->cdp_per_trace;
    
    /* look for the closest trace existing on the
       line we need to use the trace increment     */
    
    nb = floor (trac1);
    if (fabs (trac1 - nb) < 0.5)
        {
        *trace = nb;
        }
    else
        {
        *trace = nb + 1;
        }
    return SUCCESS;
    
    }
/* END:     */
