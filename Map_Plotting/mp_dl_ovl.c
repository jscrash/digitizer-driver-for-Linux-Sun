/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DL_OVL.C*/
/*  *3    17-AUG-1990 22:04:16 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:53:49 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:00 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DL_OVL.C*/
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
#include    "esi_asp_yesno.h"
#include    "esi_wi.h"
#include    "esi_ts.h"
#include    "esi_md_defs.h"
#include    "esi_md_msg.h"
#include    "md_mapdef.rh"

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Routine to delete an overlay from the map definition 
    being built through the map definition dialog.

Prototype:
    publicdef INT mp_delete_overlay(INT id);

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
publicdef INT mp_delete_overlay (INT id)
#else
publicdef INT mp_delete_overlay (id)
INT id;
#endif
    {
    MAP_STRUCTURE *mapdef;          /* Data for present map        */
    MD_STATE_WORKSPACE *md_state;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    INT start_delete;
    INT end_delete;
    INT i, j, k;                    /* Counter                     */
    ASP_YESNO_WORKSPACE yesno;
    CHAR msg[81];
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    mapdef->change_flag = TRUE;
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * CONFIRM DELETION * */
    /* -------------------- */
    
    yesno.cancel = FALSE;
    i = md_state->crnt_pref_verb_start - 1;
    sprintf (msg, mg_message (MD_DELETE_OVER_MSG), ts_snowhite (display->verbs[i]));
    strcat (msg, "...");
    strcpy (yesno.question, msg);
    asp_yesno (&yesno);
    
    if (yesno.answer)
        {
        /* ADJUST MAC DISPLAY */
        
        if (md_state->crnt_pref_verb_end EQUALS display->last_row_used)
            {
            start_delete = md_state->crnt_pref_verb_end;
            end_delete = MAX (1, md_state->crnt_pref_verb_start - 1);
            }
        else
            {
            start_delete = md_state->crnt_pref_verb_end + 1;
            end_delete = md_state->crnt_pref_verb_start;
            }
        for (i = start_delete; i >= end_delete; i--)
            {
            wi_delete_cd_line (id, MD_OVER_PREFS, i);
            wi_delete_cd_line (id, MD_KEY_PREFS, i);
            wi_delete_cd_line (id, MD_PARM_PREFS, i);
            j = i - 1;
            am_free (display->parameters[j]);
            display->parameters[j] = NULL;
            }
        wi_set_selections (id, MD_KEY_SELECT, MD_KEY_SCROLL, 0, 0, (CHAR **)0);
        md_state->last_key_obj = 0;
        
        /* ADJUST WORKSTATION DISPLAY IF NEEDED */
        
        i = display->compiled_row[md_state->crnt_pref_verb_start - 1];
        if (compiled->overlay_wm_id[i] > 0)
            {
            mp_erase_overlay (i);
            }
        /* ADJUST DEFINED FILE STRUCTURES */
        
        mpz_delete_file_check (TRUE);
        
        /* ADJUST DISPLAY STRUCTURE */
        
        verbstruc = compiled->verb_ptr[i];
        for (j = 0; j < verbstruc->nrows; j++)
            {
            md_free_parms (verbstruc->parm_ptr[j]);
            }
        am_free (verbstruc);
        /* CMD BY JJC, set the pointer to null. */
        compiled->verb_ptr[i] = (MD_COMPILED_VERB *)0;
        
        i = start_delete - end_delete + 1;
        for (j = md_state->crnt_pref_verb_end + 1; j < display->last_row_used; j++)
            {
            k = j - i;
            strcpy (display->verbs[k], display->verbs[j]);
            strcpy (display->keywords[k], display->keywords[j]);
            display->overlay_displayed[k] = display->overlay_displayed[j];
            display->parameters[k] = display->parameters[j];
            display->verb_ndx[k] = display->verb_ndx[j];
            display->key_ndx[k] = display->key_ndx[j];
            display->compiled_row[k] = display->compiled_row[j];
            }
        for (j = display->last_row_used - i; j < display->last_row_used; j++)
            {
            display->parameters[j] = NULL;
            }
        display->last_row_used -= i;
        
        /* * UPDATE DIALOG STATUS * */
        /* ------------------------ */
        
        mapdef->change_flag = TRUE;
        mapdef->map_displayed = FALSE;
        md_state->verb_pending = FALSE;
        
        md_state->active_verb_ndx = -1;
        md_state->crnt_pref_verb_start = 0;
        md_state->crnt_pref_verb_end = 0;
        
        md_state->button_status[0] = OFF;
        md_state->button_status[1] = OFF;
        md_state->button_status[2] = OFF;
        md_state->button_status[3] = OFF;
        md_state->button_status[4] = OFF;
        md_state->button_status[5] = OFF;
        mp_set_mapdef_buttons ();
        
        wi_set_default_sel (id, MD_OVER_SELECT, 0);
        wi_set_default_sel (id, MD_KEY_SELECT, 0);
        wi_set_default_sel (id, MD_PARM_SELECT, 0);
        wi_set_default_sel (id, MD_OVER_PREFS, 0);
        wi_set_default_sel (id, MD_KEY_PREFS, 0);
        wi_set_default_sel (id, MD_PARM_PREFS, 0);
        }
    return SUCCESS;
    }
/* END:     */
