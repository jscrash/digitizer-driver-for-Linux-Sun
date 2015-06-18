/*  DEC/CMS REPLACEMENT HISTORY, Element MP_KEY_LOOKUP.C*/
/*  *2    17-AUG-1990 22:06:32 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:12:46 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_KEY_LOOKUP.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Routine to look for the supplied key_code in the verb
    defined by the verb index and to return the index for
    the keyword if it is found.

Prototype:
    publicdef BOOL mp_key_lookup(INT key_code,INT verb_ndx,INT *return_ndx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    key_code        -(INT)
    verb_ndx        -(INT)
    return_ndx      -(INT *)

Return Value/Status:
    Function returns TRUE/FALSE.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL mp_key_lookup (INT key_code, INT verb_ndx, INT *return_ndx)
#else
publicdef BOOL mp_key_lookup (key_code, verb_ndx, return_ndx)
INT key_code;
INT verb_ndx;
INT *return_ndx;
#endif
    {
    
    PROGNAME ("MP_VERB_LOOKUP");
    MD_KEY_INFO *key_info;
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = FALSE;
    *return_ndx = -1;
    
    key_info = overlay_verbs[verb_ndx].key_list;
    
    for (i = 0; i < overlay_verbs[verb_ndx].no_of_keywords; i++)
        {
        if (key_code EQUALS key_info[i].key_code)
            {
            *return_ndx = i;
            status = TRUE;
            break;
            }
        }
    return status;
    }
/* END:     */
