/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_UNIQ.C*/
/* *3    17-AUG-1990 21:52:18 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    13-OCT-1989 07:54:06 GORDON "(SPR 4999) remove version from VMS filename (This rtn doomed to extinction because of*/
/*tmpnam())"*/
/* *1    19-JUN-1989 12:58:56 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_GET_UNIQ.C*/
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
#include "esi_ho_files.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Creates a generic guaranteed-to-be-unique file name
    with no extension for general use by plotting routines
    which need temporary files and don't want to be bothered
    creating a unique name. The caller can append an informative
    extension if desired.

Prototype:
    publicdef INT ho_get_unique_filename (FILENAME filename);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) filename    -(FILENAME) Unique file name.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_get_unique_filename (FILENAME filename) 
#else
publicdef INT ho_get_unique_filename (filename)
FILENAME filename;
#endif
    {                               /* start of HO_GET_UNIQUE_FILENAME */
    CHAR time_string[32];
    
    ho_ctime (time_string);         /* get the host's time string */

#if UNIX
    
    /* Note: HO_CTIME on UNIX returns format "Tue Jan 23 07:34:26 1989".
             We want to extract the time with no colons. */
    
    sprintf (filename, "T_%.2s%.2s%.2s", &time_string[11], &time_string[14],
             &time_string[17]);
    return SUCCESS;

#else       /* not UNIX */

#ifdef vms
    
    /* Note: HO_CTIME on VMS returns format "Tue Jan 23 07:34:26 1989".
             We want to extract the time with no colons. */
    
    sprintf (filename, "T_%.2s%.2s%.2s", &time_string[11], &time_string[14],
             &time_string[17]);
    return SUCCESS;

#endif      /* VMS */

#ifdef primos
    
    /* Note: HO_CTIME on PRIMOS returns format "03 Jan 89 07:34:26 Tuesday".
             We want to extract the time with no colons. */
    
    sprintf (filename, "T_%.2s%.2s%.2s", &time_string[10], &time_string[13],
             &time_string[16]);
    return SUCCESS;

#endif      /* PRIMOS */
#endif      /* UNIX */
    }                               /* end of HO_GET_UNIQUE_FILENAME */
/* END:     */
