/* DEC/CMS REPLACEMENT HISTORY, Element CO_CONTOUR.C*/
/* *3    15-SEP-1990 13:11:24 CHARLIE "(SPR -1) enhanced perspective options"*/
/* *2    14-AUG-1990 11:41:56 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:01:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_CONTOUR.C*/
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

#ifndef ESI_CO_MSG_H

#include "esi_co_msg.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif


/* Function Description -----------------------------------------------------
Description:
    Function to allocate the workspace necessary for DGI and
    pass all the arguments to the correct DGI contouring
    routine.

Prototype:
    publicdef INT co_contour(CONTOURS *contours, FLOAT *grid, INT ncols,
                         INT nrows);
                         
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contours overlay information.
    (O) grid        -(FLOAT *) Grid array to fill.
    (I) nrows       -(INT) Number of grid rows.
    (I) ncols       -(INT) Number of grid columns.
    
Return Value/Status:
    SUCCESS - Successful completion of call.
    CO_NO_FAULT_DATA - There is no fault data to load.
    The function could also return the value returned by co_set_faults (+/-). 
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_contour (CONTOURS *contours, FLOAT *grid, INT ncols, INT nrows) 
#else
publicdef INT co_contour (contours, grid, ncols, nrows)
CONTOURS *contours;
FLOAT *grid;
INT nrows;
INT ncols;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    PROGNAME ("CO_CONTOUR");
    INT status;                     /* RETURN STATUS */
    INT *workspace;                 /* WORKSPACE FOR DGI */
    INT workspace_size;             /* SIZE OF WORK SPACE */
    BOOL faulted = FALSE;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * CHECK FOR FAULTED CONTOURING * */
    /* -------------------------------- */
    
    if (contours->faulted != NULL)
        {
        status = co_set_faults (contours, 1);
        if (status < 0)
            {
            goto finished;
            }
        else if (status != CO_NO_FAULT_DATA)
            {
            faulted = TRUE;
            }
        }
    /* * ALLOCATE WORKSPACE * */
    /* ---------------------- */
    
    if (contours->suppress_smoothing)
        {
        workspace_size = MAX ((ncols * nrows), 10000);
        }
    else
        {
        workspace_size = 250000 + (ncols * nrows * 6);
        }
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
        switch (contours->view_type)
            {
        case MAP_TYPE:
            co_conmap_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
        case CONTOUR_TYPE:
            co_convue_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
        case CONTOUR_KEY_TYPE:
            co_conkey_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
        case MESH_TYPE:
            co_mshvue_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
	case RIDGE_TYPE:
	    co_rdgvue_gc (grid, ncols, nrows, workspace, workspace_size);
	    break;
	case TRACE_TYPE:
	    co_flttrc_gc (grid, ncols, nrows, workspace, workspace_size);
	    break;
            }
        }
    else
        {
        switch (contours->view_type)
            {
        case MAP_TYPE:
            co_fltmap_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
        case CONTOUR_TYPE:
            co_fltkey_gc (grid, ncols, nrows, workspace, workspace_size);
	    break;
        case CONTOUR_KEY_TYPE:
            co_fltvue_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
        case MESH_TYPE:
            co_fltmsh_gc (grid, ncols, nrows, workspace, workspace_size);
            break;
	case RIDGE_TYPE:
	    co_fltrdg_gc (grid, ncols, nrows, workspace, workspace_size);
	    break;
	case TRACE_TYPE:
	    co_flttrc_gc (grid, ncols, nrows, workspace, workspace_size);
	    break;
            }
        }
    /* * FREE WORKSPACE * */
    /* ------------------ */
    
    am_free (workspace);
    
finished:
    return status;
    }
/* END:     */
