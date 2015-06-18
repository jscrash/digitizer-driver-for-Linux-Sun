/* DEC/CMS REPLACEMENT HISTORY, Element HO_TERMINAL.C*/
/* *4    17-AUG-1990 21:54:02 VINCE "(SPR 5644) Code Cleanup"*/
/* *3    26-FEB-1990 18:59:13 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *2    18-JUL-1989 15:36:56 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:59:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_TERMINAL.C*/
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

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif
static CHAR tt_name[50] = 
    {
    '\0'
    };

/* Internal terminal name */

/* Function Description -----------------------------------------------------
Description:
    HO_TERMINAL returns a string containing the name of the terminal
    on which the user is running.

Prototype:
    publicdef CHAR *ho_terminal(CHAR *name);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) name        -(CHAR *) Terminal name.

Return Value/Status:
    Function returns pointer to internal buffer if no name pointer was
        provided.
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ho_terminal (CHAR *name) 
#else
publicdef CHAR *ho_terminal (name)
CHAR *name;
#endif
    {
    CHAR *tt_pointer;               /* Pointer to terminal name */
    CHAR **tcp;
    
    /************************************************************************/
    
    /* Initialize terminal name. */

#ifdef vms
    
    /* Translate VMS symbol TT, which is
        the current user terminal. */
    
    ho_translate_symbol ("TT", &tcp);   /* returns a list */
    strcpy (tt_name, *tcp);         /* use the first item in the list */
    ts_tcp_free (tcp);              /* free the list */

#endif

#ifdef primos
    fortran user$();
    SHORT user_id;
    SHORT num_users;
    
    tt_name[0] = 0;                 /* Initialize terminal name. */
    
    user$(user_id, num_users);
    
    sprintf (tt_name, "%d", user_id);
    

#endif

#if UNIX
    gethostname (tt_name, sizeof tt_name);
#endif
    
    /*  if the caller gave us a pointer move the name there,
        otherwise return the pointer to our internal buffer */
    
    if (name == NULL)
        tt_pointer = tt_name;
    else
        {
        strcpy (name, tt_name);
        tt_pointer = name;
        }
    return tt_pointer;
    }
/* END:     */
