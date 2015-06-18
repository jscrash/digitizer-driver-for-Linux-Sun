/* DEC/CMS REPLACEMENT HISTORY, Element AS_SAVE.C*/
/* *2    14-AUG-1990 11:41:38 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:57 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AS_SAVE.C*/
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

#define SAVE_RSRC "AS_SAVE"

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef AS_SAVE_RH

#include "as_save.rh"

#endif

#ifndef ESI_AS_SAVE_H

#include "esi_as_save.h"

#endif

#ifndef ESI_AS_MSG_H

#include "esi_as_msg.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#include "esi_as_err.h"

/* File Description ---------------------------------------------------------
Overview:
    AS_SAVE is an application server which solicits a filename from the user.
    At the caller's discretion, the user may or may not depress a cancel button.

    You start the AS_SAVE server via the am_start_server application
    manager service.

    The protocol between the application and the AS_SAVE server is
    defined in the include file esi_as_save.h. The application which
    starts AS_SAVE is responsible for providing the protocol buffer.
    The protocol buffer is called AS_SAVE_WORKSPACE.

    Certain options are available to the calling application. These are
    specified by setting the values of items in the AS_SAVE_WORKSPACE
    protocol buffer before starting the AS_SAVE server.

         .cancel - boolean flag indicating whether the user is allowed
                   to CANCEL. Set .cancel to TRUE if the user is allowed
                   allowed to cancel.

    When the application's server completion routine is called, the
    following information is returned in the AS_SAVE_WORKSPACE
    protocol buffer.

         .status - Status information indicating the user's answer, or
                   indicating that the user cancelled.

         .filename - The name of the file 

     The following status codes may be returned:

         SUCCESS - The operator depressed the OK button

         AS_ERR_CANCEL - The operator depressed the CANCEL button.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT as_save(AS_SAVE_WORKSPACE *p);

Private_Functions:
    static INT as_save_server(INT id,INT item, AS_SAVE_WORKSPACE *p);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT as_save_server(INT id,INT item, AS_SAVE_WORKSPACE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Id number.
    (I) item        -(INT) Item number.
    (M) p           -(AS_SAVE_WORKSPACE *) Protocol buffer as defined above.

Return Value/Status:
    See above description.
    
Scope:
    PRIVATE to <as_save>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT as_save_server (INT id, INT item, AS_SAVE_WORKSPACE *p) 
#else
static INT as_save_server (id, item, p)
INT id;
INT item;
AS_SAVE_WORKSPACE *p;
#endif
    {
    /* ********************************************************************** */
    
    PROGNAME ("as_save_server");
    INT value;
    
    switch (item)
        {
        
    case AS_SAVE_BTN:
        p->status = SUCCESS;
        wiqccdi (id, AS_FILENAME, &value, p->filename);
        if (strlen (p->filename) == 0)
            {
            wiscdt (id, AS_SAVE_DIAG, mg_message (AS_INPUT_FILE));
            return SUCCESS;
            }
        break;
        
    case AS_CANCEL_BTN:
        p->status = AS_ERR_CANCEL;
        break;
        }
    return am_quit ();
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT as_save(AS_SAVE_WORKSPACE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) p           -(AS_SAVE_WORKSPACE) Protocol buffer as defined above.

Return Value/Status:
    See above description.
    
Scope:
    PUBLIC

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT as_save (AS_SAVE_WORKSPACE *p) 
#else
publicdef INT as_save (p)
AS_SAVE_WORKSPACE *p;
#endif
    {
    PROGNAME ("as_save");
    INT id;
    
    /* ********************************************************************** */
    
    am_declare ("AS_SAVE");
    
    am_define_resource (AM_DIALOG, SAVE_RSRC, 1, as_save_server, &id, p);
    
    if (p->cancel)
        wiencdi (id, AS_CANCEL_BTN, ON);
    else
        wiencdi (id, AS_CANCEL_BTN, OFF);
    
    wiscdt (id, AS_SAVE_MESSAGE, p->message);
    wiscdt (id, AS_FILENAME, p->filename);
    
    wi_open_dialog (id);
    
    wi_activate (id, AS_FILENAME, ON);
    
    return SUCCESS;
    }
/* END:     */
