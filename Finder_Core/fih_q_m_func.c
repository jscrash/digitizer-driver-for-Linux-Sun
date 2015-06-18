/* DEC/CMS REPLACEMENT HISTORY, Element FIH_Q_M_FUNC.C */
/* *4    14-AUG-1990 13:12:23 VINCE "(SPR 5644) Header Standardization" */
/* *3    27-NOV-1989 15:38:02 GORDON "(SPR 0) Use NULL_MENU_ID, not 0" */
/* *2    27-NOV-1989 11:14:25 GORDON "(SPR 0) just moving it over from include directory" */
/* *1    27-NOV-1989 11:12:55 GORDON "Just moving it from include directory" */
/* DEC/CMS REPLACEMENT HISTORY, Element FIH_Q_M_FUNC.C */
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

#include "esi_fi_int.h"

#ifndef ESI_AM_INT_H

#include "esi_am_int.h"

#endif

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Given a pointer to a function, FIH_QUERY_MENU_FUNCTION will search
    through the FINDER menus, and attempt to locate a menu item whose
    "application entry point" field matches that address (naturally, only
    those menu items of type 3 or 4 are checked).  If a match is found,
    that menu item's ID and ITEM values will be returned to the caller's 
    pointers and SUCCESS is returned; otherwise, FAIL is returned.

Prototype:
    publicdef INT fih_query_menu_function (INT *menuid, INT * itemid, INT (*function)());
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) menuid      -(INT *) Pointer to a place to store the menu's ID.
    (M) itemid      -(INT *) Pointer to a place to store the menu's ITEM.
    (I) function    -(INT (*)()) Pointer to the function whose menu item is to be found.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fih_query_menu_function (INT *menuid, INT *itemid, INT (*function)()) 
#else
publicdef INT fih_query_menu_function (menuid, itemid, function)
INT *menuid, *itemid;
INT (*function)();
#endif
    {
    FINDER_MENU_STRUCT *m;
    
    for (m = fiz_finder_menus (); m->id != NULL_MENU_ID; m++)
        {
        if ((m->Type == EventsFunction) || (m->Type == NoEventsFunction))
            {
            if (m->Activates.AppEntry == function)
                {                   /* function found - return MENU/ITEM to caller's ptr */
                *menuid = m->id;
                *itemid = m->item;
                return SUCCESS;
                }
            }
        }
    return FAIL;                    /* FUNCTION not found in FINDER menus */
    }
/* END:     */
