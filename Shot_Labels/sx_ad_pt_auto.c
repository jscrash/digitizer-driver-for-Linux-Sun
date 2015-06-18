/* DEC/CMS REPLACEMENT HISTORY, Element SX_AD_PT_AUTO.C*/
/* *4    17-AUG-1990 22:19:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 18:32:20 GORDON "(SPR -1) checkin from gulf"*/
/* *2    25-JUL-1989 20:09:21 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_AD_PT_AUTO.C*/
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

#include "esi_c_lib.h"
#include "esi_mg.h"
#include "esi_sx.h"
#include "esi_math.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#include "esi_sx_err.h"

/* Function Description -----------------------------------------------------
Description:
    Add auto new shotpoints to cdp xy nlist.

Prototype:
    publicdef INT sx_add_point_auto(SX_STRUCT *psx,NLIST_HEADER nlist,COORD x,COORD y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    nlist           -(NLIST_HEADER) Cdp x,y nlist.  
    x               -(COORD) -\
    y               -(COORD) X,y location of cdp.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_add_point_auto (SX_STRUCT *psx, NLIST_HEADER nlist, COORD x, COORD y)
#else
publicdef INT sx_add_point_auto (psx, nlist, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
COORD x, y;
#endif
    {
    INT status;
    CDP ncdp;
    FLOAT fx, fy;                   /* 4 byte float */
    CDP fcdp;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    UINT npoints;
    INT i;
    UINT nshots;
    CDP intcdp;
    CHAR text[128];
    DOUBLE dist, deltax, deltay, angle;
    
    /* if auto manual entry, go to it */
    
    if (psx->dialog_spo.shot_spacing_method == SX_MANUAL OR psx->nshots == 0)
        {
        status = sx_add_point (psx, nlist, x, y);
        if (status == SUCCESS)
            {
            psx->dialog_spo.current_shot += (psx->dialog_spo.shot_incr *
                                             psx->dialog_spo.shot_incr_factor);
            }
        SX_RETURN;
        }
    /* auto shot point spacing */
    /* convert to cdp */
    
    if (psx->new_line)
        {
        ncdp = psx->dialog_spo.current_shot * psx->new_line_slope +
               psx->new_line_intercept;
        }
    else
        {
        se_cdp_from_sp (&psx->pso->current_line, psx->dialog_spo.current_shot,
                        &intcdp);
        ncdp = intcdp;
        }
    /* see if already exists */
    
    if (psx->nshots != 0)
        {
        nl_set_current_point (nlist, 1, 1);
        status = so_find_point (nlist, ncdp);
        if (status == SUCCESS)
            {
            sprintf (text, "%s %6.1f", mg_message (SX_ALREADY_EXISTS),
                     psx->dialog_spo.current_shot);
            am_message (AM_DIALOG, text);
            status = FAIL;
            SX_RETURN;
            }
        }
    /* get limits of nlist */
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    value_list[0] = (VOIDPTR) & fcdp;
    value_list[1] = (VOIDPTR) & fx;
    value_list[2] = (VOIDPTR) & fy;
    
    nl_first_point (nlist, 3, dim_list, value_list);
    
    /* determine position in nlist to start from, see if before first cdp */
    /* get last cdp in list */
    
    if (ncdp > fcdp)
        {
        nl_last_point (nlist, 3, dim_list, value_list);
        }
    /* see if in middle */
    
    if (ncdp < fcdp)
        {
        
        /* find the first cdp greater than new cdp */
        
        nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *) & npoints);
        nl_first_point (nlist, 3, dim_list, value_list);
        
        /* (start with 1 because we know cdp > first) */
        
        for (i = 1; i < npoints; i++)
            {
            nl_next_point (nlist, 3, dim_list, value_list);
            if (fcdp > ncdp)
                {
                break;
                }
            }
        /* get previous point */
        
        nl_previous_point (nlist, 3, dim_list, value_list);
        }
    /* add new shotpoints */
    
    deltay = y - fy;
    deltax = x - fx;
    
    dist = hypot (deltay, deltax);
    nshots = dist / (psx->dialog_spo.shot_spacing * psx->dialog_spo.shot_incr);
    
    if (nshots == 0)
        {
        am_message (AM_ALERT, mg_message (SX_DIST_LESS_SPACE));
        status = FAIL;
        SX_RETURN;
        }
    angle = atan2 (deltay, deltax);
    
    deltax = psx->dialog_spo.shot_spacing * psx->dialog_spo.shot_incr * cos (angle);
    deltay = psx->dialog_spo.shot_spacing * psx->dialog_spo.shot_incr * sin (angle);
    
    for (i = 1; i <= nshots; i++)
        {
        fx += deltax;
        fy += deltay;
        status = sx_add_point (psx, nlist, fx, fy);
        if (status != SUCCESS)
            {
            break;
            }
        psx->dialog_spo.current_shot += (psx->dialog_spo.shot_incr *
                                         psx->dialog_spo.shot_incr_factor);
        }
    SX_RETURN;
    }
/* END:     */
