/* DEC/CMS REPLACEMENT HISTORY, Element NL_READ_GRID.C */
/* *2    12-JUN-1991 19:27:25 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:26:22 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_READ_GRID.C */
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
    publicdef INT nl_read_grid(FM_FILE gridname, DOUBLE *znull,
                               DOUBLE *xmin, DOUBLE *xmax,
                               DOUBLE *ymin, DOUBLE *ymax,
                               DOUBLE *zmin, DOUBLE *zmax,
                               INT *nrows, INT *ncols,
                               FLOAT **grid_z, PROJECTION_ID *pid,
                               CHAR *vert_uom, CHAR *flt_nam,
                               CHAR *top_surf, CHAR *base_surf)
Parameters:
    (I) gridname    -(FM_FILE)  Grid name.
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
    (O) vert_uom    -(CHAR *)   UOM for grid.
    (O) flt_nam     -(CHAR *)   Name of fault used for gridding.
                                If none, use null string.
    (O) top_surf    -(CHAR *)   Top surface name.
                                If none, use null string.
    (O) base_surf   -(CHAR *)   Base surface name.
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

#include "esi_c_lib.h"
#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif
#ifndef ESI_FM_H
#include "esi_fm.h"
#endif
#ifndef ESI_FM_ERR_H
#include "esi_fm_err.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#if USE_PROTOTYPES
publicdef INT nl_read_grid(FM_FILE gridname, DOUBLE *znull, 
                           DOUBLE *xmin, DOUBLE *xmax, 
                           DOUBLE *ymin, DOUBLE *ymax,
                           DOUBLE *zmin, DOUBLE *zmax,
                           INT *nrows, INT *ncols,
                           FLOAT **grid_z, PROJECTION_ID *pid,
                           CHAR *vert_uom, CHAR *flt_nam,
                           CHAR *top_surf, CHAR *base_surf)
#else
publicdef INT nl_read_grid(gridname, znull, xmin, xmax, ymin, ymax,
                           zmin, zmax, nrows, ncols, grid_z,
                           pid, vert_uom, flt_nam,
                           top_surf, base_surf)
FM_FILE gridname;                        /* GRID NAME            */
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
CHAR *vert_uom;
CHAR *flt_nam;
CHAR *top_surf;
CHAR *base_surf;
#endif
{

    /* * VARIABLE LIST * */
    /* ----------------- */
    INT status;                             /* RETURN STATUS */
    FM_DESCRIB desc;
    FILENAME os_name;                       /* GRID FILE NAME */


    /* ********************************************************************** */

    status = SUCCESS;


    /* PRE-PROCESSING FOR GRID CONSTRUCTION * */
    /* -------------------------------------- */
    strcpy(desc,"");
    status = fm_init_file("ESI$GRIDS",gridname,"GRID","r",-1,
		                  desc,NULL_FUNCTION_PTR,os_name,"MAPPING");
		    

    if (status != SUCCESS)
        {
        goto finished;
        }

    status = nl_getgrd(os_name, znull, xmin, xmax, ymin, ymax,
                       zmin, zmax, nrows, ncols, grid_z,
                       pid, vert_uom, flt_nam,
                       top_surf, base_surf);

    fm_release_file("ESI$GRIDS",gridname,"GRID");

    /* * FREE UP ALLOCATED MEMORY * */
    /* ---------------------------- */
finished:

    return status;
} /* end of nl_read_grid */
