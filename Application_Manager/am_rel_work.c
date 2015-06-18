/* DEC/CMS REPLACEMENT HISTORY, Element AM_REL_WORK.C*/
/* *3    14-AUG-1990 11:40:48 VINCE "(SPR 5644) Header Standardization"*/
/* *2     5-SEP-1989 10:11:03 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *1    19-JUN-1989 12:00:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_REL_WORK.C*/
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

#include "esi_am_int.h"
#include "esi_er.h"
#include "esi_mg.h"
#include "esi_am_err.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    AM_RELEASE_WORKSPACE undefines a workspace which was defined via the
    AM_DEFINE_WORKSPACE application manager service. The memory
    associated with the named workspace is freed.  AM_RELEASE_WORKSPACE 
    services both APPLICATION and GLOBAL workspaces.

Prototype:
    publicdef INT am_release_workspace(CHAR *name, INT type);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name        -(CHAR *) workspace name.
    (I) type        -(INT) workspace type: APPLICATION or GLOBAL.
    
Return Value/Status:
    SUCCESS - Successful release of workspace.
    AM_ERR_NOT_DEFINED - Workspace is not defined to AM for this application.
    AM_ERR_APPLICATION_QUIT - Application resources have been previously released.
    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary.
    AM_ERR_ILLEGAL_KEY - Length of workspace name is illegal.
    AM_ERR_ILLEGAL_TYPE - Incorrect workspace type was specified.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_release_workspace (CHAR *name, INT type) 
#else
publicdef INT am_release_workspace (name, type)
CHAR *name;
INT type;
#endif
    {
    INT status;                     /* service completion status */
    INT application_id;             /* application id */
    INT count;                      /* count of defined workspaces */
    INT i;                          /* counter */
    INT len;                        /* length of passed workspace name */
    LISTHEAD *pList;                /* list head for application workspaces */
    WORKSPACE_LIST work;            /* workspace node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    /*   Assume success. */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit. */
    
    if (application_static->application_id == -1)
        status = AM_ERR_APPLICATION_QUIT;
    
    /*   Check pointer parameters. */
    
    if (am_check_pointer ((VOIDPTR)name, "r"))
        status = AM_ERR_ACCESS_VIOLATION;
    
    /*   If workspace name is legal size ... */
    
    if (status == SUCCESS)
        if ((name == NULL)OR ((len = strlen (name)) == 0)OR (len >
                                                             AM_WORKSPACE_KEY_SIZE))
            status = AM_ERR_ILLEGAL_KEY;
        
        /*   Check for illegal type. */
        
        if ((type != AM_APPLICATION) && (type != AM_GLOBAL))
            status = AM_ERR_ILLEGAL_TYPE;
        
        /*   If everything is OK ... */
        
        if (status == SUCCESS)
            {
            /*   if GLOBAL workspace, use zero as the
                 application id, else use current
                 application id. */
            
            if (type == AM_GLOBAL)
                application_id = 0;
            else
                application_id = application_static->application_id;
            
            /*   Assume it can't be found. */
            
            status = AM_ERR_NOT_DEFINED;
            
            /*   Find it. */
            
            pList = &application_static->workspace_list;
            count = llcount (pList);
            if (count > 0)
                {
                for (i = 0, llfirst (pList, (VOIDPTR) & work); i < count;
                     i++, llnext (pList, (VOIDPTR) & work))
                    {
                    
                    /*   If this is the workspace we are
                         looking for ... */
                    
                    if ((strcmp (work.name,
                                name) == 0)AND (work.application_id == application_id))
                        {
                        
                        /*   Free the workspace memory. */
                        
                        status = tc_free (work.data);
                        
                        /*   Report any memory offenders */
                        
                        if (status != SUCCESS)
                            am_message (AM_STATUS, mg_message (status));
                        
                        /*   Delete workspace entry from
                             WORKSPACE_LIST. */
                        
                        lldela (pList);
                        
                        /*   Indicate success. */
                        
                        status = SUCCESS;
                        
                        /*   Stop looking. */
                        
                        break;
                        }
                    }
                }
            }
    return status;
    }
/* END:     */
