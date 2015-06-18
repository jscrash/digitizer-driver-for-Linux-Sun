/* DEC/CMS REPLACEMENT HISTORY, Element OV_NEW_OBJECT.C*/
/* *2    25-JUL-1990 19:59:19 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:21:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_NEW_OBJECT.C*/
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

#include    "esi_c_lib.h"
#include    "esi_ov.h"
#include    "esi_ts.h"
#include    "esi_wm.h"

/* Function Description -----------------------------------------------------
Description:
    Allocate the window manager group for a new graphics object.
    Sets the standard window manager object attributes as follows:
                
        Class:      as specified, caller obtains from table
                    GRAPHIC_OBJECT_CODES,
        Dbkey:      "object id", as specified
        Username:   "<object name> <object id>", where object name
                    is from the table GRAPHIC_OBJECTS (same as the
                    overlay name).

Prototype:
    publicdef INT ov_new_object(INT *group_id,CHAR *object_type,INT object_class,
        INT object_id );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) object_type     -(CHAR  *)
    (I) object_class    -(INT)
    (I) object_id       -(INT)
    (O) group_id        -(INT) Allocated by WM.

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_new_object (INT *group_id, CHAR *object_type, INT object_class,
                             INT object_id)
#else
publicdef INT ov_new_object (group_id, object_type, object_class, object_id)
INT *group_id;
CHAR *object_type;
INT object_class, object_id;
#endif
    {
    CHAR std_name[41], user_name[41], db_key[41];
    
    wmgallocate (group_id);
    
    sprintf (db_key, "%d", object_id);
    
    /*  Convert object type to pretty print standard name: 
        e.g., Tobin_Rivers */
    
    strcpy (std_name, object_type);
    ts_standard_name (std_name);
    sprintf (user_name, "%s %d", std_name, object_id);
    
    wmoset (*group_id, object_class, db_key, user_name);
    
    return SUCCESS;
    }
/* END:     */
