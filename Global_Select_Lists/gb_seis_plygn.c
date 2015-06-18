/* DEC/CMS REPLACEMENT HISTORY, Element GB_SEIS_PLYGN.C */
/* *10    7-OCT-1991 15:20:35 JULIAN "(SPR 4156) added check to select distinct lines" */
/* *9    30-SEP-1991 17:57:45 JULIAN "(SPR 4136) added trans points after picking seismic line(s)" */
/* *8    14-AUG-1990 13:27:18 VINCE "(SPR 5644) Header Standardization" */
/* *7    22-MAY-1990 14:17:00 JULIAN "(SPR 5345) add checks on no seismic condition" */
/* *6     3-MAY-1990 09:35:13 JULIAN "(SPR 5276) fix return status when > 0" */
/* *5     1-APR-1990 10:25:39 GILLESPIE "(SPR 1) Use mp_front_map instead of mp_get_current_map" */
/* *4    12-FEB-1990 10:19:22 GILLESPIE "(SPR 6008) Use ag_side instead of jg_side" */
/* *3    30-OCT-1989 14:34:57 JULIAN "(SPR 5039) Seismic Select List Mods" */
/* *2    14-SEP-1989 18:18:07 GORDON "(SPR -1) checkin from gulf" */
/* *1    14-SEP-1989 17:33:15 GORDON "a" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_SEIS_PLYGN.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_gb.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_SO_H

#include "esi_so.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_AG_H

#include "esi_ag.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"
#include "esi_tc_err.h"

#endif

#ifndef ESI_HT_H

#include "esi_ht.h"

#endif

#ifndef ESI_HT_ERR_H

#include "esi_ht_err.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Gb_seis_polygon manages the user interaction to select a
    subset of the displayed/visible seismic lines on the current
    map by banding.

    The returned select list will have one row for each segment of
    a seismic line which is inside the user's polygon.  In particular,
    a line which crosses in and out of the polygon will have multiple
    rows in the select list, each containing the corresponding 
    first and last cdps which are interior to the polygon.

    The current implementation does NOT interpolate to the cdp
    which lies nearest to the interior of the polygon -- rather it
    returns the nearest cdp existing in the SHOT_XY nlist.

    INITIAL CONDITIONS:

        1.  a map must be displayed with at least one seismic overlay

Prototype:
    publicdef INT gb_seis_polygon (NLIST_HEADER *select_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) select_list -(NLIST_HEADER *) Pointer to created select list:
                        LINENAME | 1ST_CDP | LAST_CDP |SURVEY

Return Value/Status:
    SUCCESS - If valid polygon returned.
    CANCEL - If user canceled interaction.
    FAIL - If trouble...(status codes to be supplied).
    TC_ERR_NO_MEMORY
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_seis_polygon (NLIST_HEADER *select_list)
#else
publicdef INT gb_seis_polygon (select_list)
NLIST_HEADER *select_list;
#endif
    {
    INT status = SUCCESS;
    INT poly_points = 0;            /* num points in user's polygon */
    DOUBLE poly_xmin, poly_ymin;    /* extents of user's polygon    */
    DOUBLE poly_xmax, poly_ymax;
    DOUBLE *poly_x, *poly_y;        /* user's polygon in array form */
    FLOAT line_xmin, line_ymin;     /* extents of current seismic line */
    FLOAT line_xmax, line_ymax;
    
    NLIST_HEADER list_of_lines;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    UINT ndim;
    UINT cur_stroke = 0, cur_point = 0, nbytes = 0, returned_points = 0;
    INT first_cdp = 0, last_cdp = 0;
    INT stroke_num;
    CHAR pick_prompt[STMAX];
    
    UINT p = 0;
    INT num_cdps = 0;
    INT *cdp_locs = (INT *)0, prev_cdp, cur_cdp;
    INT inside = FALSE;
    FLOAT *x_locs = (FLOAT *)0, *y_locs = (FLOAT *)0;
    DOUBLE cur_x, cur_y;
    MAP_STRUCTURE *mapdef;
    SO_STRUCT *pso;
    SX_USER_STRUCT user_struct;
    INT i;
    HASH_TABLE *line_list=(HASH_TABLE *)0;     /* (case 4156) */

    strcpy (pick_prompt, "Draw a Polygon to Select Seismic Lines.");
    
    /*------------------------------------------------------*/
    /* return NULL select_list pointer if no lines selected */
    /*------------------------------------------------------*/
    
    *select_list = (NLIST_HEADER)0;
    
    /*----------------------------------------------------------------------*/
    /* get the map structure that holds the seismic overlay object pointer. */
    /* and the pso pointer that has the seismic overlay context...          */
    /*----------------------------------------------------------------------*/
    
    mp_front_map (&mapdef);
    if (mapdef EQUALS (MAP_STRUCTURE *)0)
        {
        return FAIL;
        }
    pso = (SO_STRUCT *)mapdef->pso;
    
    if (pso EQUALS (SO_STRUCT *)0)
        {
        am_message (AM_ALERT, "No seismic in displayed map");
        return SUCCESS;
        }
    /*------------------------------------------------------*/
    /* position window manager context to front map window  */
    /* and select the map transformation                    */
    /*------------------------------------------------------*/
    
    status = gbz_init_map_window ();
    if (status == SUCCESS)
        {
        am_message (AM_DIALOG, pick_prompt);
        
        /*------------------------------------------*/
        /* allow user to sketch polygon, returning  */
        /* a polygon with explicit closure.         */
        /* status is FAIL if user canceled, or      */
        /* digitized less than 3 points             */
        /*------------------------------------------*/
        if (ug_if_gpx ())
            wm_set_mouse_menu ((CHAR **)0, (INT *)0);
        status = gb_draw_polygon_array (&poly_points, &poly_xmin, &poly_ymin,
                                        &poly_xmax, &poly_ymax, &poly_x, &poly_y);
           /* transform points to PROJECT PROJECTION (case 4136) */
	ct_project_xy(poly_xmin,poly_ymin,&poly_xmin,&poly_ymin);
	ct_project_xy(poly_xmax,poly_ymax,&poly_xmax,&poly_ymax);
	for(i = 0; i < poly_points; i++)
	  ct_project_xy(poly_x[i],poly_y[i],&poly_x[i],&poly_y[i]);

        if ((status == SUCCESS))
            {
            /*------------------------------------------------------*/
            /* build list_of_lines containing one stroke for each   */
            /* CDP|X|Y location nlist of currently visible seismic  */
            /* lines.                                               */
            /*------------------------------------------------------*/
            
            status = so_build_common_nlist (pso, &list_of_lines);
            if (status == SUCCESS)
                {
                /*--------------------------------------------------*/
                /* create the returned select list of lines clipped */
                /* to interior of user's polygon:                   */
                /* LINE | 1stCDP | lastCDP | SURVEY                 */
                /*--------------------------------------------------*/
                
                sl_init_select_list (SEISMIC_DATA_TYPE, select_list);
                nl_start_table (*select_list, (UINT *) & stroke_num);
                
		/* (case 4156) add hash table to check the distinctive of 
		   selected lines */
		ht_init_hash_table(100, 10, HT_INT_KEY, sizeof(INT), 0,
				   HT_APPLICATION_TABLE, &line_list);


                /*--------------------------------------------------*/
                /* sequence thru each stroke (corresponding to a    */
                /* seismic location nlist). If stroke intersects    */
                /* the extents of user's polygon, then submit each  */
                /* point of stroke to polygon inclusion test.       */
                /*--------------------------------------------------*/
                
                for (cur_stroke = 1;
                     nl_set_current_row (list_of_lines, 1, cur_stroke) >= 0;
                     cur_stroke++)
                    {
                    dim_list[0] = 2;    /* X column */
                    dim_list[1] = 3;    /* Y column */
                    value_list[0] = (VOIDPTR) & line_xmax;
                    value_list[1] = (VOIDPTR) & line_ymax;
                    
                    nl_inq_table_info (list_of_lines, cur_stroke, NL_TABLE_MAX, 2,
                                       dim_list, value_list);
                    /*-------------------------------------------------------------*/
                    /* reject this line if in left or right half-planes of polygon */
                    /*-------------------------------------------------------------*/
                    if (line_xmax < poly_xmin || line_ymax < poly_ymin)
                        continue;
                    
                    value_list[0] = (VOIDPTR) & line_xmin;
                    value_list[1] = (VOIDPTR) & line_ymin;
                    nl_inq_table_info (list_of_lines, cur_stroke, NL_TABLE_MIN, 2,
                                       dim_list, value_list);
                    /*--------------------------------------------------------------*/
                    /* reject this line if in upper or lower half-planes of polygon */
                    /*--------------------------------------------------------------*/
                    if (line_xmin > poly_xmax || line_ymin > poly_ymax)
                        continue;
                    
                    status = nl_inq_table_user_struct (list_of_lines, cur_stroke,
                                                       sizeof(SX_USER_STRUCT),
                                                       (VOIDPTR) & user_struct,
                                                       (size_t *) & nbytes);

                    /*----------------------------------------------*/
                    /* fetch CDP|X|Y arrays of current seismic line */
                    /*----------------------------------------------*/
                    
                    nl_inq_table_int (list_of_lines, cur_stroke, NL_TABLE_NROWS,
                                      &num_cdps);
                    cdp_locs = (INT *)tc_alloc (num_cdps * sizeof(INT));
                    if (cdp_locs EQUALS (INT *)0)
                        {
                        status = TC_ERR_NO_MEMORY;
                        goto free_memory;
                        }
                    x_locs = (FLOAT *)tc_alloc (num_cdps * sizeof(FLOAT));
                    if (x_locs EQUALS (FLOAT *)0)
                        {
                        status = TC_ERR_NO_MEMORY;
                        goto free_memory;
                        }
                    y_locs = (FLOAT *)tc_alloc (num_cdps * sizeof(FLOAT));
                    if (y_locs EQUALS (FLOAT *)0)
                        {
                        status = TC_ERR_NO_MEMORY;
                        goto free_memory;
                        }
                    ndim = 3;
                    dim_list[0] = 1;
                    dim_list[1] = 2;
                    dim_list[2] = 3;
                    value_list[0] = (VOIDPTR)cdp_locs;
                    value_list[1] = (VOIDPTR)x_locs;
                    value_list[2] = (VOIDPTR)y_locs;
                    status = nl_get_rows (list_of_lines, num_cdps, ndim, dim_list,
                                          value_list, &returned_points);
                    if (status < 0)
                        {
                        goto free_memory;
                        }
                    /*----------------------------------*/
                    /* set nlist context for adding     */
                    /* included lines to select_list    */
                    /*----------------------------------*/
                    ndim = 3;
                    dim_list[0] = 1;    /* line id    */
                    dim_list[1] = 2;    /* 1st cdp      */
                    dim_list[2] = 3;    /* last cdp     */
                    
                    value_list[0] = (VOIDPTR) & user_struct.line_name.id;
                    value_list[1] = (VOIDPTR) & first_cdp;
                    value_list[2] = (VOIDPTR) & last_cdp;
                    
                    /*------------------------------------------------------*/
                    /* append a new row of LINEID | 1stCDP | lastCDP        */
                    /* whenever seismic line:                               */
                    /*      - crosses inside to outside, or                 */
                    /*      - last cdp is inside                            */
                    /* initialize a new row whenever:                       */
                    /*      - crosses outside to inside, or                 */
                    /*      - first cdp is inside                           */
                    /*------------------------------------------------------*/
                    
                    inside = ag_inside (x_locs[0], y_locs[0], poly_x, poly_y, 0,
                                        poly_points);
                    prev_cdp = cdp_locs[0];
                    if (inside)
                        {
                        first_cdp = cdp_locs[0];
                        }
                    for (p = 1; p < num_cdps; prev_cdp = cur_cdp, p++)
                        {
                        cur_cdp = cdp_locs[p], cur_x = x_locs[p], cur_y = y_locs[p];
                        if (cur_x >= poly_xmin && cur_x <= poly_xmax &&
                            cur_y >= poly_ymin && cur_y <= poly_ymax &&
                            ag_inside (cur_x, cur_y, poly_x, poly_y, 0, poly_points))
                            {
                            if (!inside)
                                {
                                /*--------------------------*/
                                /* crossing in, init row    */
                                /*--------------------------*/
                                first_cdp = cur_cdp;
                                }
                            inside = TRUE;
                            }
                        else
                            {
                            if (inside)
                                {
				  /* (case 4156) add check of the distinctive 
				     of selected lines */
				  if((status = 
				      ht_insert(line_list,
						(VOIDPTR)&user_struct.line_name.id,
						0)) EQUALS HT_DUPLICATE_KEY) 
				    continue;

                                /*--------------------------*/
                                /* crossing out, append row */
                                /*--------------------------*/
                                last_cdp = prev_cdp;
                                status = nl_add_rows (*select_list, 1, ndim, dim_list,
                                                      value_list);
                                if (status < 0)
                                    {
                                    goto free_memory;
                                    }
                                }
                            inside = FALSE;
                            }
                        }
                    /*------------------------------*/
                    /* if last cdp was inside       */
                    /* add this segment of current  */
                    /* line to returned select list */
                    /*------------------------------*/
                    if (inside)
                        {
                        last_cdp = prev_cdp;
			/* (case 4156) add check of the distinctive of 
			   selected lines */
			if((status = 
			    ht_insert(line_list,
				      (VOIDPTR)&user_struct.line_name.id,
				      0)) EQUALS HT_DUPLICATE_KEY) 
			  continue;

                        status = nl_add_rows (*select_list, 1, ndim, dim_list,
                                              value_list);
                        if (status < 0)
                            {
                            goto free_memory;
                            }
                        }
                    /*-----------------------------------*/
                    /* free the temporary CDP|X|Y arrays */
                    /*-----------------------------------*/
                    
                    tc_free (cdp_locs);
                    cdp_locs = (INT *)0;
                    tc_free (x_locs);
                    x_locs = (FLOAT *)0;
                    tc_free (y_locs);
                    y_locs = (FLOAT *)0;
                    
                    }
                /* for each line on map */
                
                nl_free_nlist (list_of_lines);
                }
            /* list_of_lines built */
            }
        /* user completed polygon */
        gbz_term_map_window ();
        }
    /* zzinit successful      */
    am_message (AM_DIALOG, "");
    
free_memory:
    tc_free (poly_x);
    tc_free (poly_y);
    tc_free (cdp_locs);
    tc_free (x_locs);
    tc_free (y_locs);
    ht_free_table (&line_list);
    if (status < 0)
        return status;
    else
        return SUCCESS;
    }
/* END:     */
