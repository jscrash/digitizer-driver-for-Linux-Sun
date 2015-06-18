/* DEC/CMS REPLACEMENT HISTORY, Element AM_REL_RS.C*/
/* *2    14-AUG-1990 11:40:43 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_REL_RS.C*/
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
#include "esi_wi.h"
#include "esi_am_err.h"

/* Function Description -----------------------------------------------------
Description:
    AM_RELEASE_RESOURCE logically releases a menu or custom dialog which
    was defined and allocated for the calling application by the
    AM_DEFINE_RESOURCE service.

Prototype:
    publicdef INT am_release_resource(INT id);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) id of resource to be released.

Return Value/Status:
    SUCCESS - Successful release of a resource.
    AM_ERR_APPLICATION_QUIT - Application resources have been previously
        released.
    AM_ERR_NOT_DEFINED - Resource not defined within this application.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_release_resource (INT id) 
#else
publicdef INT am_release_resource (id)
INT id;
#endif
    {
    INT status;                     /* service completion status */
    RESOURCE_LIST rsrc;             /* resource node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ********************************************************************** */
    
    /*   Assume success. */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit. */
    
    if (application_static->application_id == -1)
        status = AM_ERR_APPLICATION_QUIT;
    
    /*   If everything is OK ... */
    
    if (status == SUCCESS)
        {
        
        /*   Find the RESOURCE_LIST node for
             the requested resource. If found ... */
        
        if (btfind (&application_static->resource_list, (VOIDPTR) & id,
                    (VOIDPTR) & rsrc) &&
            (INT)(rsrc.application_id == application_static->application_id))
            {
            
            /*   Close the menu or dialog box. */
            
            if (rsrc.type == AM_DIALOG)
                wiclcd (id);
            else
                wivismn (id, 0);
            
            /*   Restore resource to default setup. */
            
            wi_clear (id);
            
            /*   Tell ESITerm it's OK to free this
                 resource if heap is needed...*/
            
            wi_change_owner_id (id, -1);
            
            /*   Indicate resource is not in use. */
            
            rsrc.application_id = 0;
            
            /*   Store update RESOURCE_LIST node data. */
            
            btrepcur (&application_static->resource_list, &id, (VOIDPTR) & rsrc);
            }
        /*   If no such resource id, indicate
             grief. */
        
        else
            status = AM_ERR_NOT_DEFINED;
        }
    return status;
    }
/* END:     */
