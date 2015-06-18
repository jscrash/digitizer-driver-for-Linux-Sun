/* DEC/CMS REPLACEMENT HISTORY, Element WM_ASPECT.C */
/* *3    14-AUG-1990 13:43:53 VINCE "(SPR 5644) Header Standardization" */
/* *2    21-DEC-1989 15:46:14 GORDON "(SPR 0) Aspect ratio lock application" */
/* *1    21-DEC-1989 15:45:18 GORDON "Aspect ratio lock application" */
/* DEC/CMS REPLACEMENT HISTORY, Element WM_ASPECT.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_gl_defs.h"
#include "esi_fi.h"
#include "esi_qs.h"
#include "esi_wm.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef wm_toggle_aspect_lock(VOIDPTR *ptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ptr             -(VOIDPTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef wm_toggle_aspect_lock (VOIDPTR *ptr) 
#else
publicdef wm_toggle_aspect_lock (ptr)
VOIDPTR *ptr;
#endif
    {
    BOOL curValue;
    INT status;
    INT menuid, itemid;
    
    am_declare ("ASPECT_LOCK");
    
    /* First, query the state variable for the current setting. */
    status = qs_inq_i (QS_ASPECT_LOCK, &curValue);
    /* Toggle the setting */
    if (curValue)
        curValue = FALSE;
    else
        curValue = TRUE;
    
    /* Update the state variable */
    qs_set_i (QS_ASPECT_LOCK, curValue);
    /* Update the menu check state */
    status = fih_query_menu_function (&menuid, &itemid, wm_toggle_aspect_lock);
    if (status == SUCCESS)
        wi_check_menu_item (menuid, itemid, curValue);
    
    am_quit ();
    return SUCCESS;
    }
/* END:     */
