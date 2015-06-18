/* DEC/CMS REPLACEMENT HISTORY, Element DR_OBJ_MENU.C*/
/* *2    12-OCT-1990 16:29:06 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *1    19-JUN-1989 12:47:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_OBJ_MENU.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_OBJ_MENU.C*/
/* *1    19-JUN-1989 12:47:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_OBJ_MENU.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_gks.h"
#include    "esi_wi.h"
#include    "dr_menus.rh"

/*------------------------------------------------------------------------*/
/*  Object menu    */
/*------------------------------------------------------------------------*/

INT dr_object_menu_server (id, item, dr_data)
INT id;
INT item;
DR_DATA *dr_data;
    {
    INT main_wkid = MAIN_WKID;
    INT status = SUCCESS;

    /* Trash any status box */

    am_message (AM_DIALOG, NULL);

    switch (item)
        {

        /*----------------------------------------------------------------*/
    case DR_OB_SELECT_OBJ:
        /*----------------------------------------------------------------*/

        /* Turn off any pick echo */

        if (dr_data->pick_echo)
            {
            dr_data->pick_echo = FALSE;
            wm_grsgwk (main_wkid);
            }
            
        status = dr_select_object (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_OB_NEW_OBJ:
        /*----------------------------------------------------------------*/

        status = dr_new_object (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_OB_MOVE:
        /*----------------------------------------------------------------*/

        /*                    status = dr_move_object(dr_data);       */
        break;

        /*----------------------------------------------------------------*/
    case DR_OB_SAVE:
        /*----------------------------------------------------------------*/

        status = dr_save_object (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_OB_DELETE:
        /*----------------------------------------------------------------*/

        status = dr_delete_object (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_OB_GROUP:
    case DR_OB_UNGROUP:
        break;
        /*----------------------------------------------------------------*/
        /*----------------------------------------------------------------*/
    default:
        break;
        /*----------------------------------------------------------------*/

        }

    dr_update_menus (dr_data, FALSE);

    return status;
    }

