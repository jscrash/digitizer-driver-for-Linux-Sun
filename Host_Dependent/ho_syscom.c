/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSCOM.C*/
/* *2    17-AUG-1990 21:53:48 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_SYSCOM.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_HO_ERR_H

#include "esi_ho_err.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Execute a system command from within a program.

Prototype:
    publicdef INT  ho_syscom (CHAR *string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) System command to execute.

Return Value/Status:
    Function returns status of system call.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_syscom (CHAR *string) 
#else
publicdef INT ho_syscom (string)
CHAR *string;
#endif

#ifdef primos
    {
    SHORT com_status;
    CHAR command[1026], dummy[10];
    CHAR *ts_cs_to_vcs ();
    SHORT invok_status, ttyon, ttyoff, zero, status;
    fortran cp$(), como$$();
    
    ts_cs_to_vcs (command, string);
    
    invok_status = 0;
    status = 0;
    zero = 0;
    ttyon = 2;
    ttyoff = 1;
    strcpy (dummy, "");
    
    como$$(ttyoff, dummy, zero, zero, status);  /* turn off TTY output */
    cp$(command, invok_status, com_status);
    como$$(ttyon, dummy, zero, zero, status);   /* turn on TTY output */
    
    if (com_status != 0)
        com_status = HO_ERR_VMS;
    
    return(com_status);
    }


#else

#if UNIX
    {
    INT status;
    
    status = system (string);
    
    return status;
    }


#else

#ifdef vms
    {
    return ho_syscom_nw (string, 0);
    }


#endif      /*  end VMS     */
#endif      /*  end UNIX    */
#endif      /*  end PRIMOS  */

/* END:     */
