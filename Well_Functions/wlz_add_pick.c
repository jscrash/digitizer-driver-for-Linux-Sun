/* DEC/CMS REPLACEMENT HISTORY, Element WLZ_ADD_PICK.C*/
/* *3    14-AUG-1990 13:40:01 VINCE "(SPR 5644) Header Standardization"*/
/* *2    17-JAN-1990 17:18:41 GILLESPIE "(SPR 6001) changes for new well structures"*/
/* *1    19-JUN-1989 13:39:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WLZ_ADD_PICK.C*/
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

#include "esi_wl.h"

#ifndef ESI_WE_WELLS_H

#include "esi_we_wells.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to add the supplied well to the picking structure.

Prototype:
    publicdef INT wlz_add_pick(PICKABLE_WELLS *picks,UWI uwi);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    picks           -(PICKABLE_WELLS *)
    uwi             -(UWI)

Return Value/Status:
    Function returns status from we_get_well_header.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wlz_add_pick (PICKABLE_WELLS *picks, UWI uwi) 
#else
publicdef INT wlz_add_pick (picks, uwi)
PICKABLE_WELLS *picks;
UWI uwi;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    WELL_STRUCT *pWell;
    
    /* ********************************************************************** */
    
    if ((status = we_get_well_header (uwi, WE_LOCATION_ONLY, &pWell)) EQUALS SUCCESS)
        {
        if (picks->nallocated EQUALS 0)
            {
            picks->nallocated = 100;
            picks->list = (WL_WELL_STRUCT *)tc_zalloc (picks->nallocated *
                                                       sizeof(WL_WELL_STRUCT));
            }
        else if (picks->nallocated EQUALS picks->nused)
            {
            picks->nallocated *= 2;
            picks->list = (WL_WELL_STRUCT *)tc_realloc (picks->list,
                                                        picks->nallocated *
                                                        sizeof(WL_WELL_STRUCT));
            }
        picks->list[picks->nused].pWell = pWell;
        (picks->nused)++;
        }
    return status;
    }
/* END:     */
