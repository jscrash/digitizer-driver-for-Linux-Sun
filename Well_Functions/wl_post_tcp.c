/* DEC/CMS REPLACEMENT HISTORY, Element WL_POST_TCP.C */
/* *1    28-SEP-1990 11:07:26 GILLESPIE "Beta Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element WL_POST_TCP.C */
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

/*****************************************************************************
******************************************************************************/

#ifndef ESI_WL_h

#include "esi_wl.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to post tcp on gks

Prototype:
   publicdef INT wl_post_tcp(DOUBLE x, DOUBLE y, DOUBLE text_size,
       INT text_position, TCP well_tcp);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(DOUBLE) x coordinate
    y               -(DOUBLE) y coordinate 
    text_size       -(DOUBLE) text height
    text_position   -(INT) text position
    well_tcp        -(TC) well tcp

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/
/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_post_tcp(DOUBLE x, DOUBLE y, DOUBLE text_size, 
			  INT text_position, TCP well_tcp)
#else
publicdef INT wl_post_tcp(x,y,text_size,text_position,well_tcp)
DOUBLE x, y, text_size;
INT text_position;
TCP well_tcp;
#endif
{
  INT i, tcp_len = ts_tcp_len(well_tcp);

  /* text_size increase to actually text size plus spacing */
  text_size *= 1.2; 
  switch (text_position) 
    {
      /* the number 1 - 5 definition is according to wl_post_well.c */
	case 1: /* above */
      y += text_size * (tcp_len-1);  /* make starting point of y  higher */
      break;
	case 2: /* below */
      break;
	case 3: /* centered */      
	case 4: /* left */
	case 5: /* right */
      y += text_size * (tcp_len-1) / 2.0;
      break;
    }

  for (i=0; i<tcp_len; i++, y-=text_size)
    {
      /* put the string on gks */
      wm_gtx(x,y,well_tcp[i]); 
    }

  /* free tcp if necessary */
  if (tcp_len > 0) 
    {
      ts_tcp_free(well_tcp);
    }

  return(SUCCESS);
}









