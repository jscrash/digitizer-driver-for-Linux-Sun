/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DL_KYWRD.C*/
/*  *3    17-AUG-1990 22:04:06 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:53:22 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:56 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DL_KYWRD.C*/
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

#include    "esi_c_lib.h"
#include    "esi_mp.h"
#include    "esi_mg.h"
#include    "esi_am.h"
#include    "esi_asp_yesno.h"
#include    "esi_wi.h"
#include    "esi_ts.h"
#include    "md_mapdef.rh"
#include    "esi_md_err.h"
#include    "esi_md_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to delete a keyword options from the map
    definitions being built through the map definition dialog.

Prototype:
    publicdef INT mp_delete_keyword(INT id);

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
publicdef INT mp_delete_keyword (INT id)
#else
publicdef INT mp_delete_keyword (id)
INT id;
#endif
    {
    MAP_STRUCTURE *mapdef;          /* Data for present map        */
    MD_STATE_WORKSPACE *md_state;
    MD_COMPILED_VERB *temp_comp_verb;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT nitems;
    INT i, j, k;                    /* Counter                     */
    ASP_YESNO_WORKSPACE yesno;
    CHAR msg[81];
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * CONFIRM DELETION * */
    /* -------------------- */
    
    i = md_state->crnt_pref_key_parm - 1;
    yesno.cancel = FALSE;
    sprintf (msg, mg_message (MD_DELETE_OVER_MSG), ts_snowhite (display->keywords[i]));
    strcat (msg, "...");
    strcpy (yesno.question, msg);
    asp_yesno (&yesno);
    
    if (yesno.answer)
        {
        /* ADJUST DEFINED FILES STRUCTURES */
        
        mpz_delete_file_check (FALSE);
        
        /* ADJUST COMPILED STRUCTURE */
        
        j = display->compiled_row[i];
        temp_comp_verb = compiled->verb_ptr[j];
        
        k = display->key_ndx[i];
        temp_comp_verb->key_used[k] = 0;
        if (temp_comp_verb->parm_ptr[k] != NULL_PARM)
            {
            md_free_parms (temp_comp_verb->parm_ptr[k]);
            temp_comp_verb->parm_ptr[k] = NULL_PARM;
            }
        /* ADJUST MAC DISPLAY AND DISPLAY STRUCT */
        
        wi_delete_cd_line (id, MD_KEY_PREFS, md_state->crnt_pref_key_parm);
        wi_delete_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm);
        
        if (md_state->crnt_pref_verb_start EQUALS md_state->crnt_pref_verb_end)
            {
            
            ts_fill (display->keywords[i], ' ', SELECTOR_SIZE);
            wi_insert_cd_line (id, MD_KEY_PREFS, md_state->crnt_pref_key_parm,
                               display->keywords[i]);
            display->key_ndx[i] = -1;
            
            am_free (display->parameters[i]);
            display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL,
                                                          SELECTOR_SIZE + 1);
            ts_fill (display->parameters[i], ' ', SELECTOR_SIZE);
            wi_insert_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm,
                               display->parameters[i]);
            }
        else
            {
            wi_delete_cd_line (id, MD_OVER_PREFS, md_state->crnt_pref_verb_end);
            am_free (display->parameters[i]);
            (md_state->crnt_pref_verb_end)--;
            (display->last_row_used)--;
            
            j = i + 1;
            strcpy (display->keywords[i], display->keywords[j]);
            display->overlay_displayed[i] = display->overlay_displayed[j];
            display->parameters[i] = display->parameters[j];
            display->verb_ndx[i] = display->verb_ndx[j];
            display->key_ndx[i] = display->key_ndx[j];
            display->compiled_row[i] = display->compiled_row[j];
            
            for (k = i + 1; k < display->last_row_used; k++)
                {
                j = k + 1;
                strcpy (display->verbs[k], display->verbs[j]);
                strcpy (display->keywords[k], display->keywords[j]);
                display->overlay_displayed[k] = display->overlay_displayed[j];
                display->parameters[k] = display->parameters[j];
                display->verb_ndx[k] = display->verb_ndx[j];
                display->key_ndx[k] = display->key_ndx[j];
                display->compiled_row[k] = display->compiled_row[j];
                }
            display->parameters[display->last_row_used] = NULL;
            }
        display->overlay_displayed[md_state->crnt_pref_verb_start - 1] = FALSE;
        
        /* * UPDATE DIALOG STATUS * */
        /* ------------------------ */
        
        mapdef->map_displayed = FALSE;
        mapdef->change_flag = TRUE;
        
        wi_set_default_sel (id, MD_KEY_PREFS, 0);
        wi_set_default_sel (id, MD_PARM_PREFS, 0);
        
        md_state->button_status[4] = OFF;
        md_state->button_status[5] = OFF;
        mp_set_mapdef_buttons ();
        }
finished:
    return SUCCESS;
    }
/* END:     */
