/*  DEC/CMS REPLACEMENT HISTORY, Element MP_REQ_CHK.C*/
/*  *3    17-AUG-1990 22:08:50 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:59:50 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:14:31 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_REQ_CHK.C*/
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
#include "esi_mp.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Function to check that all required keywords have been
    used.

Prototype:
    publicdef INT mp_req_chk(INT display_line,INT *req_ndx);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT ) Line number in display struct.
    req_ndx         -(INT *) Returned value of required index.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_req_chk (INT display_line, INT *req_ndx)
#else
publicdef INT mp_req_chk (display_line, req_ndx)
INT display_line;
INT *req_ndx;
#endif
    {
    
    PROGNAME ("MP_REQ_CHK");
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    verbstruc = compiled->verb_ptr[display->compiled_row[display_line - 1]];
    
    for (*req_ndx = 0; *req_ndx < verbstruc->nrows; (*req_ndx)++)
        {
        if (verbstruc->key_used[*req_ndx] EQUALS - 1)
            {
            status = FAIL;
            break;
            }
        }
    return status;
    }
/* END:     */
