/* DEC/CMS REPLACEMENT HISTORY, Element VI_RLOD_D.C */
/* *2    12-JUN-1991 20:20:13 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:14:57 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_RLOD_D.C */
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
    Function to move word (sizeof(DOUBLE)) samples from one array to another.

Prototype:
    publicdef VOID vi_rlod_d (DOUBLE *a, INT i, DOUBLE *c, INT n, INT k, INT iflg)
    
Parameters:
    (I) a           -(DOUBLE *) Input aray.
    (I) i           -(INT)      Increment between samples in 'a'.
    (O) c           -(DOUBLE *) Output aray.
    (I) n           -(INT)      Number of samples to move into 'c'.
    (I) k           -(INT)      Increment between samples in 'c'.
    (I) iflg        -(INT)      Number of words per sample to move
                                (1 for length of 1 word).

Return Value/Status:
    None.

Scope:
    PUBLIC

Limitations/Assumptions:
    
-----------------------------------------------------------------------------*/

#include "esi_vi.h"

#if USE_PROTOTYPES
publicdef VOID vi_rlod_d (DOUBLE *a, INT i, DOUBLE *c, INT n, INT k, INT iflg)
#else
publicdef VOID vi_rlod_d (a, i, c, n, k, iflg)
DOUBLE *a, *c;
INT i, n, k, iflg;
#endif
{
    register DOUBLE *na, *nc;
    register DOUBLE *ndxa, *ndxc;
    register INT j, m, kk, ii;
    INT isw;
    
    isw = iflg;
    isw--;
    kk = k;
    ii = i;
    
    if (!isw)                   /* if each sample only single word long */
        {
        nc = c;
        na = a;
        for (j = 0; j < n; j++)
            {
            *nc = *na;
            nc += kk;
            na += ii;
            }
        }
    else                        /* each sample multiple words long */
        {
        ndxa = a;
        ndxc = c;
        for (j = 0; j < n; j++)
            {
            na = ndxa;
            nc = ndxc;
            for (m = 0; m < iflg; m++)
                {
                *nc++ = *na++;
                }
            ndxa += ii;
            ndxc += kk;
            }
        }
    return;
    
}   /* end rlod */
