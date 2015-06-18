/* DEC/CMS REPLACEMENT HISTORY, Element VI_WT_AVG.C */
/* *2    12-JUN-1991 20:20:36 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:17:25 VOSSLER "new file 0" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_WT_AVG.C */
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
    Function to perform weighted average filtering on grid data.     

Prototype:
    publicdef INT vi_wt_avg (INT iwt, INT nrows, INT ncols, FLOAT *x)
    
Parameters:
    (I) iwt         -(INT)      Weighting code
                                = 1, average point with values above,
                                     below, left & right
                                = 2, same as (1), but add above left,
                                     above right, below left &
                                     below right.
    (I) nrows       -(INT)      Number rows in grid.
    (I) ncols       -(INT)      Number columns in grid.
    (M) x           -(FLOAT *)  Grid values.

Return Value/Status:
    SUCCESS.
    FAIL code.

Scope:
    PUBLIC

Limitations/Assumptions:
    
-----------------------------------------------------------------------------*/

#include "esi_vi.h"
#include "esi_tc.h"

#if USE_PROTOTYPES
publicdef INT vi_wt_avg (INT iwt, INT nrows, INT ncols, FLOAT *x)
#else
publicdef INT vi_wt_avg (iwt, nrows, ncols, x)
INT iwt;
INT nrows;
INT ncols;
FLOAT *x;
#endif
{
    
    INT status = SUCCESS;
    INT i;
    INT j;
    INT jwt;
    FLOAT *g;                       /* work space           */
    FLOAT *d;                       /* work pointer         */
    FLOAT *in;                      /* work pointer         */
    FLOAT *out;                     /* work pointer         */
    FLOAT work;
    
    jwt = iwt - 1;
    
    /* Get workspace */
    /* ------------- */
    g = (FLOAT *) tc_alloc (nrows * ncols * sizeof(FLOAT));
    if (g EQUALS (FLOAT *)0)
        {
        status = -1;
        return status;
        }

    /* Make working copy of grid */
    /* ------------------------- */
    d = g;
    for (i = 0; i < nrows * ncols; i++)
        {
        *d++ = *x++;
        }
    x -= nrows * ncols;
    
    /* Loop over columns (skip outside boundary for now) */
    /* ------------------------------------------------- */
    in = g + nrows;
    out = x + nrows;
    for (i = 1; i < ncols - 1; i++)
        {
        in += 1;
        out += 1;
        for (j = 1; j < nrows - 1; j++)
            {
            work = *(in - 1) + *in + *(in + 1) + *(in - nrows)
                   + *(in + nrows);
            if (jwt)
                {
                work += *(in - nrows - 1) + *(in - nrows + 1)
                        + *(in + nrows - 1) + *(in + nrows + 1);
                *out = work / 9.;
                }
            else
                {
                *out = work / 5.;
                }
            in++;
            out++;
            }
        in++;
        out++;
        }

    /* Now do 1st and last columns */
    /* --------------------------- */
    in = g;
    out = x;
    for (i = 1; i < nrows - 1; i++)
        {
        in++;
        out++;
        work = *in + *in + *(in - 1) + *(in + 1) + *(in + nrows);
        if (jwt)
            {
            work += *in + *in + *(in + nrows - 1) + *(in + nrows + 1);
            *out = work / 9.;
            }
        else
            {
            *out = work / 5.;
            }
        }
    in = g + (ncols - 1) * nrows;
    out = x + (ncols - 1) * nrows;
    for (i = 1; i < nrows - 1; i++)
        {
        in++;
        out++;
        work = *in + *in + *(in - 1) + *(in + 1) + *(in - nrows);
        if (jwt)
            {
            work += *in + *in + *(in - nrows - 1) + *(in - nrows + 1);
            *out = work / 9.;
            }
        else
            {
            *out = work / 5.;
            }
        }

    /* Now do 1st and last rows */
    /* ------------------------ */
    in = g;
    out = x;
    for (i = 1; i < ncols - 1; i++)
        {
        in += nrows;
        out += nrows;
        work = *in + *in + *(in - nrows) + *(in + 1) + *(in + nrows);
        if (jwt)
            {
            work += *in + *in + *(in - nrows + 1) + *(in + nrows + 1);
            *out = work / 9.;
            }
        else
            {
            *out = work / 5.;
            }
        }
        
    in = g + (nrows - 1);
    out = x + (nrows - 1);
    for (i = 1; i < nrows - 1; i++)
        {
        in += nrows;
        out += nrows;
        work = *in + *in + *(in - 1) + *(in + nrows) + *(in - nrows);
        if (jwt)
            {
            work += *in + *in + *(in - nrows - 1) + *(in + nrows - 1);
            *out = work / 9.;
            }
        else
            {
            *out = work / 5.;
            }
        }

    /* Now do corners */
    /* -------------- */
    in = g;
    out = x;
    work = 3. * (*in) + *(in + 1) + *(in + nrows);
    if (jwt)
        {
        work += 3. * (*in) + *(in + nrows + 1);
        *out = work / 9.;
        }
    else
        {
        *out = work / 5.;
        }
    in = g + nrows - 1;
    out = x + nrows - 1;
    work = 3. * (*in) + *(in - 1) + *(in + nrows);
    if (jwt)
        {
        work += 3. * (*in) + *(in + nrows - 1);
        *out = work / 9.;
        }
    else
        {
        *out = work / 5.;
        }
    in = g + (ncols - 1) * nrows;
    out = x + (ncols - 1) * nrows;
    work = 3. * (*in) + *(in + 1) + *(in - nrows);
    if (jwt)
        {
        work += 3. * (*in) + *(in - nrows + 1);
        *out = work / 9.;
        }
    else
        {
        *out = work / 5.;
        }
    in = g + nrows * ncols - 1;
    out = x + nrows * ncols - 1;
    work = 3. * (*in) + *(in - 1) + *(in - nrows);
    if (jwt)
        {
        work += 3. * (*in) + *(in - nrows - 1);
        *out = work / 9.;
        }
    else
        {
        *out = work / 5.;
        }

    /* Free workspace */
    /* -------------- */
    tc_free (g);
    
    return status;
    
}   /* end of vi_wt_avg */
