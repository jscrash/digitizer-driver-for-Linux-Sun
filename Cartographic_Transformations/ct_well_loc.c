/* DEC/CMS REPLACEMENT HISTORY, Element CT_WELL_LOC.C */
/* *3    14-AUG-1990 13:08:05 VINCE "(SPR 5644) Header Standardization" */
/* *2    17-JAN-1990 17:28:54 GILLESPIE "(SPR 6001) Change TRUE to WE_LOCATION_ONLY" */
/* *1    17-JAN-1990 17:27:34 GILLESPIE "Rename from ct_well_loc.pc" */
/* DEC/CMS REPLACEMENT HISTORY, Element CT_WELL_LOC.C */
/* DEC/CMS REPLACEMENT HISTORY, Element CT_WELL_LOC.PC*/
/* *2    10-OCT-1989 09:47:37 JULIAN "(SPR 5020) Mods for CT parameter change"*/
/* *1    19-JUN-1989 12:29:47 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_WELL_LOC.PC*/
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

#include "esi_ct.h"
#include "esi_ct_err.h"
#include "esi_we_wells.h"

/* Function Description -----------------------------------------------------
Description:
    Function to get the location for the supplied well.
    
Prototype:
    publicdef INT ct_get_well_location(UWI uwi, DOUBLE *x, DOUBLE *y);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) uwi         -(UWI) Unique well identifier.
    (O) x           -(DOUBLE *) X value to return.
    (O) y           -(DOUBLE *) Y value to return.

Return Value/Status:
    SUCCESS - Successful completion
    CT_WELL_NOT_FOUND - Well cannot be found.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_get_well_location (UWI uwi, DOUBLE *x, DOUBLE *y) 
#else
publicdef INT ct_get_well_location (uwi, x, y)
UWI uwi;
DOUBLE *x;
DOUBLE *y;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    INT status;                     /* RETURN STATUS */
    WELL_STRUCT *pwell;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (we_get_well_header (uwi, WE_LOCATION_ONLY, &pwell) EQUALS SUCCESS)
        {
        ct_map_xy (pwell->bottomhole_xy.x, pwell->bottomhole_xy.y, x, y);
        }
    else
        {
        status = CT_WELL_NOT_FOUND;
        }
    return status;
    }
/* END:     */
