/* DEC/CMS REPLACEMENT HISTORY, Element SO_GET_ENDS.C */
/* *4    26-JUL-1990 17:20:48 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting" */
/* *3    19-JUL-1990 13:25:44 GILLESPIE "(SPR 1) Handle negative sp/cdp ratio" */
/* *2    10-FEB-1990 11:11:29 GILLESPIE "(SPR 1) Remove SQL from so_get_ends.pc - use se_retrieve_header" */
/* *1    10-FEB-1990 11:08:35 GILLESPIE "Replace so_get_ends.pc" */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_GET_ENDS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_GET_ENDS.PC                              */
/* *4    31-OCT-1989 17:23:31 GILLESPIE "(SPR 30) Fix reference to map structure"   */
/* *3    10-OCT-1989 10:01:39 JULIAN "(SPR 5020) Mods for CT parameter change"       */
/* *2    17-AUG-1989 16:50:33 CONROY "(SPR 9999) add survey to where clause of select"  */
/* *1    26-JUL-1989 00:15:42 GILLESPIE "New seismic system mods"                   */
/* DEC/CMS REPLACEMENT HISTORY, Element SO_GET_ENDS.PC                              */

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* NAME:     SO_GET_ENDS.C                                                */
/* AUTHOR:   STEVE SPENCE                                                           */
/* FUNCTION: FUNCTION TO MANAGE SHOTPOINT LABELING AND Z VALUES                     */
/*                                                                                  */
/************************************************************************************/

#include "esi_so.h"
#include "esi_c_lib.h"
#include "esi_math.h"

#ifndef ESI_AG_H
#include "esi_ag.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#if USE_PROTOTYPES
publicdef INT so_get_ends(SO_STRUCT *pso, CDP *first_cdp, CDP *last_cdp, SP *first_sp,
                          SP *last_sp, INT *cdps_per_sp)
#else
publicdef INT so_get_ends(pso, first_cdp, last_cdp, first_sp, last_sp, cdps_per_sp)
SO_STRUCT *pso;                     /* seismic overlay structure                    */
CDP *first_cdp;
CDP *last_cdp;
SP *first_sp;
SP *last_sp;
INT *cdps_per_sp;
#endif
    {
    INT status;
    COORD x1, y1, x2, y2;
    DOUBLE shot_inc;
    DOUBLE precision;
    DOUBLE delta_x, delta_y;
    INT code1, code2;
    INT cdp_inc, delta_n;
    INT cntr;
    CDP test_cdp1, test_cdp2;
    SP sp;
    UINT table;
    UINT Col_List[3];
    VOIDPTR Var_List[3];
    NLIST_HEADER temp_nlist;
    FLOAT fl_x, fl_y;               /* 4 byte floats                                */
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    LINE_HDR *p_hdr;
    
    /********************************************************************************/
    
    status = se_retrieve_header(&pso->current_line, &p_hdr);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    *first_cdp = p_hdr->init_cdp;
    *last_cdp = p_hdr->final_cdp;
    *first_sp = p_hdr->init_sp;
    *last_sp = p_hdr->final_sp;
    *cdps_per_sp = p_hdr->cdp_per_sp;
    
    /* * IF LINE WAS CLIPPED, THEN NEED MORE PROCCESSING *                          */
    /* ---------------------------------------------------                          */
    
    /* *** NOTE: FOR NOW ASSUME THAT THE INITIAL_CDP = THE FIRST POINT IN NLIST
                 AND THE FINAL_CDP = THE LAST POINT IN NLIST *** */
    
    if (pso->clipped)
        {
        Col_List[0] = 1;
        Var_List[0] = (VOIDPTR) & test_cdp2;
        
        /* FIND FIRST CDP                                                           */
        
        nl_first_row(pso->cdp_xy, 1, Col_List, Var_List);
        se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
        ct_map_xy(x2, y2, &x2, &y2);
        cntr = 1;
        
        ag_clip_code(x2, y2, (DOUBLE)pmap->lower_left_xy.x,
                     (DOUBLE)pmap->lower_left_xy.y, (DOUBLE)pmap->upper_right_xy.x,
                     (DOUBLE)pmap->upper_right_xy.y, &code2);
        while (code2 != AG_IN_WINDOW)
            {
            code1 = code2;
            test_cdp1 = test_cdp2;
            x1 = x2;
            y1 = y2;
            status = nl_next_row(pso->cdp_xy, 1, Col_List, Var_List);
            if (status > 0 AND status & NL_EOL)
                {
                /*  If this condition is hit, then we have
                    scanned the entire list without finding a
                    point in the window.  Time to get de hail
                    outta here                  */
                
                return FAIL;
                }
            se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
            ct_map_xy(x2, y2, &x2, &y2);
            cntr++;
            ag_clip_code(x2, y2, (DOUBLE)pmap->lower_left_xy.x,
                         (DOUBLE)pmap->lower_left_xy.y, (DOUBLE)pmap->upper_right_xy.x,
                         (DOUBLE)pmap->upper_right_xy.y, &code2);
            }
        if (cntr != 1)
            {
            delta_n = abs((INT)test_cdp2 - (INT)test_cdp1);
            delta_x = fabs(x2 - x1) / (DOUBLE)delta_n;
            delta_y = fabs(y2 - y1) / (DOUBLE)delta_n;
            if (code1 & AG_LEFT_WINDOW)
                {
                cdp_inc = floor((pmap->lower_left_xy.x - x1) / delta_x) + 1;
                if (code1 & AG_TOP_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1);
                    }
                else if (code1 & AG_BOTTOM_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((pmap->lower_left_xy.y - y1) / delta_y) + 1);
                    }
                }
            else if (code1 & AG_RIGHT_WINDOW)
                {
                cdp_inc = floor((x1 - pmap->upper_right_xy.x) / delta_x) + 1;
                if (code1 & AG_TOP_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1);
                    }
                else if (code1 & AG_BOTTOM_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((pmap->lower_left_xy.y - y1) / delta_y) + 1);
                    }
                }
            else if (code1 & AG_TOP_WINDOW)
                {
                cdp_inc = floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1;
                }
            else if (code1 & AG_BOTTOM_WINDOW)
                {
                cdp_inc = floor((pmap->lower_left_xy.y - y1) / delta_y) + 1;
                }
            *first_cdp = test_cdp1 + cdp_inc;
            *first_sp = p_hdr->init_sp + ((DOUBLE)(*first_cdp - p_hdr->init_cdp) /
                                          (DOUBLE)p_hdr->cdp_per_sp);
            }
        /* FIND LAST CDP                                                            */
        
        nl_last_row(pso->cdp_xy, 1, Col_List, Var_List);
        se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
        ct_map_xy(x2, y2, &x2, &y2);
        cntr = 1;
        
        ag_clip_code(x2, y2, (DOUBLE)pmap->lower_left_xy.x,
                     (DOUBLE)pmap->lower_left_xy.y, (DOUBLE)pmap->upper_right_xy.x,
                     (DOUBLE)pmap->upper_right_xy.y, &code2);
        while (code2 != AG_IN_WINDOW)
            {
            code1 = code2;
            test_cdp1 = test_cdp2;
            x1 = x2;
            y1 = y2;
            status = nl_previous_row(pso->cdp_xy, 1, Col_List, Var_List);
            if (status > 0 AND status & NL_EOL)
                {
                /*  If this condition is hit, then we have
                    scanned the entire list without finding a
                    point in the window.  Time to get de hail
                    outta here                  */
                
                return FAIL;
                }
            se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
            ct_map_xy(x2, y2, &x2, &y2);
            cntr++;
            ag_clip_code(x2, y2, (DOUBLE)pmap->lower_left_xy.x,
                         (DOUBLE)pmap->lower_left_xy.y, (DOUBLE)pmap->upper_right_xy.x,
                         (DOUBLE)pmap->upper_right_xy.y, &code2);
            }
        if (cntr != 1)
            {
            delta_n = abs((INT)test_cdp2 - (INT)test_cdp1);
            delta_x = fabs(x2 - x1) / (DOUBLE)delta_n;
            delta_y = fabs(y2 - y1) / (DOUBLE)delta_n;
            if (code1 & AG_LEFT_WINDOW)
                {
                cdp_inc = floor((pmap->lower_left_xy.x - x1) / delta_x) + 1;
                if (code1 & AG_TOP_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1);
                    }
                else if (code1 & AG_BOTTOM_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((pmap->lower_left_xy.y - y1) / delta_y) + 1);
                    }
                }
            else if (code1 & AG_RIGHT_WINDOW)
                {
                cdp_inc = floor((x1 - pmap->upper_right_xy.x) / delta_x) + 1;
                if (code1 & AG_TOP_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1);
                    }
                else if (code1 & AG_BOTTOM_WINDOW)
                    {
                    cdp_inc = MAX(cdp_inc,
                                  floor((pmap->lower_left_xy.y - y1) / delta_y) + 1);
                    }
                }
            else if (code1 & AG_TOP_WINDOW)
                {
                cdp_inc = floor((y1 - pmap->upper_right_xy.y) / delta_y) + 1;
                }
            else if (code1 & AG_BOTTOM_WINDOW)
                {
                cdp_inc = floor((pmap->lower_left_xy.y - y1) / delta_y) + 1;
                }
            *last_cdp = test_cdp1 - cdp_inc;
            *last_sp = p_hdr->init_sp + ((DOUBLE)(*last_cdp - p_hdr->init_cdp) /
                                         (DOUBLE)p_hdr->cdp_per_sp);
            }
        }
    /* * MAKE SURE CDPS POINT TO SHOT POINTS *                                      */
    /* ---------------------------------------                                      */
    
    shot_inc = 1.0 / (DOUBLE)p_hdr->cdp_per_sp;
    precision = 0.5 * ABS(shot_inc);
    
    FOREVER
        {
        sp = ABS(*first_sp);
        if ((sp - floor(sp)) > precision)
            {
            if ((ceil(sp) - sp) > precision)
                {
                (*first_cdp)++;
                (*first_sp) += shot_inc;
                continue;
                }
            }
        break;
        }
    FOREVER
        {
        sp = ABS(*last_sp);
        if ((sp - floor(sp)) > precision)
            {
            if ((ceil(sp) - sp) > precision)
                {
                (*last_cdp)--;
                (*last_sp) -= shot_inc;
                continue;
                }
            }
        break;
        }

    if (pso->clipped AND pso->seismic_generic.draw_endpoints_only)
        {
        nl_clone_nlist(pso->cdp_xy, &temp_nlist);
        nl_start_table(temp_nlist, &table);
        Var_List[1] = (VOIDPTR) & fl_x;
        Var_List[2] = (VOIDPTR) & fl_y;
        Col_List[1] = 2;
        Col_List[2] = 3;
        
        test_cdp2 = *first_cdp;
        se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
        ct_map_xy(x2, y2, &x2, &y2);
        fl_x = x2;
        fl_y = y2;
        nl_add_rows(temp_nlist, 1, 3, Col_List, Var_List);
        
        test_cdp2 = *last_cdp;
        se_xy_from_cdp(&(pso->current_line), test_cdp2, 0.0, &x2, &y2);
        ct_map_xy(x2, y2, &x2, &y2);
        fl_x = x2;
        fl_y = y2;
        nl_add_rows(temp_nlist, 1, 3, Col_List, Var_List);
        
        nl_free_nlist(pso->cdp_xy);
        pso->cdp_xy = temp_nlist;
        }
    return SUCCESS;
    }
