/* DEC/CMS REPLACEMENT HISTORY, Element SX_CLIP_DRAW.C*/
/* *6    17-AUG-1990 22:20:08 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    14-FEB-1990 13:19:00 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *4    17-JAN-1990 15:40:26 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *3     2-NOV-1989 09:54:38 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:10:18 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_CLIP_DRAW.C*/
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

#include "esi_sx.h"

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#include "esi_nl_meth_hdr.h"

/* Function Description -----------------------------------------------------
Description:
    Draw an array of points and clip by using mm_draw_line.

Prototype:
    publicdef INT sx_clip_draw(SX_STRUCT *psx,INT ngpl,FLOAT *xarray,FLOAT *yarray);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    ngpl            -(INT)
    xarray          -(FLOAT *)
    yarray          -(FLOAT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_clip_draw (SX_STRUCT *psx, INT ngpl, FLOAT *xarray, FLOAT *yarray)
#else
publicdef INT sx_clip_draw (psx, ngpl, xarray, yarray)
SX_STRUCT *psx;
INT ngpl;
FLOAT *xarray;
FLOAT *yarray;
#endif
    {
    INT units[2];
    INT value_inc[2];
    INT data_types[2];
    size_t data_sizes[2];
    VOIDPTR value_list[2];
    UINT dim_list[2];
    INT status = SUCCESS;
    NLIST_HEADER nlist;
    NLM_CLIP_HEADER clip;
    UINT stroke;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;
    static VOIDPTR c_name[2] = 
        {
        "X",
        "Y"
        };
    
    /* set up line */
    
    dim_list[0] = 1, dim_list[1] = 2;
    units[0] = units[1] = 0;
    value_inc[0] = value_inc[1] = VALUE_LIST;
    data_types[0] = data_types[1] = FLOAT_TYPE;
    data_sizes[0] = data_sizes[1] = sizeof(FLOAT);
    
    /* build nlist out of points */
    
    nl_init_nlist (&nlist, 2, units, data_sizes, data_types, value_inc, 0);
    nl_set_nlist_info (nlist, NL_NLIST_COLUMN_NAME, 2, dim_list, c_name);
    nl_start_stroke (nlist, &stroke);
    value_list[0] = (VOIDPTR)xarray;
    value_list[1] = (VOIDPTR)yarray;
    nl_add_points (nlist, ngpl, 2, dim_list, value_list);
    
    /* build clipping window arrays */
    
    clip.Col_List[0] = 1;
    clip.Col_List[1] = 2;
    clip.minList[0] = pmap->lower_left_xy.x;
    clip.minList[1] = pmap->lower_left_xy.y;
    clip.maxList[0] = pmap->upper_right_xy.x;
    clip.maxList[1] = pmap->upper_right_xy.y;
    
    /* clip */
    
    status = nl_invoke_nlist_method (nlist, NL_CLIP_METHOD, &clip);
    
    /* draw */
    
    nl_draw_nlist (clip.outNlist, 1, 2);
    
    /* free */
    
    nl_free_nlist (nlist);
    nl_free_nlist (clip.outNlist);
    SX_RETURN;
    }
/* END:     */
