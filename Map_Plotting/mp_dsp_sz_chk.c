/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_SZ_CHK.C*/
/*  *3    17-AUG-1990 22:04:58 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:54:58 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:29 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_DSP_SZ_CHK.C*/
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
    Routine to check that the display arrays can handle given
    row number.
    The counter "rownum" starts from 1 (to match the selector
    box counting system).

Prototype:
    publicdef INT mp_display_size_check(INT rownum);

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
publicdef INT mp_display_size_check (INT rownum)
#else
publicdef INT mp_display_size_check (rownum)
INT rownum;
#endif
    {
    
    PROGNAME ("MP_CREATE_MAP");
    INT status;
    static BOOL inited = FALSE;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    INT i;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    
    if (rownum > mapdef->display->nrows)
        {
        mp_init_display_struct (&display, rownum);
        
        /* * TRANSFER OLD VALUES TO NEW ARRAYS * */
        /* ------------------------------------- */
        
        display->last_row_used = mapdef->display->last_row_used;
        for (i = 0; i <= display->last_row_used; i++)
            {
            display->parameters[i] = mapdef->display->parameters[i];
            }
        hoblockmove (mapdef->display->overlay_displayed, display->overlay_displayed,
                     i * sizeof(BOOL));
        
        hoblockmove (mapdef->display->verbs, display->verbs, i * sizeof(MD_DSP_VERB));
        
        hoblockmove (mapdef->display->keywords, display->keywords,
                     i * sizeof(MD_DSP_KEY));
        hoblockmove (mapdef->display->verb_ndx, display->verb_ndx, i * sizeof(INT));
        hoblockmove (mapdef->display->key_ndx, display->key_ndx, i * sizeof(INT));
        hoblockmove (mapdef->display->compiled_row, display->compiled_row,
                     i * sizeof(INT));
        
        /* * FREE OLD DISPLAY STRUCTURE * */
        /* ------------------------------ */
        
        am_free (mapdef->display->overlay_displayed);
        am_free (mapdef->display->verbs);
        am_free (mapdef->display->keywords);
        am_free (mapdef->display->parameters);
        am_free (mapdef->display->verb_ndx);
        am_free (mapdef->display->key_ndx);
        am_free (mapdef->display->compiled_row);
        
        am_free (mapdef->display);
        
        /* * REPLACE DISPLAY STRUCTURE * */
        /* ----------------------------- */
        
        mapdef->display = display;
        
        }
    return status;
    }
/* END:     */
