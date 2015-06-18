/* DEC/CMS REPLACEMENT HISTORY, Element FI_EN_MENU.C*/
/* *3    14-AUG-1990 13:23:32 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-NOV-1989 15:38:30 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *1    19-JUN-1989 12:49:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_EN_MENU.C*/
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

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Look up the menu entry that initiated this entry point - set the
    menu item to Enabled.

Prototype:
    publicdef INT fi_enable_menu(INT (*entry_point)());
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    entry_point     -(INT (*)())
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_enable_menu (INT (*entry_point)()) 
#else
publicdef INT fi_enable_menu (entry_point)
INT (*entry_point)();
#endif
    {
    FINDER_MENU_STRUCT *pm;
    INT status;
    
    /*  loop through each entry in the externally
        defined menu structure find the one that
        matches this entry point (but NEVER enable
        non-executable entry points)            */
    
    if (entry_point != NULL_FUNCTION_PTR)
        {
        for (pm = fiz_finder_menus (); pm->id != NULL_MENU_ID; pm++)
            {
            if (pm->Activates.AppEntry EQUALS entry_point)
                {
                if (pm->flag EQUALS OFF)
                    {
                    wi_enable_menu_item (pm->id, pm->item, pm->flag = ON);
                    }
                break;
                }
            }
        }
    return SUCCESS;
    }
/* END:     */
