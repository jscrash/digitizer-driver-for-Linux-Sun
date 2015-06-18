/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAP_WINDOW.C*/
/*  *7     3-MAY-1991 16:12:12 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/*  *6    15-OCT-1990 13:49:45 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*   4B1  12-OCT-1990 17:18:55 GILLESPIE "Merge Ernie Deltas"*/
/*  *5    17-AUG-1990 22:07:07 VINCE "(SPR 5644) Code Cleanup"*/
/*  *4    10-OCT-1989 16:57:39 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *3    14-SEP-1989 16:18:54 GORDON "(SPR 100) Gulf/Sun/Unix mods"*/
/*  *2    26-JUN-1989 10:57:20 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:13 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_MAP_WINDOW.C*/
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

#include    "esi_math.h"
#include    "esi_c_lib.h"
#include    "esi_mp.h"
#include    "esi_am.h"
#include    "esi_wm.h"
#include    "esi_lu.h"
#include    "esi_wm_cls.h"

#ifndef ESI_GTYPES_H
#include    "esi_gtypes.h"
#endif

#ifndef ESI_GKS_H
#include    "esi_gks.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include    "esi_md_msg.h"
#include    "esi_md_err.h"

#define MARGIN 1.4

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Creates a window for a map.                                                   */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT mp_map_window();                                                */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    None.                                                                         */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    MD_CANCEL                                                                     */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT mp_map_window (void)
#else
publicdef INT mp_map_window ()
#endif
    {
    INT wmStatus, tnr, pet;
    FLOAT wx1, wy1, wx2, wy2;
    INT status;
    MAP_STRUCTURE *map;
    struct 
        {
        FLOAT x, y;
        } lower_left_box, upper_right_box;
    struct 
        {
        FLOAT x, y;
        } lower_left_wcs, upper_right_wcs;
    DOUBLE one_inch, screen_margin, margin_wcs, twice_margin;
    GKS_REAL ndc_min_x, ndc_max_x, ndc_min_y, ndc_max_y;
    DOUBLE diff;
    DOUBLE map_ratio, screen_ratio;
    INT button, dum1, dum2, dum3;
    INT color;
    DOUBLE plot_wcs_to_inches, plot_width, plot_height;
    LINE_ATTRIBUTE line;
    
    /********************************************************************************/
    
    /* * IF WINDOW IS ALREADY CREATED, RETURN *                                     */
    /* ----------------------------------------                                     */
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&map);
    if (map->map_window_id != 0)
        {
        return SUCCESS;
        }
    /* * GET % OF SCREEN = 1 INCH *                                                 */
    /* ----------------------------                                                 */
    
    /*                                                                              */
    /* * (This is hardcoded because GQDSP does not return the size of               */
    /* *  the display in meters)                                                    */
    /*                                                                              */
    
    gkz_inq_workstation (&dum1, &dum2, &dum3);
    if (dum1 EQUALS 180)
        {
        one_inch = 0.07272;
        }
    else if (dum1 EQUALS 110)
        {
        one_inch = 0.11429;
        }
    screen_margin = one_inch * MARGIN;
    
    /* * GET MAXIMUM POSSIBLE WINDOW SIZE (EITHER FULL SCREEN OR BANDED RECT) *     */
    /* ------------------------------------------------------------------------     */
    
    dum1 = 1;
    wm_set_current_screen (dum1);   /* Assign to LEFT screen                        */
    
    if (map->full_screen_flag)      /* Make window full screen                      */
        {
        wm_inq_wkwn (&lower_left_box.x, &upper_right_box.x, &lower_left_box.y,
                     &upper_right_box.y);
        }
    else
        {                           /* Request the corners from the user/operator   */
        
        am_message (AM_DIALOG, mg_message (MD_BAND_WINDOW));
        dum2 = UGRQ_TYPE_BAND_RECTANGLE;
        twice_margin = 2.0 * screen_margin;
        dum3 = 0;
        wm_gselnt (dum3);
        
        FOREVER
            {
#ifdef SUN_GKS_OR_PRIOR_GKS
            pet = 1;
            wmz_getxy (&tnr, &wx1, &wy1, &button, pet);
            gpx_wmtbandrectangle (&wmStatus, wx1, wy1, &wx2, &wy2, &button);
            ndc_min_x = (DOUBLE)wx1;
            ndc_min_y = (DOUBLE)wy1;
            ndc_max_x = (DOUBLE)wx2;
            ndc_max_y = (DOUBLE)wy2;
#else       
            wm_grqlc (dum1, dum1, &status, &dum3, &ndc_min_x, &ndc_min_y);
            
            ugrqband_gc (dum2, ndc_min_x, ndc_min_y, &status, &dum3, &ndc_max_x,
                         &ndc_max_y, &button);
#endif
            if (button EQUALS WM_BUTTON_CANCEL)
                {
                return MD_CANCEL;
                }
            if (fabs (ndc_max_x - ndc_min_x) <=
                twice_margin OR fabs (ndc_max_y - ndc_min_y) <= twice_margin)
                {
                am_message (AM_DIALOG, "Rectangle too small, try again");
                }
            else
                {
                lower_left_box.x = MIN (ndc_min_x, ndc_max_x);
                lower_left_box.y = MIN (ndc_min_y, ndc_max_y);
                upper_right_box.x = MAX (ndc_min_x, ndc_max_x);
                upper_right_box.y = MAX (ndc_min_y, ndc_max_y);
                break;
                }
            }
        am_message (AM_DIALOG, "");
        }
    /* * COMPUTE MAX NDC'S MINUS MARGIN *                                           */
    /* ----------------------------------                                           */
    
    ndc_min_x = lower_left_box.x + screen_margin;
    ndc_min_y = lower_left_box.y + screen_margin;
    ndc_max_x = upper_right_box.x - screen_margin;
    ndc_max_y = upper_right_box.y - screen_margin;
    
    /* * COMPUTE NDC'S TO FIT MAP *                                                 */
    /* ----------------------------                                                 */
    
    screen_ratio = (ndc_max_y - ndc_min_y) / (ndc_max_x - ndc_min_x);
    
    map_ratio = (map->upper_right_xy.y - map->lower_left_xy.y) /
        (map->upper_right_xy.x - map->lower_left_xy.x);
    
    /* USE FULL Y, MODIFY X                                                         */
    if (map_ratio > screen_ratio)
        {
        diff = (ndc_max_x - ndc_min_x) * screen_ratio / map_ratio;
        
        ndc_min_x += ((ndc_max_x - ndc_min_x) - diff) / 2.0;
        ndc_max_x = ndc_min_x + diff;
        lower_left_box.x = ndc_min_x - screen_margin;
        upper_right_box.x = ndc_max_x + screen_margin;
        
        map->wcs_to_inches = (diff / one_inch) / (map->upper_right_xy.x -
                                                  map->lower_left_xy.x);
        }
    /* USE FULL X, MODIFY Y                                                         */
    else if (map_ratio < screen_ratio)
        {
        diff = (ndc_max_y - ndc_min_y) * map_ratio / screen_ratio;
        
        ndc_min_y += ((ndc_max_y - ndc_min_y) - diff) / 2.0;
        ndc_max_y = ndc_min_y + diff;
        lower_left_box.y = ndc_min_y - screen_margin;
        upper_right_box.y = ndc_max_y + screen_margin;
        
        map->wcs_to_inches = (diff / one_inch) / (map->upper_right_xy.y -
                                                  map->lower_left_xy.y);
        }
    /* JUST COMPUTE WCS_TO_INCHES                                                   */
    else
        {
        map->wcs_to_inches = ((ndc_max_y - ndc_min_y) / one_inch) /
            (map->upper_right_xy.y - map->lower_left_xy.y);
        }
    /* * COMPUTE WINDOW EXTENTS IN WCS *                                            */
    /* ---------------------------------                                            */
    
    margin_wcs = MARGIN / map->wcs_to_inches;
    lower_left_wcs.x = map->lower_left_xy.x - margin_wcs;
    lower_left_wcs.y = map->lower_left_xy.y - margin_wcs;
    upper_right_wcs.x = map->upper_right_xy.x + margin_wcs;
    upper_right_wcs.y = map->upper_right_xy.y + margin_wcs;
    
    /* * CREATE THE WINDOW *                                                        */
    /* ---------------------                                                        */
    
    lu_text_to_index ("COLOR", &color, "BACKGROUND");
    wmwallocate (&(map->map_window_id), lower_left_box.x, upper_right_box.x,
                 lower_left_box.y, upper_right_box.y, color);
    line.style = 1;
    line.width = 1.0;
    lu_text_to_index ("COLOR", &line.color, "BLACK");
    wm_draw_window_border (map->map_window_id, &line);
    wmtallocate (&(map->map_tnr), 0.0, 1.0, 0.0, 1.0, lower_left_wcs.x,
                 upper_right_wcs.x, lower_left_wcs.y, upper_right_wcs.y);
    
    /* * DETERMINE PLOT SIZE *                                                      */
    /* -----------------------                                                      */
    
    mp_get_plot_wcs_to_inches (map, &plot_wcs_to_inches);
    plot_width = (upper_right_wcs.x - lower_left_wcs.x) * plot_wcs_to_inches;
    plot_height = (upper_right_wcs.y - lower_left_wcs.y) * plot_wcs_to_inches;
    wm_set_window_size (map->map_window_id, "INCHES", plot_width, plot_height);
    
    wmwclose (map->map_window_id);
    
    /*                                                                              */
    /* *    strcpy(user_string,WM_CLSTR_MAP);                                       */
    /* *    strcat(user_string,": ");                                               */
    /* *    strcat(user_string,map->map_name);                                      */
    /* *    wmoset(map->map_window_id,WM_CLASS_MAP,"",user_string);                 */
    /*                                                                              */
    
    return SUCCESS;
    }
/* END:                                                                             */
