/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_LS_STUP.C*/
/* *3    17-AUG-1990 22:22:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:13:36 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_LS_STUP.C*/
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
    Set up line selection menu.

Prototype:
    publicdef INT sx_menu_ls_setup(SX_STRUCT *psx);

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
publicdef INT sx_menu_ls_setup (SX_STRUCT *psx)
#else
publicdef INT sx_menu_ls_setup (psx)
SX_STRUCT *psx;
#endif
    {
    INT flag;
    INT nlines;
    INT status = SUCCESS;
    
    nlines = psx->nlines;
    if (psx->new_line AND psx->need_to_save)
        nlines--;
    
    flag = (nlines) ? ON : OFF;
    wienmni (psx->menu_id.ls, ITEM_LS_GRAPHIC_PICKING, flag);
    
    flag = (psx->line_selected) ? ON : OFF;
    wienmni (psx->menu_id.ls, ITEM_LS_DELETE, flag);
    
    wienmni (psx->menu_id.ls, ITEM_LS_MANUAL_ENTRY, ON);
    
    flag = (psx->need_to_save) ? ON : OFF;
    wienmni (psx->menu_id.ls, ITEM_LS_SAVE, flag);
    
    flag = (psx->nshots > 0) ? ON : OFF;
    wienmni (psx->menu_id.ls, ITEM_LS_REDRAW, flag);
    wienmni (psx->menu_id.ls, ITEM_LS_QUIT, ON);
    wivismn (psx->menu_id.ls, ON);
    SX_RETURN;
    }
/* END:     */
