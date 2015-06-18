/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_GRD_LI.C*/
/* *4    14-AUG-1990 11:42:38 VINCE "(SPR 5644) Header Standardization"*/
/* *3     7-MAR-1990 23:16:53 GILLESPIE "(SPR 1) Minor fix"*/
/* *2     7-MAR-1990 18:12:12 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *1    19-JUN-1989 12:02:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_GET_GRD_LI.C*/
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

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to determine and return the grid cell limit 
    information to send to the DGI.

Prototype:
    publicdef INT co_get_grid_limits(CONTOURS *contours, FLOAT *scat_z, INT npts,
                                 DOUBLE *grid_min, DOUBLE *grid_max);
                                 
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay information.
    (I) scat_z      -(FLOAT *) Scatter Z values in array.
    (I) npts        -(INT) Number of scatter points.
    (I) grid_min    -(DOUBLE *) Minimum value for grid.
    (I) grid_max    -(DOUBLE *) Maximum value for grid.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_get_grid_limits (CONTOURS *contours, FLOAT *scat_z, INT npts,
                                  DOUBLE *grid_min, DOUBLE *grid_max) 
#else
publicdef INT co_get_grid_limits (contours, scat_z, npts, grid_min, grid_max)
CONTOURS *contours;
FLOAT *scat_z;
INT npts;
DOUBLE *grid_min;
DOUBLE *grid_max;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    UOM_NAME grid_uom;              /* UOM STRING FOR GRID INTERVAL */
    UOM_NAME dummy_uom;             /* DUMMY UOM STRING */
    DOUBLE range;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    *grid_min = 1000000000.0;
    *grid_max = -1000000000.0;
    
    /* * NEED TO CONSTRUCT EITHER MIN OR MAX? * */
    /* ---------------------------------------- */
    
    if (contours->grid_minimum EQUALS NULL OR contours->grid_maximum EQUALS NULL)
        {
        for (i = 0; i < npts; i++, scat_z++)
            {
            *grid_min = MIN (*grid_min, *scat_z);
            *grid_max = MAX (*grid_max, *scat_z);
            }
        range = *grid_max - *grid_min;
        *grid_min -= (range * 2);
        *grid_max += (range * 2);
        }
    /* * GET GRID MINIMUM FROM SUPPLIED STRING * */
    /* ----------------------------------------- */
    
    if (contours->grid_minimum != NULL)
        {
        tu_parse_unit (contours->grid_minimum, grid_min, grid_uom, dummy_uom);
        }
    /* * GET GRID MAXIMUM FROM SUPPLIED STRING * */
    /* ----------------------------------------- */
    
    if (contours->grid_maximum != NULL)
        {
        tu_parse_unit (contours->grid_maximum, grid_max, grid_uom, dummy_uom);
        }
    return status;
    }
/* END:     */
