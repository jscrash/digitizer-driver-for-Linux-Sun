/* DEC/CMS REPLACEMENT HISTORY, Element OV_OPN_OVL.C*/
/* *4    25-JUL-1990 19:59:24 VINCE "(SPR 5644) Header Standardization"*/
/* *3    23-OCT-1989 10:37:06 CONROY "(SPR 5043) Mods for new MAPPING UI programmer tools"*/
/* *2    26-JUN-1989 11:02:15 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 13:22:01 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element OV_OPN_OVL.C*/
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

#include "esi_gl_defs.h"
#include "esi_am.h"
#include "esi_mp.h"
#include "esi_ov.h"
#include "esi_wm.h"
#include "esi_md_dbdefs.h"

/* Function Description -----------------------------------------------------
Description:
    Function to open the supplied overlay with the supplied
    attributes.

Prototype:
    publicdef INT ov_open_overlay(INT display_line,CHAR *user_name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    display_line    -(INT)
    user_name       -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ov_open_overlay (INT display_line, CHAR *user_name)
#else
publicdef INT ov_open_overlay (display_line, user_name)
INT display_line;
CHAR *user_name;
#endif
    {
    INT status;
    INT position;
    CHAR *group_name;
    BOOL sub_groups;
    BOOL found;
    MAP_STRUCTURE *mapdef;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    MD_COMPILED_VERB *verbstruc;
    INT i;
    INT row;
    INT code;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    display = mapdef->display;
    compiled = mapdef->compiled;
    
    row = display->compiled_row[display_line];
    
    /* * DETERMINE IF THIS IS A DEFAULT NAMED OR USER NAMED OVERLAY * */
    /* -------------------------------------------------------------- */
    
    if (user_name EQUALS NULL OR strlen (user_name) EQUALS 0)
        {
        sub_groups = FALSE;
        if (compiled->verb_code[row] != MP_GRA_OBJS)
            {
            group_name = display->verbs[display_line];
            }
        else
            {
            verbstruc = compiled->verb_ptr[row];
            for (i = 0; i < verbstruc->nrows; i++)
                {
                if (verbstruc->key_code[i] EQUALS 20)
                    {
                    group_name = verbstruc->parm_ptr[i]->parmval.cval;
                    break;
                    }
                }
            }
        }
    else
        {
        group_name = user_name;
        sub_groups = TRUE;
        }
    /* * GET THE POSITION IN THE WM GROUP LIST FROM MAP * */
    /* -------------------------------------------------- */
    
    found = ov_search_list (group_name, sub_groups, &position);
    compiled->wm_arr_ndx[row] = position;
    (mapdef->wm_id_count[position])++;
    if (mapdef->wm_group_names[position] EQUALS NULL)
        {
        mapdef->wm_group_names[position] = (CHAR *)am_allocate (AM_GLOBAL,
                                                                strlen (group_name) +
                                                                1);
        strcpy (mapdef->wm_group_names[position], group_name);
        }
    /* * OPEN OVERLAY GROUP * */
    /* ---------------------- */
    
    code = MD_NAMED_OVERLAY + compiled->verb_code[row];
    if (sub_groups)
        {
        if (found AND mapdef->wm_id_list[position] > 0)
            {
            wmgopen (mapdef->wm_id_list[position]);
            }
        else
            {
            wmgallocate (&(mapdef->wm_id_list[position]));
            wmoset (mapdef->wm_id_list[position], MD_NAMED_OVERLAY, "", group_name);
            }
        compiled->overlay_wm_id[row] = mapdef->wm_id_list[position];
        wmgallocate (&(compiled->verb_wm_id[row]));
        if (compiled->verb_code[row] != MP_GRA_OBJS)
            {
            wmoset (compiled->verb_wm_id[row], code, "", display->verbs[display_line]);
            }
        else
            {
            verbstruc = compiled->verb_ptr[row];
            for (i = 0; i < verbstruc->nrows; i++)
                {
                if (verbstruc->key_code[i] EQUALS 20)
                    {
                    wmoset (compiled->verb_wm_id[row], code, "",
                            verbstruc->parm_ptr[i]->parmval.cval);
                    break;
                    }
                }
            }
        }
    else
        {
        wmgallocate (&(mapdef->wm_id_list[position]));
        compiled->overlay_wm_id[row] = mapdef->wm_id_list[position];
        wmoset (compiled->overlay_wm_id[row], code, "", group_name);
        }
    return status;
    }
/* END:     */
