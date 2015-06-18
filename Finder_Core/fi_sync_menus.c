/* DEC/CMS REPLACEMENT HISTORY, Element FI_SYNC_MENUS.C*/
/* *4    14-AUG-1990 13:23:59 VINCE "(SPR 5644) Header Standardization"*/
/* *3    27-NOV-1989 15:40:59 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *2    18-NOV-1989 09:48:38 HALL "(SPR 5075) Global overhaul"*/
/* *1    19-JUN-1989 12:50:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_SYNC_MENUS.C*/
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

#ifndef ESI_FI_INT_H

#include "esi_fi_int.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    FI_SYNC_MN removes all of the Finder menus from the menu bar which
    are lower, in the menu hierarchy (and thus have greater indices into
    the finder_menus array) than the one specified by the "index"
    parameter. The menu specified by "index" is made visible if it is not
    already visible.

Prototype:
    publicdef INT fi_sync_mn(INT indx);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) indx        -(INT) Index into finder_menus array.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_sync_mn (INT indx) 
#else
publicdef INT fi_sync_mn (indx)
INT indx;
#endif
    {
    PROGNAME ("fi_sync_mn");
    FINDER_MENU_STRUCT *p;
    char menu_name[256], gl_str[256], fi_str[256];
    
    /* initialize local strings */
    strcpy (gl_str, "GLOBAL");
    strcpy (fi_str, "FINDER");
    
    /* Check the caller's "menu index" against every menu's index, */
    /* and make any thing that matches visible, and anything that  */
    /* is subordinate invisible (also, don't overlook the GLOBAL   */
    /* and FINDER menus, which should NEVER be made invisible).    */
    
    for (p = fiz_finder_menus (); p->id != NULL_MENU_ID; p += p->item)
        {
        if (p->id EQUALS indx AND p->flag EQUALS OFF)
            {                       /* indicate that it is visible. */
            wivismn (indx, p->flag = ON);
            }
        else if (p->id > indx AND p->flag EQUALS ON)
            {                       /* make all menus which are subordinate invisible */
            wivismn (p->id, p->flag = OFF);
            }
        /* get a local copy (UPPER case) of the menu's name */
        strcpy (menu_name, p->Name);
        ts_sto_upper (menu_name);
        
        /* make sure the GLOBAL menu wasn't made invisible */
        if (ARE_SAME (menu_name, gl_str))
            {                       /* the GLOBAL menu is ALWAYS visible */
            wivismn (p->id, p->flag = ON);
            }
        /* make sure the FINDER menu wasn't made invisible */
        if (ARE_SAME (menu_name, fi_str))
            {                       /* the FINDER menu is ALWAYS visible */
            wivismn (p->id, p->flag = ON);
            }
        }
    return SUCCESS;
    }
/* END:     */
