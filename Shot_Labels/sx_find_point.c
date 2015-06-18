/* DEC/CMS REPLACEMENT HISTORY, Element SX_FIND_POINT.C*/
/* *4    17-AUG-1990 22:21:42 VINCE "(SPR 5644) Code Cleanup"*/
/* *3     2-NOV-1989 09:54:53 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:12:26 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_FIND_POINT.C*/
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

#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    Function to find a cdp in a cdp nlist by x and y.

Prototype:
    publicdef INT sx_find_point(SX_STRUCT    *psx,NLIST_HEADER nlist,DOUBLE x,DOUBLE y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    nlist           -(NLIST_HEADER) Nlist header.
    x               -(DOUBLE)
    y               -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_find_point (SX_STRUCT *psx, NLIST_HEADER nlist, DOUBLE x, DOUBLE y)
#else
publicdef INT sx_find_point (psx, nlist, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
DOUBLE x, y;
#endif
    {
    INT status;
    UINT npoints, ndim;
    INT i;
    FLOAT lookx, looky;             /* 4 byte floats */
    UINT dim_list[2];
    VOIDPTR value_list[2];
    DOUBLE dist, save_dist;
    UINT save_point, save_stroke;
    DOUBLE tol, xtol, ytol;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)psx->pso->map;
    
    status = FAIL;
    
    xtol = 0.01 * (pmap->upper_right_xy.x - pmap->lower_left_xy.x);
    ytol = 0.01 * (pmap->upper_right_xy.y - pmap->lower_left_xy.y);
    
    tol = MAX (xtol, ytol);
    
    /* check empty list */
    
    nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *) & npoints);
    
    if (npoints == 0)
        {
        return status;
        }
    dim_list[0] = 2;
    dim_list[1] = 3;
    value_list[0] = (VOIDPTR) & lookx;
    value_list[1] = (VOIDPTR) & looky;
    ndim = 2;
    
    status = nl_first_point (nlist, ndim, dim_list, value_list);
    save_dist = HUGE_VAL;
    save_point = save_stroke = 0;
    
    for (i = 0; i < npoints; i++)
        {
        
        /* do not look at any outside of map */
        
        if (x <= pmap->lower_left_xy.x || x >= pmap->upper_right_xy.x ||
            y <= pmap->lower_left_xy.y || y >= pmap->upper_right_xy.y)
            {
            continue;
            }
        dist = hypot (lookx - x, looky - y);
        if (dist < save_dist AND dist < tol)
            {
            save_dist = dist;
            nl_inq_current_point (nlist, &save_point, &save_stroke);
            }
        nl_next_point (nlist, ndim, dim_list, value_list);
        }
    if (save_point != 0)
        {
        nl_set_current_point (nlist, save_point, save_stroke);
        status = SUCCESS;
        }
    return status;
    }
/* END:     */
