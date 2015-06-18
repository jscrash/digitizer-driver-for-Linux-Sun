/* DEC/CMS REPLACEMENT HISTORY, Element GR_DRW_GRATG.C*/
/* *14   12-JUN-1991 08:04:49 CHARLIE "(SPR -1) change algorithm for 'precision' if 'degrees'"*/
/* *13    5-DEC-1990 10:31:43 JESSIE "(SPR 6233) accept xtic/ytic with DEGREES"*/
/* *12   13-NOV-1990 10:47:06 JESSIE "(SPR 0) add DMS input"*/
/* *11   14-AUG-1990 13:28:56 VINCE "(SPR 5644) Header Standardization"*/
/* *10   30-MAY-1990 08:50:52 JULIAN "(SPR 5350) fix graticule marks inconsistent"*/
/* *9    19-APR-1990 12:00:40 WALTERS "(SPR 0) Correct seconds labeling"*/
/* *8    18-APR-1990 17:19:18 WALTERS "(SPR 5185) Fix memory allocation problems causing memory bashing"*/
/* *7     7-MAR-1990 17:53:36 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *6    26-FEB-1990 13:43:02 WALTERS "(SPR 0) Fix rounding on longitudinal labels"*/
/* *5    14-FEB-1990 13:17:35 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *4    17-JAN-1990 15:31:32 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3    10-OCT-1989 09:54:32 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2    18-JUL-1989 15:28:52 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:53:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GR_DRW_GRATG.C*/
/*****************************************************************************/
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

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GR_H

#include "esi_gr.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#include "esi_nl_meth_hdr.h"

#if USE_PROTOTYPES
static INT grz_make_label (DOUBLE value, INT precision, CHAR *label, BOOL latitude,
                               BOOL degrees, DOUBLE full_circ, DOUBLE half_circ);
static INT grz_draw_label (DOUBLE x, DOUBLE y, CHAR *label, DOUBLE size,
                               BOOL latitude);
static INT grz_draw_tick (DOUBLE x, DOUBLE y, DOUBLE dist,
                              PROJECTION_STRUCTURE *map, PROJECTION_STRUCTURE *geo,
                              BOOL horizontal);
#else
static INT grz_make_label ();
static INT grz_draw_label ();
static INT grz_draw_tick ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gr_draw_graticule_geo(GRAT_OV_STRUCT *grat,UOM_STRUCTURE *uom);

Private_Functions:
    
    static INT grz_make_label(DOUBLE value, INT precision, CHAR *label,
        BOOL latitude, BOOL degrees, DOUBLE full_circ, DOUBLE half_circ);
    
    static INT grz_draw_label(DOUBLE x, DOUBLE y, CHAR *label, DOUBLE size, BOOL latitude);

    static INT grz_draw_tick(DOUBLE x, DOUBLE y, DOUBLE dist,
        PROJECTION_STRUCTURE *map, PROJECTION_STRUCTURE *geo, BOOL horizontal);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT gr_draw_graticule_geo(GRAT_OV_STRUCT *grat,UOM_STRUCTURE *uom);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    grat            -(GRAT_OV_STRUCT *)
    uom             -(UOM_STRUCTURE *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gr_draw_graticule_geo (GRAT_OV_STRUCT *grat, UOM_STRUCTURE *uom)
#else
publicdef INT gr_draw_graticule_geo (grat, uom)
GRAT_OV_STRUCT *grat;
UOM_STRUCTURE *uom;
#endif
    {
    
    INT status;
    MAP_STRUCTURE *map;
    NLIST_HEADER map_lines = (NLIST_HEADER)0;
    NLM_CLIP_HEADER clip;
    DOUBLE *x = (DOUBLE *)0;
    DOUBLE *y = (DOUBLE *)0;
    DOUBLE temp_x;
    DOUBLE temp_y;
    INT max_pts;
    DOUBLE map_point[2];
    DOUBLE geo_point[2];
    DOUBLE ll_geo[2];
    DOUBLE ul_geo[2];
    DOUBLE lr_geo[2];
    DOUBLE ur_geo[2];
    DOUBLE min_lat;
    DOUBLE min_long;
    DOUBLE max_lat;
    DOUBLE max_long;
    DOUBLE range;
    DOUBLE lat_interval = 0.0;
    DOUBLE long_interval = 0.0;
    PROJECTION_STRUCTURE *map_projection;
    PROJECTION_STRUCTURE *geo_projection;
    CHAR label[15];
    INT i, j, jt, jn;
    UINT k, n;
    DOUBLE xrect[5], yrect[5];
    FLOAT xxrect[5], yyrect[5];
    BOOL degrees;
    DOUBLE choice[7];
    DOUBLE test[6];
    DOUBLE full_circle, half_circle;
    UOM_NAME uom1, uom2;
    INT precision;
    
    INT units[2];
    size_t d_size[2];
    INT d_typ[2];
    INT v_type[2];
    UINT d_list[2];
    VOIDPTR v_list[2];
    static CHAR *c_name[3] = 
        {
        "X",
        "Y",
        NULL
        };
    
    /* ********************************************************************** */
    
    /* * INITIALIZE GEODETIC PROJECTION STRUCTURE * */
    /* -------------------------------------------- */
    
    status = ct_get_projection ("GEODETIC", &geo_projection);
    if (status != SUCCESS)
        {
        goto finished;
        }
    /* * GET MAP AND INITIALIZE MAP PROJECTION STRUCTURE * */
    /* --------------------------------------------------- */
    
    status = mp_get_current_map (&map);
    if (status != SUCCESS)
        {
        goto finished;
        }
    status = ct_get_projection (map->map_projection, &map_projection);
    if (status != SUCCESS)
        {
        goto finished;
        }
    /* * GET MAP CORNERS IN GEODETIC * */
    /* ------------------------------- */
    
    /* LOWER LEFT */
    map_point[0] = map->lower_left_xy.x;
    map_point[1] = map->lower_left_xy.y;
    ct_transform_point (map_point[0], map_point[1], map_projection, &ll_geo[0],
                        &ll_geo[1], geo_projection);
    
    /* UPPER LEFT */
    map_point[1] = map->upper_right_xy.y;
    ct_transform_point (map_point[0], map_point[1], map_projection, &ul_geo[0],
                        &ul_geo[1], geo_projection);
    
    /* UPPER RIGHT */
    map_point[0] = map->upper_right_xy.x;
    ct_transform_point (map_point[0], map_point[1], map_projection, &ur_geo[0],
                        &ur_geo[1], geo_projection);
    
    /* LOWER RIGHT */
    map_point[1] = map->lower_left_xy.y;
    ct_transform_point (map_point[0], map_point[1], map_projection, &lr_geo[0],
                        &lr_geo[1], geo_projection);
    
    /* * DETERMINE MIN AND MAX VALUES IN CORRECT SYSTEM * */
    /* -------------------------------------------------- */
    
    min_lat = MIN (MIN (ll_geo[1], lr_geo[1]), MIN (ul_geo[1], ur_geo[1]));
    max_lat = MAX (MAX (ll_geo[1], lr_geo[1]), MAX (ul_geo[1], ur_geo[1]));
    max_long = MAX (MAX (ll_geo[0], lr_geo[0]), MAX (ul_geo[0], ur_geo[0]));
    min_long = MIN (MIN (ll_geo[0], lr_geo[0]), MIN (ul_geo[0], ur_geo[0]));
    
    if (((status = con_dms2dec(grat->x_tic_interval, &temp_x)) == SUCCESS)
     || ((status = con_dms2dec(grat->y_tic_interval, &temp_y)) == SUCCESS))
        {
        degrees = TRUE;
        full_circle = 360.0;
        half_circle = 180.0;
        }
    else if(((status = tu_parse_unit(grat->x_tic_interval,xrect,uom1,uom2)) == SUCCESS)
	 || ((status = tu_parse_unit(grat->y_tic_interval,yrect,uom1,uom2)) == SUCCESS))
        {
	if ((status = strcmp(uom1,"")) == SUCCESS)
	    {
	    degrees = TRUE;
	    full_circle = 360.0;
	    half_circle = 180.0;
	    }
	}
    else if ((grat->y_tic_interval EQUALS NULL)
	  && (grat->x_tic_interval EQUALS NULL)) 	
	{
	degrees = TRUE;
	full_circle = 360.0;
	half_circle = 180.0;
	}
    else	
	{
	degrees = FALSE;
	full_circle = 360.0 / uom->uom_definition;
	half_circle = 180.0 / uom->uom_definition;
        
	min_lat /= uom->uom_definition;
	max_lat /= uom->uom_definition;
	min_long /= uom->uom_definition;
	max_long /= uom->uom_definition;
	}

    /* * DETERMINE TICK INTERVAL * */
    /* --------------------------- */

    if (grat->x_tic_interval != NULL)
        {
        if ((status = con_dms2dec(grat->x_tic_interval,xrect)) != SUCCESS)
	    tu_parse_unit (grat->x_tic_interval, xrect, uom1, uom2);
	else
	    uom1[0] = '\0';
        long_interval = xrect[0];
        tu_unit_conversion (long_interval, uom1, xrect, grat->uom);
        long_interval = xrect[0];
        }
    if (grat->y_tic_interval != NULL)
        {
        if ((status = con_dms2dec(grat->y_tic_interval,yrect)) != SUCCESS)
	     tu_parse_unit (grat->y_tic_interval, yrect, uom1, uom2);
	else
	    uom1[0] = '\0';
        lat_interval = yrect[0];
        tu_unit_conversion (lat_interval, uom1, yrect, grat->uom);
        lat_interval = yrect[0];
        }
    if (long_interval != 0.0)
        {
        if (lat_interval EQUALS 0.0)
            {
            lat_interval = long_interval;
            }
        }
    else if (lat_interval != 0.0)
        {
        long_interval = lat_interval;
        }
    /* CALCULATE DEFAULT */
    
    else
        {
        if (degrees)
            {
            choice[0] = 1.0 / 120.0;
            choice[1] = 1.0 / 30.0;
            choice[2] = 1.0 / 4.0;
            }
        else
            {
            choice[0] = 1.0 / 100.0;
            choice[1] = 5.0 / 100.0;
            choice[2] = 2.0 / 10.0;
            }
        choice[3] = 1.0;
        choice[4] = 2.0;
        choice[5] = 5.0;
        choice[6] = 10.0;
        test[0] = 0.1;
        test[1] = 0.5;
        test[2] = 2.0;
        test[3] = 10.0;
        test[4] = 20.0;
        test[5] = 40.0;
        
        range = MIN ((max_lat - min_lat), (max_long - min_long));
        for (i = 0; i < 6; i++)
            {
            if (range <= test[i])
                {
                break;
                }
            }
        lat_interval = choice[i];
        long_interval = choice[i];
        }
    /* * DETERMINE PRECISION FOR LABELLING * */
    /* ------------------------------------- */
    
    range = MIN (lat_interval, long_interval);
    if (degrees)
        {
/* (CP) this won't work when long_interval = 1 (degree) and lat_interval = 
1.5 (degrees). Results are wrong 'precision' in the y direction labels */
#if 0
        temp_y = 0.5 / 60.0;
        j = range + temp_y;
        temp_x = range - j;
        
        if (fabs (temp_x) > 0.00001)
            {
            j = (range * 60.0) + temp_y;
            temp_x = range - (j / 60.0);
            precision = (fabs (temp_x) > 0.00001) ? 2 : 1;
            }
        else
            {
            precision = 0;
            }
#endif   
/* logic to set precision for geodetic graticule */
	jt = lat_interval;
	jn = long_interval;
	if (((lat_interval-jt) < 0.00001) && ((long_interval-jn) < 0.00001))
	{
		precision = 0;
	}
	else if (((lat_interval-jt)*60.0 - (INT) ((lat_interval-jt) *60.0)) <
		0.00001 && 
	((long_interval-jn)*60.0 - (INT) ((long_interval-jn) *
		60.0)) < 0.00001)
	{
		precision = 1;
	}
	else
	{
		precision = 2;
	}
        }
    else
        {
        temp_y = 1.0;
        precision = 0;
        
        FOREVER
            {
            j = (range * temp_y) + 0.5;
            temp_x = range - (j / temp_y);
            if (fabs (temp_x) <= 0.00001)
                {
                break;
                }
            precision++;
            temp_y *= 10.0;
            }
        }
    /* * REDO MINS AND MAXS TO EVEN INTERVAL VALUES * */
    /* ---------------------------------------------- */
    
    i = (min_lat > 0.0) ? (min_lat / lat_interval) : (min_lat / lat_interval) - 1;
    min_lat = i * lat_interval;
    
    i = (max_lat >= 0.0) ? (max_lat / lat_interval) + 1 : (max_lat / lat_interval);
    max_lat = i * lat_interval;
    
    i = (min_long > 0.0) ? (min_long / long_interval) : (min_long / long_interval) - 1;
    min_long = i * long_interval;
    
    i = (max_long >= 0.0) ?
        (max_long / long_interval) + 1 : (max_long / long_interval);
    max_long = i * long_interval;
    
    /* * ALLOCATE POINT ARRAYS * */
    /* ------------------------- */
    
    max_pts = (MAX ((max_lat - min_lat) / lat_interval,
                    (max_long - min_long) / long_interval)) + 2;
    
    x = (DOUBLE *)am_allocate (AM_GLOBAL, max_pts * sizeof(DOUBLE));
    y = (DOUBLE *)am_allocate (AM_GLOBAL, max_pts * sizeof(DOUBLE));
    
    units[0] = units[1] = 0;
    d_size[0] = d_size[1] = sizeof(DOUBLE);
    d_typ[0] = d_typ[1] = FLOAT_TYPE;
    v_type[0] = v_type[1] = VALUE_LIST;
    d_list[0] = 1, d_list[1] = 2;
    v_list[0] = (VOIDPTR)x, v_list[1] = (VOIDPTR)y;
    
    /* * INITIALIZE NLIST FOR GEODETIC LINES * */
    /* --------------------------------------- */
    
    if ((status = nl_init_nlist (&map_lines, 2, units, d_size, d_typ, v_type,
                                 max_pts)) != SUCCESS)
        {
        goto finished;
        }
    if ((status = nl_set_nlist_info (map_lines, NL_NLIST_COLUMN_NAME, 2, d_list,
                                     (VOIDPTR)c_name)) != SUCCESS)
        {
        goto finished;
        }
    /* * SET GKS CONSTANTS * */
    /* --------------------- */
    
    wm_gsln (1);
    wm_gslwsc (1.0);
    wm_gsplci (grat->color);
    wm_gschh (grat->label_size);
    wm_gschup (0.0, 1.0);
    wm_gstxci (grat->label_color);
    wm_gstxp (GRIGHT);
    wm_gschxp (1.0);
    wm_gschsp (0.0);
    wm_gstxfp (SIMPLEX, GSTRKP);
    wm_gstxal (GALEFT, GABASE);
    
    /* * DRAW ENCLOSING BOX * */
    /* ---------------------- */
    
    xxrect[0] = map->lower_left_xy.x;
    yyrect[0] = map->lower_left_xy.y;
    xxrect[1] = map->upper_right_xy.x;
    yyrect[1] = map->lower_left_xy.y;
    xxrect[2] = map->upper_right_xy.x;
    yyrect[2] = map->upper_right_xy.y;
    xxrect[3] = map->lower_left_xy.x;
    yyrect[3] = map->upper_right_xy.y;
    xxrect[4] = map->lower_left_xy.x;
    yyrect[4] = map->lower_left_xy.y;
    i = 5;
    wm_gpl (5, xxrect, yyrect);
    
    /* * SET CLIPPING VALUES * */
    /* ----------------------- */
    
    clip.Col_List[0] = 1;
    clip.Col_List[1] = 2;
    clip.minList[0] = map->lower_left_xy.x;
    clip.minList[1] = map->lower_left_xy.y;
    clip.maxList[0] = map->upper_right_xy.x;
    clip.maxList[1] = map->upper_right_xy.y;
    
    /* * SET ONE HALF INDEX POINT SIZE IN WCS * */
    /* ---------------------------------------- */
    
    range = grat->index_point_size / 2.0;
    
    /* * DO LATITUDINAL LINES AND LABELS * */
    /* ----------------------------------- */
    
    if (min_long > max_long)
        {
        max_long += full_circle;
        }
    max_lat += (0.5 * lat_interval);
    max_long += (0.5 * long_interval);
    
    /* CONSTRUCT LINES */
    
    for (temp_y = min_lat; temp_y <= max_lat; temp_y += lat_interval)
        {
        i = 0;
        for (temp_x = min_long; temp_x <= max_long; temp_x += long_interval)
            {
            geo_point[0] = (temp_x <= half_circle) ? temp_x : (temp_x - full_circle);
            geo_point[1] = temp_y;
            if (NOT degrees)
                {
                geo_point[0] *= uom->uom_definition;
                geo_point[1] *= uom->uom_definition;
                }
            ct_transform_point (geo_point[0], geo_point[1], geo_projection,
                                &map_point[0], &map_point[1], map_projection);
            x[i] = map_point[0];
            y[i] = map_point[1];
            i++;
            }
        status = nl_start_stroke (map_lines, (UINT *) & j);
        if (status < 0)
            {
            return status;
            }
        status = nl_add_points (map_lines, i, 2, d_list, v_list);
        if (status < 0)
            {
            return status;
            }
        status = nl_set_stroke_user_struct (map_lines, j, &temp_y, sizeof(DOUBLE));
        if (status < 0)
            {
            return status;
            }
        }
    /* CLIP LINES */
    
    status = nl_invoke_nlist_method (map_lines, NL_CLIP_METHOD, &clip);
    if (status < 0)
        {
        return status;
        }
    /* CONSTRUCT AND DRAW LABELS AND TICKS */
    
    temp_x = -999.0;
    status = nl_inq_nlist_int (clip.outNlist, NL_NLIST_NSTROKES, &i);
    if (status < 0)
        {
        return status;
        }
    for (j = 1; j <= i; j++)
        {
        status = nl_inq_stroke_user_struct (clip.outNlist, j, sizeof(DOUBLE),
                                            (VOIDPTR) & temp_y, (size_t *) & k);
        if (status < 0)
            {
            return status;
            }
        if (temp_y != temp_x)
            {
            grz_make_label (temp_y, precision, label, 1, degrees, full_circle,
                            half_circle);
            temp_x = temp_y;
            }
        /* LABELS */
        status = nl_set_current_point (clip.outNlist, 1, j);
        if (status < 0)
            {
            return status;
            }
        status = nl_get_points (clip.outNlist, max_pts, 2, d_list, v_list, &k);
        if (status < 0)
            {
            return status;
            }
        k--;
        if (x[0] <= clip.minList[0])
            {
            grz_draw_label (x[0], y[0], label, grat->label_size, 1);
            /* SIDE TICKS */
            grz_draw_tick (x[0], y[0], range, geo_projection, map_projection, TRUE);
            }
        if (x[k] >= clip.maxList[0])
            {
            grz_draw_label (x[k], y[k], label, grat->label_size, 1);
            /* SIDE TICKS */
            grz_draw_tick (x[k], y[k], range, geo_projection, map_projection, TRUE);
            }
        
        if (grat->index_points)
            {
            for (n = 1; n < k; n++)
                {
                grz_draw_tick (x[n], y[n], range, geo_projection, map_projection,
                               TRUE);
                grz_draw_tick (x[n], y[n], range, geo_projection, map_projection,
                               FALSE);
                }
            }
        }
    /* * RESET NLISTS * */
    /* ---------------- */
    
    status = nl_clear_nlist (map_lines);
    if (status < 0)
        {
        return status;
        }
    status = nl_free_nlist (clip.outNlist);
    if (status < 0)
        {
        return status;
        }
    /* * DO LONGITUDINAL LINES AND LABELS * */
    /* ------------------------------------ */
    
    /* CONSTRUCT LINES */
    
    for (temp_x = min_long; temp_x <= max_long; temp_x += long_interval)
        {
        i = 0;
        for (temp_y = min_lat; temp_y <= max_lat; temp_y += lat_interval)
            {
            geo_point[0] = (temp_x <= half_circle) ? temp_x : (temp_x - full_circle);
            geo_point[1] = temp_y;
            if (NOT degrees)
                {
                geo_point[0] *= uom->uom_definition;
                geo_point[1] *= uom->uom_definition;
                }
            ct_transform_point (geo_point[0], geo_point[1], geo_projection,
                                &map_point[0], &map_point[1], map_projection);
            x[i] = map_point[0];
            y[i] = map_point[1];
            i++;
            }
        status = nl_start_stroke (map_lines, (UINT *) & j);
        if (status < 0)
            {
            return status;
            }
        status = nl_add_points (map_lines, i, 2, d_list, v_list);
        if (status < 0)
            {
            return status;
            }
        status = nl_set_stroke_user_struct (map_lines, j, &temp_x, sizeof(DOUBLE));
        if (status < 0)
            {
            return status;
            }
        }
    /* CLIP LINES */
    
    status = nl_invoke_nlist_method (map_lines, NL_CLIP_METHOD, &clip);
    if (status < 0)
        {
        return status;
        }
    /* CONSTRUCT AND DRAW LABELS AND TICKS */
    
    temp_y = -999.0;
    status = nl_inq_nlist_int (clip.outNlist, NL_NLIST_NSTROKES, &i);
    if (status < 0)
        {
        return status;
        }
    for (j = 1; j <= i; j++)
        {
        status = nl_inq_stroke_user_struct (clip.outNlist, j, sizeof(DOUBLE),
                                            (VOIDPTR) & temp_x, (size_t *) & k);
        if (status < 0)
            {
            return status;
            }
        if (temp_x != temp_y)
            {
            grz_make_label (temp_x, precision, label, 0, degrees, full_circle,
                            half_circle);
            temp_y = temp_x;
            }
        status = nl_set_current_point (clip.outNlist, 1, j);
        if (status < 0)
            {
            return status;
            }
        status = nl_get_points (clip.outNlist, max_pts, 2, d_list, v_list, &k);
        if (status < 0)
            {
            return status;
            }
        k--;
        if (y[0] <= clip.minList[1])
            {
            grz_draw_label (x[0], y[0], label, grat->label_size, 0);
            /* SIDE TICKS */
            grz_draw_tick (x[0], y[0], range, geo_projection, map_projection, FALSE);
            }
        if (y[k] >= clip.maxList[1])
            {
            grz_draw_label (x[k], y[k], label, grat->label_size, 0);
            /* SIDE TICKS */
            grz_draw_tick (x[k], y[k], range, geo_projection, map_projection, FALSE);
            }
        }

    /* * RETURN * */
    /* ---------- */
    
finished:
    am_free (x);
    am_free (y);
    status = nl_free_nlist (map_lines);
    if (status < 0)
        {
        return status;
        }
    status = nl_free_nlist (clip.outNlist);
    if (status < 0)
        {
        return status;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT grz_make_label(DOUBLE value,INT precision,CHAR label[15],BOOL latitude,
        BOOL degrees,DOUBLE full_circle,DOUBLE half_circle);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    value           -(DOUBLE)
    precision       -(INT)
    label           -(CHAR [])
    latitude        -(BOOL)
    degrees         -(BOOL)
    full_circle     -(DOUBLE)
    half_circle     -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gr_drw_gratg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT grz_make_label (DOUBLE value, INT precision, CHAR label[15],
                               BOOL latitude, BOOL degrees, DOUBLE full_circle,
                               DOUBLE half_circle)
#else
static INT grz_make_label (value, precision, label, latitude, degrees, full_circle,
                               half_circle)
DOUBLE value;
INT precision;
CHAR label[15];
BOOL latitude;
BOOL degrees;
DOUBLE full_circle;
DOUBLE half_circle;
#endif
    {
    INT i, j;
    CHAR cdum[5];
    CHAR dir[2];
    DOUBLE fudge;
    
    /* ********************************************************************** */
    
    /* * MAKE SURE LABEL IS REASONABLE NUMBER * */
    /* ---------------------------------------- */
    
    if (latitude)
        {
        strcpy (dir, value > 0 ? "N" : "S");
        }
    else
        {
        while (value > half_circle)
            {
            value -= full_circle;
            }
        while (value <= (-1 * half_circle))
            {
            value += full_circle;
            }
        strcpy (dir, value > 0 ? "E" : "W");
        }
    switch (precision)
        {
    case 0:
        fudge = 0.5;
        break;
        
    case 1:
        fudge = 1.0 / 120.0;
        break;
        
    case 2:
        fudge = 1.0 / 7200.0;
        break;
        }
    value = fabs (value) + fudge;
    
    if (degrees)
        {
        
        /* * DO DEGREES * */
        /* -------------- */
        
        i = value;
        sprintf (label, "%d ", i);
        
        /* * DO MINUTES (SUPPRESS IF INTERVAL IS EVEN DEGREES) * */
        /* ----------------------------------------------------- */
        
        if (precision > 0)
            {
            j = (value - i) * 60.0;
            sprintf (cdum, "%02d'", j);
            strcat (label, cdum);
            
            /* * DO SECONDS (SUPPRESS IF INTERVAL IS EVEN MINUTES) * */
            /* ----------------------------------------------------- */
            
            if (precision EQUALS 2)
                {
                j = (value - i - (j / 60.0)) * 3600.0;
                sprintf (cdum, "%02d\"", j);
                strcat (label, cdum);
                }
            }
        }
    /* * VALUES NOT IN DEGREES, WRITE AS DECIMAL * */
    /* ------------------------------------------- */
    
    else
        {
        sprintf (cdum, "%%.%df", precision);
        sprintf (label, cdum, value);
        }
    strcat (label, dir);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT grz_draw_label(DOUBLE x,DOUBLE y,CHAR label[15],DOUBLE size,BOOL latitude);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(DOUBLE)
    y               -(DOUBLE)
    size            -(DOUBLE)
    label           -(CHAR [])
    latitude        -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gr_drw_gratg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT grz_draw_label (DOUBLE x, DOUBLE y, CHAR label[15], DOUBLE size,
                               BOOL latitude)
#else
static INT grz_draw_label (x, y, label, size, latitude)
DOUBLE x;
DOUBLE y;
DOUBLE size;
CHAR label[15];
BOOL latitude;
#endif
    {
    FLOAT label_x;
    FLOAT label_y;
    FLOAT degree_y;
    INT wkid;
    FLOAT xcat, ycat;
    FLOAT xrect[4], yrect[4];
    FLOAT dum_x, dum_y;
    FLOAT hor_size;
    CHAR cdum[15];
    CHAR *cptr;
    MAP_STRUCTURE *map;
    INT i, j;
    INT edge;
    
    /* ********************************************************************** */
    
    /* * FIND EDGE FOR LABELLING * */
    /* --------------------------- */
    
    mp_get_current_map (&map);
    if (latitude)
        {
        if (x EQUALS map->upper_right_xy.x) /* RIGHT */
            {
            edge = 3;
            }
        else if (x EQUALS map->lower_left_xy.x) /* LEFT */
            {
            edge = 4;
            }
        else if (y EQUALS map->upper_right_xy.y)    /* TOP */
            {
            edge = 1;
            }
        else if (y EQUALS map->lower_left_xy.y) /* BOTTOM */
            {
            edge = 2;
            }
        else                        /* INTERIOR */
            {
            edge = 5;
            }
        }
    else
        {
        if (y EQUALS map->upper_right_xy.y) /* TOP */
            {
            edge = 1;
            }
        else if (y EQUALS map->lower_left_xy.y) /* BOTTOM */
            {
            edge = 2;
            }
        else
            {
            goto finished;
            }
        }
    /* * GET SIZE OF STRING * */
    /* ---------------------- */
    
    dum_x = 0.0;
    dum_y = 0.0;
    wkid = MAIN_WKID;
    wm_gqtxx (wkid, dum_x, dum_y, label, &j, &xcat, &ycat, xrect, yrect);
    hor_size = fabs (xrect[1] - xrect[0]);
    
    /* * DETERMINE STARTING POINT * */
    /* ---------------------------- */
    
    switch (edge)
        {
    case 1:
        label_x = x - (0.5 * hor_size);
        label_y = y + (0.5 * size);
        break;
        
    case 2:
        label_x = x - (0.5 * hor_size);
        label_y = y - (1.5 * size);
        break;
        
    case 3:
    case 5:
        label_x = x + size;
        label_y = y - (0.5 * size);
        break;
        
    case 4:
        label_x = x - (hor_size + size);
        label_y = y - (0.5 * size);
        break;
        }
    /* * LOOK FOR BLANK * */
    /* ------------------ */
    
    cptr = strchr (label, ' ');
    
    /* * DEGREE SIGN IS IN LABEL STRING * */
    /* ---------------------------------- */
    
    if (cptr EQUALS NULL)
        {
        wm_gtx (label_x, label_y, label);
        goto finished;
        }
    /* * DEGREE SIGN NOT IN STRING: SYNTHESIZE ONE * */
    /* --------------------------------------------- */
    
    else
        {
        /* POST DEGREE INTEGER */
        i = cptr - label;
        strncpy (cdum, label, i);
        cdum[i] = '\0';
        wm_gtx (label_x, label_y, cdum);
        
        /* POST DEGREE SIGN */
        
        wm_gqtxx (wkid, dum_x, dum_y, cdum, &j, &xcat, &ycat, xrect, yrect);
        hor_size = fabs (xrect[1] - xrect[0]);
        label_x += hor_size;
        degree_y = label_y + (0.5 * size);
        strcpy (cdum, "o");
        wm_gtx (label_x, degree_y, cdum);
        
        /* POST REST OF STRING */
        cptr++;
        if (strlen (cptr))
            {
            wm_gqtxx (wkid, dum_x, dum_y, cdum, &j, &xcat, &ycat, xrect, yrect);
            hor_size = fabs (xrect[1] - xrect[0]);
            label_x += hor_size;
            strcpy (cdum, cptr);
            wm_gtx (label_x, label_y, cdum);
            }
        }
finished:
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT grz_draw_tick(DOUBLE x,DOUBLE y,DOUBLE distance,PROJECTION_STRUCTURE *geo_projection,
        PROJECTION_STRUCTURE *map_projection,BOOL horizontal);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(DOUBLE)
    y               -(DOUBLE)
    distance        -(DOUBLE)
    map_projection  -(PROJECTION_STRUCTURE *)
    geo_projection  -(PROJECTION_STRUCTURE *)
    horizontal      -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gr_drw_gratg.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT grz_draw_tick (DOUBLE x, DOUBLE y, DOUBLE distance,
                              PROJECTION_STRUCTURE *geo_projection,
                              PROJECTION_STRUCTURE *map_projection, BOOL horizontal)
#else
static INT grz_draw_tick (x, y, distance, geo_projection, map_projection,
                              horizontal)
DOUBLE x;
DOUBLE y;
DOUBLE distance;
PROJECTION_STRUCTURE *map_projection;
PROJECTION_STRUCTURE *geo_projection;
BOOL horizontal;
#endif
    {
    FLOAT x_tick[22];
    FLOAT y_tick[22];
    DOUBLE geo_point[2];
    DOUBLE start_point[2];
    DOUBLE map_point[2];
    DOUBLE inc;
    DOUBLE dist1, dist2;
    INT ndx, other;
    MAP_STRUCTURE *map;
    INT i;
    
    /* ********************************************************************** */
    
    mp_get_current_map (&map);
    
    /* * SET UP VARIABLE TO USE BASED ON LINE TRYING TO DRAW * */
    /* ------------------------------------------------------- */
    
    if (horizontal)
        {
        ndx = 0;
        other = 1;
        dist1 = MIN (distance, fabs ((x - map->lower_left_xy.x)));
        dist2 = MIN (distance, fabs ((x - map->upper_right_xy.x)));
        }
    else
        {
        ndx = 1;
        other = 0;
        dist1 = MIN (distance, fabs ((y - map->lower_left_xy.y)));
        dist2 = MIN (distance, fabs ((y - map->upper_right_xy.y)));
        }
    /* * GET STARTING POINT * */
    /* ---------------------- */
    
    map_point[0] = x;
    map_point[1] = y;
    ct_transform_point (map_point[0], map_point[1], map_projection, &start_point[0],
                        &start_point[1], geo_projection);
    
    /* * DO FIRST HALF OF LINE * */
    /* ------------------------- */
    
    map_point[ndx] -= dist1;
    ct_transform_point (map_point[0], map_point[1], map_projection, &geo_point[0],
                        &geo_point[1], geo_projection);
    
    inc = (geo_point[ndx] - start_point[ndx]) / 4.0;
    geo_point[other] = start_point[other];
    for (i = 0; i < 4; i++)
        {
        geo_point[ndx] = start_point[ndx] + ((4 - i) * inc);
        ct_transform_point (geo_point[0], geo_point[1], geo_projection, &map_point[0],
                            &map_point[1], map_projection);
        x_tick[i] = map_point[0];
        y_tick[i] = map_point[1];
        }
    x_tick[i] = x;
    y_tick[i] = y;
    
    /* * DO SECOND HALF OF LINE * */
    /* -------------------------- */
    
    map_point[0] = x;
    map_point[1] = y;
    map_point[ndx] += dist2;
    ct_transform_point (map_point[0], map_point[1], map_projection, &geo_point[0],
                        &geo_point[1], geo_projection);
    
    inc = (geo_point[ndx] - start_point[ndx]) / 4.0;
    geo_point[other] = start_point[other];
    for (i = 5; i < 9; i++)
        {
        geo_point[ndx] = start_point[ndx] + ((i - 4) * inc);
        ct_transform_point (geo_point[0], geo_point[1], geo_projection, &map_point[0],
                            &map_point[1], map_projection);
        x_tick[i] = map_point[0];
        y_tick[i] = map_point[1];
        }
    wm_gpl (i, x_tick, y_tick);
    
    return SUCCESS;
    }
/* END:     */
