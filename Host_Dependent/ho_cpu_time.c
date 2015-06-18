/* DEC/CMS REPLACEMENT HISTORY, Element HO_CPU_TIME.C*/
/* *2    17-AUG-1990 21:50:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:56:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_CPU_TIME.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_gl_defs.h"

/* Function Description -----------------------------------------------------
Description:
    HO_CPU_TIME returns the amount of CPU time used during the login session.

Prototype:
    publicdef DOUBLE ho_cpu_time();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the amount of CPU time used during the login session.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE ho_cpu_time () 
#else
publicdef DOUBLE ho_cpu_time ()
#endif
    {
    DOUBLE time_value;

#if UNIX

#include "esi_time.h"
    time_value = (DOUBLE)(clock () / CLK_TCK);  /* CLK_TCK is in TIME.H */
#else

#ifdef vms
    struct                          /* Accumulated CPU times in 10ms units */
        {
        INT proc_user_time;
        INT proc_system_time;
        INT child_user_time;
        INT child_system_time;
        } time_buffer;
    
    times (&time_buffer);
    time_value = ((double)(time_buffer.proc_user_time +
                           time_buffer.proc_system_time)) / 100.0;
#endif

#ifdef primos
    fortran DOUBLE CTIM$A ();
    INT junk;
    
    time_value = CTIM$A (&junk);
#endif
#endif      /* UNIX */
    return time_value;
    }
/* END:     */
