/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_TRC.C*/
/* *2    17-AUG-1990 22:39:39 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:15 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_TRC.C*/
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

#ifndef ESI_XS_DEFS_H

#include "esi_xs_defs.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_TC_C

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Frees the trace, trace by trace, then frees linked list.

Prototype:
    publicdef INT xs_free_curves(LISTHEAD *trace_items);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) trace_items -(LISTHEAD *) Pointer to trace list head.

Return Value/Status:
    SUCCESS - Successful completion.
    XS_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_free_curves (LISTHEAD *trace_items)
#else
publicdef INT xs_free_curves (trace_items)
LISTHEAD *trace_items;
#endif
    {
    INT totalItems = 0;
    INT i = 0;
    LP_TRACE_STRUCT *data;
    
    /* error checking.            */
    
    if (trace_items EQUALS (LISTHEAD *)NULL)
        return XS_INVALID_POINTER;
    
    /* if there is no items, returns error*/
    
    totalItems = llcount (trace_items);
    if (totalItems <= 0)
        return SUCCESS;
    
    for (i = 0, llfirst (trace_items, &data); i < totalItems; i++)
        {
        /* free the N-LIST first.            */
        
        lg_ltdm_free_trace (&(data->trace_struct));
        tc_free (data);
        llnext (trace_items, &data);
        }
    /* then frees the linked list and returns
                           NULL pointer.                    */
    
    llfree (trace_items);
    trace_items = (LISTHEAD *)NULL;
    return(INT)trace_items;
    }
/* END:     */
