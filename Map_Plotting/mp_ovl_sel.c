/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OVL_SEL.C*/
/*  *3    17-AUG-1990 22:07:57 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:58:27 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:54 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OVL_SEL.C*/
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
#include    "esi_md_basis.x"
#include    "md_mapdef.rh"
#include    "esi_md_msg.h"
#include    "esi_md_err.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to process the map definition dialog after an
    overlay has been picked.  This includes setting up
    the options selector box and updating the state of
    the dialog.

Prototype:
    publicdef INT mp_overlay_selected(INT id);

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
publicdef INT mp_overlay_selected (INT id)
#else
publicdef INT mp_overlay_selected (id)
INT id;
#endif
    {
    MD_STATE_WORKSPACE *md_state;
    MD_VERB_INFO *temp_verb_info;
    INT i;
    INT selected;                   /* Selection indicator         */
    CHAR msg[81];
    
    /************************************************************************/
    
    if (NOT mpz_pending_ok ())
        {
        return FAIL;
        }
    /* * GET SELECTION * */
    /* ----------------- */
    
    wi_query_selections (id, MD_OVER_SELECT, &i, &selected);
    
    /* * UPDATE DIALOG STATUS * */
    /* ------------------------ */
    
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    md_state->active_verb_ndx = selected - 1;
    md_state->crnt_select_verb = selected;
    md_state->active_key_ndx = -1;
    md_state->crnt_pref_verb_start = 0;
    md_state->crnt_pref_verb_end = 0;
    
    md_state->button_status[0] = ON;
    md_state->button_status[1] = OFF;
    md_state->button_status[2] = OFF;
    md_state->button_status[3] = OFF;
    md_state->button_status[4] = OFF;
    md_state->button_status[5] = OFF;
    
    /* * LOAD THE KEYWORD SELECTOR * */
    /* ----------------------------- */
    
    temp_verb_info = &(overlay_verbs[md_state->active_verb_ndx]);
    if (md_state->last_key_obj EQUALS 0 OR temp_verb_info->obj_id !=
        md_state->last_key_obj)
        {
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
    wi_set_default_sel (id, MD_OVER_SELECT, selected);
    wi_set_default_sel (id, MD_OVER_PREFS, 0);
    wi_set_default_sel (id, MD_KEY_SELECT, 0);
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_set_default_sel (id, MD_PARM_SELECT, 0);
    wi_set_default_sel (id, MD_PARM_PREFS, 0);
    
    mp_set_mapdef_buttons ();
    
    return SUCCESS;
    }
/* END:     */
