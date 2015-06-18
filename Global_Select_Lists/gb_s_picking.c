/* DEC/CMS REPLACEMENT HISTORY, Element GB_S_PICKING.C */
/* *6    30-SEP-1991 17:57:59 JULIAN "(SPR 4136) added trans points after picking seismic line(s)" */
/* *5     8-JUL-1991 15:45:09 JULIAN "(SPR 1554) add missing text in status box" */
/* *4    30-OCT-1989 14:35:16 JULIAN "(SPR 5039) Seismic Select List Mods" */
/* *3    10-OCT-1989 09:51:37 JULIAN "(SPR 5020) mods for CT parameter change" */
/* *2    11-AUG-1989 20:17:41 CONROY "(SPR 0) Install revised seismic selects & picking" */
/* *1    11-AUG-1989 19:53:21 CONROY "Global seismic picking" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_S_PICKING.C */
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

/* * INCLUDES * */
/* ------------ */

#include "esi_c_lib.h"
#ifndef ESI_GB_H
#include "esi_gb.h"
#endif
#ifndef ESI_GB_ERR_H
#include "esi_gb_err.h"
#endif
#ifndef ESI_GB_MSG_H
#include "esi_gb_msg.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_WI_H
#include "esi_wi.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_SE_H
#include "esi_se.h"
#endif
#ifndef ESI_SO_H
#include "esi_so.h"
#endif
#ifndef ESI_CT_H
#include "esi_ct.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif


#define ARRAY_BLOCKSIZE 20


/* * FUNCTION * */
/* ------------ */
/* **************************************************************** */

publicdef INT gb_seismic_picking(listPtr)

NLIST_HEADER *listPtr;

/* **************************************************************** */
/* Interactive selection of all or parts of seismic lines from
   the current map window.

   Status return is SUCCESS if a new list of lines was selected,
		    FAIL    if user terminated session without setting new selection
*/
/* **************************************************************** */

{

INT status = SUCCESS;
INT wmStatus, button;
INT total_picks = 0;
INT n_used = 0;
INT n_alloc = 0;
INT start_point;
FULL_LINE_NAME seis_line;
SEIS_LINE_ID *id_arr = (SEIS_LINE_ID *)0;
CDP *cdp1_arr = (INT *)0;
CDP *cdp2_arr = (INT *)0;
CHAR prompt[STMAX], status_string[STMAX];
FLOAT f_pick_x, f_pick_y;
DOUBLE d_pick_x,d_pick_y;
DOUBLE cdp1_x, cdp1_y;
MAP_STRUCTURE *map;
INT i;
SX_WORKSPACE *w;
INT defmod,regmod;
INT appSave, winSave, tnrSave, groSave;
UINT stroke;
UINT ndim;
UINT d_list[4];
VOIDPTR v_list[4];
CDP_FLOAT cdp_float;
DOUBLE x1,y1,x2,y2;
CDP cdp_end;

/**************************************************************************/

wm_inq_deferral(&defmod,&regmod);
wm_hold_updates();

/* * GET MAP FOR PICKING * */
/* ----------------------- */

if ( (status = mp_front_map(&map)) != SUCCESS)
    {
    am_message(AM_ALERT,mg_message(GB_ERR_NO_MAP_WKSP));
    return (FAIL);
    }
wm_update();


/* * SAVE WM STATE AND CLOSE OUT TO APPLICATION * */
/* ---------------------------------------------- */

wmowhere(&appSave,&winSave,&tnrSave,&groSave,&wmStatus);


/* * ATTACH TO TOP LEVEL * */
/* ----------------------- */

wmoattach(appSave,0,0,0,&wmStatus);
if (wmStatus != SUCCESS)
    {
    am_message(AM_ALERT,"wmoattach() failed");
    return (FAIL);
    }


wmwopen(map->map_window_id);
wmtselect(map->map_tnr);
if (ug_if_gpx())
    {
    wm_set_mouse_menu((CHAR**)0,(INT*)0);   /* default menu */
    }


/* * ALLOCATE PICKING ARRAYS * */
/* --------------------------- */

n_alloc += ARRAY_BLOCKSIZE;
id_arr = (SEIS_LINE_ID *)tc_zalloc(n_alloc*sizeof(SEIS_LINE_ID));
cdp1_arr = (CDP *)tc_zalloc(n_alloc*sizeof(CDP));
cdp2_arr = (CDP *)tc_zalloc(n_alloc*sizeof(CDP));


/* * PICKING LOOP * */
/* ---------------- */

strncpy(prompt,mg_message(GB_SEIS_PICK_PROMPT),STMAX);
prompt[STMAX-1] = 0;
FOREVER
    {


/* * GET CROSSHAIR LOCATION * */
/* -------------------------- */

    am_message(AM_DIALOG,prompt);
    wmtlocate(&wmStatus,&f_pick_x,&f_pick_y,&button);
    if (button EQUALS WM_BUTTON_DONE  OR  button EQUALS WM_BUTTON_CANCEL)
	{
	break;
	}

    
/* * FILL PICKING STRUCTURE IF NEEDED, (FIRST TIME ONLY) * */
/* ------------------------------------------------------- */

    if (am_open_workspace("SX_STATIC_SPACE",AM_GLOBAL,(VOIDPTR *)&w) != SUCCESS)
	{
	status = am_define_workspace("SX_STATIC_SPACE",AM_GLOBAL,
				sizeof(SX_WORKSPACE),(VOIDPTR)&w);
	w->changed = TRUE;
	}

    if (w->changed)
	{
	w->changed = FALSE;
	if (w->temp_nlist != (NLIST_HEADER) NULL)
	    {
	    nl_free_nlist(w->temp_nlist);
	    }
	if (w->master_nlist != (NLIST_HEADER) NULL)
	    {
	    nl_free_nlist(w->master_nlist);
	    }
	so_build_common_nlist((SO_STRUCT *)(map->pso),&w->temp_nlist);
	nl_copy_nlist(w->temp_nlist,&w->master_nlist);
	}


/* * FIND NEAREST SEISMIC LINE * */
/* ----------------------------- */

    d_pick_x = (DOUBLE) f_pick_x;
    d_pick_y = (DOUBLE) f_pick_y;

    /* transform picked point to Project projection (case 4136) */
    ct_project_xy(d_pick_x,d_pick_y,&d_pick_x,&d_pick_y);

    if (sx_pick_line((SO_STRUCT *)(map->pso),d_pick_x,d_pick_y,&seis_line,&i) 
								EQUALS SUCCESS)
	{


/* * LOOK FOR FOUND LINE IN ID_ARR * */
/* --------------------------------- */

	for (i=0; i<n_used; i++)
	    {
	    if (seis_line.id EQUALS id_arr[i])
		{
		break;
		}
	    }


/* * IF FOUND IN LIST, REMOVE * */
/* ---------------------------- */

	if (i < n_used)
	    {
	    id_arr[i] = 0;
	    total_picks--;
	    sprintf(status_string,mg_message(GB_REMOVE_FROM_LIST),
						    seis_line.line_name);
	    }


/* * ELSE ADD TO LIST * */
/* -------------------- */

	else
	    {
	    if (n_used EQUALS n_alloc)
		{
		n_alloc += ARRAY_BLOCKSIZE;
		id_arr = (SEIS_LINE_ID *) tc_realloc((VOIDPTR)id_arr,
					    n_alloc*sizeof(SEIS_LINE_ID));
		cdp1_arr = (CDP *) tc_realloc((VOIDPTR)cdp1_arr,
					    n_alloc*sizeof(CDP));
		cdp2_arr = (CDP *) tc_realloc((VOIDPTR)cdp2_arr,
					    n_alloc*sizeof(CDP));
		}


/* * GET FIRST CDP OR TAKE WHOLE LINE * */
/* ------------------------------------ */

	    FOREVER
		{
		am_message(AM_DIALOG,mg_message(GB_SEIS_LOC1_PROMPT));
		wmtlocate(&wmStatus,&f_pick_x,&f_pick_y,&button);

		    /* IF CANCEL, DROP THIS LINE AND RETURN TO LINE PROMPT */

		if (button EQUALS WM_BUTTON_CANCEL)
		    {
		    goto next_line;
		    }

		    /* ELSE IF DONE, TAKE WHOLE LINE * */

		else if (button EQUALS WM_BUTTON_DONE)
		    {
		    se_first_last_cdp(&seis_line,&(cdp1_arr[n_used]),
							&(cdp2_arr[n_used]));
		    break;
		    }

		    /* ELSE FIND NEAREST CDP TO POINT */

		else
		    {
		    cdp1_x = f_pick_x;
		    cdp1_y = f_pick_y;
		    ct_project_xy(cdp1_x,cdp1_y,&d_pick_x,&d_pick_y);
		    if (se_cdp_from_xy(&seis_line,d_pick_x,d_pick_y,-1.0,
				    &(cdp1_arr[n_used]),&cdp_float,&x1,&y1,
					    &cdp_end,&x2,&y2) EQUALS SUCCESS)
			{


/* * GET SECOND CDP * */
/* ------------------ */

			FOREVER
			    {
			    am_message(AM_DIALOG,
					    mg_message(GB_SEIS_LOC2_PROMPT));
			    wmtbandvector(&wmStatus,cdp1_x,cdp1_y,
						&f_pick_x,&f_pick_y,&button);

		    /* IF CANCEL, DROP THIS LINE AND RETURN TO LINE PROMPT */

			    if (button EQUALS WM_BUTTON_CANCEL)
				{
				goto next_line;
				}

		    /* ELSE IF DONE, TAKE WHOLE LINE * */

			    else if (button EQUALS WM_BUTTON_DONE)
				{
				se_first_last_cdp(&seis_line,
				    &(cdp1_arr[n_used]),&(cdp2_arr[n_used]));
				break;
				}

		    /* ELSE FIND NEAREST CDP TO POINT */

			    else
				{
				d_pick_x = f_pick_x;
				d_pick_y = f_pick_y;
				ct_project_xy(d_pick_x,d_pick_y,&d_pick_x,
						&d_pick_y);
				if (se_cdp_from_xy(&seis_line,
					d_pick_x,d_pick_y,-1.0,
					    &(cdp2_arr[n_used]),&cdp_float,
						&x1,&y1,&cdp_end,&x2,&y2)
							    EQUALS SUCCESS)
				    {
				    break;
				    }
				else
				    {
				    am_message(AM_DIALOG,
					    mg_message(GB_SEIS_CDP_FIND_ERR));
				    }
				}
			    }

			break;
			}
		    else
			{
			am_message(AM_DIALOG,mg_message(GB_SEIS_CDP_FIND_ERR));
			}
		    }
		}
	    id_arr[n_used] = seis_line.id;
	    n_used++;
	    total_picks++;
	    if(IS_EMPTY_STRING(seis_line.line_name))
	      sprintf(status_string,mg_message(GB_ADD_TO_LIST),
						    "Nothing");
	    else
	      sprintf(status_string,mg_message(GB_ADD_TO_LIST),
						    seis_line.line_name);
	    }

	strncpy(prompt,mg_message(GB_SEIS_PICK_PROMPT),STMAX);
	prompt[STMAX-1] = 0;
	strncat(prompt,status_string,STMAX);
	}
    else
	{
	strncpy(prompt,mg_message(GB_SEIS_MISS_PROMPT),STMAX);
	prompt[STMAX-1] = 0;
	}

    next_line:
	;
    }


/* * END OF PICKING LOOP, NOW BUILD SELECT LIST IF NOT CANCELLED * */
/* --------------------------------------------------------------- */

if ((total_picks > 0)  AND (button != WM_BUTTON_CANCEL))
    {
    sl_init_select_list(SEISMIC_DATA_TYPE,listPtr);
    nl_start_stroke(*listPtr,&stroke);
    ndim = 3;
    d_list[0] = 1;  d_list[1] = 2;  d_list[2] = 3;  

    start_point = 0;
    total_picks = 0;
    for (i=0; i<n_used; i++)
	{
	if (id_arr[i] EQUALS 0)
	    {
	    if (total_picks > 0)
		{
		v_list[0] = (VOIDPTR) &(id_arr[start_point]);
		v_list[1] = (VOIDPTR) &(cdp1_arr[start_point]);
		v_list[2] = (VOIDPTR) &(cdp2_arr[start_point]);
		
		nl_add_points(*listPtr,total_picks,ndim,d_list,v_list);
		total_picks = 0;
		}
	    start_point = i+1;
	    }
	else
	    {
	    total_picks++;
	    }
	}
    if (total_picks > 0)
	{
	v_list[0] = (VOIDPTR) &(id_arr[start_point]);
	v_list[1] = (VOIDPTR) &(cdp1_arr[start_point]);
	v_list[2] = (VOIDPTR) &(cdp2_arr[start_point]);

	nl_add_points(*listPtr,total_picks,ndim,d_list,v_list);
	}
    status = SUCCESS;
    }
else
    {
    status = FAIL;    /* So list will not be made current */
    }

tc_free(id_arr);
tc_free(cdp1_arr);
tc_free(cdp2_arr);
wmoattach(appSave,winSave,tnrSave,groSave,&wmStatus);
am_message(AM_DIALOG,"");
wm_set_deferral(defmod,regmod);
return(status);
}
