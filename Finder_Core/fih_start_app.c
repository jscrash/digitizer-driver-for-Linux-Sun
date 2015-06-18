/* DEC/CMS REPLACEMENT HISTORY, Element FIH_START_APP.C*/
/* *6    14-AUG-1990 22:50:05 GILLESPIE "(SPR 5644) Fix problems with last checkin"*/
/* *5    14-AUG-1990 13:12:34 VINCE "(SPR 5644) Header Standardization"*/
/* *4     1-APR-1990 10:52:26 GILLESPIE "(SPR 1) Pass name/password and -project name to .com and .exe files"*/
/* *3    18-NOV-1989 09:48:25 HALL "(SPR 5075) Global overhaul"*/
/* *2    31-JUL-1989 14:19:51 GORDON "(SPR 101) add ProgramNoWait and CPNoWait types to FINDER_MENU_STRUCT"*/
/* *1    19-JUN-1989 12:49:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FIH_START_APP.C*/
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

#include "esi_fi_int.h"
#include "esi_am_int.h"

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#include "esi_fi_msg.h"

static INT fiz_get_name_proj (CHAR *buffer) ;
/* File Description ---------------------------------------------------------
Overview:
    FIH_START_APPLICATION starts the specified application program
    and issues a "startup" message in the Message window.

    The menus are synced so that only the Finder menu remains.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT fih_start_application(FINDER_MENU_STRUCT *pm);
    
Private_Functions:
    static INT fiz_get_name_proj(CHAR *buffer);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
Prototype:
    publicdef INT fih_start_application(FINDER_MENU_STRUCT *pm);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pm          -(FINDER_MENU_STRUCT *) Applications program structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT fih_start_application (FINDER_MENU_STRUCT *pm) 
#else
publicdef INT fih_start_application (pm)
FINDER_MENU_STRUCT *pm;
#endif
    {
    INT status;
    CHAR text[241];
    CHAR text1[241];
    FILENAME program_name;
    EVENT_ID_STRUCT event;
    PROJECT_NAME project;
    
    /************************************************************************/
    
    /*  Construct start message. */
    
    sprintf (text, "\"%s\" ",
             pm->AppName EQUALS (CHAR *)0 ? "Undefined" : pm->AppName);
    
    /*  If application is not present, say
        not implemented. */
    
    if (pm->Activates.AppEntry EQUALS NULL_FUNCTION_PTR AND (pm->
                                                            Type EQUALS EventsFunction 
                                                             OR pm->
                                                          Type EQUALS 
                                                             NoEventsFunction))
        {
        strcat (text, mg_message (FI_APP_NOT_IMPLEMENTED));
        am_message (AM_STATUS, text);
        status = SUCCESS;
        }
    /*  If application is present, say started. */
    
    else
        {
        strcat (text, mg (FI_APP_STARTED));
        sprintf (text1, text, am_static ()->next_application_id);
        am_message (AM_STATUS, text1);
        
        switch (pm->Type)
            {
            
        case NoEventsFunction:
            
            /*  Get rid of all static menus so that pipes
                will not get events from them. */
            
            fi_push ();
            event.menu_id = pm->id;
            event.item_id = pm->item;
            status = am_start_application (pm->Activates.AppEntry, (VOID *) & event);
            fi_pop ();
            break;
            
        case EventsFunction:        /* 'Normal' Finder Application */
            
            /* disable the menu entry - it is
                re-enabled by calling am_quit */
            
            if (pm->id > 0)
                {
                wi_enable_menu_item (pm->id, pm->item, pm->flag = OFF);
                }
            event.menu_id = pm->id;
            event.item_id = pm->item;
            status = am_start_application (pm->Activates.AppEntry, (VOID *) & event);
            break;
            
        case ProgramName:           /* application is stand-alone program */
        case ProgramNowait:
            
            wi_enable_keyboard (ON);
            
            ho_add_path ("ESI$BIN", pm->AppName, sizeof(FILENAME), program_name);
            fiz_get_name_proj (text1);
#ifdef vms
            
            /* Implementation Note:  In order to allow command line arguments to be
            passed to an executable program in VMS, the executable must be defined as a
            Foreign Command.  This is done by the following assignment prior to
            running the program:
            
                myprog :== $disk:[directory]myprog
            
            where disk is the name of the disk, and directory the name of the directory
            where myprog.exe is located. The device and directory path
            to the program are mandatory.  The leading dollar sign is essential
            and is NOT part of the disk name.
            
            */
            sprintf (text, "%s %s", program_name, text1);
#endif

#ifdef primos
            sprintf (text, "R %s %s", program_name, text1);
#endif

#if UNIX
            sprintf (text, "%s %s", program_name, text1);
#endif
            status = ho_syscom_nw (text, pm->Type == ProgramName ? 0 : 1);
            break;
            
        case CPName:                /* application is stand-alone command procedure */
        case CPNowait:
            
            wi_enable_keyboard (ON);
            
            ho_add_path ("ESI$BIN", pm->AppName, sizeof(FILENAME), program_name);
            fiz_get_name_proj (text1);
#ifdef vms
            sprintf (text, "@%s %s", program_name, text1);
#endif

#ifdef primos
            sprintf (text, "R %s %s", program_name, text1);
#endif

#if UNIX
            sprintf (text, "%s %s", program_name, text1);
#endif
            status = ho_syscom_nw (text, pm->Type == CPName ? 0 : 1);
            break;
            
        case CommandName:           /* application is CLI command */
            
            wi_enable_keyboard (ON);
            
            status = ho_syscom_nw (pm->AppName, 0);
            break;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Construct the login information necessary for Finder stand-alone
    programs to get started.  This includes the name and password of the
    current session, and the name of the project, preceeded by the symbol
    -project.  For example, if the current analyst name is JONES with a
    password of BOY and the current project is WHAMO, then the following
    string would be produced:

        "JONES/BOY" -project WHAMO

    If JONES is attached to a remote database name FARGO, then the produced
    string is

        "JONES/BOY@FARGO" -project WHAMO

Prototype:
    static INT fiz_get_name_proj(CHAR *buffer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) buffer      -(CHAR *) Application startup buffer.
    
Return Value/Status:
    Function returns the length of the startup buffer.
    
Scope:
    PRIVATE to <fih_start_application>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT fiz_get_name_proj (CHAR *buffer) 
#else
static INT fiz_get_name_proj (buffer)
CHAR *buffer;
#endif
    {
    PROJECT_NAME project;
    ANALYST_NAME analyst;
    ORACLE_NAME password;
    ORACLE_NAME database;
    INT len, dlen;
    
    qs_inq_c (QS_PROJECT_NAME, project, (INT *)0);
    qs_inq_c (QS_FINDER_ACCOUNT, analyst, (INT *)0);
    qs_inq_c (QS_FINDER_PASSWORD, password, (INT *)0);
    qs_inq_c (QS_DATABASE, project, &dlen);
    
    if (dlen > 0)
        {
        len = sprintf ((char *)buffer, "\"%s/%s@%s\" -project %s", analyst, password,
                       database, project);
        }
    else
        {
        len = sprintf ((char *)buffer, "\"%s/%s\" -project %s", analyst, password,
                       project);
        }
    return len;
    }
/* END:     */
