/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_VBAR.C*/
/*  10   27-FEB-1992 14:53:52 JESSIE "(SPR 6180) TVD display of trace and tops"*/
/*  9    27-FEB-1992 09:24:23 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *8    17-AUG-1990 22:37:07 VINCE "(SPR 5644) Code Cleanup"*/
/* *7     8-MAY-1990 12:01:00 MING "(SPR 0) change float to double to fix crash"*/
/* *6    23-APR-1990 12:28:27 JULIAN "(SPR 0) fix text aspect ratio"*/
/* *5    20-APR-1990 18:30:52 JULIAN "(SPR 0) fix text leaking"*/
/* *4    18-APR-1990 11:55:47 JULIAN "(SPR 0) fix casting"*/
/* *3    18-APR-1990 10:06:35 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *2    14-SEP-1989 16:23:44 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *1    19-JUN-1989 13:43:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_VBAR.C*/
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

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#ifndef ESI_LU_H

#include "esi_lu.h"

#endif

#define DEBUG 0    /* 1 = print diagnostics to message window */
#define USE_SEG 1  /* 1 = draw into segment, 0 = No segments  */

/* Function Description -----------------------------------------------------
Description:
    This routine draws a vertical scale bar on stratigraphic & structural
    cross sections.  Above the vertical scale bar, the scale string is drawn
    and below the scale bar, the datum name is displayed.

Prototype:
    publicdef INT xs_gr_vbar(XS_STRUCT  *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) The Cross Section object.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xs_gr_vbar (XS_STRUCT *pxs)
#else
publicdef INT xs_gr_vbar (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT depth_here, status, step_size, itop, ibase, old_itop, old_ibase;
    INT sc_seg_id;
    COLOR_INDEX sc_text_color;
    INT font, prec;
    LINE_ATTRIBUTE sc_line;
    
    CHAR string[25], string2[25], datum_string[255];
    
    FLOAT xarray[10], yarray[10];
    
    DOUBLE a,                       /* general purpose DOUBLE accumulator      */
        text_y, total_depth, pix_per_ft, char_size, chh, delta_top, delta_base,
        new_top_y, new_base_y, max_limit, min_limit, new_length, scale_str_x,
        scale_str_y, screen_xmax, dist_to_edge, sc_total_width, tick_width, bar_width,
        text_width, a_little_bit, tick_xmin, tick_xmax, bar_xmin, bar_xmax, bar_length,
        text_xmin, text_xmax, sc_upper_limit, sc_lower_limit, sc_base_y,
        sc_top_y, datum_y;
    
    TG_LINE_STRUCT line;            /* line description struct for tg_drawline     */
    
    
    /* check for valid entry conditions        */
    /* there is no scale bar on the NO_DATUM       */
    /* X-section                   */
    if (pxs == ((XS_STRUCT *)0) OR (pxs->xs_type == XS_NO_DATUM))
        {
        return FAIL;
        }
    if (pxs->drawing.sc_upper_limit == 0.0 OR (pxs->drawing.sc_lower_limit == 0.0))
        {
#if DEBUG == 1
        printf ("pxs->drawing.sc_upper/lower limits were zero!\n");
#endif
        return FAIL;
        }
    /* set the x-section def. transformation       */
    wmtselect (pxs->drawing.tnr_id);
    
    /* work out how much real estate is for the    */
    /* scale bar                   */
    screen_xmax = pxs->drawing.xorigin + pxs->drawing.width;
    dist_to_edge = screen_xmax - pxs->drawing.sc_bar_xorigin;
    sc_total_width = pxs->drawing.width * 0.035;
    sc_total_width = MIN (dist_to_edge, sc_total_width);
    
    /* then split it up among the three components */
    tick_width = sc_total_width * 0.2;
    bar_width = sc_total_width * 0.1;
    text_width = sc_total_width * 0.6;
    
    /* the space left over remains 'air'  for      */
    /* neatness                    */
    a_little_bit = sc_total_width * 0.01;
    
    /* set the positions of the parts          */
    tick_xmin = pxs->drawing.sc_bar_xorigin;
    tick_xmax = tick_xmin + tick_width;
    
    bar_xmin = tick_xmax;
    bar_xmax = bar_xmin + bar_width;
    bar_length = pxs->drawing.sc_bar_length;
    
    text_xmin = bar_xmax + a_little_bit;
    text_xmax = text_xmin + text_width;
    
    sc_base_y = pxs->drawing.sc_bar_yorigin;
    sc_top_y = bar_length + sc_base_y;
    
    if (bar_length <= 0.0)
        return FAIL;                /* WHAT !                  */
    
    lu_text_to_index ("COLOR", (INDEX *)&sc_line.color, "CYAN");
    wm_gsplci (sc_line.color);
    
    lu_text_to_index ("COLOR", (INDEX *)&sc_text_color, "NORMAL");
    wm_gstxci (sc_text_color);
    
    sc_line.width = 1.0;            /* normal                      */
    wm_gslwsc (sc_line.width);      /* set poly-line width             */
    
    sc_line.style = 1;               /* normal, solid line              */
    wm_gsln (sc_line.style);
    
    /* set the visible state to TRUE for future    */
    /* draw (expansion)                */
    /* set the upper and lower depths          */
    sc_upper_limit = pxs->drawing.sc_upper_limit;
    sc_lower_limit = pxs->drawing.sc_lower_limit;
    
    if (sc_upper_limit == 0.0 OR (sc_lower_limit == 0.0))
        {
#if DEBUG == 1
        printf ("SCALE_BAR: upper or lower limit was 0.0....FAIL!\n");
#endif
        return FAIL;
        }
    max_limit = MAX (sc_upper_limit, sc_lower_limit);
    min_limit = MIN (sc_upper_limit, sc_lower_limit);
    
    /* calc the step size between marks on the     */
    /* bar. (In UNITS, i.e. feet any depth range > */
    /* 5000 will be displayed in 1000 UNIT steps   */
    step_size = 1000;
    
    a = max_limit - min_limit;
    
    if (a <= 5000.0)
        {
        step_size = 500;
        }
    if (a <= 2500.0)
        {
        step_size = 250;
        }
    if (a <= 2000.0)
        {
        step_size = 200;
        }
    if (a <= 1500.0)
        {
        step_size = 150;
        }
    if (a <= 1000.0)
        {
        step_size = 100;
        }
    if (a <= 500.0)
        {
        step_size = 50;
        }
    if (a <= 250.0)
        {
        step_size = 25;
        }
    if (a <= 100.0)
        {
        step_size = 10;
        }
    if (a <= 50.0)
        {
        step_size = 5;
        }
    if (a <= 10.0)
        {
        step_size = 1;
        }
    if (a <= 0.0)                   /* great ride, but if we fall in here were     */
        /* hurt'in                     */
        {
#if DEBUG == 1
        printf ("The step size was less than 1   ERROR\n");
#endif

#if USE_SEG == 1
        wmsclose (sc_seg_id);
        wmsdelete (sc_seg_id);
#endif
        return FAIL;
        }
    /* cast an integer set of the top,base vars.   */
    /* for later use in a modulo           */
    old_itop = itop = (INT)sc_upper_limit;
    old_ibase = ibase = (INT)sc_lower_limit;
    
    /* crude effective way to get a rounded set of */
    /* scale numbers                   */
    while (ibase % step_size)
        {
        ibase++;
        }
    while (itop % step_size)
        {
        itop--;
        }
    /* determine the raw version of total_depth    */
    /* etc.  so we can have a starting reference   */
    /* point this is important because when we     */
    /* round off the top & base we have to know    */
    /* how much to trim the ymin, ymax of the      */
    /* scale bar skeleton, and to know where to    */
    /* start the actual draw               */
    
    total_depth = max_limit - min_limit;
    if (total_depth == 0.0)
        {
        printf ("total_depth was zero!\n");
        return FAIL;
        }
    /* this is a simple depth to screen Y units    */
    /* transform                   */
    pix_per_ft = bar_length / total_depth;
    
    /* get the amount trimmed in rounding      */
    delta_base = ibase - old_ibase;
    delta_top = old_itop - itop;
    
    /* we are starting to draw things so allocate  */
    /* the segment now                 */
#if USE_SEG == 1
    wmsallocate (&sc_seg_id);       /* this switch makes for easier debugging      */
#endif
    
    /* this char_size will always show the same    */
    /* size letters regardless of scale since it   */
    /* calcs. char_size as a percentage of the     */
    /* overall window                  */
    chh = (pxs->drawing.yorigin + pxs->drawing.height) * 0.013;
    char_size = chh;
    wm_gschh (chh);                 /* set it for GKS                  */
    /* set the text spaceing factor            */
    /*------------------------------*/
    /* set it for GKS               */
    /* set the text spaceing factor */
    /*------------------------------*/
    wm_gschh (chh);
    
    a = 0.125;
    wm_gschsp (a);
    
    a = 1.0;                        /* set the expansion factor            */
    wm_gschxp (a);
    /* from here out we are concerned with drawing */
    /* the ticks and annotation            */
    
    max_limit = MAX (itop, ibase);  /*    get the new max/min limits  */
    min_limit = MIN (itop, ibase);
    
    total_depth = max_limit - min_limit;
    
    if (total_depth == 0.0)
        {
#if DEBUG == 1
        printf ("total_depth is 0  ERROR\n");
#endif
        return FAIL;
        }
    /* this is our new working set after rounding  */
    new_base_y = sc_base_y + (delta_base * pix_per_ft);
    new_top_y = sc_top_y - (delta_top * pix_per_ft);
    new_length = new_top_y - new_base_y;
    
    /* adjust for scaling              */
    new_base_y = new_top_y - (itop - ibase) * (new_length / total_depth);
    
    /* draw the skeleton with the corrected top    */
    line.npts = 5;                  /* and base y coords five points describe the  */
    /* rectangle                   */
    /*--------------------------------------------*/
    /* and base y coords five points describe the */
    /* rectangle                                  */
    /*--------------------------------------------*/
    line.npts = 5;
    
    line.xarray = xarray;
    line.yarray = yarray;
    lu_text_to_index ("COLOR", &line.line.color, "CYAN");
    line.line.style = 1;
    line.line.width = 1.0;
    wm_gsln (line.line.style);
    wm_gslwsc (line.line.width);
    wm_gsplci (line.line.color);
    
    xarray[0] = (FLOAT)bar_xmin;
    yarray[0] = (FLOAT)new_base_y;
    
    xarray[1] = (FLOAT)bar_xmin;
    yarray[1] = (FLOAT)new_top_y;
    
    xarray[2] = (FLOAT)bar_xmax;
    yarray[2] = (FLOAT)new_top_y;
    
    xarray[3] = (FLOAT)bar_xmax;
    yarray[3] = (FLOAT)new_base_y;
    
    xarray[4] = (FLOAT)bar_xmin;
    yarray[4] = (FLOAT)new_base_y;
    
    status = tg_drawline (&line);
    
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    /* set up and draw the depth scale string      */
    scale_str_x = tick_xmin;
    scale_str_x -= (text_width * 2.0);
    scale_str_y = new_top_y + (char_size * 2.0);
    
    /* size the scale string to the available      */
    /* space                       */
    tg_fit_string (pxs->depth_scale_str, scale_str_x, scale_str_y, text_xmax, 0.0, chh,
                   &char_size);
    wm_gschh (char_size);
    
    scale_str_y = new_top_y + (char_size * 2.0);
    wm_gtx (scale_str_x, scale_str_y, pxs->depth_scale_str);
    
    /* draw the datum                  */
    if (pxs->xs_type == XS_STRATIGRAPHIC)
        {
        sprintf (datum_string, "Datum: %s", pxs->xs_datum_select);
        tg_fit_string (datum_string, scale_str_x, scale_str_y, text_xmax, 0.0, chh,
                       &char_size);
        wm_gschh (char_size);
        datum_y = new_base_y - (char_size * 2.0);
        
        wm_gtx (scale_str_x, datum_y, datum_string);
        }
    /* now size the text of the annotation     */
    sprintf (string, "%d", (INT)min_limit); /*    discern which string will take  */
    sprintf (string2, "%d", (INT)max_limit);    /*  the most space when drawn   */
    
    if (strlen (string) > strlen (string2))
        {
        tg_fit_string (string, text_xmin, new_top_y, text_xmax, 0.0, chh, &char_size);
        }
    else
        {
        tg_fit_string (string2, text_xmin, new_top_y, text_xmax, 0.0, chh, &char_size);
        }
    wm_gschh (char_size);
    /* re-configure the control structure used by  */
    /* tg_drawline                 */
    line.npts = 2;
    line.line.color = sc_line.color;
    
    xarray[0] = (FLOAT)tick_xmin;
    xarray[1] = (FLOAT)(tick_xmin + tick_width);
    line.xarray = xarray;
    line.yarray = yarray;
    
    /* draw the ticks and depths           */
    for (depth_here = ibase; depth_here <= itop; depth_here += step_size)
        {
        if (depth_here >= 0.0)
            {
            sprintf (string, " %d", depth_here);    /*  consider the sign   */
            }
        else
            {
            sprintf (string, "%d", depth_here);
            }
        /* given a depth, calculate the screen     */
        /* position                    */
        text_y = new_top_y - ((itop - depth_here) * pix_per_ft);
        
        yarray[0] = yarray[1] = text_y; /*  tick y pos          */
        
        /* line up the text with the tick          */
        text_y -= (char_size * 0.45);   /*    actual text y pos       */
        
        if (text_y < 0.0)
            {
#if DEBUG == 1
            printf ("y was negative ERROR\n");
#endif
            continue;
            }
        wm_gsln (line.line.style);
        wm_gslwsc (line.line.width);
        wm_gsplci (line.line.color);
        tg_drawline (&line);        /* draw tick                   */
        
        wm_gtx (text_xmin, text_y, string); /*        annotate depth     */
        }
#if USE_SEG == 1
    wmsclose (sc_seg_id);
    wmoset (sc_seg_id, WM_CLASS_SEGMENT, "", "Vertical Scale Bar");
#endif
    return SUCCESS;
    }
/* END:     */
