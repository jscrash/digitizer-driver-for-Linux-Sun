/* DEC/CMS REPLACEMENT HISTORY, Element GB_W_PICKING.C */
/* *7     8-JUL-1991 15:45:18 JULIAN "(SPR 1554) add missing text in status box" */
/* *6    23-OCT-1990 13:51:54 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *5     6-MAR-1990 16:18:09 WALTERS "(SPR 0) Show well name instead of uwi in messages" */
/* *4    10-OCT-1989 13:01:58 GORDON "(SPR 4999) Break out of loop if no wells are pickable." */
/* *3    14-SEP-1989 16:15:38 GORDON "(SPR 100) Gulf/Sun/Unix mods" */
/* *2    11-AUG-1989 20:18:17 CONROY "(SPR 0) Install revised seismic selects & picking" */
/* *1    11-AUG-1989 19:54:00 CONROY "Global well picking" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_W_PICKING.C */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_PICKING.C                                */
/* *4    25-JUL-1989 19:39:14 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *3    12-JUL-1989 07:37:33 GORDON "(SPR 0) add calls to wm_set_mouse_menu"       */
/* *2    26-JUN-1989 10:44:49 GILLESPIE "(SPR 20) Fix castings"                     */
/* *1    19-JUN-1989 12:51:31 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_PICKING.C                                */

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

/* * INCLUDES *                                                                     */
/* ------------                                                                     */

#include "esi_c_lib.h"

#ifndef ESI_GB_H
#include "esi_gb.h"
#endif

#ifndef ESI_GB_ERR_H
#include "esi_gb_err.h"
#endif

#ifndef ESI_GB_MSG_H
#include "esi_gb_msg.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WE_WELLS_H
#include "esi_we_wells.h"
#endif

#define ARRAY_BLOCKSIZE 20

/* ****************************************************************                 */

publicdef INT gb_well_picking (listPtr)
NLIST_HEADER *listPtr;

/* ****************************************************************                 */
    {
    INT status = SUCCESS;
    INT wmStatus, button;
    INT i;
    SELECT_KEY_STRING uwi;
    CHAR prompt[STMAX], status_string[STMAX];
    DOUBLE d_pick_x, d_pick_y;
    DOUBLE x_loc, y_loc;
    FLOAT f_pick_x, f_pick_y;
    GKS_REAL gks_px[2], gks_py[2];
    GKS_INT c_index;
    INT defmod, regmod;
    MAP_STRUCTURE *map;
    SELECT_KEY_STRING *pick_arr;
    INT n_alloc = 0;
    INT n_used = 0;
    INT total_picks = 0;
    INT start_point;
    APPLICATION_ID appSave;
    WINDOW_ID winSave;
    TNR_ID tnrSave;
    GROUP_ID groSave;
    UINT stroke;
    UINT ndim;
    UINT d_list[1];
    VOIDPTR v_list[1];
    WELL_STRUCT *ww_hdr;
    WELL_HDR_1 *ww_hdr_1;
    
    /********************************************************************************/
    
    wm_inq_deferral (&defmod, &regmod);
    wm_hold_updates ();
    
    /* * GET MAP FOR PICKING *                                                      */
    /* -----------------------                                                      */
    
    if ((status = mp_front_map (&map)) != SUCCESS)
        {
        am_message (AM_ALERT, mg_message (GB_ERR_NO_MAP_WKSP));
        return(FAIL);
        }
    wm_update ();
    
    /* * SAVE WM STATE AND CLOSE OUT TO APPLICATION *                               */
    /* ----------------------------------------------                               */
    
    wmowhere (&appSave, &winSave, &tnrSave, &groSave, &wmStatus);
    
    /* * ATTACH TO TOP LEVEL *                                                      */
    /* -----------------------                                                      */
    
    wmoattach (appSave, (WINDOW_ID)0, (TNR_ID)0, (GROUP_ID)0, &wmStatus);
    if (wmStatus != SUCCESS)
        {
        am_message (AM_ALERT, "wmoattach() failed");
        return(FAIL);
        }
    wmwopen (map->map_window_id);
    wmtselect (map->map_tnr);
    if (ug_if_gpx ())
        {
        wm_set_mouse_menu ((CHAR **)0, (INT *)0);   /* default menu                 */
        }
    /* * ALLOCATE PICKING ARRAY *                                                   */
    /* --------------------------                                                   */
    
    pick_arr = (SELECT_KEY_STRING *) tc_zalloc (ARRAY_BLOCKSIZE *
                                                sizeof(SELECT_KEY_STRING));
    n_alloc += ARRAY_BLOCKSIZE;
    
    /* * PICKING LOOP *                                                             */
    /* ----------------                                                             */
    
    wm_gsmk (GAST);
#ifdef PRIOR_GKS   /* ========== BEGIN PRIOR GKS ========== */
    wm_gsmksc (2.0);
#endif  /* ========== END PRIOR GKS ========== */
    strncpy (prompt, mg_message (GB_WELL_PICK_PROMPT), STMAX);
    prompt[STMAX - 1] = 0;
    FOREVER
        {
        
        /* * GET CROSSHAIR LOCATION *                                               */
        /* --------------------------                                               */
        
        am_message (AM_DIALOG, prompt);
        wmtlocate (&wmStatus, &f_pick_x, &f_pick_y, &button);
        if (((wmStatus EQUALS SUCCESS) AND (
                                       button EQUALS 
                                    WM_BUTTON_DONE))OR (
                                                      button EQUALS WM_BUTTON_CANCEL))
            {
            break;
            }
        /* * SEARCH FOR NEAREST VISIBLE WELL *                                      */
        /* -----------------------------------                                      */
        
        d_pick_x = (DOUBLE)f_pick_x;
        d_pick_y = (DOUBLE)f_pick_y;
        if (gb_pick_well (d_pick_x, d_pick_y, uwi, &x_loc, &y_loc) EQUALS SUCCESS)
            {
            
            /* * LOOK FOR FOUND WELL IN PICK_ARR *                                  */
            /* -----------------------------------                                  */
            
            for (i = 0; i < n_used; i++)
                {
                if (ARE_SAME (uwi, pick_arr[i]))
                    {
                    break;
                    }
                }
            /* * IF FOUND IN LIST, REMOVE AND TURN BLINKING OFF *                   */
            /* --------------------------------------------------                   */
            
            if (i < n_used)
                {
                pick_arr[i][0] = 0; /* set to empty string                          */
                total_picks--;
                lu_text_to_index ("COLOR", &c_index, "BACKGROUND");
                we_get_well_header (uwi, WELL_HDR_1_DETAIL, &ww_hdr);
                ww_hdr_1 = (WELL_HDR_1 *)ww_hdr->detail.ptr;
                sprintf (status_string, mg_message (GB_REMOVE_FROM_LIST),
                         ww_hdr_1->name);
                }
            /* * ELSE ADD TO LIST AND TURN BLINKING ON *                            */
            /* -----------------------------------------                            */
            
            else
                {
                if (n_used EQUALS n_alloc)
                    {
                    n_alloc += ARRAY_BLOCKSIZE;
                    pick_arr = (SELECT_KEY_STRING *) tc_realloc ((VOIDPTR)pick_arr,
                                                                 n_alloc *
                                                             sizeof(
                                                                  SELECT_KEY_STRING));
                    }
                strcpy (pick_arr[n_used], uwi);
                n_used++;
                total_picks++;
                lu_text_to_index ("COLOR", &c_index, "HIGHLIGHT");
                we_get_well_header (uwi, WELL_HDR_1_DETAIL, &ww_hdr);
                ww_hdr_1 = (WELL_HDR_1 *)ww_hdr->detail.ptr;
		if(IS_EMPTY_STRING(ww_hdr_1->name))
		  sprintf (status_string, mg_message (GB_ADD_TO_LIST), 
			   "Nothing");
		else
		  sprintf (status_string, mg_message (GB_ADD_TO_LIST), ww_hdr_1->name);
                }
            wm_gspmci (c_index);
            gks_px[0] = x_loc;
            gks_py[0] = y_loc;
            wm_gpm (1, gks_px, gks_py);
            
            strncpy (prompt, mg_message (GB_WELL_PICK_PROMPT), STMAX);
            prompt[STMAX - 1] = 0;
            strncat (prompt, status_string, STMAX);
            }
        else
            {
            am_message (AM_ALERT, mg_message (GB_WELL_MISS_PROMPT));
            break;
            }
        }
    /* * END OF PICKING LOOP, NOW BUILD SELECT LIST IF NOT CANCELLED *              */
    /* ---------------------------------------------------------------              */
    
    if ((wmStatus EQUALS SUCCESS) AND (total_picks > 0)AND (button !=
                                                            WM_BUTTON_CANCEL))
        {
        sl_init_select_list (WELLS_DATA_TYPE, listPtr);
        nl_start_stroke (*listPtr, &stroke);
        ndim = 1;
        d_list[0] = 1;
        
        start_point = 0;
        total_picks = 0;
        for (i = 0; i < n_used; i++)
            {
            if (IS_EMPTY_STRING (pick_arr[i]))
                {
                if (total_picks > 0)
                    {
                    v_list[0] = (VOIDPTR)pick_arr[start_point];
                    nl_add_points (*listPtr, total_picks, ndim, d_list, v_list);
                    total_picks = 0;
                    }
                start_point = i + 1;
                }
            else
                {
                total_picks++;
                }
            }
        if (total_picks > 0)
            {
            v_list[0] = (VOIDPTR)pick_arr[start_point];
            nl_add_points (*listPtr, total_picks, ndim, d_list, v_list);
            }
        status = SUCCESS;
        }
    else
        {
        status = FAIL;              /* So list will not be made current             */
        }
    tc_free (pick_arr);
    wmoattach (appSave, winSave, tnrSave, groSave, &wmStatus);
    am_message (AM_DIALOG, "");
    wm_set_deferral (defmod, regmod);
    return(status);
    }
