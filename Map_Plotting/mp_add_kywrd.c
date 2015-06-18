/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ADD_KYWRD.C*/
/*  *5     6-NOV-1990 11:24:24 JULIAN "(SPR 5719) fix crashes when adding more than 27 overlays"*/
/*  *4    17-AUG-1990 22:03:38 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    20-OCT-1989 11:34:40 CONROY "(SPR 5030) remove include file esi_md_tlu"*/
/*  *2    26-JUN-1989 10:52:06 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:23 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ADD_KYWRD.C*/
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
    Routine to add a keyword to the map definition 
    being built through the map definition dialog.

Prototype:
    publicdef INT mp_add_keyword(INT id);

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
publicdef INT mp_add_keyword (INT id)
#else
publicdef INT mp_add_keyword (id)
INT id;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;          /* Data for present map        */
    MD_STATE_WORKSPACE *md_state;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    MD_COMPILED_VERB *temp_comp_verb;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_PARM_STRUCT *parm_struct;    /* Parms list pointer          */
    INT i, j, k;                    /* Counter                     */
    CHAR parm_string[MD_MAX_PARM_STRING];
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * INITIALIZE TEMP STRUCTURES * */
    /* ------------------------------ */
    
    i = md_state->crnt_pref_verb_end - 1;
    temp_verb_info = &(overlay_verbs[md_state->active_verb_ndx]);
    temp_key_info = &(temp_verb_info->key_list[md_state->active_key_ndx]);
    
    /* * GET PARAMETER IF NEEDED * */
    /* --------------------------- */
    
    status = SUCCESS;
    if (temp_key_info->parm_req)
        {
        parm_struct = NULL_PARM;
        status = mp_get_parameter (FALSE, temp_key_info, parm_string, &parm_struct);
        }
    /* * UPDATE MAPDEF DISPLAY AND COMPILED STRUCTURES * */
    /* ------------------------------------------------- */
    
    if (status EQUALS SUCCESS)
        {
        j = display->compiled_row[md_state->crnt_pref_verb_start - 1];
        temp_comp_verb = compiled->verb_ptr[j];
        
        /* UPDATE KEYWORD USED FLAG */
        
        if (temp_comp_verb->key_used[md_state->active_key_ndx] EQUALS 2)
            {
            md_free_parms (temp_comp_verb->parm_ptr[md_state->active_key_ndx]);
            }
        temp_comp_verb->key_used[md_state->active_key_ndx] = 1;
        
        /* UPDATE FOR FIRST KEYWORD IN OVERLAY */
        
        if (display->key_ndx[i] EQUALS - 1)
            {
            wi_delete_cd_line (id, MD_KEY_PREFS, md_state->crnt_pref_verb_end);
            wi_insert_cd_line (id, MD_KEY_PREFS, md_state->crnt_pref_verb_end,
                               temp_key_info->keyword);
            
            strcpy (display->keywords[i], temp_key_info->keyword);
            display->key_ndx[i] = md_state->active_key_ndx;
            
            if (temp_key_info->parm_req)
                {
                wi_delete_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_verb_end);
                wi_insert_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_verb_end,
                                   parm_string);
                
                if (strlen (display->parameters[i]) < strlen (parm_string))
                    {
                    display->parameters[i] = (CHAR *)
                                             am_reallocate (display->parameters[i],
                                                            strlen (parm_string) + 1);
                    }
                strcpy (display->parameters[i], parm_string);
                temp_comp_verb->parm_ptr[md_state->active_key_ndx] = parm_struct;
                }
            }
        /* UPDATE FOR SUBSEQUENT KEYWORDS */
        
        else
            {
            i = display->last_row_used;
            (display->last_row_used)++;
            mp_display_size_check (display->last_row_used + 1);
	    display = mapdef->display;
            
            /* PUSH DOWN DISPLAY RECORDS */
            
            if (i != md_state->crnt_pref_verb_end)
                {
                for (k = i; k >= md_state->crnt_pref_verb_end; k--)
                    {
                    j = k - 1;
                    strcpy (display->verbs[k], display->verbs[j]);
                    strcpy (display->keywords[k], display->keywords[j]);
                    display->parameters[k] = display->parameters[j];
                    display->overlay_displayed[k] = display->overlay_displayed[j];
                    display->verb_ndx[k] = display->verb_ndx[j];
                    display->key_ndx[k] = display->key_ndx[j];
                    display->compiled_row[k] = display->compiled_row[j];
                    }
                i = md_state->crnt_pref_verb_end;
                }
            /* ADD NEW KEY TO DISPLAY */
            
            (md_state->crnt_pref_verb_end)++;
            
            ts_fill (display->verbs[i], ' ', SELECTOR_SIZE);
            wi_insert_cd_line (id, MD_OVER_PREFS, md_state->crnt_pref_verb_end,
                               display->verbs[i]);
            display->verb_ndx[i] = display->verb_ndx[i - 1];
            
            wi_insert_cd_line (id, MD_KEY_PREFS, md_state->crnt_pref_verb_end,
                               temp_key_info->keyword);
            strcpy (display->keywords[i], temp_key_info->keyword);
            display->key_ndx[i] = md_state->active_key_ndx;
            
            if (temp_key_info->parm_req)
                {
                wi_insert_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_verb_end,
                                   parm_string);
                display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL,
                                                              strlen (parm_string) +
                                                              1);
                strcpy (display->parameters[i], parm_string);
                temp_comp_verb->parm_ptr[md_state->active_key_ndx] = parm_struct;
                }
            else
                {
                display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL,
                                                              SELECTOR_SIZE + 1);
                ts_fill (display->parameters[i], ' ', SELECTOR_SIZE);
                wi_insert_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_verb_end,
                                   display->parameters[i]);
                }
            display->compiled_row[i] = display->compiled_row[i - 1];
            }
        display->overlay_displayed[md_state->crnt_pref_verb_start - 1] = FALSE;
        md_state->verb_pending = TRUE;
        mapdef->change_flag = TRUE;
        mapdef->map_displayed = FALSE;
        }
    /* * UPDATE THE DIALOG STATE * */
    /* --------------------------- */
    
    md_state->active_key_ndx = -1;
    md_state->button_status[1] = OFF;
    md_state->button_status[2] = OFF;
    mp_set_mapdef_buttons ();
    
    wi_set_default_sel (id, MD_OVER_SELECT, 0);
    wi_set_default_sel (id, MD_OVER_PREFS, 0);
    wi_set_default_sel (id, MD_OVER_PREFS, md_state->crnt_pref_verb_start);
    wi_set_default_sel (id, MD_OVER_SELECT, md_state->active_verb_ndx + 1);
    
    wi_set_default_sel (id, MD_PARM_SELECT, 0);
    wi_set_default_sel (id, MD_KEY_SELECT, 0);
    
    return SUCCESS;
    }
/* END:     */
