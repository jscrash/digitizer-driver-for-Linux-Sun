/* DEC/CMS REPLACEMENT HISTORY, Element AM_INQ_RSRC.C*/
/* *2    14-AUG-1990 11:40:05 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 12:00:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element AM_INQ_RSRC.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Find RESOURCE_LIST node for menu or custom dialog from which event
    was generated, getting the resource filename and resource id number. 

Prototype:
    publicdef INT am_inq_rsrc(INT id, INT item, FILENAME return_file, 
                          INT *return_id);
                          
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) id          -(INT) menu or custom dialog id.
    (I) item        -(INT) item on menu or custom dialog.
    (O) return_file -(FILENAME) resource filename.
    (O) return_id   -(INT *) resource id number.

Return Value/Status:
    SUCCESS - Successful retrieval of RESOURCE_LIST;
    Returns the value of the resource filename and resource id number.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT am_inq_rsrc (INT id, INT item, FILENAME return_file, INT *return_id) 
#else
publicdef INT am_inq_rsrc (id, item, return_file, return_id)
INT id;
INT item;
FILENAME return_file;
INT *return_id;
#endif
    {
    INT status;                     /* service completion status */
    RESOURCE_LIST rsrc;             /* resource node storage */
    APPLICATION_STATIC *application_static = am_static ();
    
    /* *********************************************************************** */
    
    /*   Find RESOURCE_LIST node for menu
         or custom dialog from which event
         was generated. */
    
    status = btfind (&application_static->resource_list, (VOIDPTR) & id,
                     (VOIDPTR) & rsrc);
    strcpy (return_file, rsrc.file);
    *return_id = rsrc.resource_id;
    
    return status;
    }
/* END:     */
