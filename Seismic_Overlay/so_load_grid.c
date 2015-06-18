/* DEC/CMS REPLACEMENT HISTORY, Element SO_LOAD_GRID.C*/
/* *6    26-JUL-1990 17:21:31 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:05:12 VINCE "(SPR 5644) Header Standardization"*/
/* *4    29-JUN-1990 19:04:17 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *3     2-NOV-1989 09:54:08 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *2    25-JUL-1989 20:07:10 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_LOAD_GRID.C*/
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
#include "esi_so.h"

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

#ifndef ESI_FM_H

#include "esi_fm.h"

#endif

#ifndef ESI_DG_DEFS_H

#include "esi_dg_defs.h"

#endif

#ifndef ESI_OV_ERR_H

#include "esi_ov_err.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to load the grid into grid struct.

Prototype:
    publicdef INT so_load_grid(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    OV_PROJECTION_MISMATCH
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_load_grid (SO_STRUCT *pso)
#else
publicdef INT so_load_grid (pso)
SO_STRUCT *pso;
#endif
    {
    INT status = SUCCESS;
    NLIST_HEADER grid_nlist;        /* nlist to store grid array */
    GRID_NLIST_USER_STRUCT grid_user;   /* nlist user struct */
    CHAR grid_name[256];
    INT i, j;
    UINT dim_list[1];
    VOIDPTR value_list[1];
    FLOAT *array_plus;
    FM_DESCRIB desc;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    /* load the grid */
    
    strcpy (desc, "");
    if ((status = fm_init_file ("ESI$GRIDS", pso->grid.velocity_filename, "GRID", "r",
                                -1, desc, NULL_FUNCTION_PTR, grid_name,
                                "MAPPING")) != SUCCESS)
        {
        return status;
        }
    status = nl_read_nlist (&grid_nlist, grid_name, "GRIDS");
    fm_release_file ("ESI$GRIDS", pso->grid.velocity_filename, "GRID");
    if (status < 0)
        {
        return status;
        }
    /* load the user structure */
    
    nl_inq_nlist_user_struct (grid_nlist, sizeof grid_user, (VOIDPTR) & grid_user,
                              (size_t *) & i);
    if (ARE_DIFFERENT (grid_user.projection,
                       pmap->map_projection) OR ARE_DIFFERENT (grid_user.coord_system,
                                                              pmap->
                                                              map_coordinate_system))
        {
        status = OV_PROJECTION_MISMATCH;
        nl_free_nlist (grid_nlist);
        return status;
        }
    /* get row and col information */
    
    nl_inq_nlist_int (grid_nlist, NL_NLIST_NSTROKES, (INT *) & pso->grid.nrow);
    nl_inq_stroke_int (grid_nlist, 1, NL_STROKE_NPOINTS, (INT *) & pso->grid.ncol);
    
    /* allocate and fill grid */
    
    pso->grid.array = array_plus = (FLOAT *)am_allocate (AM_APPLICATION,
                                                         pso->grid.nrow *
                                                         pso->grid.ncol *
                                                         sizeof(FLOAT));
    
    for (i = 1; i <= pso->grid.nrow; i++)
        {
        nl_set_current_point (grid_nlist, 1, i);
        dim_list[0] = 1, value_list[0] = (VOIDPTR)array_plus;
        nl_get_points (grid_nlist, pso->grid.ncol, 1, dim_list, value_list,
                       (UINT *) & j);
        array_plus += pso->grid.ncol;
        }
    /* GET NULL VALUE */
    
    value_list[0] = (VOIDPTR) & (pso->grid.null_value);
    nl_inq_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, dim_list, value_list);
    
    pso->grid.x_origin = grid_user.xmin;
    pso->grid.y_origin = grid_user.ymin;
    
    pso->grid.xmax = grid_user.xmax;
    pso->grid.ymax = grid_user.ymax;
    
    pso->grid.x_node_size = (pso->grid.xmax - pso->grid.x_origin) / (pso->grid.ncol -
                                                                     1);
    pso->grid.y_node_size = (pso->grid.ymax - pso->grid.y_origin) / (pso->grid.nrow -
                                                                     1);
    
    nl_free_nlist (grid_nlist);
    
    return status;
    }
/* END:     */
