/* DEC/CMS REPLACEMENT HISTORY, Element CT_MAP_NL_XY.C*/
/* *3    14-AUG-1990 13:06:45 VINCE "(SPR 5644) Header Standardization"*/
/* *2    30-JUN-1989 10:35:28 GILLESPIE "(SPR 33) Fix INT/UINT typedefs in calling sequence"*/
/* *1    19-JUN-1989 12:45:10 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_MAP_NL_XY.C*/
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

#include "esi_c_lib.h"
#include "esi_ct.h"
#include "esi_ct_msg.h"

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function retieves map data based on the projection data given.
    
Prototype:
    publicdef INT ct_map_nlist_xy(NLIST_HEADER nlist_in, NLIST_HEADER *nlist_out,
                              UINT *dim_list, DOUBLE tolerance);
                              
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist_in    -(NLIST_HEADER) The n-list in the project system.
    (O) nlist_out   -(NLIST_HEADER *) The n-list in the map system.
    (I) dim_list    -(UINT *) Indices of the X & Y coords.
    (I) tolerance   -(DOUBLE) Line conversion minimum arc.

Return Value/Status:
    SUCCESS - Successful completion
    CT_SAME_PROJECTIONS - Projection id's are equal.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_map_nlist_xy (NLIST_HEADER nlist_in, NLIST_HEADER *nlist_out,
                               UINT *dim_list, DOUBLE tolerance) 
#else
publicdef INT ct_map_nlist_xy (nlist_in, nlist_out, dim_list, tolerance)
NLIST_HEADER nlist_in;
NLIST_HEADER *nlist_out;
UINT *dim_list;
DOUBLE tolerance;
#endif
    {
    INT status = SUCCESS;
    PROJECTION_ID project_projection;
    PROJECTION_ID map_projection;
    PROJECTION_STRUCTURE *project_data;
    PROJECTION_STRUCTURE *map_data;
    
    /**************************************************************************/
    
    /* * COMPARE THE PROJECTION ID'S * */
    /* ------------------------------- */
    
    qs_inq_c (QS_PROJECT_PROJECTION, project_projection, (INT *)0);
    qs_inq_c (QS_MAP_PROJECTION, map_projection, (INT *)0);
    if (strcmp (project_projection, map_projection) EQUALS ZERO)
        {
        status = CT_SAME_PROJECTIONS;
        }
    else
        {
        ct_get_projection (project_projection, &project_data);
        ct_get_projection (map_projection, &map_data);
        status = ct_transform_nlist (nlist_in, project_data, nlist_out, map_data,
                                     dim_list, tolerance);
        }
    return status;
    }
/* END:     */
