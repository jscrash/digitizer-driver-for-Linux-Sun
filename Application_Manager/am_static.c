/* DEC/CMS REPLACEMENT HISTORY, Element AM_STATIC.C*/
/* *2    14-AUG-1990 11:41:20 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_STATIC.C*/
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

#include "esi_am_int.h"

/* Function Description -----------------------------------------------------
Description:
    AM_STATIC returns a pointer to the application manager's
    application_static data structure.

Prototype:
    publicdef APPLICATION_STATIC *am_static();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    The function returns a pointer to the application manager's
    application_static data structure.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    This routine is for internal use only, and is not normally to be
    used by application programs. Generally, modifying the
    application_static structure could seriously jeopardize the
    operation of the application manager, and therefore the product
    system.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef APPLICATION_STATIC *am_static () 
#else
publicdef APPLICATION_STATIC *am_static ()
#endif
    {
    static APPLICATION_STATIC application_static;
    
    return &application_static;
    }
/* END:     */
