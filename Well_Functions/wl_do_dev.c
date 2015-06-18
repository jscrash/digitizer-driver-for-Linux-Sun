/* DEC/CMS REPLACEMENT HISTORY, Element WL_DO_DEV.C*/
/* *13   18-OCT-1991 16:24:10 JTM "(SPR 0) Fixes by Charlie for WELL DEPTH and ISOPACH - SOURCE option was ignored. Case 4568."*/
/* *12    9-JUL-1991 20:28:10 CHARLIE "(SPR 8500) engage suppress surface symbol"*/
/* *11   23-MAY-1991 10:03:54 MING "(SPR -1) check status from wl_get_md. this code from CMS ver 9 not 10. Cod in 10 unsavable.*/
/*(CHARLIE)"*/
/* *10   30-NOV-1990 15:47:49 JULIAN "(SPR 6234) Add clipping after transform well nlist"*/
/* *9    17-OCT-1990 16:45:55 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  7B1  17-OCT-1990 16:12:37 GILLESPIE "Merge Ernie deltas"*/
/* *8    14-AUG-1990 13:40:10 VINCE "(SPR 5644) Header Standardization"*/
/* *7     1-APR-1990 10:50:43 GILLESPIE "(SPR 1) Zero out arrays to avoid uninitialized values for x_ and y_offset"*/
/* *6    17-JAN-1990 17:23:35 GILLESPIE "(SPR 6001) Fix up calling args for picks"*/
/* *5    18-DEC-1989 11:52:21 CONROY "(SPR 5078) Mods for plotting GKS polymarkers"*/
/* *4    27-OCT-1989 08:20:22 CONROY "(SPR 5049) Always get horizon marker offset from top"*/
/* *3    10-OCT-1989 10:04:21 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2     2-AUG-1989 16:04:26 GORDON "(SPR 102) bug in call to wm_gsmk, range checking xmax instead of ymax"*/
/* *1    19-JUN-1989 13:39:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_DO_DEV.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* * FILE DESCRIPTION *                                                             */
/* --------------------                                                             */
/* ************************************************************************         */
/*                                                                                  */
/*   NAME: WL_DO_DEVIATED_WELLS.C                                                   */
/*                                                                                  */
/*   AUTHOR: JOHN CONROY                                                            */
/*                                                                                  */
/*   DESCRIPTION: ROUTINE TO POST THE TOP POSITION, TRACE, HORIZON LOCATORS,        */
/*      AND WELL SYMBOL (AT BOTTOM LOCATION) FOR DEVIATED WELLS.                    */
/*      THIS ROUTINE ALSO DETERMINES THE ANNOTATION OFFSETS AND                     */
/*      UP VECTORS FOR ANNOTATION ALONG THE TRACE.                                  */
/*                                                                                  */
/*      FIRST THE WELL SYMBOLS ARE POSTED, THEN THE TRACES, AND                     */
/*      ANY TOP POSITIONS AND HORIZON LOCATIONS THAT ARE WITHIN                     */
/*      THE MAP WINDOW.  IF THE HORIZON LOCATION IS NOT VISIBLE,                    */
/*      THEN THE UP VECTOR IS SET TO (0,0).                                         */
/*                                                                                  */
/*      THREE STYLES OF TRACES ARE POSSIBLE:                                        */
/*          1) FULL TRACE: DRAWS THE TRANSFORMED AND CLIPPED                        */
/*              NLIST FOR THE TRACE FOR THE WELL.  THIS                             */
/*              IS AVAILABLE FOR ANY OF THE THREE OVERLAY                           */
/*              TYPES ("WELLS", "WELL DEPTH", AND "WELL ISOPACH").                  */
/*                                                                                  */
/*          2) FAST TRACE - WELLS OVERLAYS: DRAWS 1 STRAIGHT LINE                   */
/*              SEGMENT FROM THE TOP POSITION TO THE BOTTOM                         */
/*              POSITION.                                                           */
/*                                                                                  */
/*          3) FAST TRACE - WELL DEPTH/WELL ISOPACH OVERLAYS:                       */
/*              DRAWS 2 STRAIGHT LINE SEGMENTS.                                     */
/*              ONE FROM TOP POSITION TO THE                                        */
/*              HORIZON LOCATOR POSITION AND A SECOND FROM                          */
/*              THE HORIZON LOCATOR TO THE BOTTOM POSITION.                         */
/*              MARKS THE TOP POSITION WITH AN ASTERISK.                            */
/*                                                                                  */
/*   ************************************************************************       */


/* * INCLUDE FILES *                                                                */
/* -----------------                                                                */

#include "esi_c_lib.h"

#ifndef ESI_WL_H
#include "esi_wl.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_CS_H
#include "esi_cs.h"
#endif

typedef DOUBLE WL_POINT[2];

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Routine to post the top position, trace, horizon locators, and well           */
/*    symbol (at bottom location) for deviated wells.  This routine also            */
/*    determines the annotation offsets and up vectors for annotation along the     */
/*    trace.                                                                        */
/*                                                                                  */
/*    First the well symbols are posted, then the traces, and any top               */
/*    positions and horizon locations that are within the map window.               */
/*    If the horizon location is not visible, then the up vector is set to (0,0).   */
/*                                                                                  */
/*    Three styles of traces are possible:                                          */
/*        1) Full trace: draws the transformed and clipped nlist for the trace      */
/*            for the well.  This is available for any of the three overlay         */
/*            types ("wells", "well depth", and "well isopach").                    */
/*                                                                                  */
/*        2) Fast trace - well overlays: draws one straight line segment            */
/*            from the top position to the bottom position.                         */
/*                                                                                  */
/*        3) Fast trace - well depth/well isopach overlays: draws 2 straight        */
/*            line segments.  One from top postion to the horizon locator           */
/*            position and a second from the horizon locator to the bottom          */
/*            position.  Marks the top position with an asterisk.                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT wl_do_deviated_wells(WELLS *wells,NLIST_HEADER dev_nlist,       */
/*        BOOL suppress_text, NLIST_HEADER label_nlist,PICKABLE_WELLS *picks);      */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    wells           -(WELLS *) Well overlay structure.                            */
/*    dev_nlist       -(NLIST_HEADER)                                               */
/*    suppress_text   -(BOOL)                                                       */
/*    label_nlist     -(NLIST_HEADER)                                               */
/*    picks           -(PICKABLE_WELLS *)                                           */
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
publicdef INT wl_do_deviated_wells (WELLS *wells, NLIST_HEADER dev_nlist,
                                    BOOL suppress_text, NLIST_HEADER label_nlist,
                                    PICKABLE_WELLS *picks)
#else
publicdef INT wl_do_deviated_wells (wells, dev_nlist, suppress_text, label_nlist,
                                    picks)
WELLS *wells;
NLIST_HEADER dev_nlist;
BOOL suppress_text;
NLIST_HEADER label_nlist;
PICKABLE_WELLS *picks;
#endif
    {
    
    INT status;
    BOOL batch;
    /* DEVIATED WELLS NLIST DIMENSIONS                                              */
    UWI *uwi;
    INT *well_symbol;
    FLOAT *base_x;
    FLOAT *base_y;
    FLOAT *top_x;
    FLOAT *top_y;
    UWI *top_position;
    /* LABEL NLIST DIMENSIONS 3-8                                                   */
    
    FLOAT *label_x;
    FLOAT *label_y;
    FLOAT *x_offset;
    FLOAT *y_offset;
    FLOAT *x_up;
    FLOAT *y_up;
    DOUBLE up_x, up_y, off_x, off_y;
    
    /* ARRAYS HOLDING ALL TOPS AND BASES (TRANSFORMED)                              */
    FLOAT *trans_base_x;
    FLOAT *trans_base_y;
    FLOAT *trans_top_x;
    FLOAT *trans_top_y;
    
    /* DISPLAY ARRAYS FOR TOPS, BASES, AND FORMATIONS                               */
    INT tops_displayed; /* this is SURFACE POSITION not TOP HORIZON */
    FLOAT *top_display_x;
    FLOAT *top_display_y;
    
    INT bases_displayed; /* this is BOTTOM HOLE LOCATION if (!symbol_type) */
    FLOAT *base_display_x;
    FLOAT *base_display_y;
    
    INT forms_displayed; /* position of '+' symbol marking formation penetration */
    FLOAT *form_display_x;
    FLOAT *form_display_y;
    
    /* ARRAY FOR STORING DEVIATION COORDS                                           */
    DOUBLE *dev_x;
    DOUBLE *dev_y;
    UINT dev_pts;
    
    /* DISPLAY ARRAYS FOR DEVIATION TRACES                                          */
    UINT trace_pts;
    FLOAT *trace_x;
    FLOAT *trace_y;
    
    /* OTHER LOCAL VARIABLES                                                        */
    MAP_STRUCTURE *map;
    INT ii, jj;
    UINT num_wells;
    INT current_array_size;
    PROJECTION_NAME projection;
    FLOAT dummy;
    DOUBLE hor_dx, hor_dy, md, tvd, size;
    DOUBLE base_hor_dx, base_hor_dy, base_md, base_tvd;
    DOUBLE elevation;
    DOUBLE trace_x1, trace_y1, trace_x2, trace_y2;
    DOUBLE xmin, ymin, xmax, ymax;
    BOOL vis, mod;
    BOOL transform;
    BOOL get_text_position;
    SYMBOL symbol;
    INT label_status;
    NLIST_HEADER deviation;
    
    WL_POINT xy_in;
    WL_POINT xy_out;
    /* NLIST VARIABLES                                                              */
    static UINT d_list[8] = 
        {
        1, 2, 3, 4, 5, 6, 7, 8
        };
    VOIDPTR v_list[8];
    static UINT d_devlist[2] = 
        {
        2, 3
        };
    static INT units[2] = 
        {
        0, 0
        };
    static INT dim_type[2] = 
        {
        FLOAT_TYPE, FLOAT_TYPE
        };
    static size_t dim_size[2] = 
        {
        sizeof(DOUBLE), sizeof(DOUBLE)
        };
    VOIDPTR v_devlist[2];
    
    /* **********************************************************************       */
    
    /* * CHECK WHETHER REALLY NEED TO DO ANYTHING *                                 */
    /* --------------------------------------------                                 */
    
    get_text_position = (NOT suppress_text OR wells->zfield);
    if (wells->suppress_symbols AND NOT get_text_position)
        {
        return SUCCESS;
        }
    /* * GET DEVIATED WELLS DATA *                                                  */
    /* ---------------------------                                                  */
    
    nl_inq_nlist_int (dev_nlist, NL_NLIST_NPOINTS, (INT *)&num_wells);
    if (num_wells EQUALS 0)
        {
        return SUCCESS;
        }
    uwi = (UWI *) tc_alloc (num_wells * sizeof(UWI));
    well_symbol = (INT *) tc_alloc (num_wells * sizeof(INT));
    base_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    base_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    top_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    top_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
    
    v_list[0] = (VOIDPTR)uwi;
    v_list[1] = (VOIDPTR)well_symbol;
    v_list[2] = (VOIDPTR)base_x;
    v_list[3] = (VOIDPTR)base_y;
    v_list[4] = (VOIDPTR)top_x;
    v_list[5] = (VOIDPTR)top_y;
    
    nl_set_current_point (dev_nlist, 1, 1);
    nl_get_points (dev_nlist, num_wells, 6, d_list, v_list, (UINT *)&ii);
    
    /* * ALLOCATE ARRAYS FOR THE OUTPUT NLIST *                                     */
    /* ----------------------------------------                                     */
    
    label_x = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    label_y = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    x_offset = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    y_offset = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    x_up = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    y_up = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
    
    if (wells->ov_type AND NOT suppress_text)
        {
        size = wells->text_size;
	if (wells->ov_type == 1)
	{
        form_display_x = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
        form_display_y = (FLOAT *) tc_zalloc (num_wells * sizeof(FLOAT));
	}
	else
	{
	/* from the 'wish' list (ie finder user documentation) - be able to post
	   BOTH horizon locations if wells->ov_type = WELL ISOPACH */
        form_display_x = (FLOAT *) tc_zalloc (2 * num_wells * sizeof(FLOAT));
        form_display_y = (FLOAT *) tc_zalloc (2 * num_wells * sizeof(FLOAT));
	}
        forms_displayed = 0;
        }
    /* * GET MAP AND DETERMINE TRANSFORMATION NEEDS *                               */
    /* ----------------------------------------------                               */
    
    mp_get_current_map (&map);
    xmin = map->lower_left_xy.x;
    ymin = map->lower_left_xy.y;
    xmax = map->upper_right_xy.x;
    ymax = map->upper_right_xy.y;
    
    qs_inq_i (QS_IF_BATCH, &batch); /* was doing this for every well, so moved here */
    qs_inq_c (QS_PROJECT_PROJECTION, projection, (INT *)0);
    if (transform = (ARE_DIFFERENT (map->map_projection, projection)))
        {
/* (CP) If the project projection is geodetic (as SOME say we support)
the UOM_TYPE will be 'ANGULAR' yet the
offsets will be in feet or meters (most likely) and these are NOT handled
by the TU_UNIT_CONVERSION tools at this time. Offsets which are transformed
to geodetic would HAVE to know at what latitude and longitude they were
measured from, otherwise the fact that the earth is a little flat on top and
a little fat around the middle would throw all the calculations off.
I am not going to address this issue now, just report my concerns here. */

        trans_base_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        trans_base_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        trans_top_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        trans_top_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        }
    else
        {
        trans_base_x = base_x;
        trans_base_y = base_y;
        trans_top_x = top_x;
        trans_top_y = top_y;
        }
    /* * INITIALIZE OTHER NEEDED THINGS IF DISPLAYING SYMBOLS *                     */
    /* --------------------------------------------------------                     */
    
    if (NOT wells->suppress_symbols)
        {
        wm_gsplci (wells->symbol_color);
        wm_gsln (1);
        wm_gslwsc (1.0);
        if (NOT wells->symbol_type)
            {
            strcpy (symbol.symbol_group, CS_V4_WELLS_GROUP);
            symbol.symbol_color = wells->symbol_color;
            symbol.symbol_size = wells->symbol_size;
            }
        else                        /* ALLOCATE BASE POSITION VARIABLES             */
            {
            base_display_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
            base_display_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
            bases_displayed = 0;
            }
        /* ALLOCATE TOP POSITION VARIABLES                                          */
        
        top_display_x = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        top_display_y = (FLOAT *) tc_alloc (num_wells * sizeof(FLOAT));
        tops_displayed = 0;
        
        /* ALLOCATE ARRAYS FOR DEVIATIONS                                           */
        
        current_array_size = (wells->full_trace) ? 100 : 3;
        dev_x = (DOUBLE *) tc_alloc (current_array_size * sizeof(DOUBLE));
        dev_y = (DOUBLE *) tc_alloc (current_array_size * sizeof(DOUBLE));
        trace_x = (FLOAT *) tc_alloc (current_array_size * sizeof(FLOAT));
        trace_y = (FLOAT *) tc_alloc (current_array_size * sizeof(FLOAT));
        }
    /* * LOOP ON WELLS *                                                            */
    /* -----------------                                                            */
    
    for (ii = 0; ii < num_wells; ii++)
        {
        
        /* * GET TOP AND BASE LOCATIONS *                                           */
        /* ------------------------------                                           */
        
        if (transform)
            {
            xy_in[0] = top_x[ii];
            xy_in[1] = top_y[ii];
            ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
            trans_top_x[ii] = xy_out[0];
            trans_top_y[ii] = xy_out[1];
            
            xy_in[0] = base_x[ii];
            xy_in[1] = base_y[ii];
            ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
            trans_base_x[ii] = xy_out[0];
            trans_base_y[ii] = xy_out[1];
            }
        /* * GET LABEL POSITION, USED IN PLOTTING 3 POINT TRACE *                   */
        /* ------------------------------------------------------                   */
        
        if (wells->ov_type)
            {
            label_status = wl_get_md (uwi[ii], wells->bottom_horizon, wells->source,
		&md, &elevation, FALSE);
            wes_get_dir_tvd (uwi[ii], md, &hor_dx, &hor_dy, &tvd);
	    if (wells->ov_type == 2 AND label_status == SUCCESS) /* WELL ISOPACH */
	    {
	    /*  we want to put isopach value by upper penetration point, yet
		save base hor_dx, hor_dy, tvd for possible later posting */
		base_hor_dx = hor_dx;
		base_hor_dy = hor_dy;
		base_md = md;
		base_tvd = tvd;
		label_status = wl_get_md( uwi[ii], wells->top_horizon,
			wells->source, &md, &elevation, FALSE);
		wes_get_dir_tvd (uwi[ii], md, &hor_dx, &hor_dy, &tvd);
	    }
            
            if (transform)
                {
                xy_in[0] = top_x[ii] + hor_dx;
                xy_in[1] = top_y[ii] + hor_dy;
                ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
                label_x[ii] = xy_out[0];
                label_y[ii] = xy_out[1];
                }
            else
                {
                label_x[ii] = top_x[ii] + hor_dx;
                label_y[ii] = top_y[ii] + hor_dy;
                }
            }
        else
            {
            label_x[ii] = trans_base_x[ii];
            label_y[ii] = trans_base_y[ii];
            }
        /* * IF WRITING TEXT, CHECK IF HORIZON POSITION IN WINDOW *                 */
        /* --------------------------------------------------------                 */
        
        if (wells->ov_type AND NOT suppress_text AND 
		(xmin <= label_x[ii] AND label_x[ii] <= xmax AND
		 ymin <= label_y[ii] AND label_y[ii] <= ymax))
            {
            if (label_status == SUCCESS)
	    {
	    form_display_x[forms_displayed] = label_x[ii];
            form_display_y[forms_displayed] = label_y[ii];
            forms_displayed++;
	    }
#if 0
/* fix this if requested. display the bottom horizon position
also with a '+' */
	    if (wells->ov_type == 2)
	    {
	    ???
	    }
/* use base_hor_dx, etc to stuff a map projection converted label_x and y
into form_displayed array. Does this effect full trace?? */
#endif
            /* GET TEXT PLACEMENT PARAMETERS                                        */
            
            /* FULL TRACE: GO UP AND DOWN 100                                       */
            if (wells->full_trace)
                {
                md -= 100.0;
                wes_get_dir_tvd (uwi[ii], md, &trace_x1, &trace_y1, &tvd);
                trace_x1 += top_x[ii];
                trace_y1 += top_y[ii];
                
                md += 200.0;
                wes_get_dir_tvd (uwi[ii], md, &trace_x2, &trace_y2, &tvd);
                trace_x2 += top_x[ii];
                trace_y2 += top_y[ii];
                
                if (transform)
                    {
                    xy_in[0] = trace_x1;
                    xy_in[1] = trace_y1;
                    ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
                    trace_x1 = xy_out[0];
                    trace_y1 = xy_out[1];
                    
                    xy_in[0] = trace_x2;
                    xy_in[1] = trace_y2;
                    ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
                    trace_x2 = xy_out[0];
                    trace_y2 = xy_out[1];
                    }
                }
            /* THREE POINT TRACE                                                    */
            else
                {
                trace_x1 = trans_top_x[ii];
                trace_y1 = trans_top_y[ii];
                trace_x2 = trans_base_x[ii];
                trace_y2 = trans_base_y[ii];
                }
            wl_text_placement (size, trace_x1, trace_y1, label_x[ii], label_y[ii],
                               trace_x2, trace_y2, &off_x, &off_y, &up_x, &up_y);
            x_offset[ii] = off_x;
            y_offset[ii] = off_y;
            x_up[ii] = up_x;
            y_up[ii] = up_y;
            }
        /* * DO PROCESSING FOR "SYMBOL" DRAWING *                                   */
        /* --------------------------------------                                   */
        
        if (NOT wells->suppress_symbols)
            {
            
            /* * DO BASE *                                                          */
            /* -----------                                                          */
            
            if (xmin <= trans_base_x[ii] AND trans_base_x[ii] <= xmax AND ymin <=
                trans_base_y[ii] AND trans_base_y[ii] <= ymax)
                {
                wlz_add_pick (picks, uwi[ii]);
                
                if (NOT wells->symbol_type)
                    {
                    symbol.symbol_location.x = trans_base_x[ii];
                    symbol.symbol_location.y = trans_base_y[ii];
                    symbol.symbol_type = well_symbol[ii];
                    mm_draw_symbol (&symbol);
                    
                    /* ------------------------------------------------             */
                    /* add symbol into well legend list during plotting             */
                    /* ------------------------------------------------             */
                    if (batch)
                        {
                        pl_add_well_symbol (symbol.symbol_type, symbol.symbol_color);
                        }
                    }
                else
                    {
                    base_display_x[bases_displayed] = trans_base_x[ii];
                    base_display_y[bases_displayed] = trans_base_y[ii];
                    bases_displayed++;
                    }
                }
            /* * DO TOP *                                                           */
            /* ----------                                                           */
            
            if (xmin <= trans_top_x[ii] AND trans_top_x[ii] <= xmax AND ymin <=
                trans_top_y[ii] AND trans_top_y[ii] <= ymax)
                {
                top_display_x[tops_displayed] = trans_top_x[ii];
                top_display_y[tops_displayed] = trans_top_y[ii];
                tops_displayed++;
                }
            /* * CONSTRUCT TRACE IN MAP PROJECTION *                                */
            /* -------------------------------------                                */
            
            /* FULL TRACE WITH DEVIATIONS                                           */
            
	    status = wes_load_dir_nlist (uwi[ii], &deviation);
            if (wells->full_trace AND status EQUALS SUCCESS)
                {
                /* REALLOC ARRAYS IF NECESSARY                                      */
                
                nl_inq_nlist_int (deviation, NL_NLIST_NPOINTS, (INT *)&jj);
                if (jj > current_array_size)
                    {
                    current_array_size = 2 * jj;
                    dev_x = (DOUBLE *) tc_realloc (dev_x,
                                                   current_array_size *
                                                   sizeof(DOUBLE));
                    dev_y = (DOUBLE *) tc_realloc (dev_y,
                                                   current_array_size *
                                                   sizeof(DOUBLE));
                    trace_x = (FLOAT *) tc_realloc (trace_x,
                                                    current_array_size *
                                                    sizeof(FLOAT));
                    trace_y = (FLOAT *) tc_realloc (trace_y,
                                                    current_array_size *
                                                    sizeof(FLOAT));
                    }
                /* GET DEVIATION DELTAS                                             */
                
                v_devlist[0] = (VOIDPTR)dev_x;
                v_devlist[1] = (VOIDPTR)dev_y;
                nl_set_current_point (deviation, 1, 1);
                nl_get_points_convert (deviation, jj, 2, d_devlist, v_devlist, units,
                                       dim_size, dim_type, &dev_pts);
                
                /* CONSTRUCT ABSOLUTE DEVIATIONS,                                   */
                /*      TRANSFORM IF NECESSARY                                      */
                
                if (transform)
                    {
                    for (jj = 0; jj < dev_pts; jj++)
                        {
                        xy_in[0] = top_x[ii] + dev_x[jj];
                        xy_in[1] = top_y[ii] + dev_y[jj];
                        ct_map_xy (xy_in[0], xy_in[1], &xy_out[0], &xy_out[1]);
                        dev_x[jj] = xy_out[0];
                        dev_y[jj] = xy_out[1];
                        }
                    }
                else
                    {
                    for (jj = 0; jj < dev_pts; jj++)
                        {
                        dev_x[jj] += top_x[ii];
                        dev_y[jj] += top_y[ii];
                        }
                    }
                }
            /* NO DEVIATIONS => 2 OR 3 POINT TRACE                                  */
            else
                {
                dev_x[0] = trans_top_x[ii];
                dev_y[0] = trans_top_y[ii];
                dev_x[1] = label_x[ii];
                dev_y[1] = label_y[ii];
                dev_x[2] = trans_base_x[ii];
                dev_y[2] = trans_base_y[ii];
                dev_pts = 3;
                }
            /* * CLIP AND DRAW TRACE *                                              */
            /* -----------------------                                              */
            
            trace_x[0] = dev_x[0];
            trace_y[0] = dev_y[0];
            trace_pts = 1;
            /* LOOP ON LINE SEGMENTS                                                */
            for (jj = 0; jj < dev_pts - 1; jj++)
                {
                trace_x1 = dev_x[jj];
                trace_y1 = dev_y[jj];
                trace_x2 = dev_x[jj + 1];
                trace_y2 = dev_y[jj + 1];
                jg_clip_line (&trace_x1, &trace_y1, &trace_x2, &trace_y2, xmin, ymin,
                              xmax, ymax, &vis, &mod);
                
                /* LINE SEGMENT VISABLE                                             */
                if (vis)
                    {
                    /* LINE SEGMENT NOT CLIPPED                                     */
                    /* FIRST POINT NOT CLIPPED                                      */
                    if (NOT mod OR (trace_x1 EQUALS dev_x[
                                                        jj] AND trace_y1 EQUALS 
                                    dev_y[jj]))
                        {
                        trace_x[trace_pts] = trace_x2;
                        trace_y[trace_pts] = trace_y2;
                        trace_pts++;
                        }
                    /* FIRST POINT CLIPPED:                                         */
                    /*DRAW AND RE-INIT                                              */
                    else
                        {
                        if (trace_pts > 1)
                            {
                            wm_gpl (trace_pts, trace_x, trace_y);
                            }
                        trace_x[0] = trace_x1;
                        trace_y[0] = trace_y1;
                        trace_x[1] = trace_x2;
                        trace_y[1] = trace_y2;
                        trace_pts = 2;
                        }
                    }
                /* LINE SEGMENT NOT VISIBLE:                                        */
                /*DRAW AND RE-INIT                                                  */
                else
                    {
                    if (trace_pts > 1)
                        {
                        wm_gpl (trace_pts, trace_x, trace_y);
                        }
                    trace_pts = 0;
                    }
                }
            /* DRAW ANY LEFTOVERS                                                   */
            if (trace_pts > 1)
                {
                wm_gpl (trace_pts, trace_x, trace_y);
                }
            }
        }
    /* * DISPLAY BASE AND TOP SYMBOLS *                                             */
    /* --------------------------------                                             */
    
    if (NOT wells->suppress_symbols)
        {
        wm_gspmci (wells->symbol_color);
        dummy = wells->symbol_size * map->wcs_to_inches;
        wm_gsmksc (dummy);
        if (wells->symbol_type AND bases_displayed > 0)
            {
            wm_gsmk (wells->symbol_type);
            wm_gpm (bases_displayed, base_display_x, base_display_y);
            }
        if (tops_displayed > 0 && (!wells->suppress_surface_symbols))
            {
            wm_gsmk (GAST);         /* Asterisk type                                */
            wm_gpm (tops_displayed, top_display_x, top_display_y);
            }
        }
    /* * POST THE HORIZON POSITION LOCATION *                                       */
    /* --------------------------------------                                       */
    
    if (NOT suppress_text AND wells->ov_type AND forms_displayed > 0)
        {
        wm_gspmci (wells->text_color);
        wm_gsmk (GPLUS);
        dummy = wells->symbol_size * map->wcs_to_inches * 0.5;
        wm_gsmksc (dummy);
        wm_gpm (forms_displayed, form_display_x, form_display_y);
        }
    /* * FILL LABEL_NLIST *                                                         */
    /* --------------------                                                         */
    
    nl_inq_nlist_int (label_nlist, NL_NLIST_NPOINTS, (INT *)&jj);
    nl_set_current_point (label_nlist, jj, 1);
    v_list[2] = (VOIDPTR)label_x;
    v_list[3] = (VOIDPTR)label_y;
    v_list[4] = (VOIDPTR)x_offset;
    v_list[5] = (VOIDPTR)y_offset;
    v_list[6] = (VOIDPTR)x_up;
    v_list[7] = (VOIDPTR)y_up;
    nl_add_points (label_nlist, num_wells, 8, d_list, v_list);
    
    wl_get_md( uwi[0], wells->top_horizon,
			wells->source, &md, &elevation, TRUE);
    /* * FREE MEMORY *                                                              */
    /* ---------------                                                              */
    
    tc_free ((VOIDPTR)uwi);
    tc_free ((VOIDPTR)well_symbol);
    tc_free ((VOIDPTR)base_x);
    tc_free ((VOIDPTR)base_y);
    tc_free ((VOIDPTR)top_x);
    tc_free ((VOIDPTR)top_y);
    
    tc_free ((VOIDPTR)label_x);
    tc_free ((VOIDPTR)label_y);
    tc_free ((VOIDPTR)x_offset);
    tc_free ((VOIDPTR)y_offset);
    tc_free ((VOIDPTR)x_up);
    tc_free ((VOIDPTR)y_up);
    
    if (transform)
        {
        tc_free ((VOIDPTR)trans_base_x);
        tc_free ((VOIDPTR)trans_base_y);
        tc_free ((VOIDPTR)trans_top_x);
        tc_free ((VOIDPTR)trans_top_y);
        }
    if (wells->ov_type && NOT suppress_text)
        {
        tc_free ((VOIDPTR)form_display_x);
        tc_free ((VOIDPTR)form_display_y);
        }
    if (NOT wells->suppress_symbols)
        {
        tc_free ((VOIDPTR)dev_x);
        tc_free ((VOIDPTR)dev_y);
        tc_free ((VOIDPTR)trace_x);
        tc_free ((VOIDPTR)trace_y);
        
        tc_free ((VOIDPTR)top_display_x);
        tc_free ((VOIDPTR)top_display_y);
        
        if (wells->symbol_type)
            {
            tc_free ((VOIDPTR)base_display_x);
            tc_free ((VOIDPTR)base_display_y);
            }
        }
    return SUCCESS;
    }
/* END:                                                                             */
