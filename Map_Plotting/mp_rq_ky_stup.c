/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RQ_KY_STUP.C*/
/*  *3    17-AUG-1990 22:09:08 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 11:00:07 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:37 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RQ_KY_STUP.C*/
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
#include    "esi_md_basis.x"    /* externals (publicdefs) needed here */

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to setup the map definition dialog into a state
    that is ready for the given required keyword to be added
    to the definition.

Prototype:
    publicdef INT mp_req_key_setup(INT req_ndx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    req_ndx         -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_req_key_setup (INT req_ndx)
#else
publicdef INT mp_req_key_setup (req_ndx)
INT req_ndx;
#endif
    {
    PROGNAME ("mp_display_map");
    INT status;
    RESOURCE_ID *cdlg_id;
    MD_STATE_WORKSPACE *md_state;
    MD_KEY_INFO *temp_key_info;
    MD_VERB_INFO *temp_verb_info;
    INT id;
    CHAR msg[80];
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    id = cdlg_id->overlays_id;
    temp_verb_info = &(overlay_verbs[md_state->active_verb_ndx]);
    temp_key_info = &(temp_verb_info->key_list[req_ndx]);
    sprintf (msg, "%s '%s'", mg_message (MD_MISSING_REQ_KEY), temp_key_info->keyword);
    am_message (AM_ALERT, msg);
    
    /* CHECK IF NEED TO CHANGE KEYWORD SELECTOR */
    
    if (temp_verb_info->obj_id EQUALS 0)
        {
        wi_define_text_object (temp_verb_info->key_tcp, temp_verb_info->no_of_keywords,
                               &(temp_verb_info->obj_id));
        }
    wi_set_selections_obj (id, MD_KEY_SELECT, MD_KEY_SCROLL,
                           temp_verb_info->no_of_keywords, 1, temp_verb_info->obj_id);
    
    /* * FILL THE PARAMETER SELECTOR BOX (IF NEEDED) * */
    /* ----------------------------------------------- */
    
    if (temp_key_info->parm_req)
        {
        mp_fill_parm_box (temp_key_info);
        }
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    wi_set_default_sel (id, MD_KEY_SELECT, 0);
    wi_set_default_sel (id, MD_OVER_PREFS, 0);
    wi_set_default_sel (id, MD_OVER_SELECT, 0);
    wi_set_default_sel (id, MD_PARM_SELECT, 0);
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_set_default_sel (id, MD_PARM_PREFS, 0);
    
    wi_set_default_sel (id, MD_KEY_SELECT, req_ndx + 1);
    wi_set_default_sel (id, MD_OVER_PREFS, md_state->crnt_pref_verb_start);
    wi_set_default_sel (id, MD_OVER_SELECT, md_state->active_verb_ndx + 1);
    
    md_state->active_key_ndx = req_ndx;
    
    md_state->button_status[0] = OFF;
    md_state->button_status[1] = ON;
    md_state->button_status[2] = OFF;
    md_state->button_status[3] = ON;
    md_state->button_status[4] = OFF;
    md_state->button_status[5] = OFF;
    mp_set_mapdef_buttons ();
    
    return SUCCESS;
    }
/* END:     */
