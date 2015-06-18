/*  DEC/CMS REPLACEMENT HISTORY, Element MP_VERB_LKUP.C*/
/*  *2    17-AUG-1990 22:10:56 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:15:21 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_VERB_LKUP.C*/
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
#include "esi_md_basis.x"   /* externals (publicdefs) needed here */

/* Function Description -----------------------------------------------------
Description:
    Routine to find the index for the supplied verb code.

Prototype:
    publicdef BOOL mp_verb_lookup(INT verb_code,INT *return_ndx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verb_code       -(INT)
    return_ndx      -(INT *)

Return Value/Status:
    Function returns TRUE/FALSE.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL mp_verb_lookup (INT verb_code, INT *return_ndx)
#else
publicdef BOOL mp_verb_lookup (verb_code, return_ndx)
INT verb_code;
INT *return_ndx;
#endif
    {
    
    PROGNAME ("MP_VERB_LOOKUP");
    INT status;
    INT i;
    
    /* ********************************************************************** */
    
    status = FALSE;
    *return_ndx = -1;
    
    for (i = 0; i < MD_MAX_OVERLAY_TYPES; i++)
        {
        if (verb_code EQUALS overlay_verbs[i].verb_code)
            {
            *return_ndx = i;
            status = TRUE;
            break;
            }
        }
    return status;
    }
/* END:     */
