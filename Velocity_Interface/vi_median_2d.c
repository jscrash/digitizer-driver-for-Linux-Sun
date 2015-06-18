/* DEC/CMS REPLACEMENT HISTORY, Element VI_MEDIAN_2D.C */
/* *3    25-JUN-1991 11:50:08 DOUG "(SPR 0) Exchanged rows with columns in the arguments per Don Vossler." */
/* *2    12-JUN-1991 20:20:05 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:14:16 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_MEDIAN_2D.C */
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
	Single pass 3 x 3 median filtering for grid data.

Prototype:
    publicdef INT vi_median_2d (INT ncols, INT nrows, FLOAT *x)
    
Parameters:
    (I) ncols       -(INT)      Number columns in grid.
    (I) nrows       -(INT)      Number rows in grid.
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
publicdef INT vi_median_2d (INT ncols, INT nrows, FLOAT *x)
#else
publicdef INT vi_median_2d (ncols, nrows, x)
INT ncols;
INT nrows;
FLOAT *x;
#endif
{
    
    INT status = SUCCESS;
    INT i;
    INT j;
    INT ii;
    INT jj;
    FLOAT *g;                       /* work space           */
    FLOAT *d;                       /* work pointer         */
    FLOAT *in;                      /* work pointer         */
    FLOAT *out;                     /* work pointer         */
    FLOAT work[9];
    FLOAT temp;
    
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
    
    /* Loop thru grid (skip outside boundary for now) */
    /* ---------------------------------------------- */
    in = g + nrows;
    out = x + nrows;
    for (i = 1; i < ncols - 1; i++)
        {
        in += 1;
        out += 1;
        for (j = 1; j < nrows - 1; j++)
            {
            work[0] = *in;
            work[1] = *(in - 1);
            work[2] = *(in + 1);
            work[3] = *(in - nrows);
            work[4] = *(in + nrows);
            work[5] = *(in - nrows - 1);
            work[6] = *(in - nrows + 1);
            work[7] = *(in + nrows - 1);
            work[8] = *(in + nrows + 1);
                
            /* Sort */
            /* ---- */
            for(ii = 8; ii > 0; ii--)
                {
                for(jj = 0; jj < ii; jj++)
                    {
                    if(work[jj] > work[jj + 1])
                        {
                        temp = work[jj];
                        work[jj] = work[jj + 1];
                        work[jj + 1] = temp;
                        }
                    }
                }
                
            *out = work[4];
            
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
        work[0] = *in;
        work[1] = *(in - 1);
        work[2] = *(in + 1);
        work[3] = *(in + nrows);
        work[4] = *(in + nrows - 1);
        work[5] = *(in + nrows + 1);
                
        /* Sort */
        /* ---- */
        for(ii = 5; ii > 0; ii--)
            {
            for(jj = 0; jj < ii; jj++)
                {
                if(work[jj] > work[jj + 1])
                    {
                    temp = work[jj];
                    work[jj] = work[jj + 1];
                    work[jj + 1] = temp;
                    }
                }
            }
                
        *out = work[2];
        }
    in = g + (ncols - 1) * nrows;
    out = x + (ncols - 1) * nrows;
    for (i = 1; i < nrows - 1; i++)
        {
        in++;
        out++;
        work[0] = *in;
        work[1] = *(in - 1);
        work[2] = *(in + 1);
        work[3] = *(in - nrows);
        work[4] = *(in - nrows - 1);
        work[5] = *(in - nrows + 1);
                
        /* Sort */
        /* ---- */
        for(ii = 5; ii > 0; ii--)
            {
            for(jj = 0; jj < ii; jj++)
                {
                if(work[jj] > work[jj + 1])
                    {
                    temp = work[jj];
                    work[jj] = work[jj + 1];
                    work[jj + 1] = temp;
                    }
                }
            }
                
        *out = work[2];
        }

    /* Now do 1st and last rows */
    /* ------------------------ */
    in = g;
    out = x;
    for (i = 1; i < ncols - 1; i++)
        {
        in += nrows;
        out += nrows;
        work[0] = *in;
        work[1] = *(in - nrows);
        work[2] = *(in + nrows);
        work[3] = *(in + 1);
        work[4] = *(in - nrows + 1);
        work[5] = *(in + nrows + 1);
                
        /* Sort */
        /* ---- */
        for(ii = 5; ii > 0; ii--)
            {
            for(jj = 0; jj < ii; jj++)
                {
                if(work[jj] > work[jj + 1])
                    {
                    temp = work[jj];
                    work[jj] = work[jj + 1];
                    work[jj + 1] = temp;
                    }
                }
            }
                
        *out = work[2];
        }
        
    in = g + (nrows - 1);
    out = x + (nrows - 1);
    for (i = 1; i < nrows - 1; i++)
        {
        in += nrows;
        out += nrows;
        work[0] = *in;
        work[1] = *(in - nrows);
        work[2] = *(in + nrows);
        work[3] = *(in - 1);
        work[4] = *(in - nrows - 1);
        work[5] = *(in + nrows - 1);
                
        /* Sort */
        /* ---- */
        for(ii = 5; ii > 0; ii--)
            {
            for(jj = 0; jj < ii; jj++)
                {
                if(work[jj] > work[jj + 1])
                    {
                    temp = work[jj];
                    work[jj] = work[jj + 1];
                    work[jj + 1] = temp;
                    }
                }
            }
                
        *out = work[2];
        }

    /* Now do corners */
    /* -------------- */
    in = g;
    out = x;
    work[0] = *in;
    work[1] = *(in + nrows);
    work[2] = *(in + 1);
    work[3] = *(in + nrows + 1);
                
    /* Sort */
    /* ---- */
    for(ii = 3; ii > 0; ii--)
        {
        for(jj = 0; jj < ii; jj++)
            {
            if(work[jj] > work[jj + 1])
                {
                temp = work[jj];
                work[jj] = work[jj + 1];
                work[jj + 1] = temp;
                }
            }
        }
                
    *out = work[1];
        
    in = g + nrows - 2;
    out = x + nrows - 1;
    work[0] = *in;
    work[1] = *(in + nrows);
    work[2] = *(in + 1);
    work[3] = *(in + nrows + 1);
                
    /* Sort */
    /* ---- */
    for(ii = 3; ii > 0; ii--)
        {
        for(jj = 0; jj < ii; jj++)
            {
            if(work[jj] > work[jj + 1])
                {
                temp = work[jj];
                work[jj] = work[jj + 1];
                work[jj + 1] = temp;
                }
            }
        }
                
    *out = work[1];
        
    in = g + (ncols - 1) * nrows;
    out = x + (ncols - 1) * nrows;
    work[0] = *in;
    work[1] = *(in - nrows);
    work[2] = *(in + 1);
    work[3] = *(in - nrows + 1);
                
    /* Sort */
    /* ---- */
    for(ii = 3; ii > 0; ii--)
        {
        for(jj = 0; jj < ii; jj++)
            {
            if(work[jj] > work[jj + 1])
                {
                temp = work[jj];
                work[jj] = work[jj + 1];
                work[jj + 1] = temp;
                }
            }
        }
                
    *out = work[1];
        
    in = g + nrows * ncols - 2;
    out = x + nrows * ncols - 1;
    work[0] = *in;
    work[1] = *(in - nrows);
    work[2] = *(in + 1);
    work[3] = *(in - nrows + 1);
                
    /* Sort */
    /* ---- */
    for(ii = 3; ii > 0; ii--)
        {
        for(jj = 0; jj < ii; jj++)
            {
            if(work[jj] > work[jj + 1])
                {
                temp = work[jj];
                work[jj] = work[jj + 1];
                work[jj + 1] = temp;
                }
            }
        }
                
    *out = work[1];

    /* Free workspace */
    /* -------------- */
    tc_free (g);
    
    return status;
    
}   /* end of vi_median_2d */
