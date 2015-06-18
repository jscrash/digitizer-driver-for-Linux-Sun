/* DEC/CMS REPLACEMENT HISTORY, Element SO_INTERP_GRD.C*/
/* *4    26-JUL-1990 17:21:04 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:04:29 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:06:40 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_INTERP_GRD.C*/
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

#include "esi_so.h"
#include "esi_co.h"

/* Function Description -----------------------------------------------------
Description:
    Interpolate a grid to return the best value for x,y values on the grid.

Prototype:
    publicdef INT so_interpolate_grid(SO_GRID  *pgrid,DOUBLE x,DOUBLE y,DOUBLE   *value);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pgrid           -(SO_GRID *) Grid structure.
    x               -(COORD) -\
    y               -(COORD) X, y for which grid value is wanted.
    value           -(DOUBLE *) Extracted grid value.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_interpolate_grid (SO_GRID *pgrid, COORD x, COORD y, DOUBLE *value)
#else
publicdef INT so_interpolate_grid (pgrid, x, y, value)
SO_GRID *pgrid;
COORD x, y;
DOUBLE *value;
#endif
    {
    INT iflgnl = 0;                 /* setgnl variables */
    FLOAT epsnul = 0.0001;
    
    FLOAT xptarr[1], yptarr[1];     /* clchgt variables */
    INT inmxyp = 1;
    FLOAT hgtarr[1];
    
    xptarr[0] = x;
    yptarr[0] = y;
    
    co_setgrd_gc (pgrid->ncol, pgrid->x_origin, pgrid->xmax, pgrid->nrow,
                  pgrid->y_origin, pgrid->ymax);
    co_setgnl_gc (iflgnl, pgrid->null_value, epsnul);
    co_clchgt_gc (xptarr, yptarr, inmxyp, pgrid->array, pgrid->ncol, pgrid->nrow,
                  hgtarr);
    
    *value = hgtarr[0];
    
    return SUCCESS;
    }
/* END:     */
