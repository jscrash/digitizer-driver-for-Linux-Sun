/*  DEC/CMS REPLACEMENT HISTORY, Element MP_HLT_WIN.C*/
/*  *3    17-AUG-1990 22:06:05 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:56:26 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:22 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_HLT_WIN.C*/
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
#include "esi_gks.h"
#include "esi_mp.h"
#include "esi_wm.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to "high-light" the "current" map.
    High-light => Front the map.
    Current => The map definition in the application workspace.

Prototype:
    publicdef INT mp_hilite_window();

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
publicdef INT mp_hilite_window ()
#else
publicdef INT mp_hilite_window ()
#endif
    {
    
    PROGNAME ("MP_HILITE_WINDOW");
    INT status;
    MAP_STRUCTURE *mapdef;
    INT window_id;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * RETRIEVE THE MAP DATA * */
    /* ------------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    
    /* * GET CURRENT FRONT WINDOW ID * */
    /* ------------------------------- */
    
    wm_inq_front_window (&window_id);
    
    /* * BRING THE WINDOW TO THE FRONT * */
    /* --------------------------------- */
    
    if (window_id != mapdef->map_window_id)
        {
        wmwfront (mapdef->map_window_id);
        window_id = MAIN_WKID;
        wm_grsgwk (window_id);
        }
    return status;
    }
/* END:     */
