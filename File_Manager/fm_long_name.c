/* DEC/CMS REPLACEMENT HISTORY, Element FM_LONG_NAME.C*/
/* *3    14-AUG-1990 13:25:02 VINCE "(SPR 5644) Header Standardization"*/
/* *2     7-NOV-1989 13:23:40 GILLESPIE "(SPR 30) Use esi_fm_int.h include file"*/
/* *1    19-JUN-1989 12:50:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FM_LONG_NAME.C*/
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
#include "esi_fm_int.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to create and return the Finder system long name.
    (CREATOR: FILENAME).

Prototype:
    publicdef INT fm_long_name(FM_LONG in_name,FM_LONG out_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_name         -(FM_LONG)
    out_name        -(FM_LONG)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fm_long_name (FM_LONG in_name, FM_LONG out_name)
#else
publicdef INT fm_long_name (in_name, out_name)
FM_LONG in_name;
FM_LONG out_name;
#endif
    {
    ANALYST_NAME creator;
    FM_FILE filename;
    
    /**************************************************************************/
    
    fm_parse_long_name (in_name, creator, filename);
    sprintf (out_name, "%s: %s", creator, filename);
    return SUCCESS;
    }
/* END:     */
