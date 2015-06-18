/* DEC/CMS REPLACEMENT HISTORY, Element AM_DECLARE.C*/
/* *2    14-AUG-1990 11:39:44 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 11:59:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_DECLARE.C*/
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
    AM_DECLARE enables the application program to set the dialog box
    title tag for all subsequently defined dialog box resources. This
    tag need be set only once during the life of an instance of an
    application.

Prototype:
    INT am_declare(CHAR *tag);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) tag         -(CHAR *) dialog box title tag.

Return Value/Status:
    SUCCESS - If tag is correct and routine completed sucessfully;
    AM_ERR_APPLICATION_QUIT - If AM_QUIT has been executed for the application;
    AM_ERR_ACCESS_VIOLATION - Pointer parameter out of memory boundary;
    AM_ERR_ILLEGAL_KEY - Cd tag exceeds 20 characters or is NUL;
    AM_ERR_DUPLICATE_TAG - Tag is already defined for an application.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    From the application program point of view, the AM_DECLARE service
    replaces the Workstation Interface (WI) services "WI_SET_OWNER_TAG"
    and "WI_CHANGE_OWNER_TAG".

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
INT am_declare (CHAR *tag) 
#else
INT am_declare (tag)
CHAR *tag;
#endif
    {
    INT status;                     /* service completion status */
    APPLICATION_LIST app;           /* application node storage */
    INT appid;                      /* application id storage(scratch) */
    INT count;                      /* number of active applications  */
    INT i;                          /* iteration control  */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* ************************************************************************ */
    
    /*   Assume success. */
    
    status = SUCCESS;
    
    /*   Make sure application didn't already
         quit. */
    
    if (application_static->application_id == -1)
        status = AM_ERR_APPLICATION_QUIT;
    
    /*   Check pointer. */
    
    if (am_check_pointer ((VOIDPTR)tag, "r"))
        status = AM_ERR_ACCESS_VIOLATION;
    
    /*   Make sure tag size is legal. */
    
    if (status == SUCCESS)
        if (strlen (tag) > AM_TAG_SIZE)
            status = AM_ERR_ILLEGAL_KEY;
        
        /*   If everything was OK ... */
        
        if (status == SUCCESS)
            {
            /*   Check for a duplicate tag  */
            
            count = btcount (&application_static->application_list);
            for (btfirst (&application_static->application_list, (VOIDPTR) & appid,
                          (VOIDPTR) & app), i = 0; i < count;
                 i++, btnext (&application_static->application_list, (VOIDPTR) & appid,
                              (VOIDPTR) & app))
                if (strcmp (app.tag, tag) == 0)
                    {
                    status = AM_ERR_DUPLICATE_TAG;
                    break;
                    }
            }
    /*  If not...    */
    
    if (status == SUCCESS)
        {
        
        /*   Get this application's application
             info. */
        
        btfind (&application_static->application_list,
                (VOIDPTR) & application_static->application_id, (VOIDPTR) & app);
        
        /*   Set the specified tag. */
        
        strcpy (app.tag, tag);
        
        /*   Replace application's info. */
        
        btrepcur (&application_static->application_list,
                  (VOIDPTR) & application_static->application_id, (VOIDPTR) & app);
        }
    return status;
    }
/* END:     */
