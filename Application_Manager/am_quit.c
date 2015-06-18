/* DEC/CMS REPLACEMENT HISTORY, Element AM_QUIT.C*/
/* *4    14-AUG-1990 11:40:12 VINCE "(SPR 5644) Header Standardization"*/
/* *3     2-MAR-1990 11:12:37 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *2     6-FEB-1990 00:14:59 JULIAN "(SPR -1) X Version"*/
/* *1    19-JUN-1989 12:00:14 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_QUIT.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_am_int.h"
#include "esi_am_err.h"

#ifdef USE_X 

#include "esi_wi_int.x"

#endif

/* Function Description -----------------------------------------------------
Description:
    AM_QUIT is called by an application immediately before it returns to
    exit.  AM_QUIT calls AM_QUIT_CURRENT with the current application id.

Prototype:
    publicdef INT am_quit ();
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful quit of application. 
    AM_ERR_APPLICATION_QUIT - Am_quit has already been executed for this
        application.
        
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_quit () 
#else
publicdef INT am_quit ()
#endif
    {
    INT status;                     /* service completion status */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit. */
    
    if (application_static->application_id == -1)
        status = AM_ERR_APPLICATION_QUIT;
    
    else                            /*   application id is ok, so call
                                         am_quit_current to quit current
                                         application (or server).  If this is a
                                         server, the parent's completion routine
                                         will be called. */
        {
        am_quit_current (application_static->application_id, TRUE);

#ifdef USE_X
        wiz_del_menu_syn_list (application_static->application_id);
#endif
        }
    return status;
    }
/* END:     */
