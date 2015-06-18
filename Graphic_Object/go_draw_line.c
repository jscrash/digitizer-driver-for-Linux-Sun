/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRAW_LINE.C*/
/* *11   28-SEP-1990 14:27:45 GILLESPIE "(SPR 1) Merge beta deltas"*/
/*  9B1  24-SEP-1990 16:23:06 GILLESPIE "Beta Deltas"*/
/* *10   14-AUG-1990 18:10:46 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *9    19-JUL-1990 18:40:09 GILLESPIE "(SPR 5411) CHanges to support fault traces"*/
/* *8    18-JUL-1990 11:40:47 GILLESPIE "(SPR 5411) Add support for seismic fault traces"*/
/* *7    17-MAR-1990 13:00:18 JULIAN "(SPR 0) undo last mods"*/
/* *6    16-MAR-1990 11:13:42 JULIAN "(SPR 5135) create nlz_strip_points"*/
/* *5    28-FEB-1990 14:33:13 GILLESPIE "(SPR 6011) Close n-List file (since it has been opened exclusive)"*/
/* *4    14-FEB-1990 13:17:29 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *3     9-NOV-1989 13:28:44 GORDON "(SPR 5058) Use nlist methods for resampling."*/
/* *2    27-JUL-1989 15:26:15 GILLESPIE "(SPR 2) Fix minor casting problems"*/
/* *1    19-JUN-1989 12:53:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GO_DRAW_LINE.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*                                                                                  */
/* * FILE: GO_DRAW_LINE.C                                                           */
/*                                                                                  */

/* include files                                                                    */
#include "esi_c_lib.h"
#include "esi_mp.h"

#ifndef ESI_GO_H
#include "esi_go.h"
#endif

#ifndef ESI_OV_H
#include "esi_ov.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_DR_H
#include "esi_dr.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#include "esi_nl_meth_hdr.h"
#include "esi_nl_methods.h"

/******************************************************************************     */
/*                                                                                  */
/*NAME:           go_draw_line                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*Fetch a single polyline or polygon from the database and draw. The line is        */
/*specified by it's ID in the LINES table.  The line is fetched via the CT          */
/*pipeline, and if clipping is specified, it is clipped to the current map window.  */
/*                                                                                  */
/*Unless suppress_editing is specified, the line will be placed into a segment of   */
/*class WM_CLASS_LINE  or WM_CLASS_POLYGON, with database key set to the line's     */
/*name/zvalue, and user name set to the ID of the line.                             */
/*                                                                                  */
/*If write_scatter_file support is specified, the optionally clipped line is        */
/*resampled by bi-linear interpolation to the specified maximum_data_interval, and  */
/*the resulting xzy values are written to the specified scatter file: scatter.      */
/*                                                                                  */
/*    Input:  values          GO_STRUCT pointer                                     */
/*        element_id          ID in LINES table of polyline/gon                     */
/*        object_type         E.g., "Rivers"                                        */
/*        clip_required       TRUE if there is possibility line s/b clipped         */
/*                                                                                  */
/*    Output: line is drawn, scatter file is optionally updated.                    */
/*                                                                                  */
/*    Status: SUCCESS, or                                                           */
/*        status as returned from subordinate routines,                             */
/*            ct_get_nlist, nl_window_clip_nlist, nl_free_nlist,                    */
/*            nl_resample_nlist, etc.                                               */
/******************************************************************************     */

#if USE_PROTOTYPES
publicdef INT go_draw_line (GO_STRUCT *values, INT element_id, CHAR *object_type,
                            BOOL clip_required, INT *segment_id)
#else
publicdef INT go_draw_line (values, element_id, object_type, clip_required, segment_id)
GO_STRUCT *values;
INT element_id;
CHAR *object_type;
BOOL clip_required;                 /*  This parameter is modified herein -- do not */
                                    /*change to pointer without protecting caller   */
                                    /*from modification                             */
INT *segment_id;
#endif 
    {
    FILENAME nlist_file;
    INT status;
    LINE line;
    FLOAT x, y, z_array[1], x_array[1], y_array[1];
    MAP_STRUCTURE *map;
    
    /*  nlist variables                                                             */
    INT flag;
    UINT nrows;
    UINT ncol = 2;
    UINT Col_List[2];
    BOOL fault_type;
    VOIDPTR Var_List[2];
    NLIST_HEADER scratch_line;
    NLM_CLIP_HEADER clip;
    NLM_RESAMPLE_HEADER parms;
    
    /********************************************************************************/
    
    Col_List[0] = 1, Col_List[1] = 2;
    
    line.line_nlist = (NLIST_HEADER)0;
    nlist_file[0] = NUL;
    
    fault_type = (strcmp (object_type, FAULTS_TYPE) == 0);
    
    if (!fault_type)
        {
        ho_add_path ("ESI$CULTURE", object_type, sizeof nlist_file, nlist_file);
        }
    status = dr_get_line (&line, element_id, nlist_file, fault_type, values->marker);
    
    if (status EQUALS SUCCESS)
        {
        
        /* * CLIPPING BLOCK *                                                       */
        /* ------------------                                                       */
        
        if (clip_required)
            {
            /* GET THE MAP LIMITS                                                   */
            
            mp_get_current_map (&map);
            clip.Col_List[0] = 1;
            clip.Col_List[1] = 2;
            clip.minList[0] = map->lower_left_xy.x;
            clip.minList[1] = map->lower_left_xy.y;
            clip.maxList[0] = map->upper_right_xy.x;
            clip.maxList[1] = map->upper_right_xy.y;
            
            /* CLIP                                                                 */
            
            status = nl_invoke_nlist_method (line.line_nlist, NL_CLIP_METHOD, &clip);
            
            /* FREE THE ORIGINAL NLIST                                              */
            
            if (status EQUALS SUCCESS)
                {
                status = nl_free_nlist (line.line_nlist);
                line.line_nlist = clip.outNlist;
                }
            else
                {
                goto finished;
                }
            }
        /* * SCATTER FILE BLOCK *                                                   */
        /* ----------------------                                                   */
        
        if (values->scatter != (NLIST_HEADER)0)
            {
            /* RESAMPLE THE CLIPPED LINE                                            */
            
            status = nl_set_nlist_method (line.line_nlist, NL_RESAMPLE_METHOD,
                                          "NLM_BILINEAR_NLIST", nlm_bilinear_nlist);
            parms.ncol = ncol;
            parms.Col_List = Col_List;
            parms.resampleInc = values->maximum_data_interval;
            parms.filterLen = 0;    /* This will be ignored anyway.                 */
            
            status = nl_invoke_nlist_method (line.line_nlist, NL_RESAMPLE_METHOD,
                                             &parms);
            if (status != SUCCESS)
                {
                goto finished;
                }
            scratch_line = parms.outNlist;  /* Get the resampled nlist              */
            
            /* WRITE EACH X,Y,Z TO THE SCATTER FILE                                 */
            
            z_array[0] = atof (line.line_name);
            Var_List[0] = (VOIDPTR)&x_array[0];
            Var_List[1] = (VOIDPTR)&y_array[0];
            
            nl_inq_nlist_int (scratch_line, NL_NLIST_NPOINTS, (INT *)&nrows);
            
            /*       TODO:  convert following code to                               */
            /*         extract the x,y as an array, and                             */
            /*         write scatter as an array, instead                           */
            /*         of row by row                                                */
            
            for (flag = nl_first_row (scratch_line, ncol, Col_List, Var_List);
                 flag >= 0 && !(flag& NL_EOL);
                 flag = nl_next_row (scratch_line, ncol, Col_List, Var_List))
                {
                status = ov_write_scatter (values->scatter, x_array, y_array, z_array,
                                           1);
                if (status != SUCCESS)
                    {
                    goto finished;
                    }
                }
            }
        /* * DRAWING BLOCK *                                                        */
        /* -----------------                                                        */
        
        if (values->line.color >= 0)
            {
            line.line.color = values->line.color;
            line.polygon_fill_color = values->line.color;
            }
        if (values->line.style >= 0)
            {
            line.line.style = values->line.style;
            }
        if (values->line.width >= 0)
            {
            line.line.width = values->line.width;
            }
        if (NOT values->suppress_editing)
            {
            ov_new_element (segment_id,
                            (line.polygon_flag ? WM_CLASS_POLYGON : WM_CLASS_LINE),
                            line.line_name, element_id);
            }
        mm_draw_line (&line, FALSE);
        
        if (NOT values->suppress_editing)
            {
            wmsclose (*segment_id);
            }
        }
finished:
    
    nl_close_nlist_file (nlist_file);
    
    if (line.line_nlist != (NLIST_HEADER)0)
        {
        nl_free_nlist (line.line_nlist);
        }
    return(status);
    }
