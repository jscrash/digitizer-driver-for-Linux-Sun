/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_NUM.C*/
/* *3    17-AUG-1990 21:54:15 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    17-MAR-1990 02:18:31 JULIAN "(SPR 1) Fixed type of uid on VMS side to be unsigned."*/
/* *1    19-JUN-1989 12:59:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_NUM.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Return the unique user number as the value of the function.                  

Prototype:
    publicdef INT  ho_user_number();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns user number.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_user_number () 
#else
publicdef INT ho_user_number ()
#endif
    {
#ifdef primos
    short user_no, dummy;
    fortran user$();
    
    user$(user_no, dummy);
    return(user_no);
#endif

#ifdef vms
    unsigned int getuid ();
    unsigned int uid;
    
    uid = getuid ();
    return(uid);
#endif

#if UNIX 
    INT uid;
    
    uid = getuid ();
    return(uid);
#endif
    }
/* END:     */
