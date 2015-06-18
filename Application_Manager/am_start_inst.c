/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_INST.C*/
/* *3    30-OCT-1990 11:52:45 GILLESPIE "(SPR 37) Fix Sun side of code"*/
/* *2    14-AUG-1990 11:41:04 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:39 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_INST.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_INST.C                             */
/* *2    14-AUG-1990 11:41:04 VINCE "(SPR 5644) Header Standardization"             */
/* *1    19-JUN-1989 12:00:39 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element AM_START_INST.C                             */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*                    Unpublished -- All rights reserved                            */
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

#include "esi_am_int.h"
#include "esi_wi.h"
#include "esi_am_err.h"

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    AM_START_INSTANCE starts an instance of an application or an                  */
/*    application server.                                                           */
/*                                                                                  */
/*    AM_START_INSTANCE is for internal use only. The interface to                  */
/*    AM_START_INSTANCE should be through AM_START_APPLICATION or                   */
/*    AM_START_SERVER.                                                              */
/*                                                                                  */
/*    You may start an instance (Application or Appliction Server) without          */
/*    actually causing an entry point to be executed by calling                     */
/*    AM_START_INSTANCE (through AM_START_APPLICATION or AM_START_SERVER) with      */
/*    a null entry_point. Immediately after doing so, you can continue executing,   */
/*    as that instance by setting the application manager's current_application_id  */
/*    (in APPLICATION_STATIC) to next_application_id minus 1. Eventually, you       */
/*    should probably reincarnate the application which you were originally.        */
/*    That application id was originally available in current_application_id.       */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT am_start_instance(INT type, INT (*entry_point)(),               */
/*                                INT (*return_point)(), VOIDPTR pointer);          */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) type        -(INT) type: APPLICATION or SERVER.                           */
/*    (I) entry_point -(INT (*)()) entry point for server or application.           */
/*    (I) return_point-(INT (*)()) application server return entry point.           */
/*    (I) pointer     -(VOIDPTR) protocol pointer.                                  */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    AM_ERR_SERVER_ACTIVE - If caller already has a server active.                 */
/*    AM_ERR_APPLICATION_QUIT - Application has "quit" prior to call.               */
/*    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary.       */
/*    Status code of called entry_point.                                            */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*    NOTES:                                                                        */
/*                                                                                  */
/*    AM_INITIALIZE tricks AM_START_INSTANCE into thinking that an application      */
/*    already exists, so that an application node for the application manager       */
/*    itself can be created. It does this by setting the current application        */
/*    id to zero (instead of -1) just before calling AM_START_INSTANCE to           */
/*    create the application called "Application Manager". In ordinary cases,       */
/*    AM_START_INSTANCE can only be called by an active application. Certain        */
/*    processing is likely to be done to the application which called               */
/*    AM_START_INSTANCE, unless it really did not exist. This is tolerated          */
/*    because AM_START_INSTANCE actually looks to see if there is a current         */
/*    application, even if the current application id indicates that there is.      */
/*                                                                                  */
/*    AM_START_INSTANCE does not provide trace information because                  */
/*    AM_START_INSTANCE is for internal use only. Callers of AM_START_INSTANCE      */
/*    should provide applicable trace information. Therefore, if trace              */
/*    information was provided, it would appear redundant. Notice that the          */
/*    trace facilities are in place, but simply disabled.                           */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT am_start_instance (INT type, INT (*entry_point)(), INT (*return_point)(),
                                 VOIDPTR pointer)
#else
publicdef INT am_start_instance (type, entry_point, return_point, pointer)
INT type;
INT (*entry_point)();
INT (*return_point)();
VOIDPTR pointer;
#endif
    {
    INT status;                     /* service completion status                    */
    INT cur_app_id;                 /* current application id                       */
    INT new_app_id;                 /* next application id                          */
    INT app_id;                     /* key of entry in app                          */
    APPLICATION_LIST new_app;       /* new app entry                                */
    APPLICATION_LIST cur_app;       /* current app entry                            */
    APPLICATION_LIST app;           /* app entry                                    */
    INT count;                      /* count of active applications                 */
    INT i;                          /* counter                                      */
    INT cur_app_exists;             /* Indicates if current application found       */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* **********************************************************************       */
    /*   Assume success.                                                            */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already                                       */
    /*     quit.                                                                    */
    
    if (application_static->application_id EQUALS - 1)
        status = AM_ERR_APPLICATION_QUIT;
    
    /*   Check pointer parameters.                                                  */
    
    if ((entry_point != NULL_FUNCTION_PTR AND am_check_pointer ((VOIDPTR)entry_point,
                                                                "r")) 
        OR (return_point !=
            NULL_FUNCTION_PTR AND am_check_pointer ((VOIDPTR)return_point,
                                                    "r"))OR (pointer !=
                                                             (VOIDPTR) NULL AND 
                                                             am_check_pointer (
                                                                          pointer,
                                                                               "r")))
        {
        status = AM_ERR_ACCESS_VIOLATION;
        }
    /*   If everything is OK ...                                                    */
    
    if (status EQUALS SUCCESS)
        {
        
        /*   Get current application id.                                            */
        
        cur_app_id = application_static->application_id;
        
        /*   Get current application node,                                          */
        /*     if possible. There may not be one, as                                */
        /*     will certainly be the case when the                                  */
        /*     Application Manager itself is started,                               */
        /*     as an application.                                                   */
        
        cur_app_exists = btfind (&application_static->application_list,
                                 (VOIDPTR)&cur_app_id, (VOIDPTR)&cur_app);
        
        /*   If caller is trying to start a server,                                 */
        /*     make sure caller doesn't already have                                */
        /*     a server active.                                                     */
        
        if (type EQUALS AM_SERVER)
            {
            count = btcount (&application_static->application_list);
            for (btfirst (&application_static->application_list, (VOIDPTR)&app_id,
                          (VOIDPTR)&app), i = 0; count > 0 AND i < count;
                 i++, btnext (&application_static->application_list, (VOIDPTR)&app_id,
                              (VOIDPTR)&app))
                {
                
                /*   If caller already has a server active,                         */
                /*     then indicate grief.                                         */
                
                if (app.type EQUALS AM_SERVER)
                    if (app.parent_id EQUALS cur_app_id)
                        {
                        status = AM_ERR_SERVER_ACTIVE;
                        break;
                        }
                }
            }
        }
    /*   If everything is ok so far ...                                             */
    
    if (status EQUALS SUCCESS)
        {
        
        /*   Get next application id.                                               */
        
        new_app_id = application_static->next_application_id;
        
        /*   Increment next application id.                                         */
        
        application_static->next_application_id++;
        
        /*   Set new application type: APPLICATION                                  */
        /*     or SERVER.                                                           */
        
        new_app.type = type;
        
        /*   Set parent application id to caller's                                  */
        /*     application id for new instance.                                     */
        
        new_app.parent_id = cur_app_id;
        
        new_app.entry_point = entry_point;
        
        /*   Set SERVER return point, applicable                                    */
        /*     for SERVERs only.                                                    */
        
        new_app.server = return_point;
        
        /*   Set protocol pointer.                                                  */
        
        new_app.pointer = pointer;
        
        /*   Indicate no status dialog yet.                                         */
        
        new_app.status_dialog_id = 0;
        
        /*   If this is an application server, then                                 */
        /*     push parent (caller) environment on                                  */
        /*     workstation so that no interactions with                             */
        /*     it's menus or dialogs can be done while                              */
        /*     the server is active.                                                */
        
        if (type EQUALS AM_SERVER)
            {
            wi_push_owner (cur_app_id);
            }
        /*   Initialize tag.                                                        */
        
        new_app.tag[0] = 0;
        
        /*   For accounting purposes, charge current                                */
        /*     application for time to this point.                                  */
        
        if (cur_app_exists AND cur_app.accounting AND btfind (&application_static->
                                                              application_list,
                                                              &cur_app_id, &cur_app))
            {
            cur_app.cpu_time += ho_cpu_time () - cur_app.start_cpu_time;
            cur_app.io_time += ho_io_time () - cur_app.start_io_time;
            btrepcur (&application_static->application_list, (VOIDPTR)&cur_app_id,
                      (VOIDPTR)&cur_app);
            }
        /*   Initialize accounting statistics.                                      */
        
        new_app.accounting = application_static->accounting;
        new_app.cpu_time = 0.0;
        new_app.io_time = 0.0;
        if (new_app.accounting)
            {
            new_app.start_cpu_time = ho_cpu_time ();
            new_app.start_io_time = ho_io_time ();
            }
        /*   Save new application data.                                             */
        
        btadd (&application_static->application_list, (VOIDPTR)&new_app_id,
               (VOIDPTR)&new_app);
        
        /*   Set current application id to new                                      */
        /*     application id, which is that of the                                 */
        /*     server or application which is being                                 */
        /*     started.                                                             */
        
        application_static->application_id = new_app_id;
        
        /*   Call application or server entry                                       */
        /*     pointer, if specified.                                               */
        
        if (entry_point IS_NOT_EQUAL_TO NULL_FUNCTION_PTR)
            {
            status = (*entry_point)(pointer);
            }
        /*   Update accounting statistics if                                        */
        /*     accounting enabled, for application                                  */
        /*     just started.                                                        */
        
        if ((new_app.accounting) AND (btfind (&application_static->application_list,
                                              (VOIDPTR)&new_app_id,
                                              (VOIDPTR)&new_app)) AND new_app.
            accounting)
            {
            new_app.cpu_time += ho_cpu_time () - new_app.start_cpu_time;
            new_app.io_time += ho_io_time () - new_app.start_io_time;
            btrepcur (&application_static->application_list, (VOIDPTR)&new_app_id,
                      (VOIDPTR)&new_app);
            }
        /*   Restart accounting for caller.                                         */
        
        if ((cur_app_exists) AND (cur_app.
                                  accounting) AND (btfind (&application_static
                                                           ->application_list,
                                                           (VOIDPTR)&cur_app_id,
                                                           (VOIDPTR)&cur_app)))
            {
            cur_app.start_cpu_time = ho_cpu_time ();
            cur_app.start_io_time = ho_io_time ();
            btrepcur (&application_static->application_list, (VOIDPTR)&cur_app_id,
                      (VOIDPTR)&cur_app);
            }
        /*   Set current application id to that of                                  */
        /*     the caller of this service.                                          */
        
        application_static->application_id = cur_app_id;
        }
    return status;
    }
/* END:                                                                             */
