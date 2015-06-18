/* DEC/CMS REPLACEMENT HISTORY, Element SE_MT_INIT.C */
/* *2    12-JUN-1991 19:34:53 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 19:33:54 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element SE_MT_INIT.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1990                          */
/*                    Unpublished -- All rights reserved                            */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc. AND MAY*/
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Finder Graphics Systems                               */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_se.h"
#include "esi_tc.h"
static INT se_mt_ndx ();

/*---------------------------------------------------------------------------*/
static INT rline;               /* reference line_id                            */
static INT rloc;                /* sequential location of 'rline' in 'lin'      */
static INT *lin;                /* pointer to allocated line storage            */
static INT nlin;                /* length of 'lin'                              */
static INT ndx_1st;             /* start index for binary search                */
static INT inc_1st;             /* index increment                              */
static DOUBLE *a;               /* pointer to allocated lst sqrs lhs array      */
static DOUBLE *b;               /* pointer to allocated lst sqrs rhs array      */
/*---------------------------------------------------------------------------*/

/* Function Description -------------------------------------------------------
Description:
    Initializes data management for seismic mis-tie computation.

Prototype:
    publicdef VOID se_mt_init(INT ref_line, INT *lines)

Parameters:
    (I) ref_line    -(INT)  Lineid of line that all other lines will be
                            corrected with respect to; i.e., this is the
                            line that will have zero correction applied.
    (I) lines       -(INT)  Array of line numbers.
    (I) num_lines   -(INT)  Length of 'lines' array.

Return Value/Status:
    None

Scope:
    PUBLIC
 
Limitations/Assumptions:
    This routine allocates space for a least squares solution for static
    mis-tie corrections for each line.  All corrections are relative to
    the specified reference line.

-----------------------------------------------------------------------------*/


#if USE_PROTOTYPES
publicdef VOID se_mt_init (INT ref_line, INT *lines, INT num_lines)
#else
publicdef VOID se_mt_init (ref_line, lines, num_lines)
INT ref_line;
INT *lines;
INT num_lines;
#endif
    {
    INT i;                          /* loop index */
    INT j;                          /* loop index */
    INT lndx;                       /* loop index */
    INT temp;                       /* temporary value storeage */
    
    /* Save reference line id and line count */
    /* ------------------------------------- */
    rline = ref_line;
    nlin = num_lines;
    
    /* Allocate necessary storage */
    /* -------------------------- */
    lin = (INT *) tc_alloc (nlin * sizeof(INT));
    b = (DOUBLE *) tc_zalloc (nlin * sizeof(DOUBLE));
    a = (DOUBLE *) tc_zalloc (nlin * nlin * sizeof(DOUBLE));
    
    /* Save line id array */
    /* ------------------ */
    for (i = 0; i < nlin; i++)
        {
        *lin++ = *lines++;
        }
    lin -= nlin;
    
    /* Sort line number array */
    /* ---------------------- */
    for (i = 0, lndx = nlin - 1; lndx > 0; --lndx)
        {
        for (j = 0; j < lndx; j++)
            {
            if (lin[j] > lin[j + 1])
                {
                temp = lin[j];
                lin[j] = lin[j + 1];
                lin[j + 1] = temp;
                }
            }
        }
    /* Save rline location in lin */
    /* -------------------------- */
    for (i = 0; i < nlin; i++)
        {
        if (rline EQUALS lin[i])
            {
            rloc = i;
            break;
            }
        }
    /* Calc and save binary search start index and increment */
    /* ----------------------------------------------------- */
    ndx_1st = nlin / 2;
    inc_1st = MAX (nlin / 4, 1);
    
    /* Initialization complete */
    /* ----------------------- */
    return;
    }

/*****************************************************************************/

/* Function Description -------------------------------------------------------
Description:
    Accumulates data on line-by-line basis.

Prototype:
    publicdef VOID se_mt_bld(INT lineno, INT num_xlines, INT *xlines,
                              DOUBLE *lin_z, DOUBLE *x_z)

Parameters:
    (I) lineno      -(INT)  Lineid of line for which data is being entered.
    (I) num_xlines  -(INT)  Length of 'xlines','lin_z','x_z' arrays.
                            This is also the count of intersections of other
                            lines with 'lineno'.
    (I) xlines      -(INT)  Array of crossing line numbers.
    (I) lin_z       -(DOUBLE) Array of z-values where 'xlines' intersect.
                            These are taken from 'lineno'.
    (I) x_z         -(DOUBLE) Cross line z-value array.

Return Value/Status:
    None

Scope:
    PUBLIC
 
Limitations/Assumptions:
    This routine accumulates the intersection data on a line-by-line basis
    in preparation for the least squares analysis.

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef VOID se_mt_bld (INT lineno, INT num_xlines, INT *xlines, DOUBLE *lin_z,
                          DOUBLE *x_z)
#else
publicdef VOID se_mt_bld (lineno, num_xlines, xlines, lin_z, x_z)
INT lineno;
INT num_xlines;
INT *xlines;
DOUBLE *lin_z;
DOUBLE *x_z;
#endif
    {
    INT ndx;
    INT ndx2;
    INT i;
    INT j;
    
    /* Get lineno index in saved line array */
    /* ------------------------------------ */
    ndx = se_mt_ndx (lineno);
    
    ndx2 = ndx * nlin;
    
    /* Now loop thru data */
    /* ------------------ */
    for (i = 0; i < num_xlines; i++)
        {
        j = se_mt_ndx (xlines[i]);
        a[ndx2 + ndx] += 1.;
        a[ndx2 + j] -= 1.;
        b[ndx] += x_z[i] - lin_z[i];
        }
    /* Done for this line */
    /* ------------------ */
    return;
    }

/*****************************************************************************/

/* Function Description -------------------------------------------------------
Description:
    Get index of line value within line array.

Prototype:
    static INT se_mt_ndx(INT lineno)

Parameters:
    (I) lineno      -(INT)  Lineid for which index is desired.

Return Value/Status:
    Index where 'lineno' found in saved line number array.

Scope:
    PRIVATE
 
Limitations/Assumptions:
    This routine does a binary search in the line number array.

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
static INT se_mt_ndx (INT lineno)
#else
static INT se_mt_ndx (lineno)
INT lineno;
#endif
    {
    INT ndx;                        /* search index */
    INT inc;                        /* index increment */
    INT i;                          /* loop index */
    
    /* Set initial search index and increment value */
    /* -------------------------------------------- */
    ndx = ndx_1st;
    inc = inc_1st;
    
    /* Now loop over data */
    /* ------------------ */
    for (i = 0; i < nlin; i++)
        {
        if (lin[ndx] < lineno)
            {
            ndx += inc;
            if (ndx >= nlin)
                {
                ndx = nlin - 1;
                }
            }
        else if (lin[ndx] > lineno)
            {
            ndx -= inc;
            if (ndx < 0)
                {
                ndx = 0;
                }
            }
        else
            {
            break;
            }
        inc = (inc + 1) / 2;
        }
    /* Now return index value */
    /* ---------------------- */
    return ndx;
    }

/*****************************************************************************/

/* Function Description -------------------------------------------------------
Description:
    Calculates the seismic mis-ties.

Prototype:
    publicdef INT se_mt_calc(INT *lines, DOUBLE *mistie)

Parameters:
    (O) lines       -(INT)  Array of line numbers.
    (O) mistie      -(DOUBLE) Array of mis-tie values corresponding to 'lines'.

Return Value/Status:
    SUCCESS - Valid solution obtained.
    FAIL - System of equations was singular - nothing returned.

Scope:
    PUBLIC
 
Limitations/Assumptions:
    This routine calculates a least squares solution for static
    mis-tie corrections for each line.
-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef INT se_mt_calc (INT *lines, DOUBLE *mistie)
#else
publicdef INT se_mt_calc (lines, mistie)
INT *lines;
DOUBLE *mistie;
#endif
    {
    INT status;                     /* operation return status */
    INT test;                       /* test value */
    INT i;                          /* loop index */
    INT j;                          /* loop index */
    DOUBLE *ia;                     /* input pointer */
    DOUBLE *oa;                     /* output pointer */
    DOUBLE *ib;                     /* input pointer */
    DOUBLE *ob;                     /* output pointer */
    
    /* Init to SUCCESS */
    /* --------------- */
    status = SUCCESS;
    
    /* Pack out unwanted rows and columns from data */
    /* -------------------------------------------- */
    ia = a;
    ib = b;
    oa = a;
    ob = b;
    for (i = 0; i < nlin; i++)
        {
        if (i EQUALS rloc)
            {
            ib += 1;
            ia += nlin;
            continue;
            }
        else
            {
            *ob++ = *ib++;
            }
        for (j = 0; j < nlin; j++)
            {
            if (j EQUALS rloc)
                {
                ia += 1;
                }
            else
                {
                *oa++ = *ia++;
                }
            }
        }
    /* Solve system of equations */
    /* ------------------------- */
    test = se_mt_simq (a, b, nlin - 1);
    
    /* Check if result ok */
    /* ------------------ */
    if (test > 0)
        {
        status = FAIL;
        return status;
        }
    /* Put zero for reference line back into output */
    /* -------------------------------------------- */
    ib = b;
    ob = mistie;
    for (i = 0; i < nlin - 1; i++)
        {
        if (i EQUALS rloc)
            {
            *ob++ = 0.0;
            }
        *ob++ = *ib++;
        }
    if (rloc EQUALS nlin - 1)
        {
        *ob = 0.0;
        }
    /* Return line array */
    /* ----------------- */
    for (i = 0; i < nlin; i++)
        {
        *lines++ = *lin++;
        }
    lin -= nlin;
    
    /* Release allocated space */
    /* ----------------------- */
    tc_free (b);
    tc_free (a);
    tc_free (lin);
    
    /* All done at last */
    /* ---------------- */
    return status;
    }
