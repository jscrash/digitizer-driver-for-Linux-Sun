/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_NAME.C*/
/* *2    17-AUG-1990 21:54:11 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 12:59:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_USER_NAME.C*/
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
char *cuserid ();

/* Function Description -----------------------------------------------------
Description:
    This function returns a pointer to a character string containing the        
    user ID of the current process.                                             

    If the argument is null, then the user name is stored internally. If        
    the argument is not null, then iit must point to a storage area of          
    length L_cuserid (defined in STDIO.H), and the name is written into         
    that storage area.                                                          

Prototype:
    publicdef CHAR *ho_user_name(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (O) string      -(CHAR *) User name.                                                                   

Return Value/Status:
    Function returns a pointer to the user id.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ho_user_name (CHAR *string) 
#else
publicdef CHAR *ho_user_name (string)
CHAR *string;
#endif
    {
    /* ********************************************************************** */
    return(cuserid (string));
    }
/* END:     */
