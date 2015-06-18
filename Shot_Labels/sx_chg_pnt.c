/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHG_PNT.C*/
/* *4    17-AUG-1990 22:19:59 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    14-SEP-1989 18:33:09 GORDON "(SPR -1) checkin from gulf"*/
/* *2    25-JUL-1989 20:10:03 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_CHG_PNT.C*/
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
#include "esi_wi.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_sx_err.h"
#include "esi_sx_msg.h"
#include "sx_cdlgs.rh"

/* Function Description -----------------------------------------------------
Description:
    Change shotpoint number in cdp xy nlist.

Prototype:
    publicdef INT sx_change_point(SX_STRUCT *psx,NLIST_HEADER nlist,NLIST_HEADER nlist_sln,
        COORD x,COORD y);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    psx             -(SX_STRUCT *)
    nlist           -(NLIST_HEADER) Cdp x,y nlist.
    nlist_sln       -(NLIST_HEADER) Shot label nlist.
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
publicdef INT sx_change_point (SX_STRUCT *psx, NLIST_HEADER nlist,
                               NLIST_HEADER nlist_sln, COORD x, COORD y)
#else
publicdef INT sx_change_point (psx, nlist, nlist_sln, x, y)
SX_STRUCT *psx;
NLIST_HEADER nlist;
NLIST_HEADER nlist_sln;
COORD x, y;
#endif
    {
    INT status;
    FLOAT nx, ny;
    UINT ncdp;
    INT change_cdp;
    UINT dim_list[3];
    VOIDPTR value_list[3];
    UINT npoints;
    INT i;
    CHAR text[128];
    SP fshot;
    UINT ngot, point, stroke;
    NLIST_HEADER temp_list;
    
    /* any points ? */
    
    if (psx->nshots == 0)
        {
        status = FAIL;
        SX_RETURN;
        }
    /* see if already exists */
    
    nx = x;
    ny = y;
    status = sx_find_point (psx, nlist, x, y);
    
    if (status != SUCCESS)
        {
        sprintf (text, "%s", mg_message (SX_PICK_CLOSER_SP));
        am_message (AM_ALERT, text);
        status = FAIL;
        SX_RETURN;
        }
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    value_list[0] = (VOIDPTR) & ncdp;
    value_list[1] = (VOIDPTR) & nx;
    value_list[2] = (VOIDPTR) & ny;
    
    nl_get_points (nlist, 1, 3, dim_list, value_list, &ngot);
    nl_inq_current_point (nlist, &point, &stroke);
    
    /* convert to shotpoint */
    
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
    
    fi_push ();
    i = psx->dialog_spo.current_shot;
    wi_request_integer (mg_message (SX_REQUEST_SHOT), "", 0, 0, i, &i, &status);
    psx->dialog_spo.current_shot = i;
    fi_pop ();
    if (status != SUCCESS)
        {
        sx_do_draw (psx, nx, ny, ncdp, fshot, SX_VISIBLE);
        SX_RETURN;
        }
    /* convert to cdp */
    
    if (psx->new_line)
        {
        change_cdp = psx->dialog_spo.current_shot * psx->new_line_slope +
                     psx->new_line_intercept;
        }
    else
        {
        se_cdp_from_sp (&psx->pso->current_line, psx->dialog_spo.current_shot,
                        &change_cdp);
        }
    /* see if already exists */
    
    nl_set_current_point (nlist, 1, 1);
    status = so_find_point (nlist, change_cdp);
    
    if (status == SUCCESS)
        {
        sprintf (text, "%s %6.1f", mg_message (SX_ALREADY_EXISTS),
                 psx->dialog_spo.current_shot);
        am_message (AM_ALERT, text);
        status = FAIL;
        wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, "");
        SX_RETURN;
        }
    /* erase old label and symbol */
    
    nl_set_current_point (nlist, point, stroke);
    sx_change_delete (psx, nlist);
    
    /* draw new label and symbol */
    
    psx->dialog_spo.add_type = SX_AUTO;
    sx_add_point (psx, nlist, nx, ny);
    nl_inq_nlist_int (nlist, NL_NLIST_NPOINTS, (INT *) & npoints);
    nl_inq_current_point (nlist, &point, &stroke);
    if (point == 1)
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
    wiscdt (psx->cdlg_id.main, ITEM_SLE_SHOTPOINT_EDIT, "");
    
    /* replace point in nlist_sln */
    
    nl_inq_nlist_int (nlist_sln, NL_NLIST_NPOINTS, (INT *) & npoints);
    if (npoints == 0)
        {
        status = SUCCESS;
        SX_RETURN;
        }
    nl_set_current_point (nlist_sln, 1, 1);
    status = so_find_point (nlist_sln, ncdp);
    
    if (status == SUCCESS)
        {
        value_list[0] = (VOIDPTR) & change_cdp;
        nl_replace_points (nlist_sln, 1, 1, dim_list, value_list);
        }
    else
        {
        status = SUCCESS;
        }
    SX_RETURN;
    }
/* END:     */
