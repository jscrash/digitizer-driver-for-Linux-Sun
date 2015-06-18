/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_IDXMP.C */
/* *7     8-JAN-1992 15:36:40 JTM "(SPR 0) Chamge prompted by BHP code. Added ``Execution continuing. map read error msg." */
/* *6    18-JUL-1991 15:23:56 MING "(SPR 0) change message for index map not exist" */
/* *5    21-DEC-1990 13:39:21 MING "(SPR 6402) do projection transformation on the primary map extent" */
/* *4     3-DEC-1990 17:40:54 MING "(SPR 6239) add more error messages" */
/* *3    18-OCT-1990 11:47:28 GILLESPIE "(SPR 1000) Merge Ernie deltas" */
/*  1B1  15-OCT-1990 17:21:24 GILLESPIE "Merge Ernie deltas" */
/* *2    15-OCT-1990 12:18:04 GILLESPIE "(SPR 1) Clean up from Ernie checkin" */
/* *1    12-OCT-1990 17:38:29 GILLESPIE "Merge Ernie Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element PL_DRAW_IDXMP.C */
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

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

#ifndef ESI_PL_H
#include "esi_pl.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_XS_DEFS_H
#include "esi_xs_defs.h"
#endif
static INT plz_connect_wells ();

#if USE_PROTOTYPES
publicdef INT pl_draw_index_map (INT bw_type, CHAR *idxmp_name, DOUBLE width,
                                 DOUBLE height, DOUBLE a, DOUBLE b, DOUBLE plot_range)
#else
publicdef INT pl_draw_index_map (bw_type, idxmp_name, width, height, a, b, plot_range)
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
    PROJECTION_STRUCTURE *map_projection, *idxmp_projection;
    CHAR msg[200];
    
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
		sprintf(msg,"Index map %s does not exist. Execution continuing.\n",
                idxmp_name);
        am_message (AM_STATUS, msg);
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
    
    mp_get_plot_wcs_to_inches (idxmp,&idxmp->wcs_to_inches);

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
    
    switch (bw_type)
        {
    case PL_MAP:
	/* --------------------------------------------------------------- */
	/* transform the extent of the primary to the index map projection */
	/* --------------------------------------------------------------- */
	status = ct_get_projection (map->map_projection,&map_projection);
	status = ct_get_projection (idxmp->map_projection,&idxmp_projection);

	status = ct_transform_point (map_ll_x,map_ll_y,map_projection,
				     &map_ll_x,&map_ll_y,idxmp_projection);
        
	status = ct_transform_point (map_ur_x,map_ur_y,map_projection,
				     &map_ur_x,&map_ur_y,idxmp_projection);
        
	status = ct_transform_point (map_x,map_y,map_projection,
				     &map_x,&map_y,idxmp_projection);
        


        /* -------------------------                                                */
        /* draw rectangle of the map                                                */
        /* --------------------------                                               */
        lu_text_to_index ("COLOR", &color, "GREY");
        wm_gsfaci (color);
        
        lu_text_to_index ("FILL_STYLE", &fill_style, "SOLID");
        wm_gsfais (fill_style);
        
        x[0] = map_ll_x;
        y[0] = map_ll_y;
        x[1] = map_ur_x;
        y[1] = map_ll_y;
        x[2] = map_ur_x;
        y[2] = map_ur_y;
        x[3] = map_ll_x;
        y[3] = map_ur_y;
        x[4] = x[0];
        y[4] = y[0];
        
        wm_gfa (5, x, y);
        
        /* -----------------------                                                  */
        /* draw pyramid                                                             */
        /* ------------------------                                                 */
        map_width_in_inch = (map_ur_x - map_ll_x) * idxmp->wcs_to_inches;
        map_height_in_inch = (map_ur_y - map_ll_y) * idxmp->wcs_to_inches;
        
        /* -------------------------------------------------------                  */
        /* only draw the pyramid when the rectangle less .2 inches                  */
        /* -------------------------------------------------------                  */
        if (map_width_in_inch < 0.2 || map_height_in_inch < 0.2)
            {
            x[0] = map_x - 1.5 / idxmp->wcs_to_inches;
            y[0] = idxmp->upper_right_xy.y + 0.5 / idxmp->wcs_to_inches;
            
            x[1] = map_x + 1.0 / idxmp->wcs_to_inches;
            y[1] = y[0];
            
            x[2] = x[1] + 1 / idxmp->wcs_to_inches / 1.414;
            y[2] = y[1] + 1 / idxmp->wcs_to_inches / 1.414;
            
            x[3] = x[0] + 1 / idxmp->wcs_to_inches / 1.414;
            y[3] = y[0] + 1 / idxmp->wcs_to_inches / 1.414;
            
            x[4] = x[0];
            y[4] = y[0];
            
            npoints = 5;
            wm_gfa (npoints, x, y);
            
            xx[0] = map_x;
            yy[0] = map_y;
            xx[1] = x[0];
            yy[1] = y[0];
            
            lu_text_to_index ("COLOR", &color, "BLACK");
            wm_gsln (1);
            wm_gsplci (color);
            wm_gslwsc (1.0);
            npoints = 2;
            wm_gpl (npoints, xx, yy);
            
            xx[1] = x[1];
            yy[1] = y[1];
            wm_gpl (npoints, xx, yy);
            
            xx[1] = x[2];
            yy[1] = y[2];
            wm_gpl (npoints, xx, yy);
            }
        break;
        
    case PL_XSECT:
        plz_connect_wells ();
        break;
        }
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


#if USE_PROTOTYPES
static INT plz_connect_wells (void)
#else
static INT plz_connect_wells ()
#endif
    {
    INT status = SUCCESS;
    INT nlayouts;
    INT i;
    GKS_INT color;
    GKS_REAL x[2], y[2];
    COORD well_x, well_y;
    XS_STRUCT **pxs;
    MAP_STRUCTURE *map;
    LAYOUT_STRUCT *pls;
    UWI uwi;
    
    status = am_open_workspace ("XS_STRUCT", AM_APPLICATION, (VOIDPTR)&pxs);
    
    /* -------------------------                                                    */
    /* get the number of layouts                                                    */
    /* -------------------------                                                    */
    nlayouts = llcount (&(*pxs)->drawing.log_plots);
    
    if (nlayouts <= 0)
        {
        am_message (AM_STATUS, "No layout is found.");
        return FAIL;
        }
    status = am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&map);
    
    /* -------------------------------------------                                  */
    /* set line attribute for connecting the wells                                  */
    /* -------------------------------------------                                  */
    wm_gsln (1);
    wm_gslwsc (2.0);
    lu_text_to_index ("COLOR", &color, "RED");
    wm_gsplci (color);
    
    /* ------------------------------------------------------------------------     */
    /* loop througth drawing struct to get the well wcs and draw the connection     */
    /* ------------------------------------------------------------------------     */
    for (i = 0; i < nlayouts; i++)
        {
        status = (i == 0) ? llfirst (&(*pxs)->drawing.log_plots,
                                     (BYTE *)&pls) : llnext (&
                                                            (*pxs)->drawing.log_plots,
                                                             (BYTE *)&pls);
        
        hoblockmove (pls->pwell->uwi, uwi, sizeof(UWI));
        
        status = ct_get_well_location (uwi, &well_x, &well_y);
        
        if (status == SUCCESS AND map->lower_left_xy.x <
            well_x AND map->lower_left_xy.y < well_y AND map->upper_right_xy.x >
            well_x AND map->upper_right_xy.y > well_y)
            {
            if (i == 0)
                {
                x[0] = well_x;
                y[0] = well_y;
                }
            else
                {
                x[1] = well_x;
                y[1] = well_y;
                
                wm_gpl (2, x, y);
                
                x[0] = well_x;
                y[0] = well_y;
                }
            }
        }
    return SUCCESS;
    }
