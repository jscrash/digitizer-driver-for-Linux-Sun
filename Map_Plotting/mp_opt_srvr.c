/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OPT_SRVR.C*/
/*  *5    17-AUG-1990 22:07:47 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4    15-FEB-1990 10:45:23 WALTERS "(SPR 0) Changes in map setup dialog"*/
/*  *3    10-OCT-1989 16:59:11 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:58:10 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:48 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OPT_SRVR.C*/
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
#include    "esi_wi.h"
#include    "esi_am.h"
#include    "esi_qs.h"
#include    "esi_md_err.h"
#include    "md_mapdef.rh"
#include    "mp_menus.rh"

/* Function Description -----------------------------------------------------
Description:
    Processes menu events from the OPTIONS menu.

Prototype:
    publicdef INT mp_options_server(INT id, INT item, BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    pointer         -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_options_server (INT id, INT item, BYTE *pointer)
#else
publicdef INT mp_options_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    RESOURCE_ID *cdlg_id;
    CHAR proj[16];
    CHAR coord[16];
    CHAR uom[21];
    FLOAT minx, maxx;
    FLOAT miny, maxy;
    BOOL full;
    BOOL screen;
    INT dummy;
    INT i;
    INT scale_sizes;
    DOUBLE wcs1, wcs2;
    
    /******************************************************************************/
    
    switch (item)
        {
    case MP_MAP_SETUP:
        /* STORE CURRENT SETUP PARAMETERS */
        
        am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
        
        if (mapdef->map_window_id != 0)
            {
            strcpy (proj, mapdef->map_projection);
            strcpy (coord, mapdef->map_coordinate_system);
            strcpy (uom, mapdef->map_scale_uom);
            minx = mapdef->lower_left_xy.x;
            maxx = mapdef->upper_right_xy.x;
            miny = mapdef->lower_left_xy.y;
            maxy = mapdef->upper_right_xy.y;
            full = mapdef->full_screen_flag;
            scale_sizes = mapdef->scale_sizes_for_plotting;
            mp_get_plot_wcs_to_inches (mapdef, &wcs1);
            }
        /* GET SETUP PARAMETERS */
        
        mp_map_setup_pipe ();
        
        /* COMPARE TO STORED PARAMETERS */
        
        if (mapdef->map_window_id != 0)
            {
            mp_get_plot_wcs_to_inches (mapdef, &wcs2);
            if (full != mapdef->full_screen_flag OR minx !=
                mapdef->lower_left_xy.x OR maxx != mapdef->upper_right_xy.x OR miny !=
                mapdef->lower_left_xy.y OR maxy !=
                mapdef->upper_right_xy.y OR scale_sizes !=
                mapdef->scale_sizes_for_plotting OR (mapdef->
                                                     scale_sizes_for_plotting AND wcs1 
                                                     != wcs2) OR 
                strcmp (proj, mapdef->map_projection) != 0 OR strcmp (coord,
                                                        mapdef->
                                                             map_coordinate_system) !=
                0 OR strcmp (uom, mapdef->map_scale_uom) != 0)
                {
                
                /* DELETE WINDOW FOR NEW MAP */
                
                qs_set_array_i (QS_MAP_HILITE_OVERLAY, mapdef->map_number, 0);
                qs_set_array_i (QS_MAP_SEISMIC_PICKING_GROUP, mapdef->map_number, 0);
                qs_set_array_i (QS_MAP_WELL_HILITE_GROUP, mapdef->map_number, 0);
                qs_set_array_i (QS_MAP_SEISMIC_HILITE_GROUP, mapdef->map_number, 0);
                
                mp_erase_map (mapdef, TRUE);
                }
            }
        break;
        
    case MP_ERASE_MAP:
        am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
        mp_erase_map (mapdef, TRUE);
        break;
        
    case MP_ERASE_OVERLAY:
        am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
        am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
        am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
        
        /* * CHECK THAT A PREFERENCE OVERLAY HAS BEEN SELECTED IF NOT FULL * */
        /* ----------------------------------------------------------------- */
        
        if (md_state->crnt_pref_verb_start EQUALS 0)
            {
            am_open_workspace ("CDLG", AM_APPLICATION, (VOIDPTR *) & cdlg_id);
            wi_beep (1);
            wi_set_dialog_text (cdlg_id->overlays_id, MD_OVER_MSG,
                                mg_message (MD_SELECT_ONE_VERB));
            }
        else
            {
            i = md_state->crnt_pref_verb_start - 1;
            if (mapdef->display->overlay_displayed[i])
                {
                mp_erase_overlay (mapdef->display->compiled_row[i]);
                mapdef->display->overlay_displayed[i] = FALSE;
                }
            }
        break;
        
    default:
        switch (item)
            {
        case MP_DISPLAY_OVERLAY:
            full = FALSE;
            screen = TRUE;
            break;
            
        case MP_DISPLAY_MAP:
            full = TRUE;
            screen = TRUE;
            break;
            
        case MP_PLOT_OVERLAY:
            full = FALSE;
            screen = FALSE;
            break;
            
        case MP_PLOT_MAP:
            full = TRUE;
            screen = FALSE;
            break;
            }
        mp_display_map (full, screen);
        }
    return SUCCESS;
    }
/* END:     */
