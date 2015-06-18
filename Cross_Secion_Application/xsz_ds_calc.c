/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_DS_CALC.C*/
/* *5    17-AUG-1990 22:34:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *4     7-MAR-1990 18:13:10 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *3    11-AUG-1989 16:46:41 JULIAN "(SPR -1) statement not reached"*/
/* *2    26-JUN-1989 11:05:43 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:42:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_DS_CALC.C*/
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

#include "esi_xs_defs.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TU_H
#include "esi_tu.h"
#endif

#ifndef ESI_GL_PROJECT_H
#include "esi_gl_project.h"
#endif

#ifndef ESI_TU_ERR_H
#include "esi_tu_err.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    To handle user requests for scaling in any units and compute   
    the correct scale factor to get from stored log depths to     
    paper inches.                                                  

Prototype:
    publicdef INT xsz_depth_scale_calc(XS_STRUCT *pxs);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure. 

Return Value / Status :
    SUCCESS-Successful completion
    FAIL-Unsuccessful completion

Scope : PUBLIC Limitations / Assumptions :

-----------------------------------------------------------------------------*/ 

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_depth_scale_calc (XS_STRUCT *pxs)
#else
publicdef INT xsz_depth_scale_calc (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    UOM_NAME uom_wcs;
    UOM_NAME uom_map;
    DOUBLE stored_to_display;
    DOUBLE display_to_inches;
    DOUBLE uom_depth_scale;
    DOUBLE one = 1.0;
    PROJECT_DEFAULT *proj;
    CHAR stored_units[32];
    
    status = tu_parse_scale2 (pxs->depth_scale_str, &uom_depth_scale, uom_wcs,
                              uom_map);
    
    /* warning - other errors could be generated besides these */
    
    switch (status)
        {
    case TU_INVALID_STRING:
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEPTH_SCALE_DEF));
        return FAIL;
    case TU_INVALID_UOM:
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEPTH_UOM_CODE));
        return FAIL;
    case SUCCESS:
        break;
    default:
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEPTH_SCALE_DEF));
        return FAIL;
        }
    /* Pull the depth uom from the 
       default project structure.     */
    
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj);
    strcpy (stored_units, proj->unit.vertical);
    
    /* If this is a ratio scale,
       set to default vertical uom.  */
    
    if (uom_map[0] == '\0' AND uom_wcs[0] == '\0')
        {
        strcpy (uom_map, stored_units);
        strcpy (uom_wcs, stored_units);
        }
    /* Set the user units flag in the
       cross section structure.       */
    
    if (ts_like (uom_map, "*CENTI*", '*') OR ts_like (uom_map, "CM", '*'))
        pxs->depth_scale_user_units = XS_CENTIMETER_UNITS;
    else
        pxs->depth_scale_user_units = XS_INCH_UNITS;
    
    /* Set the scale factor in the
       cross section structure.  This
       must take into account the uom
       that the logs are stored in.   */
    
    /* Compute the appropriate factor 
       from stored to displayed uom.  */
    
    status = tu_unit_conversion (one, uom_wcs, &stored_to_display, stored_units);
    
    if (status == TU_INVALID_UOM)
        {
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEFAULT_UNITS));
        return FAIL;
        }
    /* Determine conversion factor to 
       paper inches.                   */
    
    status = tu_unit_conversion (one, uom_map, &display_to_inches, "INCHES");
    if (status < SUCCESS)
        {
        status = am_message (AM_ALERT, mg_message (XS_INCHES_REQUIRED));
        return FAIL;
        }
    /* Compute the scale factor to get 
       the logs drawn in paper inches. */
    
    pxs->depth_scale = uom_depth_scale * stored_to_display / display_to_inches;
    
    /* Copy the display units to the 
       structure for depth track label.*/
    
    strcpy (pxs->depth_scale_map_units, uom_wcs);
    
    return SUCCESS;
    }
/* END:     */
