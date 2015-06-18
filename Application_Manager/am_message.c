/* DEC/CMS REPLACEMENT HISTORY, Element AM_MESSAGE.C*/
/* *13   14-AUG-1991 11:57:07 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *12   18-JUN-1991 15:26:27 KEE "(SPR -1) changes for db_verb_tab calling am_message(AM_DIALOG,text), if there's no x window*/
/*open, simply return.  db_verb_tab can be linked w or w/o -w flag"*/
/* *11   23-OCT-1990 13:51:31 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *10   23-AUG-1990 16:11:31 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *9    23-AUG-1990 15:29:01 JULIAN "(SPR 0) fix compiling error"*/
/* *8    23-JUL-1990 13:42:44 KEE "(SPR -1) replace \n or \r to blank "*/
/* *7    20-JUL-1990 12:04:56 KEE "(SPR -1) Put am_status message at toplevel title bar"*/
/* *6    27-JUN-1990 17:24:46 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5     1-JUN-1990 12:14:33 SCD "(SPR 1) Fix XSetInputFocus error by closing dialog before setting text."*/
/* *4    26-FEB-1990 18:57:14 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *3     5-SEP-1989 10:08:22 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *2    30-JUN-1989 09:28:39 GILLESPIE "(SPR 30) Consolidate all of the internal functions - make them static"*/
/* *1    19-JUN-1989 12:00:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_MESSAGE.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*                            Exploration Systems                                   */
/*                            c/o Finder Graphics Systems                           */
/*                            201 Tamal Vista Blvd.                                 */
/*                            Corte Madera, CA  94925                               */
/*                            (415) 927-0100                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"
#include "esi_am_int.h"
#include "esi_am_err.h"

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifdef USE_X

#include "esi_wi_int.x"

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#include <X11/Xatom.h>

#endif

#if USE_PROTOTYPES
static INT amz_message_dialog (CHAR *text);
static INT amz_message_status (CHAR *text);
static INT amz_message_journal (CHAR *text);
static CHAR *amz_message_prefix (void);
static INT amz_message_alert (CHAR *text);
static INT amz_message_report (CHAR *text);
static INT amz_message_file_report (CHAR *text);
static INT amz_message_file_status (CHAR *text);

#else
static INT amz_message_dialog ();
static INT amz_message_status ();
static INT amz_message_journal ();
static CHAR *amz_message_prefix ();
static INT amz_message_alert ();
static INT amz_message_report ();
static INT amz_message_file_report ();
static INT amz_message_file_status ();

#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    The AM_MESSAGE function provides a generalized interface for                  */
/*    programs which wish to write status messages to the "message"                 */
/*    window, or to alert the operator of error level conditions                    */
/*    via an alert box.  Whether or not an alert box is required,                   */
/*    the message is written to the "message" window.                               */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT am_message(INT level, CHAR *text);                              */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) level       -(INT) message level.                                         */
/*    (I) text        -(CHAR *) message text.                                       */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successfull message display.                                        */
/*    AM_ERR_CANT_OPEN_FILE - Cannot open the journal file.                         */
/*    AM_ERR_FILE_NOT_OPEN - Journal file is not open to write message.             */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT am_message (INT level, CHAR *text)
#else
publicdef INT am_message (level, text)
int level;                          /* message level                                */
char *text;                         /* message text                                 */
#endif
    {
    APPLICATION_STATIC *application_static = am_static ();
    
    INT status;                     /* completion status                            */
    
    /* ********************************************************************         */
    
    switch (level)
        {
        
    case AM_DIALOG:
        status = amz_message_dialog (text);
        break;
        
    case AM_OPEN_JOURNAL_FILE:
        application_static->journal_file = ho_open (text, "w", NULL);
	status = ho_lock(application_static->journal_file, ON);
        if (application_static->journal_file && status == SUCCESS)
            status = SUCCESS;
        else
            status = AM_ERR_CANT_OPEN_FILE;
        break;
        
    case AM_ALERT:
        status = amz_message_status (text);
        status = amz_message_alert (text);
        break;
        
    case AM_CLOSE_JOURNAL_FILE:
        if (application_static->journal_file)
            {
	    status = ho_lock(application_static->journal_file, OFF);
            fclose (application_static->journal_file);
            application_static->journal_file = NULL_FILE_PTR;
            status = SUCCESS;
            }
        else
            status = AM_ERR_FILE_NOT_OPEN;
        break;
        
    case AM_STATUS:
        status = amz_message_status (text);
        break;
        
    case AM_FILE_STATUS:
        status = amz_message_file_status (text);
        break;
        
    case AM_REPORT:
        status = amz_message_report (text);
        break;
        
    case AM_FILE_REPORT:
        status = amz_message_file_report (text);
        break;
        }
    return status;
    }

/************************************************************************************/
static INT amz_message_alert (text) /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    APPLICATION_LIST app;
    INT application_id;
    APPLICATION_STATIC *application_static = am_static ();
    
    /********************************************************************************/
    
    application_id = application_static->application_id;
    
    /*   If caller is a server, use application                                     */
    /*     id and tag of parent for MAC's sake so                                   */
    /*     that, to the user, it all looks like                                     */
    /*     the same application.                                                    */
    
    while (TRUE)
        {
        btfind (&application_static->application_list, &application_id, &app);
        if (app.type == AM_SERVER)
            application_id = app.parent_id;
        else
            break;
        }
    wi_set_owner_id (application_id);
    wi_set_owner_tag (app.tag);
    
    /*  Put message in alert box.                                                   */
    
    wi_alertx (0, "", "", text, application_id, app.tag);
    
    return SUCCESS;
    }

/*************************************************************************          */
/*                                                                                  */
/*NAME: AMZ_MESSAGE_DIALOG                                                          */
/*                                                                                  */
/*AUTHOR:   W. Jaeck, 30-Mar-1987                                                   */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    AMZ_MESSAGE_DIALOG is an internal AM routine which processes the              */
/*    AM_DIALOG function of the AM_MESSAGE service.                                 */
/*                                                                                  */
/*    AMZ_MESSAGE_DIALOG places the indicated message text into a                   */
/*    "Status" dialog, which is fronted if not already.                             */
/*                                                                                  */
/*    The dialog used is transparent to the calling application.                    */
/*                                                                                  */
/*************************************************************************          */

#include "am_message_dialog.rh"
static INT amz_message_dialog (text)    /*************************************************************************/

CHAR *text;
    {
#ifdef USE_X
    CHAR *tempstr;
    
    if (wiv.toplevel IS_NOT_EQUAL_TO (Widget)0)
      {
	if (IS_EMPTY_STRING (text) == FALSE)
	  {
	    for (tempstr = strchr (text, '\n'); tempstr != (CHAR *)0;
		 * tempstr = ' ', tempstr = strchr (text, '\n'))
	      ;
	    for (tempstr = strchr (text, '\r'); tempstr != (CHAR *)0;
		 * tempstr = ' ', tempstr = strchr (text, '\r'))
	      ;
	    XChangeProperty (wiv.current_Display, XtWindow (wiv.toplevel), XA_WM_NAME,
			     XA_STRING, 8, PropModeReplace, text, strlen (text));
	  }
	else
	  {
	    XChangeProperty (wiv.current_Display, XtWindow (wiv.toplevel), XA_WM_NAME,
			     XA_STRING, 8, PropModeReplace, "Finder", strlen ("Finder"));
	  }
	wiz_XFlush (wiv.current_Display);
      }
    return(SUCCESS);

#else
    INT status;                     /* completion status                            */
    APPLICATION_LIST app;           /* application_list node for current app        */
    INT app_id;                     /* current application id                       */
    INT status_dialog_id;           /* status dialog id for current app             */
    APPLICATION_STATIC *application_static = am_static ();
    
    /*  Assume success.                                                             */
    
    status = SUCCESS;
    
    /*  Get current application id.                                                 */
    
    app_id = application_static->application_id;
    
    /*  If current application is not active                                        */
    /*    (e.g. it quit, or something) then indicate                                */
    /*    that application quit, and no status                                      */
    /*    message can be displayed.                                                 */
    
    if (btfind (&application_static->application_list, &app_id, &app))
        {
        
        /*  Application is active. If no status                                     */
        /*    dialog has been loaded for this                                       */
        /*    application ...                                                       */
        
        if (app.status_dialog_id == 0)
            {
            
            /*  Load one.                                                           */
            
            status = am_define_resource (AM_DIALOG, "AM_MESSAGE_DIALOG", 1,
                                         NULL_FUNCTION_PTR, &status_dialog_id, NULL);
            
            /*  If it loaded ok ...                                                 */
            
            if (status == SUCCESS)
                {
                
                /*  Relocate node in application tree. This                         */
                /*    must be done because of the intervening                       */
                /*    am calls which may have changed currency                      */
                /*    in the application_list tree.                                 */
                
                btfind (&application_static->application_list, &app_id, &app);
                
                /*  Record id of status dialog.                                     */
                
                app.status_dialog_id = status_dialog_id;
                btrepcur (&application_static->application_list, &app_id, &app);
                }
            }
        }
    /*  Indicate application quit if no node                                        */
    /*    in application list.                                                      */
    
    else
        status = AM_ERR_APPLICATION_QUIT;
    
    /*  If everything is ok so far.                                                 */
    
    if (status >= SUCCESS)
        {
        
        /*  If caller specified NULL message, then                                  */
        /*    remove status dialog from screen.                                     */
        
        if ((text == NULL) || (text[0] == 0))
            status = wi_close_dialog (app.status_dialog_id);
        
        /*  If caller specified non-null text message,                              */
        /*    then write message to status dialog,                                  */
        /*    and front the status dialog.                                          */
        
        else
            {
            status = wi_set_dialog_text (app.status_dialog_id, AM_STATUS_DIALOG_TEXT,
                                         text);
            if (status >= SUCCESS)
                status = wi_open_dialog (app.status_dialog_id);
            }
        }
    return status;

#endif
    }

/************************************************************************************/
static INT amz_message_file_report (text)   /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    
    /********************************************************************************/
    
    status = SUCCESS;               /*  Assume success.                             */
    
    amz_message_journal (text);     /*  Journal message.                            */
    
    return status;
    }


/************************************************************************************/
static INT amz_message_file_status (text)   /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    INT len;
    CHAR final_text[300];           /* Constructed message                          */
    
    /********************************************************************************/
    
    status = SUCCESS;               /*  Assume success.                             */
    
    strcpy (final_text, amz_message_prefix ()); /*  Construct message prefix.       */
    
    len = sizeof(final_text) - strlen (final_text);
    if (strlen (text) > len)
        text[len - 1] = '\0';
    
    strcat (final_text, text);      /*  Append actual text message to prefix.       */
    
    amz_message_journal (final_text);   /*  Journal message.                        */
    
    return status;
    }

/************************************************************************************/
static INT amz_message_journal (text)   /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    APPLICATION_STATIC *application_static = am_static ();
    
    /********************************************************************************/
    
    /*  If there is an active journal file ...                                      */
    
    if (application_static->journal_file != NULL_FILE_PTR)
        {
        
        fputs (text, application_static->journal_file); /*  Write the text message. */
        
        fputs ("\n", application_static->journal_file); /*  And a new-line.         */
        
        /*  Flush buffer so that in case something                                  */
        /*    nasty happens, journal file will be                                   */
        /*    up to date.                                                           */
        
        fflush (application_static->journal_file);
        
        }
    return SUCCESS;
    }

/* **********************************************************************           */
/*                                                                                  */
/*    FILE:     AMZ_MESSAGE_PREFIX.CC                                               */
/*                                                                                  */
/*    AUTHOR:   W. Jaeck, 16-April-1987                                             */
/*                                                                                  */
/*    DESCRIPTION:                                                                  */
/*                                                                                  */
/*      AMZ_MESSAGE_PREFIX constructs a message prefix consisting of the            */
/*      current time, the application's tag, and id. A pointer to the               */
/*      constructed prefix is returned.                                             */
/*                                                                                  */
/*   **********************************************************************         */
#include "esi_time.h"

#ifdef primos

#define CTIME_TIME_OFFSET 10

#else

#define CTIME_TIME_OFFSET 11

#endif

/* **********************************************************************           */
static CHAR *amz_message_prefix ()  /* **********************************************************************   */
    {
    static CHAR prefix[50];
    time_t bintim;                  /* system binary time                           */
    CHAR *st;                       /* system date and time ASCII                   */
    CHAR timestr[9];                /* system time ASCII                            */
    CHAR program_id[AM_TAG_SIZE + 10];  /* tag + application_id                     */
    APPLICATION_LIST app;
    INT app_id;                     /* key of data in app                           */
    INT status;                     /* completion status                            */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************         */
    
    /* Get system date/time (binary)                                                */
    
    time (&bintim);
    
    /* Convert binary date/time to ASCII                                            */
    
    st = (char *) ctime (&bintim);
    
    /* Extract time and add null                                                    */
    
    strncpy (prefix, st + CTIME_TIME_OFFSET, 9);
    prefix[8] = 0;
    
    program_id[0] = 0;
    
    if (application_static->initialized)
        {
        app_id = application_static->application_id;
        
        while (TRUE)
            if (btfind (&application_static->application_list, &app_id, &app))
                if (app.type == AM_SERVER)
                    app_id = app.parent_id;
                else
                    {
                    sprintf (program_id, " %s (%d)", app.tag, app_id);
                    strcat (prefix, program_id);
                    break;
                    }
                else
                    break;
        }
    strcat (prefix, " -- ");
    
    return prefix;
    }
/************************************************************************************/
static INT amz_message_report (text)    /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    
    /********************************************************************************/
    
    /*  Assume success.                                                             */
    
    status = SUCCESS;
    
    /*  Write message to message window.                                            */
    
    wi_message (text);
    
    /*  Journal message.                                                            */
    
    amz_message_journal (text);
    
    return status;
    }

/************************************************************************************/
static INT amz_message_status (text)    /*************************************************************************/

CHAR *text;
    {
    INT status;                     /* Completion status                            */
    INT len;
    CHAR final_text[300];           /* Constructed message                          */
    
    /********************************************************************************/
    
    strcpy (final_text, amz_message_prefix ()); /*  Construct message prefix.       */
    
    len = sizeof(final_text) - strlen (final_text);
    if (strlen (text) > len)
        text[len - 1] = '\0';
    
    strcat (final_text, text);      /*  Append actual text message to prefix.       */
    
    wi_message (final_text);        /*  Write message to message window.            */
    
    amz_message_journal (final_text);   /*  Journal message.                        */
    
    return SUCCESS;
    }
/* END:                                                                             */
