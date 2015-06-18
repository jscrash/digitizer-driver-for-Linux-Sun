/* DEC/CMS REPLACEMENT HISTORY, Element GQZ_INIT_CT.C*/
/* *2    20-JUL-1990 11:52:18 VINCE "(SPR 5644) Header Standardization"*/
/* *1    21-JUN-1990 19:22:59 GILLESPIE "Geoquest Link"*/
/* DEC/CMS REPLACEMENT HISTORY, Element GQZ_INIT_CT.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_GL_PROJECT_H

#include "esi_gl_project.h"

#endif

#include "interface.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to initialize interface cartographic structures
    from the current Finder session.  It is assumed that a call
    to finder_config_project has been made prior to invocation.

Prototype:
    publicdef gqz_init_carto(PROJECTION *proj, GEOID *geoid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    proj            -(PROJECTION *)
    geoid           -(GEOID *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef gqz_init_carto (PROJECTION *proj, GEOID *geoid)
#else
publicdef gqz_init_carto (proj, geoid)
PROJECTION *proj;
GEOID *geoid;
#endif
    {
    INT status;
    PROJECTION_STRUCTURE *projection;
    PROJECT_DEFAULT *defaults;
    
    /* Retrieve the project defaults structure
    This is filled out in the FINDER_CONFIG_PROJECT function
    (which must have been executed before this function is called) */
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR) & defaults);
    if (status != SUCCESS)
        {
        return status;
        }
    /* Get the project projection information ready to go */
    status = ct_get_projection (defaults->projection, &projection);
    if (status != SUCCESS)
        {
        return status;
        }
    proj->usgs_proj_code = projection->projection_type;
    switch (projection->spheroid_units)
        {
    case 1:
        strcpy (proj->units, "Meters");
        break;
    case 2:
        strcpy (proj->units, "Feet");
        break;
    default:
        strcpy (proj->units, "Degrees");
        break;
        }
    proj->proj_zone = projection->zone_code;
    proj->grid_long_0 = projection->parameters.parm_4;
    proj->grid_lat_0 = projection->parameters.parm_5;
    proj->false_northing = projection->parameters.parm_6;
    proj->false_easting = projection->parameters.parm_7;
    
    geoid->usgs_geoid_code = projection->spheroid_code;
    
    return SUCCESS;
    }
/* END:     */
