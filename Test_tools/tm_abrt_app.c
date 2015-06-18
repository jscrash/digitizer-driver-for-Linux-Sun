/* DEC/CMS REPLACEMENT HISTORY, Element TM_ABRT_APP.C*/
/* *3    17-AUG-1990 22:27:55 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-NOV-1989 09:48:50 HALL "(SPR 5075) Global overhaul"*/
/* *1    19-JUN-1989 13:32:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TM_ABRT_APP.C*/
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

#include "esi_c_lib.h"
#include "esi_am_int.h"
#include "esi_fi.h"
#include "esi_mg.h"
#include "esi_tm_msg.h"
#include "esi_wi.h"
#include "am_test.rh"
static CHAR DIALOG_RSRC[] = "AM_TEST";

#if USE_PROTOTYPES
static INT abort_server (INT id, INT item, BYTE *pointer);
#else
static INT abort_server ();
#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tm_abort_application(BYTE *pointer);

Private_Functions:
    static INT abort_server(INT id,INT item,BYTE *pointer);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT tm_abort_application(BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tm_abort_application (BYTE *pointer)
#else
publicdef INT tm_abort_application (pointer)
BYTE *pointer;                      /* not used */
#endif
    {
    INT id;                         /* dialog id */
    
    /* ********************************************************************** */
    
    am_declare ("ABORT APPLICATION");
    am_define_resource (AM_DIALOG, DIALOG_RSRC, ABORT_APPLICATION_DIALOG, abort_server,
                        &id, NULL);
    wiopcd (id);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT abort_server(INT id,INT item,BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Dialog id.
    item            -(INT) Dialog button.
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <tm_abrt_app.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT abort_server (INT id, INT item, BYTE *pointer)
#else
static INT abort_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    INT status;                     /* completion status */
    INT cdvalue;                    /* value */
    INT application_id;             /* applicaton id */
    INT current_application_id;     /* current application id */
    CHAR text[80];                  /* messages */
    APPLICATION_LIST app;           /* entry in APPLICATION_LIST */
    APPLICATION_STATIC *a;          /* pointer to APPLICATION_STATIC */
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    a = am_static ();
    
    if (item == ABORT_APPLICATION_ABORT)
        {
        wiqccdi (id, ABORT_APPLICATION_ID, &cdvalue, text);
        application_id = atoi (text);
        if (application_id)
            {
            if (application_id == a->application_id)
                sprintf (text, mg (TM_ABORT_CANT_SUICIDE));
            else
                {
                if (btfind (&a->application_list, &application_id, &app))
                    {
                    current_application_id = a->application_id;
                    a->application_id = application_id;
                    status = am_abort ();
                    a->application_id = current_application_id;
                    sprintf (text, mg (TM_ABORT_ABORTED), application_id);
                    status = SUCCESS;
                    }
                else
                    sprintf (text, mg (TM_ABORT_NOT_ACTIVE), application_id);
                }
            am_message (AM_STATUS, text);
            }
        else
            status = am_start_application (finder_quit, (VOID *)NULL);
        }
    am_quit ();
    
    return status;
    }
/* END:     */
