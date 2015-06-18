/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSPOSRVR.C*/
/* *3    17-AUG-1990 22:20:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:11:35 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGSPOSRVR.C*/
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
#include "sx_cdlgs.rh"
#include "sx_menus.rh"
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Server for shotpoint entry dialog box.

Prototype:
    publicdef INT sx_dialog_spo_srvr(INT id,INT item,SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
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
publicdef INT sx_dialog_spo_srvr (INT id, INT item, SX_STRUCT *psx)
#else
publicdef INT sx_dialog_spo_srvr (id, item, psx)
INT id;
INT item;
SX_STRUCT *psx;
#endif
    {
    INT status;
    CHAR text[128];
    INT ivalue;
    FLOAT fvalue;
    
    status = SUCCESS;
    
    /* process dialog box event */
    
    switch (item)
        {
        
    case ITEM_SPO_OK:
        status = sx_dialog_spo_read (psx);
        if (status == SUCCESS)
            {
            sx_process_shotpoints (psx, ITEM_SP_ADD);
            wienmni (psx->menu_id.sp, ITEM_SP_ADD, ON);
            am_release_resource (id);
            }
        break;
        
        /*      case ITEM_SPO_HELP:  JRB 12/9/87     */
        /*      break;               JRB 12/9/87     */
        
    case ITEM_SPO_CANCEL:
        am_release_resource (id);
        wienmni (psx->menu_id.sp, ITEM_SP_ADD, ON);
        /*      status = FAIL;       JRB 12/9/87     */
        break;
        
    case ITEM_SPO_LABEL_ENTRY_AUTO:
        psx->dialog_spo.add_type = SX_AUTO;
        sx_dialog_spo_setup (psx);
        break;
        
    case ITEM_SPO_LABEL_ENTRY_MANUAL:
        psx->dialog_spo.add_type = SX_MANUAL;
        sx_dialog_spo_setup (psx);
        break;
        
    case ITEM_SPO_LABEL_ORDER_ASCEND:
        psx->dialog_spo.shot_incr_factor = SX_ASCEND;
        break;
        
    case ITEM_SPO_LABEL_ORDER_DESCEND:
        psx->dialog_spo.shot_incr_factor = SX_DESCEND;
        break;
        
    case ITEM_SPO_SHOT_SPAC_MANUAL:
        psx->dialog_spo.shot_spacing_method = SX_MANUAL;
        wiencdi (id, ITEM_SPO_SHOT_SPAC_EDIT, OFF);
        break;
        
    case ITEM_SPO_SHOT_SPAC_AUTO:
        psx->dialog_spo.shot_spacing_method = SX_AUTO;
        wiencdi (id, ITEM_SPO_SHOT_SPAC_EDIT, ON);
        break;
        
    case ITEM_SPO_SHOT_LABELS_NUMERIC:
        psx->dialog_spo.shot_label_type = SX_NUMERIC;
        break;
        
    case ITEM_SPO_SHOT_LABELS_ARBITRARY:
        psx->dialog_spo.shot_label_type = SX_ARBITRARY;
        break;
        }
    SX_RETURN;
    }
/* END:     */
