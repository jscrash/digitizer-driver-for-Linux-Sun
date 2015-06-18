/* DEC/CMS REPLACEMENT HISTORY, Element AS_YESNO.C*/
/* *3    23-AUG-1990 15:29:07 JULIAN "(SPR 0) fix compiling error"*/
/* *2    14-AUG-1990 11:41:44 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:01:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AS_YESNO.C*/
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

#define YESNO_RSRC "WI_YESNO"

#include "esi_gl_defs.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "wi_yesno.rh"
#include "esi_as_yesno.h"

/* File Description ---------------------------------------------------------
Overview:
        AS_YESNO is an application server which solicits the answer to a
    yes/no question from the user. At the caller's discretion, the user
    may or may not depress a cancel button.

    You start the AS_YESNO server via the am_start_server application
    manager service.

    The protocol between the application and the AS_YESNO server is
    defined in the include file esi_as_yesno.h. The application which
    starts AS_GET_AREA is responsible for providing the protocol buffer.
    The protocol buffer is called AS_YESNO_WORKSPACE.

    Certain options are available to the calling application. These are
    specified by setting the values of items in the AS_YESNO_WORKSPACE
    protocol buffer before starting the AS_YESNO server.

         .cancel - boolean flag indicating whether the user is allowed
                   to CANCEL. Set .cancel to TRUE if the user is allowed
                   to CANCEL. Set .cancel to FALSE if the user is not
                   allowed to cancel.

    When the application's server completion routine is called, the
    following information is returned in the AS_YESNO_WORKSPACE
    protocol buffer.

         .status - Status information indicating the user's answer, or
                   indicating that the user cancelled.

         .answer - indicates whether the user chose YES or NO. A value
                   of 1 indicates YES, 0 indicates NO.

    The application's server completion routine should always check
    .status to determine the validity of the .answer indicator. The
    following status codes may be returned:

         SUCCESS - The operator depressed YES or NO. The .answer
                   field contains the response.

         AS_ERR_CANCEL - The operator depressed the CANCEL button.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT as_yesno(AS_YESNO_WORKSPACE *p);

Private_Functions:
    static INT as_yesno_server(INT id, INT item, AS_YESNO_WORKSPACE *p);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT as_yesno(AS_YESNO_WORKSPACE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) p           -(AS_YESNO_WORKSPACE *) Protocol buffer as defined
                        above.

Return Value/Status:
    See above description.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

#if USE_PROTOTYPES
static INT as_yesno_server (INT id, INT item, AS_YESNO_WORKSPACE *p);
#else
static INT as_yesno_server ();
#endif

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT as_yesno (AS_YESNO_WORKSPACE *p) 
#else
publicdef INT as_yesno (p)
AS_YESNO_WORKSPACE *p;
#endif
    {
    INT id;
    
    /* ********************************************************************** */
    
    am_declare ("AS_YESNO");
    
    am_define_resource (AM_DIALOG, YESNO_RSRC, 2, as_yesno_server, &id, p);
    
    if (p->cancel)
        wiencdi (id, YES_NO_CANCEL_CANCEL, 1);
    else
        wiencdi (id, YES_NO_CANCEL_CANCEL, 0);
    
    wiscdt (id, YES_NO_CANCEL_QUESTION, p->question);
    
    wiopcd (id);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT as_yesno_server(INT id, INT item, AS_YESNO_WORKSPACE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Id number.
    (I) item        -(INT) Item number.
    (M) p           -(AS_YESNO_WORKSPACE *) Protocol buffer as defined above.

Return Value/Status:
    See above description.
    
Scope:
    PRIVATE to <as_yesno>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT as_yesno_server (INT id, INT item, AS_YESNO_WORKSPACE *p) 
#else
static INT as_yesno_server (id, item, p)
INT id;
INT item;
AS_YESNO_WORKSPACE *p;
#endif
    {
    /* ********************************************************************** */
    
    switch (item)
        {
        
    case YES_NO_CANCEL_YES:
        p->status = SUCCESS;
        p->answer = 1;
        break;
        
    case YES_NO_CANCEL_NO:
        p->status = SUCCESS;
        p->answer = 0;
        break;
        
    case YES_NO_CANCEL_CANCEL:
        p->status = AS_ERR_CANCEL;
        break;
        }
    return am_quit ();
    }
/* END:     */
