/* DEC/CMS REPLACEMENT HISTORY, Element SX_S1LABELPOS.C*/
/* *7    17-AUG-1990 22:22:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    20-SEP-1989 16:09:50 GORDON "(SPR 200) get in sync with Gulf"*/
/* *5    19-SEP-1989 13:39:17 GILLESPIE "(SPR 100) GULF MODS"*/
/* *4    14-SEP-1989 18:37:07 GORDON "(SPR -1) checkin from gulf"*/
/* *3     6-SEP-1989 09:40:51 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:14:15 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_S1LABELPOS.C*/
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

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#include "esi_sx_msg.h"
#include "sx_cdlgs.rh"
#include "sx_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Set up default shotpoint label position for 1 shot point.

Prototype:
    publicdef INT sx_set_1_label_position(SX_STRUCT *psx);

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
publicdef INT sx_set_1_label_position (SX_STRUCT *psx)
#else
publicdef INT sx_set_1_label_position (psx)
SX_STRUCT *psx;
#endif
    {
    INT status;
    
    CHAR text[128];
    INT id;
    SX_CLP_STRUCT *pclp;
    INT iang;
    INT button;
    FLOAT x, y;
    VOIDPTR value_list[3];
    UINT dim_list[3];
    FLOAT nx, ny;                   /* 4 byte floats */
    UINT ncdp;
    NLIST_HEADER nlist = psx->pso->cdp_xy;
    UINT ngot;
    SP fshot;
    
    /* open window and set tnr */
    
    am_message (AM_DIALOG, mg_message (SX_PICK_SHOT));
    wmwopen (psx->graphics.window);
    wmtselect (psx->graphics.tnr);
    sx_locate (psx, &status, &x, &y, &button);
    wmwclose (psx->graphics.window);
    am_message (AM_DIALOG, "");
    
    /* check or cancel button */
    
    if (button EQUALS WM_BUTTON_CANCEL)
        {
        SX_RETURN;
        }
    /* see if already exists */
    
    nx = x;
    ny = y;
    
    status = sx_find_point (psx, nlist, nx, ny);
    
    if (status != SUCCESS)
        {
        sprintf (text, "%s", mg_message (SX_PICK_CLOSER_SP));
        am_message (AM_ALERT, text);
        SX_RETURN;
        }
    /* get point */
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    value_list[0] = (VOIDPTR) & ncdp;
    value_list[1] = (VOIDPTR) & nx;
    value_list[2] = (VOIDPTR) & ny;
    
    nl_get_points (nlist, 1, 3, dim_list, value_list, &ngot);
    
    /* convert to shotpoint */
    
    if (psx->new_line)
        {
        fshot = psx->dialog_spo.current_shot = (ncdp - psx->new_line_intercept) /
                psx->new_line_slope;
        }
    else
        {
        se_sp_from_cdp (&psx->pso->current_line, ncdp, &fshot);
        }
    /* blink it */
    
    sx_do_draw (psx, nx, ny, ncdp, fshot, SX_BLINK);
    
    /* allocate PCLP STRUCT */
    
    pclp = (SX_CLP_STRUCT *)am_allocate (AM_APPLICATION, sizeof(SX_CLP_STRUCT));
    pclp->psx = psx;
    pclp->cdp = ncdp;
    pclp->menu_item = ITEM_LP_CHANGE_INDIVIDUAL;
    
    /* see if already special */
    
    nl_inq_nlist_int (psx->pso->disp_opt.shot_label_nlist, NL_NLIST_NPOINTS,
                      (INT *) & ngot);
    if (ngot EQUALS 0)
        {
        status = FAIL;
        }
    else
        {
        nl_set_current_point (psx->pso->disp_opt.shot_label_nlist, 1, 1);
        status = so_find_point (psx->pso->disp_opt.shot_label_nlist, ncdp);
        }
    if (status EQUALS SUCCESS)
        {
        dim_list[0] = 2;
        dim_list[1] = 3;
        value_list[0] = (VOIDPTR) & pclp->angle;
        value_list[1] = (VOIDPTR) & pclp->paraperp;
        nl_get_points (psx->pso->disp_opt.shot_label_nlist, 1, 2, dim_list, value_list,
                       &ngot);
        }
    else
        {
        iang = psx->pso->disp_opt.shot_label_angle / 45.0 + 0.5;
        iang *= 45;
        pclp->angle = iang;
        pclp->paraperp = psx->pso->disp_opt.shot_label_paraperp;
        }
    /* define resource */
    
    am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_CLP, sx_dialog_clp_srvr, &id,
                        pclp);
    
    /* set title and save id */
    
    sprintf (text, "Set Shotpoint Label Position for %.0f", fshot);
    psx->cdlg_id.dialog_clp_1lab = id;
    
    /* update title */
    
    wi_set_dialog_title (id, text);
    
    /* turn off menu */
    
    wienmni (psx->menu_id.lp, ITEM_LP_CHANGE_INDIVIDUAL, OFF);
    
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
