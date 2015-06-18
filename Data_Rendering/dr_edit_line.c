/* DEC/CMS REPLACEMENT HISTORY, Element DR_EDIT_LINE.C*/
/* *12   16-OCT-1990 15:29:36 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*  9B2  12-OCT-1990 16:23:21 GILLESPIE "Merge Ernie Deltas"*/
/* *11    4-OCT-1990 17:54:43 GILLESPIE "(SPR 100) Merge beta deltas"*/
/*  9B1  24-SEP-1990 15:31:49 GILLESPIE "Beta Deltas"*/
/* *10   14-AUG-1990 16:27:32 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTE"*/
/* *9    18-JUL-1990 07:43:16 GILLESPIE "(SPR 1) Change counters to UINT"*/
/* *8    22-JAN-1990 11:34:43 ADEY "(SPR -1) fix keymap_first for use_keymap"*/
/* *7    22-DEC-1989 14:20:49 GORDON "(SPR 0) go_copy_stroke, not go_copy_table"*/
/* *6    21-DEC-1989 15:40:34 GORDON "(SPR 0) Add some error checking, use new nlist nomenclature"*/
/* *5    18-OCT-1989 10:09:42 JULIAN "(SPR 0) complete prev mod"*/
/* *4    18-OCT-1989 10:04:43 JULIAN "(SPR 0) mod param passing on nl_inq_stroke_user_struct"*/
/* *3    19-SEP-1989 13:36:27 GILLESPIE "(SPR 100) GULF MODS"*/
/* *2    26-JUN-1989 10:40:58 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_EDIT_LINE.C*/
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
/*            Exploration Systems, Inc.                                             */
/*            579 Market Street                                                     */
/*            San Francisco, CA  USA 94105                                          */
/*            (415) 974-1600                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */


/******************************************************************************     */
/*                                                                                  */
/*NAME:           dr_edit_line                                                      */
/*                                                                                  */
/*DESCRIPTION:    Edit a line                                                       */
/*                                                                                  */
/*                                                                                  */
/*     Input:     dr_data - The standard data structure                             */
/*                                                                                  */
/*     Output:    dr_data - updated                                                 */
/*                                                                                  */
/*     Status:    SUCCESS                                                           */
/*                                                                                  */
/******************************************************************************     */

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_gks.h"
#include    "esi_le_nled.h"
#include    "esi_mm.h"
#include    "esi_tc.h"
#include    "esi_lu.h"
#include    "esi_wm.h"

#if USE_PROTOTYPES
publicdef INT dr_edit_line (DR_DATA *dr_data)
#else
publicdef INT dr_edit_line (dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status;
    FLOAT x, y;
    UINT Col_List[2];
    VOIDPTR Var_List[2];
    UINT ndim = 2;
    LE_PARLIST le_struct;
    GO_WORKSPACE *w, **global_w;
    UINT table, ntables, nrows;
    INT i;
    size_t nbytes;
    GO_USER_STRUCT user_struct;
    COLOR_INDEX old_color;
    BOOL clip_flag = TRUE;
    static COLOR_INDEX erase_color = (COLOR_INDEX)-1;
    static COLOR_INDEX highlight_color = (COLOR_INDEX)-1;
    
    if (erase_color < 0)
        {
        /* ----------------------------------                                       */
        /* Remember the erase color for speed                                       */
        /* ----------------------------------                                       */
        lu_text_to_index ("COLOR", (INDEX *)&erase_color, "BACKGROUND");
        lu_text_to_index ("COLOR", (INDEX *)&highlight_color, "HIGHLIGHT");
        }
    /* ----------------------------                                                 */
    /* Caller is required to set                                                    */
    /* current line pointer to tell                                                 */
    /* editor which line to work on                                                 */
    /* ----------------------------                                                 */
    
    if (dr_data->current_line == (LINE *)0)
        {
        am_message (AM_STATUS, "Please Pick a Line to Edit");
        return FAIL;
        }
    /* -------------------------------------                                        */
    /*  Turn off the old segment, if present                                        */
    /*  by drawing over the old line in                                             */
    /*  background color                                                            */
    /* -------------------------------------                                        */
    
    if (dr_data->current_line_seg != 0)
        {
        wmsvis (dr_data->current_line_seg, GINVIS);
        old_color = dr_data->current_line->line.color;
        dr_data->current_line->line.color = erase_color;
        dr_data->current_line->polygon_fill_color = erase_color;
        
        mm_draw_line (dr_data->current_line, clip_flag);
        
        dr_data->current_line->line.color = old_color;
        dr_data->current_line->polygon_fill_color = old_color;
        }
    tc_zeroes (&le_struct, sizeof(LE_PARLIST));
    
    strcpy (le_struct.title, "Line Editor");
    le_struct.nlist = dr_data->current_line->line_nlist;
    le_struct.line = dr_data->current_line->line;
    le_struct.erase_color = erase_color;
    le_struct.hilight_color = highlight_color;
    le_struct.initkeymap = TRUE;
    le_struct.use_keymap = TRUE;
    le_struct.key_swap = TRUE;
    le_struct.only_app_mac_dialog = FALSE;
    
    status = le_nlist_editor (&le_struct);
    
    /* ---------------------------------                                            */
    /* Line editor status is FAIL                                                   */
    /* if user cancelled.                                                           */
    /* But, if didn't cancel, still                                                 */
    /* could be an empty nlist, so check                                            */
    /* it before drawing proceding                                                  */
    /* ---------------------------------                                            */
    
    if (status == SUCCESS)
        {
        nl_inq_nlist_int (le_struct.nlist, NL_NLIST_NROWS, (INT *)&nrows);
        status = nrows >= 2 ? SUCCESS : FAIL;
        if (status != SUCCESS)
            {
            nl_free_nlist (le_struct.nlist);
            }
        }
    if (status == SUCCESS)
        {
        /* -----------------------------                                            */
        /* Free the old line nlist and                                              */
        /* set current line to new nlist                                            */
        /* -----------------------------                                            */
        dr_data->object_changed = TRUE;
        nl_free_nlist (dr_data->current_line->line_nlist);
        dr_data->current_line->line_nlist = le_struct.nlist;
        
        /* ------------------------------------                                     */
        /* Update the object extents to include                                     */
        /* this new line                                                            */
        /* ------------------------------------                                     */
        Col_List[0] = 1, Var_List[0] = (VOIDPTR)&x;
        Col_List[1] = 2, Var_List[1] = (VOIDPTR)&y;
        
        nl_inq_nlist_info (le_struct.nlist, NL_NLIST_MIN, ndim, Col_List, Var_List);
        if (dr_data->lower_left_extent.x < x)
            dr_data->lower_left_extent.x = x;
        if (dr_data->lower_left_extent.y < y)
            dr_data->lower_left_extent.y = y;
        
        nl_inq_nlist_info (le_struct.nlist, NL_NLIST_MAX, ndim, Col_List, Var_List);
        if (dr_data->upper_right_extent.x < x)
            dr_data->upper_right_extent.x = x;
        if (dr_data->upper_right_extent.y < y)
            dr_data->upper_right_extent.y = y;
        
        /* ---------------------------------                                        */
        /* Repaint the line in a new segment                                        */
        /* If the line already existed on                                           */
        /* the display, a refresh will occur                                        */
        /* at this point due to the pending                                         */
        /* invisibility setting on the old                                          */
        /* segment.                                                                 */
        /* ---------------------------------                                        */
        dr_redraw_line (dr_data);
        
        /* -------------------------------                                          */
        /* update static nlist for picking                                          */
        /* -------------------------------                                          */
        status = am_open_workspace ("GO_STATIC_SPACE", AM_GLOBAL, (VOIDPTR)&global_w);
        w = *global_w;
        
        /* --------------------------------                                         */
        /* delete all tables associated                                             */
        /* with the current line and create                                         */
        /* as many tables as the edited                                             */
        /* line has                                                                 */
        /* --------------------------------                                         */
        
        status = nl_inq_nlist_int (w->temp_nlist, NL_NLIST_NTABLES, (INT *)&ntables);
        for (i = ntables; i > 0; i--)
            {
            status = nl_inq_table_user_struct (w->temp_nlist, i,
                                               sizeof(GO_USER_STRUCT),
                                               (VOIDPTR)&user_struct,
                                               (size_t *)&nbytes);
            if (user_struct.elem_id == dr_data->current_line->line_id)
                status = nl_delete_table (w->temp_nlist, i);
            }
        status = nl_inq_nlist_int (le_struct.nlist, NL_NLIST_NTABLES, (INT *)&ntables);
        for (i = 1; i <= ntables; i++)
            {
            status = go_copy_stroke (dr_data->current_line->line_nlist, i,
                                     w->temp_nlist, &table);
            user_struct.elem_id = dr_data->current_line->line_id;
            user_struct.segment_id = dr_data->current_line_seg;
            user_struct.if_text = FALSE;
            user_struct.obj_id = dr_data->current_object_key;
            user_struct.group_id = dr_data->current_object;
            status = nl_set_table_user_struct (w->temp_nlist, table, &user_struct,
                                               sizeof(GO_USER_STRUCT));
            }
        }
    else
        {
        /* -----------------------------------------------------------              */
        /* User cancelled, so make the original line segment visible.               */
        /* Redraw the line in current color, so user can see it until               */
        /* a subsequent screen refresh puts back the segment                        */
        /* -----------------------------------------------------------              */
        
        if (dr_data->current_line_seg != 0)
            {
            wmsvis (dr_data->current_line_seg, GVISI);
            mm_draw_line (dr_data->current_line, clip_flag);
            }
        }
    return SUCCESS;
    }
