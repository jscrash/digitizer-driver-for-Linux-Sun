/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAPPING.C*/
/*  *7    11-MAR-1991 11:04:57 KEE "(SPR -1) Totally modify Charlie Purmoy's Edit Well Depth Application base on QA new*/
/* specifications"*/
/*  *6     4-JAN-1991 10:44:16 VINCE "(SPR 6373) Dim the EDIT WELLS menu pick"*/
/*  *5    17-AUG-1990 22:06:53 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4    20-OCT-1989 11:33:18 CONROY "(SPR 5030) remove include file esi_md_tlu"*/
/*  *3    10-OCT-1989 14:22:42 GORDON "(SPR 4999) A little cleanup"*/
/*  *2    26-JUN-1989 10:57:07 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:02 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAPPING.C*/
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
#include "esi_wm.h"
#include "esi_wi.h"
#include "esi_md_msg.h"
#include "esi_md_err.h"
#include "esi_conf_ws.h"
#include "esi_am_err.h"

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#define MD_BASIS_INIT

#include    "esi_md_basis.ah"
#include    "mp_menus.rh"
#include    "md_mapdef.rh"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

static CHAR MAP_MENUS[]    = "MP_MENUS";
static CHAR MAP_RESOURCE[] = "MD_MAPDEF";

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

/* Function Description -----------------------------------------------------
Description:
    This routine is the root for all mapping procedures.

Prototype:
    publicdef INT mp_mapping(BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointer         -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_mapping (BYTE *pointer)
#else
publicdef INT mp_mapping (pointer)
BYTE *pointer;
#endif
{
    INT			    status;
    MENU_ID		    *menu_id;	/*  Area containing menu id's	*/
    RESOURCE_ID		    *cdlg_id;	/*  Area containing cdlg id's	*/
    MP_TTLBLK_VARS	    *ttlblk;
    INT			    *last_map_number;	/*  Map number area	*/
    CONFIGURATION_WORKSPACE *config;
    
    /*************************************************************************/
    
    status = am_declare ("MAPPING");
    if (status == AM_ERR_DUPLICATE_TAG)
        {
        status = MD_MAPPING_ACTIVE;
        goto finished;
        }

    /* * SET UP A WORKSPACE TO HOLD THE MENU ID'S * */
    /* -------------------------------------------- */
    
    am_define_workspace ("CDLG", AM_APPLICATION, sizeof(RESOURCE_ID),
                         (VOIDPTR *) & cdlg_id);
    am_define_workspace ("MENU", AM_APPLICATION, sizeof(MENU_ID),
                         (VOIDPTR *) & menu_id);
    am_define_workspace ("MP_TTLBLK_VARS", AM_APPLICATION, sizeof(MP_TTLBLK_VARS),
                         (VOIDPTR *) & ttlblk);
    
    /* * PUT UP THE BASE DIALOG * */
    /* -------------------------- */
    
    status = am_define_resource (AM_DIALOG, MAP_RESOURCE, MD_BASE_DIALOG,
                                 NULL_FUNCTION_PTR, &cdlg_id->base_dialog_id,
				 NULL);
    if (status != SUCCESS)
        {
        goto finished;
        }

    wi_open_dialog (cdlg_id->base_dialog_id);
    
    /* * SETUP THE MAP NUMBERING * */
    /* --------------------------- */
    
    status = am_open_workspace ("MAP_NUMBER", AM_GLOBAL,
                                (VOIDPTR *) & last_map_number);
    if (status != SUCCESS)
        {
        if (status == AM_ERR_NOT_DEFINED)
            {
            am_define_workspace ("MAP_NUMBER", AM_GLOBAL, sizeof(INT),
                                 (VOIDPTR *) & last_map_number);
            }
        else
            goto finished;
        }

    /* * DEFINE THE MENUS * */
    /* -------------------- */
    
    /* MAPPING */
    
    status = am_define_resource (AM_MENU, MAP_MENUS, MAPPING_MENU,
				 mp_mapping_server,
				 &menu_id->mapping_id,
				 NULL);
    if (status != SUCCESS)
        {
        goto finished;
        }

    /* MAKE */
    status = am_define_resource (AM_MENU, MAP_MENUS, MAKE_MAP, mp_make_map,
                                 &menu_id->make_map_id, NULL);
    if (status != SUCCESS)
        {
        goto finished;
        }

    /* OPTIONS */
    status = am_define_resource (AM_MENU, MAP_MENUS, MAP_DISPLAY_OPTIONS,
                                 mp_options_server, &menu_id->display_options_id,
                                 NULL);
    if (status != SUCCESS)
        {
        return FAIL;
        }

    /* -------------------------------------------------------------------- */
    /* When mapping is entered more than once during a single Finder	    */
    /* session the statics in mp_menu_status need to be re-initialized. */
    /* The following two lines will set all menu items to OFF and force	    */
    /* mp_menu_status to initialize itself each time MAPPING is started.    */
    /* -------------------------------------------------------------------- */

    tc_zeroes (menu_id->menu_item_status, sizeof menu_id->menu_item_status );
    mp_menu_status ();    

    /* * SET THE INITIAL STATE FOR THE MENUS * */
    /* --------------------------------------- */
    
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *) & config);

    menu_id->menu_item_status[2][0] = NOT config->maconly;  /* Display Map  */
    menu_id->menu_item_status[2][1] = NOT config->maconly;  /* Display Ovl  */
    menu_id->menu_item_status[2][2] = ON;		    /* Plot Map	    */
    menu_id->menu_item_status[2][3] = ON;		    /* Plot Ovl	    */
    menu_id->menu_item_status[2][4] = NOT config->maconly;  /* Erase Map    */
    menu_id->menu_item_status[2][5] = NOT config->maconly;  /* Erase Ovl    */
    menu_id->menu_item_status[2][6] = ON;		    /* Map Setup    */
    
    /* LAYOUT */
    status = am_define_resource (AM_MENU, MAP_MENUS, MAP_LAYOUT,
				 mp_layout_server,
                                 &menu_id->layout_id,
				 NULL);
    if (status != SUCCESS)
        {
        return FAIL;
        }

    menu_id->menu_item_status[3][0] = ON;   /* Title Block  */
    menu_id->menu_item_status[3][1] = ON;   /* Map Setup    */
    menu_id->menu_item_status[3][2] = ON;   /* ???	    */
    
    /* * MAKE THE MAPPING MENU VISIBLE * */
    /* --------------------------------- */
    
    menu_id->menu_status[0] = ON;

    menu_id->menu_item_status[0][0] = ON;		    /* Make A Map   */
    menu_id->menu_item_status[0][1] = NOT config->maconly;  /* Reg. A Map   */
    menu_id->menu_item_status[0][2] = NOT config->maconly;  /* Edit Seis Ln */
    menu_id->menu_item_status[0][3] = NOT config->maconly;  /* Edit Well    */
    menu_id->menu_item_status[0][4] = NOT config->maconly;  /* Elec Drftng  */
    menu_id->menu_item_status[0][5] = ON;		    /* Quit	    */

    /* * INITIATE THE NEW MENU STATUS * */
    /* -------------------------------- */

    mp_menu_status ();    
    
    /* * NOW LET THE SERVER TAKE OVER * */
    /* -------------------------------- */
    
finished:
    if (status != SUCCESS)
        {
        am_message (AM_ALERT, mg_message (status));
        am_quit ();
        }

    return SUCCESS;
}
/* END:     */
