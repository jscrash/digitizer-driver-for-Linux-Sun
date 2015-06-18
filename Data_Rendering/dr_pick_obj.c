/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_OBJ.C*/
/* *6    12-OCT-1990 16:31:05 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *5    24-SEP-1990 15:35:00 GILLESPIE "Beta Deltas"*/
/* *4    18-JUL-1990 11:03:29 GILLESPIE "(SPR 1) Add esi_dr_msg.h,esi_dr_err.h"*/
/* *3    22-DEC-1989 14:44:13 GORDON "(SPR 0) fix wmoinq call, set pick_object on good pick."*/
/* *2    13-JUL-1989 13:49:24 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:47:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_OBJ.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_OBJ.C*/
/* *4    18-JUL-1990 11:03:29 GILLESPIE "(SPR 1) Add esi_dr_msg.h,esi_dr_err.h"*/
/* *3    22-DEC-1989 14:44:13 GORDON "(SPR 0) fix wmoinq call, set pick_object on good pick."*/
/* *2    13-JUL-1989 13:49:24 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:47:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_OBJ.C*/

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


/******************************************************************************

NAME:           dr_pick_object 

DESCRIPTION:    Graphic pick an object 


     Input:     dr_data - The standard structure

     Output:    dr_data - updated as follows:
                    current_object      0 if pick fails, else WM id.
                    pick_object         0 if pick fails, else WM id.
                    current_object_key  0 if pick fails, else INT db_key.
                    
                The WM group for the picked object is opened.

     Status:    SUCCESS
                DR_CANCEL
******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"

#include    "esi_am.h"
#include    "esi_wm.h"

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "esi_dr_msg.h"
#include "esi_dr_err.h"

#if USE_PROTOTYPES
publicdef INT dr_pick_object (DR_DATA *dr_data)
#else
publicdef INT dr_pick_object (dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status = SUCCESS;
    CHAR prompt[80];
    CHAR user_name[40];
    CHAR db_key[41];
    BOOL found, not_found;
    BOOL cancel;
    INT obj_id, group_id;
    PICKABLE_OBJECT elem_list;
    GO_USER_STRUCT object;
    INT button;
    INT segment, pick, window, tnr, application, class;
    FLOAT wx, wy;
    FILENAME nlist_file;
    LINE line;
    TEXT text;

    strcpy (prompt, mg_message (DR_PICK_OBJECT));
    cancel = FALSE;
    not_found = TRUE;

    if (ug_if_gpx ())
        wm_set_mouse_menu ((CHAR * *)0, (INT *)0);  /* use default menu */

    while (not_found && !cancel)
        {
        am_message (AM_DIALOG, prompt);
        wmtlocate (&status, &wx, &wy, &button);
        cancel = (button == WM_BUTTON_CANCEL) || (button == WM_BUTTON_DONE);
        not_found = (status == FAIL);
        if (!(not_found || cancel))
            {
            status = dr_select_go_element (dr_data, &wx, &wy, &object);
            if (status != SUCCESS)
                not_found = TRUE;
            else
                {
                wmoinq (object.group_id, &class, db_key, user_name);
                if (strcmp (user_name, "Non-editable Graphic Objects") == 0)
                    {
                    status = DR_NOT_EDITABLE;
                    am_message (AM_ALERT, mg_message (DR_NOT_EDITABLE));
                    }
                else
                    {
                    /* -------------------------------------------- */
                    /* This is the object type's database OBJECT_ID */
                    /* -------------------------------------------- */
                    dr_data->current_object_key = object.obj_id;
                    dr_data->current_object = object.group_id;
                    
                    /* ------------------------------------- */
                    /* The segment id is the WM object id of */
                    /* the object picked                     */
                    /* ------------------------------------- */
                    dr_data->pick_object = object.segment_id;
                    am_message (AM_DIALOG, NULL);
                    return(SUCCESS);
                    }
                }
            }
        if (not_found && !cancel)
            strcpy (prompt, mg_message (DR_PICK_CLOSER_OBJECT));
        else
            status = DR_CANCEL;
        }

    /*  Clear the prompt area */

    am_message (AM_DIALOG, NULL);

    return status;
    }

