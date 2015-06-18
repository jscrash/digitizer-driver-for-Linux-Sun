/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_TRK.C*/
/* *2    17-AUG-1990 22:39:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_TRK.C*/
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

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Frees the track, track by track, then frees linked list.

Prototype:
    publicdef INT xs_free_track(LISTHEAD *track_items);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) track_items -(LISTHEAD *) Pointer to track list head.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_free_track (LISTHEAD *track_items)
#else
publicdef INT xs_free_track (track_items)
LISTHEAD *track_items;
#endif
    {
    INT totalItems = 0;
    INT i;
    TRACK_STRUCT *data;
    
    /* error checking.                    */
    if (track_items EQUALS (LISTHEAD *)NULL)
        return XS_INVALID_POINTER;
    
    /* if there is no items, returns error*/
    
    totalItems = llcount (track_items);
    
    if (totalItems <= 0)
        return SUCCESS;
    
    for (i = 0, llfirst (track_items, &data); i < totalItems; i++)
        {
        /* otherwise, go througth all items and
           frees the character string.       */
        /* later has to free the SHADING linked
           list.                             */
        
        tc_free (data);
        llnext (track_items, &data);
        }
    /* then frees the linked list and returns
                           NULL pointer.                    */
    
    llfree (track_items);
    track_items = (LISTHEAD *)NULL;
    return(INT)track_items;
    }
/* END:     */
