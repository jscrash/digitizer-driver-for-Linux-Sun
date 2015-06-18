/* DEC/CMS REPLACEMENT HISTORY, Element VI_GRID_INTERP.C */
/* *2    12-JUN-1991 20:19:25 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:06:36 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_GRID_INTERP.C */
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
    Function to perform bi-linear interpolation in grid cell.

Prototype:
    publicdef DOUBLE vi_grid_interp (DOUBLE a, DOUBLE b, DOUBLE c,
                                     DOUBLE d, DOUBLE fx, DOUBLE fy)
Parameters:
    (I) a           -(DOUBLE)   Lower left corner value of cell.
    (I) b           -(DOUBLE)   Lower right corner value of cell.
    (I) c           -(DOUBLE)   Upper right corner value of cell.
    (I) d           -(DOUBLE)   Upper left corner value of cell.
    (I) fx          -(DOUBLE)   Fraction of left-to-right distance
                                to interpolated point.
    (I) fy          -(DOUBLE)   Fraction of bottom-to-top distance
                                to interpolated point.

Return Value/Status:
    Bi-linearly interpolated value.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#include "esi_math.h"

#if USE_PROTOTYPES
publicdef DOUBLE vi_grid_interp (DOUBLE a, DOUBLE b, DOUBLE c,
                                 DOUBLE d, DOUBLE fx, DOUBLE fy)
#else
publicdef DOUBLE vi_grid_interp (a, b, c, d, fx, fy)
DOUBLE a;
DOUBLE b;
DOUBLE c;
DOUBLE d;
DOUBLE fx;
DOUBLE fy;
#endif
    {
    DOUBLE fxr;     /* fxr + fx = 1 */
    DOUBLE fyr;     /* fyr + fy = 1 */
    DOUBLE out;     /* output value */ 
    
    fxr = 1.0 - fx;
    fyr = 1.0 - fy;
    
    /* Interpolate */
    
    out = a * fxr * fyr + b * fx * fyr + c * fx * fy + d * fxr * fy;
    
    return out;
    
    }   /* end of vi_grid_interp */
