/* DEC/CMS REPLACEMENT HISTORY, Element CO_FLT_CON.C*/
/* *2    14-AUG-1990 11:42:16 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:02:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_FLT_CON.C*/
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
    pass all the arguements to the routine FLTMAP.

Prototype:
    publicdef INT co_faulted_contour(FLOAT *grid, INT nrows, INT ncols);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) grid        -(FLOAT *) Grid array to fill.
    (I) nrows       -(INT) Number of grid rows.
    (I) ncols       -(INT) Number of grid columns.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_faulted_contour (FLOAT *grid, INT nrows, INT ncols) 
#else
publicdef INT co_faulted_contour (grid, nrows, ncols)
FLOAT *grid;
INT nrows;
INT ncols;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    PROGNAME ("CO_FAULTED_CONTOUR");
    INT status;                     /* RETURN STATUS */
    INT *workspace;                 /* WORKSPACE FOR DGI */
    INT workspace_size;             /* SIZE OF WORK SPACE */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * ALLOCATE WORKSPACE * */
    /* ---------------------- */
    
    workspace_size = (ncols * nrows) / 10 + 1;
    workspace = (INT *)am_allocate (AM_APPLICATION, workspace_size * sizeof(INT));
    
    /* * SEND TO DGI * */
    /* --------------- */
    
    co_fltmap_gc (grid, ncols, nrows, workspace, workspace_size);
    
    /* * FREE WORKSPACE * */
    /* ------------------ */
    
    am_free (workspace);
    
    return status;
    }
/* END:     */
