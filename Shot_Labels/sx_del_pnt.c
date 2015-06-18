/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEL_PNT.C*/
/* *7    30-OCT-1990 11:54:37 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *6    17-AUG-1990 22:20:27 VINCE "(SPR 5644) Code Cleanup"*/
/* *5    14-SEP-1989 18:33:57 GORDON "(SPR -1) checkin from gulf"*/
/* *4     9-AUG-1989 10:29:06 JULIAN "(SPR -1) fixing minor casting problem"*/
/* *3    25-JUL-1989 20:10:45 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:03:15 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:28:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEL_PNT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEL_PNT.C                                */
/* *6    17-AUG-1990 22:20:27 VINCE "(SPR 5644) Code Cleanup"                       */
/* *5    14-SEP-1989 18:33:57 GORDON "(SPR -1) checkin from gulf"                   */
/* *4     9-AUG-1989 10:29:06 JULIAN "(SPR -1) fixing minor casting problem"        */
/* *3    25-JUL-1989 20:10:45 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *2    26-JUN-1989 11:03:15 GILLESPIE "(SPR 20) Fix castings"                     */
/* *1    19-JUN-1989 13:28:34 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DEL_PNT.C                                */
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

#include "esi_c_lib.h"
#include "esi_sx.h"

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Delete 1 old shotpoint from cdp xy nlist.                                     */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT sx_delete_point(SX_STRUCT *psx,NLIST_HEADER nlist,NLIST_HEADER nlist_sln,   */
/*        DOUBLE x,DOUBLE y);                                                       */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    psx             -(SX_STRUCT *)                                                */
/*    nlist           -(NLIST_HEADER) Cdp x,y nlist.                                */
/*    nlist_sln       -(NLIST_HEADER) Shot label nlist.                             */
/*    x               -(DOUBLE) -\                                                  */
/*    y               -(DOUBLE) X,y location of cdp.                                */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL    - Unsuccessful completion.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT sx_delete_point (SX_STRUCT *psx, NLIST_HEADER nlist,
                               NLIST_HEADER nlist_sln, DOUBLE x, DOUBLE y)
#else
publicdef INT sx_delete_point (psx, nlist, nlist_sln, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
NLIST_HEADER nlist_sln;
DOUBLE x, y;
#endif
    {
    INT status;
    FLOAT nx, ny;
    CDP ncdp;
    FLOAT fx, fy;
    CDP fcdp;
    FLOAT lx, ly;
    CDP lcdp;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    FLOAT gplx[3], gply[3];
    UINT ngpl;
    UINT npoints;
    CHAR text[128];
    SP fshot;
    UINT ngot, point, stroke, nstrokes;
    NLIST_HEADER temp_list;
    SX_WORKSPACE *w;
    
    /* see if any points                                                            */
    
    if (psx->nshots == 0)
        {
        status = FAIL;
        SX_RETURN;
        }
    /* see if already exists                                                        */
    
    nx = x;
    ny = y;
    status = sx_find_point (psx, nlist, nx, ny);
    
    if (status != SUCCESS)
        {
        sprintf (text, "%s", mg_message (SX_PICK_CLOSER_SP));
        am_message (AM_ALERT, text);
        status = FAIL;
        SX_RETURN;
        }
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *)&w);
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    value_list[0] = (VOIDPTR)&ncdp;
    value_list[1] = (VOIDPTR)&nx;
    value_list[2] = (VOIDPTR)&ny;
    
    nl_get_points (nlist, 1, 3, dim_list, value_list, &ngot);
    nl_inq_current_point (nlist, &point, &stroke);
    nl_inq_stroke_int (nlist, stroke, NL_STROKE_NPOINTS, (INT *)&npoints);
    nl_inq_nlist_int (nlist, NL_NLIST_NSTROKES, (INT *)&nstrokes);
    
    /* convert to shotpoint                                                         */
    
    if (psx->new_line)
        {
        fshot = (ncdp - psx->new_line_intercept) / psx->new_line_slope;
        }
    else
        {
        se_sp_from_cdp (&psx->pso->current_line, ncdp, &fshot);
        }
    ngpl = 3;
    
    /* get previous point                                                           */
    
    if (point != 1)
        {
        value_list[0] = (VOIDPTR)&fcdp;
        value_list[1] = (VOIDPTR)&fx;
        value_list[2] = (VOIDPTR)&fy;
        nl_previous_point (nlist, 3, dim_list, value_list);
        nl_set_current_point (nlist, point, stroke);
        }
    else
        {
        fx = nx;
        fy = ny;
        fcdp = ncdp;
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_ERASE, SX_FIRST_LABEL);
        nl_set_current_point (nlist, point, stroke);
        psx->pso->cdp_xy = temp_list;
        }
    gplx[0] = fx;
    gply[0] = fy;
    gplx[1] = nx;
    gply[1] = ny;
    
    /* get next point                                                               */
    
    if (point != npoints)
        {
        value_list[0] = (VOIDPTR)&lcdp;
        value_list[1] = (VOIDPTR)&lx;
        value_list[2] = (VOIDPTR)&ly;
        nl_next_point (nlist, 3, dim_list, value_list);
        nl_set_current_point (nlist, point, stroke);
        }
    else
        {
        lx = nx;
        ly = ny;
        lcdp = ncdp;
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_ERASE, SX_LAST_LABEL);
        nl_set_current_point (nlist, point, stroke);
        psx->pso->cdp_xy = temp_list;
        }
    gplx[2] = lx;
    gply[2] = ly;
    
    /* erase old line                                                               */
    
    sx_do_draw (psx, nx, ny, ncdp, fshot, SX_ERASE);
    wm_gsplci (psx->graphics.background_color);
    sx_clip_draw (psx, ngpl, gplx, gply);
    
    /* draw new line if not first or last point deleted                             */
    
    if (point != 1 AND point != npoints)
        {
        ngpl = 2;
        gplx[1] = lx;
        gply[1] = ly;
        wm_gsplci (psx->pso->seismic_generic.seismic_line.line.color);
        sx_clip_draw (psx, ngpl, gplx, gply);
        }
    /* delete point from nlist                                                      */
    
    psx->nshots--;
    psx->nchanges++;
    nl_delete_points (nlist, 1);
    nl_delete_points (w->temp_nlist, 1);
    
    /* delete stroke if necessary                                                   */
    
    if (nstrokes > 1 AND npoints == 1)  /* now 0 that we deleted 1                  */
        {
        nl_delete_stroke (nlist, stroke);
        nl_inq_current_point (w->temp_nlist, &point, &stroke);
        nl_delete_stroke (w->temp_nlist, stroke);
        }
    else if (point == 1)
        {
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_VISIBLE, SX_FIRST_LABEL);
        psx->pso->cdp_xy = temp_list;
        }
    else if (point == npoints)
        {
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_VISIBLE, SX_LAST_LABEL);
        psx->pso->cdp_xy = temp_list;
        }
    status = SUCCESS;
    
    /* delete point from seismic label nlist if necessary                           */
    
    nl_inq_nlist_int (nlist_sln, NL_NLIST_NPOINTS, (INT *)&npoints);
    if (npoints == 0)
        {
        SX_RETURN;
        }
    nl_set_current_point (nlist_sln, 1, 1);
    status = so_find_point (nlist, ncdp);
    
    if (status == SUCCESS)
        {
        nl_delete_points (nlist_sln, 1);
        }
    else
        {
        status = SUCCESS;
        }
    SX_RETURN;
    }
/* END:                                                                             */
