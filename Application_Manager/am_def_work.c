/* DEC/CMS REPLACEMENT HISTORY, Element AM_DEF_WORK.C*/
/* *4    14-AUG-1990 23:56:01 GILLESPIE "(SPR 5644) Fix last checkin"*/
/* *3    14-AUG-1990 11:39:48 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-JUN-1989 10:39:11 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 11:59:50 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_DEF_WORK.C*/
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
#include "esi_tc.h"
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_DEFINE_WORKSPACE defines and allocates memory for a workspace which 
    does not yet exist. Both APPLICATION and GLOBAL workspaces are created
    via this service.                                                      

Prototype:
    publicdef INT am_define_workspace(CHAR *name, INT type, size_t length, 
                                  VOIDPTR pointer);
                                  
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name        -(CHAR *) name of workspace.
    (I) type        -(INT) type, APPLICATION or GLOBAL.
    (I) length      -(size_t) size, in bytes.
    (O) pointer     -(VOIDPTR *) returned pointer to workspace memory.

Return Value/Status:
    SUCCESS - Successfull workspace definition;
    AM_ERR_ALREADY_DEFINED - AM_GLOBAL workspace exists for the name specified,
            or if the AM_APPLICATION type, the named workspace exists in this
            application;
    AM_ERR_ILLEGAL_KEY - Workspace name is of incorrect length;
    AM_ERR_APPLICATION_QUIT - Am_quit has been executed for the application;
    AM_ERR_NO_MEMORY - Lacked system memory to alllocate;
    AM_ERR_ILLEGAL_TYPE - Memory type parameter is incorrect;
    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_define_workspace (CHAR *name, INT type, size_t length,
                                   VOIDPTR *pointer) 
#else
publicdef INT am_define_workspace (name, type, length, pointer)
CHAR *name;
INT type;
size_t length;
VOIDPTR *pointer;
#endif
    {
    INT application_id;             /*   application id                 */
    INT count;                      /*   count of nodes in WORKSPACE_LIST list      */
    INT i;                          /*   counter                    */
    WORKSPACE_LIST work;            /*   WORKSPACE_LIST node storage            */
    INT len;                        /*   size, in bytes, of workspace           */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* **********************************************************************   */
    
    /*     Check type parameter.            */
    
    if ((type != AM_APPLICATION) && (type != AM_GLOBAL))
        {
        return AM_ERR_ILLEGAL_TYPE;
        }
    /*     Check pointer parameters.            */
    
    if ((am_check_pointer ((VOIDPTR)name, "r")) || ((pointer != (VOIDPTR *)NULL) &&
                                                    (am_check_pointer (
                                                                     (VOIDPTR)pointer,
                                                                       "w"))))
        {
        return AM_ERR_ACCESS_VIOLATION;
        }
    /*     Make sure workspace name is legal size.  */
    
    if (((len = strlen (name)) == 0) || (len > AM_WORKSPACE_KEY_SIZE))
        {
        return AM_ERR_ILLEGAL_KEY;
        }
    /*     If GLOBAL workspace, set owning      */
    /*     application to zero, else set to current */
    /*     application id for APPLICATION       */
    /*     workspace.                   */
    
    if (type == AM_GLOBAL)
        {
        application_id = 0;
        }
    else
        {
        /*     Make sure application didn't already     */
        /*     quit.                    */
        
        if (application_static->application_id == -1)
            {
            return AM_ERR_APPLICATION_QUIT;
            }
        else
            {
            application_id = application_static->application_id;
            }
        }
    /*     Scan WORKSPACE_LIST linked list to       */
    /*     insure that the requested workspace name */
    /*     is not already defined.          */
    
    count = llcount (&application_static->workspace_list);
    if (count > 0)
        {
        for (llfirst (&application_static->workspace_list, (VOIDPTR) & work), i = 0;
             i < count;
             i++, llnext (&application_static->workspace_list, (VOIDPTR) & work))
            {
            if ((strcmp (work.name,
                         name) == 0)AND (work.application_id == application_id))
                {
                return AM_ERR_ALREADY_DEFINED;
                }
            }
        }
    /*     Set name in WORKSPACE_LIST node.     */
    
    strcpy (work.name, name);
    
    /*     Set application id in WORKSPACE_LIST     */
    /*     node.                    */
    
    work.application_id = application_id;
    
    /*     Allocate memory for workspace and set    */
    /*     pointer in WORKSPACE_LIST node.      */
    
    work.data = tc_alloc (length);
    
    /*     If couldn't allocate memory for      */
    /*     workspace, indicate grief.           */
    
    if (work.data == (VOIDPTR)NULL)
        {
        return AM_ERR_NO_MEMORY;
        }
    work.length = length;
    tc_zeroes (work.data, length);
    
    /*     Return pointer to caller, if specified.  */
    
    if (pointer != (VOIDPTR *)NULL)
        {
        *pointer = work.data;
        }
    llinsa (&application_static->workspace_list, &work);
    
    return SUCCESS;
    }
/* END:     */
