/* DEC/CMS REPLACEMENT HISTORY, Element VI_LIN_INTP.C */
/* *2    12-JUN-1991 20:19:48 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:12:33 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_LIN_INTP.C */
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
    Function to linearly interpolate velocity.

Prototype:
    publicdef DOUBLE vi_lin_intp (INT n, DOUBLE *tim, DOUBLE *vel, DOUBLE tout)

Parameters:
    (I) n           -(INT)      Length of time,velocity arrays.
    (I) tim         -(DOUBLE *) Array of times.
    (I) vel         -(DOUBLE *) Array of velocities.
    (I) tout        -(DOUBLE)   Time at which vel to be calculated.

Return Value/Status:
    Interpolated velocity value.

Scope:
    PUBLIC

Limitations/Assumptions:
    For tout < tim[0];   vout = vavg[0]
        tout > tim[n-1]; vout = vavg[n-1]      

-----------------------------------------------------------------------------*/

#include "esi_math.h"

#if USE_PROTOTYPES
publicdef DOUBLE vi_lin_intp (INT n, DOUBLE *tim, DOUBLE *vel, DOUBLE tout)
#else
publicdef DOUBLE vi_lin_intp (n, tim, vel, tout)
INT n;
DOUBLE *tim;
DOUBLE *vel;
DOUBLE tout;
#endif
{
    INT i;
    DOUBLE t;                       /* working time value           */
    DOUBLE v;                       /* working velocity value       */
    DOUBLE vout;                    /* interpolated velocity value  */
    
    /* If outside range of input, return endpoint of input array */
    /* --------------------------------------------------------- */
    if (tout < *tim)
        {
        vout = *vel;
        return vout;
        }
        
    if (tout > tim[n - 1])
        {
        vout = vel[n - 1];
        return vout;
        }

    for (i = 1; i < n; i++)
        {
        if (tout > tim[i])
            {
            continue;
            }
            
        v = vel[i] - vel[i - 1];
        t = tim[i] - tim[i - 1];
        vout = vel[i - 1] + v * (tout - tim[i - 1]) / t;
        
        return vout;
        }
    
}   /* end of vi_lin_interp */
