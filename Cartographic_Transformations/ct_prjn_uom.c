/* DEC/CMS REPLACEMENT HISTORY, Element CT_PRJN_UOM.C*/
/* *2    14-AUG-1990 13:07:05 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:45:19 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_PRJN_UOM.C*/
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

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to return the default UOM for the supplied projection
    as defined in the projection table "projections" by the field
    "projection_units", which is found in the "spheroid_units" field
    of the projection structure.  This is hardcoded to reflect the 
    UOM codes as used by the USGS routine for converting from one
    projection to another and not the Finder UOM codes.

Prototype:
    publicdef INT ct_get_projection_uom(PROJECTION_ID projection_name,
                                    UOM_NAME uom_name);
                                    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) projection_name -(PROJECTION_ID) The projection name.
    (O) uom_name        -(UOM_NAME) The UOM name of the passed projection name.
    
Return Value/Status:
    SUCCESS - Successful completion
    FAIL    - Unsuccessful completion

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_get_projection_uom (PROJECTION_ID projection_name, UOM_NAME uom_name) 
#else
publicdef INT ct_get_projection_uom (projection_name, uom_name)
PROJECTION_ID projection_name;
UOM_NAME uom_name;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    INT status;
    PROJECTION_STRUCTURE *projection_data;
    
    /**************************************************************************/
    
    if ((status = ct_get_projection (projection_name, &projection_data)) EQUALS SUCCESS)
        {
        switch (projection_data->spheroid_units)
            {
        case 0:
            strcpy (uom_name, "RADIANS");
            break;
        case 1:
            strcpy (uom_name, "FEET");
            break;
        case 2:
            strcpy (uom_name, "METERS");
            break;
        case 3:
            strcpy (uom_name, "SECONDS");
            break;
        case 4:
            strcpy (uom_name, "DEGREES");
            break;
        default:
            break;
            }
        }
    return status;
    }
/* END:     */
