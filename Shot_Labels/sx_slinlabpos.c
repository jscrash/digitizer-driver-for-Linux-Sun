/* DEC/CMS REPLACEMENT HISTORY, Element SX_SLINLABPOS.C*/
/* *8    30-OCT-1990 11:55:10 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *7    17-AUG-1990 22:22:41 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    14-FEB-1990 13:19:17 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *5     2-NOV-1989 09:55:28 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *4    14-SEP-1989 18:38:22 GORDON "(SPR -1) checkin from gulf"*/
/* *3     6-SEP-1989 09:43:25 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    25-JUL-1989 20:14:51 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SLINLABPOS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SLINLABPOS.C                             */
/* *7    17-AUG-1990 22:22:41 VINCE "(SPR 5644) Code Cleanup"                       */
/* *6    14-FEB-1990 13:19:17 GILLESPIE "(SPR 6005) Make nlist window clipping a method"    */
/* *5     2-NOV-1989 09:55:28 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"    */
/* *4    14-SEP-1989 18:38:22 GORDON "(SPR -1) checkin from gulf"                   */
/* *3     6-SEP-1989 09:43:25 GORDON "(SPR 200) Gulf changes (August trip)"         */
/* *2    25-JUL-1989 20:14:51 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *1    19-JUN-1989 13:30:24 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SLINLABPOS.C                             */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_sx.h"
#include "esi_wi.h"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#include "esi_nl_meth_hdr.h"
#include "esi_sx_msg.h"
#include "sx_cdlgs.rh"
#include "sx_menus.rh"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Set up default line label position for 1 line label.                          */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef  INT sx_set_line_label_positions(SX_STRUCT *psx);                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    psx             -(SX_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT sx_set_line_label_positions (SX_STRUCT *psx)
#else
publicdef INT sx_set_line_label_positions (psx)
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
    FLOAT nx, ny;
    CDP ncdp;
    NLIST_HEADER nlist = psx->pso->cdp_xy;
    NLM_CLIP_HEADER clip;
    UINT ngot, intcdp;
    CDP_FLOAT fshot;
    UINT point, stroke, npoints;
    BOOL ok = FALSE;
    BOOL need_to_free = FALSE;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;
    
    status = SUCCESS;
    
    value_list[0] = (VOIDPTR)&ncdp;
    value_list[1] = (VOIDPTR)&nx;
    value_list[2] = (VOIDPTR)&ny;
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    /* check if line is clipped, if so replace it by clipped line                   */
    
    nl_first_point (nlist, 3, dim_list, value_list);
    
    if (nx < pmap->lower_left_xy.x OR nx > pmap->upper_right_xy.x OR ny <
        pmap->lower_left_xy.y OR ny > pmap->upper_right_xy.y)
        {
        /* clip                                                                     */
        clip.Col_List[0] = 2;
        clip.Col_List[1] = 3;
        clip.minList[0] = pmap->lower_left_xy.x;
        clip.minList[1] = pmap->lower_left_xy.y;
        clip.maxList[0] = pmap->upper_right_xy.x;
        clip.maxList[1] = pmap->upper_right_xy.y;
        status = nl_invoke_nlist_method (nlist, NL_CLIP_METHOD, &clip);
        need_to_free = TRUE;
        nlist = clip.outNlist;
        }
    else
        {
        nl_last_point (nlist, 3, dim_list, value_list);
        
        if (nx < pmap->lower_left_xy.x OR nx > pmap->upper_right_xy.x OR ny <
            pmap->lower_left_xy.y OR ny > pmap->upper_right_xy.y)
            {
            /* clip                                                                 */
            clip.Col_List[0] = 2;
            clip.Col_List[1] = 3;
            clip.minList[0] = pmap->lower_left_xy.x;
            clip.minList[1] = pmap->lower_left_xy.y;
            clip.maxList[0] = pmap->upper_right_xy.x;
            clip.maxList[1] = pmap->upper_right_xy.y;
            status = nl_invoke_nlist_method (nlist, NL_CLIP_METHOD, &clip);
            need_to_free = TRUE;
            nlist = clip.outNlist;
            }
        }
    /* open window and set tnr                                                      */
    
    while (!ok)
        {
        am_message (AM_DIALOG, mg_message (SX_PICK_ENDLINE));
        wmwopen (psx->graphics.window);
        wmtselect (psx->graphics.tnr);
        sx_locate (psx, &status, &x, &y, &button);
        wmwclose (psx->graphics.window);
        am_message (AM_DIALOG, "");
        
        /* check or cancel button                                                   */
        
        if (button == WM_BUTTON_CANCEL)
            {
            SX_RETURN;
            }
        /* see if already exists                                                    */
        
        nx = x;
        ny = y;
        nl_set_current_point (nlist, 1, 1);
        status = sx_find_point (psx, nlist, nx, ny);
        
        if (status != SUCCESS)
            {
            sprintf (text, "%s", mg_message (SX_PICK_CLOSER_SP));
            am_message (AM_ALERT, text);
            continue;
            }
        /* see if first or last point                                               */
        
        nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *)&npoints);
        nl_inq_current_point (nlist, &point, &stroke);
        if (point != 1 AND point != npoints)
            {
            am_message (AM_ALERT, mg_message (SX_PICK_END));
            continue;
            }
        ok = TRUE;
        }
    if (need_to_free)
        {
        nl_free_nlist (nlist);
        }
    /* allocate PCLP STRUCT                                                         */
    
    pclp = (SX_CLP_STRUCT *) am_allocate (AM_APPLICATION, sizeof(SX_CLP_STRUCT));
    pclp->psx = psx;
    pclp->cdp = ncdp;
    pclp->menu_item = ITEM_LP_CHANGE_LINE;
    if (point == 1)
        {
        sx_do_line_labels (psx, SX_BLINK, SX_FIRST_LABEL);
        pclp->first = TRUE;
        iang = psx->pso->disp_opt.first_line_label_angle / 45.0 + 0.5;
        iang *= 45;
        pclp->angle = iang;
        pclp->paraperp = psx->pso->disp_opt.first_line_label_paraperp;
        }
    else
        {
        sx_do_line_labels (psx, SX_BLINK, SX_LAST_LABEL);
        pclp->first = FALSE;
        iang = psx->pso->disp_opt.last_line_label_angle / 45.0 + 0.5;
        iang *= 45;
        pclp->angle = iang;
        pclp->paraperp = psx->pso->disp_opt.last_line_label_paraperp;
        }
    /* define resource                                                              */
    
    am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_CLP, sx_dialog_clp_srvr, &id,
                        pclp);
    
    /* set title and save id                                                        */
    
    strcpy (text, "Set Line Label Position");
    psx->cdlg_id.dialog_clp_lab = id;
    
    /* update title                                                                 */
    
    wi_set_dialog_title (id, text);
    
    /* turn off menu                                                                */
    
    wienmni (psx->menu_id.lp, ITEM_LP_CHANGE_LINE, OFF);
    
    /* no label suppression                                                         */
    
    wi_set_dialog_value (id, ITEM_CLP_SUPPRESS, OFF);
    
    /* set up box                                                                   */
    
    sx_dialog_clp_setup (pclp, id);
    
    /* open box                                                                     */
    
    wi_open_dialog (id);
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:                                                                             */
