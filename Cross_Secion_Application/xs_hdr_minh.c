/* DEC/CMS REPLACEMENT HISTORY, Element XS_HDR_MINH.C */
/* *3    25-JUL-1991 11:00:03 KEE "(SPR -1) Fix Header Info Splills over boundary of header box, case 3309" */
/* *2    27-MAR-1991 13:29:58 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter" */
/* *1    27-MAR-1991 12:47:54 KEE "Get log header text minimum height" */
/* DEC/CMS REPLACEMENT HISTORY, Element XS_HDR_MINH.C */
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_xs_gr.h"

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#define FIT_TEXT 0      /* 1 to massage text size, 0 for default */

/* Function Description -----------------------------------------------------
Description:
    Cross section graphics slave routine; Draw a logplot header block.
    This routine requires that all of the positioning, scaling & 
    text item variables have been resolved in the XS_STRUCT.
    The text within the header block is 'massaged' to fit the 
    rectangular region within the header block.  A two line space
    is left at the bottom for drawing a well symbol.

    Get the minimum character height of all the track logplot header block.

Prototype:
    publicdef INT xs_get_hdr_min_height(XS_STRUCT *pxs, DOUBLE *char_height);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) Cross section object.
    (O) char_height -(DOUBLE *) Minimum character height.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_get_hdr_min_height(XS_STRUCT *pxs, DOUBLE *char_height)
#else
publicdef INT xs_get_hdr_min_height(pxs, char_height)
XS_STRUCT *pxs;
DOUBLE *char_height;
#endif
    {
    INT text_nlines;                   /* linked list count */
    INT status = SUCCESS;
    INT i, n, nlayouts;
    DOUBLE xmin, xmax, ymin, ymax,     /* positioning  */
           txmin, txmax, tymin, tymax, /* text positioning */
           max_char_height,            /* max char height */
           loc_char_height,            /* local char height */
           char_size,                  /* GKS character size */
           tinc,                       /* distance between drawn text lines */
           dx, dy,                     /* x y difference */
           temp;
    LAYOUT_STRUCT *pls;
    CHAR *str;

    *char_height = 99999.0;
    if (pxs == (XS_STRUCT *)0)
      return FAIL;
    
    nlayouts = llcount(&pxs->drawing.log_plots); /* COUNT WELLS & CHECK LISTS */
    if (nlayouts <= 0)
      return FAIL;
    
    for (n = 0; n < nlayouts; n++)
      {
	status = (n EQUALS 0) ? 
	  llfirst(&pxs->drawing.log_plots, (BYTE *)&pls) :
	    llnext(&pxs->drawing.log_plots, (BYTE *)&pls);

	/* determine if the entry conditions are good */
	if (pls == (LAYOUT_STRUCT *)0)
	  return FAIL;
	
	xmin = pls->log_hdr.xorigin;
	xmax = xmin + pls->log_hdr.width;

	ymin = pls->log_hdr.yorigin;
	ymax = ymin + pls->log_hdr.height;
    
	/* these deltas describe the height and width, they are used everywhere */
	dx = xmax - xmin;
	dy = ymax - ymin;
    
	tymin = ymin + (dy * RCH);
	tymax = ymax - (dy * RCH);
	txmin = xmin + (dx * RCH);
	txmax = xmax - (2 * dx * RCH); /*  times 2 to insure enough room for
					   the string */
    
	/* count how many lines of text will be printed/fitted in the box */
	text_nlines = llcount (&pls->log_hdr.hdr_items);

#if 0
	/* calculate max height for a string, so it will not be drawn off the border */
	temp = text_nlines + (0.5 * (text_nlines - 1));
	max_char_height = (tymax - tymin) / temp;
#endif
	char_size = pls->log_hdr.text_height;
	
	/* this switch determines whether we massage the text to fit */
	/* or just use a default expansion factor and hope for the best! */
#if FIT_TEXT == 1
	
	/* calculate a width factor to fit the box */
	/* this routine determines the longest string in the list and fits */
	/* that string to the area of the header.   */
#if 0
	temp = tymin + max_char_height;
#endif 
	status = tg_fit_text (&pls->log_hdr.hdr_items, txmin, tymin, txmax, 
			      0.0, char_size, &loc_char_height);
	if (status == FAIL)
	  { /* getting a bad return status here is very unlikely */
	    /* but should it happen it would be very bad since */
	    return FAIL;                
	  }
	/* the status bubbles up directly from GKS */
#else
	loc_char_height = char_size;        /* force the characters to a 1/1 ratio, and fixed size */

	for (i = 0; i < text_nlines; i++)
	  {
	    status = (i == 0) ? llfirst (&pls->log_hdr.hdr_items,(BYTE *) & str) : 
	      llnext (&pls->log_hdr.hdr_items, (BYTE *) & str);
        
	    if (status != FAIL AND (status >= 0))
	      {
		str = ts_snowhite (str);
		if (strlen (str) > 0) 
		  {
		    loc_char_height = MIN( loc_char_height, 
					  MIN( (tymax-tymin)/text_nlines , 
					      (txmax-txmin)/strlen(str)));
		  }
	      }
	  }
#endif
	
	/* this is the distance in Y between strings when drawn */
	tinc = loc_char_height + (loc_char_height * 0.45);
	
	if ((tinc * text_nlines) > dy)
	  {
	    loc_char_height *= (dy / (tinc * text_nlines));
	    tinc = loc_char_height +(loc_char_height * 0.45);
	  }
	if (loc_char_height < *char_height)
	  {
	    *char_height = loc_char_height;
	  }
      }

    return SUCCESS;
    }
/* END:     */






