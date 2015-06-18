/* DEC/CMS REPLACEMENT HISTORY, Element SX_SAVE_LINE.C*/
/*  11    9-MAR-1992 16:01:21 JANTINA "(SPR 0) added ifndef ESI_CT_MSG_H before include esi_ct_msg.h"*/
/* *10   11-JUN-1991 11:13:47 KEE "(SPR 7247) Fix seismic line editor redisplay bug, case 1504"*/
/* *9     4-JAN-1991 12:32:03 MING "(SPR 6272) (for purna) fix line disappearance after save. reset on map for newly created line"*/
/* *8    23-AUG-1990 17:22:31 JESSIE "(SPR 1) fix include files"*/
/* *7    16-NOV-1989 21:28:13 GILLESPIE "(SPR 1) fixes for seismic line editor"*/
/* *6    19-SEP-1989 13:39:23 GILLESPIE "(SPR 100) GULF MODS"*/
/* *5    14-SEP-1989 18:37:32 GORDON "(SPR -1) checkin from gulf"*/
/* *4     1-SEP-1989 14:40:47 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *3    25-JUL-1989 20:14:25 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:04:14 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:30:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SAVE_LINE.C*/

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
/* PROGRAM:  SX_SAVE_LINE.C                                                  */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: SAVE A LINE                                                     */
/*                                                                           */
/*****************************************************************************/
#include "esi_c_lib.h"
#include "esi_sx.h"
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#include "esi_ct_msg.h"
#endif
#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"

#include "sx_cdlgs.rh"

publicdef INT sx_save_line(psx,ifask)
SX_STRUCT *psx;
BOOL      ifask;
{
    INT status;
    INT ifyes;
    UINT nrows;
    CHAR prompt[128];
    CHAR bt_key[62];
    INT len, id1;
    CHAR test_string[2];
    FILENAME nlist_file;
    FILENAME full_filename;
    SEGMENT_ID segment_id;
    NLIST_HEADER temp_nlist,temp_nlist2;
    UINT ct_Col_List[3];
    SX_WORKSPACE *w;
    BOOL switch_back;
    NLIST_HEADER converted_cdp_xy;

    ct_Col_List[0] = 2, ct_Col_List[1] = 3, ct_Col_List[2] = 1;
    /* see if need to be here */

    status = SUCCESS;
    wi_set_dialog_text(psx->cdlg_id.main,ITEM_SLE_LINE_EDIT,"");
    wi_set_dialog_text(psx->cdlg_id.main,ITEM_SLE_SHOTPOINT_EDIT,"");

    if (! psx->need_to_save) 
        {
        SX_RETURN;
        }

			     /*	open window and group and set tnr	    */

    wmwopen(psx->graphics.window);
    wmgopen(psx->graphics.group);
    wmtselect(psx->graphics.tnr);


			     /*	prompt to see if they wish to save the last */
			     /*	line					    */

    psx->line_selected=FALSE;
    sprintf(bt_key,"%s-%s",psx->pso->current_line.line_name,
				    psx->pso->current_line.survey);
    btfind(&psx->lines,bt_key,&segment_id);

    if(ifask)
        {
        fi_push();
	sprintf(prompt, "%s%s", mg_message(SX_SAVE_YESNO),bt_key);
        wiyesno(prompt,"",&ifyes,&status);
        fi_pop();
        }
    else
        {
        ifyes = TRUE;
        }


/* * DONT SAVE * */
/* ------------- */

    if((! ifyes) OR psx->nchanges == 0 OR psx->nshots == 0)
        {
        if (psx->on_map AND segment_id != 0)
            {
            wmsvis(segment_id,GVISI);
             }
        }


/* * SAVE * */
/* -------- */

    else
	{
	psx->on_map = TRUE;
			     /*	transform nlist back to project nlist	    */

	status = ct_project_nlist_xy(psx->pso->cdp_xy,&temp_nlist,
							ct_Col_List, 0.0);
	if ( status IS_NOT_EQUAL_TO CT_SAME_PROJECTIONS)
	    {    
	    temp_nlist2 = psx->pso->cdp_xy;
	    switch_back = TRUE;
	    psx->pso->cdp_xy = temp_nlist;
	    }
	else
	    {
	    switch_back = FALSE;
	    status = SUCCESS;
	    }
    
			     /*	save cdp xy changes			    */

	status = sx_convert_cdpxy(psx, &converted_cdp_xy);
	status = se_put_position(converted_cdp_xy,&psx->pso->current_line,FALSE);
			     /*	Remove the special one-time unusual
			     	pre-correct transitional ephemeral
			     	pseudo-line id				    */
	
	se_delete_index(-1);

	if (status == SUCCESS)
	    {

			     /*	save nlist of shot_label stuff		    */

	    nl_inq_nlist_int(psx->pso->disp_opt.shot_label_nlist,
					NL_NLIST_NROWS,(INT *)&nrows);

	    if (nrows > 0)
		{
		sx_adj_sln(psx);
		status = se_bld_nlist_ids(&psx->pso->current_line,
				(VOIDPTR)0,SEIS_SHOT_LABEL_DATA,
				    psx->pso->disp_opt.shot_label_nlist_id,
					    nlist_file, full_filename);
		nl_delete_nlist(full_filename,
				psx->pso->disp_opt.shot_label_nlist_id);
		if (nl_write_nlist(psx->pso->disp_opt.shot_label_nlist,
			    full_filename,
			    psx->pso->disp_opt.shot_label_nlist_id) != SUCCESS)
		    {
		    strcpy(psx->pso->disp_opt.shot_label_nlist_id,"");
		    am_message(AM_ALERT,mg_message(SX_CANT_WRITE_SLN));
		    }
		}
	    else
		{
		nl_free_nlist(psx->pso->disp_opt.shot_label_nlist);
		psx->pso->disp_opt.shot_label_nlist = (NLIST_HEADER)0;
		}

			     /*	save seis map disp opt stuff		    */

	    if (nrows > 0  OR  NOT psx->default_filter)
		{
		if (sx_save_disp_opt(psx) != SUCCESS)
		    {
		    am_message(AM_ALERT,mg_message(SX_CANT_SAVE_DOPT));
		    }
		}


	    if (psx->on_map)
		{
			     /*	delete old segment if it existed	    */
 
		if (segment_id != 0)
		    {
		    status = wmsdelete(segment_id);
		    }

		wmsallocate(&segment_id);
		btrepcur(&psx->lines,bt_key,&segment_id);
		if( switch_back )
		    {
		    nl_free_nlist(psx->pso->cdp_xy);
		    psx->pso->cdp_xy = temp_nlist2;
		    }
		psx->pso->current_line.id = 0; /* reset id to 0 to force
						  reload line data from
						  database */
		so_draw_seismic_line(psx->pso);
		wmsclose(segment_id);
		wmoset(segment_id,WM_CLASS_SEISMIC,bt_key,
			    psx->pso->current_line.line_name);    
		}
	    }

	}

			     /*	release line info			    */

			     /*	update sx_workspace			    */
    status = am_open_workspace("SX_STATIC_SPACE",AM_GLOBAL,(VOIDPTR *)&w);
    if (w->temp_nlist != (NLIST_HEADER) 0)
	{
	status = nl_free_nlist(w->temp_nlist);
	}
    status = nl_copy_nlist(w->master_nlist,&w->temp_nlist); 

    if (NOT psx->on_map)
            {
            psx->nlines--;
	    if (segment_id != 0)
		{
		wmsdelete(segment_id);
		segment_id = 0;
		btrepcur(&psx->lines,bt_key,&segment_id);
	        }
	    wmsclose (segment_id);	  
            }

    wm_grsgwk(MAIN_WKID);
    wmgclose(psx->graphics.group);
    wmwclose(psx->graphics.window);

    nl_free_nlist(psx->pso->disp_opt.shot_label_nlist);
    nl_free_nlist(psx->pso->cdp_xy);
    psx->pso->disp_opt.shot_label_nlist = (NLIST_HEADER)0;
    psx->pso->cdp_xy = (NLIST_HEADER)0;
    psx->need_to_save = FALSE;
    psx->nshots=0;
    psx->nchanges=0;
    sx_menu_ls_setup(psx);
    sx_menu_lp_setup(psx);
    sx_menu_sp_setup(psx);
    status = SUCCESS;

    SX_RETURN;
}

/* ********************************************************************* */

publicdef INT sx_adj_sln(psx)
SX_STRUCT *psx;
{
    NLIST_HEADER nlist = psx->pso->disp_opt.shot_label_nlist;
    INT i,status;
    UINT nrows;
    INT *cdps;
    UINT Col_List[1];
    VOIDPTR value_list[1];
    INT sortflags[1];
    NLIST_HEADER sorts;
    CDP_FLOAT shot;

			     /*	if old line, cdp / shot is ok		    */

    if(! psx->new_line)
        {
        SX_RETURN;
        }

    Col_List[0] = 1;
    nl_inq_nlist_int(nlist,NL_NLIST_NROWS,(INT *)&nrows);
    cdps = (INT *) tc_zalloc( nrows * sizeof (INT));
    value_list[0] = (VOIDPTR)cdps;
    nl_set_current_row(nlist,1,1);
    nl_get_rows(nlist,nrows,1,Col_List,value_list,&nrows);

			     /*	change from sle cdp to shot to data base    */
			     /*	cdp to shot				    */

    for (i=0;i<nrows;i++)
        {
        sx_sp_from_cdp(psx,cdps[i],&shot);
        se_cdp_from_sp(&psx->pso->current_line,shot,&cdps[i]);
        }
    nl_set_current_row(nlist,1,1);
    nl_replace_rows(nlist,nrows,1,Col_List,value_list);
    tc_free(cdps);
    sortflags[0] = NL_ASCENDING;
    nl_sort_nlist(nlist,1,Col_List,sortflags,&sorts);
    nl_free_nlist(nlist);
    psx->pso->disp_opt.shot_label_nlist = sorts;
    status = SUCCESS;
    SX_RETURN;
}
