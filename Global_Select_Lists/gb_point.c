/* DEC/CMS REPLACEMENT HISTORY, Element GB_POINT.C*/
/* *2    14-AUG-1990 13:26:51 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:51:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_POINT.C*/
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

#include "esi_gb.h"

/* Function Description -----------------------------------------------------
Description:
    Handle graphic picking of wells, seismic, etc.

    START-HISTORY :
    04/12/87 SCD      Final version of V5 global process done under V4.
    04/04/87 SCD      Promote "make list current" to top level, convert to AM calls.
    03/04/87 SCD      Install changes for ** current selection ** version of global retrievals
    END-HISTORY  

Prototype:
    publicdef INT gb_point (SL_DATA_TYPE type, NLIST_HEADER *SLP );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    type            -(SL_DATA_TYPE)
    SLP             -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_point (SL_DATA_TYPE type, NLIST_HEADER *SLP)
#else
publicdef INT gb_point (type, SLP)
SL_DATA_TYPE type;
NLIST_HEADER *SLP;
#endif
    {
    INT status;
    
    status = 0;
    
    switch (type)
        {
    case WELLS_DATA_TYPE:
        status = gb_well_picking (SLP);
        break;
    case SEISMIC_DATA_TYPE:
        status = gb_seismic_picking (SLP);
        break;
        }
    return status;
    }
/* END:     */
