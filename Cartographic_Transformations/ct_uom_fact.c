/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT.C*/
/* *5     6-MAY-1991 16:45:08 JULIAN "(SPR 0) add uom conversion from and to degrees type"*/
/* *4    14-AUG-1990 13:08:00 VINCE "(SPR 5644) Header Standardization"*/
/* *3     7-MAR-1990 18:12:17 GILLESPIE "(SPR 1) Change tu... argument from FLOAT to DOUBLE"*/
/* *2    10-OCT-1989 09:46:00 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *1    19-JUN-1989 12:45:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CT_UOM_FACT.C*/
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
#include "esi_tu.h"
#include "esi_ts.h"
#include "esi_qs.h"

/* Function Description -----------------------------------------------------
Description:
    Compute the UOM conversion factor between the map projections.
    
Prototype:
    publicdef INT ct_get_uom_factor(CHAR *from_projection,
    CHAR *to_projection, DOUBLE *factor);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) factor      -(DOUBLE *) UOM factor.
    
Return Value/Status:
    SUCCESS - Successful completion
    Function returns status of tu_conversion_factor.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ct_get_uom_factor (CHAR *from_projection,
				 CHAR *to_projection,DOUBLE *factor) 
#else
publicdef INT ct_get_uom_factor (from_projection,to_projection,factor)
CHAR *from_projection;
CHAR *to_projection;
DOUBLE *factor;
#endif
    {
    INT status = SUCCESS;
    UOM_NAME from_uom;
    UOM_NAME to_uom;
    
    /*************************************************************************/
    
    /* * COMPARE THE PROJECTION ID'S * */
    /* ------------------------------- */
    
    if (ARE_SAME (from_projection, to_projection))
        {
        *factor = 1.0;
        }
    /* * COMPARE THE PROJECTION UOMS * */
    /* ------------------------------- */
    
    else
        {
        ct_get_projection_uom (from_projection, from_uom);
        ct_get_projection_uom (to_projection, to_uom);
        if (ARE_SAME (from_uom, to_uom))
            {
            *factor = 1.0;
            }
        else
            {
            status = tu_conversion_factor (to_uom, from_uom, factor);
            }
        }
    return status;
    }
/* END:     */
