/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RET_WKSPC.C*/
/*  *3    17-AUG-1990 22:09:03 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:59:57 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:34 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RET_WKSPC.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Routine to retrieve the global workspace and move it to the
    application workspace.

Prototype:
    publicdef INT mp_retrieve_workspace(INT num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    num             -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_retrieve_workspace (INT num)
#else
publicdef INT mp_retrieve_workspace (num)
INT num;
#endif
    {
    
    PROGNAME ("MP_RETRIEVE_WORKSPACE");
    INT status;
    MAP_STRUCTURE *app_mapdef;
    MAP_STRUCTURE *glb_mapdef;
    MAP_NAME name;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    sprintf (name, "MP_WORKSPACE #%d", num);
    if (am_open_workspace (name, AM_GLOBAL, (VOIDPTR *) & glb_mapdef) EQUALS SUCCESS)
        {
        if (am_open_workspace ("MAPDEF", AM_APPLICATION,
                               (VOIDPTR *) & app_mapdef) != SUCCESS)
            {
            am_define_workspace ("MAPDEF", AM_APPLICATION, sizeof(MAP_STRUCTURE),
                                 (VOIDPTR *) & app_mapdef);
            }
        hoblockmove (glb_mapdef, app_mapdef, sizeof(MAP_STRUCTURE));
        }
    else
        {
        status = FAIL;
        }
    return status;
    }
/* END:     */
