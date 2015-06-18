/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ST_TOPS_V.C*/
/* *2    17-AUG-1990 22:36:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_ST_TOPS_V.C*/
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

#include "esi_c_lib.h"
#include "esi_xs_defs.h"

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This module includes initial and server routines for operating
    tops; An internal help routine to set the visibilities of TOPS 
    in the TOP_TABLE.

Prototype:
    publicdef INT xsz_set_tops_vis(XS_STRUCT *pxs, INT total_tops, CHAR **tops, 
        INT selected_list[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) pxs             -(XS_STRUCT *) A pointer points to the XS_STRUCT.
    (I) total_tops      -(INT) The total number of tops should be ON.
    (I) tops            -(CHAR **) The table of tops name.
    (I) selected_list   -(INT []) The order of tops in the TOPS table.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_set_tops_vis (XS_STRUCT *pxs, INT total_tops, CHAR **tops,
                                INT selected_list[])
#else
publicdef INT xsz_set_tops_vis (pxs, total_tops, tops, selected_list)
XS_STRUCT *pxs;
CHAR **tops;
INT total_tops, selected_list[];
#endif
    {
    INT status = SUCCESS;
    INT total_tops_in_table = 0;
    INT i, j;
    INT found = FALSE;
    CHAR **ptr;
    TOP_INFO *top_info;
    /* ************************************************************************* */
    
    total_tops_in_table = llcount (&(pxs->drawing.top_table));
    if (total_tops_in_table EQUALS 0)
        goto rtnpt;
    for (i = 0, llfirst (&(pxs->drawing.top_table), &top_info);
         i < total_tops_in_table; i++, llnext (&(pxs->drawing.top_table), &top_info))
        {
        found = FALSE;
        j = 0;
        while (!found AND j < total_tops)
            {
            if (!strcmp (top_info->top, tops[selected_list[j++] - 1]))
                found = TRUE;
            }
        top_info->visible = found;
        }
rtnpt:
    return status;
    }
/* END:     */
