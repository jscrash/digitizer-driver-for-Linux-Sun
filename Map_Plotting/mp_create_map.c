/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CREATE_MAP.C*/
/*  *4    17-AUG-1990 22:03:58 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    10-OCT-1989 16:55:50 CONROY "(SPR 5028) Mods for new plotting capability"*/
/*  *2    26-JUN-1989 10:53:10 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:10:48 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_CREATE_MAP.C*/
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to start the creation of a new map.

Prototype:
    publicdef INT mp_create_map();

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_create_map ()
#else
publicdef INT mp_create_map ()
#endif
    {
    
    PROGNAME ("MP_CREATE_MAP");
    INT status;
    static BOOL inited = FALSE;
    MAP_STRUCTURE *defaults;
    MAP_STRUCTURE *mapdef;
    PROJECT_DEFAULT *project_defaults;
    INT i;
    INT *map_number;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * OPEN OR INITIALIZE DEFAULT MAP WORKSPACE * */
    /* -------------------------------------------- */
    
    if (NOT inited)
        {
        am_open_workspace ("PROJECT_DEFAULTS", AM_GLOBAL,
                           (VOIDPTR *) & project_defaults);
        am_define_workspace ("MP_DFLTS", AM_GLOBAL, sizeof(MAP_STRUCTURE),
                             (VOIDPTR *) & defaults);
        
        tc_zeroes (defaults, sizeof(MAP_STRUCTURE));
        strcpy (defaults->map_name, "UNTITLED");
        defaults->valid = TRUE;
        strcpy (defaults->map_projection, project_defaults->map.projection);
        strcpy (defaults->map_scale_uom, project_defaults->map.scale_unit);
        strcpy (defaults->map_coordinate_system,
                project_defaults->map.coordinate_system);
        defaults->map_header_id = 0;
        defaults->upper_right_xy.x = project_defaults->map.upper_right_x;
        defaults->upper_right_xy.y = project_defaults->map.upper_right_y;
        defaults->lower_left_xy.x = project_defaults->map.lower_left_x;
        defaults->lower_left_xy.y = project_defaults->map.lower_left_y;
        defaults->map_scale = project_defaults->map.scale;
        defaults->map_displayed = FALSE;
        defaults->map_title_on_map = FALSE;
        defaults->full_screen_flag = TRUE;
        defaults->wcs_to_inches = 1.0;
        
        defaults->scale_sizes_for_plotting = mp_default_size_scale ();
        
        inited = TRUE;
        }
    else
        {
        am_open_workspace ("MP_DFLTS", AM_GLOBAL, (VOIDPTR *) & defaults);
        }
    /* * OPEN APPLICATION MAP WORKSPACE * */
    /* ---------------------------------- */
    
    status = am_open_workspace ("MAPDEF", AM_APPLICATION, (VOIDPTR *) & mapdef);
    if (status != SUCCESS)
        {
        am_define_workspace ("MAPDEF", AM_APPLICATION, sizeof(MAP_STRUCTURE),
                             (VOIDPTR *) & mapdef);
        }
    /* * FILL APPLICATION WORKSPACE WITH DEFAULTS * */
    /* -------------------------------------------- */
    
    hoblockmove (defaults, mapdef, sizeof(MAP_STRUCTURE));
    
    /* * INITIALIZE OTHER MAPDEF ELEMENTS * */
    /* ------------------------------------ */
    
    /* MAP NUMBER */
    am_open_workspace ("MAP_NUMBER", AM_GLOBAL, (VOIDPTR *) & map_number);
    (*map_number)++;
    mapdef->map_number = *map_number;
    
    /* DISPLAY STRUCTURE */
    mp_init_display_struct (&(mapdef->display), 1);
    mapdef->display->last_row_used = 0;
    
    /* COMPILED MAPDEF STRUCT */
    mp_init_compiled_mapdef (&(mapdef->compiled), 1);
    mapdef->compiled->last_row_used = -1;
    
    /* WINDOW MANAGER STRUCTS */
    mapdef->wm_id_list = (INT *)am_allocate (AM_GLOBAL, 100 * sizeof(INT));
    tc_zeroes (mapdef->wm_id_list, 100 * sizeof(INT));
    mapdef->wm_id_count = (INT *)am_allocate (AM_GLOBAL, 100 * sizeof(INT));
    tc_zeroes (mapdef->wm_id_count, 100 * sizeof(INT));
    mapdef->wm_group_names = (CHAR **)am_allocate (AM_GLOBAL, 100 * sizeof(CHAR *));
    tc_zeroes (mapdef->wm_group_names, 100 * sizeof(CHAR *));
    mapdef->wm_nalloc = 100;
    
    /* * INITIALIZE MAP DEFINITION DIALOG * */
    /* ------------------------------------ */
    
    mp_init_map_server ();
    
    return SUCCESS;
    }
/* END:     */
