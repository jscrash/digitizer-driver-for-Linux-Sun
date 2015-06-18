/* DEC/CMS REPLACEMENT HISTORY, Element MP_IS_SEISOVL.C */
/* *2     7-JUN-1991 11:48:55 MING "(SPR 0) change seismic time to seismic data" */
/* *1    28-SEP-1990 11:02:05 GILLESPIE "Beta Deltas" */
/* DEC/CMS REPLACEMENT HISTORY, Element MP_IS_SEISOVL.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND MAY
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

#include "esi_gl_defs.h"
#ifndef ESI_MD_DBDEFS_H
#include "esi_md_dbdefs.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    This routine simply determines whether the given
    map overlay code # is one of the various seismic overlays.

Prototype:
    publicdef BOOL mp_is_seismic_overlay(INT verb_code);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verb_code   (I) one of the code constants defined in esi_md_dbdefs.h.

Return Value/Status:
    TRUE  - Verb_code is one of the seismic overlays;
    FALSE - otherwise.


Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL mp_is_seismic_overlay(INT verb_code)
#else
publicdef BOOL mp_is_seismic_overlay( verb_code )
INT verb_code;
#endif
    {
    INT status = TRUE;
    switch ( verb_code )
        {    
    case MP_SEISMIC:
    case MP_SEISDPTH:
    case MP_SEISCHRN:
    case MP_SEISPACH:
    case MP_MISTIE:
    case MP_SEISDATA:
        break;
        
    default:
        status = FALSE;
        }
    return status;
    }
/* END:     */
