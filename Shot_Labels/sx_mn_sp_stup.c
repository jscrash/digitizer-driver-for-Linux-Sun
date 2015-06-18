/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_SP_STUP.C*/
/* *3    17-AUG-1990 22:22:12 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:13:43 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_SP_STUP.C*/
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

#include "esi_sx.h"
#include "sx_menus.rh"
#include "esi_wi.h"

/* Function Description -----------------------------------------------------
Description:
    Set up shot point position menu.

Prototype:
    publicdef INT sx_menu_sp_setup(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_menu_sp_setup (SX_STRUCT *psx)
#else
publicdef INT sx_menu_sp_setup (psx)
SX_STRUCT *psx;
#endif
    {
    INT flag;
    INT status;
    
    /* assume success */
    
    status = SUCCESS;
    
    flag = (psx->line_selected) ? ON : OFF;
    
    wienmni (psx->menu_id.sp, ITEM_SP_ADD, flag);
    
    flag = psx->line_selected && psx->nshots;
    
    wienmni (psx->menu_id.sp, ITEM_SP_MOVE, flag);
    wienmni (psx->menu_id.sp, ITEM_SP_DELETE, flag);
    wienmni (psx->menu_id.sp, ITEM_SP_CHANGE, flag);
    /* for now connect is dimmed and will stay dimmed until code written
       or the menu item is removed from the sx_menus.rs */
    /*    wienmni(psx->menu_id.sp,ITEM_SP_CONNECT,flag); */
    wivismn (psx->menu_id.sp, ON);
    SX_RETURN;
    }
/* END:     */
