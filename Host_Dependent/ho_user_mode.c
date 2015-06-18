/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_MODE.C*/
/* *3    17-AUG-1990 21:54:07 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    26-FEB-1990 18:59:23 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *1    19-JUN-1989 12:59:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_MODE.C*/
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
#include "esi_ho.h"

#ifdef vms

#include "jpidef.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    HO_USER_MODE returns HO_INTERACTIVE or HO_BATCH, defined in ESI_HO.H,
    depending on the mode of the current process.

Prototype:
    publicdef INT ho_user_mode();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns either HO_INTERACTIVE or HO_BATCH.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_user_mode () 
#else
publicdef INT ho_user_mode ()
#endif
    {
    INT mode = HO_INTERACTIVE;      /* assume interactive at first */

#ifdef primos
    INT isatty ();
    
    if (isatty (stdin) == 1)
        mode = HO_INTERACTIVE;
    else
        mode = HO_BATCH;
#endif

#if UNIX
    CHAR *getlogin ();
    CHAR *login;
    
    if (login = getlogin ())
        mode = HO_INTERACTIVE;
    else
        mode = HO_BATCH;
#endif

#ifdef vms
    CHAR cmode[128];
    INT cmode_length;
    
    hoz_getjpi (JPI$_MODE, cmode, sizeof cmode, &cmode_length);
    if (strcmp (cmode, "BATCH") == 0)
        mode = HO_BATCH;
    else
        mode = HO_INTERACTIVE;
#endif
    return mode;
    }
/* END:     */
