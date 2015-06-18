/* DEC/CMS REPLACEMENT HISTORY, Element MP_QT_TTLBLK.C*/
/* *6    17-AUG-1990 22:08:33 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    27-JUN-1990 16:38:23 KEE "(SPR -1) fixing wi_close_dialog(cdlg_id->title_block_id = 0) bug"*/
/* *4    30-MAY-1990 16:00:45 SCD "(SPR -1) Comment out wm_update after wmwdelete for X version by Kee"*/
/* *3    15-FEB-1990 11:39:47 GORDON "(SPR 0) Refresh screen after quitting (lose the titleblock window)"*/
/* *2    26-JUN-1989 10:59:32 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:14:28 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MP_QT_TTLBLK.C*/
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

#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "md_mapdef.rh"
#include "mp_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Function to QUIT the menu and dialog items for
    the TITLE BLOCK functions.

Prototype:
    publicdef INT mp_quit_title_block();

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
publicdef INT mp_quit_title_block ()
#else
publicdef INT mp_quit_title_block ()
#endif
    {
    PROGNAME ("MP_QUIT_TITLE_BLOCK");
    RESOURCE_ID *cdlg_id;
    MENU_ID *menu_id;
    MP_TTLBLK_VARS *ttlblk;
    
    /****************************************************************************/
    
    /* * OPEN WORKSPACES * */
    /* ------------------- */
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *) & menu_id);
    am_open_workspace ("MP_TTLBLK_VARS", AM_APPLICATION, (VOIDPTR *) & ttlblk);
    
    /* * TAKE DOWN THE TITLE BLOCK CDLG * */
    /* --------------------------------- */
    if (cdlg_id->title_block_id != 0)   /* test if being defined */
        {
        wi_close_dialog (cdlg_id->title_block_id);
        }
    /* * TURN OFF THE TITLE BLOCK MENU; TURN ON THE OPTION IN THE LAYOUT MENU * */
    /* ------------------------------------------------------------------------ */
    
    menu_id->menu_status[4] = OFF;
    menu_id->menu_item_status[3][0] = ON;
    mp_menu_status ();
    
    /* * DELETE THE TITLE BLOCK DISPLAY * */
    /* ---------------------------------- */
    
    wmwdelete (ttlblk->window_id);
    /* if USE_X, we don't want to do wm_update because MAIN_WKID == 0 */
#ifndef USE_X
    wm_update ();
#endif
    return SUCCESS;
    }
/* END:     */
