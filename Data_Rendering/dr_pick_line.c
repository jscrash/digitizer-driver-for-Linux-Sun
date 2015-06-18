/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_LINE.C*/
/* *6    12-OCT-1990 23:42:03 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*  4B1  12-OCT-1990 16:29:59 GILLESPIE "Merge Ernie Deltas"*/
/* *5    14-AUG-1990 21:55:56 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *4    18-JUL-1990 08:34:42 GILLESPIE "(SPR 1) Add esi_dr_msg.h,esi_dr_err.h"*/
/* *3    12-JUN-1990 10:12:35 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit "*/
/* *2    13-JUL-1989 13:50:31 GORDON "(SPR 0) added call to wm_set_mouse_menu"*/
/* *1    19-JUN-1989 12:47:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_PICK_LINE.C*/
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

NAME:           dr_pick_line

DESCRIPTION:    Graphic pick a line element -- set current line to
                point to the picked line element, and current line
                segment to the picked segment.


     Input:     dr_data - The standard structure

     Output:    dr_data - updated

     Status:    SUCCESS     - if current line updated.
                DR_CANCEL   - if user cancelled the pick.
                FAIL        - if no matching line found.

******************************************************************************/

#include    "esi_c_lib.h"
#include    "esi_dr.h"
#include    "esi_lu.h"
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
#include "esi_mg.h"
#endif

#include "esi_dr_msg.h"
#include "esi_dr_err.h"

#if USE_PROTOTYPES
publicdef INT dr_pick_line (DR_DATA *dr_data)
#else
publicdef INT dr_pick_line (dr_data)
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
    OBJECT_STRUCTURE *this_object;
    INT item_id;
    INT button;
    INT temp_seg, old_color;
    INT highlight_color;
    FLOAT wx, wy;
    GO_USER_STRUCT object;

    lu_text_to_index ("COLOR", &highlight_color, "HIGHLIGHT");
#if 0
    status = wmgdtec (dr_data->current_object, GDETEC);
#endif

    strcpy (prompt, mg_message (DR_PICK_LINE));
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
            status = dr_slt_go_element_of_obj (dr_data, &wx, &wy, &object);
            if ((status != SUCCESS) || (object.if_text == TRUE))
                {
                not_found = TRUE;
                }
            }
        if (not_found && !cancel)
            strcpy (prompt, mg_message (DR_PICK_LINE_CLOSER));
        }
#if 0
    status = wmgdtec (dr_data->current_object, GUNDET);
#endif

    /* ------------------------------------------- */
    /*  Locate the line element matching item_id   */
    /* ------------------------------------------- */

    if (!cancel)
        {
        dr_data->pick_echo = TRUE;
        item_id = object.elem_id;
        temp_seg = object.segment_id;
        status = dr_find_element (dr_data, item_id, temp_seg, DR_LINE_OBJECT);
        if (status == SUCCESS)
            {
            dr_set_poly_pref (dr_data, dr_data->current_line->polygon_flag,
                              (dr_data->current_line->polygon_fill_style ==
                               SOLID));
                            
            /* ---------------------- */
            /* Save the current color */
            /* ---------------------- */
            old_color = dr_data->current_line->line.color;
            
            /* ----------------------------------- */
            /* Draw picked line in highlight color */
            /* ----------------------------------- */
            dr_data->current_line->line.color = highlight_color;
            dr_data->current_line->polygon_fill_color = highlight_color;
            mm_draw_line (dr_data->current_line, FALSE);
            
            /* --------------------------------- */
            /* Reset line color to current color */
            /* --------------------------------- */
            dr_data->current_line->line.color = old_color;
            dr_data->current_line->polygon_fill_color = old_color;
            }
        }
    else
        {
        status = DR_CANCEL;
        }

    /*  Kill status box  */

    am_message (AM_DIALOG, NULL);

    return status;
    }

