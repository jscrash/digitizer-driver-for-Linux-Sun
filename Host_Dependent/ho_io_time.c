/* DEC/CMS REPLACEMENT HISTORY, Element HO_IO_TIME.C*/
/* *4    30-OCT-1990 12:45:01 GILLESPIE "(SPR 37) Fix up include file stuff for sun"*/
/* *3    17-AUG-1990 21:52:23 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:55 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:58:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_IO_TIME.C*/
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

#include "esi_ho.h"

#ifdef sun
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifdef vms
#include "jpidef.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    HO_IO_TIME returns the amount of DISK time used during the login session.

Prototype:
    publicdef DOUBLE ho_io_time();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns the amout of disk time used during login session.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef DOUBLE ho_io_time (void) 
#else
publicdef DOUBLE ho_io_time ()
#endif
    {
    
    DOUBLE time_value = 0.0;

#ifdef primos
    fortran DOUBLE CTIM$A ();
    INT junk;

    time_value = CTIM$A (&junk);
#endif                                                                          

#ifdef vms
    INT hoz_getjpi ();
    INT len;
    INT buf;

    hoz_getjpi (JPI$_DIRIO, &buf, sizeof buf, &len);
    if (len IS_NOT_EQUAL_TO 4)
        return (DOUBLE)FAIL;
    else
        time_value = buf;
#endif                                                                          

#ifdef sun
    struct rusage rstruct;
    getrusage (RUSAGE_SELF, &rstruct);
    time_value = (DOUBLE)rstruct.ru_stime.tv_usec;
#endif
    return time_value;
    }
/* END:     */
