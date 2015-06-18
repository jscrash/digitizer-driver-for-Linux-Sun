/* DEC/CMS REPLACEMENT HISTORY, Element NL_GETGRD.C */
/* *2    12-JUN-1991 19:13:10 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:12:14 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_GETGRD.C */
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1991
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/* Function Description -------------------------------------------------------
Description:
    Restore a FINDER grid from disk to memory.

Prototype:
    publicdef INT nl_getgrd(FILENAME os_name, DOUBLE *znull,
                            DOUBLE *xmin, DOUBLE *xmax,
                            DOUBLE *ymin, DOUBLE *ymax,
                            DOUBLE *zmin, DOUBLE *zmax,
                            INT *nrows, INT *ncols,
                            FLOAT **grid_z, PROJECTION_ID *pid,
                            CHAR *coord_sys, CHAR *flt_nam,
                            CHAR *top_surf, CHAR *base_surf)
Parameters:
    (I) os_name     -(FILENAME) Grid name (fullpath name).
    (O) znull       -(DOUBLE *) Null value for grid.
    (O) xmin        -(DOUBLE *) Minimum grid x-value.
    (O) xmax        -(DOUBLE *) Maximum grid x-value.
    (O) ymin        -(DOUBLE *) Minimum grid y-value.
    (O) ymax        -(DOUBLE *) Maximum grid y-value.
    (O) zmin        -(DOUBLE *) Minimum grid z-value.
    (O) zmax        -(DOUBLE *) Maximum grid z-value.
    (O) nrows       -(INT *)    Number of grid rows.
    (O) ncols       -(INT *)    Number of grid columns.
    (O) grid_z      -(FLOAT **) Grid z-values.  Memory allocation
                                done here.  Use 'tc_free' to release
                                memory.
    (O) pid         -(PROJECTION_ID *) Id for projection.
    (O) coord_sys   -(CHAR *)   Coordinate system (e.g., "XY") for grid.
    (O) flt_nam     -(CHAR *)   Name of fault used for gridding.
                                If none, use null string.
    (O) top_surf    -(CHAR *)   Top surface name.
                                If none, use null string.
    (O) base_surf   -(CHAR *)   Base surface name.
                                If none, use null string.

Return Value/Status:
    SUCCESS.
    FAIL status from 'nl_read_nlist'.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* * INCLUDE FILES * */
/* ----------------- */

#include "esi_c_lib.h"
#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#if USE_PROTOTYPES
publicdef INT nl_getgrd(FILENAME os_name, DOUBLE *znull, 
                        DOUBLE *xmin, DOUBLE *xmax, 
                        DOUBLE *ymin, DOUBLE *ymax,
                        DOUBLE *zmin, DOUBLE *zmax,
                        INT *nrows, INT *ncols,
                        FLOAT **grid_z, PROJECTION_ID *pid,
                        CHAR *coord_sys, CHAR *flt_nam,
                        CHAR *top_surf, CHAR *base_surf)
#else
publicdef INT nl_getgrd(os_name, znull, xmin, xmax, ymin, ymax,
                        zmin, zmax, nrows, ncols, grid_z,
                        pid, coord_sys, flt_nam,
                        top_surf, base_surf)
FILENAME os_name;                        /* GRID NAME            */
DOUBLE *znull;                           /* NULL VALUE FOR GRID  */
DOUBLE *xmin;                            /* MINIMUM GRID X VALUE */
DOUBLE *xmax;                            /* MAXIMUM GRID X VALUE */
DOUBLE *ymin;                            /* MINIMUM GRID Y VALUE */
DOUBLE *ymax;                            /* MAXIMUM GRID Y VALUE */
DOUBLE *zmin;                            /* MINIMUM GRID Z VALUE */
DOUBLE *zmax;                            /* MAXIMUM GRID Z VALUE */
INT *nrows;                              /* NUMBER OF GRID ROWS  */
INT *ncols;                              /* NUMBER OF GRID COLUMNS */
FLOAT **grid_z;                          /* GRIDDED Z VALUES     */
                                         /* allocation done here */
                                         /* use am_free to release */
PROJECTION_ID *pid;
CHAR *coord_sys;
CHAR *flt_nam;
CHAR *top_surf;
CHAR *base_surf;
#endif
{

    /* * VARIABLE LIST * */
    /* ----------------- */
    INT status;                             /* RETURN STATUS */
    FLOAT *dummy;                           /* DUMMY FLOAT POINTER */
    FLOAT temp;
    INT i;                                  /* DUMMY INDEX */
    UINT j;                                  /* DUMMY INDEX */
    NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z */
    GRID_NLIST_USER_STRUCT grid_user;       /* NLIST USER STRUCT FOR CONTOURS */

					/* NLIST INFORMATION */
    UINT d_list[1];                         /*      DIM LIST */
    VOIDPTR v_list[1];                      /*      VALUE LIST */
    size_t d_size[1];						/* data field width */
    INT d_type[1];                          /* data type */
    INT units[1];

    /* ********************************************************************** */

    status = SUCCESS;


    status = nl_read_nlist(&grid_nlist, os_name, "GRIDS");
    if (status != SUCCESS)
        {
        goto finished;
        }

    nl_inq_nlist_user_struct(grid_nlist,sizeof(GRID_NLIST_USER_STRUCT),
       			    (VOIDPTR)&grid_user,(size_t *)&i);
    
    strcpy (base_surf, grid_user.base_surface);
    strcpy (top_surf, grid_user.top_surface);
    strcpy (flt_nam, grid_user.fault);
    strcpy (pid, grid_user.projection);
    strcpy (coord_sys, grid_user.coord_system);

    /* GET ROW AND COL INFORMATION */
    /* --------------------------- */
    nl_inq_nlist_int(grid_nlist,NL_NLIST_NSTROKES,nrows);
    nl_inq_stroke_int(grid_nlist,1,NL_STROKE_NPOINTS,ncols);

    /* ALLOCATE AND FILL GRID */
    /* ---------------------- */
    *grid_z = (FLOAT *) tc_alloc((*ncols * (*nrows)) * sizeof(FLOAT));

    dummy = *grid_z;
    d_list[0] = 1;
    for (i=1; i< *nrows+1; i++)
        {
        nl_set_current_point(grid_nlist, 1, i);
        v_list[0] = (VOIDPTR) dummy;
        nl_get_points(grid_nlist, *ncols, 1, d_list, v_list, &j);
        dummy += *ncols;
        }

    /* GET NULL VALUE */
    /* -------------- */
    d_list[0] = 1;
    v_list[0] = (VOIDPTR) &temp;
    nl_inq_nlist_info(grid_nlist, NL_NLIST_NULL_VALUE, 1, d_list, v_list);
    *znull = (DOUBLE)temp;

    /* GET MIN Z-VALUE */
    /* --------------- */
    d_size[0] = sizeof(DOUBLE);
    d_type[0] = FLOAT_TYPE;
    d_list[0] = 1;
    v_list[0] = (VOIDPTR) zmin;
    nl_inq_nlist_info_convert(grid_nlist, NL_NLIST_MIN, 1, d_list,
                              v_list, units, d_size, d_type);

    /* GET MAX Z-VALUE */
    /* --------------- */
    d_size[0] = sizeof(DOUBLE);
    d_type[0] = FLOAT_TYPE;
    d_list[0] = 1;
    v_list[0] = (VOIDPTR) zmax;
    nl_inq_nlist_info_convert(grid_nlist, NL_NLIST_MAX, 1, d_list,
                              v_list, units, d_size, d_type);
                

    /* Return x,y min & max */
    /* -------------------- */
    *xmin = grid_user.xmin;
    *xmax = grid_user.xmax;
    *ymin = grid_user.ymin;
    *ymax = grid_user.ymax;

    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */
    finished:
    nl_free_nlist(grid_nlist);

    return status;
} /* end of nl_getgrd */
