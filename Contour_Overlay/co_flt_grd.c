/* DEC/CMS REPLACEMENT HISTORY, Element CO_FLT_GRD.C*/
/* *2    14-AUG-1990 11:42:22 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:02:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_FLT_GRD.C*/
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

#ifndef ESI_CO_H

#include "esi_co.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Function to allocate the workspace necessary for DGI and
    pass all the arguements to the routine CLCFLT.

Prototype:
    publicdef INT co_faulted_grid(CONTOURS* contours, FLOAT *x, FLOAT *y, FLOAT *z,
                              INT npts, INT nrows, INT ncols, FLOAT *grid);
                              
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay information.
    (I) x           -(FLOAT *) x scatter array.
    (I) y           -(FLOAT *) y scatter array.
    (I) z           -(FLOAT *) z scatter array.
    (I) npts        -(INT) Size of scatter arrays.
    (I) nrows       -(INT) Number of grid rows.
    (I) ncols       -(INT) Number of grid columns.
    (O) grid        -(FLOAT *) Grid array to fill.

Return Value/Status:
    SUCCESS - Successful completion
   
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_faulted_grid (CONTOURS *contours, FLOAT *x, FLOAT *y, FLOAT *z,
                               INT npts, INT nrows, INT ncols, FLOAT *grid) 
#else
publicdef INT co_faulted_grid (contours, x, y, z, npts, nrows, ncols, grid)
CONTOURS *contours;
FLOAT *x;
FLOAT *y;
FLOAT *z;
INT npts;
INT nrows;
INT ncols;
FLOAT *grid;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    PROGNAME ("CO_FAULTED_GRID");
    INT status;                     /* RETURN STATUS */
    INT *workspace;                 /* WORKSPACE FOR DGI */
    INT workspace_size;             /* SIZE OF WORK SPACE */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * ALLOCATE WORKSPACE ( FROM DG_FGALOC @~ LINE 100) * */
    /* ---------------------------------------------------- */
    
    workspace_size = MAX ((ncols * nrows + 3 * npts),
                          (2203 + npts / 7 + (ncols * nrows / 31)));
    workspace = (INT *)am_allocate (AM_APPLICATION, workspace_size * sizeof(INT));
    
    /* * SEND TO DGI * */
    /* --------------- */
    
    if (contours->isopach)
        {
        co_clcisf_gc (x, y, z, npts, grid, ncols, nrows, workspace, workspace_size);
        }
    else
        {
        co_clcflt_gc (x, y, z, npts, grid, ncols, nrows, workspace, workspace_size);
        }
    /* * FREE WORKSPACE * */
    /* ------------------ */
    
    am_free (workspace);
    
    return status;
    }
/* END:     */
