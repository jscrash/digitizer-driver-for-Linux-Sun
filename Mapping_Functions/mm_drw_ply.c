/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_PLY.C*/
/* *9    25-SEP-1990 09:27:08 GILLESPIE "(SPR 1) Merge beta"*/
/*  7B1  24-SEP-1990 17:09:32 GILLESPIE "Beta Deltas"*/
/* *8    14-AUG-1990 12:43:38 GILLESPIE "(SPR 5284) Re-enable window clip"*/
/* *7    29-APR-1990 11:15:35 SCD "(SPR 1) Temporarily disable nlm_window_clip_poly until JGG fixes bug."*/
/* *6    14-FEB-1990 13:18:06 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *5    21-DEC-1989 15:43:09 GORDON "(SPR 0) Would you believe, status checking on nlist calls?"*/
/* *4     9-AUG-1989 13:10:37 GORDON "(SPR 104) Move value_list initialization inside loop"*/
/* *3     4-AUG-1989 15:58:00 GORDON "(SPR 103) fix overlapping use of variable j (use k)"*/
/* *2    28-JUL-1989 10:35:23 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:08:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MM_DRW_PLY.C*/

/******************************************************************************/
/*  MAP MANAGER								      */
/******************************************************************************/

#include "esi_mm.h"

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#include "esi_nl_meth_hdr.h"

/******************************************************************************

NAME:           mm_draw_polygon

DESCRIPTION:    Draw and optionally, index a polygon object


     Input:     pPolygon - A pointer to a polygon structure
                index_flag - TRUE -> index, FALSE -> don't index
                clip_flag - TRUE -> clip, FALSE -> don't clip
                gi_obj_type - Object type, used by index routine

     Output:    None

     Status:    SUCCESS
                MM_INVALID_LINE
                MM_MAP_NOT_INIT

******************************************************************************/

#if USE_PROTOTYPES
publicdef INT mm_draw_polygon(LINE *pPolygon, BOOL clip_flag)
#else
publicdef INT mm_draw_polygon(pPolygon, clip_flag)
LINE *pPolygon;
BOOL clip_flag;
#endif
    {
    INT status;
    NLM_CLIP_HEADER clip;
    MAP_STRUCTURE *map;
    UINT Col_List[2];
    VOIDPTR Var_List[2];
    INT units[2];
    size_t data_sizes[2];
    INT data_types[2];
    UINT nrows, pts_out, non_null_rows;
    UINT ntables;
    FLOAT null_value;
    register INT i, j, k;
    GKS_REAL *array_x;
    GKS_REAL *array_y;
    
    /*  Set the graphic attributes                                                  */
    /*  Line style                                                                  */
    
    wm_gsln(pPolygon->line.style);
    
    /*  Line width                                                                  */
    
    wm_gslwsc(pPolygon->line.width);
    
    /*  Line color                                                                  */
    
    wm_gsplci(pPolygon->line.color);
    
    /*   Fill Style                                                                 */
    
    wm_gsfais(pPolygon->polygon_fill_style);
    
    /*   Fill Color                                                                 */
    
    wm_gsfaci(pPolygon->polygon_fill_color);
    
    /*  Clip the polygon, if required                                               */
    
    Col_List[0] = 1, Col_List[1] = 2;
    units[0] = units[1] = 0;
    data_types[0] = data_types[1] = FLOAT_TYPE;
    data_sizes[0] = data_sizes[1] = sizeof(GKS_REAL);
    
    if (clip_flag)
        {
        /*  Get the map limits                                                      */
        
        mp_get_current_map(&map);
        clip.Col_List[0] = 1;
        clip.Col_List[1] = 2;
        clip.minList[0] = map->lower_left_xy.x;
        clip.minList[1] = map->lower_left_xy.y;
        clip.maxList[0] = map->upper_right_xy.x;
        clip.maxList[1] = map->upper_right_xy.y;
        
        /*   Clip                                                                   */
        nl_set_nlist_method(pPolygon->line_nlist, NL_CLIP_METHOD,
                            "NLM_WINDOW_CLIP_POLY", nlm_window_clip_poly);
        
        status = nl_invoke_nlist_method(pPolygon->line_nlist, NL_CLIP_METHOD, &clip);
        if (status != SUCCESS)
            return status;
        
        /*   Draw                                                                   */
        
        status = nl_inq_nlist_int(clip.outNlist, NL_NLIST_NROWS, (INT *) & nrows);
        if (status < SUCCESS)
            return status;
        status = nl_inq_nlist_int(clip.outNlist, NL_NLIST_NTABLES, (INT *) & ntables);
        if (status < SUCCESS)
            return status;
        array_x = (GKS_REAL *)tc_alloc(nrows * sizeof(GKS_REAL));
        array_y = (GKS_REAL *)tc_alloc(nrows * sizeof(GKS_REAL));
        
        for (k = 1; k <= ntables; k++)
            {
            Var_List[0] = (VOIDPTR)array_x;
            Var_List[1] = (VOIDPTR)array_y;
            nl_set_current_row(clip.outNlist, 1, k);
            nl_inq_table_int(clip.outNlist, k, NL_TABLE_NROWS, (INT *) & nrows);
            
            status = nl_get_rows_convert(clip.outNlist, nrows, 2, Col_List, Var_List,
                                         units, data_sizes, data_types, &pts_out);
            
            /*   Toss out any null values                                           */
            
            j = 0;
            Var_List[0] = (VOIDPTR) & non_null_rows;
            nl_inq_nlist_info(clip.outNlist, NL_NLIST_NON_NULL, 1, Col_List, Var_List);
            
            /* When nl_inq_nlist_info is coded to handle NL_NLIST_NON_NULL key, this
               code will make sense.  Right now, it just returns 0, so clipping is
               assumed to always have occurred.  We can speed things up by fixing
               nl_inq_nlist_info.  8/4/89 GS */
            
            if (non_null_rows < nrows)  /*  Clipping has occurred                   */
                {
                Var_List[0] = (VOIDPTR) & null_value;
                nl_inq_nlist_info(clip.outNlist, NL_NLIST_NULL_VALUE, 1, Col_List,
                                  Var_List);
                for (i = 0; i < pts_out; i++)
                    {
                    if (array_x[i] != null_value)
                        {
                        array_x[j] = array_x[i];
                        array_y[j] = array_y[i];
                        j++;
                        }
                    }
                nrows = j;
                }
            if (nrows > 2)
                {
                wm_gfa(nrows, array_x, array_y);
                }
            else if (nrows EQUALS 2)
                {
                wm_gpl(nrows, array_x, array_y);
                }
            }
        /*  free up memory (Added 4 May 88 JGG)                                     */
        tc_free(array_x);
        tc_free(array_y);
        nl_free_nlist(clip.outNlist);
        
        }
    else
        {
        
        /*   Draw un-clipped                                                        */
        status = nl_inq_nlist_int(pPolygon->line_nlist, NL_NLIST_NROWS,
                                  (INT *) & nrows);
        if (status < SUCCESS)
            return status;
        status = nl_inq_nlist_int(pPolygon->line_nlist, NL_NLIST_NTABLES,
                                  (INT *) & ntables);
        if (status < SUCCESS)
            return status;
        array_x = (GKS_REAL *)tc_alloc(nrows * sizeof(GKS_REAL));
        array_y = (GKS_REAL *)tc_alloc(nrows * sizeof(GKS_REAL));
        Var_List[0] = (VOIDPTR)array_x;
        Var_List[1] = (VOIDPTR)array_y;
        for (j = 0; j < ntables; j++)
            {
            nl_set_current_row(pPolygon->line_nlist, 1, j + 1);
            nl_inq_table_int(pPolygon->line_nlist, j + 1, NL_TABLE_NROWS,
                             (INT *) & nrows);
            nl_get_rows_convert(pPolygon->line_nlist, nrows, 2, Col_List, Var_List,
                                units, data_sizes, data_types, &pts_out);
            if (pts_out > 2)
                {
                wm_gfa(pts_out, array_x, array_y);
                }
            else if (pts_out EQUALS 2)
                {
                wm_gpl(pts_out, array_x, array_y);
                }
            }
        /*  free up memory (Added 4 May 88 JGG)                                     */
        tc_free(array_x);
        tc_free(array_y);
        }
    return SUCCESS;
    }
