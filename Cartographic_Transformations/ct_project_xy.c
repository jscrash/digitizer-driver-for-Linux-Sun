/* DEC/CMS REPLACEMENT HISTORY, Element CT_PROJECT_XY.C*/
/* *3    14-AUG-1990 13:07:17 VINCE "(SPR 5644) Header Standardization"*/
/* *2    10-OCT-1989 09:43:00 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 12:45:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_PROJECT_XY.C*/
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

#include "esi_gl_defs.h"
#include "esi_ct.h"
#include "esi_qs.h"

/* Function Description -----------------------------------------------------
Description:
    This function converts a single point from the map to the project projection.

Prototype:
    publicdef INT ct_project_xy(DOUBLE x_coord_in, DOUBLE y_coord_in, DOUBLE *x_coord_out, 
                            DOUBLE *y_coord_out);
                            
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x_coord_in  -(DOUBLE) Point in mao projection.
    (I) y_coord_in  -(DOUBLE) Y-coordinate of above.
    (O) x_coord_out -(DOUBLE *) Point to be in project projection.
    (O) y_coord_out -(DOUBLE *) Y-coordinate of above.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_project_xy (DOUBLE x_coord_in, DOUBLE y_coord_in, DOUBLE *x_coord_out,
                             DOUBLE *y_coord_out) 
#else
publicdef INT ct_project_xy (x_coord_in, y_coord_in, x_coord_out, y_coord_out)
DOUBLE x_coord_in;
DOUBLE y_coord_in;
DOUBLE *x_coord_out;
DOUBLE *y_coord_out;
#endif
    {
    INT status = SUCCESS;
    PROJECTION_ID project_projection;
    PROJECTION_ID map_projection;
    PROJECTION_STRUCTURE *project_data;
    PROJECTION_STRUCTURE *map_data;
    
    /* ********************************************************************** */
    
    /* * COMPARE THE PROJECTION ID'S * */
    /* ------------------------------- */
    
    qs_inq_c (QS_PROJECT_PROJECTION, project_projection, (INT *)0);
    qs_inq_c (QS_MAP_PROJECTION, map_projection, (INT *)0);
    if (strcmp (project_projection, map_projection) EQUALS ZERO)
        {
        *x_coord_out = x_coord_in;
        *y_coord_out = y_coord_in;
        }
    else
        {
        ct_get_projection (project_projection, &project_data);
        ct_get_projection (map_projection, &map_data);
        status = ct_transform_point (x_coord_in, y_coord_in, map_data, x_coord_out,
                                     y_coord_out, project_data);
        }
    return status;
    }
/* END:     */
