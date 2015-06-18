/* DEC/CMS REPLACEMENT HISTORY, Element AM_ABORT.C*/
/* *3    14-AUG-1990 11:39:31 VINCE "(SPR 5644) Header Standardization"*/
/* *2    22-SEP-1989 15:29:42 GILLESPIE "(SPR 101) Remove RCSID"*/
/* *1    19-JUN-1989 11:59:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_ABORT.C*/
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
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_ABORT is called by an application (or server) to quit all servers and
    the application associated with the current application (or server).

Prototype:
    publicdef INT am_abort();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successfull completion of closing servers;
    AM_ERR_APPLICATION_QUIT - If servers are already closed.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_abort () 
#else
publicdef INT am_abort ()
#endif
    {
    INT status;                     /* service completion status */
    INT app_id;                     /* application id */
    APPLICATION_LIST app;           /* application node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    /*   Make sure application or server didn't
         quit already. */
    
    if (application_static->application_id == -1)
        status = AM_ERR_APPLICATION_QUIT;
    
    /*   Get top of application id */
    else
        {
        app_id = application_static->application_id;
        btfind (&application_static->application_list, (VOIDPTR) & app_id,
                (VOIDPTR) & app);
        
        /*   Find the parent application */
        
        while (app.type != AM_APPLICATION)
            {
            app_id = app.parent_id;
            btfind (&application_static->application_list, (VOIDPTR) & app_id,
                    (VOIDPTR) & app);
            }
        /*   Now quit chain begining with parent
             application */
        
        am_quit_current (app_id, FALSE);
        }
    return status;
    }
/* END:     */
