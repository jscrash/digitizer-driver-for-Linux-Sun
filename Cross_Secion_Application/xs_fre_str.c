/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_STR.C*/
/* *2    17-AUG-1990 22:39:34 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_STR.C*/
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

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Supervises freeing memory (linked lists in the draw structure.

Prototype:
    publicdef INT xs_free_struct(LISTHEAD *log_plots);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) log_plots   -(LISTHEAD *) Pointer to track list head.

Return Value/Status:
    SUCCESS - Successful completion.
    XS_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_free_struct (LISTHEAD *log_plots)
#else
publicdef INT xs_free_struct (log_plots)
LISTHEAD *log_plots;
#endif
    {
    INT i, totalItems = 0;
    LAYOUT_STRUCT *data;
    
    /* error checking.                    */
    
    if (log_plots EQUALS (LISTHEAD *)NULL)
        return XS_INVALID_POINTER;
    
    /* if there is no items, returns error*/
    
    totalItems = llcount (log_plots);
    if (totalItems <= 0)
        return XS_INVALID_POINTER;
    
    for (i = 0, llfirst (log_plots, &data); i < totalItems; i++)
        {
        /* otherwise, go througth all items and
           frees the character string.       */
        
        /* first frees header items.         */
        
        if (data->log_hdr.total_items > 0)
            tc_free (data->log_hdr.items);
        
        xs_free_hdr (&(data->log_hdr.hdr_items));
        
        /* second frees linked list of tracks*/
        
        xs_free_track (&(data->tracks));
        
        /* third, frees linked list of traces*/
        
        xs_free_curves (&(data->traces));
        
        tc_free (data);
        llnext (log_plots, &data);
        }
    /* then frees the linked list and returns
                           NULL pointer.                    */
    
    llfree (log_plots);
    log_plots = (LISTHEAD *)NULL;
    return(INT)log_plots;
    }
/* END:     */
