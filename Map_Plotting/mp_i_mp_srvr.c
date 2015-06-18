/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_MP_SRVR.C*/
/*  *5    15-OCT-1990 13:46:31 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*   3B1  12-OCT-1990 17:16:48 GILLESPIE "Merge Ernie Deltas"*/
/*  *4    17-AUG-1990 22:06:22 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    10-OCT-1989 14:20:22 GORDON "(SPR 4999) Mods for new lookup method"*/
/*  *2    26-JUN-1989 10:56:34 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:38 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_I_MP_SRVR.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Exploration Systems, Inc.                                               */
/*          579 Market Street                                                       */
/*          San Francisco, CA  USA 94105                                            */
/*          (415) 974-1600                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include    "esi_gl_defs.h"
#include    "esi_mp.h"
#include    "esi_md_basis.x"        /* externals needed here */
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "md_mapdef.rh"
#include    "mp_menus.rh"

#define IDENT "mp_init_map_server"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Initializes the main mapping dialog server.                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mp_init_map_server();                                           */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mp_init_map_server (void)
#else
publicdef INT mp_init_map_server ()
#endif
    {
    static INT over_tcp_obj_id = 0;
    INT status;
    RESOURCE_ID *cdlg_id;
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    INT i;
    INT id;
    MAP_NAME title;
    BOOL on_off;
    MENU_ID *menus;
    
    /* * OPEN WORKSPACES *                                                          */
    /* -------------------                                                          */
    
    am_open_workspace ("MENU", AM_APPLICATION, (VOIDPTR *)&menus);
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *)&cdlg_id);
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&mapdef);
    
    /* * CHECK THE SETUP PARAMETERS *                                               */
    /* ------------------------------                                               */
    
    mapdef->setup_ok = (mp_check_setup () EQUALS SUCCESS);
    mp_set_current_map (mapdef);
    
    /* * DEFINE THE MAPDEF CREATION DIALOG *                                        */
    /* -------------------------------------                                        */
    
    if (cdlg_id->overlays_id EQUALS 0)
        {
        am_define_resource (AM_DIALOG, "MD_MAPDEF", MD_OVERLAYS, mp_map_server,
                            &cdlg_id->overlays_id, NULL);
        
        status = am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *)&md_state);
        if (status != SUCCESS)
            {
            am_define_workspace ("MAPDEF_STATE", AM_GLOBAL, sizeof(MD_STATE_WORKSPACE),
                                 (VOIDPTR *)&md_state);
            }
        wi_define_text_object (overlay_tcp, MD_MAX_OVERLAY_TYPES, &over_tcp_obj_id);
        }
    else
        {
        wi_clear (cdlg_id->overlays_id);
        am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *)&md_state);
        }
    id = cdlg_id->overlays_id;
    
    /* * SET THE DIALOG TITLE *                                                     */
    /* ------------------------                                                     */
    
    mp_init_mapdef_buttons ();
    sprintf (title, "MAP #%d", mapdef->map_number);
    wi_set_dialog_text (id, MD_MAP_NUMBER, title);
    wi_set_dialog_text (id, MD_MAP_NAME, mapdef->map_name);
    wi_activate (id, MD_MAP_NAME, 0);
    
    /* * FILL THE OVERLAY VERB SELECTOR BOX *                                       */
    /* --------------------------------------                                       */
    
    wi_set_selections_obj (id, MD_OVER_SELECT, MD_OVER_SCROLL, MD_MAX_OVERLAY_TYPES, 1,
                           over_tcp_obj_id);
    
    /* * FILL THE PREFERENCES SELECTOR BOXES *                                      */
    /* ---------------------------------------                                      */
    
    if (mapdef->display->last_row_used > 0)
        {
        for (i = 0; i < mapdef->display->last_row_used; i++)
            {
            wi_insert_cd_line (id, MD_OVER_PREFS, i + 1, mapdef->display->verbs[i]);
            wi_insert_cd_line (id, MD_KEY_PREFS, i + 1, mapdef->display->keywords[i]);
            wi_insert_cd_line (id, MD_PARM_PREFS, i + 1,
                               mapdef->display->parameters[i]);
            }
        }
    wi_set_selection_limit (id, MD_OVER_PREFS, 1);
    wi_set_selection_limit (id, MD_KEY_PREFS, 1);
    wi_set_selection_limit (id, MD_PARM_PREFS, 1);
    
    /* * SET NO DEFAULT SELECTIONS *                                                */
    /* -----------------------------                                                */
    
    wi_set_default_sel (id, MD_OVER_SELECT, 0);
    wi_set_default_sel (id, MD_KEY_SELECT, 0);
    wi_set_default_sel (id, MD_PARM_SELECT, 0);
    wi_set_default_sel (id, MD_OVER_PREFS, 0);
    wi_set_default_sel (id, MD_KEY_PREFS, 0);
    wi_set_default_sel (id, MD_PARM_PREFS, 0);
    
    /* * ACTIVATE THE AUTO-SELECT FIELDS *                                          */
    /* -----------------------------------                                          */
    
    wi_activate (id, MD_OVER_SELECT, ON);
    wi_activate (id, MD_KEY_SELECT, ON);
    wi_activate (id, MD_OVER_PREFS, ON);
    wi_activate (id, MD_KEY_PREFS, ON);
    wi_activate (id, MD_PARM_PREFS, ON);
    
    /* * DEFINE THE DIALOG STATE *                                                  */
    /* ---------------------------                                                  */
    
    for (i = 0; i < 6; i++)
        {
        md_state->button_status[i] = OFF;
        }
    mp_set_mapdef_buttons ();
    
    md_state->verb_pending = FALSE;
    md_state->crnt_select_verb = 0;
    md_state->crnt_pref_verb_start = 0;
    md_state->crnt_pref_verb_end = 0;
    md_state->active_verb_ndx = -1;
    md_state->active_key_ndx = -1;
    md_state->last_key_obj = 0;
    md_state->last_parm_obj = 0;
    md_state->parm_list = (CHAR **)0;
    md_state->keyword[0] = NUL;
    md_state->no_of_parameters = 0;
    
    /* * MAKE THE DIALOG VISIBLE *                                                  */
    /* ---------------------------                                                  */
    
    wi_open_dialog (id);
    
    return SUCCESS;
    }
