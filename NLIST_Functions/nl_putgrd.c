/* DEC/CMS REPLACEMENT HISTORY, Element NL_PUTGRD.C */
/* *2    12-JUN-1991 19:21:42 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:20:51 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PUTGRD.C */
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
    Store a grid into nlist.

Prototype:
    publicdef INT nl_putgrd(FILENAME os_name, DOUBLE znull,
                            DOUBLE xmin, DOUBLE xmax,
                            DOUBLE ymin, DOUBLE ymax,
                            INT nrows, INT ncols, FLOAT *grid_z,
                            PROJECTION_ID *pid, CHAR *coord_sys,
                            CHAR *flt_nam, CHAR *top_surf,
                            CHAR *base_surf)
Parameters:
    (I) osname      -(FILENAME)Grid name (fullpath name).
    (I) znull       -(DOUBLE)  Null value for grid.
    (I) xmin        -(DOUBLE)  Minimum grid x-value.
    (I) xmax        -(DOUBLE)  Maximum grid x-value.
    (I) ymin        -(DOUBLE)  Minimum grid y-value.
    (I) ymax        -(DOUBLE)  Maximum grid y-value.
    (I) nrows       -(INT)     Number of grid rows.
    (I) ncols       -(INT)     Number of grid columns.
    (I) grid_z      -(FLOAT *) Grid z-values.
    (I) pid         -(PROJECTION_ID *) Id for projection.
    (I) coord_sys   -(CHAR *)  Coordinate system (e.g., "XY") for grid.
    (I) flt_nam     -(CHAR *)  Name of fault used for gridding.
                               If none, use null string.
    (I) top_surf    -(CHAR *)  Top surface name.
                               If none, use null string.
    (I) base_surf   -(CHAR *)  Base surface name.
                               If none, use null string.

Return Value/Status:
    SUCCESS.
    FAIL status from 'fm_init_file' or 'nl_read_nlist'.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* * INCLUDE FILES * */
/* ----------------- */
#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifdef PROTOTYPES
publicdef INT nl_putgrd (FILENAME os_name, DOUBLE znull, 
                         DOUBLE xmin, DOUBLE xmax,
                         DOUBLE ymin, DOUBLE ymax, 
                         INT nrows, INT ncols, FLOAT *grid_z,
                         PROJECTION_ID *pid, CHAR *coord_sys,
                         CHAR *flt_nam, CHAR *top_surf,
                         CHAR *base_surf)
#else
publicdef INT nl_putgrd (os_name, znull, xmin, xmax, ymin, ymax,
                         nrows, ncols, grid_z, pid, coord_sys,
                         flt_nam, top_surf, base_surf)  
FILENAME os_name;                   /* GRID NAME                 */
DOUBLE znull;                       /* NULL VALUE FOR GRID       */
DOUBLE xmin;                        /* MINIMUM GRID X VALUE      */
DOUBLE xmax;                        /* MAXIMUM GRID X VALUE      */
DOUBLE ymin;                        /* MINIMUM GRID Y VALUE      */
DOUBLE ymax;                        /* MAXIMUM GRID Y VALUE      */
INT    nrows;                       /* NUMBER OF GRID ROWS       */
INT    ncols;                       /* NUMBER OF GRID COLUMNS    */
FLOAT *grid_z;                      /* GRIDDED Z VALUES          */
PROJECTION_ID *pid;                 /* PROJECTION ID STRING      */
CHAR  *coord_sys;                   /* COORD STRING FOR GRID     */
CHAR  *flt_nam;                     /* FAULT NAME USED           */
CHAR  *top_surf;                    /* TOP SURFACE NAME          */
CHAR  *base_surf;                   /* BASE SURFACE NAME         */
#endif
{

	/* * VARIABLE LIST * */
	/* ----------------- */
    INT status;                     /* RETURN STATUS       */
    FLOAT *dummy = (FLOAT *)0;      /* DUMMY FLOAT POINTER */
	FLOAT temp;
    INT i;                          /* DUMMY INDEX         */
    UINT j;                         /* DUMMY INDEX         */
    NLIST_HEADER grid_nlist = (NLIST_HEADER)0;  /* NLIST TO STORE GRID_Z */
    GRID_NLIST_USER_STRUCT grid_user;   /* GRID NLIST USER STRUCT        */
    
    /* NLIST INFORMATION */
    /* ----------------- */
    INT units[1];                   /*      UNITS             */
    UINT d_list[1];                 /*      DIM LIST          */
    size_t d_size[1];               /*      DIM SIZE IN BYTES */
    INT d_type[1];                  /*      DIM TYPE          */
    INT v_type[1];                  /*      VALUE TYPE        */
    VOIDPTR v_list[1];              /*      VALUE LIST        */
    static CHAR *c_name[2] = 
        {
        "Z", NULL
        };
    
    /* *******************************************************************/
    
    status = SUCCESS;
    
    /* INITIALIZE GRID NLIST */
    /* --------------------- */   
    units[0] = 0;
    d_size[0] = sizeof(FLOAT);
    d_type[0] = FLOAT_TYPE;
    v_type[0] = VALUE_LIST;
    d_list[0] = 1;
    
    status = nl_init_nlist (&grid_nlist, 1, units, d_size, d_type,
                            v_type, ncols);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
        
    status = nl_set_nlist_info (grid_nlist, NL_NLIST_COLUMN_NAME, 1,
                                d_list, (VOIDPTR)c_name);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }

    temp = (FLOAT)znull;
    v_list[0] = (VOIDPTR)&temp;
    status = nl_set_nlist_info (grid_nlist, NL_NLIST_NULL_VALUE, 1, 
   							    d_list, v_list);
	if (status IS_NOT_EQUAL_TO SUCCESS)
	    {
 	    return status;
 	    }
 	    
    /* * STORE GRID IN NLIST (EACH ROW IS A TABLE) * */
    /* --------------------------------------------- */
    strcpy (grid_user.projection, pid);
    strcpy (grid_user.coord_system, coord_sys);
    
    grid_user.xmin = (FLOAT)xmin;
    grid_user.xmax = (FLOAT)xmax;
    grid_user.ymin = (FLOAT)ymin;
    grid_user.ymax = (FLOAT)ymax;
    grid_user.uniform = TRUE;
    
    strcpy (grid_user.base_surface, base_surf);
    strcpy (grid_user.top_surface, top_surf);
    strcpy (grid_user.fault, flt_nam);
    
    status = nl_set_nlist_user_struct (grid_nlist, &grid_user,
                                       sizeof(GRID_NLIST_USER_STRUCT));
    if (status < 0)
        {
        goto finished;
        }

	/* store the grid now */
	/* ------------------ */
    dummy = grid_z;
    for (i = 0; i < nrows; i++)
        {
        status = nl_start_table (grid_nlist, &j);
        if (status < 0)
            {
            goto finished;
            }

        v_list[0] = (VOIDPTR)dummy;
        
        status = nl_add_rows (grid_nlist, ncols, 1, d_list, v_list);
        if (status < 0)
            {
            goto finished;
            }

        dummy += ncols;
        }

    nl_delete_nlist (os_name, "GRIDS");
        
    status = nl_write_nlist (grid_nlist, os_name, "GRIDS");


    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */    
finished:
    nl_free_nlist (grid_nlist);
    
    return status;
} /* end of nl_putgrd */
