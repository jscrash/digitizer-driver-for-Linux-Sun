/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_TOPS_TCP.C*/
/* *3    18-JUL-1991 17:06:38 JANTINA "(SPR 0) Check for total_top containing negative error code"*/
/* *2    17-AUG-1990 22:37:02 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_TOPS_TCP.C*/
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

#include "esi_xs_defs.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This module includes initial and server routines for operating
    tops; A internal help routine to create a TCP for the TOP_TABLE.

Prototype:
    publicdef INT xsz_make_tops_tcp(XS_STRUCT *pxs, CHAR ***tcp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) A pointer points to the XS_STRUCT.
    (O) tcp         -(CHAR ***) A pointer points to the address of TCP.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_make_tops_tcp (XS_STRUCT *pxs, CHAR ***tcp)
#else
publicdef INT xsz_make_tops_tcp (pxs, tcp)
XS_STRUCT *pxs;
CHAR ***tcp;
#endif
    {
    INT status = SUCCESS;
    INT total_tops = 0;
    INT i;
    CHAR **ptr;
    TOP_INFO *top_info;
    /* ************************************************************************* */
    
    total_tops = llcount (&(pxs->drawing.top_table));
    if(total_tops < 0) total_tops = 0;
    *tcp = ptr = (CHAR **)tc_zalloc (sizeof(CHAR *) * (total_tops + 1));
    if (total_tops EQUALS 0)
        goto rtnpt;
    for (i = 0, llfirst (&(pxs->drawing.top_table), &top_info); i < total_tops;
         i++, llnext (&(pxs->drawing.top_table), &top_info))
        {
        ptr[i] = (CHAR *)tc_zalloc (strlen (top_info->top) + 1);
        strcpy (ptr[i], top_info->top);
        }
rtnpt:
    return status;
    }
/* END:     */
