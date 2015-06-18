/* DEC/CMS REPLACEMENT HISTORY, Element SX_STRUGGER.C*/
/* *5    17-AUG-1990 22:22:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    14-SEP-1989 18:39:33 GORDON "(SPR -1) checkin from gulf"*/
/* *3     6-SEP-1989 09:45:13 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:15:19 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_STRUGGER.C*/
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

#include "esi_c_lib.h"
#include "esi_sx.h"
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#include "sx_cdlgs.rh"
#include "sx_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Set up filter data for shotpoints, shot labels and data labels.

Prototype:
    publicdef INT sx_set_trigger(SX_STRUCT *psx,INT item);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    item            -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_set_trigger (SX_STRUCT *psx, INT item)
#else
publicdef INT sx_set_trigger (psx, item)
SX_STRUCT *psx;
INT item;
#endif
    {
    INT status;
    CHAR text[128];
    INT id;
    
    /* define resource */
    
    am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_SIO, sx_dialog_sio_srvr, &id,
                        psx);
    
    /* set title and save id */
    
    switch (item)
        {
        
    case ITEM_LP_SET_SYMTRIG:
        psx->cdlg_id.dialog_sio_sym = id;
        strcpy (text, "Set Shotpoint Symbol Posting Filter");
        break;
        
    case ITEM_LP_SET_LABTRIG:
        psx->cdlg_id.dialog_sio_lab = id;
        strcpy (text, "Set Shotpoint Label Posting Filter");
        break;
        
    case ITEM_LP_SET_SHOTTRIG:
        psx->cdlg_id.dialog_sio_zval = id;
        strcpy (text, "Set Data Value Posting Filter");
        break;
        
    default:
        psx->cdlg_id.dialog_sio_sym = id;
        strcpy (text, "Set Shotpoint Symbol Posting Filter");
        break;
        }
    /* update title */
    
    wi_set_dialog_title (id, text);
    
    /* turn off menu */
    
    wienmni (psx->menu_id.lp, item, OFF);
    
    /* set up box */
    
    sx_dialog_sio_setup (psx, id, TRUE);
    
    /* open box */
    
    wiopcd (id);
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:     */
