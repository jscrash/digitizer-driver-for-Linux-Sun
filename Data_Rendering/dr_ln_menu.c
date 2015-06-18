/* DEC/CMS REPLACEMENT HISTORY, Element DR_LN_MENU.C*/
/* *2    12-OCT-1990 16:28:16 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *1    19-JUN-1989 12:46:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_LN_MENU.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_LN_MENU.C*/
/* *1    19-JUN-1989 12:46:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_LN_MENU.C*/

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
/*  Lines menu */
/*------------------------------------------------------------------------*/

INT dr_line_menu_server (id, item, dr_data)
INT id;
INT item;
DR_DATA *dr_data;
    {
    INT main_wkid = MAIN_WKID;
    INT status = SUCCESS;

    /*  Trash any status box                       */

    am_message (AM_DIALOG, NULL);

    switch (item)
        {

        /*----------------------------------------------------------------*/
    case DR_LI_SELECT:
        /*----------------------------------------------------------------*/

        /* Turn off any pick echo */

        if (dr_data->pick_echo)
            {
            dr_data->pick_echo = FALSE;
            wm_grsgwk (main_wkid);
            }
        status = dr_pick_line (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_LI_NEW:
        /*----------------------------------------------------------------*/

        status = dr_new_line (dr_data);
        if (status == SUCCESS)
            status = dr_edit_line (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_LI_NEW_RECTANGLE:
        /*----------------------------------------------------------------*/

        status = dr_new_rectangle (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_LI_EDIT:
        /*----------------------------------------------------------------*/

        status = dr_edit_line (dr_data);
        break;

        /*----------------------------------------------------------------*/
    case DR_LI_MOVE:
        break;
        /*----------------------------------------------------------------*/
        /*----------------------------------------------------------------*/
    case DR_LI_DELETE:
        /*----------------------------------------------------------------*/

        status = dr_delete_line_element (dr_data);
        /* Redraw to erase */
        wm_grsgwk (main_wkid);
        break;

        /*----------------------------------------------------------------*/
    case DR_LI_SETNAME:
        /*----------------------------------------------------------------*/

        status = dr_set_name (dr_data);
        break;


        /*----------------------------------------------------------------*/
    default:
        break;
        /*----------------------------------------------------------------*/

        }

    dr_update_menus (dr_data, FALSE);

    return status;
    }

