/* DEC/CMS REPLACEMENT HISTORY, Element WE_GET_WELL_XY.C */
/* *2    11-MAR-1991 11:05:43 KEE "(SPR -1) Totally modify Charlie Purmoy's Edit Well Depth Application base on QA new */
/*specifications" */
/* *1    11-MAR-1991 11:02:06 KEE "Get Well xy coordinate for edit well depth application" */
/* DEC/CMS REPLACEMENT HISTORY, Element WE_GET_WELL_XY.C */
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

#ifndef ESI_GL_DEFS_H
#include "esi_gl_defs.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_WL_H
#include "esi_wl.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

/* * FUNCTION TITLE * */
/* ------------------ */

publicdef INT we_get_well_xy(f_pick_x, f_pick_y, f_ret_x, f_ret_y, well_picks, 
			      well_pick_ndx)

/* * ARGUMENT LIST *  */
/* ------------------ */

FLOAT f_pick_x;
FLOAT f_pick_y;
FLOAT *f_ret_x;
FLOAT *f_ret_y;
PICKABLE_WELLS *well_picks;
INT *well_pick_ndx;

/* * VARIABLE LIST * */
/* ----------------- */

{

  INT status;	/* RETURN STATUS */
  INT j;
  INT row;
  DOUBLE pick_x = (DOUBLE) f_pick_x;
  DOUBLE pick_y = (DOUBLE) f_pick_y;
  DOUBLE ret_x;
  DOUBLE ret_y;
  DOUBLE x, y;
  DOUBLE dist = HUGE_VAL;
  DOUBLE test_dist;
  DOUBLE dist_x;
  DOUBLE dist_y;
  BOOL found_one = FALSE;
  CHAR msg[240];

/* ********************************************************************** */


/* * CONVERT PICK POINT COORDINATES FROM MAP TO PROJECT COORDINATES * */
/* ------------------------------------------------------------------ */

  ct_project_xy(pick_x, pick_y, &x, &y);
  *well_pick_ndx = -1;
  if (well_picks->seg_id != 0)
    {
      status = wm_inq_seg_vis(well_picks->seg_id);
      if (status == GVISI)
	{
	  for (j = 0; j < well_picks->nused; j++)
	    {
	      dist_x = x - well_picks->list[j].pWell->bottomhole_xy.x;
	      dist_y = y - well_picks->list[j].pWell->bottomhole_xy.y;
	      test_dist = (dist_x * dist_x) + (dist_y * dist_y);
	      if (test_dist < dist)
		{
		  *well_pick_ndx = j;
		  dist = test_dist;
		}
	    }
	  status = SUCCESS;
	}
    }

/* * RETURN FOUND WELL OR ERROR STATUS * */
/* ------------------------------------- */
  if (*well_pick_ndx != -1)
    {
      ct_map_xy(well_picks->list[*well_pick_ndx].pWell->bottomhole_xy.x, 
		well_picks->list[*well_pick_ndx].pWell->bottomhole_xy.y, 
		&ret_x, &ret_y);
      *f_ret_x = (FLOAT) ret_x;
      *f_ret_y = (FLOAT) ret_y;
    }
  return status;
}

