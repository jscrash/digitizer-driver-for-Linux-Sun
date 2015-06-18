/* DEC/CMS REPLACEMENT HISTORY, Element AM_QUIT_CUR.C*/
/* *4    12-OCT-1990 23:33:37 GILLESPIE "(SPR 100) Merge Ernie deltas"*/
/*  1B2  12-OCT-1990 15:23:03 GILLESPIE "Merge Ernie Deltas"*/
/* *3    24-SEP-1990 15:46:14 GILLESPIE "(SPR 1) Beta update"*/
/*  1B1  24-SEP-1990 15:27:28 GILLESPIE "Beta Deltas"*/
/* *2    14-AUG-1990 11:40:16 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_QUIT_CUR.C*/
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

#include "esi_am_int.h"
#include "esi_er.h"
#include "esi_fi.h"
#include "esi_tc.h"
#include "esi_wi.h"

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    AM_QUIT_CURRENT closes and releases all of the application's menus and
    dialogs, releases all of it's workspaces, and releases all of the
    allocated data memory.  AM_QUIT_CURRENT terminates all children servers
    associated with the application program (or application server) which has
    the application id equal to the parameter, current_app_id, which is passed
    in to AM_QUIT_CURRENT.

    AM_QUIT_CURRENT is an internal routine of the application manager.  It
    should only be called by AM_ABORT, AM_QUIT, and recursively by itself,
    AM_QUIT_CURRENT.


    Application rundown is done in the following order.

         1.   If children servers are running, AM_QUIT_CURRENT recursively
              calls itself to terminate the child server until no more children
              are found.
         2.   Resources (menus and custom) dialogs are released.
         3.   Data memory is freed.
         4.   Workspaces are released.
        
Prototype:
    publicdef INT am_quit_current(INT current_app_id, BOOL call_completion_routine);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) current_app_id  -(INT) application id number which is to be terminated.
    (I) call_completion_routine -(BOOL) if the call_completion_routine is TRUE and
                    the routine whose application id is equal to current_app_id
                    is an application server, then the parent's server completion
                    routine is called.  If the call_completion_routine is FALSE or
                    if the routine whose application id is equal to current_app_id
                    is not an application server, then no server completion routine
                    is called.

Return Value/Status:
    SUCCESS - Successful quit.
    FAIL - if the current_app_id passed does not exist in the application list
        of running applications.
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_quit_current (INT current_app_id, BOOL call_completion_routine) 
#else
publicdef INT am_quit_current (current_app_id, call_completion_routine)
INT current_app_id;
BOOL call_completion_routine;
#endif
    {
    INT status;                     /* service completion status */
    INT i;                          /* counter */
    INT count;                      /* counter */
    BOOL found;                     /* TRUE if entry found, else FALSE */
    CHAR text[80];                  /* Messages */
    APPLICATION_LIST app;           /* application node storage */
    APPLICATION_LIST parent_app;    /* parent application node storage */
    RESOURCE_LIST rsrc;             /* resource node storage */
    WORKSPACE_LIST work;            /* workspace node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    INT app_id;                     /* key of data in app */
    INT rsrc_id;                    /* key of data in rsrc */
    INT front_id;                   /* id of the front dialog after quitting */
    
    typedef DATA_LIST *DATA_LIST_PTR;
    typedef BYTE *BYTE_PTR;
    
    /*-------------------------------------------*/
    /* number of entries in data_list table      */
    /*-------------------------------------------*/
    UINT nkeys;
    
    /* ----------------------------------------*/
    /* pointer to the buffer of keys returned  */
    /* by call to ht_get_all                   */
    /*-----------------------------------------*/
    BYTE_PTR key_buff;
    
    /*-------------------------------------*/
    /* pointer to the buffer of data items */
    /* returned by call to ht_get_all      */
    /*-------------------------------------*/
    BYTE_PTR data_buff;
    
    /*--------------------------------------*/
    /* pointer to a single data item in the */
    /* buffer of data items                 */
    /*--------------------------------------*/
    DATA_LIST_PTR curr_data;
    
    /*---------------------------------------*/
    /* pointer to a single key in the buffer */
    /* of keys                               */
    /*---------------------------------------*/
    BYTE_PTR *curr_key;
    
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /*   Make sure current application exists */
    
    if (btfind (&application_static->application_list, (VOIDPTR) & current_app_id,
                (VOIDPTR) & app)EQUALS FALSE)
        {
        status = FAIL;
        }
    /*   If everything is OK ... */
    
    if (status EQUALS SUCCESS)
        {
        /*   If this application has a server
             then first quit it (can be recursive) */
        found = TRUE;
        while (found EQUALS TRUE)
            {
            found = FALSE;
            count = btcount (&application_static->application_list);
            for (i = 1; count > 0 AND i <= count; i++)
                {
                if (i EQUALS 1)
                    btfirst (&application_static->application_list, (VOIDPTR) & app_id,
                             (VOIDPTR) & app);
                else
                    btnext (&application_static->application_list, (VOIDPTR) & app_id,
                            (VOIDPTR) & app);
                
                if ((app.parent_id EQUALS current_app_id)AND (app.
                                                              type EQUALS AM_SERVER))
                    {
                    /*  Recursively call am_quit_current to
                        quit child server, but don't call
                        completion routine */
                    
                    am_quit_current (app_id, FALSE);
                    found = TRUE;
                    break;
                    }
                }
            }
        /*   Set application_id to quit current
             application */
        
        application_static->application_id = current_app_id;
        
        /*   Release all of the current application's
             resources: menus and dialogs. */
        
        found = TRUE;
        while (found EQUALS TRUE)
            {
            found = FALSE;
            count = btcount (&application_static->resource_list);
            for (i = 1; count > 0 AND i <= count; i++)
                {
                if (i EQUALS 1)
                    btfirst (&application_static->resource_list, (VOIDPTR) & rsrc_id,
                             (VOIDPTR) & rsrc);
                else
                    btnext (&application_static->resource_list, (VOIDPTR) & rsrc_id,
                            (VOIDPTR) & rsrc);
                
                if (rsrc.application_id EQUALS current_app_id)
                    {
                    am_release_resource (rsrc_id);
                    found = TRUE;
                    break;
                    }
                }
            }
        /*   Release all of the current application's
             allocated data. */
        
        /*   first retrieve a list of all the entries
             in the DATA_LIST hash table    */
        
        status = ht_get_all (application_static->data_list, &nkeys,
                             (VOIDPTR) & key_buff, (VOIDPTR) & data_buff);
        
        if (status EQUALS SUCCESS)
            {
            curr_data = (DATA_LIST_PTR)data_buff;
            curr_key = (BYTE_PTR *)key_buff;
            
            /*   traverse the list, find entries that
             belong to this application and delete
             them from the hash table         */
            
            for (i = 0; i < nkeys; i++)
                {
                if (curr_data->application_id EQUALS current_app_id)
                    {
                    status = ht_delete (application_static->data_list,
                                        (VOIDPTR)curr_key);
                    if (status IS_NOT_EQUAL_TO SUCCESS)
                        {
                        sprintf (text, "AM_QUIT_CURRENT:%s:%d",
                                 "ht_delete returned bad status", status);
                        er_log (text);
                        }
                    }
                curr_key++;
                curr_data++;
                
                }
            /*   free the key and data buffers   */
            tc_free ((VOIDPTR)data_buff);
            tc_free ((VOIDPTR)key_buff);
            }
        /*   Release all of the current application's
             workspaces. */
        status = SUCCESS;
        found = TRUE;
        while (found EQUALS TRUE)
            {
            found = FALSE;
            count = llcount (&application_static->workspace_list);
            if (count > 0)
                {
                for (i = 1; i <= count; i++)
                    {
                    if (i EQUALS 1)
                        llfirst (&application_static->workspace_list,
                                 (VOIDPTR) & work);
                    else
                        llnext (&application_static->workspace_list, (VOIDPTR) & work);
                    
                    if (work.application_id EQUALS current_app_id)
                        {
                        am_release_workspace (work.name, AM_APPLICATION);
                        found = TRUE;
                        break;
                        }
                    }
                }
            }
        /*   Get current application's
             application data. */
        
        btfind (&application_static->application_list, (VOIDPTR) & current_app_id,
                (VOIDPTR) & app);
        
        /*   Process accounting. */
        
        if (app.accounting)
            {
            app.cpu_time += ho_cpu_time () - app.start_cpu_time;
            app.io_time += ho_io_time () - app.start_io_time;
            
            /*   If application, print statistics. */
            
            if (app.type EQUALS AM_APPLICATION)
                {
                sprintf (text, "Quit. CPU=%1.3f, IO=%1.3f", app.cpu_time, app.io_time);
                am_message (AM_STATUS, text);
                }
            /*   If server, add server stats to parent */
            
            else
                {
                if ((btfind (&application_static->application_list,
                             (VOIDPTR) & app.parent_id,
                             (VOIDPTR) & parent_app))AND (parent_app.accounting))
                    {
                    parent_app.cpu_time += app.cpu_time;
                    parent_app.io_time += app.io_time;
                    btrepcur (&application_static->application_list,
                              (VOIDPTR) & app.parent_id, (VOIDPTR) & parent_app);
                    }
                }
            }
        /* Enable the Finder menu item that started this application */
        
        fi_enable_menu (app.entry_point);
        
        /*   Remove current application's
             data because it is history. */
        
        btdel (&application_static->application_list, (VOIDPTR) & current_app_id);
        
        /*   If current application is an application
             server and call_completion_routine flag
             is TRUE, then call parent's server
             completion routine. */
        
        if ((app.type EQUALS AM_SERVER)AND (call_completion_routine EQUALS TRUE))
            {
            application_static->application_id = app.parent_id;
            wi_pop_owner (application_static->application_id);
            if (app.server IS_NOT_EQUAL_TO NULL_FUNCTION_PTR)
                (*app.server)(app.pointer);
            }
#if 0
        /*   Indicate no current application */
        
        application_static->application_id = -1;
#endif
        /* spr to fix using global after quiting an application - find one */
        front_id = -1;
        wi_query_front_dialog (&front_id);
        if (front_id <= 0)
            application_static->application_id = -1;
        else
            application_static->application_id = front_id;
        /* end spr. C.P. */
        }
    return status;
    }
/* END:     */
