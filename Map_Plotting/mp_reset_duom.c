/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RESET_DUOM.C*/
/*  *3    17-AUG-1990 22:08:54 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    13-DEC-1989 14:32:36 CONROY "(SPR 5071) Mods for label plotting fix"*/
/*  *1    13-DEC-1989 14:30:00 CONROY "Resets DUOM parameters to plotter inches"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_RESET_DUOM.C*/
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
#include "esi_md_basis.x"   /* publicdefs (externs) needed here */

/* Function Description -----------------------------------------------------
Description:
    Routine to reset the parameter structures for display
    unit of measure keys which the user has explicitly set to
    plotter inches.

Prototype:
    publicdef INT mp_reset_duom(INT display_line);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_reset_duom (INT display_line)
#else
publicdef INT mp_reset_duom (display_line)
INT display_line;
#endif
    {
    
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    INT i;
    INT comp_row;
    INT verb_ndx;
    INT key_ndx;
    DOUBLE map_wcs_to_inches;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    comp_row = display->compiled_row[display_line];
    verbstruc = compiled->verb_ptr[comp_row];
    
    verb_ndx = display->verb_ndx[display_line];
    temp_verb_info = &(overlay_verbs[verb_ndx]);
    
    /* * FAKE OUT WCS * */
    /* ---------------- */
    
    map_wcs_to_inches = mapdef->wcs_to_inches;
    mapdef->wcs_to_inches = 1.0;
    
    /* * PROCESS LOOP * */
    /* ---------------- */
    
    for (i = display_line;
         i < display->last_row_used, comp_row EQUALS display->compiled_row[i]; i++)
        {
        key_ndx = display->key_ndx[i];
        if (key_ndx != -1)
            {
            temp_key_info = &(temp_verb_info->key_list[key_ndx]);
            
            if (temp_key_info->val_code EQUALS UOMD$)
                {
                am_free (verbstruc->parm_ptr[key_ndx]);
                md_verify_uomd (temp_key_info, display->parameters[i],
                                &(verbstruc->parm_ptr[key_ndx]));
                }
            }
        }
    /* * UNDO FAKE OUT * */
    /* ----------------- */
    
    mapdef->wcs_to_inches = map_wcs_to_inches;
    return status;
    }
/* END:     */
