/* DEC/CMS REPLACEMENT HISTORY, Element AM_DEF_RES.C*/
/* *8    23-OCT-1990 13:51:25 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *7    23-AUG-1990 15:28:41 JULIAN "(SPR 0) fix compiling error"*/
/* *6    27-JUN-1990 17:24:40 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-MAR-1990 00:01:05 GILLESPIE "(SPR 1) Change to lower case before attempting to open - better for Unix"*/
/* *4     2-MAR-1990 11:12:29 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *3     9-FEB-1990 16:40:44 JULIAN "(SPR -1) Repositioning USE_X code (kee)"*/
/* *2     6-FEB-1990 00:14:47 JULIAN "(SPR -1) X Version"*/
/* *1    19-JUN-1989 11:59:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_DEF_RES.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_c_lib.h"

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_RS_H
#include "esi_rs.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_AM_ERR_H
#include "esi_am_err.h"
#endif

#define APP_MAN_MODULE 3

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    AM_DEFINE_RESOURCE reads and defines a resource (menu or custom               */
/*    dialog box) for an application program, or allocates one which was            */
/*    previously read and defined to the application if the specified               */
/*    resource already resides in the MAC but is not currently in use.              */
/*                                                                                  */
/*    From the application program point of view, AM_DEFINE_RESOURCE                */
/*    replaces the Workstation Interface (WI) services:                             */
/*                                                                                  */
/*         -    wirdfmn                                                             */
/*         -    wirdfcd                                                             */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT am_define_resource (INT type, FILENAME file, INT resource_id,   */
/*                                  INT (*server)(), INT *id, VOIDPTR pointer);     */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    (I) type        -(INT) resource type: MENU or DIALOG.                         */
/*    (I) file        -(FILENAME) resource (.RSRC) file name.                       */
/*    (I) resource_id -(INT) resource id in file.                                   */
/*    (I) server      -(INT) resource event server address.                         */
/*    (O) id          -(INT *) returned MAC id.                                     */
/*    (I) pointer     -(VOIDPTR) passed pointer.                                    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successfully deifnes resource;                                      */
/*    AM_ERR_ILLEGAL_KEY - Filename is of incorrect length;                         */
/*    AM_ERR_APPLICATION_QUIT - Am_quit has been executed for the application;      */
/*    AM_ERR_ILLEGAL_TYPE - Resource type not acceptable;                           */
/*    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary;       */
/*    FAIL - Can't define resource (see above specific errors).                     */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT am_define_resource (INT type, FILENAME file, INT resource_id,
                                  INT (*server)(), INT *id, VOIDPTR pointer)
#else
publicdef INT am_define_resource (type, file, resource_id, server, id, pointer)
INT type;                           /* resource type: MENU or DIALOG                */
FILENAME file;                      /* resource (.RSRC) file name                   */
INT resource_id;                    /* resource id in file                          */
INT (*server)();                    /* resource event server address                */
INT *id;                            /* returned MAC id                              */
VOIDPTR pointer;                    /* passed pointer                               */
#endif
    {
    INT status;                     /* service completion status                    */
    INT rsstatus;                   /* status from resource manager (RS)            */
    INT junk;                       /* scratch                                      */
    INT application_id;             /* application id                               */
    INT count;                      /* count of nodes in B-tree                     */
    INT rsrc_id;                    /* RESOURCE_LIST key (id)                       */
    INT i;                          /* counter                                      */
    INT numitems;                   /* count of items loaded                        */
    CHAR filename[AM_RESOURCE_KEY_SIZE];    /* work filename                        */
    size_t len;                     /* length of specified filename                 */
    RESOURCE_LIST rsrc;             /* resource node storage                        */
    APPLICATION_LIST app;           /* application node storage                     */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ***********************************************************************      */
    /*   Assume success.                                                            */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already                                       */
    /*     quit.                                                                    */
    
    if (application_static->application_id EQUALS - 1)
        {
        status = AM_ERR_APPLICATION_QUIT;
        }
    /*   Check pointer parameters.                                                  */
    
    else if (am_check_pointer ((VOIDPTR)file,
                               "r") OR am_check_pointer ((VOIDPTR)id,
                                                         "w") OR (server !=
                                                                  NULL_FUNCTION_PTR AND 
                                                                  am_check_pointer (
                                                                              (VOIDPTR)server
                                                                                    , 
                                                                                   "r"))
             )
        {
        status = AM_ERR_ACCESS_VIOLATION;
        }
    /*   Make sure specified file name is                                           */
    /*     legal size.                                                              */
    
    if (status EQUALS SUCCESS AND ((len =
                              strlen (file)) EQUALS 0) OR (len > AM_RESOURCE_KEY_SIZE))
        {
        status = AM_ERR_ILLEGAL_KEY;
        }
    /*   Make sure a legal type was                                                 */
    /*   specified.                                                                 */
    
    if ((type != AM_MENU) AND (type != AM_DIALOG))
        {
        status = AM_ERR_ILLEGAL_TYPE;
        }
    /*   If everything is OK ...                                                    */
    
    if (status EQUALS SUCCESS)
        {
        /*   Convert to lower case for comparisons                                  */
        /*     and storage                                                          */
        
        strcpy (filename, file);
        ts_sto_lower (filename);
        
        /*   Save caller's application id.                                          */
        
        application_id = application_static->application_id;
        
        /*   If caller is a server, use                                             */
        /*     application id and tag of parent for                                 */
        /*     MAC's sake so that, to the user, it                                  */
        /*     all looks like the same application.                                 */
        
        FOREVER
            {
            btfind (&application_static->application_list, (VOIDPTR)&application_id,
                    (VOIDPTR)&app);
            if (app.type EQUALS AM_SERVER)
                {
                application_id = app.parent_id;
                }
            else
                {
                break;
                }
            }
        /*   Assume that resource isn't already                                     */
        /*     loaded and available in MAC.                                         */
        
        status = FAIL;
        
        /*   Look in RESOURCE_LIST for an                                           */
        /*     available resource which is the same                                 */
        /*     as what the caller wants. Succeed if                                 */
        /*     found.                                                               */
        
        count = btcount (&application_static->resource_list);
        if (count > 0)
            {
            for (btfirst (&application_static->resource_list, &rsrc_id, &rsrc), i = 0;
                 i < count;
                 btnext (&application_static->resource_list, &rsrc_id, &rsrc), i++)
                {
                if ((resource_id EQUALS rsrc.resource_id) AND (rsrc.
                                                               application_id EQUALS 0) 
                    AND (type EQUALS rsrc.type) AND ARE_SAME (filename, rsrc.file))
                    {
                    status = SUCCESS;
                    break;
                    }
                }
            }
        /*   If the resource that the caller                                        */
        /*     wants is already loaded in the MAC,                                  */
        /*     then set the id for the caller, and                                  */
        /*     change the owner and tag for the                                     */
        /*     resource.                                                            */
        
        if (status EQUALS SUCCESS)
            {
            *id = rsrc_id;
            status = wi_change_owner_id (rsrc_id, application_id);
            
            if (status EQUALS SUCCESS)
                {
                wi_change_owner_tag (rsrc_id, app.tag);
                }
            else
                {
                btdel (&application_static->resource_list, &rsrc_id);
                }
            }
        /*   If the resource that the caller                                        */
        /*     wants isn't already loaded in the                                    */
        /*     MAC ...                                                              */
        
        if (status != SUCCESS)
            {
            
            /*   Assume it won't fit.                                               */
            
            status = WI_MAC_OUT_ROOM;
            
            /*   Set application id and tag for (to                                 */
            /*     be) loaded resource file.                                        */
            
            wi_set_owner_id (application_id);
            wi_set_owner_tag (app.tag);
            
            /*   Load the resource.                                                 */
            
            rsmdopen (APP_MAN_MODULE);
            rspush (APP_MAN_MODULE);
            rsstatus = rsopen (filename);
            if (rsstatus)
                {
                rspop (&junk);
                rsmdclos (APP_MAN_MODULE);
                status = rsstatus;
                }
            switch (type)
                {
            case AM_MENU:
                status = wirdfmn (resource_id, " ", &numitems, id);
                break;
            case AM_DIALOG:
                status = wirdfcd (resource_id, " ", &numitems, id);
                break;
                }
            rsclose (filename);
            rspop (&junk);
            rsmdclos (APP_MAN_MODULE);
            
            /*   If resource was successfully loaded                                */
            /*     into the MAC, add corresponding node                             */
            /*     and information to RESOURCE_LIST.                                */
            
            if (status EQUALS SUCCESS)
                {
                rsrc.type = type;
                strcpy (rsrc.file, filename);
                rsrc.resource_id = resource_id;
                btdel (&application_static->resource_list, id);
                btadd (&application_static->resource_list, id, &rsrc);
                }
            }
        /*   If resource was successfully loaded                                    */
        /*     or allocated ...                                                     */
        
        if (status EQUALS SUCCESS)
            {

#ifdef USE_X
            E_CHECK_WIDGET (*id, 0);
            (E_DIALOG_MENU (*id))->server = server;
            (E_DIALOG_MENU (*id))->pointer = pointer;
            (E_DIALOG_MENU (*id))->app_id = application_id;
            if (type == AM_MENU)
                {
                wiz_ins_menu_syn_list (application_id, *id);
                }
#endif
            
            /*   Set MAC id in RESOURCE_LIST node.                                  */
            
            rsrc_id = *id;
            
            /*   Set application id in RESOURCE_LIST                                */
            /*     node.                                                            */
            
            rsrc.application_id = application_static->application_id;
            
            /*   Set server address in RESOURCE_LIST                                */
            /*     node.                                                            */
            
            rsrc.server = server;
            
            /*   Set passed pointer in RESOURCE_LIST                                */
            /*     node.                                                            */
            
            rsrc.pointer = pointer;
            
            /*   Set the language type to C                                         */
            
            rsrc.language = 0;
            
            /*   Replace RESOURCE_LIST data.                                        */
            
            btrepcur (&application_static->resource_list, (VOIDPTR)id, (VOIDPTR)&rsrc);
            }
        }
    return status;
    }
/* END:                                                                             */
