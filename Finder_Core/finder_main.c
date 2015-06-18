/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_MAIN.C*/
/*  17   15-OCT-1991 16:20:04 PURNA "(SPR 2567) set aspect ratio ON when initializing finder"*/
/* *16   10-JUL-1991 15:36:43 SANTIAGO "(SPR -1) increased argc from 20 to 40 to allow for command line options"*/
/* *15    3-MAY-1991 16:01:56 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECGKS, DECW/PRIORGKS and DECW/DECGKS version"*/
/* *14    6-NOV-1990 11:15:06 PURNA "(SPR 6087) add version number"*/
/* *13   29-OCT-1990 13:23:30 GILLESPIE "(SPR 35) Add esi_sl_meth.ah"*/
/* *12   14-AUG-1990 22:22:39 GILLESPIE "(SPR 2001) Changes for new include files"*/
/* *11   14-AUG-1990 13:12:42 VINCE "(SPR 5644) Header Standardization"*/
/* *10    2-MAR-1990 11:12:55 PURNA "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *9    26-FEB-1990 18:57:32 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *8     7-FEB-1990 06:53:38 GORDON "(SPR 0) Once again, remove references to wm_set_zoom_aspect!!"*/
/* *7     6-FEB-1990 00:17:10 JULIAN "(SPR -1) X Version "*/
/* *6    11-JAN-1990 11:00:23 GORDON "(SPR 0) Remove reference to wm_set_zoom_aspect"*/
/* *5     5-DEC-1989 14:44:15 GORDON "(SPR 0) fix bad comment delimiter"*/
/* *4    19-NOV-1989 12:30:54 GILLESPIE "(SPR 5075) Tweek menu visibility logic"*/
/* *3    18-NOV-1989 09:48:28 HALL "(SPR 5075) Global overhaul"*/
/* *2    26-JUN-1989 10:43:02 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:49:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element FINDER_MAIN.C*/
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

#include "esi_fi_int.h"

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_GB_H
#include "esi_gb.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#ifndef ESI_SL_METH_AH
#include "esi_sl_meth.ah"
#endif
 
#ifndef ESI_AS_GETP_H
#include "esi_as_getp.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_FI_MSG_H
#include "esi_fi_msg.h"
#endif

#include "esi_fi_err.h"

/* Resource files */

#define FINDER_CDLG_RSRC "FI_DLG"

#include "fi_dlg.rh"

#if 0

#define TEST_MENU_RSRC   "AM_TEST"

#include "am_test.rh"

#endif

#ifdef MOTIF
#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif
#endif

/* LOCAL FUNCTIONS DEFINED HERE */
#if USE_PROTOTYPES
privatedef INT finder_initialization (AS_GET_PROJECT_WORKSPACE *);
privatedef INT finder (VOIDPTR);
#if 0
extern INT tm_menu_server ();
#endif
publicdef INT main (int argc, char **argv);
#else
privatedef INT finder_initialization ();
privatedef INT finder ();
#if 0
extern INT tm_menu_server ();
#endif
publicdef INT main ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    FINDER is the C main() function for the Finder system.

    This function's main purpose is to initialize the system, and then
    to request and dispatch events until the user requests termination,
    and then finally to terminate the system.

    FINDER also contains various utility functions used by FINDER and
    its servers.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT main(int argc, char *argv[]);
    
Private_Functions:
    privatedef INT finder_initialization(AS_GET_PROJECT_WORKSPACE *a);
    
    privatedef INT finder(VOIDPTR pointer);
    
-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    See above.
    
Prototype:
    publicdef INT main(int argc, char *argv[]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    Entry parameters.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT main (int argc, char *argv[]) 
#else
publicdef INT main (argc, argv)
int argc;
char *argv[];
#endif
    {
    INT status;
    INT event_class;                /* event class from wirqevt id from        */
    INT id;                         /* wirqevt item from wirqevt            */
    INT item;
    CHAR msg[81];
    INT i;                          /* counter TRUE if id is one of FINDER's  */
    CHAR *s;                        /* debug/trace node                */
#ifdef primos
    SHORT len;
    privatedef CHAR access_violation[] = "ACCESS_VIOLATION$";
    privatedef CHAR illegal_segno[] = "ILLEGAL_SEGNO$";
    privatedef CHAR pointer_fault[] = "POINTER_FAULT$";
    extern VOID prime_onunit ();
    fortran mkon$p ();
#endif
    APPLICATION_STATIC *a;
    CONFIGURATION_WORKSPACE *c;
    FINDER_MENU_STRUCT *m;
    FINDER_WORKSPACE *w;            /* pointer to command line option        */
#ifdef MOTIF
    XtAppContext AppContext;
#endif

    
    /* ********************************************************************** */
    
    /* if this image not defined as a foreign
       image, then argc may be outrageous        */
    
    if ((argc > 20) OR (argc < 1))
        argc = 1;
    
    status = finder_initialize (argc, argv);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        finder_terminate ();
        exit (EXIT_FAILURE);
        }
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & c);
    
    /*   Define condition handlers.        */
    
    if (c->on_units)
        {
#ifdef vms
        extern void VAXC$ESTABLISH ();
        
        VAXC$ESTABLISH (fiz_onunit);
#endif      /* VMS */

#ifdef primos           /* MUST SET UP ONUNITS IN LINE HERE! */
        len = strlen (access_violation);
        mkon$p (access_violation, len, prime_onunit);
        len = strlen (illegal_segno);
        mkon$p (illegal_segno, len, prime_onunit);
        mkon$p (pointer_fault, len, prime_onunit);
#endif
        }
    else
        {
        am_message (AM_STATUS, mg_message (FI_ON_UNITS_SUPPRESSED));
        }
    /*   Indicate FINDER Global workspace has
         not been accessed yet.            */
    
    w = (FINDER_WORKSPACE *)0;
    
    /*   Start the Finder application.        */
    
    am_start_application (finder, (VOID *)NULL);
    
    am_open_workspace ("FINDER", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & w);
    
    /*   Access application_static structure. */
    
    a = am_static ();
    
    /* Fetch 'Pick from Menus' string */
    
    strcpy (msg, mg_message (FI_PICK_FROM_MENUS));
    
    /*   Request events forever (until Finder
         is terminated by user.            */
    
    setjmp (fiz_static ()->event_env);
    
    /*******************/
    /* BEGIN MAIN LOOP */
    /*******************/

#ifdef USE_X
#ifdef MOTIF
    AppContext = XtWidgetToApplicationContext (wiv.toplevel);    
    XtAppMainLoop (AppContext);
#else
    XtMainLoop ();
#endif
    return EXIT_SUCCESS;
#else
    FOREVER
        {
        wi_request_event (-1, &event_class, &id, &item);
        
        /*   Become application manager.  The
             reason for this is that, if an
             application just quit, the services
             will no longer work.            */
        
        a->application_id = 0;
        
        /*   If Finder global workspace is
         available (and hence the Finder set
         of menus are loaded ...            */
        
        wi_set_dialog_text (w->finder_dialog_id, FINDER_DIALOG_TEXT_1, "");
        
        /*   If event is not from one of the
         Finder menus, then take down all of
         the visible Finder sub-menus if any
         are visible on the menu bar.        */
        
        if (event_class EQUALS WI_MENU_EVENT)
            {
            for (m = fiz_finder_menus (); m->Type != Undefined; m += m->item)
                {
                if (id EQUALS m->id)
                    {
                    fi_sync_mn (id);
                    break;
                    }
                }
            }
        /*   Dispatch the event to appropriate
         application event server.        */
        
        
        status = am_dispatcher (event_class, id, item);
        
        /*   If server returns an error status
         ...                    */
        
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            
            /*   If user quit Finder, then stop
                 requesting events.            */
            
            
            if (status EQUALS FI_ERR_TERMINATE)
                {
                break;              /*   out of while(TRUE) loop            */
                }
            /*   Process application server error.    */
            
            else
                {
                /* lets display the actual error message before the
                 * generic one.                         VSC  3/8/89  */
                am_message (AM_STATUS, mg_message (status));
                sprintf (msg, mg_message (FI_DISPATCHER_SAW_ERROR), status);
                am_message (AM_STATUS, msg);
                strcpy (msg, mg_message (FI_PICK_FROM_MENUS));
                }
            }
        wi_set_dialog_text (w->finder_dialog_id, FINDER_DIALOG_TEXT_1, msg);
        }
    /* end of main event loop            */
    
    /*   Terminate Finder.            */
    
    finder_terminate ();
    
    /*   Return to OS.                */
    
    am_message (AM_CLOSE_JOURNAL_FILE, (CHAR *)0);
    
    exit (EXIT_SUCCESS);
#endif
    }                               /* end of FINDER_MAIN */
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    FINDER is the Finder application program entry point. It defines
    all of the Finder menus and the Finder dialog box, and creates
    certain global Workspaces.

Prototype:
    privatedef INT finder(VOIDPTR pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) pointer     -(VOIDPTR)
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
privatedef INT finder (VOIDPTR pointer) 
#else
privatedef INT finder (pointer)
VOIDPTR pointer;
#endif
    {
    INT status;
    PROJECT_NAME project_name;
    INT project_len;
    AS_GET_PROJECT_WORKSPACE *a;
    
    /* ********************************************************************** */
    
    /*   Assume success.                */
    
    status = SUCCESS;
    
    /*   Declare dialog box tag.            */
    
    am_declare ("FINDER");
    
    /*   If the project is not already set,
         then get project from user.        */
    
    qs_inq_c (QS_PROJECT_NAME, project_name, &project_len);
    if (project_len > 0)
        {
        finder_initialization ((AS_GET_PROJECT_WORKSPACE *)0);
        }
    else
        {
        
        /*   Get project from user.  Start Finder
             application when the project has
             been set.                */
        
        am_define_workspace ("PROJECT", AM_APPLICATION,
                             sizeof(AS_GET_PROJECT_WORKSPACE), (VOIDPTR *) & a);
        a->quit = FALSE;
        a->cancel = FALSE;
        a->no_selection = FALSE;
        am_start_server (as_get_project, finder_initialization, (VOIDPTR)a);
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    privatedef INT finder_initialization(AS_GET_PROJECT_WORKSPACE *a);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) a           -(AS_GET_PROJECT_WORKSPACE *) 
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <main>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
privatedef INT finder_initialization (AS_GET_PROJECT_WORKSPACE *a) 
#else
privatedef INT finder_initialization (a)
AS_GET_PROJECT_WORKSPACE *a;
#endif
    {
    INT status;                     /* Application completion status current  */
    INT gl_id;
    INT gl_item;
    INT id;
    CHAR text[100];                 /* FINDER workspace pointer to        */
    PROJECT_NAME project_name;      /* CONFIGURATION workspace            */
    FINDER_WORKSPACE *w;
    CONFIGURATION_WORKSPACE *c;
    extern INT wm_control ();
    
    /* ********************************************************************** */
    
    /*   Access CONFIGURATION global        */
    /*   workspace.                */
    
    am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)(VOIDPTR *) & c);
    
    /*   Set project if one is supplied.        */
    
    if (a IS_NOT_EQUAL_TO (AS_GET_PROJECT_WORKSPACE *)0)
        {
        qs_set_c (QS_PROJECT_NAME, a->project, strlen (a->project));
        sprintf (text, mg_message (FI_PROJECT_IS), a->project);
        am_message (AM_STATUS, text);
        am_release_workspace ("PROJECT", AM_APPLICATION);
        }
    /*   Define FINDER global workspace.        */
    
    am_define_workspace ("FINDER", AM_GLOBAL, sizeof(FINDER_WORKSPACE),
                         (VOIDPTR *) & w);
    
    /*   Say loading Finder menus.        */
    
    am_message (AM_STATUS, mg_message (FI_LOADING_MENUS));

#if 0
    /*   Load test menu if requested.        */
    
    if (c->test)
        {
        status = am_define_resource (AM_MENU, "am_test", GLOBAL_TEST_MENU,
                                     tm_menu_server, &id, NULL);
        if (status EQUALS SUCCESS)
            {
            wivismn (id, ON);
            
            /*   Check "Accounting" command if
                 accounting is enabled.            */
            
            if (am_static ()->accounting)
                {
                wichmni (id, GLOBAL_TEST_MENU_ACCOUNTING, ON);
                }
            }
        }
#endif
    fi_init_menus ();
    
    /*  enable/disable graphical GLOBAL menu picks, */
    /*  depending on MACONLY mode           */
    
    if (fih_query_menu_function (&gl_id, &gl_item, gb_whereami) == SUCCESS)
        wi_enable_menu_item (gl_id, gl_item, !(c->maconly));
    
    if (fih_query_menu_function (&gl_id, &gl_item, wm_control) == SUCCESS)
        wi_enable_menu_item (gl_id, gl_item, !(c->maconly));
    
    /*   Define and open Finder dialog box.   */
    
    am_define_resource (AM_DIALOG, FINDER_CDLG_RSRC, FINDER_DIALOG, NULL_FUNCTION_PTR,
                        &w->finder_dialog_id, NULL);
    sprintf(text,"Finder Version %d.%d",FINDER_VERSION,FINDER_VERSION_INCREMENT);
    wi_set_dialog_title(w->finder_dialog_id,text);
    wi_set_dialog_text (w->finder_dialog_id, FINDER_DIALOG_TEXT_1,
                        mg_message (FI_PICK_FROM_MENUS));
    qs_inq_c (QS_PROJECT_NAME, project_name, (INT *)0);
    sprintf (text, mg_message (FI_PROJECT_NAME), project_name);
    wi_set_dialog_text (w->finder_dialog_id, FINDER_DIALOG_TEXT_2, text);
    wi_open_dialog (w->finder_dialog_id);
    
    /*   Remove status message.            */
    
    am_message (AM_DIALOG, (CHAR *)0);
    
    /*   Make Finder menu visible.        */
    
    fi_sync_mn (fiz_finder_menus ()->id);
    
    return status;
    }
/* END:     */
