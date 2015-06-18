/* DEC/CMS REPLACEMENT HISTORY, Element SX_PROC_SHPTS.C*/
/* *4    23-AUG-1990 17:24:22 JESSIE "(SPR 1) fix include files"*/
/* *3     2-NOV-1989 09:55:21 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:14:07 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:30:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_PROC_SHPTS.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  SX_PROCESS_SHOTPOINTS.C                                         */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: PROCESS AN ADD, DELETE, MOVE, OR CHANGE SHOTPOINTS              */
/*                                                                           */
/*****************************************************************************/

#include "esi_sx.h"
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"

#include "sx_menus.rh"

publicdef INT sx_process_shotpoints(psx,item)
SX_STRUCT *psx;
INT       item;
{
    INT status;
    NLIST_HEADER nlist;                    /* copy of cdp_xy nlist */
    NLIST_HEADER nlist_sln;                /* copy of shot_label_nlist */
    INT button;                            /* locate variables */
    FLOAT x,y;                             
    UINT dim_list[3];                       /* nlist operation  stuff */
    VOIDPTR value_list[3];
    FLOAT xtol,ytol;
    INT   cdptol;                          /* find tolerances */
    UINT   nshots_save,nchanges_save;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;

    /* make copy of nlists in case one hits cancel */

    nl_copy_nlist(psx->pso->cdp_xy,&nlist);
    nl_copy_nlist(psx->pso->disp_opt.shot_label_nlist,&nlist_sln);
    nshots_save = psx->nshots;
    nchanges_save = psx->nchanges;

    /* set tolerances */

    xtol = 0.01 * (pmap->upper_right_xy.x -
                   pmap->lower_left_xy.x);
    ytol = 0.01 * (pmap->upper_right_xy.y -
                   pmap->lower_left_xy.y);
    cdptol = 0;

    dim_list[0]=1;
    dim_list[1]=2;
    dim_list[2]=3;

    value_list[0] = (VOIDPTR) &cdptol;
    value_list[1] = (VOIDPTR) &xtol;
    value_list[2] = (VOIDPTR) &ytol;

    nl_set_nlist_info(nlist,NL_NLIST_FIND_TOLERANCE,3,dim_list,value_list);

    value_list[0] = (VOIDPTR) &cdptol;
    nl_set_nlist_info(nlist_sln,NL_NLIST_FIND_TOLERANCE,1,dim_list,value_list);
    
    /* open window and set tnr */

    wmwopen(psx->graphics.window);
    wmtselect(psx->graphics.tnr);
    
    /* process requests until user hits done or cancel */

    FOREVER
        {
        if(item == ITEM_SP_ADD)
            {
            am_message(AM_DIALOG,mg_message(SX_DIGI_SHOT_LOC));
            }
        else
            {
            am_message(AM_DIALOG,mg_message(SX_PICK_SHOT));
            }
         
        sx_locate(psx,&status,&x,&y,&button);
/* to stop flashing on mac, dialog take down moved to cancel or done buttons */
/* spr fix */
/*        am_message(AM_DIALOG,"");           */

        /* check or cancel button */

        if(button == WM_BUTTON_CANCEL)
            {
            am_message(AM_DIALOG,"");
            status = FAIL;
            nl_free_nlist(nlist);
            nl_free_nlist(nlist_sln);
            psx->nshots = nshots_save;
            psx->nchanges = nchanges_save;
            wm_grsgwk(MAIN_WKID); 
            if(psx->nshots)
                {
                so_draw_seismic_line(psx->pso);
                }
            wmwclose(psx->graphics.window);
            SX_RETURN;
            }

        /* check or done button */

        if(button == WM_BUTTON_DONE)
            {
            am_message(AM_DIALOG,"");
            status = SUCCESS;
            break;
            }

        switch (item)
            {
            
            case ITEM_SP_ADD:
          
            if(psx->dialog_spo.add_type == SX_AUTO)
                {
                status = sx_add_point_auto(psx,nlist,x,y);
                }
            else
                {            
                status = sx_add_point(psx,nlist,x,y);
                }
            break;

            case ITEM_SP_MOVE:

            status = sx_move_point(psx,nlist,x,y);
            break;

            case ITEM_SP_CHANGE:

            status = sx_change_point(psx,nlist,nlist_sln,x,y);
            break;

            case ITEM_SP_DELETE:

            status = sx_delete_point(psx,nlist,nlist_sln,x,y);
            break;
   
            default: break;
            }                    /* end switch on item */
        }                        /* end forever loop */
         
    /* update nlist points */

    nl_free_nlist(psx->pso->cdp_xy);   
    psx->pso->cdp_xy = nlist;

    nl_free_nlist(psx->pso->disp_opt.shot_label_nlist);
    psx->pso->disp_opt.shot_label_nlist = nlist_sln;

    /* set save parameters */

    if(psx->nchanges > 0)
        {
        psx->need_to_save = TRUE;
        }

    /* set up menus to reflect changes */

    sx_menu_ls_setup(psx);
    sx_menu_lp_setup(psx);
    sx_menu_sp_setup(psx);
    wmwclose(psx->graphics.window);

    SX_RETURN;
}               
