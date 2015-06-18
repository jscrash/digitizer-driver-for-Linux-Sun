/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ADD_OVL.C*/
/*  *6     6-AUG-1991 14:54:09 KEE "(SPR 8764) Fix map definnition where overlay selections are not registered as users pick them*/
/* (case 2582)"*/
/*  *5    14-NOV-1990 15:41:24 MING "(SPR 6153) add mpz_pending_ok"*/
/*  *4     6-NOV-1990 11:24:32 JULIAN "(SPR 5719) fix crashes when adding more than 27 overlays"*/
/*  *3    17-AUG-1990 22:03:44 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:52:23 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:27 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ADD_OVL.C*/
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
#include    "esi_ts.h"
#include    "esi_wi.h"
#include    "md_mapdef.rh"
#include    "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Routine to add an overlay to the map definition being
    built through the map definition dialog.

Prototype:
    publicdef INT mp_add_overlay(INT id);

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
publicdef INT mp_add_overlay (INT id)
#else
publicdef INT mp_add_overlay (id)
INT id;
#endif
    {
    MAP_STRUCTURE *mapdef;          /* Data for present map        */
    MD_STATE_WORKSPACE *md_state;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT i;                          /* Counter                     */
    INT k;
    
    /****************************************************************************/
    if (NOT mpz_pending_ok ())
      {
      return FAIL;
      }
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);

    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * ADD DASHES IN PREFERENCES * */
    /* ----------------------------- */
    
    if (display->last_row_used != 0)
        {
        
        i = display->last_row_used;
        (display->last_row_used)++;
        mp_display_size_check (display->last_row_used + 1);
	display = mapdef->display;
        
        ts_fill (display->verbs[i], '-', SELECTOR_SIZE);
        wi_insert_cd_line (id, MD_OVER_PREFS, display->last_row_used,
                           display->verbs[i]);
        display->verb_ndx[i] = -1;
        
        ts_fill (display->keywords[i], '-', SELECTOR_SIZE);
        wi_insert_cd_line (id, MD_KEY_PREFS, display->last_row_used,
                           display->keywords[i]);
        display->key_ndx[i] = -1;
        
        display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL, SELECTOR_SIZE + 1);
        ts_fill (display->parameters[i], '-', SELECTOR_SIZE);
        wi_insert_cd_line (id, MD_PARM_PREFS, display->last_row_used,
                           display->parameters[i]);
        
        display->compiled_row[i] = -1;
        }
    /* * ADD THE NEW VERB * */
    /* -------------------- */
    
    i = display->last_row_used;
    (display->last_row_used)++;
    temp_verb_info = &(overlay_verbs[md_state->active_verb_ndx]);
    
    /* ADJUST DISPLAY */
    
    mp_display_size_check (display->last_row_used + 1);
    display = mapdef->display;
    
    wi_insert_cd_line (id, MD_OVER_PREFS, display->last_row_used,
                       temp_verb_info->verb);
    strcpy (display->verbs[i], temp_verb_info->verb);
    display->verb_ndx[i] = md_state->active_verb_ndx;
    
    ts_fill (display->keywords[i], ' ', SELECTOR_SIZE);
    wi_insert_cd_line (id, MD_KEY_PREFS, display->last_row_used, display->keywords[i]);
    display->key_ndx[i] = -1;
    
    display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL, SELECTOR_SIZE + 1);
    ts_fill (display->parameters[i], ' ', SELECTOR_SIZE);
    wi_insert_cd_line (id, MD_PARM_PREFS, display->last_row_used,
                       display->parameters[i]);
    
    k = compiled->last_row_used + 1;
    display->compiled_row[i] = k;
    
    /* ADJUST THE COMPILED MAPDEF STRUCT */
    
    compiled->last_row_used = k;
    mp_compiled_size_check (k);
    
    compiled->verb_code[k] = temp_verb_info->verb_code;
    compiled->overlay_wm_id[k] = 0;
    compiled->verb_wm_id[k] = 0;
    
    mp_init_comp_verb (&(compiled->verb_ptr[k]), temp_verb_info);
    
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    if (md_state->button_status[1] EQUALS ON)
        {
        md_state->active_key_ndx = -1;
        md_state->button_status[1] = OFF;
        wi_set_default_sel (id, MD_KEY_SELECT, 0);
        }
    else if (md_state->active_key_ndx != -1)
        {
        md_state->button_status[1] = ON;
        }
    md_state->button_status[3] = ON;
    md_state->button_status[4] = OFF;
    md_state->button_status[5] = OFF;
    mp_set_mapdef_buttons ();
    
    md_state->verb_pending = TRUE;
    md_state->crnt_pref_verb_start = display->last_row_used;
    md_state->crnt_pref_verb_end = display->last_row_used;
    wi_set_default_sel (id, MD_OVER_PREFS, 0); /* clear selection 
						  just in case */
    wi_set_default_sel (id, MD_OVER_PREFS, display->last_row_used);
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_set_default_sel (id, MD_PARM_PREFS, 0);
    mapdef->map_displayed = FALSE;
    mapdef->change_flag = TRUE;
    
    return SUCCESS;
    }
/* END:     */
