/* DEC/CMS REPLACEMENT HISTORY, Element SX_SHUTDOWN.C*/
/* *5    17-AUG-1990 22:22:36 VINCE "(SPR 5644) Code Cleanup"*/
/* *4    15-JAN-1990 08:12:56 GORDON "(SPR 0) Fix casting."*/
/* *3    25-JUL-1989 20:14:43 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:04:26 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:30:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_SHUTDOWN.C*/
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

#include "esi_sx.h"
#include "esi_am.h"
#include "esi_mp.h"

/* Function Description -----------------------------------------------------
Description:
    Shut down sle.

Prototype:
    publicdef INT sx_shutdown(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_shutdown (SX_STRUCT *psx)
#else
publicdef INT sx_shutdown (psx)
SX_STRUCT *psx;
#endif
    {
    INT status;
    MENU_ID *menu_id;
    
    /* assume success */
    
    status = SUCCESS;
    
    /* check save */
    
    sx_save_line (psx, TRUE);
    
    /* b tree of lines */
    
    btfree (&psx->lines);
    
    /* close down current map */
    
    mp_set_current_map ((MAP_STRUCTURE *)0);
    
    /* resources */
    
    am_release_resource (psx->menu_id.ls);
    am_release_resource (psx->menu_id.lp);
    am_release_resource (psx->menu_id.sp);
    am_release_resource (psx->cdlg_id.main);
    
    /* disp opt */
    /* make seis line edit more wysiwyg (see also sx_main) */
#if 0
    psx->pso->disp_opt.ov = psx->stash;
#endif
    psx->pso->psx = (VOIDPTR)0;
    
    am_free (psx);
    am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *) & menu_id);
    menu_id->menu_status[0] = ON;
    mp_menu_status ();
    
    SX_RETURN;
    }
/* END:     */
