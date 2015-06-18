/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_SCR.C*/
/*  *8    16-JUL-1991 17:43:16 SANTIAGO "(SPR -1) commented out call to wm_update, unnecessary redraw"*/
/*  *7    15-OCT-1990 13:45:18 GILLESPIE "(SPR 1000) Merge ernie deltas"*/
/*   5B1  12-OCT-1990 17:15:41 GILLESPIE "Merge Ernie Deltas"*/
/*  *6    17-AUG-1990 22:03:19 VINCE "(SPR 5644) Code Cleanup"*/
/*  *5    21-MAY-1990 08:05:19 SCD "(SPR 1) Ensure deferral set after a window is allocated."*/
/*  *4    17-MAY-1990 09:53:54 JULIAN "(SPR 0) add return status on wm_inq_deferral"*/
/*  *3    10-OCT-1989 16:54:29 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:51:34 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:16 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_TO_SCR.C*/
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

#include    "esi_mp.h"
#include    "esi_am.h"
#include    "esi_ct.h"
#include    "esi_qs.h"
#include    "esi_wm.h"
#include    "esi_gks.h"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Drives drawing process for maps.                                              */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mpz_map_to_screen(BOOL full);                                   */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    full            -(BOOL)                                                       */
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
publicdef INT mpz_map_to_screen (BOOL full)
#else
publicdef INT mpz_map_to_screen (full)
BOOL full;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT i, j;
    INT defmod, regmod;
    INT def_status;
    /********************************************************************************/
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *)&md_state);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    
    /* * CREATE OR UPDATE THE WINDOW *                                              */
    /* -------------------------------                                              */
    def_status = wm_inq_deferral (&defmod, &regmod);
    
    if (mapdef->map_window_id EQUALS 0)
        {
        qs_set_c (QS_MAP_PROJECTION, mapdef->map_projection, 0);
        am_message (AM_DIALOG, "Creating the map window");
        mp_map_window ();
        wm_set_deferral (GBNIL, GSUPPD);
        }
    else
        {
        mp_hilite_window ();
        if (full)
            {
            mp_erase_map (mapdef, FALSE);
            }
        else
            {
            i = display->compiled_row[md_state->crnt_pref_verb_start - 1];
            mp_erase_overlay (i);
            }
        }
    mp_window_header ();
    
    /* * OPEN THE WINDOW *                                                          */
    /* -------------------                                                          */
    
    wmwopen (mapdef->map_window_id);
    wmtselect (mapdef->map_tnr);
    
    /* * DISPLAY THE OVERLAYS *                                                     */
    /* ------------------------                                                     */
    
    if (full)
        {
        i = 0;
        j = display->last_row_used;
        }
    else
        {
        i = md_state->crnt_pref_verb_start - 1;
        j = md_state->crnt_pref_verb_start;
        }
    
    status = mpz_display_overlays (i, j);
    
    if (full AND status EQUALS SUCCESS)
        {
        mapdef->map_displayed = TRUE;
        }
    /* * CLOSE THE WINDOW *                                                         */
    /* --------------------                                                         */
    
    wmwclose (mapdef->map_window_id);
    mpz_defined_file_check ();

/* ------------------------------------------------------------ */
/* RJS - removed for unnecessary redraw of GKS window           */
/*    wm_update ();                                             */
/* ------------------------------------------------------------ */
    if (def_status EQUALS SUCCESS)
        wm_set_deferral (defmod, regmod);
    
finished:
    return SUCCESS;
    }
/* END:                                                                             */
