/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_XS_WIN.C */
/* *3    27-MAR-1991 13:28:09 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter" */
/* *2    15-OCT-1990 17:23:33 GILLESPIE "(SPR 1000) Merge Ernie deltas" */
/* *1    12-OCT-1990 17:44:09 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_XS_WIN.C */
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

#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif

#ifndef ESI_XS_GR_H
#include "esi_xs_gr.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#if USE_PROTOTYPES
static INT plz_parse_xs_win_name (CHAR *win_name,CHAR *xs_lyt_name,
				      CHAR *project_name, CHAR *analyst_name,
				      CHAR *selected_phrase);
#else
static INT plz_parse_xs_win_name ();
#endif



#if USE_PROTOTYPES
publicdef INT pl_draw_xs_win (PLOT_LAYOUT_WIN *win, PLOT_LAYOUT *pl)
#else
publicdef INT pl_draw_xs_win (win,pl)
PLOT_LAYOUT_WIN *win;
PLOT_LAYOUT *pl;
#endif
{
  INT status = SUCCESS;
  INT n = 0;
  INT nlayouts = 0;
  DOUBLE plot_range;
  DOUBLE actual_width, actual_height;
  DOUBLE diff;
  DOUBLE ll_ndc_x,ll_ndc_y,ur_ndc_x,ur_ndc_y;
  DOUBLE xs_ratio;
  DOUBLE window_ratio;
  DOUBLE min_char_height;
  CHAR xs_lyt_name[80];
  PROJECT_NAME project_name;
  ANALYST_NAME analyst_name;
  CHAR selected_phrase[256];
  XS_STRUCT *pxs;
  LAYOUT_STRUCT  *pls;
  XS_DIS *pdraw;      /* Drawinfo struct.  See ESI_XS_GR.H for desc. */

/* -------------------------------------------------------------------- */
/* for cross section window, the window name is used to stored the      */
/* xs_layout_name, project name, analyst name , and the selected phrase */
/* so the window name should be parsed to get all those information     */
/* -------------------------------------------------------------------- */

  status = plz_parse_xs_win_name (win->window_name,
				  xs_lyt_name,
				  project_name,
				  analyst_name,
				  selected_phrase);

  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Invalid cross section window name.");
    return FAIL;
    }

  status = am_define_workspace ("XS_WIN",AM_APPLICATION,
				   sizeof (XS_STRUCT),(VOIDPTR *)&pxs);

  pxs->ptemp = (LAYOUT_STRUCT *) am_allocate (AM_SYSTEM,sizeof(LAYOUT_STRUCT));
  pxs->plo = (LAYOUT_STRUCT *) am_allocate (AM_SYSTEM,sizeof(LAYOUT_STRUCT));

  if (pxs->ptemp == (LAYOUT_STRUCT *)NULL OR pxs->plo   == (LAYOUT_STRUCT *)NULL)
    {
    am_message (AM_STATUS,"Fail to allocate cross section memory.");
    return FAIL;
    }

  status = xsz_init_struct (pxs);

  status = xsz_free_top_table (pxs);

  status = xs_get_layout (pxs,xs_lyt_name,project_name,analyst_name);

  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Fail to retrieve layout.");
    return FAIL;
    }

  status = sl_read_list (WELLS_DATA_TYPE,selected_phrase,&pxs->well_list);

  if (status != SUCCESS)
    {
    am_message (AM_STATUS,"Fail to retrieve selected list.");
    return FAIL;
    }

  status = xs_free_struct (&pxs->drawing.log_plots);

  status = xs_create_drawing (pxs);

  status = xs_copy_well_info (pxs);

  status = xs_chg_drawing (pxs);

  nlayouts = llcount(&pxs->drawing.log_plots);	/* COUNT WELLS & CHECK LISTS */
  if (nlayouts <= 0)
    return FAIL;

/*
*** FILL OUT THE DRAW_INFO STRUCTURE ONCE JUST BEFORE DRAWING 
*/
  pdraw = (XS_DIS *)tc_zalloc(sizeof(XS_DIS));
  if ((status = xs_gr_get_drawinfo(pxs,pdraw)) == FAIL)
    return FAIL;


/* -------------------------- */
/* calculate aspect ratio     */
/* -------------------------- */
  xs_ratio = pxs->drawing.height / pxs->drawing.width;
  window_ratio = win->plot_height / win->plot_width;

  plot_range = (pl->width > pl->height) ? pl->width : pl->height;

  if (xs_ratio > window_ratio)
    {
    /* ---------- */
    /* Use full Y */
    /* ---------- */
    actual_width = win->plot_height / xs_ratio;
    actual_height = win->plot_height;
    diff = win->plot_width - actual_width;
    ll_ndc_x = (win->x + diff/2) / plot_range;
    ll_ndc_y = win->y / plot_range;
    ur_ndc_x = (win->x + diff/2 + actual_width) / plot_range;
    ur_ndc_y = (win->y + actual_height) / plot_range;
    }
  else
    {
    /* ---------- */
    /* Use full X */
    /* ---------- */
    actual_width = win->plot_width;
    actual_height = win->plot_width * xs_ratio;
    diff = win->plot_height - actual_height;
    ll_ndc_x = win->x / plot_range;
    ll_ndc_y = (win->y + diff/2) / plot_range;
    ur_ndc_x = (win->x + actual_width) / plot_range;
    ur_ndc_y = (win->y + diff/2 + actual_height) / plot_range;
    }


  wmwallocate (&pxs->drawing.window_id,ll_ndc_x,ur_ndc_x,ll_ndc_y,ur_ndc_y,-1);
  wmtallocate (&pxs->drawing.tnr_id,0.0,1.0,0.0,1.0,0.0,pxs->drawing.width,
					   0.0,pxs->drawing.height);

  status = xs_get_hdr_min_height(pxs, &min_char_height);
/*
*** LOOP ON WELLS DRAWING EACH ONE AND IT'S COMPONENTS 
*/
  for (n = 0; n < nlayouts; n++)
    {
    status = (n EQUALS 0) ? llfirst(&pxs->drawing.log_plots, (BYTE *)&pls) :
                            llnext(&pxs->drawing.log_plots, (BYTE *)&pls);

    pls->log_hdr.tnr_id = pxs->drawing.tnr_id;	/* PROPAGATE XFORM ID */

			    /* EACH WELL IN ONE SEGMENT FOR PLOTTING */
    status = wmsallocate(&pls->seg_id);
    status = wmsvis(pls->seg_id, GVISI);
	
	        
/*
**** DRAW THE TEXT BLOCK *****
*/	
    if (pls->text_block.visible EQUALS TRUE)
      {
      status = xs_gr_draw_text_blk(pxs,pls);	
      }



/*
****    DRAW TRACKS AND TRACES
*/	
    status = xs_gr_draw_tracks(pxs,pls,TRUE);	/* DO ALL TRACKS ON THIS WELL */
    status = xs_gr_draw_traces(pxs,pls);	/* AND ALL THE TRACES */



/*
****  DRAW THE TRACE SCALE BLOCK
*/	
    if (pls->scale_block.visible EQUALS TRUE) 
      {
      status = lg_gr_plot_draw_scale(pxs,pls,pdraw);    
      }


/*
****  DRAW THE LOG HEADER BLOCK
*/	
    if (pls->log_hdr.visible EQUALS TRUE)
      {
      status = xs_gr_plot_draw_hdr(pxs, pls, min_char_height);	
      }
	
	
    status = wmsclose(pls->seg_id);    
    }        


  status = xsz_display_tops(pxs, min_char_height);
                                /* DONE WITH LAYOUTS.  DRAW THE VISIBLE TOPS */
  if (pxs->xs_suppress_vbar EQUALS FALSE)
    status = xs_gr_vbar(pxs);	/* DRAW SCALE BAR IF NOT NO_DATUM SECTION */


/* CLOSE AND NAME THE WINDOW */
  status = wmwclose(pxs->drawing.window_id);	

  return SUCCESS;

}


static INT plz_parse_xs_win_name (win_name,layout_name,project_name,
				 analyst_name,selected_phrase)
CHAR *win_name;
CHAR *layout_name;
CHAR *project_name;
CHAR *analyst_name;
CHAR *selected_phrase;
{
  CHAR *p;
  CHAR *q;

  p = ts_next_token (win_name,layout_name,',');
  if (p == (CHAR *) NULL)
    {
    am_message (AM_STATUS, "Invaild cross section window name.");
    return FAIL;
    }
  q = ts_snowhite (layout_name);
  if (strcmp (q,"") == 0)
    {
    am_message (AM_STATUS, "Invaild cross section window name.");
    return FAIL;
    }

  p = ts_next_token (p,project_name,',');
  if (p == (CHAR *) NULL)
    {
    am_message (AM_STATUS,"Invaild cross section window name.");
    return FAIL;
    }
  q = ts_snowhite (project_name);
  if (strcmp (q,"") == 0)
    {
    am_message (AM_STATUS, "Invaild cross section window name.");
    return FAIL;
    }

  p = ts_next_token (p,analyst_name,',');
  if (p == (CHAR *) NULL)
    {
    am_message (AM_STATUS,"Invaild cross section window name.");
    return FAIL;
    }
  q = ts_snowhite (analyst_name);
  if (strcmp (q,"") == 0)
    {
    am_message (AM_STATUS, "Invaild cross section window name.");
    return FAIL;
    }

  if (p == (CHAR *) NULL || strcmp (p,"") == 0)
    {
    am_message (AM_STATUS,"Invaild cross section window name.");
    return FAIL;
    }
  strcpy (selected_phrase,p);
  ts_snowhite (selected_phrase);

  return SUCCESS;

}
