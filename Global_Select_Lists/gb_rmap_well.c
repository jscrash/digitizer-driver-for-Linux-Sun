/* DEC/CMS REPLACEMENT HISTORY, Element GB_RMAP_WELL.C */
/* *7    14-AUG-1990 13:27:04 VINCE "(SPR 5644) Header Standardization" */
/* *6     1-APR-1990 10:29:49 GILLESPIE "(SPR 1) Change mp_get_current_map to mp_front_map" */
/* *5    17-JAN-1990 22:06:23 GILLESPIE "(SPR 6001) Changes for new wells struct stuff" */
/* *4    10-OCT-1989 09:50:55 JULIAN "(SPR 5020) mods for CT parameter change" */
/* *3    14-SEP-1989 16:15:33 GORDON "(SPR 100) Gulf/Sun/Unix mods" */
/* *2    11-AUG-1989 20:40:43 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes" */
/* *1    11-AUG-1989 20:08:14 CONROY "Get wells within bounding rectangle on map" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_RMAP_WELL.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_gb.h"
#include "esi_gb_msg.h"
#include "esi_gks.h"
#include "esi_mp.h"
#include "esi_wl.h"
#include "esi_sl.h"
#include "esi_ht.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_we_wells.h"
#include "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Function to locate and return the a hash table of wells on
    the current map which fall within the supplied rectangle.  These 
    may be only the visible wells or all wells currently on the map
    depending on the value of the supplied boolean parameter.
                
    The returned hash table is allocated and initialized by this function.
    Therefore the caller is responsible for freeing the hash table space.
                
    The hash table keys are the well UWI's.
    The hash table data contains the x,y locations of the wells, already
    transformed to the current map projection.

Prototype:
    publicdef INT gb_rect_map_wells (DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, DOUBLE ymax, 
        HASH_TABLE_P *ht, BOOL full);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xmin            -(DOUBLE)
    ymin            -(DOUBLE)
    xmax            -(DOUBLE)
    ymax            -(DOUBLE)
    ht              -(HASH_TABLE_P *)
    full            -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_rect_map_wells (DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, DOUBLE ymax,
                                 HASH_TABLE_P *ht, BOOL full)
#else
publicdef INT gb_rect_map_wells (xmin, ymin, xmax, ymax, ht, full)
DOUBLE xmin;
DOUBLE ymin;
DOUBLE xmax;
DOUBLE ymax;
HASH_TABLE_P *ht;
BOOL full;
#endif
    {
    
    INT status = SUCCESS;           /* RETURN STATUS */
    INT temp_row;
    INT ov_num;
    INT i, j, k;
    DOUBLE xy_out[2];
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    PICKABLE_WELLS *well_picks;
    WELL_STRUCT *pwell;
    SELECT_KEY_STRING uwi;
    BOOL do_test, add;
    CHAR msg[240];
    
    /* ********************************************************************** */
    /* * INITIALIZE HASH TABLE * */
    /* ------------------------- */
    
    if (ht_init_hash_table (50, 10, HT_CHAR_KEY, sizeof(SELECT_KEY_STRING),
                            sizeof(xy_out), HT_SYSTEM_TABLE, ht) != SUCCESS)
        {
        return FAIL;
        }
    /* * GET MAP * */
    /* ----------- */
    
    mp_front_map (&map);
    display = map->display;
    compiled = map->compiled;
    
    /* * CHECK IF NEED TO DO WINDOW TEST * */
    /* ----------------------------------- */
    
    if (xmin > map->lower_left_xy.x OR xmax < map->upper_right_xy.x OR ymin >
        map->lower_left_xy.y OR ymax < map->upper_right_xy.y)
        {
        do_test = TRUE;
        }
    else
        {
        do_test = FALSE;
        add = TRUE;
        }
    /* * TRAVERSE MAP OVERLAY BY OVERLAY * */
    /* ----------------------------------- */
    
    ov_num = 0;
    temp_row = -1;
    for (i = 0; i < display->last_row_used; i++)
        {
        if (display->compiled_row[i] != temp_row)
            {
            if ((temp_row = display->compiled_row[i]) EQUALS - 1)
                {
                continue;
                }
            ov_num++;
            if ((display->overlay_displayed[i]) AND (compiled->
                                                    verb_code[
                                                           temp_row] EQUALS MP_WELLS OR 
                                                    compiled->
                                                 verb_code[
                                                        temp_row] EQUALS MP_WELLDPTH OR 
                                                    compiled->
                                                 verb_code[
                                                         temp_row] EQUALS MP_WELLPACH))
                {
                well_picks = (PICKABLE_WELLS *)compiled->picks[temp_row];
                
                /* GET ALL WELLS IN RECTANGLE */
                /* OR JUST THE VISIBLE WELLS */
                
                k = (full) ? GVISI : wm_inq_seg_vis (well_picks->seg_id);
                switch (k)
                    {
                case GVISI:
                    for (j = 0; j < well_picks->nused; j++)
                        {
                        pwell = well_picks->list[j].pWell;
                        
                        if (do_test)
                            {
                            ct_map_xy (pwell->bottomhole_xy.x, pwell->bottomhole_xy.y,
                                       &xy_out[0], &xy_out[1]);
                            
                            add = ((xmin <= xy_out[0]AND xy_out[0] <=
                                    xmax) AND (ymin <= xy_out[1]AND xy_out[1] <= ymax));
                            }
                        if (add)
                            {
                            tc_zeroes (uwi, sizeof(SELECT_KEY_STRING));
                            strcpy (uwi, pwell->uwi);
                            ht_insert (*ht, uwi, xy_out);
                            }
                        }
                    break;
                    
                case FAIL:
                    sprintf (msg, mg_message (GB_CANT_GET_LIST), "WELLS", ov_num,
                             overlay_verbs[display->verb_ndx[i]].verb);
                    am_message (AM_ALERT, msg);
                    break;
                    
                    }
                }
            }
        }
    return status;
    }
/* END:     */
