/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_TRK.C*/
/*  16   27-FEB-1992 09:26:50 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/*  15    7-FEB-1992 10:47:24 JEFF "(SPR 5767) fixed grid depth shift bug (see also xs_gr_tools)"*/
/* *14   11-JUN-1991 11:09:11 KEE "(SPR -1) fix interval calculation"*/
/* *13    4-JUN-1991 17:52:29 DOUG "(SPR 2651) Case #2651.  Rewrote logic for drawing logarathmic cycles in the track."*/
/* *12   21-MAY-1991 15:31:11 KEE "(SPR -1) fix interval calculation"*/
/* *11   27-MAR-1991 13:29:32 KEE "(SPR -1) Cross Section bug fixing and enhancement for Canadian Hunter"*/
/* *10   28-SEP-1990 12:04:56 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  7B1  28-SEP-1990 11:09:43 GILLESPIE "Beta Deltas"*/
/* *9    27-AUG-1990 14:10:41 JULIAN "(SPR 0) bug fixes from ernie"*/
/* *8    17-AUG-1990 22:38:51 VINCE "(SPR 5644) Code Cleanup"*/
/* *7    23-APR-1990 12:29:42 JULIAN "(SPR 0) fix text aspect ratio"*/
/* *6    20-APR-1990 18:31:07 JULIAN "(SPR 0) fix text leaking"*/
/* *5    18-APR-1990 10:07:06 JULIAN "(SPR 1) fix depth tracks bad aspect ratio"*/
/* *4     7-MAR-1990 18:13:26 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *3    14-SEP-1989 16:23:59 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/* *2    26-JUN-1989 11:06:17 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:44:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_TRK.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"
#include "esi_xs_gr.h"      /* this includes all the others */

#ifndef ESI_TG_H
#include "esi_tg.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"     /* this is for the segment classes */
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#if USE_PROTOTYPES
static INT zz_depth_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt);
static INT zz_log_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt);
static INT zz_linear_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt);
#else       /* no prototypes */
static INT zz_depth_track ();
static INT zz_log_track ();
static INT zz_linear_track ();

#endif      /* of #if USE_PROTOTYPES */

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Cross Section Graphics slave routine.                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_gr_draw_tracks(XS_STRUCT   *pxs,LAYOUT_STRUCT  *pls,INT plot_flag);  */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT zz_depth_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls,            */
/*        TRACK_STRUCT *pt);                                                        */
/*                                                                                  */
/*    static INT zz_log_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls,              */
/*        TRACK_STRUCT *pt);                                                        */
/*                                                                                  */
/*    static INT zz_linear_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls,           */
/*        TRACK_STRUCT *pt);                                                        */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This routine is a server that determines the type of track to                 */
/*    be drawn and then calls the appropriate routine to do it.                     */
/*                                                                                  */
/*    The types of tracks that may be drawn are:                                    */
/*                                                                                  */
/*        Depth track  - A track containing depth markers and annotation            */
/*        Log track    - A track grid drawn with logarithmic divisions              */
/*        Linear track - A track grid drawn with linear divisions                   */
/*        Text track   - A track with text annotation                               */
/*                                                                                  */
/*    The data that describes each individual track is expected to be               */
/*    completely resolved at this point and is found in the TRACK linked-           */
/*    list with the given layout structure (pls).                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_gr_draw_tracks(XS_STRUCT   *pxs,LAYOUT_STRUCT  *pls,INT plot_flag);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Main cross section object.                     */
/*    pls             -(LAYOUT_STRUCT *) Individual well layout.                    */
/*    plot_flag       -(INT) True if we are plotting not interactive.               */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_gr_draw_tracks (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, INT plot_flag)
#else
publicdef INT xs_gr_draw_tracks (pxs, pls, plot_flag)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
INT plot_flag;
#endif
    {
    TRACK_STRUCT *pt;               /* POINTER TO A TRACK INFO STRUCTURE            */
    
    INT i = 0, first = 0, status = SUCCESS, ntracks = 0;
    
    /*********************** START CODE *****************************               */
    /* RE-SELECT OUR DEFAULT XFORM FOR XSECTION                                     */
    wmtselect (pxs->drawing.tnr_id);
    
    /* LOOP ON THE TRACKS, CALL DRAW BASED ON                                       */
    /* TYPE.                                                                        */
    ntracks = llcount (&pls->tracks);
    for (first = TRUE, i = 0; i < ntracks; first = FALSE, i++)
        {
        status = (first) ? llfirst (&pls->tracks,
                                    (BYTE *)&pt) : llnext (&pls->tracks, (BYTE *)&pt);
        
        /* SHOULD THIS TRACK BE DRAWN AT ALL?                                       */
        if (pt->visible)
            {
            if (!plot_flag)         /*  SEGMENTS ONLY IN INTERACTIVE MODE           */
                wmsallocate (&pt->seg_id);
            
            switch (pt->type)
                {
            case DEPTH_TRACK:
                status = zz_depth_track (pxs, pls, pt);
                break;
                
            case LINEAR_TRACK:
                status = zz_linear_track (pxs, pls, pt);
                break;
                
            case LOG_TRACK:
                status = zz_log_track (pxs, pls, pt);
                break;
                
            case TIME_TRACK:
                break;
                
            case TEXT_TRACK:
                break;
                
            default:
                status = FAIL;
                break;
                }
            /* IF DRAWING IN INTERACTIVE MODE, CLOSE                                */
            /* SEGMENT & NAME IT                                                    */
            if (!plot_flag)
                {
                wmsclose (pt->seg_id);
                
                switch (pt->type)
                    {
                case DEPTH_TRACK:
                    wmoset (pt->seg_id, WM_CLASS_SEGMENT, "", "Depth Track");
                    break;
                    
                case LINEAR_TRACK:
                    wmoset (pt->seg_id, WM_CLASS_SEGMENT, "", "Linear Track");
                    break;
                    
                case LOG_TRACK:
                    wmoset (pt->seg_id, WM_CLASS_SEGMENT, "", "Logarithmic Track");
                    break;
                    
                case TIME_TRACK:
                    wmoset (pt->seg_id, WM_CLASS_SEGMENT, "", "Time Track");
                    break;
                    
                case TEXT_TRACK:
                    wmoset (pt->seg_id, WM_CLASS_SEGMENT, "", "Text Track");
                    break;
                    
                default:
                    break;
                    }
                }
            /* END IF PLOT_FLAG                                                     */
            }
        /* END IF VIS.                                                              */
        }
    /* END FOR EACH TRACK                                                           */
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Draw a depth track with info from pls.                                        */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT zz_depth_track(XS_STRUCT *pxs,LAYOUT_STRUCT *pls,TRACK_STRUCT *pt);    */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*    pls             -(LAYOUT_STRUCT *)                                            */
/*    pt              -(TRACK_STRUCT *)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_drw_trk.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT zz_depth_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt)
#else
static INT zz_depth_track (pxs, pls, pt)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
TRACK_STRUCT *pt;
#endif
    {
    DOUBLE xmin, xmax, ymin, ymax;  /*   positioning vars.                          */
    
    DOUBLE dx, tminx, tmaxx;        /*    text positioning vars                     */
    
    DOUBLE text_x, text_y,          /* actual text coords                           */
        text_xspace;                /* character spacing factor                     */
    DOUBLE text_exp;                /* text expansion facter: height/width ratio    */
    
    INT status, center;             /* TRUE to center text                          */
    
    DOUBLE max_depth;               /* maximum depth to handle                      */
    
    CHAR depth_str[10], max_depth_str[10];
    
    DOUBLE sp,                      /* screen position                              */
        gap, depth_here;
    
    DOUBLE ft_per_pix;              /* simple depth to screen position transform    */
    DOUBLE mark_freq;
    
    DOUBLE pix_per_ft;
    DOUBLE top, base;
    DOUBLE total_depth, char_size, char_width, chh;
    
    INT depth, label_freq, itop,    /* integer version of the regular top/base      */
        /* vars                                                                     */
        ibase;                      /* used in a modulo operation later             */
    COLOR_INDEX normalColor;
    INT font, prec;
    PROJECT_DEFAULT *proj;
    CHAR stored_units[32];
    CHAR interval_units[32];
    BOOL first;

    /* Pull the depth uom from the                                                  */
    /*   default project structure.                                                 */
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    strcpy (stored_units, proj->unit.vertical);
  
    /* initialize the positioning vars                                              */
    xmin = pt->xorigin;
    xmax = pt->width + xmin;
    
    ymin = pt->yorigin;
    ymax = pt->length + ymin;
    
    /* these deltas represent the height and width                                  */
    /* (WCS) of this track                                                          */
    dx = xmax - xmin;
    
    /* set a temporary upper most Y for drawing                                     */
    /* the track frame up into the scale block.                                     */
    /* text_y is used for convenience                                               */
    text_y = pls->scale_block.height + ymax;
    
    /* draw the frame                                                               */
    xs_gr_rect (xmin, ymin, xmax, text_y, pt->line.color, pt->line.width);
    
    /* the upper and lower depth limits                                             */
    /* convert the measure units from stroed units                                  */
    /* to the unser requested map units, if necessay                                */
    
#if 0
    status = tu_unit_conversion (pls->interval.track_top, stored_units, &top,
                                 pxs->depth_scale_map_units);
    status = tu_unit_conversion (pls->interval.track_base, stored_units, &base,
                                 pxs->depth_scale_map_units);
#else
    /* I think what need to be converted is the conversion between 
       interval_units and stored_units - KLY */
    switch (pxs->depth_interval_scale_units)
      {
      case XS_INCH_UNITS:
        strcpy (interval_units, "FEET");
	break;
      case XS_CENTIMETER_UNITS:
      default:
	strcpy(interval_units,"METERS");
	break;
      }
    status = tu_unit_conversion (pls->interval.track_top, 
				 stored_units,
				 &top, interval_units);
    status = tu_unit_conversion (pls->interval.track_base, 
				 stored_units,
				 &base, interval_units);
#endif
    
    /* integer copy for modulo operation later                                      */
    itop = (INT)top;
    ibase = (INT)base;

    /* the number of feet between annotations                                       */
    /* convert the units of interval from user                                      */
    /* specified to user map requested.                                             */
    
#if 0
    status = tu_unit_conversion (pt->primary.frequency, interval_units, &mark_freq,
                                 pxs->depth_scale_map_units);
#else
    mark_freq = pt->primary.frequency;
#endif

    if (mark_freq <= 0.0)
        return SUCCESS;             /* interval of 0 turns off labels               */
    
    /* total footage or whatever to display                                         */
    total_depth = base - top;

    /* set up the depth to Y coord transform                                        */
    ft_per_pix = total_depth / pt->length;
    pix_per_ft = pt->length / total_depth;
    
    /* set a char size for the text as a fraction                                   */
    /* of the total screen height                                                   */
    chh = (pxs->drawing.yorigin + pxs->drawing.height) * 0.012;
    char_size = chh;
    /* use stroke text centering                                                    */
    center = TRUE;
    
    /* set all the relevant GKS text attribute to                                   */
    /* a known state                                                                */
    lu_text_to_index ("COLOR", (INDEX *)&normalColor, "NORMAL");
    xs_gr_set_text (char_size, 1.0, normalColor, center);
    lu_text_to_index ("FONT", &font, "SIMPLEX");
    lu_text_to_index ("TEXT_PRECISION", &prec, "STROKE");
    wm_gstxfp (font, prec);
    
    /* put a little gap between letters for                                         */
    /* readability                                                                  */
    text_xspace = 0.15;
    wm_gschsp (text_xspace);
    
    /* determine the appropriate width for the                                      */
    /* depth notation by sizing a dummy string                                      */
    /* that is hopefully larger than the longest                                    */
    /* depth string we will encounter                                               */
    max_depth = ((ymax - 1.0) * ft_per_pix) + top;
    sprintf (max_depth_str, "l%dl", (INT)max_depth);
    
    tminx = xmin;
    tmaxx = xmax;
    
    /* set expansion factor                                                         */
    text_exp = 1.0;
    wm_gschxp (text_exp);
    
    tg_fit_string (max_depth_str, tminx, ymin, tmaxx, 0.0, chh, &char_size);
    wm_gschh (char_size);
    
    /* the width of a character string for GKS is                                   */
    /* determined by the following text_xmax =                                      */
    /* char_size * text_exp * strlen(string)                                        */
    /* multiply the expansion factor with the                                       */
    /* character height to get the width                                            */
    char_width = char_size * text_exp;
    
    /* position the text                                                            */
    text_x = tminx;
    text_x -= (char_width / 2.0);
    text_y = ymin;
    
    /* get a rounded set of annotation figures by                                   */
    /* this crude method                                                            */
    label_freq = (INT)mark_freq;
    depth = ((ibase / label_freq) * label_freq);
    sp = ymax - ((depth - itop) * pix_per_ft);
    
    /* loop through the depth range and annotate                                    */
    /* the track                                                                    */
    for (first = TRUE, gap = mark_freq * pix_per_ft; sp < ymax; sp += gap)
        {
        if ((sp - char_size) < ymin)
            {
            continue;
            }
	if (first)
	  { /* if first pass - calculate the first label value */
	    depth_here = ((((ymax - sp) * ft_per_pix) + top + 0.5) / label_freq)
	      * label_freq;
	    first = FALSE;
	  }
	else
	  { /* other than first pass just decreased by the frequency */
	    depth_here -= mark_freq;
	  }
	sprintf (depth_str, " %d", (INT)depth_here);
        text_y = sp - (char_size / 2.0);
        wm_gtx (text_x, text_y, depth_str);
        }
    /* determine the best text height for drawing                                   */
    /* the unit of measure                                                          */
    /* 12/12/88 GS (from Prime 1.1) let unit label be same size as depth labels     */
    /*    unless window isn't big enough, then make it have window height           */
    if (char_size > pls->scale_block.height / 2.0)
        {
        chh = pls->scale_block.height / 2.0;
        char_size = chh;
        }
    /* draw the unit of measure in the upper part                                   */
    /* of the depth track actually, this is drawn                                   */
    /* in the scale block                                                           */
    text_y = (pls->scale_block.height * 0.25) + ymax;
    tminx = xmin + (dx * 0.1);
    tmaxx = xmax - (dx * 0.1);
    text_x = tminx;
    
    tg_fit_string (interval_units, text_x, text_y, tmaxx, 0.0, chh, &char_size);
    wm_gschh (char_size);
    
    wm_gtx (text_x, text_y, interval_units);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Draw a track with logarithmic divisions.                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT zz_log_track(XS_STRUCT *pxs,LAYOUT_STRUCT *pls,TRACK_STRUCT *pt);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*    pls             -(LAYOUT_STRUCT *)                                            */
/*    pt              -(TRACK_STRUCT *)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_drw_trk.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT zz_log_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt)
#else
static INT zz_log_track (pxs, pls, pt)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
TRACK_STRUCT *pt;
#endif
    {
    
    static GKS_REAL ticks[100];
    FLOAT *pticks, *first_tick;
    DOUBLE cycle_width, xlast, xmin, xmax, ymin, ymax, left_side, zcycle, xp,
        tick_height, half_tick, sp, gap;
    
    GKS_REAL xarray[5], yarray[5];
    INT lpc,                        /* lines (divisions) per cycle                  */
        i, j, num_cycles;
    
    DOUBLE pix_per_ft;
    
    DOUBLE top, base, total_depth;
    
    INT depth, label_freq;
    
    INT itop, ibase;
    DOUBLE loga[10];
    DOUBLE xstart;
    INT istart, iinc;
    DOUBLE dlog, dstart, dreslt;
    
    /* top & base depth plus their integer counter                                  */
    /* parts                                                                        */
    top = pls->interval.track_top;
    base = pls->interval.track_base;
    itop = (INT)top;                /* integer for modulo operation later           */
    ibase = (INT)base;
    
    /* our total depth range to work with                                           */
    total_depth = base - top;
    
    /* a depth to screen pos. transform                                             */
    pix_per_ft = pt->length / total_depth;
    
    /* set up the GKS drawing attributes                                            */
    
    wm_gsln (pt->divisions.line.style);
    wm_gslwsc (pt->divisions.line.width);
    
    wm_gsplci (pt->divisions.line.color);
    
    /* initialize the positioning vars.                                             */
    xmin = pt->xorigin;
    ymin = pt->yorigin;
    
    xmax = pt->width + xmin;
    ymax = pt->length + ymin;
    
    /* the following sets up the log divisions to                                   */
    /* correspond to the requested range i.e. .2 -                                  */
    /* 2000 or .1 to 1000 etc.                                                      */
    
    dlog = log10 (pt->divisions.start_value);
    dreslt = pow (10.0, dlog);
    dstart = (DOUBLE)(pt->divisions.start_value / dreslt);
    istart = (INT)dstart;
    iinc = 1;
    loga[0] = log10 (dstart);
    
    /* build a local array of logarithms.  This                                     */
    /* speeds things up by allowing us to avoid                                     */
    /* calls to the C funct. LOG10()                                                */
    for (i = 1; i < 10; i++)
        {
        if (istart == 10)
            {
            iinc = 10;
            }
        istart += iinc;
        xstart = istart;
        loga[i] = log10 (xstart) - loga[0];
        }
    loga[0] = 0.0;
    
    /* the number of logarithmic cycles                                             */
    
    if (pt->divisions.number == 0)
        {
        pt->divisions.number = 1;
        }
    num_cycles = pt->divisions.number;
    
    cycle_width = pt->width / (DOUBLE)num_cycles;
    
    /* this is the position to start drawing                                        */
    left_side = pt->xorigin;
    
    /* the ticks array and its pointer pticks hold                                  */
    /* the actual X positions of each division as                                   */
    /* it is calculated. the divisions are drawn                                    */
    /* by a seperate routine (a specialist!)                                        */
    pticks = ticks;
    first_tick = ticks;
    
    /* logarithmic cycles are done by decades                                       */
    lpc = (INT)10.0;                /* lines per cycle                              */
    
    /* if complete top to bottom division lines                                     */
    /* are to be drawn, do this                                                     */
    if (pt->divisions.type EQUALS LINES)
        {
        *pticks = left_side;
        
        for (i = 0, ++pticks, zcycle = left_side; i < num_cycles; i++, zcycle = xlast)
            {
            for (j = 0; j < lpc; j++, ++pticks)
                {
                xp = loga[j] * cycle_width + zcycle;
                *pticks = xlast = xp;
                }
            }
        *pticks = -9.0;             /* flag that this is the last tick              */
        pticks = first_tick;        /* reset pticks to beginning of array           */
        
        /* do the actual drawing of the divisions                                   */
        xs_gr_draw_ticks (pxs, pls, pticks, ymin, ymax, pt);
        }
    /* otherwise, we are faced with the much                                        */
    /* tougher case of just drawing the divisions                                   */
    /* on PRIMARY marking lines, this mode is used                                  */
    /* when tracks are crowded 'cause it leaves                                     */
    /* more screen for the traces                                                   */
    else if (pt->divisions.type EQUALS TICS AND (pt->primary.frequency > 0.0))
        {
        
        tick_height = pt->primary.frequency * pix_per_ft * YTICKSCALE;
        half_tick = tick_height * 0.5;
        
        /* find a rounded depth to begin on                                         */
        label_freq = (INT)pt->primary.frequency;
        
        depth = ((ibase / label_freq) * label_freq) - label_freq;
        sp = ymax - ((depth - itop) * pix_per_ft);
        
        gap = pt->primary.frequency * pix_per_ft;
        for (; sp < ymax; sp += gap)
            {
            for (i = 0, zcycle = left_side; i < num_cycles; i++, zcycle = xlast)
                {
                for (j = 1; j <= lpc; j++)
                    {
                    
                    xp = (1.0 + log10 ((DOUBLE)j / (DOUBLE)lpc)) * cycle_width +
                        zcycle;
                    xlast = xp;
                    if (sp == ymin OR (sp - ymin <= half_tick))
                        {
                        xarray[0] = xarray[1] = xp;
                        yarray[0] = ymin;
                        yarray[1] = sp + half_tick;
                        wm_gpl (2, xarray, yarray);
                        }
                    else if ((ymax - sp) < half_tick)
                        {
                        xarray[0] = xarray[1] = xp;
                        yarray[0] = sp - half_tick;
                        yarray[1] = ymax;
                        wm_gpl (2, xarray, yarray);
                        }
                    else
                        {
                        xarray[0] = xarray[1] = xp;
                        yarray[0] = sp - half_tick;
                        yarray[1] = sp + half_tick;
                        wm_gpl (2, xarray, yarray);
                        }
                    }
                }
            }
        }
    /* frame this track                                                             */
    xs_gr_rect (xmin, ymin, xmax, ymax, pt->line.color, pt->line.width);
    
    /* draw the markings                                                            */
    xs_gr_draw_marks (pxs, pls, pt);
    
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Draw a track with linear divisions.                                           */
/*                                                                                  */
/*    This routine is very similar to the logarithmic track drawing                 */
/*    routine, please refer to that routine for a more commented                    */
/*    and better explained description of the code.                                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT zz_linear_track(XS_STRUCT *pxs,LAYOUT_STRUCT *pls,TRACK_STRUCT *pt);   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*    pls             -(LAYOUT_STRUCT *)                                            */
/*    pt              -(TRACK_STRUCT *)                                             */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_drw_trk.c>                                                     */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT zz_linear_track (XS_STRUCT *pxs, LAYOUT_STRUCT *pls, TRACK_STRUCT *pt)
#else
static INT zz_linear_track (pxs, pls, pt)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
TRACK_STRUCT *pt;
#endif
    {
    static FLOAT ticks[100];
    FLOAT *pticks, *first_tick;
    DOUBLE cycle_width, xmin, xmax, ymin, ymax, left_side, xp, tick_height, half_tick,
        sp, gap;
    
    GKS_REAL xarray[5], yarray[5];
    INT j, num_cycles;
    
    DOUBLE pix_per_ft;
    
    DOUBLE top, base, total_depth;
    
    INT depth, label_freq, itop, ibase;
    
    /* top & base depth plus their integer counter                                  */
    /* parts                                                                        */
    top = pls->interval.track_top;
    base = pls->interval.track_base;
    itop = (INT)top;                /* integer for modulo operation later           */
    ibase = (INT)base;
    
    /* our total depth range to work with                                           */
    total_depth = base - top;
    
    /* a depth to screen pos. transform                                             */
    pix_per_ft = pt->length / total_depth;
    
    /* set up the GKS drawing attributes                                            */
    wm_gsln (pt->divisions.line.style);
    
    wm_gslwsc (pt->divisions.line.width);
    wm_gsplci (pt->divisions.line.color);
    
    /* initialize the positioning vars.                                             */
    xmin = pt->xorigin;
    ymin = pt->yorigin;
    
    xmax = pt->width + xmin;
    ymax = pt->length + ymin;
    
    if (pt->divisions.number == 0)
        {
        pt->divisions.number = 1;
        }
    num_cycles = pt->divisions.number;
    
    cycle_width = pt->width / (DOUBLE)num_cycles;
    left_side = pt->xorigin;
    
    /* the ticks array and its pointer pticks hold                                  */
    /* the actual X positions of each division as                                   */
    /* it is calculated. the divisions are drawn                                    */
    /* by a seperate routine (a specialist!)                                        */
    pticks = ticks;
    first_tick = ticks;
    
    /* if complete top to bottom division lines                                     */
    /* are to be drawn, do this                                                     */
    if (pt->divisions.type EQUALS LINES)
        {
        for (j = 1; j <= num_cycles; j++, ++pticks)
            {
            *pticks = xp = j * cycle_width + left_side;
            }
        *pticks = -9;               /* flag that this is the last tick              */
        pticks = first_tick;        /* reset pticks to beginning of array           */
        xs_gr_draw_ticks (pxs, pls, pticks, ymin, ymax, pt);
        }
    /* other wise we are faced with the much                                        */
    /* tougher case of just drawing the divisions                                   */
    /* on PRIMARY marking lines, this mode is used                                  */
    /* when tracks are crowded 'cause it leaves                                     */
    /* more screen for the traces                                                   */
    else if (pt->divisions.type EQUALS TICS AND (pt->primary.frequency > 0.0))
        {
        
        tick_height = pt->primary.frequency * pix_per_ft * YTICKSCALE;
        half_tick = tick_height * 0.5;
        
        label_freq = (INT)pt->primary.frequency;
        depth = ((ibase / label_freq) * label_freq) - label_freq;
        sp = ymax - ((depth - itop) * pix_per_ft);
        
        gap = pt->primary.frequency * pix_per_ft;
        for (; sp < ymax; sp += gap)
            {
            for (j = 1; j <= num_cycles; j++)
                {
                xp = j * cycle_width + left_side;
                if (sp == ymin OR (sp - ymin <= half_tick))
                    {
                    xarray[0] = xarray[1] = xp;
                    yarray[0] = ymin;
                    yarray[1] = sp + half_tick;
                    wm_gpl (2, xarray, yarray);
                    }
                else if ((ymax - sp) < half_tick)
                    {
                    xarray[0] = xarray[1] = xp;
                    yarray[0] = sp - half_tick;
                    yarray[1] = ymax;
                    wm_gpl (2, xarray, yarray);
                    }
                else
                    {
                    xarray[0] = xarray[1] = xp;
                    yarray[0] = sp - half_tick;
                    yarray[1] = sp + half_tick;
                    wm_gpl (2, xarray, yarray);
                    }
                }
            }
        }
    xs_gr_rect (xmin, ymin, xmax, ymax, pt->line.color, pt->line.width);
    xs_gr_draw_marks (pxs, pls, pt);
    return SUCCESS;
    }
/* END:                                                                             */
