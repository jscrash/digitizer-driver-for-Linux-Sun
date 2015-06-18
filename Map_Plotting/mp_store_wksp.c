/*  DEC/CMS REPLACEMENT HISTORY, Element MP_STORE_WKSP.C*/
/*  *3    17-AUG-1990 22:09:55 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 11:00:52 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:57 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_STORE_WKSP.C*/
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
    Routine to store the application workspace into the
    appropriate global workspace.

Prototype:
    publicdef INT mp_store_workspace();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_store_workspace ()
#else
publicdef INT mp_store_workspace ()
#endif
    {
    
    PROGNAME ("MP_STORE_WORKSPACE");
    MAP_STRUCTURE *app_mapdef;
    MAP_STRUCTURE *glb_mapdef;
    MAP_NAME name;
    
    /* ********************************************************************** */
    
    if (am_open_workspace ("MAPDEF", AM_APPLICATION,
                           (VOIDPTR *) & app_mapdef) EQUALS SUCCESS)
        {
        sprintf (name, "MP_WORKSPACE #%d", app_mapdef->map_number);
        if (am_open_workspace (name, AM_GLOBAL, (VOIDPTR *) & glb_mapdef) != SUCCESS)
            {
            am_define_workspace (name, AM_GLOBAL, sizeof(MAP_STRUCTURE),
                                 (VOIDPTR *) & glb_mapdef);
            }
        hoblockmove (app_mapdef, glb_mapdef, sizeof(MAP_STRUCTURE));
        }
    return SUCCESS;
    }
/* END:     */
