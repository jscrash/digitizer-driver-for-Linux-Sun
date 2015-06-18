/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_HDR.C*/
/* *10    8-AUG-1991 16:09:54 JESSIE "(SPR 0) set char space to default(0.0) instead of 0.12 case#3309"*/
/* *9    25-JUL-1991 10:59:57 KEE "(SPR -1) Fix Header Info Splills over boundary of header box, case 3309"*/
/* *8    27-MAR-1991 13:29:27 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *7     4-JAN-1991 12:22:27 JESSIE "(SPR 6426) adjust text size on header"*/
/* *6    17-AUG-1990 22:38:35 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    23-APR-1990 12:29:01 JULIAN "(SPR 0) fix text aspect ratio"*/
/* *4    20-APR-1990 18:30:59 JULIAN "(SPR 0) fix text leaking"*/
/* *3    18-APR-1990 10:06:54 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *2    14-SEP-1989 16:23:52 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:43:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_HDR.C*/
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

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#include "esi_lu.h"

/* Function Description -----------------------------------------------------
Description:
    Cross section graphics slave routine; Draw a logplot header block.
    This routine requires that all of the positioning, scaling & 
    text item variables have been resolved in the XS_STRUCT.
    The text within the header block is 'massaged' to fit the 
    rectangular region within the header block.  A two line space
    is left at the bottom for drawing a well symbol.

Prototype:
    publicdef INT xs_gr_plot_draw_hdr(XS_STRUCT *pxs,LAYOUT_STRUCT  *pls);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pxs         -(XS_STRUCT *) Cross section object.
    (I) pls         -(LAYOUT_STRUCT *) Data structure for a single well.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_gr_plot_draw_hdr (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, 
				   DOUBLE min_char_height)
#else
publicdef INT xs_gr_plot_draw_hdr (pxs, pls, min_char_height)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
DOUBLE min_char_height;
#endif
    {
    INT text_nlines;                /* linked list count */
    INT font, prec;
    INT status = SUCCESS;
    INT i;
    DOUBLE xmin, xmax,              /* positioning  */
        ymin, ymax, firstline,      /* screen pos. */
        text_xspace,                /* text character spacing factor for GKS */
        textx = 0.0, texty,         /* actual text drawing coords. */
        char_size,                  /* GKS character size */
        float_one = 1.0,
        dx, dy;
    CHAR *str;                      /* pointer to linked list text items */
    COLOR_INDEX  normalColor;
    
    /* determine if the entry conditions are good */
    if (pxs == ((XS_STRUCT *)0) OR (pls == ((LAYOUT_STRUCT *)0)))
        {
        return FAIL;
        }
    /* activate the x-section defualt transformation */
    wmtselect (pxs->drawing.tnr_id);
    
    /* set the color of the poly lines to be drawn for the header box */
    wm_gsplci (pls->log_hdr.line.color);
    
    /* positioning variable initialization */
    xmin = pls->log_hdr.xorigin;
    xmax = xmin + pls->log_hdr.width;
    
    ymin = pls->log_hdr.yorigin;
    ymax = ymin + pls->log_hdr.height;
    
    /* these deltas describe the height and width, they are used everywhere */
    dx = xmax - xmin;
    dy = ymax - ymin;
    
    /* draw a rectangular 'frame' around the header if requested */
    if (pls->log_hdr.visible_border EQUALS TRUE)
        {
        xs_gr_rect (xmin, ymin, xmax, ymax, pls->log_hdr.line.color,
                    pls->log_hdr.line.width);
        }

    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    /* count how many lines of text will be printed/fitted in the box */
    text_nlines = llcount (&pls->log_hdr.hdr_items);
    
    /****************************************************************/
    /* change character space to default(0.0) instead of .12        */
    /* Because .12 is too big to fit a long string.                 */
    /****************************************************************/
    /*text_xspace = 0.12;              this is a convenient knob for adjusting the
    wm_gschsp (text_xspace);         text spacing. (0.10 - 0.25) */

    wm_gschsp (0.0);        /* text spacing. */
    
                                    /* set expansion factor */
    wm_gschxp (float_one );
    
    wm_gschh (min_char_height);         /* update the char_height with our calculated value */
    
    /* set up text stuff to defualt state. The TRUE flag tells it to center */
    /* we are drawing stroke precision text */
    lu_text_to_index ("COLOR", &normalColor, "NORMAL");
    xs_gr_set_text (min_char_height, float_one, normalColor, TRUE);
    
    
    /* this is the first string position */
    firstline = ymax - (min_char_height * 1.05) - (dy * RCH);
    
    /* textx is the actual X position for drawing the text string */
    textx = xmin + (dx * RCH);
    
    /* loop on all the strings, drawing each */
    for (i = 0; i < text_nlines; i++)
        {
        status = (i == 0) ? 
	  llfirst (&pls->log_hdr.hdr_items, (BYTE *) & str) :
	    llnext (&pls->log_hdr.hdr_items, (BYTE *) & str);
        
        /* strip white space space from the string */
        str = ts_snowhite (str);
        
        /* bump the Y pos. to the next screen line */
        texty = firstline - i * min_char_height * 2.0;
        
        /* draw the string */
        wm_gtx (textx, texty, str);
        }
    /* all's well, lets split */
    return SUCCESS;
    }
/* END:     */
