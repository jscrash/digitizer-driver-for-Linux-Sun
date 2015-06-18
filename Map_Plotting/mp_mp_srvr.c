/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MP_SRVR.C*/
/*  *5    11-MAR-1991 11:05:07 KEE "(SPR -1) Totally modify Charlie Purmoy's Edit Well Depth Application base on QA new*/
/* specifications"*/
/*  *4    21-SEP-1990 08:40:37 CHARLIE "(SPR -1) start well editor v1.0"*/
/*  *3    17-AUG-1990 22:07:19 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:57:48 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:32 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MP_SRVR.C*/
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

#ifndef ESI_C_LIB_H

#include    "esi_c_lib.h"

#endif

#ifndef ESI_MP_H

#include    "esi_mp.h"

#endif

#ifndef ESI_AM_H

#include    "esi_am.h"

#endif

#ifndef ESI_WI_H

#include    "esi_wi.h"

#endif

#ifndef MP_MENUS_RS

#include    "mp_menus.rh"

#endif

#ifndef ESI_MD_MSG_H

#include    "esi_md_msg.h"

#endif

#ifndef ESI_MD_ERR_H

#include    "esi_md_err.h"

#endif

#ifndef ESI_DR_H

#include    "esi_dr.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    A routine which services the mapping menu.

Prototype:
    publicdef INT mp_mapping_server(INT id,INT item,BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    pointer         -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_mapping_server (INT id, INT item, BYTE *pointer)
#else
publicdef INT mp_mapping_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    PROGNAME ("mp_mapping_server");
    INT status;
    MENU_ID *menu_id;
    BYTE p;
    
    /*************************************************************************/
    
    publicdef am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *) & menu_id);
    
    /* * TURN OFF THE MAPPING MENU * */
    /* ----------------------------- */
    
    menu_id->menu_status[0] = OFF;
    
    /* * SPECIAL CASE FOR MAKE (TURN ON MAKE MENU, NO ROUTINE CALL) * */
    /* -------------------------------------------------------------- */
    
    if (item EQUALS MP_MAKE_MAP)
        {
        menu_id->menu_status[1] = ON;
        
        menu_id->menu_item_status[1][0] = ON;
        menu_id->menu_item_status[1][1] = ON;
        menu_id->menu_item_status[1][2] = OFF;
        menu_id->menu_item_status[1][3] = OFF;
        menu_id->menu_item_status[1][4] = ON;
        menu_id->menu_item_status[1][5] = ON;
        menu_id->menu_item_status[1][6] = ON;
        
        mp_menu_status ();
        }
    /* * PROCESS ALL OTHER POSSIBLE MENU PICKS * */
    /* ----------------------------------------- */
    
    else
        {
        mp_menu_status ();
        
        switch (item)
            {
            /*------------------------------------------------------------------------*/
        case MP_REGISTER_MAP:       /*  Map registration process                                                 */
            /*------------------------------------------------------------------------*/
            
            status = mr_register_map ();
            menu_id->menu_status[0] = ON;
            mp_menu_status ();
            break;
            
            /*------------------------------------------------------------------------*/
        case MP_EDIT_SEISMIC:       /*  Root for seismic editing                                                 */
            /*------------------------------------------------------------------------*/
            
            status = sx_main (pointer);
            
            /* YOU CANNOT TURN ON MAPPING MENUS HERE, UNLESS AN ERROR OCCURS IN SX_MAIN.
               IF SX_MAIN STARTS NORMALLY THE MAPPING MENUS ARE TURNED ON AGAIN
               IN SX_SHUTDOWN AFTER PICKING QUIT. I HAVE MADE THIS CHANGE TWICE
               AND WOULD APPRECIATE IT IF THIS COULD BE LEFT AS IT IS.
                                                                    CHARLES E. PUMROY. */
            if (status != SUCCESS)
                {
                menu_id->menu_status[0] = ON;
                mp_menu_status ();
                }
            break;
            
            /*------------------------------------------------------------------------*/
        case MP_EDIT_WELLS:         /*  Root for well editing                                                 */
            /*------------------------------------------------------------------------*/
            status = we_edit_well_init (pointer);
	    if (status != SUCCESS)
	      {
		menu_id->menu_status[0] = ON;
		mp_menu_status ();
	      }
            break;
            
            /*------------------------------------------------------------------------*/
        case MP_GRAPHICS_EDITOR:    /*  Electronic drafting of graphic objects                                */
            /*------------------------------------------------------------------------*/
            
            /* NOTE: Menus are turned back on in dr_term - JGG */
            status = dr_electronic_drafting (&p);
            break;
            
            /*------------------------------------------------------------------------*/
        case MP_QUIT_MAP_APPLICATION:   /*  Quit the mapping module                                               */
            /*------------------------------------------------------------------------*/
            
            /* NOTE: This style creates a sticky interface that interferes with then
            next instantiation.  Flags serviced by the mp_menu_status module must be
            correctly synchronized, which means a bit more traffic on the interface,
            and some pretty strange coding styles.  JGG 29 June 88 */
            
            am_quit ();
            break;
            
            /*------------------------------------------------------------------------*/
        default:
            break;
            /*------------------------------------------------------------------------*/
            
            }
        }
    return SUCCESS;
    }
/* END:     */
