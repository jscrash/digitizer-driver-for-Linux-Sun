/* DEC/CMS REPLACEMENT HISTORY, Element CO_GRID_SIZE.C*/
/* *2    14-AUG-1990 11:43:08 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:02:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_GRID_SIZE.C*/
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

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

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
    Function to determine the grid interval to use based upon
    the scattered data.

Prototype:
    publicdef INT co_grid_size(CONTOURS *contours, FLOAT *x, FLOAT *y, INT npts);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) contours    -(CONTOURS *) Contour overlay information.
    (I) x           -(FLOAT *) X scatter array.
    (I) y           -(FLOAT *) Y scatter array.
    (I) npts        -(INT) Number of scatterpoints.

Return Value/Status:
    SUCCESS - Successful completion
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT co_grid_size (CONTOURS *contours, FLOAT *x, FLOAT *y, INT npts) 
#else
publicdef INT co_grid_size (contours, x, y, npts)
CONTOURS *contours;
FLOAT *x;
FLOAT *y;
INT npts;
#endif
/* * VARIABLE LIST * */
/* ----------------- */
    {
    
    DOUBLE xmin;
    DOUBLE xmax;
    DOUBLE ymin;
    DOUBLE ymax;
    DOUBLE size;
    DOUBLE dum;
    INT i;
    
    /* ********************************************************************** */
    
    xmin = *x;
    xmax = *x;
    ymin = *y;
    ymax = *y;
    
    x++;
    y++;
    
    for (i = 1; i < npts; i++, x++, y++)
        {
        xmin = MIN (xmin, *x);
        xmax = MAX (xmax, *x);
        ymin = MIN (ymin, *y);
        ymax = MAX (ymax, *y);
        }
    size = sqrt ((xmax - xmin) * (ymax - ymin)) / sqrt ((DOUBLE)npts);
    dum = (DOUBLE)tc_magnitude (size);
    dum = pow (10.0, dum);
    i = size / dum;
    if ((size - (i * dum)) >= (dum / 2.0))
        {
        i++;
        }
    size = i * dum;
    contours->grid_interval = (CHAR *)am_allocate (AM_APPLICATION, 20);
    sprintf (contours->grid_interval, "%lf", size);
    
    return SUCCESS;
    }
/* END:     */
