/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCITM.C*/
/* *3    17-AUG-1990 22:20:32 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    25-JUL-1989 20:10:55 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:28:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_DLGCLPCITM.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Term clp dialog shotpoint label position for 1 shot point.

Prototype:
    publicdef INT sx_dialog_clp_citerm(SX_CLP_STRUCT *pclp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pclp            -(SX_CLP_STRUCT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT sx_dialog_clp_citerm (SX_CLP_STRUCT *pclp)
#else
publicdef INT sx_dialog_clp_citerm (pclp)
SX_CLP_STRUCT *pclp;
#endif
    {
    INT status;
    
    VOIDPTR value_list[3];
    UINT dim_list[3];
    INT sortflags[1];
    NLIST_HEADER sorted_list;
    UINT ngot;
    FLOAT nx, ny;
    CDP_FLOAT shot_point;
    UINT ncdp;
    
    /* dim list duh */
    
    dim_list[0] = 1;
    dim_list[1] = 2;
    dim_list[2] = 3;
    
    nl_set_current_point (pclp->psx->pso->cdp_xy, 1, 1);
    status = so_find_point (pclp->psx->pso->cdp_xy, pclp->cdp);
    
    value_list[0] = (VOIDPTR) & ncdp;
    value_list[1] = (VOIDPTR) & nx;
    value_list[2] = (VOIDPTR) & ny;
    
    nl_get_points (pclp->psx->pso->cdp_xy, 1, 3, dim_list, value_list, &ngot);
    sx_sp_from_cdp (pclp->psx, pclp->cdp, &shot_point);
    sx_do_draw (pclp->psx, nx, ny, pclp->cdp, shot_point, SX_ERASE);
    
    /* see if already special */
    
    nl_inq_nlist_int (pclp->psx->pso->disp_opt.shot_label_nlist, NL_NLIST_NPOINTS,
                      (INT *) & ngot);
    if (ngot == 0)
        {
        status = FAIL;
        }
    else
        {
        nl_set_current_point (pclp->psx->pso->disp_opt.shot_label_nlist, 1, 1);
        status = so_find_point (pclp->psx->pso->disp_opt.shot_label_nlist, pclp->cdp);
        }
    value_list[0] = (VOIDPTR) & pclp->cdp;
    value_list[1] = (VOIDPTR) & pclp->angle;
    value_list[2] = (VOIDPTR) & pclp->paraperp;
    
    /* if already special, do a replace */
    
    if (status == SUCCESS)
        {
        nl_replace_points (pclp->psx->pso->disp_opt.shot_label_nlist, 1, 3, dim_list,
                           value_list);
        }
    else                            /* just do an add */
        {
        nl_add_points (pclp->psx->pso->disp_opt.shot_label_nlist, 1, 3, dim_list,
                       value_list);
        sortflags[0] = NL_ASCENDING;
        nl_sort_nlist (pclp->psx->pso->disp_opt.shot_label_nlist, 1, dim_list,
                       sortflags, &sorted_list);
        nl_free_nlist (pclp->psx->pso->disp_opt.shot_label_nlist);
        pclp->psx->pso->disp_opt.shot_label_nlist = sorted_list;
        }
    if (pclp->paraperp != SO_NOPARA)
        {
        sx_do_draw (pclp->psx, nx, ny, pclp->cdp, shot_point, SX_VISIBLE);
        }
    pclp->psx->nchanges++;
    pclp->psx->need_to_save = TRUE;
    sx_menu_ls_setup (pclp->psx);
    
    status = SUCCESS;
    SX_RETURN;
    }
/* END:     */
