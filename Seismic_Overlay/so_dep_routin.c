/* DEC/CMS REPLACEMENT HISTORY, Element SO_DEP_ROUTIN.C*/
/* *4    26-JUL-1990 17:18:39 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *3    25-JUL-1990 20:02:45 VINCE "(SPR 5644) Header Standardization"*/
/* *2    25-JUL-1989 20:04:53 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:26:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_DEP_ROUTIN.C*/
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

/* File Description ---------------------------------------------------------
Overview:
    Seismic overlay - depth conversion routines.                       

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT so_depth_convert(SO_GRID  *pgrid,COORD x,COORD y,DOUBLE datum,
        DOUBLE time,DOUBLE   *depth);

    publicdef INT so_depth_convert_isopach(SO_GRID  *pgrid,COORD x,COORD y,DOUBLE datum,
        INT time1, INT time2,DOUBLE    *depth);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Convert a time to depth for a given x,y, and time.

Prototype:
    publicdef INT so_depth_convert(SO_GRID  *pgrid,COORD x,COORD y,DOUBLE datum,
        DOUBLE time,DOUBLE   *depth);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(COORD) -|
    y               -(COORD) -\
    datum           -(DOUBLE)  datum for which depth will be calculated.
    depth           -(DOUBLE *) Returned depth.
    time            -(DOUBLE) Time for which depth will be calculated.
    pgrid           -(SO_GRID *) Velocity grid structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_depth_convert (SO_GRID *pgrid, COORD x, COORD y, DOUBLE datum,
                                DOUBLE time, DOUBLE *depth)
#else
publicdef INT so_depth_convert (pgrid, x, y, datum, time, depth)
COORD x, y;
DOUBLE datum;
DOUBLE *depth;
DOUBLE time;
SO_GRID *pgrid;
#endif
    {
    DOUBLE vel;
    INT status;
    
    *depth = SO_NULL;
    
    /* if no time, go home */
    
    if (time == SO_NULL)
        {
        return FAIL;
        }
    /* get grid value */
    
    if (NOT (x < pgrid->x_origin OR x > pgrid->xmax OR y < pgrid->y_origin OR y >
             pgrid->ymax))
        {
        status = so_interpolate_grid (pgrid, x, y, &vel);
        if (status == SUCCESS)
            {
            *depth = -(time * vel / 1000.0 - datum);
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Retrieve isopach depth, given interval velocity grid.

Prototype:
    publicdef INT so_depth_convert_isopach(SO_GRID  *pgrid,COORD x,COORD y,DOUBLE datum,
        INT time1, INT time2,DOUBLE    *depth);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    x               -(COORD) -|
    y               -(COORD) -\
    datum           -(DOUBLE) datum for which depth will be calculated. 
    depth           -(DOUBLE *) Returned depth.
    time1           -(INT) Time for which depth will be calculated.
    time2           -(INT) Time for which depth will be calculated.
    pgrid           -(SO_GRID *) Interval velocity grid structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_depth_convert_isopach (SO_GRID *pgrid, COORD x, COORD y,
                                        DOUBLE datum, INT time1, INT time2,
                                        DOUBLE *depth)
#else
publicdef INT so_depth_convert_isopach (pgrid, x, y, datum, time1, time2, depth)
COORD x, y;
DOUBLE datum;
DOUBLE *depth;
INT time1;
INT time2;
SO_GRID *pgrid;
#endif
    {
    DOUBLE vel;
    INT status;
    
    *depth = SO_NULL;
    
    /* if no time, go home */
    
    if (time1 == SO_NULL || time2 == SO_NULL)
        {
        return FAIL;
        }
    /* get grid value */
    
    if (NOT (x < pgrid->x_origin OR x > pgrid->xmax OR y < pgrid->y_origin OR y >
             pgrid->ymax))
        {
        status = so_interpolate_grid (pgrid, x, y, &vel);
        if (status == SUCCESS)
            {
            *depth = (time2 - time1) * vel / 1000.0;
            }
        }
    return status;
    }
/* END:     */
