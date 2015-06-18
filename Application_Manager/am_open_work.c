/* DEC/CMS REPLACEMENT HISTORY, Element AM_OPEN_WORK.C*/
/* *4    15-AUG-1990 00:06:23 GILLESPIE "(SPR 5644) Fix last checkin"*/
/* *3    14-AUG-1990 11:40:08 VINCE "(SPR 5644) Header Standardization"*/
/* *2    26-JUN-1989 10:39:21 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:00:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_OPEN_WORK.C*/
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
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_OPEN_WORKSPACE returns a pointer to the named workspace.
    Both APPLICATION and GLOBAL workspaces are serviced.

Prototype:
    publicdef INT am_open_workspace(CHAR *name, INT type, VOIDPTR pointer);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) name        -(CHAR *) workspace name.           
    (I) type        -(INT) type: APPLICATION or GLOBAL.
    (O) pointer     -(VOIDPTR *) returned pointer to workspace.

Return Value/Status:
    SUCCESS - Successfull opening of workspace.
    AM_ERR_ACCESS_VIOLATION - Pointer parameter is out of program boundary.
    AM_ERR_ILLEGAL_KEY - Length of workspace name is incorect.
    AM_ERR_ILLEGAL_TYPE - Workspace type is incorrect.
    AM_ERR_NOT_DEFINED - Workspace was not previously defined.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_open_workspace (CHAR *name, INT type, VOIDPTR *pointer) 
#else
publicdef INT am_open_workspace (name, type, pointer)
CHAR *name;
INT type;
VOIDPTR *pointer;
#endif
    {
    INT status;                     /*  service completion status           */
    INT application_id;             /*  application id                  */
    INT count;                      /*  count of defined workspaces         */
    INT i;                          /*  counter                     */
    INT len;                        /*  length (bytes) of workspace name        */
    WORKSPACE_LIST work;            /*  workspace node                  */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    /*    Check pointer parameters.         */
    
    if ((am_check_pointer ((VOIDPTR)name, "r")) ||
        (am_check_pointer ((VOIDPTR)pointer, "w")))
        {
        return AM_ERR_ACCESS_VIOLATION;
        }
    /*    Make sure name is legal.          */
    
    if (((len = strlen (name)) == 0) || (len > AM_WORKSPACE_KEY_SIZE))
        {
        return AM_ERR_ILLEGAL_KEY;
        }
    /*    Make sure a legal type was specified.     */
    
    if ((type != AM_APPLICATION) && (type != AM_GLOBAL))
        {
        return AM_ERR_ILLEGAL_TYPE;
        }
    /*    If GLOBAL workspace requested, then
          search for match with application_id
          zero, else look for that of current
          application id.               */
    
    application_id = ((type == AM_GLOBAL) ? 0 : application_static->application_id);
    
    /*    Assume it can't be found.         */
    
    status = AM_ERR_NOT_DEFINED;
    
    /*    Find it.                  */
    
    count = llcount (&application_static->workspace_list);
    if (count > 0)
        {
        for (llfirst (&application_static->workspace_list, (VOIDPTR) & work), i = 0;
             i < count;
             i++, llnext (&application_static->workspace_list, (VOIDPTR) & work))
            {
            
            /*    If this is the one we are looking for ... */
            
            if (work.application_id == application_id && strcmp (work.name, name) == 0)
                {
                
                /*    Indicate found as requested.          */
                
                status = SUCCESS;
                
                /*    Return pointer to workspace.          */
                
                *pointer = work.data;
                /*    And stop looking.             */
                break;
                }
            }
        }
    return status;
    }
/* END:     */
