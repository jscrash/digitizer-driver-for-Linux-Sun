/* DEC/CMS REPLACEMENT HISTORY, Element WE_PICK_WELL.C */
/* *2    21-SEP-1990 09:02:22 CHARLIE "(SPR -1) initial well depth editor (non-graphic)" */
/* *1    21-SEP-1990 08:34:15 CHARLIE "non_graphic edit well depth (from Robb Hall)" */
/* DEC/CMS REPLACEMENT HISTORY, Element WE_PICK_WELL.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Finder Graphics Systems, Inc.
			201 Tamal Vista Blvd
			Corte Madera, CA  USA 94925
			(415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* * FILE DESCRIPTION * */
/* -------------------- */
/* ************************************************************************

   NAME: WE_NEW_PICK_WELL.C

   DESCRIPTION: Function to locate and return the visible well nearest 
		the supplied supplied point (pick_x,pick_y).

   ************************************************************************ */




/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_math.h"
#include "esi_gb.h"
#include "esi_gb_msg.h"
#include "esi_mp.h"
#include "esi_wl.h"
#include "esi_mg.h"
#include "esi_am.h"
#include "esi_we_wells.h"
#include "esi_md_basis.x"
#include "esi_gks.h"

/* * FUNCTION TITLE * */
/* ------------------ */

publicdef INT we_new_pick_well(pick_x,pick_y,ret_uwi,ret_x,ret_y,top,
				source, label_position, ov_selected)

/* * ARGUMENT LIST *  */
/* ------------------ */

DOUBLE pick_x;				/* X PICK COORD */
DOUBLE pick_y;				/* Y PICK COORD */
UWI ret_uwi;				/* RETURNED UWI */
DOUBLE *ret_x;				/* RETURNED X COORD */
DOUBLE *ret_y;				/* RETURNED Y COORD */
CHAR *top;					/* RETURNED FORMATION NAME */
CHAR * source;
INT *label_position;
INT *ov_selected;			/* OVERLAY THIS DATA CAME FROM */

/* * VARIABLE LIST * */
/* ----------------- */

{

INT status;				/* RETURN STATUS */
INT count;
INT ndx;
INT temp_row;
INT i,j;
INT ov_num;
DOUBLE x,y;
DOUBLE dist;
DOUBLE test_dist;
DOUBLE dist_x;
DOUBLE dist_y;
BOOL found_one = FALSE;
MAP_STRUCTURE *map;
MD_DISPLAY *display;
MD_COMPILED_MAPDEF *compiled;
PICKABLE_WELLS *well_picks;
WELL_STRUCT *pwell;
CHAR msg[240];

/* ********************************************************************** */


/* * CONVERT PICK POINT COORDINATES FROM MAP TO PROJECT COORDINATES * */
/* ------------------------------------------------------------------ */

ct_project_xy(pick_x,pick_y,&x,&y);

/* * TRAVERSE MAP OVERLAY BY OVERLAY * */
/* ----------------------------------- */

mp_front_map(&map);
display = map->display;
compiled = map->compiled;

ov_num = 0;
dist = HUGE_VAL;
temp_row = -1;
for (i=0; i<display->last_row_used; i++)
    {
    if (display->compiled_row[i] != temp_row)
	{
	if ( (temp_row = display->compiled_row[i]) EQUALS -1)
	    {
	    continue;
	    }

	ov_num++;
	if ( (display->overlay_displayed[i])  AND
		    (compiled->verb_code[temp_row] EQUALS MP_WELLDPTH))
	    {
		for (j=0; j<compiled->verb_ptr[temp_row]->nrows; j++)
			{
			if (compiled->verb_ptr[temp_row]->key_code[j] == 19) /* HORIZON */
				{
				strcpy(top,compiled->verb_ptr[temp_row]->parm_ptr[j]->parmval.cval);
				}
			else if (compiled->verb_ptr[temp_row]->key_code[j] == 22) /* source */
				{
				strcpy(source,compiled->verb_ptr[temp_row]->parm_ptr[j]->parmval.cval);
				}
			else if (compiled->verb_ptr[temp_row]->key_code[j] == 21) /* label position */
				{
				*label_position = compiled->verb_ptr[temp_row]->parm_ptr[j]->parmval.ival;
				}
			}

	    well_picks = (PICKABLE_WELLS *) compiled->picks[temp_row];
	    if (well_picks->seg_id != 0)
		{
		switch (wm_inq_seg_vis(well_picks->seg_id))
		    {
		    case GVISI:
			for (j=0; j<well_picks->nused; j++)
			    {
			    pwell = well_picks->list[j].pWell;

			    dist_x = x - pwell->bottomhole_xy.x;
			    dist_y = y - pwell->bottomhole_xy.y;
			    test_dist = (dist_x * dist_x) + (dist_y * dist_y);

			    if (test_dist < dist)
				{
				dist = test_dist;
				found_one = TRUE;
				count = temp_row;
				ndx = j;
				}
			    }
			break;

		    case FAIL:
			sprintf(msg,mg_message(GB_CANT_GET_LIST),"WELLS",ov_num,
				    overlay_verbs[display->verb_ndx[i]].verb);
			am_message(AM_ALERT,msg);
			break;

		    }
		}
	    }
	}
    }


/* * RETURN FOUND WELL OR ERROR STATUS * */
/* ------------------------------------- */

if (found_one)
    {
    status = SUCCESS;
    well_picks = (PICKABLE_WELLS *) compiled->picks[count];
    pwell = well_picks->list[ndx].pWell;
    ct_map_xy(pwell->bottomhole_xy.x,pwell->bottomhole_xy.y,ret_x,ret_y);
    strcpy(ret_uwi,pwell->uwi);
	*ov_selected = count;
    }

else
    {
    status = FAIL;
    }

return status;
}
