/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_CUR_MP.C*/
/*  *2    17-AUG-1990 22:10:08 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:14:59 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ST_CUR_MP.C*/
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
#include "esi_qs.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to set up the supplied map def as the current
    map.

Prototype:
    publicdef INT mp_set_current_map(MAP_STRUCTURE *mapdef);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mapdef          -(MAP_STRUCTURE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_set_current_map (MAP_STRUCTURE *mapdef)
#else
publicdef INT mp_set_current_map (mapdef)
MAP_STRUCTURE *mapdef;
#endif
    {
    
    PROGNAME ("MP_SET_CURRENT_MAP");
    INT status;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (mapdef != (MAP_STRUCTURE *)0)
        {
        qs_set_i (QS_CUR_MAP_NUMBER, mapdef->map_number);
        qs_set_c (QS_MAP_PROJECTION, mapdef->map_projection,
                  strlen (mapdef->map_projection));
        }
    else
        {
        qs_set_i (QS_CUR_MAP_NUMBER, 0);
        }
    return status;
    }
/* END:     */
