/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_XS.C */
/* *2    27-MAR-1991 13:28:04 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter" */
/* *1    12-OCT-1990 17:43:36 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_XS.C */
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

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif

#ifndef ESI_XS_GR_H
#include "esi_xs_gr.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_xs (PL_BASE_WINDOW *bw)
#else
publicdef INT pl_draw_xs (bw)
PL_BASE_WINDOW *bw;
#endif
{
  INT	n = 0;
  INT	status = SUCCESS;
  INT	nlayouts = 0;
  DOUBLE min_char_height;
  LAYOUT_STRUCT  *pls;
  XS_DIS *pdraw;      /* Drawinfo struct.  See ESI_XS_GR.H for desc. */
  XS_STRUCT **pxs;

  status = am_open_workspace ("XS_STRUCT",AM_APPLICATION,(VOIDPTR *) &pxs);

  (*pxs)->drawing.tnr_id = bw->tnr_id;
  (*pxs)->drawing.window_id = bw->win_id;

  nlayouts = llcount(&(*pxs)->drawing.log_plots);	/* COUNT WELLS & CHECK LISTS */
  if (nlayouts <= 0)
    return FAIL;

/*
*** FILL OUT THE DRAW_INFO STRUCTURE ONCE JUST BEFORE DRAWING 
*/
  pdraw = (XS_DIS *)tc_zalloc(sizeof(XS_DIS));
  if ((status = xs_gr_get_drawinfo(*pxs,pdraw)) == FAIL)
    return FAIL;


  status = xs_get_hdr_min_height(*pxs, &min_char_height);

/*
*** LOOP ON WELLS DRAWING EACH ONE AND IT'S COMPONENTS 
*/

  for (n = 0; n < nlayouts; n++)
    {
    status = (n EQUALS 0) ? llfirst(&(*pxs)->drawing.log_plots, (BYTE *)&pls) :
                            llnext(&(*pxs)->drawing.log_plots, (BYTE *)&pls);

    pls->log_hdr.tnr_id = (*pxs)->drawing.tnr_id;	/* PROPAGATE XFORM ID */

			    /* EACH WELL IN ONE SEGMENT FOR PLOTTING */
    status = wmsallocate(&pls->seg_id);
    status = wmsvis(pls->seg_id, GVISI);
	
	        
/*
**** DRAW THE TEXT BLOCK *****
*/	
    if (pls->text_block.visible EQUALS TRUE)
      {
      status = xs_gr_draw_text_blk(*pxs,pls);	
      }



/*
****    DRAW TRACKS AND TRACES

*/	
    status = xs_gr_draw_tracks(*pxs,pls,TRUE);	/* DO ALL TRACKS ON THIS WELL */
    status = xs_gr_draw_traces(*pxs,pls);	/* AND ALL THE TRACES */



/*
****  DRAW THE TRACE SCALE BLOCK
*/	
    if (pls->scale_block.visible EQUALS TRUE)
      {
      status = lg_gr_plot_draw_scale(*pxs,pls,pdraw);    
      }


/*
****  DRAW THE LOG HEADER BLOCK
*/	
    if (pls->log_hdr.visible EQUALS TRUE)
      {
	status = xs_gr_plot_draw_hdr(*pxs, pls, min_char_height);	
      }
	
	
    status = wmsclose(pls->seg_id);    
    }        


  status = xsz_display_tops(*pxs, min_char_height);	
                                /* DONE WITH LAYOUTS.  DRAW THE VISIBLE TOPS */
  if ((*pxs)->xs_suppress_vbar EQUALS FALSE)
    status = xs_gr_vbar(*pxs);	/* DRAW SCALE BAR IF NOT NO_DATUM SECTION */


#if 0
/* NAME THE WINDOW */
  status = wmoset((*pxs)->drawing.window_id, WM_CLASS_WINDOW, "","Cross Section Window");
#endif

  return SUCCESS;
}

