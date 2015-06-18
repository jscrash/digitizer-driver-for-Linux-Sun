/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ED_PRM.C*/
/*  *3    17-AUG-1990 22:05:23 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:55:21 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:43 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ED_PRM.C*/
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
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "md_mapdef.rh"
#include    "esi_md_msg.h"
#include    "esi_md_err.h"
#include    "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Routine to edit parameters already in the map definition.
    Sets up correct editing dialog dependent on type of
    parameter.

Prototype:
    publicdef INT mp_edit_parameter(INT id);

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
publicdef INT mp_edit_parameter (INT id)
#else
publicdef INT mp_edit_parameter (id)
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
    INT nitems;                     /* Counter                     */
    INT i, j, k;                    /* Counter                     */
    INT verb_ndx, key_ndx;
    CHAR parm_string[MD_MAX_PARM_STRING];
    
    /* * OPEN THE WORKSPACE * */
    /* ---------------------- */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    /* * FILL THE SELECTOR BOX IF NECESSARY * */
    /* -------------------------------------- */
    
    i = md_state->crnt_pref_key_parm - 1;
    temp_verb_info = &(overlay_verbs[display->verb_ndx[i]]);
    temp_key_info = &(temp_verb_info->key_list[display->key_ndx[i]]);
    status = mp_fill_parm_box (temp_key_info);
    if (status > 0)
        {
        strcpy (parm_string, display->parameters[i]);
        }
    /* * EDIT PARAMETER * */
    /* ------------------ */
    
    /* SET THE INDICES */
    verb_ndx = md_state->active_verb_ndx;
    key_ndx = md_state->active_key_ndx;
    md_state->active_verb_ndx = display->verb_ndx[i];
    md_state->active_key_ndx = display->key_ndx[i];
    
    parm_struct = NULL_PARM;
    status = mp_get_parameter (TRUE, temp_key_info, parm_string, &parm_struct);
    if (status EQUALS SUCCESS)
        {
        /* ADJUST MAC DISPLAY */
        
        wi_delete_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm);
        wi_insert_cd_line (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm,
                           parm_string);
        wi_set_default_sel (id, MD_PARM_PREFS, md_state->crnt_pref_key_parm);
        
        /* ADJUST COMPILED STRUCTURE */
        
        j = display->compiled_row[i];
        temp_comp_verb = compiled->verb_ptr[j];
        
        k = display->key_ndx[i];
        md_free_parms (temp_comp_verb->parm_ptr[k]);
        temp_comp_verb->parm_ptr[k] = parm_struct;
        
        am_free (display->parameters[i]);
        display->parameters[i] = (CHAR *)am_allocate (AM_GLOBAL,
                                                      strlen (parm_string) + 1);
        strcpy (display->parameters[i], parm_string);
        
        display->overlay_displayed[md_state->crnt_pref_verb_start - 1] = FALSE;
        mapdef->map_displayed = FALSE;
        mapdef->change_flag = TRUE;
        
        if ((temp_verb_info->verb_code EQUALS MP_CONTOURS OR temp_verb_info->
             verb_code EQUALS MP_PERSPEC OR temp_verb_info->
             verb_code EQUALS MP_GRID_DSP) AND (temp_key_info->
                                               val_code EQUALS FILER$ OR temp_key_info->
                                               val_code EQUALS FILEW$))
            {
            md_state->verb_pending = TRUE;
            }
        }
    /* * UPDATE DIALOG STATUS * */
    /* ------------------------ */
    
    /* RESET ACTIVE INDICES (NECESSARY ??) */
    
    md_state->active_verb_ndx = verb_ndx;
    md_state->active_key_ndx = key_ndx;
    
    mp_set_mapdef_buttons ();
    
finished:
    
    return SUCCESS;
    }
/* END:     */
