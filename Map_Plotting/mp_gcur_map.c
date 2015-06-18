/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GCUR_MAP.C*/
/*  *3    17-AUG-1990 22:05:46 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:56:03 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:04 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_GCUR_MAP.C*/
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
#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_qs.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to get the global workspace of the current
    map and return its address.

Prototype:
    publicdef INT mp_get_current_map(MAP_STRUCTURE **mapdef);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mapdef          -(MAP_STRUCTURE **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_get_current_map (MAP_STRUCTURE **mapdef)
#else
publicdef INT mp_get_current_map (mapdef)
MAP_STRUCTURE **mapdef;
#endif
    {
    
    INT map_number;
    CHAR name[30];
    INT status;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    qs_inq_i (QS_CUR_MAP_NUMBER, &map_number);
    if (map_number > 0)
        {
        sprintf (name, "MP_WORKSPACE #%d", map_number);
        if (am_open_workspace (name, AM_GLOBAL, (VOIDPTR *)mapdef) != SUCCESS)
            {
            status = FAIL;
            *mapdef = (MAP_STRUCTURE *)0;
            }
        }
    else
        {
        status = FAIL;
        *mapdef = (MAP_STRUCTURE *)0;
        }
    return status;
    }
/* END:     */
