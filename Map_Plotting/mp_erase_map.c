/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ERASE_MAP.C*/
/*   7    13-FEB-1992 16:08:40 JULIAN "(SPR 5770) added func to update MHL dlg box and added func to set window_id to 0"*/
/*  *6    15-AUG-1991 17:22:43 JTM "(SPR 0) Changed argument to wm_gslwsc() to DOUBLE."*/
/*  *5    17-AUG-1990 22:05:29 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4    17-MAY-1990 13:54:47 SCD "(SPR 1) Ensure that we do not refresh a workstation which has been deleted."*/
/*  *3    14-SEP-1989 16:18:48 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/*  *2    26-JUN-1989 10:55:29 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:47 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ERASE_MAP.C*/
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

#include "esi_gl_defs.h"
#include "esi_mp.h"
#include "esi_am.h"
#include "esi_wm.h"
#include "esi_tc.h"
#include "esi_gks.h"
#include "esi_dr.h"
#include "esi_lu.h"

/* Function Description -----------------------------------------------------
Description:
    Erase a map.

Prototype:
    publicdef INT mp_erase_map(MAP_STRUCTURE *map,BOOL full_delete);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    map             -(MAP_STRUCTURE *)
    full_delete     -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_erase_map (MAP_STRUCTURE *map, BOOL full_delete)
#else
publicdef INT mp_erase_map (map, full_delete)
MAP_STRUCTURE *map;
BOOL full_delete;
#endif
    {
    INT status;
    FLOAT nwcxmin, nwcxmax, nwcymin, nwcymax;
    FLOAT wcsxmin, wcsxmax, wcsymin, wcsymax;
    FLOAT x[5], y[5];
    INT temp_seg;
    DOUBLE dummy;
    INT i, color;
    CHAR go_of_map[128];
    GO_WORKSPACE *w;
    /******************************************************************************/
    
    map->map_displayed = FALSE;
    
    /* * MAKE SURE ACTUALLY HAVE SOMETHING TO ERASE * */
    /* ---------------------------------------------- */
    
    if (map->map_window_id EQUALS 0)
        {
        return SUCCESS;
        }
    /* * CHECK FOR FULL DELETION OR JUST BLANKING OUT * */
    /* ------------------------------------------------ */
    
    if (full_delete)
        {
        
        /* DELETE WINDOW */
        
        wmwclose (map->map_window_id);
        wmwdelete (map->map_window_id);
        
        /* REFRESH SCREEN IF THIS WAS FRONT WINDOW */
        
        wm_inq_front_window (&i);
        if (map->map_window_id EQUALS i)
            {
            /* ------------------------------------ */
            /* Only refresh screen if using the old */
            /* single GKS workstation graphics --   */
            /* Else there is no workstation to      */
            /* refresh                              */
            /* ------------------------------------ */
#ifndef USE_X
            i = MAIN_WKID;
            wm_grsgwk (i);
#endif
            }
        /* RE-INIT MAP WM_ID'S STRUCTURES */
        
        map->map_window_id = 0;
        map->map_tnr = 0;
        map->map_header_id = 0;
        }
    /* * JUST BLANKING * */
    /* ----------------- */
    
    else
        {
        wmwopen (map->map_window_id);
        
        /* DELETE OVERLAY GROUPS ASSOCIATED WITH MAP */
        
        i = 0;
        while (map->wm_group_names[i] != NULL)
            {
            if (map->wm_id_count[i] > 0)
                {
                wmgdelete (map->wm_id_list[i]);
                }
            i++;
            }
        /* BLANK OUT INTERIOR OF MAP WINDOW */
        
        wmtselect (map->map_tnr);
        wmtinq (map->map_tnr, &nwcxmin, &nwcxmax, &nwcymin, &nwcymax, &wcsxmin,
                &wcsxmax, &wcsymin, &wcsymax);
        
        x[0] = wcsxmin;
        y[0] = wcsymin;
        x[1] = wcsxmin;
        y[1] = wcsymax - (0.75 / map->wcs_to_inches);
        x[2] = wcsxmax;
        y[2] = y[1];
        x[3] = wcsxmax;
        y[3] = wcsymin;
        x[4] = wcsxmin;
        y[4] = wcsymin;
        
        wmsallocate (&temp_seg);
        
        i = 1;
        wm_gsfais (i);
        wm_gsln (i);
        dummy = 1.0;
        wm_gslwsc (dummy);
        
        lu_text_to_index ("COLOR", &color, "BACKGROUND");
        wm_gsfaci (color);
        lu_text_to_index ("COLOR", &color, "NORMAL");
        wm_gsplci (color);
        
        i = 5;
        wm_gfa (i, x, y);
        wm_gpl (i, x, y);
        
        wmsclose (temp_seg);
        wmsdelete (temp_seg);
        
        wmwclose (map->map_window_id);
        }
    /* * RE-INIT MAP WM STRUCTURES * */
    /* ----------------------------- */
    
    i = map->wm_nalloc * sizeof(INT);
    tc_zeroes (map->wm_id_list, i);
    tc_zeroes (map->wm_id_count, i);
    
    /* * RE-INIT MAP DISPLAY OVERLAY_DISPLAYED FLAGS * */
    /* ----------------------------------------------- */
    
    i = (map->display->last_row_used + 1) * sizeof(INT);
    tc_zeroes (map->display->overlay_displayed, i);
    
    /* * RE-INIT MAP COMPILED WM STRUCTURES * */
    /* -------------------------------------- */
    
    i = (map->compiled->last_row_used + 1) * sizeof(INT);
    tc_zeroes (map->compiled->overlay_wm_id, i);
    tc_zeroes (map->compiled->verb_wm_id, i);
    
    /* If there is a graphics object software picking workpace, release it. */
    /* open the GO workspace for this specified map. */
    
    sprintf (go_of_map, "GO_SPACE_%d", map->map_number);
    status = am_open_workspace (go_of_map, AM_GLOBAL, (VOIDPTR *) & w);
    if (status EQUALS SUCCESS)
        {
        if (w->temp_nlist != (NLIST_HEADER)NULL)
            status = nl_free_nlist (w->temp_nlist);
        
        if (w->master_nlist != (NLIST_HEADER)NULL)
            status = nl_free_nlist (w->master_nlist);
        status = am_release_workspace (go_of_map, AM_GLOBAL);
        }
    return SUCCESS;
    }
/* END:     */
