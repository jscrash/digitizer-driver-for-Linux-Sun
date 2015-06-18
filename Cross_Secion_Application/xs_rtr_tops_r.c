/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TOPS_R.C*/
/* *2    17-AUG-1990 22:43:35 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:45:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_RTR_TOPS_R.C*/
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

#include "esi_gl_defs.h"
#include "esi_xs_defs.h"
#include "esi_ll.h"

/* Function Description -----------------------------------------------------
Description:
    This module includes initial and server routines for operating
    tops.

    To retrieve the existing tops for the current WELL_LIST, when
    user call CREATE DRAWING.

Prototype:
    publicdef INT xs_rtr_tops_when_redraw (XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_rtr_tops_when_redraw (XS_STRUCT *pxs)
#else
publicdef INT xs_rtr_tops_when_redraw (pxs)
XS_STRUCT *pxs;
#endif
    {
    PROGNAME ("xs_rtr_tops_when_redraw");
    INT status = SUCCESS;
    INT initial;
    INT total_exist_tops = 0;
    INT i;
    TOP_INFO *top_info;
    
    total_exist_tops = llcount (&(pxs->drawing.top_table));
    initial = (total_exist_tops > 0) ? FALSE : TRUE;
    for (i = 0, llfirst (&(pxs->drawing.top_table), &top_info); i < total_exist_tops;
         i++, llnext (&(pxs->drawing.top_table), &top_info))
        {
        status = xsz_rtr_wells_tops (pxs, top_info->top, 1, 1, 1.0, initial);
        initial = FALSE;
        }
    return SUCCESS;
    }
/* END:     */
