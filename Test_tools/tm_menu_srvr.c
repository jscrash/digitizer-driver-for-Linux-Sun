/* DEC/CMS REPLACEMENT HISTORY, Element TM_MENU_SRVR.C*/
/* *2    17-AUG-1990 22:28:26 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:32:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element TM_MENU_SRVR.C*/
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

#ifndef ESI_GL_DEFS_H

#include "esi_gl_defs.h"

#endif

#ifndef ESI_WI_H

#include "esi_wi.h"

#endif

#ifndef ESI_AM_INT_H

#include "esi_am_int.h"

#endif

#ifndef ESI_FI_INT_H

#include "esi_fi_int.h"

#endif

#ifndef ESI_MG_H

#include "esi_mg.h"

#endif

#ifndef ESI_TM_MSG_H

#include "esi_tm_msg.h"

#endif

#ifndef ESI_FI_H

#include "esi_fi.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#include "am_test.rh"

#define MAX_MSG 80      /* maximum length of total message */
extern INT tm_am_dump ();
extern INT tm_abort_application ();
extern INT testit ();
extern INT tm_test_rs ();
extern INT tm_test_deferral ();
static INT tm_interval_time ();

static BOOL testit_is_pipe = FALSE;
static BOOL timer_is_started = FALSE;
static BOOL memory_police_active = FALSE;

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT tm_menu_server(INT id,INT item,BYTE *pointer);

Private_Functions:
    static INT tm_interval_time(CHAR *opcode, CHAR *message );

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT tm_menu_server(INT id,INT item,BYTE *pointer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    id              -(INT) Menu id.
    item            -(INT) Menu command.
    pointer         -(BYTE *) Not used.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT tm_menu_server (INT id, INT item, BYTE *pointer)
#else
publicdef INT tm_menu_server (id, item, pointer)
INT id;
INT item;
BYTE *pointer;
#endif
    {
    INT status;                     /* completion status */
    CHAR text[100];
    FINDER_MENU_STRUCT app;
    
    /* ********************************************************************** */
    
    tc_zeroes (&app, sizeof app);
    app.AppName = text;
    
    switch (item)
        {
        
    case GLOBAL_TEST_MENU_DUMP:
        app.Activates.AppEntry = tm_am_dump;
        strcpy (app.AppName, "AM Dump");
        app.Type = EventsFunction;
        status = fih_start_application (&app);
        break;
        
    case GLOBAL_TEST_MENU_ABORT:
        app.Activates.AppEntry = tm_abort_application;
        strcpy (app.AppName, "Abort Application");
        app.Type = EventsFunction;
        status = fih_start_application (&app);
        break;
        
    case GLOBAL_TEST_MENU_ACCOUNTING:
        if (am_static ()->accounting)
            {
            am_static ()->accounting = FALSE;
            wichmni (id, GLOBAL_TEST_MENU_ACCOUNTING, 0);
            am_message (AM_STATUS, mg_message (TM_MENU_SRVR_ACC_OFF));
            }
        else
            {
            am_static ()->accounting = TRUE;
            wichmni (id, GLOBAL_TEST_MENU_ACCOUNTING, 1);
            am_message (AM_STATUS, mg_message (TM_MENU_SRVR_ACC_ON));
            }
        status = SUCCESS;
        break;
        
    case GLOBAL_TEST_MENU_PIPE:
        testit_is_pipe = !testit_is_pipe;
        wichmni (id, GLOBAL_TEST_MENU_PIPE, testit_is_pipe);
        status = SUCCESS;
        break;
        
    case GLOBAL_TEST_MENU_TEST_APP:
        strcpy (text, "Test Application");
        if (testit_is_pipe)
            strcat (text, " (Pipe)");
        else
            strcat (text, " (Event Driven)");
        app.Activates.AppEntry = testit;
        app.Type = testit_is_pipe ? NoEventsFunction : EventsFunction;
        status = fih_start_application (&app);
        break;
        
    case GLOBAL_TEST_MENU_TEST_RS:
        app.Activates.AppEntry = tm_test_rs;
        strcpy (app.AppName, "Test Resource");
        app.Type = EventsFunction;
        status = fih_start_application (&app);
        break;
        
    case GLOBAL_TEST_MENU_MEM_POLICE:
        memory_police_active = !memory_police_active;
        wichmni (id, GLOBAL_TEST_MENU_MEM_POLICE, memory_police_active);
        break;
        
    case GLOBAL_TEST_MENU_INTERVAL_TIME:
        if (timer_is_started)
            {
            tm_interval_time ("END", "");
            }
        else
            {
            tm_interval_time ("START", "");
            }
        timer_is_started = !timer_is_started;
        wichmni (id, GLOBAL_TEST_MENU_INTERVAL_TIME, timer_is_started);
        status = SUCCESS;
        break;
    case GLOBAL_TEST_MENU_DEFERRAL:
        app.Activates.AppEntry = tm_test_deferral;
        strcpy (app.AppName, "Test Deferral");
        app.Type = EventsFunction;
        status = fih_start_application (&app);
        break;
        
    case GLOBAL_TEST_MENU_END_TEST:
        wivismn (id, OFF);
        status = SUCCESS;
        fi_sync_mn (fiz_finder_menus ()->id);
        break;
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Tm_interval_time is called as a programmer facility to
    simplify instrumenting interval CPU time consummed by 
    a process.
            
    Tm_interval_time is first called with opcode "START" to initialize
    the interval time, then with opcode "END" and a message string
    to cause the corresponding interval time to be displayed, via
    am_message, in the form:
            
    <message string> Interval CPU Time ddd.dd Sec.
            
Prototype:
    static INT tm_interval_time(CHAR *opcode, CHAR *message );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    opcode          -(CHAR *) opcode    "START", begin interval timing,
                        "END", end interval timing, display status message.
    message         -(CHAR *) String to be displayed along with interval time.
            
Return Value/Status:
    SUCCESS - If opcode is recognized.
    FAIL - If opcode is invalid, or message string too long.

Scope:
    PRIVATE to <module_description>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT tm_interval_time (CHAR *opcode, CHAR *message)
#else
static INT tm_interval_time (opcode, message)
CHAR *opcode, *message;
#endif
    {
    static DOUBLE start, end, interval;
    INT status = SUCCESS;
    CHAR msg[MAX_MSG];
    
    interval = 0.0;
    
    if (strcmp (opcode, "START") == 0)
        {
        start = ho_cpu_time ();
        }
    else if (strcmp (opcode, "END") == 0)
        {
        end = ho_cpu_time ();
        interval = end - start;
        if (strlen (message) <= MAX_MSG - 30)
            {
            sprintf (msg, "%s Interval CPU Time %6.2lf Sec.", message, interval);
            am_message (AM_STATUS, msg);
            }
        else
            status = FAIL;
        }
    else
        status = FAIL;
    
    return status;
    }
/* END:     */
