/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_WIN.C */
/*  4     3-MAR-1992 13:20:50 JILL "(SPR 0) Add switch/handler for GPD types" */
/* *3     6-JUN-1991 16:49:03 MING "(SPR 0) new plot file management" */
/* *2    15-OCT-1990 12:18:17 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:42:25 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_WIN.C */
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



/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT func (arg);

Parameters:	    (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) arg	- description

Return Value/Status:
    SUCCESS	- Successfull completion
    FAIL	- Unsuccessfull completion

Scope:
    PUBLIC
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/


#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#if USE_PROTOTYPES 
publicdef INT pl_draw_win (PLOT_LAYOUT *pl, PL_BASE_WINDOW *bw,
			   CHAR *plot_file, CHAR *meta_type)
#else
publicdef INT pl_draw_win (pl, bw, plot_file, meta_type)
PLOT_LAYOUT *pl;
PL_BASE_WINDOW *bw;
CHAR *plot_file;
CHAR *meta_type;
#endif
{
  INT status = SUCCESS;
  INT color;
  INT win_id;
  INT tnr_id;
  INT i,j;
  LINE_ATTRIBUTE line;
  GKS_REAL ll_ndc_x,ll_ndc_y,ur_ndc_x,ur_ndc_y;
  DOUBLE plot_range;
  DOUBLE width;
  DOUBLE height;
  CHAR scale[80];

  
  status = ug_init_plot_gks (plot_file,meta_type);
  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"ug_init_plot_gks fail");
    return FAIL;
    }

/* -------------------------- */
/* Compute the max plot range */
/* -------------------------- */
  if (pl->width > pl->height)
    {
    plot_range = pl->width;
    }
  else
    {
    plot_range = pl->height;
    }

/* ------------------ */
/* Get the ndc extent */
/* ------------------ */
  ll_ndc_x = 0.0;
  ll_ndc_y = 0.0;
  ur_ndc_x = pl->width / plot_range;
  ur_ndc_y = pl->height / plot_range;


/* ----------- */
/* draw border */
/* ----------- */
  wmwallocate (&(pl->win_id),ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
  line.style = 1;
  lu_text_to_index ("COLOR",&line.color,"BLACK");
  line.width = 1.0;
  wmtallocate (&(pl->tnr_id),0.0,1.0,0.0,1.0,0.0,pl->width,0.0,pl->height);

  pl_draw_border (pl->border,pl->width,pl->height);

  wmwclose (pl->win_id);

/* ---------------- */
/* draw base window */
/* ---------------- */
  ll_ndc_x = bw->x / plot_range;
  ll_ndc_y = bw->y / plot_range;
  ur_ndc_x = (bw->x + bw->width) / plot_range;
  ur_ndc_y = (bw->y + bw->height) / plot_range;
  wmwallocate (&(bw->win_id),ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
  wmtallocate (&(bw->tnr_id),0.0,1.0,0.0,1.0,bw->ll_wcs_x,bw->ur_wcs_x,
					   bw->ll_wcs_y,bw->ur_wcs_y);
  switch (bw->type)
    {
    case PL_MAP:
      pl_draw_map (bw);
      break;

    case PL_XSECT:
      pl_draw_xs (bw);
      break;
    }
  wmwclose (bw->win_id);

  for (i = 0; i < 4; ++i)
    {
    for (j = 0; j < pl->zone[i].nwins; ++j)
      {
/* -------------------------------------- */
/* if window is not dropped, then draw it */
/* -------------------------------------- */
      if (pl->zone[i].win[j].drop == FALSE)
	{
/* ----------- */
/* draw border */
/* ----------- */
        width = pl->zone[i].win[j].plot_width + 2*pl->zone[i].win[j].plot_margin;
        height = pl->zone[i].win[j].plot_height + 2*pl->zone[i].win[j].plot_margin;

        ll_ndc_x = pl->zone[i].win[j].x / plot_range;
        ll_ndc_y = pl->zone[i].win[j].y / plot_range;
        ur_ndc_x = (pl->zone[i].win[j].x + width) / plot_range;
        ur_ndc_y = (pl->zone[i].win[j].y + height) / plot_range;

        wmwallocate (&win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
        wmtallocate (&tnr_id,0.0,1.0,0.0,1.0,0.0,width,0.0,height);

	pl_draw_border (pl->zone[i].win[j].border,width,height);

	wmwclose (win_id);

/* -------------------------------------- */
/* Calculate the ndc extent of the window */
/* -------------------------------------- */
        ll_ndc_x = (pl->zone[i].win[j].x + pl->zone[i].win[j].plot_margin)
		   / plot_range;
        ll_ndc_y = (pl->zone[i].win[j].y + pl->zone[i].win[j].plot_margin) 
		   / plot_range;
        ur_ndc_x = (pl->zone[i].win[j].x + pl->zone[i].win[j].plot_width + 
		   pl->zone[i].win[j].plot_margin) / plot_range;
        ur_ndc_y = (pl->zone[i].win[j].y + pl->zone[i].win[j].plot_height +
		   pl->zone[i].win[j].plot_margin) / plot_range;


        switch (pl->zone[i].win[j].window_type)
          {
	  case PL_MAP:
/* ---------------------------------------------------------------------------- */
/* allocate window,transformation inside the draw index map routine, since it   */
/* is the only place we can get the wcs extent information and the aspect ratio */
/* ---------------------------------------------------------------------------- */
	    pl_draw_map_win  ( bw->type,
			       pl->zone[i].win[j].window_name,
			       pl->zone[i].win[j].plot_width,
			       pl->zone[i].win[j].plot_height,
			       pl->zone[i].win[j].x+pl->zone[i].win[j].plot_margin,
			       pl->zone[i].win[j].y+pl->zone[i].win[j].plot_margin,
			       plot_range);
            break;

	  case PL_INDEX_MAP:
/* ----------------------------------------------------------------------------- */
/* allocate window,transformation inside the draw index map routine, since it    */
/* is the  only place we can get the wcs extent information and the aspect ratio */
/* ----------------------------------------------------------------------------- */
	    pl_draw_index_map (bw->type,
			       pl->zone[i].win[j].window_name,
			       pl->zone[i].win[j].plot_width,
			       pl->zone[i].win[j].plot_height,
			       pl->zone[i].win[j].x+pl->zone[i].win[j].plot_margin,
			       pl->zone[i].win[j].y+pl->zone[i].win[j].plot_margin,
			       plot_range);
            break;

          case PL_TTLBLK:
            wmwallocate (&pl->zone[i].win[j].win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
            wmtallocate (&pl->zone[i].win[j].tnr_id,0.0,1.0,0.0,1.0,0.0,
	     pl->zone[i].win[j].plot_width,0.0,pl->zone[i].win[j].plot_height);
	    pl_draw_ttlblk (bw,
			    pl->zone[i].win[j].window_name,
			    pl->zone[i].win[j].plot_width,
			    pl->zone[i].win[j].plot_height);
	    wmwclose (pl->zone[i].win[j].win_id);
	    break;

          case PL_NORTH_ARROW:
            wmwallocate (&pl->zone[i].win[j].win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
            wmtallocate (&pl->zone[i].win[j].tnr_id,0.0,1.0,0.0,1.0,0.0,
	     pl->zone[i].win[j].plot_width,0.0,pl->zone[i].win[j].plot_height);
	    pl_draw_arrow (pl->zone[i].win[j].plot_width,
	                    pl->zone[i].win[j].plot_height);
	    wmwclose (pl->zone[i].win[j].win_id);
	    break;
         
	  case PL_SCALE_BAR:
            wmwallocate (&pl->zone[i].win[j].win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
            wmtallocate (&pl->zone[i].win[j].tnr_id,0.0,1.0,0.0,1.0,0.0,
	     pl->zone[i].win[j].plot_width,0.0,pl->zone[i].win[j].plot_height);
            sprintf (scale,"%.2f %s",bw->scale,bw->scale_uom);
	    pl_draw_bar (pl->zone[i].win[j].window_name,
			 scale,
			 pl->zone[i].win[j].plot_width,
	                 pl->zone[i].win[j].plot_height);
	    wmwclose (pl->zone[i].win[j].win_id);
	    break;

          case PL_WELL_LEGEND:
            wmwallocate (&pl->zone[i].win[j].win_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
            wmtallocate (&pl->zone[i].win[j].tnr_id,0.0,1.0,0.0,1.0,0.0,
	     pl->zone[i].win[j].plot_width,0.0,pl->zone[i].win[j].plot_height);
            sprintf (scale,"%.2f %s",bw->scale,bw->scale_uom);
	    pl_draw_well_symbol (pl->zone[i].win[j].plot_width,
	                         pl->zone[i].win[j].plot_height);
	    wmwclose (pl->zone[i].win[j].win_id);
	    break;

          case PL_METAFILE:
	    pl_draw_metafile (pl->zone[i].win[j].window_name,ll_ndc_x,ur_ndc_x,
	                                                     ll_ndc_y,ur_ndc_y);
	    break;

          case PL_XSECT:
	    pl_draw_xs_win (&pl->zone[i].win[j],pl);
	    break;

          }
        }
      }
    }

    status = ug_term_plot_gks ();
}

