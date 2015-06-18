/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ER_OVL.C*/
/*  *3    17-AUG-1990 22:05:37 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:55:38 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:11:51 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_ER_OVL.C*/
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
#include    "esi_mp.h" 
#include    "esi_am.h" 
#include    "esi_wm.h" 
#include    "esi_gks.h" 
#include    "esi_dr.h" 

#define IDENT "mp_erase_overlay" 

/* Function Description -----------------------------------------------------
Description:
    Erase any overlay from the current map. 
    Deletes the overlay from the global workspace lists 
    for the current map. 

Prototype:
    publicdef INT mp_erase_overlay(INT comp_row);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    comp_row        -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_erase_overlay (INT comp_row)
#else
publicdef INT mp_erase_overlay (comp_row)
INT comp_row;
#endif
    {
    INT status;
    INT id;
    MAP_STRUCTURE *mapdef;
    MD_COMPILED_MAPDEF *compiled;
    GO_WORKSPACE *w;
    CHAR go_of_map[128];
    
    status = SUCCESS;
    
    am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    compiled = mapdef->compiled;
    id = compiled->wm_arr_ndx[comp_row];
    
    if (compiled->overlay_wm_id[comp_row] != 0 AND mapdef->wm_id_count[id] > 0)
        {
        wmwopen (mapdef->map_window_id);
        if (compiled->verb_wm_id[comp_row] EQUALS 0)
            {
            wmgdelete (compiled->overlay_wm_id[comp_row]);
            compiled->overlay_wm_id[comp_row] = 0;
            }
        else
            {
            wmgopen (compiled->overlay_wm_id[comp_row]);
            wmgdelete (compiled->verb_wm_id[comp_row]);
            compiled->verb_wm_id[comp_row] = 0;
            wmgclose (compiled->overlay_wm_id[comp_row]);
            }
        (mapdef->wm_id_count[id])--;
        
        id = MAIN_WKID;
        wm_grsgwk (id);
        wmwclose (mapdef->map_window_id);
        
        /* set the CHANGE flag to be ON, if the specified overlay is GRAPHIC OBJECT. */
        /* open the GO workspace for this specified map. */
        
        sprintf (go_of_map, "GO_SPACE_%d", mapdef->map_number);
        status = am_open_workspace (go_of_map, AM_GLOBAL, (VOIDPTR *) & w);
        if (status EQUALS SUCCESS)
            {
            if (w->temp_nlist != (NLIST_HEADER)NULL)
                status = nl_free_nlist (w->temp_nlist);
            
            if (w->master_nlist != (NLIST_HEADER)NULL)
                status = nl_free_nlist (w->master_nlist);
            status = am_release_workspace (go_of_map, AM_GLOBAL);
            }
        }
    return status;
    }
/* END:     */
