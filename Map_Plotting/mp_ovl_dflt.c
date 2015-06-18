/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OVL_DFLT.C*/
/*  *5    30-OCT-1990 12:04:49 GILLESPIE "(SPR 37) Fix up Sun compiles"*/
/*  *4    17-AUG-1990 22:07:52 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    10-OCT-1989 16:59:46 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:58:19 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:13:51 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_OVL_DFLT.C*/
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
#include "esi_md_basis.x"   /* externals needed here */

/* Function Description -----------------------------------------------------
Description:
    Routine to fill the verb structure with any default values
    that are needed.

Prototype:
    publicdef INT mp_overlay_defaults(INT verb_ndx,INT comp_row);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    verb_ndx        _(INT)
    comp_row        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_overlay_defaults (INT verb_ndx, INT comp_row)
#else
publicdef INT mp_overlay_defaults (verb_ndx, comp_row)
INT verb_ndx;
INT comp_row;
#endif
    {
    INT status;
    MAP_STRUCTURE *mapdef;
    MD_COMPILED_VERB *verbstruc;
    MD_VERB_INFO *temp_verb_info;
    MD_KEY_INFO *temp_key_info;
    MD_PARM_STRUCT *parmstruc;
    INT i;
    DOUBLE plot_wcs_to_inches, map_wcs_to_inches;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *)&mapdef);
    
    /* * FAKE OUT FOR PLOT SCALING * */
    /* ----------------------------- */
    
    if (mapdef->scale_sizes_for_plotting)
        {
        map_wcs_to_inches = mapdef->wcs_to_inches;
        mp_get_plot_wcs_to_inches (mapdef, &plot_wcs_to_inches);
        mapdef->wcs_to_inches = plot_wcs_to_inches;
        }
    /* * LOOP TO FIND DEFAULTS AND SET THEM * */
    /* -------------------------------------- */
    
    verbstruc = mapdef->compiled->verb_ptr[comp_row];
    temp_verb_info = &(overlay_verbs[verb_ndx]);
    
    for (i = 0; i < verbstruc->nrows; i++)
        {
        temp_key_info = &(temp_verb_info->key_list[i]);
        if (verbstruc->key_used[i] EQUALS 0 AND strlen (temp_key_info->key_default) !=
            0)
            {
            if (mp_parm_ok (temp_key_info->key_default, verb_ndx, i,
                            &(verbstruc->parm_ptr[i]), &status))
                {
                verbstruc->key_used[i] = 2;
                }
            }
        else if (verbstruc->key_used[i] EQUALS 2 AND temp_key_info->
                 val_code EQUALS UOMD$)
            {
            am_free (verbstruc->parm_ptr[i]);
            md_verify_uomd (temp_key_info, temp_key_info->key_default,
                            &(verbstruc->parm_ptr[i]));
            }
        }
    /* * UNDO FAKE OUT * */
    /* ----------------- */
    
    if (mapdef->scale_sizes_for_plotting)
        {
        mapdef->wcs_to_inches = map_wcs_to_inches;
        }
    return status;
    }
/* END:     */
