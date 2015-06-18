/* DEC/CMS REPLACEMENT HISTORY, Element VI_CDP_INTP.C */
/* *2    12-JUN-1991 20:19:00 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:03:33 VOSSLER "new file 0" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_CDP_INTP.C */
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
    Interpolate so that value exists at every CDP.

Prototype:
    publicdef VOID vi_cdp_intp(INT knt, INT ref_cdp, INT *cdp, DOUBLE *value,
                               INT *ocdp, DOUBLE *out)
                                  
Parameters:
    (I) knt         -(INT)      Length of 'cdp', 'value' arrays.
    (I) ref_cdp     -(INT)      Reference cdp value to set zero index.
    (I) cdp         -(INT *)    Array of cdp numbers.
    (I) value       -(DOUBLE *) Array of values associated with 'cdp'.
    (O) ocdp        -(INT *)    Fully filled out cdp array.  It will be
                                of length: cdp[knt-1] - cdp[0] + 1.
                                Interpolated values will be set to '-1'*cdp
                                so that they can be told apart.
    (O) out         -(DOUBLE *) Fully filled out value array.  It will be
                                of length: cdp[knt-1] - cdp[0] + 1.

Return Value/Status:
    None.

Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/
#include "esi_gl_defs.h"

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef VOID vi_cdp_intp(INT knt, INT ref_cdp, INT *cdp, DOUBLE *value,
                           INT *ocdp, DOUBLE *out)
#else
publicdef VOID vi_cdp_intp(knt, ref_cdp, cdp, value, ocdp, out)
INT     knt;
INT     ref_cdp;
INT    *cdp;
DOUBLE *value;
INT    *ocdp;
DOUBLE *out;
#endif
{
    INT last,ic;
    INT i;
    INT j;
    INT idif;
    DOUBLE old;
    DOUBLE delta;
    DOUBLE dl;
    
    /* 1st point always saved */
    /* ---------------------- */
    i = *cdp - ref_cdp;
    ocdp += i;
    out += i;
    *ocdp++ = *cdp;
    *out++ = *value;
    
    last = *cdp++;
    old = *value++;
    
    /* Loop thru data */
    /* -------------- */
    for(i = 1; i < knt; i++)
        {
        idif = *cdp - last;
        ic = last;
        last = *cdp++;
        
        if(idif > 1)
            {
            delta = (*value - old)/(DOUBLE)(idif);
            dl = old;
            for(j = 0; j < idif - 1; j++)
                {
                dl += delta;
                *out++ = dl;
                ic += 1;
                *ocdp++ = -ic;
                }
            }
            
        old = *value++;
        *out++ = old;
        *ocdp++ = last;
        }
    
} /* end of vi_cdp_intp */
