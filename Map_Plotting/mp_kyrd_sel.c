/*  DEC/CMS REPLACEMENT HISTORY, Element MP_KYRD_SEL.C*/
/*  *4    17-AUG-1990 22:06:38 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    24-APR-1990 11:45:57 JULIAN "(SPR 0) fix error when pick keyword twice"*/
/*  *2    26-JUN-1989 10:56:50 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:51 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_KYRD_SEL.C*/
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

#include    "esi_gl_defs.h"
#include    "esi_mp.h"
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "md_mapdef.rh"
#include    "esi_md_msg.h"
#include    "esi_md_err.h"
#include    "esi_md_basis.x"        /* externals needed here */

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to do necessary processing when a keyword is
    selected from the options selector box.  This includes
    initialization for parameter selection and updating
    the state of the dialog.

Prototype:
    publicdef INT mp_keyword_selected(INT id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_keyword_selected (INT id)
#else
publicdef INT mp_keyword_selected (id)
INT id;
#endif
    {
    PROGNAME ("mp_map_server");
    MAP_STRUCTURE *mapdef;          /* Data for present map        */
    MD_STATE_WORKSPACE *md_state;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    MD_COMPILED_VERB *temp_comp_verb;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT selected;                   /* Selection indicator         */
    INT j;
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * CONFIRM THAT EXACTLY ONE SELECTION HAS BEEN MADE * */
    /* ---------------------------------------------------- */
    
    wi_query_selections (id, MD_KEY_SELECT, &j, &selected);
    if (j <= 0)
        {
        goto finished;
        }
    /* * CHECK THAT THIS KEYWORD NOT ALREADY PICKED * */
    /* ---------------------------------------------- */
    
    md_state->active_key_ndx = selected - 1;
    if (md_state->crnt_pref_verb_start)
        {
        j = display->compiled_row[md_state->crnt_pref_verb_start - 1];
        temp_comp_verb = compiled->verb_ptr[j];
        if (temp_comp_verb->key_used[md_state->active_key_ndx] EQUALS 1)
            {
            wi_beep (1);
            wi_set_dialog_text (id, MD_OVER_MSG, mg_message (MD_KEY_IN_OVERLAY));
            md_state->button_status[1] = OFF;
            md_state->button_status[2] = OFF;
            wi_set_default_sel (id, MD_PARM_SELECT, 0);
            wi_set_default_sel (id, MD_KEY_SELECT, 0);
            goto finished;
            }
        }
    /* * FILL THE PARAMETER SELECTOR BOX (IF NEEDED) * */
    /* ----------------------------------------------- */
    
    temp_verb_info = &(overlay_verbs[md_state->active_verb_ndx]);
    temp_key_info = &(temp_verb_info->key_list[md_state->active_key_ndx]);
    
    if (temp_key_info->parm_req)
        {
        j = mp_fill_parm_box (temp_key_info);
        if (j < 0)
            {
            wi_beep (1);
            wi_set_dialog_text (id, MD_OVER_MSG, mg_message (j));
            md_state->button_status[1] = OFF;
            goto finished;
            }
        }
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    md_state->button_status[0] = ON;
    md_state->button_status[2] = OFF;
    md_state->button_status[4] = OFF;
    md_state->button_status[5] = OFF;
    if (md_state->crnt_pref_verb_start EQUALS 0)
        {
        md_state->button_status[1] = OFF;
        md_state->button_status[3] = OFF;
        }
    else
        {
        md_state->button_status[1] = ON;
        md_state->button_status[3] = ON;
        }
    wi_set_default_sel (id, MD_KEY_SELECT, selected);
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_set_default_sel (id, MD_PARM_SELECT, 0);
    wi_set_default_sel (id, MD_PARM_PREFS, 0);
    
finished:
    mp_set_mapdef_buttons ();
    return SUCCESS;
    }
/* END:     */
