/* DEC/CMS REPLACEMENT HISTORY, Element CT_GET_NLIST.C*/
/* *4     6-AUG-1991 14:50:20 KEE "(SPR 8779) Fix nlist file locking problem (case 3490)"*/
/* *3    14-AUG-1990 13:06:34 VINCE "(SPR 5644) Header Standardization"*/
/* *2    28-FEB-1990 14:34:24 GILLESPIE "(SPR 6011) Open in exclusive mode"*/
/* *1    19-JUN-1989 12:45:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_GET_NLIST.C*/
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

#include "esi_ct.h"

#ifndef ESI_CT_MSG_H

#include "esi_ct_msg.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Retrieve an n-list and convert it to the MAPDEF projection.

Prototype:
    publicdef INT ct_get_nlist(NLIST_HEADER *nlist, FILENAME nlist_name, 
                           NLIST_ID nlist_id, UINT dim_list[], DOUBLE tolerance);
                           
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) nlist       -(NLIST_HEADER *) Pointer to returned n-list.
    (I) nlist_name  -(FILENAME) N-list file name.
    (I) nlist_id    -(NLIST_ID) N-list ID.
    (I) dim_list    -(UINT []) Array of dimension numbers.
    (I) tolerance   -(DOUBLE) Line conversion minimum arc.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_get_nlist (NLIST_HEADER *nlist, FILENAME nlist_name,
                            NLIST_ID nlist_id, UINT dim_list[], DOUBLE tolerance) 
#else
publicdef INT ct_get_nlist (nlist, nlist_name, nlist_id, dim_list, tolerance)
NLIST_HEADER *nlist;
FILENAME nlist_name;
NLIST_ID nlist_id;
UINT dim_list[];
DOUBLE tolerance;
#endif
    {
    INT status;
    NLIST_HEADER nl_dummy;
    
    /************************************************************************/
    
    /* * READ THE NLIST * */
    /* ------------------ */
    
    if ((status = nl_read_nlist (nlist, nlist_name, nlist_id)) >= 0)
        {
        switch (status = ct_map_nlist_xy (*nlist, &nl_dummy, dim_list, tolerance))
            {
        case SUCCESS:
            nl_free_nlist (*nlist);
            *nlist = nl_dummy;
            break;
            
        case CT_SAME_PROJECTIONS:
            status = SUCCESS;
            break;
            }
        }
    return status;
    }
/* END:     */
