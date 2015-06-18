/* DEC/CMS REPLACEMENT HISTORY, Element SX_SDEFLABPOS.C*/
/* *5    17-AUG-1990 22:22:32 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    14-SEP-1989 18:37:58 GORDON "(SPR -1) checkin from gulf"*/
/* *3     6-SEP-1989 09:42:15 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:14:36 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SDEFLABPOS.C*/
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
    Set up default shotpoint label positions.

Prototype:
    publicdef INT sx_set_def_label_positions(SX_STRUCT *psx);

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
publicdef INT sx_set_def_label_positions (SX_STRUCT *psx)
#else
publicdef INT sx_set_def_label_positions (psx)
SX_STRUCT *psx;
#endif
    {
    INT status;
    
    CHAR text[128];
    INT id;
    SX_CLP_STRUCT *pclp;
    INT iang;
    
    /* allocate and set up pclp struct */
    
    pclp = (SX_CLP_STRUCT *)am_allocate (AM_APPLICATION, sizeof(SX_CLP_STRUCT));
    pclp->psx = psx;
    iang = psx->pso->disp_opt.shot_label_angle / 45.0 + 0.5;
    iang *= 45;
    pclp->angle = iang;
    pclp->paraperp = psx->pso->disp_opt.shot_label_paraperp;
    pclp->cdp = SO_NULL;
    pclp->menu_item = ITEM_LP_SET_DEFPOS;
    
    /* define resource */
    
    am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_CLP, sx_dialog_clp_srvr, &id,
                        pclp);
    
    /* set title and save id */
    
    strcpy (text, "Set Shotpoint Labels Default Position");
    psx->cdlg_id.dialog_clp_line = id;
    
    /* update title */
    
    wi_set_dialog_title (id, text);
    
    /* turn off menu */
    
    wienmni (psx->menu_id.lp, ITEM_LP_SET_DEFPOS, OFF);
    
    /* no label suppression */
    
    wiscdv (id, ITEM_CLP_SUPPRESS, OFF);
    
    /* set up box */
    
    sx_dialog_clp_setup (pclp, id);
    
    /* open box */
    
    wiopcd (id);
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:     */
