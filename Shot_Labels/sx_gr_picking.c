/* DEC/CMS REPLACEMENT HISTORY, Element SX_GR_PICKING.C*/
/* *9    23-AUG-1990 17:23:01 JESSIE "(SPR 1) fix include files"*/
/* *8     2-NOV-1989 09:54:59 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *7    20-SEP-1989 14:59:19 GORDON "(SPR 200) Get in sync with Gulf"*/
/* *6    19-SEP-1989 13:38:57 GILLESPIE "(SPR 100) GULF MODS"*/
/* *5    14-SEP-1989 18:34:59 GORDON "(SPR -1) checkin from gulf"*/
/* *4     1-SEP-1989 14:39:27 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *3    31-JUL-1989 14:42:11 GORDON "(SPR 101) add call to wm_set_mouse_menu for default menu "*/
/* *2    25-JUL-1989 20:12:32 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_GR_PICKING.C*/

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
/* PROGRAM:  SX_GRAPHIC_PICKING.C                                            */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: PERFORM GRAPHIC SELECTION OF A SEISMIC LINE                     */
/*                                                                           */
/*****************************************************************************/

/* set HARD_PICK to 1 for hardware pick else set to 0 */
#define HARD_PICK 0

#include "esi_sx.h"
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "esi_sx_msg.h"
#include "esi_sx_err.h"

#include "sx_cdlgs.rh"

publicdef INT sx_graphic_picking(psx)
SX_STRUCT *psx;
{
    INT  dummy;
    SEGMENT_ID segment_id;
    GKS_INT old_color;
    INT  pick;
    INT  class;
    INT  button;
    BOOL keep_on_picking;
    INT  status;
    FLOAT xtol,ytol,cdptol;
    FLOAT x,y;
    DOUBLE dx,dy;
    VOIDPTR value_list[3];
    UINT  dim_list[3];
    INT id1, len;
    CHAR test_string[2];
    CHAR text[63];
    INT main_wkid = MAIN_WKID;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;

    status = SUCCESS;
    /* save previous line if necesary */

    if(psx->line_selected AND psx -> need_to_save )
        {
        sx_save_line(psx,TRUE);
        }

    /* open window and group */

    wmwopen(psx->graphics.window);
    wmgopen(psx->graphics.group);
    wmtselect(psx->graphics.tnr);

    /* pick the line */

    am_message(AM_DIALOG,mg_message(SX_PICK_A_LINE));
    keep_on_picking = TRUE;

    while(keep_on_picking)
        {

    /* request pick */
#if HARD_PICK == 1

        wmprequest(&status, &segment_id, &pick, &button);
#else

        if (ug_if_gpx())
	    wm_set_mouse_menu( (CHAR**)0, (INT*)0 );  /* use default menu */      
        wmtlocate(&status,&x,&y,&button);

#endif

    /* check cancel */

        if(button == WM_BUTTON_CANCEL)
            {
	    am_message(AM_DIALOG,"");
            status = FAIL;
            wmgclose(psx->graphics.group);
            wmwclose(psx->graphics.window);
            SX_RETURN;
            }

    /* check done */

        if(button == WM_BUTTON_DONE)
            {
            am_message(AM_DIALOG,"");
            status = SUCCESS;
            wmgclose(psx->graphics.group);
            wmwclose(psx->graphics.window);
            SX_RETURN;
            }

    /* check no pick */

        if(status == SUCCESS)
            {
            keep_on_picking = FALSE;

#if HARD_PICK == 0

	    dx = x; dy = y;
	    status = sx_pick_line(psx->pso, dx, dy,
				  &(psx->pso->current_line), &segment_id);

 	    if (segment_id <= 0)
  	        {
		am_message(AM_DIALOG,mg_message(SX_PICK_CLOSER));
		keep_on_picking = TRUE;
		}
	    else
		{
		psx->on_map = TRUE;
/* set blinking, have to update pso structure and change WM groups if more than
one seismic overlay on the map */ 
		old_color = psx->pso->seismic_generic.seismic_line.line.color;
		lu_text_to_index("COLOR",
		    &psx->pso->seismic_generic.seismic_line.line.color,
		    "HIGHLIGHT");
		status = so_line_load(psx->pso);
		status = so_draw_line_itself(psx->pso);
		psx->pso->seismic_generic.seismic_line.line.color = old_color;
		}		
#endif
           }
        }

    am_message(AM_DIALOG,"");

#if HARD_PICK ==1

/* inquire line name */
    wmoinq(segment_id, &class, psx->pso->current_line.line_name,
				psx->pso->current_line.line_name);

#endif


    /* set dialog text */
    sprintf(text, "%s-%s", psx->pso->current_line.line_name,
	psx->pso->current_line.survey);
    wiscdt(psx->cdlg_id.main,ITEM_SLE_LINE_EDIT,text);

#if HARD_PICK == 1    
    /* draw it outside of segments */
    wmsvis(segment_id,GINVIS);
    wmgclose(psx->graphics.group);
    wm_grsgwk(main_wkid);
#endif    
    psx->new_line = FALSE;
#if 0
    sx_special_pso(psx);
#endif
    psx->pso->disp_opt.shot_label_nlist = (NLIST_HEADER) 0;
#if HARD_PICK == 1
    so_process_line(psx->pso,FALSE,&dummy);
#endif
    sx_check_sln(psx);

    nl_inq_nlist_int(psx->pso->cdp_xy,NL_NLIST_NPOINTS,&psx->nshots);

    /* close window  */

    wmwclose(psx->graphics.window);

    /* reset menus */

    psx->line_selected = TRUE;
    psx->need_to_save  = TRUE;
    sx_menu_ls_setup(psx);
    sx_menu_sp_setup(psx);
    sx_menu_lp_setup(psx);

    /* set tolerances */

    xtol = 0.01 * (pmap->upper_right_xy.x -
                   pmap->lower_left_xy.x);
    ytol = 0.01 * (pmap->upper_right_xy.y -
                   pmap->lower_left_xy.y);
    cdptol = 0.001;

    dim_list[0]=1;
    dim_list[1]=2;
    dim_list[2]=3;

    value_list[0] = (VOIDPTR) &xtol;
    value_list[1] = (VOIDPTR) &ytol;
    value_list[2] = (VOIDPTR) &cdptol;

    nl_set_nlist_info(psx->pso->cdp_xy,
                      NL_NLIST_FIND_TOLERANCE,3,dim_list,value_list);

    value_list[0] = (VOIDPTR) &cdptol;
    nl_set_nlist_info(psx->pso->disp_opt.shot_label_nlist,
                      NL_NLIST_FIND_TOLERANCE,1,dim_list,value_list);
    

    SX_RETURN;
}
