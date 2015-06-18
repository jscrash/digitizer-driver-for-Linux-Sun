/* DEC/CMS REPLACEMENT HISTORY, Element AM_SET_SVR_TP.C */
/* *3    14-AUG-1990 11:40:53 VINCE "(SPR 5644) Header Standardization" */
/* *2    21-SEP-1989 09:43:47 GILLESPIE "(SPR 1) Removed in error" */
/* *1    21-SEP-1989 09:42:00 GILLESPIE "a" */
/* DEC/CMS REPLACEMENT HISTORY, Element AM_SET_SVR_TP.C */
/* DEC/CMS REPLACEMENT HISTORY, Element AM_SET_SVR_TP.C*/
/* *1    19-JUN-1989 12:00:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_SET_SVR_TP.C*/
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
    Sets the language type of resource server functions:
    0 = C --> pass arguments by value;
    1 = FORTRAN --> pass arguments by reference.
    
Prototype:
    publicdef INT am_set_server_type(INT id, INT language);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) resource ID, as returned from am_define_resource.
    (I) language    -(INT) code representing language / argument passing mechanism.

Return Value/Status:
    SUCCESS - Successful server language set.
    AM_ERR_ILLEGAL_TYPE - Language unrecognizable.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_set_server_type (INT id, INT language) 
#else
publicdef INT am_set_server_type (id, language)
INT id;
INT language;
#endif
    {
    INT status;                     /* service completion status */
    RESOURCE_LIST rsrc;             /* resource node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* *********************************************************************** */
    
    /*   Find RESOURCE_LIST node for menu
         or custom dialog */
    
    status = btfind (&application_static->resource_list, (VOIDPTR) & id,
                     (VOIDPTR) & rsrc);
    
    /*   If RESOURCE_LIST node was found ... */
    
    if (status)
        {
        rsrc.language = language;
        btrepcur (&application_static->resource_list, (VOIDPTR) & id,
                  (VOIDPTR) & rsrc);
        status = SUCCESS;
        }
    /*   If RESOURCE_LIST node was not found,
         indicate that the event is
         unrecognizable. */
    else
        {
        status = AM_ERR_ILLEGAL_TYPE;
        }
    return status;
    }
/* END:     */
