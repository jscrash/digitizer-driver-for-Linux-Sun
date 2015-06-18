/* DEC/CMS REPLACEMENT HISTORY, Element VI_RMS_AVG.C */
/* *2    12-JUN-1991 20:20:19 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:15:33 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_RMS_AVG.C */
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
    Function to convert from stacking velocity to average velocity.

Prototype:
    publicdef VOID vi_rms_avg (INT n, DOUBLE *tim, DOUBLE *vrms,
                               DOUBLE *vavg)
    
Parameters:
    (I) n           -(INT)      Length of time,velocity arrays.
    (I) tim         -(DOUBLE *) Array of times.
    (I) vrms        -(DOUBLE *) Array of stacking velocities.
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
publicdef VOID vi_rms_avg (INT n, DOUBLE *tim, DOUBLE *vrms, DOUBLE *vavg)
#else
publicdef VOID vi_rms_avg (n, tim, vrms, vavg)
INT n;
DOUBLE *tim;
DOUBLE *vrms;
DOUBLE *vavg;
#endif
{
    INT i;
    DOUBLE sumt;                    /* summation of times                   */
    DOUBLE sumtv;                   /* summation of time-velocity           */
    DOUBLE sumtv2;                  /* summation of time-velocity squared   */
    DOUBLE t;                       /* working time value                   */
    DOUBLE v;                       /* working velocity value               */
    DOUBLE v2;                      /* working velocity squared value       */
    DOUBLE vint;                    /* working interval velocity            */
        

    /* set 1st output value equal to 1st input value */
    /* --------------------------------------------- */    
    *vavg = *vrms;
    
    sumt = *tim;
    sumtv = *tim * *vrms;
    sumtv2 = sumtv * *vrms;
    
    /* Convert RMS velocities to AVG velocities (Dix) */
    /* ---------------------------------------------- */
    for (i = 1; i < n; i++)
        {
        v = vrms[i];
        v2 = v * v;
        t = tim[i];

        vint = sqrt ((v2 * t - sumtv2) / (tim[i]-tim[i-1]));
        sumtv2 = (t * v2);  

        sumt  += t;
        sumtv += (t * vint);
        vavg[i] = sumtv / sumt;
        }

    return;
    
}   /* end of vi_rms_avg */
