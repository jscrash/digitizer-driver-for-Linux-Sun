/* DEC/CMS REPLACEMENT HISTORY, Element VI_RMS_AVG2.C */
/* *3    16-AUG-1991 16:54:49 JESSIE "(SPR 0) for BHP's modification" */
/* *2    12-JUN-1991 20:20:24 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:16:07 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_RMS_AVG2.C */
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
    publicdef INT vi_rms_avg2 (INT n, DOUBLE *tim, DOUBLE *vrms,
                               DOUBLE *vavg, INT *nout)
    
Parameters:
    (I) n           -(INT)      Length of time,velocity arrays.
    (I) tim         -(DOUBLE *) Array of times.
    (I) vrms        -(DOUBLE *) Array of stacking velocities.
    (O) vavg        -(DOUBLE *) Array of average velocities.
    (O) nout        -(INT *)    Length of 'vavg'.  'nout' <= 'n'.

Return Value/Status:
    SUCCESS.
    FAIL:
        -1 - unexpected inversion of horizon times - possible horizons
             out of order.
        -2 - unexpected velocity inversion.
        -3 - both time and velocity out of order.

Scope:
    PUBLIC

Limitations/Assumptions:
    This routine is based on the Dix equation.
    
-----------------------------------------------------------------------------*/

#include "esi_math.h"

#if USE_PROTOTYPES
publicdef INT vi_rms_avg2 (INT n, DOUBLE *tim, DOUBLE *vrms,
                           DOUBLE *vavg, INT *nout)
#else
publicdef INT vi_rms_avg2 (n, tim, vrms, vavg, nout)
INT n;
DOUBLE *tim;
DOUBLE *vrms;
DOUBLE *vavg;
INT    *nout;
#endif
{
    INT status;                     /* processing status                    */
    INT i;
    DOUBLE sumt;                    /* summation of times                   */
    DOUBLE sumtv;                   /* summation of time-velocity           */
    DOUBLE sumtv2;                  /* summation of time-velocity squared   */
    DOUBLE t;                       /* working time value                   */
    DOUBLE told;                    /* previous value of t                  */
    DOUBLE dt;                      /* t - told                             */
    DOUBLE vold;                    /* previous value of v                  */
    DOUBLE v;                       /* working velocity value               */
    DOUBLE v2;                      /* working velocity squared value       */
    DOUBLE vint;                    /* working interval velocity            */
        
    status = SUCCESS;
    
    /* set 1st output value equal to 1st input value */
    /* --------------------------------------------- */    
    *vavg = *vrms;
    
    sumt = *tim;
    sumtv = *tim * *vrms;
    sumtv2 = sumtv * *vrms;
    t = *tim;
    v = *vrms;
    *nout = 1;
    
    /* Convert RMS velocities to AVG velocities (Dix) */
    /* ---------------------------------------------- */
    for (i = 1; i < n; i++)
        {
        vold = v;
        v = vrms[i];
        v2 = v * v;
        told = t;
        t = tim[i];
        dt = t - told;
        
#if 0
        if(dt < 0.0)
            {
            status = -1;
            }
        if(v < vold)
            {
            status -= 2;
            }
        if(status != SUCCESS)
            {
            return status;
            }
#endif
        /** In the case of invalid time, valocity values, set the current
            time and velocity as the previous time and pre velocity,
            DO NOT return the flag as FAIL because the program will
            stop processing data and we can not find where is an invalid
	    data value in the huge data file ** HH **/
        
         if(dt < 0.0 || v < vold)
            {
            t = told;
            v = vold;
            }         

        vint = sqrt ((v2 * t - sumtv2) / dt);
        sumtv2 = (t * v2);  

        sumt  += t;
        sumtv += (t * vint);
        if( sumt > 0)
          {
          vavg[i] = sumtv / sumt;
          }
        *nout += 1;
        }

    return status;
    
}   /* end of vi_rms_avg2 */
