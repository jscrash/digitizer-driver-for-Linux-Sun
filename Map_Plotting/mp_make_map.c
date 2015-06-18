/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAKE_MAP.C*/
/*  *3    17-AUG-1990 22:06:47 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:56:58 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:58 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAKE_MAP.C*/
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
#include "mp_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Processes menu events from the MAKE MAP menu.

Prototype:
    publicdef INT mp_make_map(INT id, INT item, BYTE *pointer);

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
publicdef INT mp_make_map (INT id, INT item, BYTE *pointer)
#else
publicdef INT mp_make_map (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    INT status;
    MENU_ID *menu_id;
    static BOOL map_open = FALSE;
    
    am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *) & menu_id);
    
    switch (item)
        {
        /*--------------------------------------------------------------------------*/
    case MP_NEW_MAP:                /*  Create a new map definition                                                */
        /*--------------------------------------------------------------------------*/
        
        status = mp_create_map ();
        if (status EQUALS SUCCESS)
            {
            map_open = TRUE;
            
            menu_id->menu_status[2] = ON;
            menu_id->menu_status[3] = ON;
            
            menu_id->menu_item_status[1][0] = OFF;
            menu_id->menu_item_status[1][1] = OFF;
            menu_id->menu_item_status[1][2] = ON;
            menu_id->menu_item_status[1][3] = ON;
            menu_id->menu_item_status[1][4] = OFF;
            
            mp_menu_status ();
            }
        break;
        
        
        /*--------------------------------------------------------------------------*/
    case MP_OPEN_MAP:               /*  Draw a map using an existing map definition                             */
        /*--------------------------------------------------------------------------*/
        
        status = mp_open_map ();
        if (status EQUALS SUCCESS)
            {
            map_open = TRUE;
            
            menu_id->menu_status[2] = ON;
            menu_id->menu_status[3] = ON;
            
            menu_id->menu_item_status[1][0] = OFF;
            menu_id->menu_item_status[1][1] = OFF;
            menu_id->menu_item_status[1][2] = ON;
            menu_id->menu_item_status[1][3] = ON;
            menu_id->menu_item_status[1][4] = OFF;
            
            mp_menu_status ();
            }
        break;
        
        
        /*--------------------------------------------------------------------------*/
    case MP_CLOSE_MAP:              /*  Close the current map definition                                           */
        /*--------------------------------------------------------------------------*/
        
        if (mpz_pending_ok ())
            {
            status = mp_close_map ();
            if (status EQUALS SUCCESS)
                {
                mp_quit_title_block ();
                map_open = FALSE;
                
                menu_id->menu_status[2] = OFF;
                menu_id->menu_status[3] = OFF;
                
                menu_id->menu_item_status[1][0] = ON;
                menu_id->menu_item_status[1][1] = ON;
                menu_id->menu_item_status[1][2] = OFF;
                menu_id->menu_item_status[1][3] = OFF;
                menu_id->menu_item_status[1][4] = ON;
                
                mp_menu_status ();
                }
            }
        break;
        
        
        /*--------------------------------------------------------------------------*/
    case MP_SAVE_MAP:               /*  Save a map definition to the database                                     */
        /*--------------------------------------------------------------------------*/
        
        if (mpz_pending_ok ())
            {
            mp_save_map_pipe ();
            }
        break;
        
        
        /*--------------------------------------------------------------------------*/
    case MP_DELETE_MAP:             /*  Delete a map definition from the database                               */
        /*--------------------------------------------------------------------------*/
        
        mp_delete_map_pipe ();
        break;
        
        /*--------------------------------------------------------------------------*/
    case MP_ACTIVATE_MAP:           /*  Activate an existing map                                                      */
        /*--------------------------------------------------------------------------*/
        
        status = mp_activate_map_pipe ();
        if (status EQUALS SUCCESS)
            {
            map_open = TRUE;
            
            menu_id->menu_status[2] = ON;
            menu_id->menu_status[3] = ON;
            
            menu_id->menu_item_status[1][0] = OFF;
            menu_id->menu_item_status[1][1] = OFF;
            menu_id->menu_item_status[1][2] = ON;
            menu_id->menu_item_status[1][3] = ON;
            menu_id->menu_item_status[1][4] = OFF;
            
            mp_menu_status ();
            }
        break;
        
        
        /*--------------------------------------------------------------------------*/
    case MP_QUIT_MAPPING:           /*  Quit the make map process                                                    */
        /*--------------------------------------------------------------------------*/
        
        if (map_open)
            {
            if (mpz_pending_ok ())
                {
                status = mp_close_map ();
                }
            }
        else
            {
            status = SUCCESS;
            }
        if (status EQUALS SUCCESS)
            {
            mp_quit_title_block ();
            map_open = FALSE;
            
            menu_id->menu_status[0] = ON;
            menu_id->menu_status[1] = OFF;
            menu_id->menu_status[2] = OFF;
            menu_id->menu_status[3] = OFF;
            
            mp_menu_status ();
            }
        break;
        
        
        /*------------------------------------------------------------------------*/
    default:
        break;
        /*------------------------------------------------------------------------*/
        }
    return SUCCESS;
    }
/* END:     */
