/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_TRACE.C */
/* *3    17-AUG-1990 22:18:58 VINCE "(SPR 5644) Code Cleanup" */
/* *2     1-NOV-1989 14:17:09 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 14:01:57 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_TRACE.C */
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
    For the line this function returns the x and y coordinates of the cdp.

Prototype:
    publicdef INT se_xy_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace, CDP_FLOAT inter_cdp,
        COORD *x, COORD *y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line_str        -(TRACE_ATTRIBUTE *)
    trace           -(TRACE)
    inter_cdp       -(CDP_FLOAT) Inter_cdp is the distance between internal
                        cdp deduced from the distance between
                        traces given in the trace files if = -1 we
                        used the distance given by the line
                        position.
    x               -(COORD *)
    y               -(COORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT se_xy_from_trace (TRACE_ATTRIBUTE *line_str, TRACE trace,
                                CDP_FLOAT inter_cdp, COORD *x, COORD *y)
#else
publicdef INT se_xy_from_trace (line_str, trace, inter_cdp, x, y)
TRACE_ATTRIBUTE *line_str;
TRACE trace;
CDP_FLOAT inter_cdp;
COORD *x, *y;
#endif
    {
    INT status;
    CDP cdp;
    
    /* first research cdp corresponding to trace   */
    
    status = se_cdp_from_trace (line_str, trace, &cdp);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* research coordinates corresponding to cdp   */
    
    return se_xy_from_cdp (&line_str->line, cdp, inter_cdp, x, y);
    }
/* END:     */
