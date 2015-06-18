/* DEC/CMS REPLACEMENT HISTORY, Element SX_ADD_POINT.C*/
/* *6    17-AUG-1990 22:19:34 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    15-JAN-1990 09:00:16 GORDON "(SPR 0) fix call to wi_request_int -- pass int ptr, not double ptr"*/
/* *4    14-SEP-1989 18:31:56 GORDON "(SPR -1) checkin from gulf"*/
/* *3    25-JUL-1989 20:09:12 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:02:53 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:27:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_ADD_POINT.C*/
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
#include "esi_sx.h"

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"

/* Function Description -----------------------------------------------------
Description:
    Add 1 new shotpoint to cdp xy nlist.

Prototype:
    publicdef INT sx_add_point(SX_STRUCT *psx,NLIST_HEADER nlist,DOUBLE x,DOUBLE y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    nlist           -(NLIST_HEADER) Cdp x,y nlist.
    x               -(DOUBLE) -\
    y               -(DOUBLE) X,y location of cdp.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_add_point (SX_STRUCT *psx, NLIST_HEADER nlist, DOUBLE x, DOUBLE y)
#else
publicdef INT sx_add_point (psx, nlist, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
DOUBLE x, y;
#endif
    {
    
    INT status;
    INT temp;
    FLOAT nx, ny;
    CDP ncdp;
    FLOAT fx, fy;
    CDP fcdp;
    FLOAT lx, ly;
    CDP lcdp;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    FLOAT gplx[3], gply[3];
    INT ngpl;
    INT npoints, i;
    CHAR text[128];
    SX_WORKSPACE *w;
    
    /* if manual entry, request shot number */
    
    if (psx->dialog_spo.add_type == SX_MANUAL)
        {
        fi_push ();
        wi_request_integer (mg_message (SX_REQUEST_SHOT), "", 0, 0, 0, &temp, &status);
        fi_pop ();
        if (status != SUCCESS)
            {
            SX_RETURN;
            }
        psx->dialog_spo.current_shot = (SP)temp;
        }
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *) & w);
    
    /* convert to cdp */
    
    if (psx->new_line)
        {
        ncdp = psx->dialog_spo.current_shot * psx->new_line_slope +
               psx->new_line_intercept;
        }
    else
        {
        se_cdp_from_sp (&psx->pso->current_line, psx->dialog_spo.current_shot, &ncdp);
        }
    /* see if already exists */
    
    if (psx->nshots == 0)
        {
        
        /* add new point */
        
        nx = x;
        ny = y;
        dim_list[0] = 1;
        dim_list[1] = 2;
        dim_list[2] = 3;
        value_list[0] = (VOIDPTR) & ncdp;
        value_list[1] = (VOIDPTR) & nx;
        value_list[2] = (VOIDPTR) & ny;
        nl_add_points (nlist, 1, 3, dim_list, value_list);
        nl_add_points (w->temp_nlist, 1, 3, dim_list, value_list);
        psx->nchanges++;
        psx->nshots++;
        sx_do_draw (psx, x, y, ncdp, (FLOAT)psx->dialog_spo.current_shot, SX_VISIBLE);
        status = SUCCESS;
        SX_RETURN;
        }
    /* see where it goes */
    
    nl_set_current_point (nlist, 1, 1);
    status = so_find_point (nlist, ncdp);
    
    if (status == SUCCESS)
        {
        sprintf (text, "%s %6.1f", mg_message (SX_ALREADY_EXISTS),
                 psx->dialog_spo.current_shot);
        am_message (AM_ALERT, text);
        status = FAIL;
        SX_RETURN;
        }
    /* new  and y */
    
    nx = x;
    ny = y;
    
    /* get limits of nlist */
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    value_list[0] = (VOIDPTR) & fcdp;
    value_list[1] = (VOIDPTR) & fx;
    value_list[2] = (VOIDPTR) & fy;
    
    nl_first_point (nlist, 3, dim_list, value_list);
    
    /* determine position in nlist, see if before first cdp */
    
    if (ncdp < fcdp)
        {
        value_list[0] = (VOIDPTR) & ncdp;
        value_list[1] = (VOIDPTR) & nx;
        value_list[2] = (VOIDPTR) & ny;
        psx->nchanges++;
        psx->nshots++;
        nl_insert_points (nlist, 1, 3, dim_list, value_list);
        nl_insert_points (w->temp_nlist, 1, 3, dim_list, value_list);
        nl_set_current_point (nlist, 1, 1);
        gplx[0] = fx;
        gply[0] = fy;
        gplx[1] = nx;
        gply[1] = ny;
        ngpl = 2;
        wm_gsplci (psx->pso->seismic_generic.seismic_line.line.color);
        sx_clip_draw (psx, ngpl, gplx, gply);
        sx_do_draw (psx, x, y, ncdp, psx->dialog_spo.current_shot, SX_VISIBLE);
        status = SUCCESS;
        SX_RETURN;
        }
    /* get last cdp in list */
    
    value_list[0] = (VOIDPTR) & lcdp;
    value_list[1] = (VOIDPTR) & lx;
    value_list[2] = (VOIDPTR) & ly;
    
    nl_last_point (nlist, 3, dim_list, value_list);
    
    /* determine position in nlist, see if after last cdp */
    
    if (ncdp > lcdp)
        {
        value_list[0] = (VOIDPTR) & ncdp;
        value_list[1] = (VOIDPTR) & nx;
        value_list[2] = (VOIDPTR) & ny;
        psx->nchanges++;
        psx->nshots++;
        nl_add_points (nlist, 1, 3, dim_list, value_list);
        nl_add_points (w->temp_nlist, 1, 3, dim_list, value_list);
        gplx[0] = lx;
        gply[0] = ly;
        gplx[1] = nx;
        gply[1] = ny;
        ngpl = 2;
        wm_gsplci (psx->pso->seismic_generic.seismic_line.line.color);
        sx_clip_draw (psx, ngpl, gplx, gply);
        sx_do_draw (psx, x, y, ncdp, psx->dialog_spo.current_shot, SX_VISIBLE);
        status = SUCCESS;
        SX_RETURN;
        }
    /* it goes in the middle somewhere */
    /* find the first cdp greater than new cdp */
    
    value_list[0] = (VOIDPTR) & lcdp;
    value_list[1] = (VOIDPTR) & lx;
    value_list[2] = (VOIDPTR) & ly;
    
    nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, &npoints);
    nl_first_point (nlist, 3, dim_list, value_list);
    
    /* (start with 1 because we know cdp > first) */
    
    for (i = 1; i < npoints; i++)
        {
        nl_next_point (nlist, 3, dim_list, value_list);
        if (lcdp > ncdp)
            {
            break;
            }
        }
    /* get previous point */
    
    value_list[0] = (VOIDPTR) & fcdp;
    value_list[1] = (VOIDPTR) & fx;
    value_list[2] = (VOIDPTR) & fy;
    
    nl_previous_point (nlist, 3, dim_list, value_list);
    
    /* add new point */
    
    value_list[0] = (VOIDPTR) & ncdp;
    value_list[1] = (VOIDPTR) & nx;
    value_list[2] = (VOIDPTR) & ny;
    
    psx->nchanges++;
    psx->nshots++;
    nl_add_points (nlist, 1, 3, dim_list, value_list);
    nl_add_points (w->temp_nlist, 1, 3, dim_list, value_list);
    
    /* erase old line */
    
    gplx[0] = fx;
    gply[0] = fy;
    gplx[1] = lx;
    gply[1] = ly;
    ngpl = 2;
    
    wm_gsplci (psx->graphics.background_color);
    sx_clip_draw (psx, ngpl, gplx, gply);
    
    /* draw new line */
    
    gplx[1] = nx;
    gply[1] = ny;
    gplx[2] = lx;
    gply[2] = ly;
    ngpl = 3;
    
    wm_gsplci (psx->pso->seismic_generic.seismic_line.line.color);
    sx_clip_draw (psx, ngpl, gplx, gply);
    sx_do_draw (psx, x, y, ncdp, psx->dialog_spo.current_shot, SX_VISIBLE);
    status = SUCCESS;
    SX_RETURN;
    }
/* END:     */
