/* DEC/CMS REPLACEMENT HISTORY, Element SX_MAN_ENTRY.C*/
/* *11   17-AUG-1990 22:22:01 VINCE "(SPR 5644) Code Cleanup"*/
/* *10   17-JAN-1990 15:40:45 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *9     2-NOV-1989 09:55:13 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *8    20-SEP-1989 15:01:47 GORDON "(SPR 200) Get in sync with Gulf"*/
/* *7    19-SEP-1989 13:39:10 GILLESPIE "(SPR 100) GULF MODS"*/
/* *6    14-SEP-1989 18:36:17 GORDON "(SPR -1) checkin from gulf"*/
/* *5     1-SEP-1989 14:40:19 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *4    12-AUG-1989 10:04:29 GILLESPIE "(SPR 65) Fix some bad references to user_struct"*/
/* *3    25-JUL-1989 20:13:22 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:03:42 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:29:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MAN_ENTRY.C*/
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

#include "esi_c_lib.h"
#include "esi_sx.h"

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#include "esi_sx_err.h"
#include "sx_cdlgs.rh"

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_LU_H
#include "esi_lu.h"
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT sx_manual_entry_srvr(INT id,INT item,SX_STRUCT *psx);

    publicdef INT sx_manual_entry(SX_STRUCT *psx);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Perform manual entry of a seismic line.

Prototype:
    publicdef INT sx_manual_entry(SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_manual_entry (SX_STRUCT *psx)
#else
publicdef INT sx_manual_entry (psx)
SX_STRUCT *psx;
#endif
    {
    SEGMENT_ID segment_id;
    INT units[3];
    INT value_inc[3];
    INT data_types[3];
    size_t data_sizes[3];
    INT status = SUCCESS;
    UINT stroke;
    INT len;
    MAP_STRUCTURE *map;
    INT id1;
    CHAR test_string[2];
    SX_WORKSPACE *w;
    INT dummy, old_color;
    SX_USER_STRUCT user_struct;
    CHAR bt_key[62];
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;
    static UINT d_list[3] = 
        {
        1,
        2,
        3
        };
    static CHAR *c_name[4] = 
        {
        "CDP",
        "X",
        "Y",
        NULL
        };
    
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *) & w);
    if (status != SUCCESS)
        {
        return status;
        }
    /* pick the line */
    /* set dialog text */
    
    sprintf (bt_key, "%s-%s", psx->pso->current_line.line_name,
             psx->pso->current_line.survey);
    wiscdt (psx->cdlg_id.main, ITEM_SLE_LINE_EDIT, bt_key);
    
    /* see if on the screen */
    
    if (btfind (&psx->lines, bt_key, &segment_id) AND segment_id > 0)
        {
        
        /* draw it outside of segments */
        
        wmwopen (psx->graphics.window);
        wmgopen (psx->graphics.group);
        wmtselect (psx->graphics.tnr);
        wmsvis (segment_id, GINVIS);
        wm_grsgwk (MAIN_WKID);
#if 0
        sx_special_pso (psx);
#endif
        /* set blinking */
        old_color = psx->pso->seismic_generic.seismic_line.line.color;
        lu_text_to_index("COLOR",
	    (INDEX *)&psx->pso->seismic_generic.seismic_line.line.color,
	    "HIGHLIGHT");
        so_process_line (psx->pso, FALSE, &dummy);
        psx->pso->seismic_generic.seismic_line.line.color = old_color;
        
        sx_check_sln (psx);
        nl_inq_nlist_int (psx->pso->cdp_xy, NL_NLIST_NPOINTS, &psx->nshots);
        psx->new_line = FALSE;
        psx->on_map = TRUE;
        psx->need_to_save = TRUE;
        psx->line_selected = TRUE;
        wmgclose (psx->graphics.group);
        wmwclose (psx->graphics.window);
        sx_menu_ls_setup (psx);
        sx_menu_sp_setup (psx);
        sx_menu_lp_setup (psx);
        SX_RETURN;
        }
    psx->nlines++;
    psx->line_selected = TRUE;
    
    /* see if in data base but not on screen */
    
    psx->pso->disp_opt.shot_label_nlist = (NLIST_HEADER)0;
    status = so_line_load (psx->pso);
    if (status == SUCCESS)
        {
        wmwopen (psx->graphics.window);
        wmgopen (psx->graphics.group);
        wmtselect (psx->graphics.tnr);
        so_process_line (psx->pso, TRUE, &segment_id);
#if 0
        sx_special_pso (psx);
#endif
        /* set blinking */
        old_color = psx->pso->seismic_generic.seismic_line.line.color;
        lu_text_to_index("COLOR",
	    (INDEX *)&psx->pso->seismic_generic.seismic_line.line.color,
	    "HIGHLIGHT");
        so_process_line (psx->pso, FALSE, &dummy);
        psx->pso->seismic_generic.seismic_line.line.color = old_color;
        
        sx_check_sln (psx);
        nl_inq_nlist_int (psx->pso->cdp_xy, NL_NLIST_NPOINTS, &psx->nshots);
        psx->line_selected = TRUE;
        psx->new_line = FALSE;
        psx->on_map = FALSE;
        psx->need_to_save = TRUE;
        wmgclose (psx->graphics.group);
        wmwclose (psx->graphics.window);
        
        /* add the line to the tree */
        
        btadd (&psx->lines, bt_key, &segment_id);
        
        /* add the line to the common nlist */
        if (w->temp_nlist EQUALS (NLIST_HEADER)NULL)
            {
            status = nl_clone_nlist (psx->pso->cdp_xy, &w->temp_nlist);
            }
        status = nl_copy_stroke (psx->pso->cdp_xy, 1, w->temp_nlist, &stroke);
        user_struct.segment_id = segment_id;
        user_struct.line_name = psx->pso->current_line;
        status = nl_set_stroke_user_struct (w->temp_nlist, stroke, &user_struct,
                                            sizeof(SX_USER_STRUCT));
        
        sx_menu_ls_setup (psx);
        sx_menu_sp_setup (psx);
        sx_menu_lp_setup (psx);
        SX_RETURN;
        }
    /* new line */
    
    /*create segment for new line */
    wmwopen (psx->graphics.window);
    /* added to the graphics group of the current overlay !! */
    wmgopen (psx->graphics.group);
    wmtselect (psx->graphics.tnr);
    wmsallocate (&segment_id);
    wmoset (segment_id, WM_CLASS_SEISMIC, psx->pso->current_line.line_name,
            psx->pso->current_line.line_name);
    btadd (&psx->lines, bt_key, &segment_id);
    
    sx_check_sln (psx);
    psx->line_selected = TRUE;
    psx->new_line = TRUE;
    psx->on_map = FALSE;
    psx->need_to_save = TRUE;
    psx->new_line_slope = 2.0;
    psx->new_line_intercept = 0.0;
    
    /* see if map registered */
    
    /* spr to re-enable registration warning 2/9/88 per ERIC E.*/
    
    if (!pmap->map_is_registered)
        {
        am_message (AM_ALERT, mg_message (SX_MAP_NOT_REGISTERED));
        }
    /* set up line */
    
    units[0] = units[1] = units[2] = 0;
    value_inc[0] = value_inc[1] = value_inc[2] = VALUE_LIST;
    data_types[0] = INT_TYPE;
    data_types[1] = data_types[2] = FLOAT_TYPE;
    data_sizes[0] = sizeof(INT);
    data_sizes[1] = data_sizes[2] = sizeof(FLOAT);
    
    nl_init_nlist (&psx->pso->cdp_xy, 3, units, data_sizes, data_types, value_inc, 0);
    status = nl_set_nlist_info (psx->pso->cdp_xy, NL_NLIST_COLUMN_NAME, 3, d_list,
                                (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    nl_start_stroke (psx->pso->cdp_xy, &stroke);
    psx->nshots = 0;
    sx_check_sln (psx);
#if 0
    sx_special_pso (psx);
#endif
    sx_default_disp_opt (psx->pso);
#if 0
    psx->pso->seismic_generic.draw_seismic_label = FALSE;
#endif
    
    /* add the line to the common nlist */
    if (w->temp_nlist EQUALS (NLIST_HEADER)NULL)
        {
        status = nl_clone_nlist (psx->pso->cdp_xy, &w->temp_nlist);
        }
    status = nl_copy_stroke (psx->pso->cdp_xy, 1, w->temp_nlist, &stroke);
    user_struct.segment_id = segment_id;
    user_struct.line_name = psx->pso->current_line;
    status = nl_set_stroke_user_struct (w->temp_nlist, stroke, &user_struct,
                                        sizeof(SX_USER_STRUCT));
    
    /* reset menus */
    
    sx_menu_ls_setup (psx);
    sx_menu_sp_setup (psx);
    sx_menu_lp_setup (psx);
    
    SX_RETURN;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT sx_manual_entry_srvr(INT id,INT item,SX_STRUCT *psx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    psx             -(SX_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_manual_entry_srvr (INT id, INT item, SX_STRUCT *psx)
#else
publicdef INT sx_manual_entry_srvr (id, item, psx)
INT id;
INT item;
SX_STRUCT *psx;
#endif
    {
    INT unused;
    CHAR text[31];
    switch (item)
        {
    case SLE_SURV_OK:
        wiqccdi (psx->cdlg_id.dialog_line_name, SLE_SURVEY_CODE, &unused,
                 psx->pso->current_line.survey);
        wiqccdi (psx->cdlg_id.dialog_line_name, SLE_LINE_NAME, &unused, text);
        strcpy (psx->pso->current_line.line_name, text);
        psx->pso->current_line.id = 0;
        ts_sto_upper (psx->pso->current_line.line_name);
        ts_sto_upper (psx->pso->current_line.survey);
        sx_manual_entry (psx);
        am_release_resource (psx->cdlg_id.dialog_line_name);
        break;
        
    case SLE_SURV_CANCEL:
        am_release_resource (psx->cdlg_id.dialog_line_name);
        break;
        
    default:
        break;
        }
    return SUCCESS;
    }
/* END:     */
