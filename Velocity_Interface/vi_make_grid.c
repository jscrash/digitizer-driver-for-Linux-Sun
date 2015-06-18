/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAKE_GRID.C */
/* *2    12-JUN-1991 20:19:53 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:13:05 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAKE_GRID.C */
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
    Function to compute velocity grids.

Prototype:
    publicdef INT vi_make_grid(CHAR *xyz_file, DOUBLE dxmin, DOUBLE dxmax,
                               DOUBLE dymin, DOUBLE dymax, FLOAT **grid_z, 
                               INT *nrows, INT *ncols, DOUBLE *grid_int)
Parameters:
    (I) xyz_file    -(CHAR *)   Name of input scatter file.
    (I) dxmin       -(DOUBLE)   Minimum grid x-value.
    (I) dxmax       -(DOUBLE)   Maximum grid x-value.
    (I) dymin       -(DOUBLE)   Minimum grid y-value.
    (I) dymax       -(DOUBLE)   Maximum grid y-value.
    (O) grid_z      -(FLOAT **) Grid z-values.  Memory allocated here,
                                when through release using 'am_free'.
    (O) nrows       -(INT *)    Number of grid rows.
    (O) ncols       -(INT *)    Number of grid columns.
    (O) grid_int    _(DOUBLE *) Computed grid interval.

Return Value/Status:
    SUCCESS.
    FAIL.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#ifndef ESI_FM_H
#include "esi_fm.h"
#endif

#include "esi_vi.h"
#include "esi_c_lib.h"

#ifndef ESI_OV_H
#include "esi_ov.h"
#endif

#ifndef ESI_MATH_H
#include "esi_math.h"
#endif

#include "esi_co.h"

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_DG_DEFS_H
#include "esi_dg_defs.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifdef PROTOTYPES
publicdef INT vi_make_grid (CHAR *xyz_file, DOUBLE dxmin, DOUBLE dxmax,
                            DOUBLE dymin, DOUBLE dymax, FLOAT **grid_z, 
                            INT *nrows, INT *ncols, DOUBLE *grid_int)
#else
publicdef INT vi_make_grid (xyz_file, dxmin, dxmax, dymin, dymax, grid_z,
                            nrows, ncols, grid_int)
CHAR   *xyz_file;
DOUBLE  dxmin;
DOUBLE  dxmax;
DOUBLE  dymin;
DOUBLE  dymax;
FLOAT **grid_z;
INT    *nrows;
INT    *ncols;
DOUBLE *grid_int;
#endif    
{
    INT status;                     /* RETURN STATUS            */
    NLIST_HEADER scatter;           /* SCATTER FILE NLIST       */
    FLOAT *scat_x = (FLOAT *)0;     /* SCATTER FILE X COORDS    */
    FLOAT *scat_y = (FLOAT *)0;     /* SCATTER FILE Y COORDS    */
    FLOAT *scat_z;                  /* SCATTER FILE Z COORDS    */
    FLOAT *pgrid_z;                 /* LOCAL COPY OF **GRID_Z   */
    INT scat_npts;                  /* NUMBER OF SCATTER POINTS */
    DOUBLE znull;                   /* NULL VALUE FOR GRID      */
    DOUBLE ztol;                    /* TOLERANCE FOR NULL VALUE */
    DOUBLE xmin;                    /* MINIMUM GRID X VALUE     */
    DOUBLE xmax;                    /* MAXIMUM GRID X VALUE     */
    DOUBLE ymin;                    /* MINIMUM GRID Y VALUE     */
    DOUBLE ymax;                    /* MAXIMUM GRID Y VALUE     */
    FILENAME os_name;               /* MAP NAME                 */
    INT i;                          /* DUMMY INDEX              */
    INT iflglg = 1;
    INT iflgrp = 1;
    INT funit;
    INT imxline = 0;
    
    DOUBLE size;
    DOUBLE dum;
    INT *wrk_array;
    INT working_size;
    
    status = SUCCESS;
    
    /* GET SCATTER POINTS */
    /* ------------------ */
    status = ov_open_scatter (&scatter, xyz_file, "r", os_name);
    if (status < 0)
        {
        fprintf (stderr, "VI_MAKE_GRID:\n");
        fprintf (stderr, "    Error-ov_open_scatter: %d\n", status);
        fprintf (stderr, "    -56104: Cant open scatter\n");
        goto finished;
        }

    ov_read_scatter (scatter, &scat_x, &scat_y, &scat_z, &scat_npts);
    ov_close_scatter (scatter, xyz_file, "r", os_name);
    if (scat_npts EQUALS 0)
        {
        fprintf (stderr, "VI_MAKE_GRID:\n");
        fprintf (stderr, "    No scatter data found.\n");
        status = CO_NO_SCATTER_DATA;
        goto finished;
        }

    /* CALCULATE GRID CELL INFORMATION */
    /* ------------------------------- */
    xmin = dxmin;
    xmax = dxmax;
    ymin = dymin;
    ymax = dymax;
    
    size = sqrt ((xmax - xmin) * (ymax - ymin)) / sqrt ((DOUBLE)scat_npts);
    dum = (DOUBLE) tc_magnitude (size);
    dum = pow (10.0, dum);
    i = size / dum;
    if ((size - (i * dum)) >= (dum / 2.0))
        {
        i++;
        }
    size = i * dum;
    *grid_int = size;
    *ncols = (INT)((xmax - xmin) / size) + 1;
    *nrows = (INT)((ymax - ymin) / size) + 1;
    
    /* ALLOCATE SPACE FOR GRID */
    /* ----------------------- */
    *grid_z = (FLOAT *) am_allocate (AM_APPLICATION,
                                     (*ncols * *nrows) * sizeof(FLOAT));
    pgrid_z = *grid_z;
    if (pgrid_z == (FLOAT *)0)
        {
        fprintf (stderr, "VI_MAKE_GRID:\n");
        fprintf (stderr, "    Grid too dense\n");
        status = CO_GRID_TOO_DENSE;
        goto finished;
        }

    /* GET NULL VALUE */
    /* -------------- */
    znull = -2000000000.0;
    ztol = fabs (znull * 0.00001);
    co_bgngrd_gc ();
    co_setgnl_gc (1, znull, ztol);
    
    /* START DGI GRIDDING */
    /* ------------------ */
    co_setglg_gc (iflglg, funit);
    co_setgrp_gc (iflgrp, funit, imxline);
    
    /* SET GRID CELL INFORMATION */
    /* ------------------------- */
    co_setgrd_gc (*ncols, xmin, xmax, *nrows, ymin, ymax);
    
    working_size = 2200 + 10 * (*ncols) * (*nrows);
    
    wrk_array = (INT *) tc_alloc (sizeof(INT) * working_size);
    
    /* GRID THE SCATTER DATA */
    /* --------------------- */
    status = co_clcgrd_gc (scat_x, scat_y, scat_z, scat_npts, 
                           pgrid_z, *ncols, *nrows,
                           wrk_array, working_size);
    if (status < SUCCESS)
        {
        fprintf (stderr, "VI_MAKE_GRID:\n");
        fprintf (stderr, "    Error return from co_clcgrd_gc.\n");
        }

    /* END DGI GRIDDING */
    /* ---------------- */
    co_endgrd_gc ();
    if (status < 0)
        {
        fprintf (stderr, "VI_MAKE_GRID:\n");
        fprintf (stderr, "    Error return from co_endgrd_gc.\n");
        goto finished;
        }

finished:
    am_free (scat_x);
    am_free (scat_y);
    am_free (scat_z);
    
    return status;
}
