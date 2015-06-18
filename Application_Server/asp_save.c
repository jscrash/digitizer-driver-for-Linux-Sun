/* DEC/CMS REPLACEMENT HISTORY, Element ASP_SAVE.C*/
/* *3    14-AUG-1990 11:41:32 VINCE "(SPR 5644) Header Standardization"*/
/* *2    29-DEC-1989 10:56:46 SCD "(SPR 100) Ensure filename returned is UPPER and cleaned of non-printing chars."*/
/* *1    19-JUN-1989 12:00:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element ASP_SAVE.C*/
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

#include "esi_gl_defs.h"
#include "esi_wi.h"
#include "esi_am.h"
#include "esi_ts.h"
#include "esi_fi.h"
#include "as_save.rh"
#include "esi_as_save.h"
#include "esi_as_err.h"

/* Function Description -----------------------------------------------------
Description:
    ASP_SAVE solicits a filename from the user.
    At the caller's discretion, the user may or may not depress a cancel 
    button.

    The protocol between the application and ASP_SAVE is
    defined in the include file esi_as_save.h. The application which
    calls ASP_SAVE is responsible for providing the protocol buffer.
    The protocol buffer is called AS_SAVE_WORKSPACE.

    Certain options are available to the calling application. These are
    specified by setting the values of items in the AS_SAVE_WORKSPACE
    protocol buffer before calling AS_SAVE.

         .cancel - boolean flag indicating whether the user is allowed
                   to CANCEL. Set .cancel to TRUE if the user is allowed
                   allowed to cancel.

    
Prototype:
    publicdef INT asp_save(AS_SAVE_WORKSPACE *p);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) p           -(AS_SAVE-WORKSPACE *) Protocol buffer to pass parameters as
                        defined above and receive status information as defined
                        below.
                        
Return Value/Status:
    SUCCESS - The operator depressed the OK button.
    FAIL - Unknown event.
    AS_ERR_CANCEL - The operator depressed the CANCEL button.
    The following information is returned in the protocol buffer:
    .status - Status information indicating the user's answer, or
        indicating that the user cancelled.
    .filename - The name of the file 
         
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT asp_save (AS_SAVE_WORKSPACE *p) 
#else
publicdef INT asp_save (p)
AS_SAVE_WORKSPACE *p;
#endif
    {
    INT id;
    INT event_class, event_id, event_item;
    INT value;
    INT status;
    FILENAME filename;
    
    /* ********************************************************************** */
    
    fi_push ();
    
    am_define_resource (AM_DIALOG, SAVE_RSRC, 1, NULL_FUNCTION_PTR, &id, p);
    
    if (p->cancel)
        wiencdi (id, AS_CANCEL_BTN, 1);
    else
        wiencdi (id, AS_CANCEL_BTN, 0);
    
    wiscdt (id, AS_SAVE_MESSAGE, p->message);
    wiscdt (id, AS_FILENAME, p->filename);
    
    wi_open_dialog (id);
    
    wi_activate (id, AS_FILENAME, ON);
    
    for (status = FAIL; status != SUCCESS; )
        {
        wi_request_event (0, &event_class, &event_id, &event_item);
        if (event_id == id)
            {
            status = SUCCESS;       /* Came from correct dialog. */
            switch (event_item)
                {
            case AS_SAVE_BTN:
                p->status = SUCCESS;
                wiqccdi (id, AS_FILENAME, &value, filename);
                if (strlen (filename) == 0)
                    {
                    wibeep (1);
                    status = FAIL;  /* No name entered, try again */
                    }
                else
                    {
                    strcpy (p->filename, ts_snowhite (ts_sto_upper (filename)));
                    }
                break;
                
            case AS_CANCEL_BTN:
                p->status = AS_ERR_CANCEL;
                break;
                
            default:
                status = FAIL;      /* Unknown event, try again.    */
                break;
                }
            }
        else
            {
            wibeep (1);
            }
        }
    am_release_resource (id);
    
    fi_pop ();
    
    return SUCCESS;
    }
/* END:     */
