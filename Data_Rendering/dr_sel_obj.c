/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_OBJ.C*/
/* *5    13-OCT-1990 01:07:14 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*  3B1  12-OCT-1990 16:38:29 GILLESPIE "Merge Ernie Deltas"*/
/* *4    14-AUG-1990 21:56:13 GILLESPIE "(SPR 2001) Use LINE_ATTRIBUTEs"*/
/* *3    18-JUL-1990 14:03:24 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *2    12-JUN-1990 10:12:46 SCD "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit "*/
/* *1    19-JUN-1989 12:48:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SEL_OBJ.C*/
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


/* DEC/CMS REPLACEMENT HISTORY, Element DR_SELECT_OBJECT.C*/
/* *7    31-JAN-1989 13:55:30 JAMES "(SPR 5002) do not change the object type, if the user clicks CANCEL button."*/
/* *6    16-JAN-1989 10:42:14 JAMES "(SPR 5002) To finish the software picking task."*/
/* *5    17-DEC-1988 09:53:19 GILLESPIE "(SPR 4981) Unisource"*/
/* *4    13-JUL-1987 18:31:08 SCD "Suppress duplicate am_message on error in picking"*/
/* *3    12-JUL-1987 16:29:04 SCD "SPR fixes"*/
/* *2     9-JUL-1987 10:54:12 SCD "Fix detectability synchronization"*/
/* *1     7-JUL-1987 10:40:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_SELECT_OBJECT.C*/
/******************************************************************************

NAME:          dr_select_object

DESCRIPTION:   Get an object


     Input:    dr_data - The standard data structure

     Output:   dr_data - updated

     Status:   SUCCESS;

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_lu.h"
#include    "esi_wi.h"
#include    "esi_wm.h"
#ifndef ESI_MG_H
#include    "esi_mg.h"
#endif
#include    "esi_gks.h"

#include "esi_dr_msg.h"
/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

#define null_string ""

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

#if USE_PROTOTYPES
publicdef INT dr_select_object (DR_DATA *dr_data)
#else
publicdef INT dr_select_object (dr_data)
DR_DATA *dr_data;
#endif
    {
    INT status = 0;
    BOOL cancel;
    CHAR msg[80];
    BOOL ifyes;
    OBJECT_STRUCTURE *obj;
    LINE *line;
    TEXT *text;
    COLOR_INDEX old_color, highlight_color;	
    INT temp_current_object, temp_current_object_key;

    /* ------------------------------------------- */
    /* Query whether to save or release the object */
    /* ------------------------------------------- */

    if ((dr_data->object_changed) && (dr_data->current_object_key > 0))
        {
        sprintf (msg, mg_message (DR_SAVE_CHANGES),
                 dr_data->current_object_key);
        wiyesno (msg, null_string, &ifyes, &status);
        if (ifyes)
            status = dr_save_object (dr_data);
        else
            status = dr_release_object (dr_data);
        }
    else if (dr_data->current_object_key > 0)
        {
        status = dr_release_object (dr_data);
        }

    if (status != SUCCESS)
        return status;

    /* ----------- */
    /* Pick object */
    /* ----------- */

    if (status == SUCCESS)
        status = dr_pick_object (dr_data);

    /* -------------------- */
    /* Remove detectability */
    /* -------------------- */

    if (status == SUCCESS)
        {
        /* ----------------------- */
        /* Change the object group */
        /* ----------------------- */

        temp_current_object = dr_data->current_object;
        temp_current_object_key = dr_data->current_object_key;
        dr_chg_obj_type_by_id (dr_data->current_object_key, dr_data);
        dr_data->current_object = temp_current_object;
        dr_data->current_object_key = temp_current_object_key;
        wmgopen (dr_data->current_object);

        /* ----------- */
        /* Load object */
        /* ----------- */

        status = dr_load_object (dr_data);
        
        /* ---------------------------------------------------- */
        /* redraw line, then text elements with highlight color */
        /* ---------------------------------------------------- */
        
        lu_text_to_index ("COLOR", &highlight_color, "HIGHLIGHT");

        for (obj = dr_data->line_ptr; obj != (OBJECT_STRUCTURE *)0;
             obj = obj->next_object)
            {
            line = obj->object_ptr.current_line;
            old_color = line->line.color;
            line->line.color = highlight_color;
            line->polygon_fill_color = highlight_color;
            mm_draw_line (line, FALSE);
            line->line.color = old_color;
            line->polygon_fill_color = old_color;
            }
            
        for (obj = dr_data->text_ptr; obj != (OBJECT_STRUCTURE *)0;
             obj = obj->next_object)
            {
            text = obj->object_ptr.current_text;
            old_color = text->text_color;
            text->text_color = highlight_color;
            mm_draw_text (text, FALSE);
            text->text_color = old_color;
            }
        }


    return status;
    }
