/* DEC/CMS REPLACEMENT HISTORY, Element HO_FOR_STDOUT.C*/
/* *3    17-AUG-1990 21:51:33 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:58:46 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:58:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_FOR_STDOUT.C*/
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
    Called by Fortran routines which need to do terminal I/O,
    this routine returns the machine-specific  unit number
    for output to console.

Prototype:
    See below.
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    See below.
    
Return Value/Status:
    Function returns the machine-specific  unit number
    for output to console.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#ifdef primos
publicdef INT ho_get_fortran_stdout ()
    {
    return 1;
    }


#endif

#ifdef vms
publicdef INT ho_get_fortran_stdout ()
    {
    return 6;
    }


#endif

#if UNIX
publicdef INT ho_get_fortran_stdout_ ()
    {
    return 6;
    }


#endif
/* END:     */
