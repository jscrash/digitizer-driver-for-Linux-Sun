/* DEC/CMS REPLACEMENT HISTORY, Element XS_CHG.C*/
/*  8    27-FEB-1992 09:25:14 JESSIE "(SPR 6180) add TVD display of traces and tops"*/
/* *7    26-OCT-1990 15:35:25 GILLESPIE "(SPR 34) Incorporate Van Zandwyk/Gulf trace unit deltas"*/
/* *6    17-AUG-1990 22:37:38 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    29-MAR-1990 16:33:55 MING "(SPR 5173) GULF bug fix"*/
/* *4    23-JAN-1990 17:59:04 GILLESPIE "(SPR 6001) Minor syntax error (oops)"*/
/* *3    23-JAN-1990 12:14:10 GILLESPIE "(SPR 6001) Move initialization of pwell1"*/
/* *2    18-JAN-1990 08:23:24 GILLESPIE "(SPR 6001) Changes for new wells struct"*/
/* *1    19-JUN-1989 13:43:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XS_CHG.C*/
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

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#if USE_PROTOTYPES
static INT xsz_well_sb_size (XS_STRUCT *pxs);

#else       /* no prototypes */
static INT xsz_well_sb_size ();

#endif      /* of #if USE_PROTOTYPES */

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Drawing structure routines.                                                   */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xs_chg_drawing(XS_STRUCT *p_xsec);                              */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT xsz_well_sb_size (XS_STRUCT *pxs);                             */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    To supervise the steps of resolving the drawing str-                          */
/*    ucture- ie call all the separate modules in the cor-                          */
/*    rect order.  Also perform some general fatal error                            */
/*    checks before entering sub-modules.                                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xs_chg_drawing(XS_STRUCT *p_xsec);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) p_xsec      -(XS_STRUCT *) Link list LISTHEAD struct;                     */
/*      Pointer to x-section.                                                       */
/*                                                                                  */
/*Return Value / Status :                                                           */
/*    SUCCESS-Successful completion                                                 */
/*    FAIL-Unsuccessful completion                                                  */
/*Scope :                                                                           */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations / Assumptions :                                                       */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xs_chg_drawing (XS_STRUCT *p_xsec)
#else
publicdef INT xs_chg_drawing (p_xsec)
XS_STRUCT *p_xsec;
#endif
    {
    LAYOUT_STRUCT *p_layout;
    TRACK_STRUCT *p_track;
    LP_TRACE_STRUCT *p_trace;
    TOP_STRUCT top_struct;
    PROJECT_DEFAULT *proj;
    DOUBLE conv;
    CHAR message[80];
    INT i, j, k;
    INT no_layouts;
    INT no_tracks;
    INT no_traces;
    INT status;
    DOUBLE greatest_depth;
    DOUBLE max_depth;
    DOUBLE min_depth;
    DOUBLE average;
    DOUBLE track_length;
    DOUBLE track_end;
    DOUBLE track_end_max;
    WELL_HDR_1 *pwell1;
    
    /* Test list of possible fatal errors and return FAIL if any encountered        */
    /* Abort if the depth scale is bad.                                             */
    if (p_xsec->depth_scale <= 0.0)
        {
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEPTH_SCALE));
        if (p_xsec->review_all_switch != TRUE)
            status = xs_depth_scale_cdlg_init (p_xsec);
        return(FAIL);
        }
    /* Abort if distance scale is bad.                                              */
    if (p_xsec->xs_spacing != XS_CONSTANT_SPACED AND p_xsec->xs_scale <
        0.000000000001 AND p_xsec->xs_scale > -0.000000000001)
        {
        status = am_message (AM_ALERT, mg_message (XS_BAD_X_SCALE));
        if (p_xsec->review_all_switch != TRUE)
            status = xs_horizontal_cdlg_init (p_xsec);
        return(FAIL);
        }
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&proj);
    
    no_layouts = llcount (&p_xsec->drawing.log_plots);  /* any layouts present?     */
    
    if (no_layouts <= 0)
        {
        status = am_message (AM_ALERT, mg_message (XS_NO_LAYOUTS));
        return(FAIL);
        }
    /* Make first pass thru all wells                                               */
    
    greatest_depth = -999999.99;
    
    status = llfirst (&p_xsec->drawing.log_plots, &p_layout);
    
    for (i = 0; i < no_layouts; i++)
        {
        /* Initialize min & max for all traces                                      */
        min_depth = 999999.9;
        max_depth = -999999.9;
        /* Warn if there are no traces.                                             */
        no_traces = llcount (&p_layout->traces);
        if (no_traces <= 0)
            {
            sprintf (message, "%s  %s", mg_message (XS_NO_TRACES),
                     p_layout->pwell->uwi);
            status = am_message (AM_STATUS, message);
            }
        else
            {
            /*  Find the min & max depths of traces                                 */
            
            status = llfirst (&p_layout->traces, &p_trace);
            for (j = 0; j < no_traces; j++)
                {
                status = tu_unit_conversion (1.0, p_trace->trace_struct.depth_uom,
                                             &conv, proj->unit.vertical);
                min_depth = MIN (min_depth,
                                 (p_trace->trace_struct.non_null_top) * conv);
                max_depth = MAX (max_depth,
                                 (p_trace->trace_struct.non_null_base) * conv);
                status = llnext (&p_layout->traces, &p_trace);
                }
            }
        /* We now know the top and base of log                                      */
        /* data so store it in the structure.                                       */
        
        p_layout->interval.true_data_top = min_depth;
        p_layout->interval.true_data_base = max_depth;
        
        /* Try to compute the length of the                                         */
        /* tracks, especially if the retrieved                                      */
        /* data interval is less than the                                           */
        /* requested data interval.                                                 */
        
        status = xsz_find_log_limits (p_layout, &min_depth, &max_depth);
        
        /* Store these in the structure.                                            */
        
        p_layout->interval.track_top = min_depth;
        p_layout->interval.track_base = max_depth;
        
        /* If max_depth = 0.0 then the base of                                      */
        /* the track to plot is indeterminate                                       */
        /* because of a missing formation top,                                      */
        /* and missing TD and missing data...                                       */
        
        greatest_depth = MAX (greatest_depth, max_depth);
        
        /* Get the next well and do it again.                                       */
        
        status = llnext (&p_xsec->drawing.log_plots, &p_layout);
        }
    /* Start the second pass throught the                                           */
    /* wells.  This time set artificial                                             */
    /* maximum depths, if necessary.  Also                                          */
    /* position the tracks.                                                         */
    
    status = llfirst (&p_xsec->drawing.log_plots, &p_layout);
    for (i = 0; i < no_layouts; i++)
        {
        if (p_layout->interval.track_base < 0.00001)
            {
            p_layout->interval.track_base = greatest_depth;
            }
        pwell1 = (WELL_HDR_1 *)p_layout->pwell->detail.ptr;
        
        /* Locate these tracks in vertical space.                                   */
        switch (p_xsec->xs_type)
            {
        case XS_NO_DATUM:
            p_layout->interval.true_rel_top = p_layout->interval.track_top;
            p_layout->interval.true_rel_base = p_layout->interval.track_base;
            break;
            
        case XS_STRUCTURAL:
            p_layout->interval.true_rel_top = pwell1->elevation.value -
                p_layout->interval.track_top;
            p_layout->interval.true_rel_base = pwell1->elevation.value -
                p_layout->interval.track_base;
            break;
            
        case XS_STRATIGRAPHIC:
            status = ls_get_top (p_layout->pwell->uwi, p_xsec->xs_datum_select,
                                 &top_struct);
            if (status < SUCCESS)
                {                   /* No top...hang in middle of data.             */
                average = (p_layout->interval.track_base -
                           p_layout->interval.track_top) * 0.5;
                p_layout->interval.true_rel_top = average -
                    p_layout->interval.track_top;
                p_layout->interval.true_rel_base = average -
                    p_layout->interval.track_base;
                }
            else
                {                   /* Hang from returned top.                      */
                p_layout->interval.true_rel_top = top_struct.Depth.md -
                    p_layout->interval.track_top;
                p_layout->interval.true_rel_base = top_struct.Depth.md -
                    p_layout->interval.track_base;
                break;
                }
            }
        /*  Calculate track length.                                                 */
        
        track_length = (p_layout->interval.track_base - p_layout->interval.track_top) /
            p_xsec->depth_scale;
        
        if (track_length < 0.00001)
            track_length = 0.00001;
        
        /* Cycle through all tracks for this well                                   */
        
        no_tracks = llcount (&p_layout->tracks);
        
        track_end_max = 0.0;        /* Initialize logplot width accumulator.        */
        
        for (j = 0; j < no_tracks; j++)
            {
            status = (j == 0) ? llfirst (&p_layout->tracks, &p_track) :
                                llnext (&p_layout->tracks, &p_track);
            
            if (p_track->width <= 0.0)  /*  Validate track width.                   */
                {
                status = am_message (AM_ALERT, mg_message (XS_BAD_TRACK_WIDTH));
                if (p_xsec->review_all_switch != TRUE)
                    status = xs_trackdef_cdlg_init (p_xsec);
                return(FAIL);
                }
            /* Accumulate logplot width.                                            */
            /* This assumes track xorigin is                                        */
            /* referenced to left side of                                           */
            /* logplot (p_layout->xorigin).                                         */
            
            track_end = p_track->xorigin + p_track->width;
            track_end_max = MAX (track_end_max, track_end);
            
            /*  Load the track length.                                              */
            p_track->length = track_length;
            
            /*  If Logarithmic track,                                               */
            /*  determine start value from                                          */
            /*  traces or hardcoded default.                                        */
            
            if (p_track->type == LOG_TRACK)
                {
                no_traces = llcount (&p_layout->traces);
                p_track->divisions.start_value = 0.2;
                
                for (k = 0; k < no_traces; k++)
                    {
                    status = (k == 0) ? llfirst (&p_layout->traces, &p_trace) :
                                        llnext (&p_layout->traces, &p_trace);
                    
                    if (p_trace->track_num == p_track->number AND p_trace->left >
                        0.00001)
                        p_track->divisions.start_value = p_trace->left;
                    }
                }
            }
        /*  Load the logplot width.                                                 */
        
        p_layout->width = track_end_max;
        p_layout->log_hdr.width = track_end_max;
        p_layout->scale_block.width = track_end_max;
        p_layout->text_block.width = track_end_max;
        
        /*   Compute the logplot header size                                        */
        
        status = xs_hdr_size (p_layout);
        
        /*   Compute the scale block size                                           */
        
        status = xs_scale_blk_size (p_layout);
        
        /*   Compute the length of a logplot                                        */
        
        llfirst (&p_layout->tracks, &p_track);
        p_layout->length = p_track->length + p_layout->log_hdr.height +
            p_layout->scale_block.height + p_layout->text_block.height;
#ifdef DEBUG
        printf ("%s  LogPlot Length = %f\n", progname, p_layout->length);
        printf ("%s  Header Length = %f\n", progname, p_layout->log_hdr.height);
        printf ("%s  Scale Length  = %f\n", progname, p_layout->scale_block.height);
        printf ("%s  Track Length  = %f\n", progname, p_track->length);
        printf ("%s  Text Length   = %f\n", progname, p_layout->text_block.height);
        printf ("%s  LogPlot Width = %f\n\n", progname, p_layout->width);
#endif
        
        /* Get the next well and do it again.                                       */
        
        status = llnext (&p_xsec->drawing.log_plots, &p_layout);
        
        }
    /* end layouts loop                                                             */
    
    xsz_well_sb_size (p_xsec);      /* set same Scale Block height for all wells    */
    
    /* Call individual modules to resolve cross section                             */
    /* Compute position of layouts in y direc.                                      */
    
    status = xs_struct_loc_welly (p_xsec);
    if (status == FAIL)
        return(FAIL);
    
    /* Compute position of layouts in x direc.                                      */
    
    status = xs_struct_loc_wellx (p_xsec);
    if (status == FAIL)
        return(FAIL);
    
    /* Resolve y locations of logplot structs                                       */
    
    status = xs_resolve_layout (&(p_xsec->drawing.log_plots));
    if (status == FAIL)
        return(FAIL);
    
    /* Resolve title block size and position                                        */
    
    status = xs_resolve_title (p_xsec);
    if (status == FAIL)
        return(FAIL);
    
    return(SUCCESS);
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    On each well, set the size of the scale_block based on                        */
/*    the largest one found in any well.                                            */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT xsz_well_sb_size(XS_STRUCT *pxs);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    pxs             -(XS_STRUCT *)                                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xs_chg.c>                                                         */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT xsz_well_sb_size (XS_STRUCT *pxs)
#else
static INT xsz_well_sb_size (pxs)
XS_STRUCT *pxs;
#endif
    {
    LAYOUT_STRUCT *pls;
    TRACK_STRUCT *pt;
    LP_TRACE_STRUCT *ptrace;
    
    INT l, j, jj;
    INT ntraces, ntracks, nlyts, count, len, st;
    INT max_tname_len, most_traces_count;
    TRACK_STRUCT *max_tname_ptrack;
    
    DOUBLE sb_ht, max_sb_ht, tmpfl;
    
    /********************** Start Code ***********************                      */
    max_tname_len = most_traces_count = len = 0;
    max_sb_ht = sb_ht = 0.0;
    max_tname_ptrack = (TRACK_STRUCT *)0;
    
    count = len = 0;
    st = SUCCESS;
    
    nlyts = llcount (&pxs->drawing.log_plots);
    
    /*  Loop through all the wells.                                                 */
    for (l = 0; l < nlyts; l++)
        {
        st = (l) ? llnext (&pxs->drawing.log_plots,
                           &pls) : llfirst (&pxs->drawing.log_plots, &pls);
        
        max_sb_ht = MAX (max_sb_ht, pls->scale_block.height);
        
        ntraces = llcount (&pls->traces);
        ntracks = llcount (&pls->tracks);
        
        if (ntraces == 0 OR (ntracks == 0))
            continue;
        
        for (j = 0; j < ntracks; j++)
            {
            st = (j) ? llnext (&pls->tracks, &pt) : llfirst (&pls->tracks, &pt);
            
            for (count = jj = 0; jj < ntraces; jj++)
                {
                st = (jj) ? llnext (&pls->traces,
                                    &ptrace) : llfirst (&pls->traces, &ptrace);
                
                if (ptrace->track_num != pt->number)
                    continue;
                
                len = MIN (MAX_TRACE_NAME_LEN,
                           strlen (ptrace->trace_struct.trace_name));
                
                /* count number of traces on this track                             */
                count++;
                
                if (len > max_tname_len)
                    {
                    max_tname_len = len;
                    max_tname_ptrack = pt;
                    }
                }
            /* End traces loop                                                      */
            /* keep a tally of the most traces                                      */
            /* found in any given track                                             */
            if (count > most_traces_count)
                {
                most_traces_count = count;
                }
            }
        /* End tracks loop                                                          */
        }
    /* End layouts loop                                                             */
    /*******************************************************                        */
    /**                                                                             */
    /*  At this point we have the length of the longest                             */
    /*  trace name, the track in which it sits, the                                 */
    /*  maximum number of traces found in any track,                                */
    /*  the track with the most traces in it and finally                            */
    /*  we have recorded the maximum scale_block height                             */
    /*  found in any well.                                                          */
    /**                                                                             */
    /*******************************************************                        */
    if (max_tname_ptrack != (TRACK_STRUCT *)0)
        {
        tmpfl = max_tname_ptrack->width / (DOUBLE)(max_tname_len + 12);
        sb_ht = (most_traces_count == 0) ?
            max_sb_ht : (4.0 * tmpfl * (DOUBLE)most_traces_count);
        }
    else
        {
        sb_ht = max_sb_ht;
        }
    /*  Loop through all the wells placing the new height value.                    */
    for (l = 0; l < nlyts; l++)
        {
        st = (l) ? llnext (&pxs->drawing.log_plots,
                           &pls) : llfirst (&pxs->drawing.log_plots, &pls);
        
        llfirst (&pls->tracks, &pt);
        pls->scale_block.height = sb_ht;
        pls->length = pt->length + pls->log_hdr.height + pls->scale_block.height +
            pls->text_block.height;
        
        }
    return SUCCESS;
    }
/* END:                                                                             */
