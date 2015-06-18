/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CMP_SIZ_CK.C*/
/*  *3    17-AUG-1990 22:03:54 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:52:58 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:40 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CMP_SIZ_CK.C*/
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
#include "esi_gl_project.h"
#include "esi_mp.h"
#include "esi_md_defs.h"
#include "esi_am.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to check that the compiled mapdef arrays can handle
    given row number.
    The counter "rownum" starts from 0.

Prototype:
    publicdef INT mp_compiled_size_check(INT rownum);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rownum          -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_compiled_size_check (INT rownum)
#else
publicdef INT mp_compiled_size_check (rownum)
INT rownum;
#endif
    {
    
    PROGNAME ("MP_COMPILED_SIZE_CHECK");
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_COMPILED_MAPDEF *compiled;
    INT i, size;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    
    if (rownum >= mapdef->compiled->nrows)
        {
        mp_init_compiled_mapdef (&compiled, rownum);
        
        /* * TRANSFER OLD VALUES TO NEW ARRAYS * */
        /* ------------------------------------- */
        
        compiled->last_row_used = mapdef->compiled->last_row_used;
        i = compiled->last_row_used;
        size = i * sizeof(INT);
        hoblockmove (mapdef->compiled->verb_code, compiled->verb_code, size);
        hoblockmove (mapdef->compiled->overlay_wm_id, compiled->overlay_wm_id, size);
        hoblockmove (mapdef->compiled->wm_arr_ndx, compiled->wm_arr_ndx, size);
        hoblockmove (mapdef->compiled->verb_wm_id, compiled->verb_wm_id, size);
        hoblockmove (mapdef->compiled->verb_ptr, compiled->verb_ptr,
                     i * sizeof(MD_COMPILED_VERB *));
        hoblockmove (mapdef->compiled->picks, compiled->picks, i * sizeof(BYTE *));
        
        /* * FREE OLD COMPILED STRUCTURE * */
        /* ------------------------------- */
        
        am_free (mapdef->compiled->verb_code);
        am_free (mapdef->compiled->overlay_wm_id);
        am_free (mapdef->compiled->verb_wm_id);
        am_free (mapdef->compiled->verb_ptr);
        am_free (mapdef->compiled->picks);
        
        am_free (mapdef->compiled);
        
        /* * REPLACE COMPILED STRUCTURE * */
        /* ------------------------------ */
        
        mapdef->compiled = compiled;
        
        }
    return status;
    }
/* END:     */
