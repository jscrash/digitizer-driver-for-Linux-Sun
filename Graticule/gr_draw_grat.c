/* DEC/CMS REPLACEMENT HISTORY, Element GR_DRAW_GRAT.C*/
/* *9    18-DEC-1990 10:01:44 JESSIE "(SPR 6345) handle tic intervals too big"*/
/* *8    14-AUG-1990 13:28:19 VINCE "(SPR 5644) Header Standardization"*/
/* *7    18-MAY-1990 11:37:26 KEE "(SPR -1) change wm_gslwsc(0) to wm_gslwsc(1.0)"*/
/* *6    12-MAR-1990 09:58:49 JULIAN "(SPR 0) fix casting"*/
/* *5     9-MAR-1990 16:28:26 JULIAN "(SPR 0) fix casting"*/
/* *4     7-MAR-1990 17:53:25 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/* *3    18-DEC-1989 11:51:56 CONROY "(SPR 5078) Mods for plotting GKS polymarkers"*/
/* *2    18-JUL-1989 15:27:07 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:53:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GR_DRAW_GRAT.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"                    /* window manager */

#endif

#ifndef ESI_GR_H

#include "esi_gr.h"                    /* graticule overlay defs */

#endif

#ifndef ESI_GR_ERR_H

#include "esi_gr_err.h"                /* error codes for gr */

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"                    /* mapping defs and structs */

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"                      /* c, math functions library */

#endif

#include "esi_gks.h"

static INT gr_get_user_interval ();
static INT gr_calc_interval ();
static INT gr_start_point ();

static UOM_NAME display_uom, projection_uom;

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gr_draw_graticule(GRAT_OV_STRUCT *grat );

Private_Functions:
    static INT gr_start_point(DOUBLE minimum,INT interval,DOUBLE *start,INT *increment);
    
    static INT gr_get_user_interval(CHAR *int_string,CHAR *int_uom,DOUBLE *delta,INT *interval);
    
    static INT gr_calc_interval(DOUBLE min,DOUBLE max,INT *interval);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This is the work horse, it draws the tics   
    and adjacent text for the current map graticule.       
    Then fill an nlist and draw appropriate text through    
    map manager routines.                                   

Prototype:
    publicdef INT gr_draw_graticule(GRAT_OV_STRUCT *grat );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    grat            -(GRAT_OV_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    GR_ERR_BAD_INTERVAL
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gr_draw_graticule (GRAT_OV_STRUCT *grat)
#else
publicdef INT gr_draw_graticule (grat)
GRAT_OV_STRUCT *grat;
#endif
    {
    
    INT status;
    INT i, j, k, n;
    INT disp_xint, disp_yint;
    INT nrows, ncols;
    CHAR label[20];
    DOUBLE x_temp, y_temp;
    FLOAT x_vect[6], y_vect[6];
    FLOAT *ndx_x, *ndx_y;
    DOUBLE disp_xmin, disp_xmax, disp_ymin, disp_ymax;
    DOUBLE x, y;
    DOUBLE x_start, y_start;
    DOUBLE tic_length;
    DOUBLE delta_x, delta_y;
    DOUBLE offset;
    UOM_NAME uom1, uom2;
    MAP_STRUCTURE *map;
    
    /***************************************************************************/
    
    /* * GET MAP * */
    /* ----------- */
    
    if ((status = mp_get_current_map (&map)) != SUCCESS)
        {
        return status;
        }
    /* * GET PROJECTION AND DISPLAY UNIT OF MEASURES * */
    /* ----------------------------------------------- */
    
    ct_get_projection_uom (map->map_projection, projection_uom);
    if (grat->uom != NULL)
        {
        strcpy (display_uom, grat->uom);
        }
    else
        {
        strcpy (display_uom, projection_uom);
        }
    /* * GET MAP CORNERS IN DISPLAY UOM * */
    /* ---------------------------------- */
    
    status = tu_unit_conversion (map->lower_left_xy.x, projection_uom, &x_temp,
                                 display_uom);
    if (status != SUCCESS)
        return status;
    
    disp_xmin = x_temp;
    
    tu_unit_conversion (map->upper_right_xy.x, projection_uom, &x_temp, display_uom);
    disp_xmax = x_temp;
    
    tu_unit_conversion (map->lower_left_xy.y, projection_uom, &y_temp, display_uom);
    disp_ymin = y_temp;
    
    tu_unit_conversion (map->upper_right_xy.y, projection_uom, &y_temp, display_uom);
    disp_ymax = y_temp;
    
    /* * GET USER ENTERED TICK INTERVALS IN DISPLAY AND PROJECTION UOM * */
    /* ----------------------------------------------------------------- */
    
    gr_get_user_interval (grat->x_tic_interval, grat->uom, &delta_x, &disp_xint);
    gr_get_user_interval (grat->y_tic_interval, grat->uom, &delta_y, &disp_yint);
    
    /* * IF EITHER OR BOTH ARE DEFAULTS MAKE BOTH EQUIVALENT * */
    /* ------------------------------------------------------- */
    
    if (grat->x_tic_interval EQUALS NULL)
        {
        if (grat->y_tic_interval EQUALS NULL)
            {
            gr_calc_interval (disp_xmin, disp_xmax, &disp_xint);
            gr_calc_interval (disp_ymin, disp_ymax, &disp_yint);
            
            disp_yint = MAX (disp_xint, disp_yint);
            y_vect[0] = disp_yint;
            tu_unit_conversion (y_vect[0], display_uom, &delta_y, projection_uom);
            }
        disp_xint = disp_yint;
        delta_x = delta_y;
        }
    else if (grat->y_tic_interval EQUALS NULL)
        {
        disp_yint = disp_xint;
        delta_y = delta_x;
        }
    /* * CHECK VALIDITY OF INTERVALS (DY AND DX RESPECTIVELY) * */
    /* -------------------------------------------------------- */
    
    if (disp_xint <= 0 OR disp_yint <= 0)
        {
        status = GR_ERR_SMALL_INTERVAL;
        return(status);
        }
    if ((disp_xint+disp_xmin) > disp_xmax OR
	(disp_yint+disp_ymin) > disp_ymax)
        {
        status = GR_ERR_BIG_INTERVAL;
        return(status);
        }
    
    /* * SET THE LINE ATTRIBUTES * */
    /* --------------------------- */
    
    wm_gsplci (grat->color);
    wm_gsln (1);
    wm_gslwsc (1.0);
    
    /* * SET THE TEXT ATTRIBUTES * */
    /* --------------------------- */
    
    wm_gstxci (grat->label_color);
    wm_gschh (grat->label_size);
    wm_gstxp (GRIGHT);
    wm_gschup (0.0, 1.0);
    wm_gschxp (1.0);
    wm_gstxfp (SIMPLEX, GSTRKP);
    wm_gstxal (GACENT, GAHALF);
    
    /* * DRAW GRATICULE BORDER * */
    /* ------------------------- */
    
    x_vect[0] = map->lower_left_xy.x;
    y_vect[0] = map->lower_left_xy.y;
    x_vect[1] = map->lower_left_xy.x;
    y_vect[1] = map->upper_right_xy.y;
    x_vect[2] = map->upper_right_xy.x;
    y_vect[2] = map->upper_right_xy.y;
    x_vect[3] = map->upper_right_xy.x;
    y_vect[3] = map->lower_left_xy.y;
    x_vect[4] = map->lower_left_xy.x;
    y_vect[4] = map->lower_left_xy.y;
    wm_gpl (5, x_vect, y_vect);
    
    /* * COMPUTE TIC MARK LENGTH AND LABEL OFFSET * */
    /* -------------------------------------------- */
    
    tic_length = grat->index_point_size / 2.0;
    offset = 2.0 * grat->label_size;
    
    /* * FOR TOP AND BOTTOM LABELS GET LEFTMOST POSITION * */
    /* --------------------------------------------------- */
    
    gr_start_point (disp_xmin, disp_xint, &x_start, &i);
    
    /* * POSTING LOOP FOR TOP AND BOTTOM (MOVE FROM LEFT TO RIGHT) * */
    /* ------------------------------------------------------------- */
    
    y_vect[0] = map->lower_left_xy.y;
    y_vect[1] = map->lower_left_xy.y + tic_length;
    y_vect[2] = map->upper_right_xy.y;
    y_vect[3] = map->upper_right_xy.y - tic_length;
    y_vect[4] = map->lower_left_xy.y - offset;
    y_vect[5] = map->upper_right_xy.y + offset;
    
    x = x_start;
    while (x <= map->upper_right_xy.x)
        {
        sprintf (label, "%d", (i * disp_xint));
        x_vect[0] = x;
        x_vect[1] = x;
        
        /* BOTTOM */
        wm_gpl (2, x_vect, y_vect);
        wm_gtx (x_vect[0], y_vect[4], label);
        
        /* TOP */
        wm_gpl (2, x_vect, &(y_vect[2]));
        wm_gtx (x_vect[0], y_vect[5], label);
        
        /* INCREMENT */
        x += delta_x;
        i++;
        }
    /* * FOR LEFT AND RIGHT LABELS GET BOTTOM MOST POSITION * */
    /* ------------------------------------------------------ */
    
    gr_start_point (disp_ymin, disp_yint, &y_start, &i);
    
    /* * POSTING LOOP FOR LEFT LABELS (MOVE FROM BOTTOM TO TOP) * */
    /* ---------------------------------------------------------- */
    
    x_vect[0] = map->lower_left_xy.x;
    x_vect[1] = map->lower_left_xy.x + tic_length;
    x_vect[2] = map->lower_left_xy.x - offset;
    wm_gstxal (GARITE, GAHALF);
    
    y = y_start;
    while (y <= map->upper_right_xy.y)
        {
        sprintf (label, "%d", (i * disp_yint));
        y_vect[0] = y;
        y_vect[1] = y;
        wm_gpl (2, x_vect, y_vect);
        wm_gtx (x_vect[2], y_vect[0], label);
        
        y += delta_y;
        i++;
        }
    /* * POSTING LOOP FOR RIGHT LABELS (MOVE FROM TOP TO BOTTOM) * */
    /* ----------------------------------------------------------- */
    
    y -= delta_y;
    i--;
    
    x_vect[0] = map->upper_right_xy.x;
    x_vect[1] = map->upper_right_xy.x - tic_length;
    x_vect[2] = map->upper_right_xy.x + offset;
    wm_gstxal (GALEFT, GAHALF);
    
    while (y >= map->lower_left_xy.y)
        {
        sprintf (label, "%d", (i * disp_yint));
        y_vect[0] = y;
        y_vect[1] = y;
        wm_gpl (2, x_vect, y_vect);
        wm_gtx (x_vect[2], y_vect[0], label);
        
        y -= delta_y;
        i--;
        }
    /* * DRAW THE INDEX POINTS IF REQUIRED * */
    /* ------------------------------------- */
    
    if (grat->index_points)
        {
        
        /* NO TICK ON BORDER */
        
        if (fabs (x_start - map->lower_left_xy.x) < 0.00005)
            {
            x_start += delta_x;
            }
        if (fabs (y_start - map->lower_left_xy.y) < 0.00005)
            {
            y_start += delta_y;
            }
        /* DETERMINE NUMBER OF ROWS AND COLS OF TICKS */
        
        ncols = (map->upper_right_xy.x - x_start) / delta_x;
        if (x_start + (ncols * delta_x) < map->upper_right_xy.x)
            {
            ncols++;
            }
        nrows = (map->upper_right_xy.y - y_start) / delta_y;
        if (y_start + (nrows * delta_y) < map->upper_right_xy.y)
            {
            nrows++;
            }
        ndx_x = (FLOAT *)am_allocate (AM_GLOBAL, (nrows * ncols) * sizeof(FLOAT));
        ndx_y = (FLOAT *)am_allocate (AM_GLOBAL, (nrows * ncols) * sizeof(FLOAT));
        
        for (i = 0, x = x_start; i < ncols; i++, x += delta_x)
            {
            j = i * nrows;
            n = j + nrows;
            for (k = j, y = y_start; k < n; k++, y += delta_y)
                {
                ndx_x[k] = x;
                ndx_y[k] = y;
                }
            }
        /* SET POLYMARKER CONTROLS */
        wm_gspmci (grat->color);
        wm_gsmk (2);
        x_vect[0] = grat->index_point_size * map->wcs_to_inches;
        wm_gsmksc (x_vect[0]);
        i = ncols * nrows;
        wm_gpm (i, ndx_x, ndx_y);
        
        /* FREE ARRAYS */
        am_free (ndx_x);
        am_free (ndx_y);
        }
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Calculate best interval from map extents    
    providing most rounded intervals.( trailing zeros )

Prototype:
    static INT gr_calc_interval(DOUBLE min,DOUBLE max,INT *interval);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    max             -(DOUBLE) -\
    min             -(DOUBLE) Extents of map, horiz. or vert. 
    interval        -(INT *) Returned calculated interval.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gr_draw_grat.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gr_calc_interval (DOUBLE min, DOUBLE max, INT *interval)
#else
static INT gr_calc_interval (min, max, interval)
DOUBLE max, min;
INT *interval;
#endif
    {
    INT i;
    DOUBLE magnitude;
    DOUBLE range;
    
    /**************************************************************************/
    
    /* * CALCULATE RANGE AND ERROR CHECK * */
    /* ----------------------------------- */
    
    range = (max - min) * 0.2;
    
    /* * KEEP NUMBER OF TICK MARKS IN [5,15) * */
    /* --------------------------------------- */
    
    magnitude = tc_magnitude (range);
    i = pow (10.0, magnitude) + 0.5;
    *interval = i * ((INT)(range / i));
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Calculate best interval from map extents   
    providing most rounded intervals.( trailing zeros ) 

Prototype:
    static INT gr_get_user_interval(CHAR *int_string,DOUBLE *delta,INT *interval);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    int_string      -(CHAR *)
    delta           -(DOUBLE *)
    interval        -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <gr_draw_grat.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gr_get_user_interval (CHAR *int_string, CHAR *int_uom, DOUBLE *delta, INT *interval)
#else
static INT gr_get_user_interval (int_string, int_uom, delta, interval)
CHAR *int_string;
CHAR *int_uom;
DOUBLE *delta;
INT *interval;
#endif
    {
    INT i;
    DOUBLE dum1, dum2;
    UOM_NAME uom1, uom2;
    
    /**************************************************************************/
    
    if (int_string != NULL)
        {
        tu_parse_unit (int_string, &dum1, uom1, uom2);
        if (strlen (uom1) > 0)
            {
            tu_unit_conversion (dum1, uom1, &dum2, projection_uom);
            *delta = dum2;
            }
        else
            {
	    if (int_uom != NULL)
		{
		tu_unit_conversion (dum1, int_uom, &dum2, projection_uom);
		*delta = dum2;
		}
	    else
		{
		strcpy (uom1, projection_uom);
		*delta = dum1;
		}
            }
        tu_unit_conversion (dum1, uom1, &dum2, display_uom);
        *interval = dum2;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Calculate starting point from minimum       
    and interval.                               

Prototype:
    static INT gr_start_point(DOUBLE minimum,INT interval,DOUBLE *start,INT *increment);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    minimum         -(DOUBLE)
    interval        -(INT)
    start           -(DOUBLE *)
    increment       -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gr_draw_grat.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gr_start_point (DOUBLE minimum, INT interval, DOUBLE *start,
                               INT *increment)
#else
static INT gr_start_point (minimum, interval, start, increment)
DOUBLE minimum;
INT interval;
DOUBLE *start;
INT *increment;
#endif
    {
    DOUBLE dum1, dum2;
    
    /**************************************************************************/
    
    (*increment) = minimum / interval;
    (*start) = interval * (*increment);
    if ((*start) < minimum)
        {
        (*start) += interval;
        (*increment)++;
        }
    dum1 = (*start);
    tu_unit_conversion (dum1, display_uom, &dum2, projection_uom);
    (*start) = dum2;
    
    return SUCCESS;
    }
/* END:     */
