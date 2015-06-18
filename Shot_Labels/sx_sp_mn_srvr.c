/* DEC/CMS REPLACEMENT HISTORY, Element SX_SP_MN_SRVR.C*/
/* *5    17-AUG-1990 22:22:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    14-SEP-1989 18:39:10 GORDON "(SPR -1) checkin from gulf"*/
/* *3     6-SEP-1989 09:44:26 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:15:12 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SP_MN_SRVR.C*/
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
#include "sx_cdlgs.rh"
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Shot point position menu server.

Prototype:
    publicdef INT sx_sp_menu_srvr(INT menuid,INT item,SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    menuid          -(INT)
    item            -(INT)
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
publicdef INT sx_sp_menu_srvr (INT menuid, INT item, SX_STRUCT *psx)
#else
publicdef INT sx_sp_menu_srvr (menuid, item, psx)
INT menuid;
INT item;
SX_STRUCT *psx;
#endif
    {
    INT status;
    
    /* assume success */
    
    status = SUCCESS;
    
    switch (item)
        {
        
        /* add requires a dialog box first */
        
    case ITEM_SP_ADD:
        am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_SPO, sx_dialog_spo_srvr,
                            &psx->cdlg_id.dialog_spo, psx);
        sx_dialog_spo_setup (psx);
        wiopcd (psx->cdlg_id.dialog_spo);
        wienmni (psx->menu_id.sp, ITEM_SP_ADD, OFF);
        break;
        
        /* move, delete, change can just go at it */
        
    case ITEM_SP_MOVE:
    case ITEM_SP_DELETE:
    case ITEM_SP_CHANGE:
        sx_process_shotpoints (psx, item);
        break;
        
    case ITEM_SP_CONNECT:
        break;
        
    default:
        break;
        }
    SX_RETURN;
    }
/* END:     */
