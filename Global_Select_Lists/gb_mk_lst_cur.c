/* DEC/CMS REPLACEMENT HISTORY, Element GB_MK_LST_CUR.C*/
/* *2    14-AUG-1990 13:26:40 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:51:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_MK_LST_CUR.C*/
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

#include "esi_gb.h"

/* Function Description -----------------------------------------------------
Description:
    This routine takes an existing select list and makes it current. 
    If the select list is empty it calls GB_CLEAR_CURRENT_LIST to set a
    state of no-current-list.                                              

    The data_type and nlist name is removed from the structure and the
    nlist name is set into appropriate data type qs variable. The nlist
    is then written to disk so it can be retrieved later.  

Prototype:
    publicdef INT  gb_make_list_current (NLIST_HEADER select_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    select_list     -(NLIST_HEADER) Nlist with user structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_make_list_current (NLIST_HEADER select_list)
#else
publicdef INT gb_make_list_current (select_list)
NLIST_HEADER select_list;
#endif
    {
    INT status = SUCCESS;
    /* update the current status of GB.*/
    
    status = gb_update_current_status (select_list);
    status = sl_save_list (select_list, 2);
    
    return(status);
    }
/* END:     */
