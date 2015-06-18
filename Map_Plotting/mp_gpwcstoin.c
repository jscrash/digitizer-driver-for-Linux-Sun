/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GPWCSTOIN.C*/
/*  *4    17-AUG-1990 22:05:57 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3     7-MAR-1990 17:24:51 WALTERS "(SPR 0) Remove FLOAT types from system"*/
/*  *2    10-OCT-1989 16:57:03 CONROY "(SPR 5028) Initial checkin"*/
/*  *1    10-OCT-1989 16:39:12 CONROY "determines wcs to inches based on plot scale"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GPWCSTOIN.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_CT_H

#include "esi_ct.h"

#endif

#ifndef ESI_TU_H

#include "esi_tu.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to determine the conversion factor of WCS
    to inches based upon the plot scale.

Prototype:
    publicdef INT mp_get_plot_wcs_to_inches(MAP_STRUCTURE *map,
        DOUBLE *wcs_to_inches);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    map             -(MAP_STRUCTURE *)
    wcs_to_inches   -(DOUBLE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_get_plot_wcs_to_inches (MAP_STRUCTURE *map, DOUBLE *wcs_to_inches)
#else
publicdef INT mp_get_plot_wcs_to_inches (map, wcs_to_inches)
MAP_STRUCTURE *map;
DOUBLE *wcs_to_inches;
#endif
    {
    INT status = SUCCESS;
    UOM_NAME uom1;
    UOM_NAME uom2;
    UOM_NAME proj_uom;
    DOUBLE dummy;
    CHAR dum_string[100];
    
    /* ********************************************************************** */
    
    ct_get_projection_uom (map->map_projection, proj_uom);
    
    sprintf (dum_string, "%f %s", map->map_scale, map->map_scale_uom);
    tu_parse_unit (dum_string, &dummy, uom1, uom2);
    
    dummy = 1.0;
    tu_unit_conversion (dummy, proj_uom, &dummy, uom1);
    
    dummy /= map->map_scale;
    tu_unit_conversion (dummy, uom2, &dummy, "INCHES");
    
    *wcs_to_inches = dummy;
    return status;
    }
/* END:     */
