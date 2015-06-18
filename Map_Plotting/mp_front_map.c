/*  DEC/CMS REPLACEMENT HISTORY, Element MP_FRONT_MAP.C*/
/*  *3    17-AUG-1990 22:05:41 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-JUN-1989 10:55:55 GILLESPIE "(SPR 20) Fix castings"*/
/*  *1    19-JUN-1989 13:12:00 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element MP_FRONT_MAP.C*/
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
#include "esi_am.h"
#include "esi_mp.h"
#include "esi_qs.h"
#include "esi_wm.h"
#include "esi_gks.h"
#include "esi_mg.h"
#include "esi_fi.h"
#include "esi_wi.h"
#include "esi_ts.h"
#include "esi_md_msg.h"
#include "esi_md_err.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to make a map appear as the front window.

    CASES:

            1) No maps defined or no defined maps drawn.
            Returns FAIL and a NULL POINTER

            2) Front window is a map.  Sets QS variables, returns
            SUCCESS and pointer to the mapdef.

            3) 1 map only exists that is not in the front.
            The map window is fronted and returns same as 2).

            4) 2 or more maps exists, none of which are the front
            window.  A selector box is placed on the screen,
            requesting which map is desired.  Reqested map is
            fronted and returns same as 2).

Prototype:
    publicdef INT mp_front_map(MAP_STRUCTURE **mapdef);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    mapdef          -(MAP_STRUCTURE **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT mp_front_map (MAP_STRUCTURE **mapdef)
#else
publicdef INT mp_front_map (mapdef)
MAP_STRUCTURE **mapdef;
#endif
    {
    
    PROGNAME ("MP_FRONT_MAP");
    INT status;
    MAP_STRUCTURE *temp_map;
    INT i;
    INT window_id;
    MAP_NAME mname;
    INT *nmaps;
    INT *map_numbers = (INT *)0;
    CHAR **map_list = (CHAR **)0;
    INT num_disp = 0;
    INT wkid;
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * GET NUMBER OF DEFINED MAPS * */
    /* ------------------------------ */
    
    status = am_open_workspace ("MAP_NUMBER", AM_GLOBAL, (VOIDPTR *) & nmaps);
    if (status != SUCCESS OR * nmaps < 0)
        {
        status = FAIL;
        temp_map = (MAP_STRUCTURE *)0;
        goto finished;
        }
    /* * LOOK UP THE CURRENT FRONT WINDOW * */
    /* ------------------------------------ */
    
    mp_store_workspace ();
    wm_inq_front_window (&window_id);
    if (window_id > 0)
        {
        map_numbers = (INT *)am_allocate (AM_GLOBAL, (*nmaps) * sizeof(INT));
        for (i = 1; i <= *nmaps; i++)
            {
            qs_set_i (QS_CUR_MAP_NUMBER, i);
            mp_get_current_map (&temp_map);
            if (temp_map->map_window_id EQUALS window_id)
                {
                goto finished;
                }
            else if (temp_map->map_window_id != 0)
                {
                sprintf (mname, "#%d: %s", temp_map->map_number, temp_map->map_name);
                ts_tcp_append (&map_list, mname);
                map_numbers[num_disp] = temp_map->map_number;
                num_disp++;
                }
            }
        }
    else
        {
        status = FAIL;
        temp_map = (MAP_STRUCTURE *)0;
        goto finished;
        }
    /* * FRONT WINDOW IS NOT A MAP * */
    /* ----------------------------- */
    
    if (num_disp EQUALS 0)           /* NO MAPS DISPLAYED */
        {
        status = FAIL;
        temp_map = (MAP_STRUCTURE *)0;
        }
    else
        {
        if (num_disp EQUALS 1)       /* ONE MAP DISPLAYED */
            {
            status = SUCCESS;
            }
        else if (num_disp > 1)      /* MULTIPLE MAPS DEFINED */
            {
            fi_push ();
            wi_ssst ("", num_disp, 1, map_list);
            wi_request_selection (mg_message (MD_PICK_MAP), &i, &num_disp, &status);
            fi_pop ();
            }
        if (status EQUALS SUCCESS)
            {
            qs_set_i (QS_CUR_MAP_NUMBER, map_numbers[num_disp - 1]);
            mp_get_current_map (&temp_map);
            wmwfront (temp_map->map_window_id);
            wkid = MAIN_WKID;
            wm_grsgwk (wkid);
            /*  once the current map is changed, the
                MAP_PROJECTION value need to be changed too
                , COM JJC 09/16/88              */
            
            qs_set_c (QS_MAP_PROJECTION, temp_map->map_projection,
                      strlen (temp_map->map_projection));
            }
        else
            {
            status = FAIL;
            temp_map = (MAP_STRUCTURE *)0;
            }
        }
finished:
    *mapdef = temp_map;
    am_free (map_numbers);
    ts_tcp_free (map_list);
    
    return status;
    }
/* END:     */
