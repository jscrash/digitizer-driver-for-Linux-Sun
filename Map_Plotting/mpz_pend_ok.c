/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_PEND_OK.C*/
/*  *3    17-AUG-1990 22:03:15 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:50:50 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:09 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MPZ_PEND_OK.C*/
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
#include "esi_md_defs.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to check if a pending verb has its required
    keywords.  If not, then setup overlay dialog for that
    keyword.

Prototype:
    publicdef BOOL mpz_pending_ok();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    Function returns TRUE/FALSE.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL mpz_pending_ok ()
#else
publicdef BOOL mpz_pending_ok ()
#endif
    {
    PROGNAME ("MP_CLOSE_MAP");
    BOOL ok = TRUE;
    MD_STATE_WORKSPACE *md_state;
    INT i;
    
    /* ********************************************************************** */
    
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *) & md_state);
    
    if (md_state->verb_pending)
        {
        if (NOT (ok = (mp_req_chk (md_state->crnt_pref_verb_start, &i) EQUALS SUCCESS)))
            {
            mp_req_key_setup (i);
            }
        else if ((ok =
                       (mp_special_checks (md_state->
                                           crnt_pref_verb_start) EQUALS SUCCESS)))
            {
            md_state->verb_pending = FALSE;
            }
        }
    return ok;
    }
/* END:     */
