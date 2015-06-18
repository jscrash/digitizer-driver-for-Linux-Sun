/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_SVR.C*/
/* *2    14-AUG-1990 11:41:14 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:42 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_SVR.C*/
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
    AM_START_SERVER starts an application server.

Prototype:
    publicdef INT am_start_server(INT (*entry_point)(),
                              INT (*return_point)(), VOIDPTR pointer);
                              
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) entry_point -(INT (*)()) server address.
    (I) return_point-(INT (*)()) caller's server compltion routine.
    (I) pointer     -(VOIDPTR) protocol pointer.

Return Value/Status:
    AM_ERR_SERVER_ACTIVE - If caller already has a server active.
    AM_ERR_APPLICATION_QUIT - Application has "quit" prior to call.
    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary.
    Status code of called entry_point.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_start_server (INT (*entry_point)(), INT (*return_point)(),
                               VOIDPTR pointer) 
#else
publicdef INT am_start_server (entry_point, return_point, pointer)
INT (*entry_point)();
INT (*return_point)();
VOIDPTR pointer;
#endif
    {
    INT status;                     /* service completion status */
    
    /* ********************************************************************** */
    
    /*   Start application server. */
    
    status = am_start_instance (AM_SERVER, entry_point, return_point, pointer);
    
    return status;
    }
/* END:     */
