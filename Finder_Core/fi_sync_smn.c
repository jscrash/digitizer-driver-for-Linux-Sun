/* DEC/CMS REPLACEMENT HISTORY, Element FI_SYNC_SMN.C*/
/* *3    14-AUG-1990 13:24:04 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-NOV-1989 15:41:23 GORDON "(SPR 0) Use NULL_MENU_ID, not 0"*/
/* *1    19-JUN-1989 12:50:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_SYNC_SMN.C*/
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
    FI_SYNC_MN makes all menus in the Finder menu hierarchy which are
    below the 1st specified menu index invisible. FI_SYNC_SMN then makes
    the sub menu, specified as the 2nd menu index parameter, visible.

Prototype:
    publicdef INT fi_sync_smn(INT menu1, INT menu2);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) menu1       -(INT) First specified menu.
    (I) menu2       -(INT) Second visible menu.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_sync_smn (INT menu1, INT menu2) 
#else
publicdef INT fi_sync_smn (menu1, menu2)
INT menu1;
INT menu2;
#endif
    {
    FINDER_MENU_STRUCT *pa;
    
    /* ********************************************************************** */
    
    /*  COM BY JJC 02-NOC-1988 In order to keep the parent menus on the
        screen
        fi_sync_mn(menu1); */
    
    for (pa = fiz_finder_menus (); pa->id != NULL_MENU_ID; pa += pa->item)
        {
        if (pa->id EQUALS menu2 AND pa->flag EQUALS OFF)
            {
            wivismn (menu2, pa->flag = ON);
            }
        }
    return SUCCESS;
    }
/* END:     */
