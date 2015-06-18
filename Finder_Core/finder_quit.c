/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_QUIT.C*/
/* *11   31-OCT-1990 09:33:10 GILLESPIE "(SPR 37) Sun compile fixes"*/
/* *10   23-AUG-1990 16:38:07 VINCE "(SPR 1) added include of esi_stdlib.h when USE_X"*/
/* *9    23-AUG-1990 15:29:38 JULIAN "(SPR 0) fix compiling error"*/
/* *8    14-AUG-1990 22:50:11 GILLESPIE "(SPR 5644) Fix problems with last checkin"*/
/* *7    14-AUG-1990 13:12:52 VINCE "(SPR 5644) Header Standardization"*/
/* *6     2-MAR-1990 11:13:03 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *5     8-FEB-1990 17:06:22 GORDON "(SPR 0) Make quit_finder_server static, like the prototype"*/
/* *4     6-FEB-1990 00:27:15 JULIAN "(SPR -1) X Version"*/
/* *3    14-NOV-1989 00:20:20 JULIAN "(SPR 1) Remove call to tbh_terminate."*/
/* *2    13-NOV-1989 13:51:49 JULIAN "(SPR 1) Integrate external tablet functions into finder."*/
/* *1    19-JUN-1989 12:49:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_QUIT.C*/
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

#include "esi_fi.h"
#include "esi_am.h"
#include "esi_mg.h"
#include "esi_wi.h"
#include "esi_fi_msg.h"
#include "esi_fi_err.h"

#ifdef USE_X
#include "esi_c_lib.h"
#endif

#define FINDER_CDLG_RSRC "fi_dlg"

#include "fi_dlg.rh"

#if USE_PROTOTYPES
static INT quit_finder_server (INT id, INT item, BYTE *pointer);

#else
static INT quit_finder_server ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    FINDER_QUIT is an application, like any other, which terminates the
    Finder program.

    FINDER_QUIT asks the user if she really wants to quit. If the user
    says "yes", then FINDER_QUIT returns FINDER_ERR_TERMINATE as it's
    value so that the main event request loop is notified to perform
    the actual termination processing.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT finder_quit(BYTE *pointer);
    
Private_Functions:
    static INT quit_finder_server(INT id, INT item, BYTE *pointer);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT finder_quit(BYTE *pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT finder_quit (BYTE *pointer)
#else
publicdef INT finder_quit (pointer)
BYTE *pointer;                      /* not used */
#endif
    {
    INT quit_id;                    /* dialog id */
    
    am_declare ("QUIT");
    am_define_resource (AM_DIALOG, FINDER_CDLG_RSRC, VERIFY, quit_finder_server,
                        &quit_id, NULL);
    
    wi_open_dialog (quit_id);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT quit_finder_server(INT id, INT item, BYTE *pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) Dialog id.
    (I) item        -(INT) Dialog button.
    (I) pointer     -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <finder_quit>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT quit_finder_server (INT id, INT item, BYTE *pointer)
#else
static INT quit_finder_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    INT status;                     /* completion status */
    
    if (item EQUALS ITEM_QUIT)
        {
        am_message (AM_STATUS, mg_message (FI_TERM_IN_PROGRESS));
        status = FI_ERR_TERMINATE;
        }
    else
        {
        am_message (AM_STATUS, mg_message (FI_TERM_CANCELLED));
        status = SUCCESS;
        }
    am_quit ();

#ifdef USE_X
    if (status != SUCCESS)
        {
        /*   Terminate Finder.            */
        finder_terminate ();
        
        /*   Return to OS.                */
        am_message (AM_CLOSE_JOURNAL_FILE, (CHAR *)0);
        exit (EXIT_SUCCESS);
        }
#endif
    return status;
    }
/* END:     */
