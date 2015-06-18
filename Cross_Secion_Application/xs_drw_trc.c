/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_TRC.C*/
/*  5    27-FEB-1992 14:41:26 JESSIE "(SPR 6180) TVD display of traces and tops"*/
/* *4    17-JUL-1991 16:20:46 JANTINA "(SPR 0) Fix for trace unit conversion"*/
/* *3    26-OCT-1990 15:35:33 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *2    17-AUG-1990 22:38:43 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:43:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_DRW_TRC.C*/
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
#include "esi_xs_gr.h"

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#ifndef DEBUG

#define DEBUG 0

#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Cross section graphics routines.                                              */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_gr_draw_traces(XS_STRUCT *pxs,LAYOUT_STRUCT *pls);           */
/*                                                                                  */
/*    publicdef INT xsz_draw_trace(XS_STRUCT *pxs,LAYOUT_STRUCT  *pls,              */
/*        LP_TRACE_STRUCT *plpts);                                                  */
/*                                                                                  */
/*Private_Functions:                                                                */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Cross Section Graphics Slave Routine; Draw log traces within track grids.     */
/*    This routine loops through the traces linked list and for each                */
/*    one that is determined to be visible, calls XS_GR_DRAW_TRACE.                 */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_gr_draw_traces(XS_STRUCT *pxs,LAYOUT_STRUCT *pls);           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *) Main cross section object.                     */
/*    pls             -(LAYOUT_STRUCT *) Individula well descriptions struct.       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_gr_draw_traces (XS_STRUCT *pxs, LAYOUT_STRUCT *pls)
#else
publicdef INT xs_gr_draw_traces (pxs, pls)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
#endif
    {
    LP_TRACE_STRUCT *plpts;         /* LOG PLOT TRACE STRUCT POINTER                */
    
    INT status, n, ntraces;         /* total count of traces in linked list         */
    
    GKS_INT clsw;
    LISTHEAD *llist;                /* linked list of traces                        */
    
    /********************************************************************************/
    
    llist = &pls->traces;           /* POINT TO THE LINKED LIST                     */
    
    clsw = 0;                       /* TURN OFF THE gks CLIPPING.                   */
    wm_gsclip (clsw);
    
    ntraces = llcount (llist);      /* LOOP ON THE TRACES                           */
    for (n = 0; n < ntraces; n++)
        {
        status = (n == 0) ? llfirst (llist,
                                    (VOIDPTR)&plpts) : llnext (llist, (VOIDPTR)&plpts);
        
        /* DRAW THIS TRACE IF IT IS VISIBLE                                         */
        if (plpts->visible == TRUE)
            {
            /* DO THE ACTUAL DISPLAYING                                             */
            if ((status = xsz_draw_trace (pxs, pls, plpts)) == FAIL)
                return FAIL;
            }
        }
    /* turn on the GKS clipping.                                                    */
    clsw = 1;
    wm_gsclip (clsw);
    
    wmtselect (pxs->drawing.tnr_id);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Cross Section Graphics Slave Routine; Fetches a given track and draws         */
/*    the trace on it.                                                              */
/*                                                                                  */
/*    This routine takes each trace and matches it to a track.  The trace           */
/*    is analyzed to determine if it will fit within the track boundaries.          */
/*    If it does not, a clip is applied that leaves just the portion of             */
/*    the trace that lies within the track.                                         */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xsz_draw_trace(XS_STRUCT *pxs,LAYOUT_STRUCT  *pls,              */
/*        LP_TRACE_STRUCT *plpts);                                                  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*    pls             -(LAYOUT_STRUCT *)                                            */
/*    plpts           -(LP_TRACE_STRUCT *)                                          */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xsz_draw_trace (XS_STRUCT *pxs, LAYOUT_STRUCT *pls,
                              LP_TRACE_STRUCT *plpts)
#else
publicdef INT xsz_draw_trace (pxs, pls, plpts)
XS_STRUCT *pxs;
LAYOUT_STRUCT *pls;
LP_TRACE_STRUCT *plpts;
#endif
    {
    TRACK_STRUCT *pt;
    LOG_TRACE_STRUCT *plts;
    
    PROJECT_DEFAULT *proj;
    DOUBLE conv;
    DOUBLE tconv;
    
    FLOAT nwc_xmin,                 /* normalized window coord pos. vars.           */
        nwc_xmax, nwc_ymin, nwc_ymax, wcs_xmin, /* world coord. pos. vars.          */
        wcs_xmax, wcs_ymin, wcs_ymax;
    
    DOUBLE one_x_pix,               /* the X axis stepping size                     */
        one_y_pix;                  /* ditto for the Y                              */
    
    DOUBLE min_depth,               /* minimum and maximum depths for a trace       */
        max_depth, min_tval,        /* the minimum and maximum (left/right) values  */
        max_tval,                   /* for the trace's peaks/troughs                */
        log_mintval,                /* ditto except for logarithmicly scaled traces */
        log_maxtval;
    
    DOUBLE trk_xmin,                /* track pos. description vars.                 */
        trk_xmax, trk_ymin, trk_ymax;
    
    INT track_num,                  /* the number of a given track                  */
        status = SUCCESS, n, ntracks, log_flag; /* TRUE if we are drawing in a LOG track    */
    
    NLIST_HEADER analyze_list;
    
    CHAR trace_name[75];            /* used to name the segment                     */
    
    /* set a pointer to the LOG_TRACE_STRUCT contained in the                       */
    /* LP_TRACE_STRUCT                                                              */
    plts = &plpts->trace_struct;
    
    wm_gsplci (plpts->line.color);  /* SET TRACE DRAWING COLOR                      */
    wm_gslwsc (plpts->line.width);  /* THICKNESS OF THE TRACE                       */
    wm_gsln (plpts->line.style);    /* DOTTED, DASHED, SOLID ETC.                   */
    
    /* SEEK A PARTICULAR TRACK                                                      */
    track_num = plpts->track_num;
    
    /* COUNT THE TRACKS                                                             */
    ntracks = llcount (&pls->tracks);
    
    for (n = 0; n < ntracks; n++)
        {
        status = (n == 0) ? llfirst (&pls->tracks, (VOIDPTR)&pt) :
                            llnext (&pls->tracks, (VOIDPTR)&pt);
        
        /* IS OUR LIST o.k.?                                                        */
        if (status < 0 OR (status == FAIL))
            {
            return FAIL;            /* IF NOT, WE CAN'T CONTINUE                    */
            }
        if (pt->number == track_num)
            {
            break;
            }
        }
    /* IF WE LEFT THE LOOP WITHOUT FINDING A MATCHING PAIR, EXIT                    */
    if (n == ntracks)
        {
        return FAIL;
        }
    /* SET TRANSFORMATION FOR THIS TRACK                                            */
    
    wmtinq (pxs->drawing.tnr_id, &nwc_xmin, &nwc_xmax, &nwc_ymin, &nwc_ymax, &wcs_xmin,
            &wcs_xmax, &wcs_ymin, &wcs_ymax);
    
    one_y_pix = (nwc_ymax - nwc_ymin) / (wcs_ymax - wcs_ymin);
    one_x_pix = (nwc_xmax - nwc_xmin) / (wcs_xmax - wcs_xmin);
    
    trk_ymin = pt->yorigin * one_y_pix; /* calculate the ndc positions              */
    trk_ymax = (pt->yorigin + pt->length) * one_y_pix;  /* of the min & max parameters  */
    
    trk_xmin = pt->xorigin * one_x_pix;
    trk_xmax = (pt->xorigin + pt->width) * one_x_pix;
    
    min_depth = MIN (pls->interval.track_base, pls->interval.track_top);
    max_depth = MAX (pls->interval.track_base, pls->interval.track_top);
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    status = tu_unit_conversion (1.0, proj->unit.vertical, &conv, plts->depth_uom);
    min_depth = min_depth * conv;
    max_depth = max_depth * conv;
    
    min_tval = MIN (plpts->left, plpts->right);
    max_tval = MAX (plpts->left, plpts->right);
    tconv = 1.0;
    if (ARE_SAME (plpts->trace_units, "T"))
        {
        if (ARE_SAME (plts->trace_uom, "MM"))
            {
            status = tu_unit_conversion (1.0, "IN", &tconv, plts->trace_uom);
            }
        else if (ARE_SAME (plts->trace_uom, "K/M3"))
            {
            status = tu_unit_conversion (1.0, "G/C3", &tconv, plts->trace_uom);
            }
        else if (ARE_SAME (plts->trace_uom, "US/M"))
            {
            status = tu_unit_conversion (1.0, "US/F", &tconv, plts->trace_uom);
            }
        else
            {
            tconv = 1.0;
            }
        }
    else if (ARE_SAME (plpts->trace_units, "M"))
        {
        if (ARE_SAME (plts->trace_uom, "IN") == 0)
            {
            status = tu_unit_conversion (1.0, "MM", &tconv, plts->trace_uom);
            }
        else if (ARE_SAME (plts->trace_uom, "G/C3"))
            {
            status = tu_unit_conversion (1.0, "K/M3", &tconv, plts->trace_uom);
            }
        else if (ARE_SAME (plts->trace_uom, "US/F"))
            {
            status = tu_unit_conversion (1.0, "US/M", &tconv, plts->trace_uom);
            }
        else
            {
            tconv = 1.0;
            }
        }
    min_tval = min_tval * tconv;
    max_tval = max_tval * tconv;
    
    if (pt->type == LOG_TRACK)
        {
        log_mintval = log10 (min_tval);
        log_maxtval = log10 (max_tval);
        
        wmtallocate (&plpts->tnr_id, trk_xmin, trk_xmax, trk_ymin, trk_ymax,
                     log_mintval, log_maxtval, min_depth, max_depth);
        }
    else                            /* non logarithmic track and transformation     */
        {
        wmtallocate (&plpts->tnr_id, trk_xmin, trk_xmax, trk_ymin, trk_ymax, min_tval,
                     max_tval, min_depth, max_depth);
        }
    /* delete the old segment for this trace if any                                 */
    if (plpts->seg_id != 0)
        {
        wmsdelete (plpts->seg_id);
        }
    /* determine type of scaling                                                    */
    log_flag = (pt->type == LOG_TRACK) ? TRUE : FALSE;
    
    /* to analyze the trace first.                                                  */
    switch (pls->data_thinning)
        {
    case DECIMATION:
        if (pls->decimate EQUALS 0)
            {
            analyze_list = plts->ptrace;
            break;
            }
        status = lg_draw_log_skip (plts->ptrace, conv * pls->interval.track_top,
                                   conv * pls->interval.track_base,
                                   tconv * plpts->left, tconv * plpts->right,
                                   pls->decimate, log_flag, &analyze_list);
        break;
    case SLOPE_CHANGE:
        if (pls->delta_slope EQUALS 0.0)
            {
            analyze_list = plts->ptrace;
            break;
            }
        status = lg_draw_log_slope (plts->ptrace, conv * pls->interval.track_top,
                                    conv * pls->interval.track_base,
                                    tconv * plpts->left, tconv * plpts->right,
                                    pls->delta_slope, log_flag, &analyze_list);
        break;
    default:
        analyze_list = plts->ptrace;
        break;
        }
    /* create a new segment for this trace to live in                               */
    status = wmsallocate (&plpts->seg_id);
    
    /* draw log trace                                                               */
    status = lg_draw_log_clip (analyze_list, conv * pls->interval.track_top,
                               conv * pls->interval.track_base, tconv * plpts->left,
                               tconv * plpts->right, log_flag, 1);
    
    /* close and display the trace segment                                          */
    status = wmsclose (plpts->seg_id);
    if (status != SUCCESS)
        {
        printf ("DRAW_TRACE: wmsclose returned %d\n", status);
        }
    sprintf (trace_name, "Trace: %s", plpts->name);
    wmoset (plpts->seg_id, WM_CLASS_SEGMENT, "", trace_name);
    
    return status;
    }
/* END:                                                                             */
