/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_HDR.C*/
/* *2    17-AUG-1990 22:39:28 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:44:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_FRE_HDR.C*/
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

#ifndef ESI_XS_ERR_H

#include "esi_xs_err.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Frees the linked list of header items within a logplot drawing.

Prototype:
    publicdef INT xs_free_hdr(LISTHEAD *hdr_items);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) hdr_items   -(LISTHEAD *) Pointer to hdr_items list head.

Return Value/Status:
    SUCCESS - Successful completion.
    XS_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_free_hdr (LISTHEAD *hdr_items)
#else
publicdef INT xs_free_hdr (hdr_items)
LISTHEAD *hdr_items;
#endif
    {
    INT totalItems = 0;
    INT i;
    CHAR *data;
    
    /* error checking.                    */
    if (hdr_items EQUALS (LISTHEAD *)NULL)
        return XS_INVALID_POINTER;
    
    /* if there is no items, returns error*/
    
    totalItems = llcount (hdr_items);
    if (totalItems <= 0)
        return SUCCESS;
    /* otherwise, go througth all items and
       frees the character string.       */
    for (i = 0, llfirst (hdr_items, &data); i < totalItems; i++)
        {
        tc_free (data);
        llnext (hdr_items, &data);
        }
    /* then frees the linked list and returns
                           NULL pointer.                    */
    
    llfree (hdr_items);
    hdr_items = (LISTHEAD *)NULL;
    return(INT)hdr_items;
    }
/* END:     */
