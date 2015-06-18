/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_N_ARROW.C*/
/*  *9    15-AUG-1991 17:09:45 JTM "(SPR 0) Cast argument to wm_gslwsc() to DOUBLE."*/
/*  *8    17-AUG-1990 22:04:26 VINCE "(SPR 5644) Code Cleanup"*/
/*  *7    10-OCT-1989 09:58:15 JULIAN "(SPR 5020) mods for CT parameter change"*/
/*  *6    20-SEP-1989 07:49:53 GORDON "(SPR 100) undo last checkin"*/
/*  *5    19-SEP-1989 13:37:44 GILLESPIE "(SPR 100) GULF MODS"*/
/*  *4    18-SEP-1989 12:43:37 JULIAN "Gulf mods under SPR 100"*/
/*  *3    14-SEP-1989 16:18:33 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/*  *2    26-JUN-1989 10:54:13 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:05 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DR_N_ARROW.C*/
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

#include "esi_math.h"
#include "esi_c_lib.h"
#include "esi_mp.h"
#include "esi_ct.h"
#include "esi_wm.h"
#include "esi_am.h"
#include "esi_lu.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to construct and post the requested north arrow.

Prototype:
    publicdef INT mp_draw_north_arrow(DOUBLE wcsxmin,DOUBLE wcsxmax,DOUBLE wcsymin,
        DOUBLE wcsymax,INT type);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mcsxmin         -(DOUBLE)
    wcsxmax         -(DOUBLE)
    wcsymin         -(DOUBLE)
    wcsymax         -(DOUBLE)
    type            -(INT)

Return Value/Status:
    Function returns status from ct_get_projection and ct_transform_point.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    The window is assumed allocated and open.  The
    transform is assumed allocated.  Segment is assumed open.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_draw_north_arrow (DOUBLE wcsxmin, DOUBLE wcsxmax, DOUBLE wcsymin,
                                   DOUBLE wcsymax, INT type)
#else
publicdef INT mp_draw_north_arrow (wcsxmin, wcsxmax, wcsymin, wcsymax, type)
DOUBLE wcsxmin;
DOUBLE wcsxmax;
DOUBLE wcsymin;
DOUBLE wcsymax;
INT type;
#endif
    {
    
    INT status;
    MAP_STRUCTURE *map;
    PROJECTION_STRUCTURE *map_projection;
    PROJECTION_STRUCTURE *geo_projection;
    DOUBLE center[2], test_point[2], geo_point[2];
    DOUBLE cos_theta, sin_theta, scale;
    FLOAT x[5], y[5];
    INT i;
    INT color;
    
    /**************************************************************************/
    
    
    /* * GET ROTATION MATRIX * */
    /* ----------------------- */
    
    /*************************************************************************
        Must have the rotation matrix for the north arrow.  Get this from
        the north direction from the map center by getting the X,Y value of
        a point 1 degree north of the center.  So first must translate the
        center into lat/long, add a degree to lat and translate back.
    **************************************************************************/
    
    /* GET GEODETIC PROJECTION DATA */
    
    if ((status = ct_get_projection ("GEODETIC", &geo_projection)) != SUCCESS)
        {
        goto finished;
        }
    /* GET ANGLE AND ROTATION MATRIX */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & map);
    center[0] = (map->lower_left_xy.x + map->upper_right_xy.x) / 2.0;
    center[1] = (map->lower_left_xy.y + map->upper_right_xy.y) / 2.0;
    if ((status = ct_get_projection (map->map_projection, &map_projection)) != SUCCESS)
        {
        goto finished;
        }
    if ((status = ct_transform_point (center[0], center[1], map_projection,
                                      &geo_point[0], &geo_point[1],
                                      geo_projection)) != SUCCESS)
        {
        goto finished;
        }
    geo_point[1] += 1.0;
    if ((status = ct_transform_point (geo_point[0], geo_point[1], geo_projection,
                                      &test_point[0], &test_point[1],
                                      map_projection)) != SUCCESS)
        {
        goto finished;
        }
    scale = 1.570796 - atan2 (test_point[1] - center[1], test_point[0] - center[0]);
    
    cos_theta = cos (scale);
    sin_theta = sin (scale);
    
    /* * GET CENTER POINT AND SCALING FACTOR FOR NORTH ARROW * */
    /* ------------------------------------------------------- */
    
    center[0] = (wcsxmin + wcsxmax) / 2.0;
    center[1] = (wcsymin + wcsymax) / 2.0;
    scale = MIN ((wcsxmax - wcsxmin), (wcsymax - wcsymin));
    
    /* * SET GKS PARAMETERS * */
    /* ---------------------- */
    lu_text_to_index ("COLOR", &color, "WHITE");
    wm_gsfaci (color);
    wm_gsplci (color);
    
    i = 1;
    wm_gsln (i);
    wm_gsfais (i);
    
    x[0] = 1.0;
    wm_gslwsc((DOUBLE)x[0]);

    /* * DO NORTH ARROW BY TYPE * */
    /* -------------------------- */
    
    switch (type)
        {
        
    case 1:
        /* LINES FOR NORTH ARROW */
        
        x[0] = (((-0.15 * cos_theta) + (-0.35 * sin_theta)) * scale) + center[0];
        y[0] = (((-0.35 * cos_theta) - (-0.15 * sin_theta)) * scale) + center[1];
        
        x[1] = (((-0.15 * cos_theta) + (0.25 * sin_theta)) * scale) + center[0];
        y[1] = (((0.25 * cos_theta) - (-0.15 * sin_theta)) * scale) + center[1];
        
        x[2] = (((0.15 * cos_theta) + (-0.35 * sin_theta)) * scale) + center[0];
        y[2] = (((-0.35 * cos_theta) - (0.15 * sin_theta)) * scale) + center[1];
        
        x[3] = (((0.15 * cos_theta) + (0.25 * sin_theta)) * scale) + center[0];
        y[3] = (((0.25 * cos_theta) - (0.15 * sin_theta)) * scale) + center[1];
        
        i = 4;
        wm_gpl (i, x, y);
        
        /* POLYGON FOR NORTH ARROW */
        
        x[0] = (((0.15 * cos_theta) + (0.40 * sin_theta)) * scale) + center[0];
        y[0] = (((0.40 * cos_theta) - (0.15 * sin_theta)) * scale) + center[1];
        
        x[1] = (((0.30 * cos_theta) + (0.15 * sin_theta)) * scale) + center[0];
        y[1] = (((0.15 * cos_theta) - (0.30 * sin_theta)) * scale) + center[1];
        
        x[2] = (((0.15 * cos_theta) + (0.25 * sin_theta)) * scale) + center[0];
        y[2] = (((0.25 * cos_theta) - (0.15 * sin_theta)) * scale) + center[1];
        
        x[3] = ((0.15 * sin_theta) * scale) + center[0];
        y[3] = ((0.15 * cos_theta) * scale) + center[1];
        
        x[4] = x[0];
        y[4] = y[0];
        
        i = 5;
        wm_gfa (i, x, y);
        break;
        
        }
finished:
    return(status);
    }
/* END:     */
