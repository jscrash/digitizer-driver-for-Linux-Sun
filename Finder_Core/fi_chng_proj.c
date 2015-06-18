/* DEC/CMS REPLACEMENT HISTORY, Element FI_CHNG_PROJ.C*/
/* *4    14-AUG-1990 13:13:03 VINCE "(SPR 5644) Header Standardization"*/
/* *3    26-JUN-1989 10:43:18 GILLESPIE "(SPR 20) Fix castings"*/
/* *2    20-JUN-1989 11:15:17 GILLESPIE "(SPR 20) Fix minor casting problem"*/
/* *1    19-JUN-1989 12:49:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FI_CHNG_PROJ.C*/
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
#include "esi_fi.h"
#include "esi_qs.h"
#include "esi_am.h"
#include "esi_as_getp.h"
#include "esi_mg.h"
#include "esi_fi_msg.h"

#if USE_PROTOTYPES
static INT change_project_server (AS_GET_PROJECT_WORKSPACE *a);

#else
static INT change_project_server ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    FI_CHANGE_PROJECT is an application which allows the user to change
    the project.

    FI_CHANGE_PROJECT uses the application server AS_GET_PROJECT to get the
    user's project selection. CHANGE_PROJECT then uses the selection to set the
    current project.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT fi_change_project(BYTE *pointer);
    
Private_Functions:
    static INT change_project_server(AS_GET_PROJECT_WORKSPACE *a);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT fi_change_project(BYTE *pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fi_change_project (BYTE *pointer) 
#else
publicdef INT fi_change_project (pointer)
BYTE *pointer;
#endif
    {
    INT status;                     /* Completion status. */
    INT junk;                       /* Scratch */
    AS_GET_PROJECT_WORKSPACE *a;    /* AS_GET_PROJECT protocol pointer */
    
    /* ********************************************************************** */
    
    am_declare ("FI_CHANGE_PROJECT");
    
    /*   Define workspace for server protocol. */
    
    am_define_workspace ("PROJECT", AM_APPLICATION, sizeof(AS_GET_PROJECT_WORKSPACE),
                         (VOIDPTR *) & a);
    
    /*   Get current project and
         use it as the default project. */
    
    qs_inq_c (QS_PROJECT_NAME, a->project, &junk);
    
    /*   Allow user to cancel or quit. */
    
    a->cancel = TRUE;
    a->quit = TRUE;
    a->no_selection = FALSE;
    
    /*   Start server. */
    
    status = am_start_server (as_get_project, change_project_server, a);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    static INT change_project_server(AS_GET_PROJECT_WORKSPACE *a);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) a           -(AS_GET_PROJECT_WORKSPACE *) AS_GET_PROJECT protocol pointer.

Return Value/Status:
    Function returns status from am_quit().
    
Scope:
    PRIVATE to <fi_change_project>
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT change_project_server (AS_GET_PROJECT_WORKSPACE *a) 
#else
static INT change_project_server (a)
AS_GET_PROJECT_WORKSPACE *a;
#endif
    {
    CHAR text[100];                 /* Messages */
    
    /* ********************************************************************** */
    
    /*   If user selected an project, set the
         new project. */
    
    if (a->status EQUALS SUCCESS)
        {
        qs_set_c (QS_PROJECT_NAME, a->project, strlen (a->project));
        
        /*   Say what the name of the new project is. */
        
        sprintf (text, mg_message (FI_PROJECT_IS), a->project);
        am_message (AM_STATUS, text);
        }
    /*   Go away. */
    
    return am_quit ();
    }
/* END:     */
