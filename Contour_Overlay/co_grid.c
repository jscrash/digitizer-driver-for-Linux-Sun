/* DEC/CMS REPLACEMENT HISTORY, Element CO_GRID.C*/
/* *3    14-AUG-1990 23:37:50 GILLESPIE "(SPR 5644) Fix up last checkin"*/
/* *2    14-AUG-1990 11:43:03 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:02:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_GRID.C*/
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

#ifndef ESI_CO_H
#include "esi_co.h"
#endif

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to allocate the workspace necessary for DGI and
    pass all the arguments to the correct DGI GRIDDING routine.

Prototype:
    publicdef INT co_grid(CONTOURS *contours, FLOAT *x, FLOAT *y, FLOAT *z,
                      INT npts, INT nrows, INT ncols, INT grid);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour overlay information.
    (I) x           -(FLOAT *) X scatter array.
    (I) y           -(FLOAT *) Y scatter array.
    (I) z           -(FLOAT *) Z scatter array.
    (I) npts        -(INT) Size of scatter arrays.
    (I) nrows       -(INT) Number of grid rows.
    (I) ncols       -(INT) Number of grid columns.
    (I) grid        -(FLOAT *) Grid array to fill.

Return Value/Status:
    SUCCESS - Successful completion
    CO_NO_FAULT_DATA - No fault data.
    Function could also return result of call to co_set_faults (+/-).
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_grid (CONTOURS *contours, FLOAT *x, FLOAT *y, FLOAT *z, INT npts,
                       INT nrows, INT ncols, FLOAT *grid) 
#else
publicdef INT co_grid (contours, x, y, z, npts, nrows, ncols, grid)
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
    
    INT status;                     /* RETURN STATUS */
    BOOL faulted = FALSE;           /* FLAG FOR FAULTED GRIDDING */
    INT *workspace;                 /* WORKSPACE FOR DGI */
    INT workspace_size;             /* SIZE OF WORK SPACE */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * CHECK FOR FAULTED GRIDDING * */
    /* ------------------------------ */
    
    if (contours->faulted != NULL)
        {
        status = co_set_faults (contours, 0);
        if (status < 0)
            {
            goto finished;
            }
        else if (status != CO_NO_FAULT_DATA)
            {
            faulted = TRUE;
            }
        }
    /* * ALLOCATE WORKSPACE  (FROM DG_FGALOC @~ LINE 100) * */
    /* ---------------------------------------------------- */
    
    workspace_size = 2200 + 10 * ncols * nrows;
    
    if (workspace_size > MAX_ALLOC_SPACE)
        workspace_size = MAX_ALLOC_SPACE;
    
    workspace = (INT *)am_allocate (AM_APPLICATION, workspace_size * sizeof(INT));
    while (workspace EQUALS (INT *)NULL)
        {
        workspace_size *= 0.8;
        workspace = (INT *)am_allocate (AM_APPLICATION, workspace_size * sizeof(INT));
        }
    /* * SEND TO DGI * */
    /* --------------- */
    
    if (NOT faulted)
        {
        if (contours->isopach)
            {
            co_clciso_gc (x, y, z, npts, grid, ncols, nrows, workspace,
                          workspace_size);
            }
        else
            {
            co_clcgrd_gc (x, y, z, npts, grid, ncols, nrows, workspace,
                          workspace_size);
            }
        }
    else
        {
        if (contours->isopach)
            {
            co_clcisf_gc (x, y, z, npts, grid, ncols, nrows, workspace,
                          workspace_size);
            }
        else
            {
            co_clcflt_gc (x, y, z, npts, grid, ncols, nrows, workspace,
                          workspace_size);
            }
        }
    /* * FREE WORKSPACE * */
    /* ------------------ */
    
    am_free (workspace);
    
finished:
    return status;
    }
/* END:     */
