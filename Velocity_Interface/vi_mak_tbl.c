/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAK_TBL.C */
/* *3    16-AUG-1991 16:55:36 JESSIE "(SPR 0) for BHP's modification" */
/* *2    12-JUN-1991 20:20:00 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:13:42 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_MAK_TBL.C */
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
    Starting at a specified index, assemble cdp, avg velocity arrays for
    next nlist table.

Prototype:
    publicdef INT vi_mak_tbl(INT ndx, INT len, INT *cdp, DOUBLE *value,
                             INT *ocdp, DOUBLE *out, INT *knt)
                                  
Parameters:
    (I) ndx         -(INT)      Index of start in 'cdp', 'value' arrays.
    (I) len         -(INT)      Length of 'cdp', 'value' arrays.
    (I) cdp         -(INT *)    Array of cdp numbers.
    (I) value       -(DOUBLE *) Array of values associated with 'cdp'.
    (O) ocdp        -(INT *)    Output cdp array.
    (O) out         -(DOUBLE *) Output value array.
    (O) knt         -(INT *)    Length of output arrays (knt <= len).

Return Value/Status:
    Index value following final index used for this access.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#include "esi_gl_defs.h"

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT vi_mak_tbl(INT ndx, INT len, INT *cdp, DOUBLE *value,
                         INT *ocdp, DOUBLE *out, INT *knt)
#else
publicdef INT vi_mak_tbl(ndx, len, cdp, value, ocdp, out, knt)
INT     ndx;
INT     len;
INT    *cdp;
DOUBLE *value;
INT    *ocdp;
DOUBLE *out;
INT    *knt;
#endif
{
    INT i;
    INT j;
    INT next;
    
    *knt = 0;
    next = len;
    
    /* Find first viable value */
    /* ----------------------- */
    for(i = ndx; i < len; i++)
        {
        if(cdp[i] > 0 AND value[i] > 0.0)
            {
            ocdp[*knt] = cdp[i];
            out[*knt] = value[i];
            *knt += 1;
            ndx = i + 1;
            break;
            }
        }
    
    /* Check if done */
    /* ------------- */
    if(*knt EQUALS 0)
        {
        return next;
        }
    
    /* Gather remainder of values */
    /* -------------------------- */
    for(i = ndx; i < len; i++)
        {
        if(cdp[i] EQUALS 0 || value[i] EQUALS 0.0)
            {
            return i;
            }
        else if(cdp[i] < 0)
            {
            continue;
            }

        ocdp[*knt] = cdp[i];
        out[*knt] = value[i];
        *knt += 1;
        }
    return next;
    
} /* end of vi_mak_tbl */
