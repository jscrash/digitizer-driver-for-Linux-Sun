/* DEC/CMS REPLACEMENT HISTORY, Element VI_INT_AVG.C */
/* *2    12-JUN-1991 20:19:30 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:10:20 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_INT_AVG.C */
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
    Function to convert from interval velocity to average velocity.

Prototype:
    publicdef VOID vi_int_avg (INT n, DOUBLE *tim, DOUBLE *vint,
                               DOUBLE *vavg)
    
Parameters:
    (I) n           -(INT)      Length of time,velocity arrays.
    (I) tim         -(DOUBLE *) Array of times.
    (I) vint        -(DOUBLE *) Array of interval velocities.
    (O) vavg        -(DOUBLE *) Array of average velocities.

Return Value/Status:
    None.

Scope:
    PUBLIC

Limitations/Assumptions:
    This routine is based on the Dix equation.
    
-----------------------------------------------------------------------------*/

#include "esi_math.h"

#if USE_PROTOTYPES
publicdef VOID vi_int_avg (INT n, DOUBLE *tim, DOUBLE *vint, DOUBLE *vavg)
#else
publicdef VOID vi_int_avg (n, tim, vint, vavg)
INT     n;
DOUBLE *tim;
DOUBLE *vint;
DOUBLE *vavg;
#endif
{
    INT i;
    DOUBLE sumt;    /* summation of times */
    DOUBLE sumtv;   /* summation of time-velocity */
    DOUBLE t;       /* working time value */
    DOUBLE v;       /* working velocity value */
    
    /* set 1st output value equal to 1st input value */
    /* --------------------------------------------- */
    *vavg = *vint;
    
    sumt = *tim;
    sumtv = *tim * *vint;
    
    for (i = 1; i < n; i++)
        {
        v = vint[i];
        t = tim[i];
        sumt += t;
        sumtv += (t * v);
        vavg[i] = sumtv / sumt;
        }
    return;
    
}   /* end of vi_int_avg */
