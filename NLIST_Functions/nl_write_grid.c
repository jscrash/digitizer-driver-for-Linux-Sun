/* DEC/CMS REPLACEMENT HISTORY, Element NL_WRITE_GRID.C */
/* *2    12-JUN-1991 19:32:00 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:31:06 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_WRITE_GRID.C */
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
	Store a grid into FINDER DB.

Prototype:
    publicdef INT nl_write_grid(FM_FILE gridname, DOUBLE znull,
                                DOUBLE xmin, DOUBLE xmax,
                                DOUBLE ymin, DOUBLE ymax,
                                INT nrows, INT ncols, FLOAT *grid_z
                                PROJECTION_ID *pid, CHAR *vert_uom,
                                CHAR *flt_nam, CHAR *app_nam,
                                CHAR *top_surf, CHAR *base_surf)
Parameters:
    (I) gridname    -(FM_FILE) Grid name.
    (I) znull       -(DOUBLE)  Null value for grid.
    (I) xmin        -(DOUBLE)  Minimum grid x-value.
    (I) xmax        -(DOUBLE)  Maximum grid x-value.
    (I) ymin        -(DOUBLE)  Minimum grid y-value.
    (I) ymax        -(DOUBLE)  Maximum grid y-value.
    (I) nrows       -(INT)     Number of grid rows.
    (I) ncols       -(INT)     Number of grid columns.
    (I) grid_z      -(FLOAT *) Grid z-values.
    (I) pid         -(PROJECTION_ID *) Id for projection.
    (I) vert_uom    -(CHAR *)  UOM for grid.
    (I) flt_nam     -(CHAR *)  Name of fault used for gridding.
                               If none, use null string.
    (I) app_nam     -(CHAR *)  Name of application.
                               If none, use null string.
    (I) top_surf    -(CHAR *)  Top surface name.
    (I) base_surf   -(CHAR *)  Base surface name.

Return Value/Status:
    SUCCESS.
    FAIL status from 'fm_init_file' or 'nl_read_nlist'.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* * INCLUDE FILES * */
/* ----------------- */
#include "esi_c_lib.h"
#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#if 0
#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif
#endif

#ifndef ESI_FM_H
#include "esi_fm.h"
#endif
#ifndef ESI_FM_ERR_H
#include "esi_fm_err.h"
#endif

#if 0
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#endif

#ifdef PROTOTYPES
publicdef INT nl_write_grid (FM_FILE gridname, DOUBLE znull, 
                            DOUBLE xmin, DOUBLE xmax,
                            DOUBLE ymin, DOUBLE ymax, 
                            INT nrows, INT ncols, FLOAT *grid_z,
                            PROJECTION_ID *pid, CHAR *vert_uom,
                            CHAR *flt_nam, CHAR *app_nam,
                            CHAR *top_surf, CHAR *base_surf)
#else
publicdef INT nl_write_grid (gridname, znull, xmin, xmax, ymin, ymax,                                        nrows, ncols, grid_z, pid, vert_uom,
                             flt_nam, app_nam, top_surf, base_surf)  
FM_FILE gridname;                   /* GRID NAME                 */
DOUBLE znull;                       /* NULL VALUE FOR GRID       */
DOUBLE xmin;                        /* MINIMUM GRID X VALUE      */
DOUBLE xmax;                        /* MAXIMUM GRID X VALUE      */
DOUBLE ymin;                        /* MINIMUM GRID Y VALUE      */
DOUBLE ymax;                        /* MAXIMUM GRID Y VALUE      */
INT    nrows;                       /* NUMBER OF GRID ROWS       */
INT    ncols;                       /* NUMBER OF GRID COLUMNS    */
FLOAT *grid_z;                      /* GRIDDED Z VALUES          */
PROJECTION_ID *pid;                 /* PROJECTION ID STRING      */
CHAR  *vert_uom;                    /* UOM STRING FOR GRID       */
CHAR  *flt_nam;                     /* FAULT NAME USED           */
CHAR  *app_nam;                     /* APPLICATION NAME          */
CHAR *top_surf;                     /* TOP SURFACE NAME          */
CHAR *base_surf;                    /* BASE SURFACE NAME         */
#endif
{

	/* * VARIABLE LIST * */
	/* ----------------- */
    INT status;                     /* RETURN STATUS       */
    FILENAME os_name;               /* INTERNAL NAME       */
    BOOL perm = TRUE;
    CHAR desc[241];

    
    /* *******************************************************************/
    
    status = SUCCESS;


    /* * GET NLIST FILE NAME AND WRITE NLIST * */
    /* --------------------------------------- */    
    desc[0] = '\0';
    if (fm_init_file ("ESI$GRIDS", gridname, "GRID", "w", perm, desc,
                      fm_grid_term_func, os_name, app_nam)EQUALS SUCCESS)
        {
        nl_delete_nlist (os_name, "GRIDS");
        }
    else
        {
        status = CO_CANT_OPEN_GRID;
        return status;
        }
        
    status = nl_putgrd (os_name, znull, xmin, xmax, ymin, ymax,
                        nrows, ncols, grid_z, pid, vert_uom,
                        flt_nam, top_surf, base_surf);
    if(status != SUCCESS)
        {
        return status;
        }
        
    fm_release_file ("ESI$GRIDS", gridname, "GRID");
        

    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */    
finished:
    
    return status;
} /* end of nl_write_grid */
