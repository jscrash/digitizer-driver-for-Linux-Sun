/* DEC/CMS REPLACEMENT HISTORY, Element GB_HOST_PROC.C*/
/* *2    14-AUG-1990 13:26:34 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:51:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_HOST_PROC.C*/
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

#include "esi_am.h"
#include "esi_c_lib.h"
#include "esi_wi.h"
#include "gb_host_processor.rh"
static CHAR RESOURCE_FILE[] = 
    {
    "GB_HOST_PROCESSOR"
    };


static CHAR last_stmt[256] = 
    {
    ""
    };


static INT gb_host_dialog_server ();

/* File Description ---------------------------------------------------------
Overview:
    Ad-hoc host command processor using ESI's workstation interface.
    The user is presented with a dialog box which takes a Host command.
    The statement is executed when the EXECUTE button is pressed.  The results
    are written to the message window and optionally journalled using the
    standard ESI journalling system.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT gb_host_processor(BYTE *dummy);

Private_Functions:
    static INT gb_host_dialog_server(INT id, INT item, BYTE *ptr);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT gb_host_processor(BYTE *dummy);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    dummy           -(BYTE *)

Return Value/Status:
    None.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_host_processor (BYTE *dummy)
#else
publicdef INT gb_host_processor (dummy)
BYTE *dummy;
#endif
    {
    INT id;                         /* dialog box ID */
    INT status = SUCCESS;
    
    am_declare ("HOST");
    
    /* define the dialog box */
    am_define_resource (AM_DIALOG, RESOURCE_FILE, HOST_PROCESSOR_CONTROL,
                        gb_host_dialog_server, &id, NULL);
    
    /* initialize dialog state */
    wiscdt (id, HOST_PROCESSOR_TEXT, last_stmt);
    wi_open_dialog (id);
    wi_activate (id, HOST_PROCESSOR_TEXT, ON);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT gb_host_dialog_server(INT id, INT item, BYTE *ptr);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT)
    item            -(INT)
    ptr             -(BYTE *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <gb_host_proc.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT gb_host_dialog_server (INT id, INT item, BYTE *ptr)
#else
static INT gb_host_dialog_server (id, item, ptr)
INT id;
INT item;
BYTE *ptr;
#endif
    {
    INT value;
    CHAR text[256];
    INT status;
    /* service dialog events */
    
    switch (item)
        {
    case HOST_PROCESSOR_DONE:
        am_quit ();
        return SUCCESS;
        
    case HOST_PROCESSOR_EXECUTE:
        wiqccdi (id, HOST_PROCESSOR_TEXT, &value, text);
        if (strlen (text) > 0)
            {
            am_message (AM_STATUS, text);   /* print out the command */
            /*---------------------*/
            /* run the command;    */
            /* wait for completion */
            /*---------------------*/
            status = ho_syscom_nw (text, 0);
            
            strcpy (last_stmt, text);   /* remember the last command */
            }
        wi_open_dialog (id);        /* pop control box back to front */
        wi_activate (id, HOST_PROCESSOR_TEXT, ON);
        break;
        }
    return SUCCESS;
    }
/* END:     */
