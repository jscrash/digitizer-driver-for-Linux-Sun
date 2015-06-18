/*  DEC/CMS REPLACEMENT HISTORY, Element MP_VALID_CHK.C*/
/*  *4    30-OCT-1990 12:05:16 GILLESPIE "(SPR 37) Fix up Sun compiles"*/
/*  *3    17-AUG-1990 22:10:52 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 11:01:41 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:15:17 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_VALID_CHK.C*/
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

#include    "esi_gl_defs.h"
#include    "esi_am.h"
#include    "esi_mp.h"

/* Function Description -----------------------------------------------------
Description:
    Check that all overlays have their required keywords.

Prototype:
    publicdef INT mp_valid_check(BOOL full);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    full            -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_valid_check (BOOL full)
#else
publicdef INT mp_valid_check (full)
BOOL full;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_STATE_WORKSPACE *md_state;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT req_ndx;
    INT temp_ndx;
    INT i;
    INT start, end;
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&mapdef);
    am_open_workspace ("MAPDEF_STATE", AM_GLOBAL, (VOIDPTR *)&md_state);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    if (full)
        {
        start = 0;
        end = display->last_row_used;
        }
    else
        {
        start = md_state->crnt_pref_verb_start - 1;
        end = md_state->crnt_pref_verb_start;
        }
    temp_ndx = -1;
    for (i = start; i < end; i++)
        {
        if (display->verb_ndx[i] != temp_ndx)
            {
            temp_ndx = display->verb_ndx[i];
            
            if (temp_ndx != -1 AND mp_req_chk (i + 1, &req_ndx) != SUCCESS)
                {
                md_state->active_verb_ndx = temp_ndx;
                md_state->crnt_pref_verb_start = i + 1;
                md_state->crnt_pref_verb_end = md_state->crnt_pref_verb_start;
                while (temp_ndx EQUALS display->verb_ndx[md_state->crnt_pref_verb_end])
                    {
                    (md_state->crnt_pref_verb_end)++;
                    }
                mp_req_key_setup (req_ndx);
                status = FAIL;
                goto finished;
                }
            }
        }
    if (full)
        mapdef->valid = TRUE;
    
finished:
    return status;
    }
/* END:     */
