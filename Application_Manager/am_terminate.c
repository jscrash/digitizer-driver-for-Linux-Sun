/* DEC/CMS REPLACEMENT HISTORY, Element AM_TERMINATE.C*/
/* *2    14-AUG-1990 11:41:25 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_TERMINATE.C*/
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

#include "esi_am_int.h"
#include "esi_wi.h"

/* Function Description -----------------------------------------------------
Description:
    AM_TERMINATE terminates all active applications, and then the
    application manager itself.

Prototype:
    publicdef INT am_terminate();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful termination.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_terminate () 
#else
publicdef INT am_terminate ()
#endif
    {
    RESOURCE_LIST rsrc;             /* resource node storage */
    INT rsrc_id;                    /* key of entry in rsrc */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    /*   Release all menus and custom
         dialogs. */
    
    while (btcount (&application_static->resource_list) > 0)
        {
        btfirst (&application_static->resource_list, (VOIDPTR) & rsrc_id,
                 (VOIDPTR) & rsrc);
        if (rsrc.type == AM_MENU)
            wirlsmn (rsrc_id);
        else
            wirlscd (rsrc_id);
        btdel (&application_static->resource_list, (VOIDPTR) & rsrc_id);
        }
    /*   Free RESOURCE_LIST structure. */
    
    btfree (&application_static->resource_list);
    application_static->initialized = FALSE;
    
    return SUCCESS;
    }
/* END:     */
