/* DEC/CMS REPLACEMENT HISTORY, Element SX_MAIN.C*/
/* *14    8-AUG-1991 16:24:27 MING "(SPR 0) assgin MAPDEF workspace to workspace return by mp_get_current_map"*/
/* *13   29-MAY-1991 15:07:10 JESSIE "(SPR 2579) pop up message box if there is no seismic overlay"*/
/* *12   30-OCT-1990 11:54:51 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *11   29-SEP-1990 11:33:24 GILLESPIE "(SPR 1) Beta Deltas"*/
/* *10   15-JAN-1990 08:10:08 GORDON "(SPR 0) Fix castings"*/
/* *9    24-OCT-1989 09:39:59 CONROY "(SPR 5043) Mods for new map UI includes"*/
/* *8    20-SEP-1989 15:00:27 GORDON "(SPR 200) Get in sync with Gulf"*/
/* *7    19-SEP-1989 13:39:03 GILLESPIE "(SPR 100) GULF MODS"*/
/* *6    14-SEP-1989 18:35:47 GORDON "(SPR -1) checkin from gulf"*/
/* *5     6-SEP-1989 09:39:02 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *4     1-SEP-1989 14:39:54 CONROY "(SPR 9999) MODS FOR NEW FULL_LINE_STRUCT"*/
/* *3    25-JUL-1989 20:13:13 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:03:27 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:29:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MAIN.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Exploration Systems, Inc.                                             */
/*            579 Market Street                                                     */
/*            San Francisco, CA  USA 94105                                          */
/*            (415) 974-1600                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/************************************************************************************/
/*                                                                                  */
/* PROGRAM:  SX_MAIN.C                                                              */
/* AUTHOR:   STEVE SPENCE                                                           */
/* FUNCTION: SEISMIC LINE EDITOR MAIN                                               */
/*                                                                                  */
/************************************************************************************/

#include "esi_c_lib.h"
#include "esi_sx.h"

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_LL_H

#include "esi_ll.h"

#endif

#ifndef ESI_AM

#include "esi_am.h"

#endif

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_MD_DBDEFS_H

#include "esi_md_dbdefs.h"

#endif

#ifndef ESI_WM_CLS_H

#include "esi_wm_cls.h"

#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"
#include "sx_menus.rh"
#include "sx_cdlgs.rh"

#if USE_PROTOTYPES
publicdef INT sx_main (BYTE *byte_pointer)
#else
publicdef INT sx_main (byte_pointer)
BYTE *byte_pointer;
#endif
    {
    MAP_STRUCTURE *map;             /*  map workspace pointer sle structure pointer */
    MAP_STRUCTURE *mapdef;          /*  map workspace pointer for sle MAPDEF ws     */
    SX_STRUCT *psx;                 /*  generic structure to open overlay status    */
    INT status;
    
    LISTHEAD lines;                 /*  list of line segments number of line        */
    INT nlines;                     /*  segments loop counter, wmo class wm object  */
    INT i, class ;                  /*  db name wm user object name wm segment id   */
    CHAR db_name[62];               /*  of seismic line the length of map name.     */
    CHAR usr_name[62];              /*  wmwinq stuff                                */
    SEGMENT_ID segment_id;
    FLOAT x1, x2, y1, y2;
    INT dummy;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    SX_WORKSPACE *w;
    
    /*  assume SUCCESS                                                              */
    
    status = SUCCESS;
    
    /*  make sure there is a map                                                    */
    
    status = mp_front_map (&map);
    if (status != SUCCESS)
        {
        status = SX_NO_MAP;
        am_message (AM_ALERT, mg_message (status));
        mp_set_current_map ((MAP_STRUCTURE *)0);
        SX_RETURN;
        }

    /* ming - 8/9/91 */
    /* The following is added to get the APPLICATION workspace of the map back */
    /* so program use am_open_workspace for MAPDEF will work at any situation  */
    /* even after QUIT mapping which will delete all mapping APPLICATION       */
    /* workspace. Otherwise, when user click SEISMIC LINE EDITOR immediately,  */
    /* when program reference the workspace by am_open_workspace, it will bomb */
    /* ----------------------------------------------------------------------- */
    status = am_open_workspace ("MAPDEF",AM_APPLICATION,(VOIDPTR *)&mapdef); 
    if (status != SUCCESS)
	{
	status = am_define_workspace ("MAPDEF",AM_APPLICATION,sizeof(MAP_STRUCTURE),
				      (VOIDPTR *)&mapdef);
        }
    mapdef = map;

    status = wmwopen (map->map_window_id);
    if (status != SUCCESS)
        {
        status = SX_NO_MAP;
        am_message (AM_ALERT, mg_message (status));
        mp_set_current_map ((MAP_STRUCTURE *)0);
        SX_RETURN;
        }
    /*  allocate SX structure                                                       */
    
    psx = (SX_STRUCT *) am_allocate (AM_APPLICATION, sizeof(SX_STRUCT));
    if (psx == (SX_STRUCT *)0)
        {
        status = FAIL;
        wmwclose (map->map_window_id);
        mp_set_current_map ((MAP_STRUCTURE *)0);
        SX_RETURN;
        }
    /*  allocate SO structure                                                       */
    
    psx->default_filter = TRUE;
    psx->graphics.group = -1;
    if (map->pso != (VOIDPTR)0)
        {
        psx->free_pso = FALSE;
        psx->pso = (SO_STRUCT *)map->pso;
        psx->pso->map = (VOIDPTR)map;
        display = map->display;
        compiled = map->compiled;
        for (i = 0; i < display->last_row_used; i++)
            {
            dummy = display->compiled_row[i];
            if (dummy >= 0 && mp_is_seismic_overlay (compiled->verb_code[dummy]))
                {
                psx->graphics.group = compiled->overlay_wm_id[dummy];
                break;
                }
            }
        }
    else
        {
        psx->free_pso = TRUE;
        status = sx_load_psx (psx, &(psx->graphics.group));
        if (status != SUCCESS)
            {
	    wi_beep(1);
	    am_message(AM_ALERT, "No seismic overlay on current map. \
		     Please add one and restart the seis line editor.");
            wmwclose (map->map_window_id);
            am_free ((VOIDPTR)psx);
            mp_set_current_map ((MAP_STRUCTURE *)0);
            SX_RETURN;
            }
        psx->pso->map = (VOIDPTR)map;
        }
    /*  get current project                                                         */
    
    qs_inq_c (QS_PROJECT_NAME, psx->project_name, (INT *)0);
    
    /*  get the window manager group for the                                        */
    /*  seismic overlay                                                             */
    
    if (psx->graphics.group == -1)
        {
        if (psx->free_pso)
            {
            am_free ((VOIDPTR)psx->pso);
            }
        am_free ((VOIDPTR)psx);
        status = SX_CANT_OPEN_OVERLAY;
        am_message (AM_ALERT, mg_message (SX_CANT_OPEN_OVERLAY));
        wmwclose (map->map_window_id);
        mp_set_current_map ((MAP_STRUCTURE *)0);
        SX_RETURN;
        }
    /*  get background color                                                        */
    
    wmwinq (map->map_window_id, &x1, &x2, &y1, &y2, &psx->graphics.background_color);
    
    /*  we now have a seismic overlay to set map ->                                 */
    /*  pso                                                                         */
    
    map->pso = (VOIDPTR)psx->pso;
    
    /*  set group detectability                                                     */
    
    wmgdtec (psx->graphics.group, TRUE);
    
    if (wmgopen (psx->graphics.group) != SUCCESS)
        {
        if (psx->free_pso)
            {
            am_free ((VOIDPTR)psx->pso);
            }
        am_free ((VOIDPTR)psx);
        status = SX_CANT_OPEN_OVERLAY;
        am_message (AM_ALERT, mg_message (SX_CANT_OPEN_OVERLAY));
        wmwclose (map->map_window_id);
        mp_set_current_map ((MAP_STRUCTURE *)0);
        SX_RETURN;
        }
    /*  initialize line btree                                                       */
    
    btinit (&psx->lines, 0, sizeof db_name, sizeof(INT), BT_KEY_IS_C_STRING);
    
    /*  see if we have any lines                                                    */
    
    llinit (&lines, 0, sizeof(INT), 0);
    wmsinqlist (WM_CLASS_SEISMIC, &lines);
    
    psx->nlines = 0;
    nlines = llcount (&lines);
    if (nlines > 0)
        {
        llfirst (&lines, &segment_id);
        for (i = 0; i < nlines; i++)
            {
            status = wmoinq (segment_id, &class , db_name, usr_name);
            if (status != SUCCESS || class  != WM_CLASS_SEISMIC)
                {
                continue;
                }
            psx->nlines++;
            btadd (&psx->lines, db_name, &segment_id);
            llnext (&lines, &segment_id);
            }
        }
    llfree (&lines);
    
    /*  set up menus                                                                */
    
    psx->line_selected = FALSE;
    psx->need_to_save = FALSE;
    psx->nshots = 0;
    
    am_define_resource (AM_MENU, "sx_menus", MENU_LS, sx_ls_menu_srvr,
                        &psx->menu_id.ls, psx);
    sx_menu_ls_setup (psx);
    
    am_define_resource (AM_MENU, "sx_menus", MENU_SP, sx_sp_menu_srvr,
                        &psx->menu_id.sp, psx);
    sx_menu_sp_setup (psx);
    
    am_define_resource (AM_MENU, "sx_menus", MENU_LP, sx_lp_menu_srvr,
                        &psx->menu_id.lp, psx);
    sx_menu_lp_setup (psx);
    
    /*  set up main dialog box                                                      */
    
    am_define_resource (AM_DIALOG, "sx_cdlgs", DIALOG_SLE, NULL_FUNCTION_PTR,
                        &psx->cdlg_id.main, (VOIDPTR)0);
    wiscdt (psx->cdlg_id.main, ITEM_SLE_LINE_EDIT, "");
    wiscdt (psx->cdlg_id.main, ITEM_SLE_MAP_EDIT, map->map_name);
    wiscdt (psx->cdlg_id.main, ITEM_SLE_AREA_EDIT, psx->project_name);
    wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, "");
    /*  THESE TWO ARE DISABLED FOR NOW AND REMOVED                                  */
    /*  FROM THE MAIN DIALOG                                                        */
#if 0
    wiscdt (psx->cdlg_id.main, ITEM_SLE_RANGE_EDIT, "");
    wiscdt (psx->cdlg_id.main, ITEM_SLE_CHANGED_EDIT, "");
#endif
    wiopcd (psx->cdlg_id.main);
    
    /*  set up so trigger struct for all shot                                       */
    /*  points                                                                      */
    
    psx->all.shot_symbol_value = 1;
    psx->all.shot_label_value = 0;
    psx->all.zvalue_value = 0;
    
    strcpy (psx->all.shot_symbol_trigger, "N");
    strcpy (psx->all.shot_label_trigger, "");
    strcpy (psx->all.zvalue_trigger, "");
    
    /*  to make seis line edit more wysiwyg - keep                                  */
    /*  old stuff in case of spr                                                    */
    psx->pso->disp_opt.ov.shot_label_value = 0;
    psx->pso->disp_opt.ov.zvalue_value = 0;
#if 0
    psx->stash = psx->pso->disp_opt.ov;
    
    psx->pso->disp_opt.ov = psx->all;
#endif
    /*  to make seis line edit more wysiwyg keep                                    */
    /*  this in case we need it                                                     */
    
    psx->stash = psx->all;
    
    /*  save and shut down graphics environment                                     */
    
    psx->graphics.window = map->map_window_id;
    psx->graphics.tnr = map->map_tnr;
    wmgclose (psx->graphics.group);
    wmwclose (psx->graphics.window);
    
    /*  set up shotpoint adding defaults                                            */
    
    psx->dialog_spo.add_type = SX_AUTO;
    psx->dialog_spo.start_shot = 0.0;
    psx->dialog_spo.shot_incr = 10.0;
    psx->dialog_spo.shot_incr_factor = SX_ASCEND;
    psx->dialog_spo.shot_spacing_method = SX_MANUAL;
    psx->dialog_spo.shot_spacing = 0.0;
    psx->dialog_spo.shot_label_type = SX_NUMERIC;
    psx->cdlg_id.dialog_spo = 0;
    psx->pso->psx = (VOIDPTR)psx;
    
    /*  open or define static workspace for picking                                 */
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *)&w);
    if (status != SUCCESS)
        {
        status = am_define_workspace ("SX_STATIC_SPACE", AM_GLOBAL,
                                      sizeof(SX_WORKSPACE), (VOIDPTR *)&w);
        w->changed = TRUE;
        }
    if (w->changed == TRUE)
        {
        if (w->temp_nlist != (NLIST_HEADER)0)
            status = nl_free_nlist (w->temp_nlist);
        if (w->master_nlist != (NLIST_HEADER)0)
            status = nl_free_nlist (w->master_nlist);
        
        status = so_build_common_nlist (psx->pso, &w->temp_nlist);
        
        /*  copy temp nlist to master nlist in                                      */
        /*  workspace                                                               */
        status = nl_copy_nlist (w->temp_nlist, &w->master_nlist);
        w->changed = FALSE;
        }
    status = SUCCESS;
    
    SX_RETURN;
    }
