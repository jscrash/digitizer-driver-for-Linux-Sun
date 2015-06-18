/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_MAP_WIN.C */
/* *3    18-OCT-1990 11:47:38 GILLESPIE "(SPR 1000) Merge Ernie deltas" */
/*  1B1  15-OCT-1990 17:22:26 GILLESPIE "Merge Ernie deltas" */
/* *2    15-OCT-1990 12:18:09 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:40:23 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_MAP_WIN.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Finder Graphics Systems, Inc.                                           */
/*          201 Tamal Vista Blvd                                                    */
/*          Corte Madera, CA  USA 94925                                             */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT func (arg);                                                     */
/*                                                                                  */
/*Parameters:       (indicate (I)/(O)/(M) for input/output/input-and-modified)      */
/*    (I) arg   - description                                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS   - Successfull completion                                            */
/*    FAIL  - Unsuccessfull completion                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*    PRIVATE to <module_description>                                               */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

#include "esi_pl.h"
#include "esi_c_lib.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_GTYPES_H
#include "esi_gtypes.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#if USE_PROTOTYPES
publicdef INT pl_draw_map_win (INT bw_type, CHAR *idxmp_name, DOUBLE width,
                               DOUBLE height, DOUBLE a, DOUBLE b, DOUBLE plot_range)
#else
publicdef INT pl_draw_map_win (bw_type, idxmp_name, width, height, a, b, plot_range)
INT bw_type;
CHAR *idxmp_name;
DOUBLE width;
DOUBLE height;
DOUBLE a;
DOUBLE b;
DOUBLE plot_range;
#endif
    {
    INT status = SUCCESS;
    INT map_no;
    INT *map_number;
    INT tnr_id;
    INT win_id;
    INT color;
    INT npoints;
    INT i, j;
    INT temp_row;
    GKS_INT fill_style;
    GKS_INT hatch_style;
    GKS_REAL chh;
    GKS_REAL x[5], y[5], xx[5], yy[5];
    DOUBLE diff;
    DOUBLE actual_width, actual_height;
    DOUBLE map_ratio;
    DOUBLE window_ratio;
    DOUBLE map_x, map_y;
    DOUBLE map_ll_x, map_ll_y, map_ur_x, map_ur_y;
    DOUBLE ll_wcs_x, ll_wcs_y, ur_wcs_x, ur_wcs_y;
    DOUBLE ll_ndc_x, ll_ndc_y, ur_ndc_x, ur_ndc_y;
    DOUBLE margin;
    DOUBLE map_width_in_inch, map_height_in_inch;
    CHAR dummy[100];
    UOM_NAME uom1, uom2;
    MAP_STRUCTURE *map;
    MAP_STRUCTURE *idxmp;
    PROJECT_DEFAULT *defaults;
    LINE_ATTRIBUTE line;
    
    switch (bw_type)
        {
    case PL_MAP:
        
        status = am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&map);
        
        /* ---------------------------------------                                  */
        /* store the coordinate of the primary map                                  */
        /* ---------------------------------------                                  */
        map_ll_x = map->lower_left_xy.x;
        map_ll_y = map->lower_left_xy.y;
        map_ur_x = map->upper_right_xy.x;
        map_ur_y = map->upper_right_xy.y;
        
        map_x = (DOUBLE)map->lower_left_xy.x + (DOUBLE)((map->upper_right_xy.x -
                                                         map->lower_left_xy.x) / 2.0);
        
        map_y = (DOUBLE)map->lower_left_xy.y + (DOUBLE)((map->upper_right_xy.y -
                                                         map->lower_left_xy.y) / 2.0);
        /* -----------------------------------------------------                    */
        /* store the map number and the workspace of the primary                    */
        /* ------------------------------------------------------                   */
        map_no = map->map_number;
        mp_store_workspace ();
        break;
        }
    /* ---------------------------------------------------                          */
    /* read the index map and set it to be the current map                          */
    /* ---------------------------------------------------                          */
    status = mp_read_map (idxmp_name);
    
    if (status != SUCCESS)
        {
        am_message (AM_STATUS, "No such index map");
        return FAIL;
        }
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&idxmp);
    mp_set_current_map (idxmp);
    
    /* --------------------------                                                   */
    /* Calculate the aspect ratio                                                   */
    /* --------------------------                                                   */
    map_ratio = (idxmp->upper_right_xy.y - idxmp->lower_left_xy.y) /
        (idxmp->upper_right_xy.x - idxmp->lower_left_xy.x);
    
    window_ratio = height / width;
    
    if (map_ratio > window_ratio)
        {
        /* ----------                                                               */
        /* Use full Y                                                               */
        /* ----------                                                               */
        actual_width = height / map_ratio;
        actual_height = height;
        diff = width - actual_width;
        ll_ndc_x = (a + diff / 2) / plot_range;
        ll_ndc_y = b / plot_range;
        ur_ndc_x = (a + diff / 2 + actual_width) / plot_range;
        ur_ndc_y = (b + actual_height) / plot_range;
        }
    else
        {
        /* ----------                                                               */
        /* Use full X                                                               */
        /* ----------                                                               */
        actual_width = width;
        actual_height = width * map_ratio;
        diff = height - actual_height;
        ll_ndc_x = a / plot_range;
        ll_ndc_y = (b + diff / 2) / plot_range;
        ur_ndc_x = (a + actual_width) / plot_range;
        ur_ndc_y = (b + diff / 2 + actual_height) / plot_range;
        }
    am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *)&defaults);
    sprintf (dummy, "%lf %s", idxmp->map_scale, idxmp->map_scale_uom);
    tu_parse_unit (dummy, &idxmp->wcs_to_inches, uom1, uom2);
    idxmp->wcs_to_inches = 1.0;
    tu_unit_conversion (idxmp->wcs_to_inches, defaults->unit.horizontal,
                        &idxmp->wcs_to_inches, uom1);
    idxmp->wcs_to_inches /= idxmp->map_scale;
    tu_unit_conversion (idxmp->wcs_to_inches, uom2, &idxmp->wcs_to_inches, "INCHES");
    
    if ((idxmp->upper_right_xy.x - idxmp->lower_left_xy.x) >
        idxmp->upper_right_xy.y - idxmp->lower_left_xy.y)
        {
        idxmp->wcs_to_inches /= (idxmp->upper_right_xy.x - idxmp->lower_left_xy.x) *
            idxmp->wcs_to_inches / actual_width;
        }
    else
        {
        idxmp->wcs_to_inches /= (idxmp->upper_right_xy.y - idxmp->lower_left_xy.y) *
            idxmp->wcs_to_inches / actual_height;
        }
    margin = 2.0 / idxmp->wcs_to_inches;
    ll_wcs_x = idxmp->lower_left_xy.x - margin;
    ll_wcs_y = idxmp->lower_left_xy.y - margin;
    ur_wcs_x = idxmp->upper_right_xy.x + margin;
    ur_wcs_y = idxmp->upper_right_xy.y + margin;
    
    /* ---------------------------------------------------                          */
    /* -1 for background color is to get rid of the border                          */
    /* ---------------------------------------------------                          */
    wmwallocate (&win_id, ll_ndc_x, ur_ndc_x, ll_ndc_y, ur_ndc_y, -1);
    wmtallocate (&tnr_id, 0.0, 1.0, 0.0, 1.0, ll_wcs_x, ur_wcs_x, ll_wcs_y, ur_wcs_y);
    
    /* ---------------------------------------------------                          */
    /* convert paramter to wcs if necessary since the data                          */
    /* in database are in user defined value and unit                               */
    /* ---------------------------------------------------                          */
    temp_row = -1;
    j = idxmp->display->last_row_used;
    for (i = 0; i < j; i++)
        {
        if (idxmp->display->compiled_row[i] != temp_row)
            {
            temp_row = idxmp->display->compiled_row[i];
            if (temp_row != -1)
                {
                mp_set_duom (i);
                }
            }
        }
    /* ----------------------                                                       */
    /* display the index map                                                        */
    /* ---------------------                                                        */
    i = 0;
    j = idxmp->display->last_row_used;
    mpz_display_overlays (i, j);
    
    switch (bw_type)
        {
    case PL_MAP:
        /* ------------------------------------------                               */
        /* decrement the map number for the index map                               */
        /* ------------------------------------------                               */
        am_open_workspace ("MAP_NUMBER", AM_GLOBAL, (VOIDPTR *)&map_number);
        
        *map_number--;
        
        /* ---------------------------------------------------------------          */
        /* retrieve the primary workspace and set it to be the current map          */
        /* ---------------------------------------------------------------          */
        mp_retrieve_workspace (map_no);
        am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&map);
        mp_set_current_map (map);
        break;
        }
    wmwclose (win_id);
    return status;
    }
