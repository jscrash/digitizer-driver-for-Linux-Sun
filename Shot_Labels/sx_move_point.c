/* DEC/CMS REPLACEMENT HISTORY, Element SX_MOVE_POINT.C*/
/* *8    30-OCT-1990 11:55:02 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *7    17-AUG-1990 22:22:16 VINCE "(SPR 5644) Code Cleanup"*/
/* *6    25-MAR-1990 04:26:45 GILLESPIE "(SPR 1) Add more set current row calls"*/
/* *5    24-MAR-1990 14:27:23 GILLESPIE "(SPR 1) change nl_get_rows logic"*/
/* *4    14-SEP-1989 18:36:42 GORDON "(SPR -1) checkin from gulf"*/
/* *3    25-JUL-1989 20:13:50 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *2    26-JUN-1989 11:03:54 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:30:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MOVE_POINT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MOVE_POINT.C                             */
/* *7    17-AUG-1990 22:22:16 VINCE "(SPR 5644) Code Cleanup"                       */
/* *6    25-MAR-1990 04:26:45 GILLESPIE "(SPR 1) Add more set current row calls"    */
/* *5    24-MAR-1990 14:27:23 GILLESPIE "(SPR 1) change nl_get_rows logic"          */
/* *4    14-SEP-1989 18:36:42 GORDON "(SPR -1) checkin from gulf"                   */
/* *3    25-JUL-1989 20:13:50 GILLESPIE "(SPR 50) Seismic system revamp"            */
/* *2    26-JUN-1989 11:03:54 GILLESPIE "(SPR 20) Fix castings"                     */
/* *1    19-JUN-1989 13:30:01 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MOVE_POINT.C                             */
/************************************************************************************/
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

#include "esi_sx.h"

#ifndef ESI_WM_H

#include "esi_wm.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"
#include "sx_cdlgs.rh"

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    Move shotpoint in cdp xy nlist.                                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT sx_move_point(SX_STRUCT    *psx,NLIST_HEADER nlist,DOUBLE x,DOUBLE y);  */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    psx             -(SX_STRUCT *)                                                */
/*    nlist           -(NLIST_HEADER) Cdp x,y nlist.                                */
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
publicdef INT sx_move_point (SX_STRUCT *psx, NLIST_HEADER nlist, DOUBLE x, DOUBLE y)
#else
publicdef INT sx_move_point (psx, nlist, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
DOUBLE x, y;
#endif
    {
    INT status;
    FLOAT nx, ny;
    CDP ncdp;
    FLOAT fx, fy;
    CDP fcdp;                       /* 4 byte int                                   */
    FLOAT lx, ly;                   /* 4 byte float                                 */
    CDP lcdp;
    UINT dim_list[3];
    VOIDPTR var_list[3];
    FLOAT gplx[3], gply[3];         /* 4 BYTE FLOAT                                 */
    UINT ngpl;
    UINT nrows;
    CHAR text[128];
    SP fshot;
    UINT ngot, row, table;
    FLOAT movex, movey;
    INT button;
    NLIST_HEADER temp_list;
    SX_WORKSPACE *w;
    
    /* see if no rows                                                               */
    
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
        am_message (AM_ALERT, mg_message (SX_PICK_CLOSER_SP));
        status = FAIL;
        SX_RETURN;
        }
    status = am_open_workspace ("SX_STATIC_SPACE", AM_GLOBAL, (VOIDPTR *)&w);
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    var_list[0] = (VOIDPTR)&ncdp;
    var_list[1] = (VOIDPTR)&nx;
    var_list[2] = (VOIDPTR)&ny;
    
    /* The 'current' row will move to the next row after the call to                */
    /*    nl_get_rows, so inquire it BEFORE the call to nl_get_rows                 */
    
    nl_inq_current_row (nlist, &row, &table);
    nl_get_rows (nlist, 1, 3, dim_list, var_list, &ngot);
    nl_inq_table_int (nlist, table, NL_TABLE_NROWS, (INT *)&nrows);
    
    /* convert to shotpoint                                                         */
    
    if (psx->new_line)
        {
        fshot = psx->dialog_spo.current_shot = (ncdp - psx->new_line_intercept) /
            psx->new_line_slope;
        }
    else
        {
        se_sp_from_cdp (&psx->pso->current_line, ncdp, &fshot);
        psx->dialog_spo.current_shot = fshot;
        }
    sx_do_draw (psx, nx, ny, ncdp, fshot, SX_BLINK);
    sprintf (text, "%6.1f", psx->dialog_spo.current_shot);
    wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, text);
    
    ngpl = 3;
    
    /* get new row                                                                  */
    
    am_message (AM_DIALOG, mg_message (SX_DIGI_NEW_LOC));
    
    status = sx_locate (psx, &status, &movex, &movey, &button);
    
    am_message (AM_DIALOG, "");
    if (button == WM_BUTTON_CANCEL OR status != SUCCESS)
        {
        wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, "");
        status = FAIL;
        SX_RETURN;
        }
    /* get previous row                                                             */
    
    if (row != 1)
        {
        var_list[0] = (VOIDPTR)&fcdp;
        var_list[1] = (VOIDPTR)&fx;
        var_list[2] = (VOIDPTR)&fy;
        nl_set_current_row (nlist, row, table);
        nl_previous_row (nlist, 3, dim_list, var_list);
        }
    else
        {
        fx = nx;
        fy = ny;
        fcdp = ncdp;
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_ERASE, SX_FIRST_LABEL);
        psx->pso->cdp_xy = temp_list;
        }
    gplx[0] = fx;
    gply[0] = fy;
    gplx[1] = nx;
    gply[1] = ny;
    
    /* get next row                                                                 */
    
    if (row != nrows)
        {
        var_list[0] = (VOIDPTR)&lcdp;
        var_list[1] = (VOIDPTR)&lx;
        var_list[2] = (VOIDPTR)&ly;
        nl_set_current_row (nlist, row, table);
        nl_next_row (nlist, 3, dim_list, var_list);
        nl_set_current_row (nlist, row, table);
        }
    else
        {
        lx = nx;
        ly = ny;
        lcdp = ncdp;
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_ERASE, SX_LAST_LABEL);
        nl_set_current_row (nlist, row, table);
        psx->pso->cdp_xy = temp_list;
        }
    gplx[2] = lx;
    gply[2] = ly;
    
    /* erase old line                                                               */
    
    sx_do_draw (psx, nx, ny, ncdp, fshot, SX_ERASE);
    wm_gsplci (psx->graphics.background_color);
    sx_clip_draw (psx, ngpl, gplx, gply);
    
    /* draw new line                                                                */
    
    if (row != 1 AND row != nrows)
        {
        ngpl = 3;
        gplx[1] = movex;
        gply[1] = movey;
        }
    else
        {
        ngpl = 2;
        gplx[0] = movex;
        gply[0] = movey;
        if (row == nrows)
            {
            gplx[1] = fx;
            gply[1] = fy;
            }
        else
            {
            gplx[1] = lx;
            gply[1] = ly;
            }
        }
    wm_gsplci (psx->pso->seismic_generic.seismic_line.line.color);
    sx_clip_draw (psx, ngpl, gplx, gply);
    
    /* draw new label and symbol                                                    */
    
    sx_do_draw (psx, movex, movey, ncdp, fshot, SX_VISIBLE);
    
    /* replace row in nlist                                                         */
    
    dim_list[0] = 2;
    dim_list[1] = 3;
    var_list[0] = (VOIDPTR)&movex;
    var_list[1] = (VOIDPTR)&movey;
    nl_replace_rows (nlist, 1, 2, dim_list, var_list);
    nl_replace_rows (w->temp_nlist, 1, 2, dim_list, var_list);
    psx->nchanges++;
    
    if (row == nrows)
        {
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_VISIBLE, SX_LAST_LABEL);
        psx->pso->cdp_xy = temp_list;
        }
    else if (row == 1)
        {
        temp_list = psx->pso->cdp_xy;
        psx->pso->cdp_xy = nlist;
        sx_do_line_labels (psx, SX_VISIBLE, SX_FIRST_LABEL);
        psx->pso->cdp_xy = temp_list;
        }
    wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, "");
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:                                                                             */
