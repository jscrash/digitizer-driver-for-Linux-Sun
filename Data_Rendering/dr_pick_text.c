/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_TEXT.C*/
/* *5    12-OCT-1990 16:31:59 GILLESPIE "(SPR 100) Merge Ernie Deltas"*/
/* *4    18-JUL-1990 08:42:53 GILLESPIE "(SPR 1) Add esi_dr_err.h,esi_dr_msg.h"*/
/* *3    12-JUN-1990 10:36:00 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *2    13-JUL-1989 13:51:30 GORDON "(SPR 0) add call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:47:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_TEXT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_TEXT.C*/
/* *4    18-JUL-1990 08:42:53 GILLESPIE "(SPR 1) Add esi_dr_err.h,esi_dr_msg.h"*/
/* *3    12-JUN-1990 10:36:00 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *2    13-JUL-1989 13:51:30 GORDON "(SPR 0) add call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:47:30 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_TEXT.C*/

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

NAME:           dr_pick_text

DESCRIPTION:    Graphic pick a text element -- set current text to
                point to the picked text element, and current text
                segment to the picked segment.


     Input:     dr_data - The standard structure

     Output:    dr_data - updated

     Status:    SUCCESS     - if current text updated.
                DR_CANCEL   - if user cancelled the pick.
                FAIL        - if no matching text found.
                
    TODO:       1)  Handle bad pick report.  Set_detect makes entire object
                    pickable, so we may get back pick on segment other than
                    a text class segment.
******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#ifndef ESI_AM_H
#include    "esi_am.h"
#endif
#ifndef ESI_GKS_H
#include    "esi_gks.h"
#endif
#ifndef ESI_WM_H
#include    "esi_wm.h"
#endif
#ifndef ESI_WM_CLS_H
#include    "esi_wm_cls.h"
#endif
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif
#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#include "esi_dr_err.h"
#include "esi_dr_msg.h"

#if USE_PROTOTYPES
publicdef INT dr_pick_text (DR_DATA *dr_data)
#else
publicdef INT dr_pick_text (dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status = 0;
    CHAR prompt[80];
    CHAR msg[80];
    CHAR user_name[40];
    BOOL not_found;
    BOOL cancel;
    INT class;
    CHAR db_key[24];
    GO_USER_STRUCT object;
    FLOAT wx, wy;
    INT item_id;
    INT button;
    INT temp_seg, old_color;
    CHAR new_color[30];

    strcpy (prompt, mg_message (DR_PICK_TEXT));
    cancel = FALSE;
    not_found = TRUE;
#if 0
    status = wmgdtec (dr_data->current_object, GDETEC);
#endif

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
            status = dr_slt_go_element_of_obj (dr_data, &wx, &wy, &object);
            if ((status != SUCCESS) || (object.if_text != TRUE))
                {
                not_found = TRUE;
                }
            }
        if (not_found && !cancel)
            strcpy (prompt, mg_message (DR_PICK_TEXT_CLOSER));
        }
#if 0
    status = wmgdtec (dr_data->current_object, GUNDET);
#endif

    /* ---------------------------------------- */
    /* Locate the text element matching item_id */
    /* ---------------------------------------- */

    if (!cancel)
        {
        dr_data->pick_echo = TRUE;
        item_id = object.elem_id;
        temp_seg = object.segment_id;
        status = dr_find_element (dr_data, item_id, temp_seg, DR_TEXT_OBJECT);
        if (status == SUCCESS)
            {
            old_color = dr_data->current_text->text_color;

            lu_text_to_index ("COLOR", &dr_data->current_text->text_color,
                              "HIGHLIGHT");
            mm_draw_text (dr_data->current_text, FALSE);
            dr_data->current_text->text_color = old_color;
            }

        status = dr_find_element (dr_data, item_id, temp_seg, DR_TEXT_OBJECT);
        }
    else
        {
        status = DR_CANCEL;
        }

    /*  Kill status box  */

    am_message (AM_DIALOG, NULL);

    return status;
    }

