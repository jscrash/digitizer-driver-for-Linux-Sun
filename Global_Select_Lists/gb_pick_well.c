/* DEC/CMS REPLACEMENT HISTORY, Element GB_PICK_WELL.C*/
/* *6    14-AUG-1990 13:26:44 VINCE "(SPR 5644) Header Standardization"*/
/* *5     1-APR-1990 10:29:44 GILLESPIE "(SPR 1) Change mp_get_current_map to mp_front_map"*/
/* *4    17-JAN-1990 17:58:49 GILLESPIE "(SPR 6001) Changes in pick structure"*/
/* *3    10-OCT-1989 09:50:05 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *2    18-JUL-1989 14:47:30 GORDON "(SPR 100) 100"*/
/* *1    19-JUN-1989 12:51:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_PICK_WELL.C*/
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

#include "esi_math.h"
#include "esi_gb.h"
#include "esi_gb_msg.h"
#include "esi_mp.h"
#include "esi_wl.h"
#include "esi_mg.h"
#include "esi_am.h"
#include "esi_we_wells.h"
#include "esi_md_basis.x"
#include "esi_gks.h"

/* Function Description -----------------------------------------------------
Description:
    Function to locate and return the visible well nearest 
    the supplied supplied point (pick_x,pick_y).

Prototype:
    publicdef INT gb_pick_well(DOUBLE pick_x,DOUBLE pick_y,UWI ret_uwi,DOUBLE *ret_x,
        DOUBLE *ret_y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pick_x          -(DOUBLE) X pick coord.
    pick_y          -(DOUBLE) Y pick coord.
    ret_uwi         -(UWI) Returned UWI.
    ret_x           -(DOUBLE *) Returned x coord.
    ret_y           -(DOUBLE *) Returned y coord.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_pick_well (DOUBLE pick_x, DOUBLE pick_y, UWI ret_uwi, DOUBLE *ret_x,
                            DOUBLE *ret_y)
#else
publicdef INT gb_pick_well (pick_x, pick_y, ret_uwi, ret_x, ret_y)
DOUBLE pick_x;
DOUBLE pick_y;
UWI ret_uwi;
DOUBLE *ret_x;
DOUBLE *ret_y;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    INT count;
    INT ndx;
    INT temp_row;
    INT i, j;
    INT ov_num;
    DOUBLE x, y;
    DOUBLE dist;
    DOUBLE test_dist;
    DOUBLE dist_x;
    DOUBLE dist_y;
    BOOL found_one = FALSE;
    MAP_STRUCTURE *map;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    PICKABLE_WELLS *well_picks;
    WELL_STRUCT *pwell;
    CHAR msg[240];
    
    /* ********************************************************************** */
    
    /* * CONVERT PICK POINT COORDINATES FROM MAP TO PROJECT COORDINATES * */
    /* ------------------------------------------------------------------ */
    
    ct_project_xy (pick_x, pick_y, &x, &y);
    
    /* * TRAVERSE MAP OVERLAY BY OVERLAY * */
    /* ----------------------------------- */
    
    mp_front_map (&map);
    display = map->display;
    compiled = map->compiled;
    
    ov_num = 0;
    dist = HUGE_VAL;
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
                if (well_picks->seg_id != 0)
                    {
                    switch (wm_inq_seg_vis (well_picks->seg_id))
                        {
                    case GVISI:
                        for (j = 0; j < well_picks->nused; j++)
                            {
                            pwell = well_picks->list[j].pWell;
                            
                            dist_x = x - pwell->bottomhole_xy.x;
                            dist_y = y - pwell->bottomhole_xy.y;
                            test_dist = (dist_x * dist_x) + (dist_y * dist_y);
                            
                            if (test_dist < dist)
                                {
                                dist = test_dist;
                                found_one = TRUE;
                                count = temp_row;
                                ndx = j;
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
        }
    /* * RETURN FOUND WELL OR ERROR STATUS * */
    /* ------------------------------------- */
    
    if (found_one)
        {
        status = SUCCESS;
        well_picks = (PICKABLE_WELLS *)compiled->picks[count];
        pwell = well_picks->list[ndx].pWell;
        ct_map_xy (pwell->bottomhole_xy.x, pwell->bottomhole_xy.y, ret_x, ret_y);
        strcpy (ret_uwi, pwell->uwi);
        }
    else
        {
        status = FAIL;
        }
    return status;
    }
/* END:     */
