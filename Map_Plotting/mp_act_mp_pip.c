/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ACT_MP_PIP.C*/
/*  *6    30-OCT-1990 11:53:18 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/*  *5    17-AUG-1990 22:03:24 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4     6-FEB-1990 15:39:33 WALTERS "(SPR 0) Fix ts_tcp_copy"*/
/*  *3    10-OCT-1989 16:55:12 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:51:48 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:19 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ACT_MP_PIP.C*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ACT_MP_PIP.C                            */
/*  *5    17-AUG-1990 22:03:24 VINCE "(SPR 5644) Code Cleanup"                      */
/*  *4     6-FEB-1990 15:39:33 WALTERS "(SPR 0) Fix ts_tcp_copy"                    */
/*  *3    10-OCT-1989 16:55:12 CONROY "(SPR 5028) Mods for new plotting capability" */
/*  *2    26-JUN-1989 10:51:48 GILLESPIE "(SPR 20) Fix castings"                    */
/*  *1    19-JUN-1989 13:10:19 SYSTEM ""                                            */
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ACT_MP_PIP.C                            */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_gl_defs.h"
#include "esi_mp.h"

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_GKS_H

#include "esi_gks.h"

#endif

#ifndef ESI_MD_MSG_H

#include "esi_md_msg.h"

#endif

#ifndef ESI_MD_ERR_H

#include "esi_md_err.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_md_basis.x"
#include "md_mapdef.rh"

#if USE_PROTOTYPES
static INT mpz_transfer_file_struct (MP_FILES *from, MP_FILES *to);
#else
static INT mpz_transfer_file_struct ();
#endif

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*    Activate a map.                                                               */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT mp_activate_map_pipe();                                         */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static INT mpz_transfer_file_struct(MP_FILES *from, MP_FILES *to);        */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mp_activate_map_pipe();                                         */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*    CANCEL                                                                        */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mp_activate_map_pipe ()
#else
publicdef INT mp_activate_map_pipe ()
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MAP_STRUCTURE *picked_map;
    MD_DISPLAY *display;
    MD_DISPLAY *picked_display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_MAPDEF *picked_comp;
    MD_COMPILED_VERB *verbstruc;
    MD_COMPILED_VERB *picked_verb;
    MD_VERB_INFO *verb_info;
    MD_PARM_STRUCT *parmstruc;
    MD_PARM_STRUCT *picked_parm;
    BOOL first;
    INT *map_number;
    CHAR *map_name;
    CHAR **map_list;
    RESOURCE_ID *cdlg_id;
    INT event_class;
    INT event_id;
    INT event_item;
    INT id;
    INT i;
    INT j;
    INT k;
    INT l;
    
    fi_push ();
    
    /* * OPEN WORKSPACES *                                                          */
    /* -------------------                                                          */
    
    am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *)&cdlg_id);
    am_open_workspace ("MAP_NUMBER", AM_GLOBAL, (VOIDPTR *)&map_number);
    
    /* * CHECK THAT HAVE A MAP TO ACTIVATE *                                        */
    /* -------------------------------------                                        */
    
    if (*map_number < 1)
        {
        am_message (AM_ALERT, mg_message (MD_NO_MAP_ACTIVE));
        status = CANCEL;
        goto finished;
        }
    /* * GET ACTIVATE MAP DIALOG *                                                  */
    /* ---------------------------                                                  */
    
    if (cdlg_id->activate_map_id EQUALS 0)
        {
        status = am_define_resource (AM_DIALOG, "MD_MAPDEF", MD_ACTIVATE_MAP,
                                     NULL_FUNCTION_PTR, &cdlg_id->activate_map_id,
                                     NULL);
        if (status != SUCCESS)
            {
            am_message (AM_ALERT, mg_message (status));
            status = CANCEL;
            goto finished;
            }
        }
    else
        {
        wi_clear (cdlg_id->activate_map_id);
        }
    id = cdlg_id->activate_map_id;
    
    /* * BUILD MAP LIST *                                                           */
    /* ------------------                                                           */
    
    map_list = ts_tcp_alloc (*map_number);
    qs_inq_i (QS_CUR_MAP_NUMBER, &j);
    for (i = 1; i <= *map_number; i++)
        {
        qs_set_i (QS_CUR_MAP_NUMBER, i);
        mp_get_current_map (&mapdef);
        map_name = (CHAR *) tc_alloc (strlen ((char *)(mapdef->map_name)) + 10);
        sprintf (map_name, "#%d: %s", i, mapdef->map_name);
        map_list[i - 1] = map_name;
        }
    qs_set_i (QS_CUR_MAP_NUMBER, j);
    
    /* * SEND LIST TO DIALOG *                                                      */
    /* -----------------------                                                      */
    
    wi_set_selections (id, MD_ACTIVATE_SELECT, MD_ACTIVATE_SCROLL, *map_number, 1,
                       map_list);
    wi_set_default_sel (id, MD_ACTIVATE_SELECT, 1);
    
    /* * OPEN DIALOG *                                                              */
    /* ---------------                                                              */
    
    wi_open_dialog (id);
    
    /* * SELECT A MAP *                                                             */
    /* ----------------                                                             */
    
    status = FAIL;
    while (status EQUALS FAIL)
        {
        wi_request_event (0, &event_class, &event_id, &event_item);
        
        if (event_id EQUALS id)
            {
            if (event_item != MD_ACTIVATE_CANCEL)
                {
                wi_query_selections (id, MD_ACTIVATE_SELECT, &k, &i);
                if (k < 1)
                    {
                    wi_beep (1);
                    continue;
                    }
                mp_retrieve_workspace (i);
                am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&mapdef);
                
                /* * MAKE A NEW MAPDEF *                                            */
                /* ---------------------                                            */
                
                if (event_item EQUALS MD_ACTIVATE_NEW)
                    {
                    
                    qs_set_i (QS_CUR_MAP_NUMBER, i);
                    mp_get_current_map (&picked_map);
                    
                    /* MODIFY MAP_STRUCTURE                                         */
                    (*map_number)++;
                    mapdef->map_number = *map_number;
                    mapdef->map_window_id = 0;
                    mapdef->change_flag = FALSE;
                    mapdef->map_displayed = FALSE;
                    mapdef->map_header_id = 0;
                    mapdef->scale_sizes_for_plotting =
                        picked_map->scale_sizes_for_plotting;
                    
                    /* TRANSFER DISPLAY STRUCTURE INFO                              */
                    
                    picked_display = picked_map->display;
                    mp_init_display_struct (&display, picked_display->last_row_used);
                    display->last_row_used = picked_display->last_row_used;
                    
                    hoblockmove (picked_display->verbs, display->verbs,
                                 picked_display->nrows * MD_VERB_LENGTH);
                    hoblockmove (picked_display->keywords, display->keywords,
                                 picked_display->nrows * MD_KEY_LENGTH);
                    hoblockmove (picked_display->verb_ndx, display->verb_ndx,
                                 picked_display->nrows * sizeof(INT));
                    hoblockmove (picked_display->key_ndx, display->key_ndx,
                                 picked_display->nrows * sizeof(INT));
                    hoblockmove (picked_display->compiled_row, display->compiled_row,
                                 picked_display->nrows * sizeof(INT));
                    ts_tcp_copy (&display->parameters, picked_display->parameters);
                    mapdef->display = display;
                    
                    tc_zeroes (display->overlay_displayed,
                               picked_display->nrows * sizeof(BOOL));
                    
                    /* TRANSFER COMPILED MAPDEF INFO                                */
                    
                    picked_comp = picked_map->compiled;
                    j = picked_comp->last_row_used + 1;
                    mp_init_compiled_mapdef (&compiled, j);
                    compiled->last_row_used = picked_comp->last_row_used;
                    
                    hoblockmove (picked_comp->verb_code, compiled->verb_code,
                                 j * sizeof(INT));
                    mapdef->compiled = compiled;
                    
                    /* TRANSFER NEEDED COMPILED VERB INFO                           */
                    
                    k = -1;
                    for (j = 0; j < display->last_row_used; j++)
                        {
                        if (display->verb_ndx[j] != k)
                            {
                            k = display->verb_ndx[j];
                            if (k >= 0)
                                {
                                verb_info = &(overlay_verbs[k]);
                                mp_init_comp_verb (&verbstruc, verb_info);
                                compiled->verb_ptr[display->compiled_row[j]] =
                                    verbstruc;
                                picked_verb =
                                              picked_comp->verb_ptr[picked_display->
                                                                    compiled_row[j]];
                                hoblockmove (picked_verb->key_code,
                                             verbstruc->key_code,
                                             picked_verb->nrows * sizeof(INT *));
                                hoblockmove (picked_verb->key_used,
                                             verbstruc->key_used,
                                             picked_verb->nrows * sizeof(INT *));
                                for (l = 0; l < picked_verb->nrows; l++)
                                    {
                                    first = TRUE;
                                    picked_parm = picked_verb->parm_ptr[l];
                                    parmstruc = NULL_PARM;
                                    while (picked_parm != NULL_PARM)
                                        {
                                        if (first)
                                            {
                                            parmstruc =
                                                        (MD_PARM_STRUCT *) 
                                                        am_allocate (AM_GLOBAL,
                                                                sizeof(
                                                                     MD_PARM_STRUCT));
                                            verbstruc->parm_ptr[l] = parmstruc;
                                            first = FALSE;
                                            }
                                        else
                                            {
                                            parmstruc->nxtparm =
                                                                 (MD_PARM_STRUCT *) 
                                                                 am_allocate (
                                                                            AM_GLOBAL,
                                                                              sizeof(
                                                                        MD_PARM_STRUCT))
                                                                 
                                            ;
                                            parmstruc = parmstruc->nxtparm;
                                            }
                                        parmstruc->parmtype = picked_parm->parmtype;
                                        
                                        switch (parmstruc->parmtype)
                                            {
                                        case 1: /* STRING                           */
                                            parmstruc->parmval.cval =
                                                                      (CHAR *) 
                                                                      am_allocate (
                                                                             AM_GLOBAL,
                                                                              strlen (
                                                                           picked_parm->
                                                                                      parmval
                                                                                      .cval
                                                                                      ) 
                                                                                   + 
                                                                                   1);
                                            strcpy (parmstruc->parmval.cval,
                                                    picked_parm->parmval.cval);
                                            break;
                                            
                                        case 2: /* INT                              */
                                            parmstruc->parmval.ival =
                                                                      picked_parm->
                                                                      parmval.ival;
                                            break;
                                            
                                        case 3: /* DOUBLE PRECISION                 */
                                            parmstruc->parmval.dbval =
                                                                       picked_parm->
                                                                       parmval.dbval;
                                            break;
                                            }
                                        picked_parm = picked_parm->nxtparm;
                                        }
                                    if (parmstruc != NULL_PARM)
                                        {
                                        parmstruc->nxtparm = NULL_PARM;
                                        }
                                    }
                                }
                            }
                        }
                    /* ALLOCATE NEW WINDOW MANAGER ARRAYS                           */
                    
                    mapdef->wm_id_list = (INT *) am_allocate (AM_GLOBAL,
                                                              100 * sizeof(INT));
                    tc_zeroes (mapdef->wm_id_list, 100 * sizeof(INT));
                    
                    mapdef->wm_id_count = (INT *) am_allocate (AM_GLOBAL,
                                                               100 * sizeof(INT));
                    tc_zeroes (mapdef->wm_id_count, 100 * sizeof(INT));
                    
                    mapdef->wm_group_names = (CHAR **) am_allocate (AM_GLOBAL,
                                                                    100 *
                                                                    sizeof(CHAR *));
                    tc_zeroes (mapdef->wm_group_names, 100 * sizeof(CHAR *));
                    
                    mapdef->wm_nalloc = 100;
                    
                    /* TRANSFER TITLE BLOCK INFO                                    */
                    
                    mapdef->map_title_on_map = picked_map->map_title_on_map;
                    strcpy (mapdef->title_block_name, picked_map->title_block_name);
                    hoblockmove (picked_map->title_size, mapdef->title_size,
                                 25 * sizeof(INT));
                    hoblockmove (picked_map->title_text, mapdef->title_text,
                                 25 * sizeof(MP_TTLBLK_LINE));
                    
                    /* TRANSFER PLOTTING LAYOUT ELEMENTS                            */
                    
                    mapdef->ttlblk_xmin = picked_map->ttlblk_xmin;
                    mapdef->ttlblk_xmax = picked_map->ttlblk_xmax;
                    mapdef->ttlblk_ymin = picked_map->ttlblk_ymin;
                    mapdef->ttlblk_ymax = picked_map->ttlblk_ymax;
                    
                    mapdef->bar_type = picked_map->bar_type;
                    mapdef->bar_xmin = picked_map->bar_xmin;
                    mapdef->bar_xmax = picked_map->bar_xmax;
                    mapdef->bar_ymin = picked_map->bar_ymin;
                    mapdef->bar_ymax = picked_map->bar_ymax;
                    
                    mapdef->arrow_type = picked_map->arrow_type;
                    mapdef->arrow_xmin = picked_map->arrow_xmin;
                    mapdef->arrow_xmax = picked_map->arrow_xmax;
                    mapdef->arrow_ymin = picked_map->arrow_ymin;
                    mapdef->arrow_ymax = picked_map->arrow_ymax;
                    
                    /* TRANSFER FILE SCATTER STRUCTURE                              */
                    
                    mpz_transfer_file_struct (&(picked_map->scatters),
                                              &(mapdef->scatters));
                    mpz_transfer_file_struct (&(picked_map->grids), &(mapdef->grids));
                    mpz_transfer_file_struct (&(picked_map->contours),
                                              &(mapdef->contours));
                    mpz_transfer_file_struct (&(picked_map->perspectives),
                                              &(mapdef->perspectives));
                    }
                /* * FRONT THE OLD MAP *                                            */
                /* ---------------------                                            */
                
                else if (mapdef->map_window_id != 0)
                    {
                    wm_inq_front_window (&i);
                    if (mapdef->map_window_id != i)
                        {
                        wmwfront (mapdef->map_window_id);
                        i = MAIN_WKID;
                        wm_grsgwk (i);
                        }
                    }
                /* * REDO FILE STRUCTURES *                                         */
                /* ------------------------                                         */
                
                mpz_defined_file_check ();
                
                /* * START THE MAP SERVER *                                         */
                /* ------------------------                                         */
                
                mp_init_map_server ();
                status = SUCCESS;
                /* Set current map unless CANCEL button hit  changed by JRB 5/22/88 SPR 4492    */
                mp_set_current_map (mapdef);
                }
            /* * CANCEL BUTTON SELECTED *                                           */
            /* --------------------------                                           */
            
            else
                {
                status = CANCEL;
                break;
                }
            }
        /* * CLICKED OUTSIDE RECOGNIZED EVENT *                                     */
        /* ------------------------------------                                     */
        
        else
            {
            wi_beep (1);
            }
        }
    /*   This reference moved to avoid calling when CANCEL button hit JRB 5/22/88   */
    /*mp_set_current_map(mapdef)                                                    */
    /*                                                                              */
    
    wi_close_dialog (id);
    ts_tcp_free (map_list);
    
finished:
    fi_pop ();
    return status;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static INT mpz_transfer_file_struct(MP_FILES *from, MP_FILES *to);        */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    from_struct     -(MP_FILES *)                                                 */
/*    to_struct       -(MP_FILES *)                                                 */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <mp_act_mp_pip.c>                                                  */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
static INT mpz_transfer_file_struct (MP_FILES *from_struct, MP_FILES *to_struct)
#else
static INT mpz_transfer_file_struct (from_struct, to_struct)
MP_FILES *from_struct;
MP_FILES *to_struct;
#endif
    {
    to_struct->n_names = from_struct->n_names;
    ts_tcp_copy (&to_struct->name, from_struct->name);
    to_struct->n_define = from_struct->n_define;
    if (to_struct->n_define != 0)
        {
        to_struct->n_references = (INT *) am_allocate (AM_GLOBAL,
                                                       to_struct->n_define *
                                                       sizeof(INT));
        hoblockmove (from_struct->n_references, to_struct->n_references,
                     to_struct->n_define * sizeof(INT));
        }
    return SUCCESS;
    }
/* END:                                                                             */
