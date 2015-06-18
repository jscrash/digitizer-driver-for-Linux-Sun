/* DEC/CMS REPLACEMENT HISTORY, Element OV_NEW_EL.C*/
/* *3    26-JUL-1990 11:56:47 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes."*/
/* *2    25-JUL-1990 19:59:14 VINCE "(SPR 5644) Header Standardization"*/
/* *1    19-JUN-1989 13:21:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_NEW_EL.C*/
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
#include    "esi_wm.h"
#include    "esi_wm_cls.h"

/* Function Description -----------------------------------------------------
Description:
    Allocate the window manager segment for a new graphics element.
    Sets the standard window manager element attributes as follows:
                
        Class:      WM_CLASS_LINE, WM_CLASS_TEXT, ... as approp.,
        Dbkey:      "element id", as specified
        Username:   "<element_name> <element_id>" if element_name given,
                    "WM_CLSTR_<type> <element_id>" otherwise

Prototype:
    publicdef INT ov_new_element(INT *segment_id, INT element_class,
                                 CHAR *element_name, INT element_id );

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) element_class   -(INT)
    (I) element_name    -(CHAR *)
    (I) element_id      -(INT)
    (O) segment_id      -(INT *) Allocated by WM.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - If specified class not recognized.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_new_element(INT *segment_id, INT element_class,
                             CHAR *element_name, INT element_id)
#else
publicdef INT ov_new_element (segment_id, element_class,
                              element_name, element_id)
INT *segment_id, element_class, element_id;
CHAR *element_name;
#endif
    {
    CHAR std_name[41], default_name[41], user_name[41], db_key[41];
    INT status = SUCCESS;
    
    switch (element_class)
        {
    case WM_CLASS_LINE:
        strcpy (default_name, WM_CLSTR_LINE);
        break;
    case WM_CLASS_POLYGON:
        strcpy (default_name, WM_CLSTR_POLYGON);
        break;
    case WM_CLASS_TEXT:
        strcpy (default_name, WM_CLSTR_TEXT);
        break;
    case WM_CLASS_SYMBOL:
        strcpy (default_name, WM_CLSTR_SYMBOL);
        break;
    default:
        status = FAIL;
        break;
        }
    if (status == SUCCESS)
        {
        wmsallocate (segment_id);
        if (strlen (element_name))
            {
            strcpy (std_name, element_name);
            ts_standard_name (std_name);
            }
        else
            strcpy (std_name, default_name);
        sprintf (db_key, "%d", element_id);
        sprintf (user_name, "%s %d", std_name, element_id);
        
        wmoset (*segment_id, element_class, db_key, user_name);
        }
    return status;
    }
/* END:     */
