/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_SP.C */
/* *4    17-AUG-1990 22:18:54 VINCE "(SPR 5644) Code Cleanup" */
/* *3    18-JUN-1990 12:10:02 GILLESPIE "(SPR 1) Changes to allow 3-D lines to be handles as select list entryes (IES deltas)" */
/* *2     1-NOV-1989 14:16:05 JULIAN "(SPR 30) splitting seism_tools.pc" */
/* *1     1-NOV-1989 14:01:33 JULIAN "seism_tools" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_XY_SP.C */
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

#include "esi_se.h"

/* Function Description -----------------------------------------------------
Description:
    For the seis_line (structure storing
    line_name + survey ) this function returns
    the x and y coordinates of the cdp cdp .

Prototype:
    publicdef INT se_xy_from_sp (FULL_LINE_NAME *seis_line, SP sp,
        COORD *x, COORD *y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    seis_line       -(FULL_LINE_NAME *)
    sp              -(SP)
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
publicdef INT se_xy_from_sp (FULL_LINE_NAME *seis_line, SP sp, COORD *x, COORD *y)
#else
publicdef INT se_xy_from_sp (seis_line, sp, x, y)
FULL_LINE_NAME *seis_line;
SP sp;
COORD *x, *y;
#endif
    {
    INT status;
    CDP cdp;
    CDP_FLOAT inter_cdp;
    
    /* set inter_cdp distance to - 1 for not using
       it in the computation                       */
    
    inter_cdp = -1.;
    
    /* first research cdp corresponding to sp      */
    
    status = se_cdp_from_sp (seis_line, sp, &cdp);
    
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* research coordinates corresponding to cdp   */
    
    return se_xy_from_cdp (seis_line, cdp, inter_cdp, x, y);
    }
/* END:     */
