/* DEC/CMS REPLACEMENT HISTORY, Element HO_CTIME.C*/
/* *2    17-AUG-1990 21:50:40 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:57:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_CTIME.C*/
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

#include "esi_c_lib.h"

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#include "esi_time.h"

/* Function Description -----------------------------------------------------
Description:
    HO_CTIME places a 26-byte C string containing the current date and
    time, in ASCII, into a string buffer specified by the parameter.

Prototype:
    publicdef INT ho_ctime (char *time_string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) time_string -(CHAR *) Location to place string.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_ctime (CHAR *time_string) 
#else
publicdef INT ho_ctime (time_string)
CHAR *time_string;
#endif
    {                               /* start of HO_CTIME */
#if UNIX
    time_t bintim;                  /* system binary time */
    
    time (&bintim);                 /* Get system date/time (binary) */
    strcpy (time_string, ctime (&bintim));  /* binary date/time to ASCII */
#else

#ifdef vms
    time_t bintim;                  /* system binary time */
    
    time (&bintim);                 /* Get system date/time (binary) */
    strcpy (time_string, ctime (&bintim));  /* binary date/time to ASCII */
#endif      /* VMS */

#ifdef primos
    int bintim;                     /* system binary time */
    
    time (&bintim);                 /* Get system date/time (binary) */
    strcpy (time_string, ctime (&bintim));  /* binary date/time to ASCII */
    
    time_string[strlen (time_string) - 1] = '\0';   /* remove End-of-Line */
#endif      /* PRIMOS */
#endif      /* UNIX */
    return SUCCESS;
    }                               /* end of HO_CTIME */
/* END:     */
