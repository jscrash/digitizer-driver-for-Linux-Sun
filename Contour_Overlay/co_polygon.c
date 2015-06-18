/* DEC/CMS REPLACEMENT HISTORY, Element CO_POLYGON.C*/
/* *2    14-AUG-1990 13:05:54 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:43:32 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_POLYGON.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to compute a bounding polygon for the scattered
    data points and to modify the grid to only exist within
    this polygon.

Prototype:
    publicdef INT co_polygon(DOUBLE znull, FLOAT *x, FLOAT *y, FLOAT *z,
                         INT npts, INT nrows, INT ncols, DOUBLE xmin,
                         DOUBLE xmax, DOUBLE ymin, DOUBLE ymax, FLOAT *grid);
                         
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) znull       -(DOUBLE) Null value to use.
    (I) x           -(FLOAT *) X value for scatter points.
    (I) y           -(FLOAT *) Y value for scatter points.
    (I) z           -(FLOAT *) Z value for scatter points.
    (I) npts        -(INT) Number of scatter points.
    (I) nrows       -(INT) Number of grid rows.
    (I) ncols       -(INT) Number of grid columns.
    (O) xmin        -(DOUBLE) X minimum of bounding polygon.
    (O) xmax        -(DOUBLE) X maximum of bounding polygon.
    (O) ymin        -(DOUBLE) Y minimum of bounding polygon.
    (O) ymax        -(DOUBLE) Y maximum of bounding polygon.
    (I) grid        -(FLOAT *) Grid array.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_polygon (DOUBLE znull, FLOAT *x, FLOAT *y, FLOAT *z, INT npts,
                          INT nrows, INT ncols, DOUBLE xmin, DOUBLE xmax, DOUBLE ymin,
                          DOUBLE ymax, FLOAT *grid) 
#else
publicdef INT co_polygon (znull, x, y, z, npts, nrows, ncols, xmin, xmax, ymin, ymax,
                          grid)
DOUBLE znull;
FLOAT *x;
FLOAT *y;
FLOAT *z;
INT npts;
INT nrows;
INT ncols;
DOUBLE xmin;
DOUBLE xmax;
DOUBLE ymin;
DOUBLE ymax;
FLOAT *grid;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    PROGNAME ("CO_POLYGON");
    INT status;                     /* RETURN STATUS */
    FLOAT *x_bnd;                   /* X VALUES FOR BOUNDARY */
    FLOAT *y_bnd;                   /* Y VALUES FOR BOUNDARY */
    INT nbound_in;                  /* SIZE OF X_BND AND Y_BND */
    INT nbound_out;                 /* NUMBER OF COMPUTED BOUNDARY POINTS */
    INT bound_type = 2;             /* BOUNDARY POLYGON TYPE (DGI) */
    INT inside = 1;                 /* INSIDE/OUTSIDE FLAG (DGI) */
    INT mincol;
    INT maxcol;
    INT minrow;
    INT maxrow;
    INT i;
    INT j;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    if (npts > 1)
        {
        
        /* * ALLOCATE BOUNDARY POLYGON * */
        /* ----------------------------- */
        
        nbound_in = MAX (100, 2 * npts);
        x_bnd = (FLOAT *)am_allocate (AM_APPLICATION, nbound_in * sizeof(FLOAT));
        y_bnd = (FLOAT *)am_allocate (AM_APPLICATION, nbound_in * sizeof(FLOAT));
        
        /* * CALCULATE BOUNDARY POLYGON * */
        /* ------------------------------ */
        
        co_clcbnd_gc (x, y, z, npts, bound_type, nbound_in, x_bnd, y_bnd, &nbound_out);
        
        /* * MODIFY GRID * */
        /* --------------- */
        
        co_clcply_gc (x_bnd, y_bnd, nbound_out, inside, znull, grid, ncols, nrows);
        
        /* * FREE BOUNDARY ARRAYS * */
        /* ------------------------ */
        
        am_free (x_bnd);
        am_free (y_bnd);
        }
    else
        {
        mincol = (((*x) - xmin) / ((xmax - xmin) / ncols)) - 1;
        maxcol = mincol + 2;
        minrow = (((*y) - ymin) / ((ymax - ymin) / nrows)) - 1;
        maxrow = minrow + 2;
        
        for (i = 0; i < nrows; i++)
            {
            nbound_in = i * ncols;
            if (i < minrow OR i > maxrow)
                {
                for (j = 0; j < ncols; j++)
                    {
                    grid[(nbound_in + j)] = znull;
                    }
                }
            else
                {
                for (j = 0; j < ncols; j++)
                    {
                    if (j < mincol OR j > maxcol)
                        {
                        grid[(nbound_in + j)] = znull;
                        }
                    }
                }
            }
        }
    return status;
    }
/* END:     */
