/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRF_PRMSEL.C*/
/*  *3    17-AUG-1990 22:08:12 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:58:59 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:15 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_PRF_PRMSEL.C*/
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
#include    "esi_md_basis.x"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif                               

/* Function Description -----------------------------------------------------
Description:
    Routine to process the map definition dialog after a 
    preference parameter has been picked.  This includes setting up
    the options selector box and updating the state of the dialog.

Prototype:
    publicdef INT mp_pref_parm_selected(INT id);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_pref_parm_selected (INT id)
#else
publicdef INT mp_pref_parm_selected (id)
INT id;
#endif
    {
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    MD_DISPLAY *display;
    INT i, j;
    CHAR msg[81];
    
    /***************************************************************************/
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    display = mapdef->display;
    
    /* * CHECK THE SELECTED PARAMETER IS NEITHER '---' NOR BLANK * */
    /* ----------------------------------------------------------- */
    
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_query_selections (id, MD_PARM_PREFS, &i, &j);
    if ((i <= 0) OR ((j < md_state->crnt_pref_verb_start OR j >
                     md_state->crnt_pref_verb_end) AND (NOT mpz_pending_ok ())))
        {
        return FAIL;
        }
    i = j - 1;
    temp_verb_info = &(overlay_verbs[display->verb_ndx[i]]);
    temp_key_info = &(temp_verb_info->key_list[display->key_ndx[i]]);
    if ((display->key_ndx[i] EQUALS - 1) OR (NOT temp_key_info->parm_req))
        {
        wi_beep (1);
        sprintf (msg, "%s: '%s'", mg_message (MD_INVALID_VERB),
                 display->parameters[i]);
        wi_set_dialog_text (id, MD_OVER_MSG, msg);
        md_state->button_status[4] = OFF;
        md_state->button_status[5] = OFF;
        mp_set_mapdef_buttons ();
        }
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    else
        {
        md_state->active_key_ndx = -1;
        md_state->crnt_pref_key_parm = j;
        
        md_state->button_status[0] = ON;
        md_state->button_status[1] = OFF;
        md_state->button_status[2] = OFF;
        md_state->button_status[3] = ON;
        md_state->button_status[4] = (temp_key_info->key_req) ? OFF : ON;
        md_state->button_status[5] = ON;
        
        j = display->verb_ndx[i];
        if (j != md_state->active_verb_ndx)
            {
            md_state->active_verb_ndx = j;
            /* LOAD THE KEYWORD SELECTOR */
            
            temp_verb_info = &(overlay_verbs[j]);
            if (temp_verb_info->obj_id EQUALS 0)
                {
                wi_define_text_object (temp_verb_info->key_tcp,
                                       temp_verb_info->no_of_keywords,
                                       &(temp_verb_info->obj_id));
                }
            wi_set_selections_obj (id, MD_KEY_SELECT, MD_KEY_SCROLL,
                                   temp_verb_info->no_of_keywords, 1,
                                   temp_verb_info->obj_id);
            
            md_state->last_key_obj = temp_verb_info->obj_id;
            }
        /* UPDATE CURR_PREF_VERB INFO */
        
        if (md_state->crnt_pref_key_parm <
            md_state->crnt_pref_verb_start OR md_state->crnt_pref_key_parm >
            md_state->crnt_pref_verb_end)
            {
            for (j = i - 1; j >= 0; j--)
                {
                if (display->verb_ndx[j] != md_state->active_verb_ndx)
                    {
                    break;
                    }
                }
            md_state->crnt_pref_verb_start = j + 2;
            
            for (j = md_state->crnt_pref_key_parm; j < display->last_row_used; j++)
                {
                if (display->verb_ndx[j] != md_state->active_verb_ndx)
                    {
                    break;
                    }
                }
            md_state->crnt_pref_verb_end = j;
            wi_set_default_sel (id, MD_OVER_SELECT, md_state->active_verb_ndx + 1);
            wi_set_default_sel (id, MD_OVER_PREFS, md_state->crnt_pref_verb_start);
            }
        wi_set_default_sel (id, MD_KEY_SELECT, 0);
        wi_set_default_sel (id, MD_KEY_PREFS, md_state->crnt_pref_key_parm);
        wi_set_default_sel (id, MD_PARM_SELECT, 0);
        wi_set_default_sel (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm);
        
        mp_set_mapdef_buttons ();
        }
    return SUCCESS;
    }
/* END:     */
