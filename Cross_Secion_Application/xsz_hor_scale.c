/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_HOR_SCALE.C*/
/* *7    25-JUL-1991 11:19:19 JESSIE "(SPR 0) add message box for invalid scale value case#3236"*/
/* *6    17-AUG-1990 22:36:00 VINCE "(SPR 5644) Code Cleanup"*/
/* *5     7-MAR-1990 18:13:19 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *4    18-SEP-1989 12:44:57 JULIAN "Gulf mods under SPR 100"*/
/* *3    11-AUG-1989 16:47:08 JULIAN "(SPR -1) statement not reached"*/
/* *2    26-JUN-1989 11:05:54 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:43:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element XSZ_HOR_SCALE.C*/
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
    To handle user requests for scaling in any units and      
    compute the correct scale factor to get from stored        
    horizontal distances to map inches.                        

Prototype:
    publicdef INT xsz_horiz_scale_calc (XS_STRUCT *pxs );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pxs             -(XS_STRUCT *) Pointer to cross section structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT xsz_horiz_scale_calc (XS_STRUCT *pxs)
#else
publicdef INT xsz_horiz_scale_calc (pxs)
XS_STRUCT *pxs;
#endif
    {
    INT status;
    UOM_NAME uom_wcs;
    UOM_NAME uom_map;
    DOUBLE stored_to_display, display_to_inches;
    DOUBLE horiz_scale;
    
    PROJECT_DEFAULT *proj;
    
    status = tu_parse_scale2 (pxs->xs_scale_str, &horiz_scale, uom_wcs, uom_map);
    
    switch (status)
        {
    case TU_INVALID_STRING:
        status = am_message (AM_ALERT, mg_message (XS_BAD_HORIZ_SCALE_DEF));
        return FAIL;
    case TU_INVALID_UOM:
        status = am_message (AM_ALERT, mg_message (XS_BAD_HORIZ_UOM_CODE));
        return FAIL;
    case TU_INVALID_VALUE:
        status = am_message (AM_ALERT, "Invalid horizonal scale value.");
        return FAIL;
    case SUCCESS:
        break;
    default:
        return FAIL;
        }
    status = am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL, (VOIDPTR *) & proj);
    
    if (uom_map[0] == '\0' AND uom_wcs[0] == '\0')
        {
        strcpy (uom_map, proj->unit.horizontal);
        strcpy (uom_wcs, proj->unit.horizontal);
        }
    if (ts_like (uom_map, "*CENTI*", '*') || ts_like (uom_map, "CM", '*') ||
        ts_like (uom_map, "METER*", '*'))
        {
        pxs->xs_scale_user_units = XS_CENTIMETER_UNITS;
        }
    else
        {
        pxs->xs_scale_user_units = XS_INCH_UNITS;
        }
    status = tu_unit_conversion (1.0, uom_wcs, &stored_to_display,
                                 proj->unit.horizontal);
    
    if (status == TU_INVALID_UOM)   /* Default units must be bad.      */
        {
        status = am_message (AM_ALERT, mg_message (XS_BAD_DEFAULT_UNITS));
        return FAIL;
        }
    status = tu_unit_conversion (1.0, uom_map, &display_to_inches, "INCHES");
    
    if (status == SUCCESS)          /* Compute the scale factor.       */
        {
        pxs->xs_scale = horiz_scale * stored_to_display / display_to_inches;
        strcpy (pxs->xs_scale_map_units, uom_wcs);
        }
    else                            /* "INCHES" must be bad.           */
        {
        status = am_message (AM_ALERT, mg_message (XS_INCHES_REQUIRED));
        return FAIL;
        }
    return SUCCESS;
    }
/* END:     */
