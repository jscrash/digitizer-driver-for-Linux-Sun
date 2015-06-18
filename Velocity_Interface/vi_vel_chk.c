/* DEC/CMS REPLACEMENT HISTORY, Element VI_VEL_CHK.C */
/* *2    12-JUN-1991 20:20:30 VOSSLER "(SPR 0) new file" */
/* *1    12-JUN-1991 20:16:45 VOSSLER "new file" */
/* DEC/CMS REPLACEMENT HISTORY, Element VI_VEL_CHK.C */
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
    publicdef INT vi_vel_chk(INT n, DOUBLE *tim, DOUBLE *vel)
    
Parameters:
    (I) n           -(INT)      Length of time,velocity arrays.
    (I) tim         -(DOUBLE *) Array of times.
    (I) vel         -(DOUBLE *) Array of stacking velocities.

Return Value/Status:
    SUCCESS.
    FAIL:
        -1 = velocity function times out of order.
        -2 = impossible velocity value - most likely a multiple or
             other bad velocity pick.

Scope:
    PUBLIC

Limitations/Assumptions:
    
-----------------------------------------------------------------------------*/

#include "esi_math.h"

#define TVSIZ  30

#if USE_PROTOTYPES
publicdef INT vi_vel_chk(INT n, DOUBLE *tim, DOUBLE *vel)
#else
publicdef INT vi_vel_chk(n, tim, vel)
INT     n;
DOUBLE *tim;
DOUBLE *vel;
#endif
{
    DOUBLE dtx[TVSIZ];
    static DOUBLE x2 = 2.5e+6;
    DOUBLE t1, t2;
    INT status;
    INT i;
    
    status = SUCCESS;
    
    /* Check that times always increase */
    /* -------------------------------- */
    t2 = *tim;
    for(i = 1; i < n; i++)
        {
        t1 = t2;
        t2 = tim[i];
        if(t2 <= t1)
            {
            status = -1;
            return status;
            }
        }
        
    /* Build dtx array */
    /* --------------- */
    for(i = 0; i < n; i++)
        {
        t1 = tim[i];
        t2 = t1*t1;
        dtx[i] = sqrt(t2 + x2/(vel[i]*vel[i])) - t1;
        }
    
    /* Validate velocity picks */
    /* ----------------------- */
    t2 = dtx[0];
    for(i = 1; i < n; i++)
        {
        t1 = t2;
        t2 = dtx[i];
        if(t2 > t1)
            {
            status = -2;
            return status;
            }
        }
        
    return status;
} /* end of vi_vel_chk */
