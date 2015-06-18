/* DEC/CMS REPLACEMENT HISTORY, Element CO_CLC_GRID.C*/
/* *3    14-AUG-1990 11:41:50 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-MAR-1990 18:11:45 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *1    19-JUN-1989 12:01:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_CLC_GRID.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to calculate and return the grid cell information
    necessary to send to the DGI routines to build the grid.

Prototype:
    publicdef INT co_clc_grid_cells(CONTOURS *contours, INT *ncols, DOUBLE *xmin,
                                DOUBLE *xmax, INT *nrows, DOUBLE *ymin,
                                DOUBLE *ymax);
                                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay information.
    (I) ncols       -(INT *) Number of columns in grid.
    (I) xmin        -(DOUBLE *) Minimum x value for grid.
    (I) xmax        -(DOUBLE *) Maximum x value for grid.
    (I) nrows       -(INT *) Number of rows in grid.
    (I) ymin        -(DOUBLE *) Minimum y value for grid.
    (I) ymax        -(DOUBLE *) Maximum y value for grid.

Return Value/Status:
    SUCCESS - Successful completion of call.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_clc_grid_cells (CONTOURS *contours, INT *ncols, DOUBLE *xmin,
                                 DOUBLE *xmax, INT *nrows, DOUBLE *ymin, DOUBLE *ymax) 
#else
publicdef INT co_clc_grid_cells (contours, ncols, xmin, xmax, nrows, ymin, ymax)
CONTOURS *contours;
INT *ncols;
DOUBLE *xmin;
DOUBLE *xmax;
INT *nrows;
DOUBLE *ymin;
DOUBLE *ymax;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT i, j;
    UOM_NAME grid_uom;              /* UOM STRING FOR GRID INTERVAL */
    UOM_NAME map_uom;               /* UOM STRING FOR GRID INTERVAL */
    UOM_NAME dummy_uom;             /* DUMMY UOM STRING */
    MAP_STRUCTURE *map;             /* MAP INFORMATION STRUCTURE */
    DOUBLE mapunitsize;             /* NUMBER OF UNITS IN MAP UOM */
    DOUBLE intervalsize;            /* NUMBER OF UNITS IN GRID INTERVAL */
    DOUBLE fudge;
    
    /* ********************************************************************** */
    
    /* * GET MAP WINDOW AND UOM * */
    /* -------------------------- */
    
    mp_get_current_map (&map);
    
    /* * PARSE GRID INTERVAL AND MAP SCALE FOR SIZE AND UNITS * */
    /* -------------------------------------------------------- */
    
    tu_parse_unit (contours->grid_interval, &intervalsize, grid_uom, dummy_uom);
    ct_get_projection_uom (map->map_projection, map_uom);
    
    /* * CONVERT GRID INTERVAL TO MAP UNITS * */
    /* -------------------------------------- */
    
    tu_unit_conversion (intervalsize, grid_uom, &mapunitsize, map_uom);
    
    /* * COMPUTE FUDGE FACTOR * */
    /* ------------------------ */
    
    fudge = 0.025 * MAX ((map->upper_right_xy.x - map->lower_left_xy.x),
                         (map->upper_right_xy.y - map->lower_left_xy.y));
    
    /* * DETERMINE WINDOW AND NCOLS AND NROWS * */
    /* ---------------------------------------- */
    
    if (mapunitsize EQUALS 0.0)
        {
        *ncols = 10;
        *nrows = 10;
        *xmin = map->lower_left_xy.x - fudge;
        *ymin = map->lower_left_xy.y - fudge;
        *xmax = map->upper_right_xy.x + fudge;
        *ymax = map->upper_right_xy.y + fudge;
        }
    else
        {
        i = (map->lower_left_xy.x - fudge) / mapunitsize;
        *xmin = i * mapunitsize;
        
        j = ((map->upper_right_xy.x + fudge) / mapunitsize) + 1;
        if ((*ncols = j - i + 1) < 10)
            {
            *ncols = 10;
            j = i + 9;
            }
        *xmax = j * mapunitsize;
        
        i = (map->lower_left_xy.y - fudge) / mapunitsize;
        *ymin = i * mapunitsize;
        
        j = ((map->upper_right_xy.y + fudge) / mapunitsize) + 1;
        if ((*nrows = j - i + 1) < 10)
            {
            *nrows = 10;
            j = i + 9;
            }
        *ymax = j * mapunitsize;
        }
    return SUCCESS;
    }
/* END:     */
