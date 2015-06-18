/* DEC/CMS REPLACEMENT HISTORY, Element WL_UPD_MP_SEL.C*/
/* *4    14-AUG-1990 13:43:20 VINCE "(SPR 5644) Header Standardization"*/
/* *3     6-FEB-1990 13:13:00 MING "(SPR 0) change date/time declaration to new data type DATE_TIME"*/
/* *2    14-SEP-1989 18:44:49 GORDON "(SPR -1) checkin from gulf"*/
/* *1    19-JUN-1989 13:40:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_UPD_MP_SEL.C*/
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
#include "esi_wl.h"

#ifndef ESI_NL_H

#include "esi_nl.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_QS_H

#include "esi_qs.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

#ifndef ESI_MP_H

#include "esi_mp.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Update the select list for the wells on map.
    
Prototype:
    publicdef INT wl_update_map_select(NLIST_HEADER well_nlist,INT num_wells);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    well_nlist      -(NLIST_HEADER)
    num_wells       -(INT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_update_map_select (NLIST_HEADER well_nlist, INT num_wells) 
#else
publicdef INT wl_update_map_select (well_nlist, num_wells)
NLIST_HEADER well_nlist;
INT num_wells;
#endif
    {
    
    INT status;
    NLIST_HEADER select_list;
    SELECT_STRUCT select_user;
    CHAR list_name[64];
    INT select_num;
    WELL_NAMES *add_wells;
    DATE_TIME date_time;
    MAP_STRUCTURE *mapdef;
    UINT dummy;
    
    UINT d_list[1];
    VOIDPTR v_list[1];
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    /* * ALLOCATE FOR ADD LIST * */
    /* ------------------------- */
    
    add_wells = (WELL_NAMES *)am_allocate (AM_APPLICATION,
                                           num_wells * sizeof(WELL_NAMES));
    v_list[0] = (VOIDPTR)add_wells;
    d_list[0] = 1;
    status = nl_set_current_point (well_nlist, 1, 1);
    status = nl_get_points (well_nlist, num_wells, 1, d_list, v_list, &dummy);
    
    /* * TEST FOR CURRENT SELECT LIST * */
    /* -------------------------------- */
    
    mp_get_current_map (&mapdef);
    status = qs_inq_array_c (QS_WELLS_ON_MAP_LIST, mapdef->map_number, list_name, 0);
    
    /* * NO CURRENT SELECT LIST, CREATE IT * */
    /* ------------------------------------- */
    
    if (status != SUCCESS)
        {
        status = sl_init_select_list (WELLS_DATA_TYPE, &select_list);
        status = nl_start_stroke (select_list, &dummy);
        status = nl_add_points (select_list, num_wells, 1, d_list, v_list);
        status = sl_save_list (select_list, 1);
        
        status = nl_inq_nlist_user_struct (select_list, sizeof(SELECT_STRUCT),
                                           (VOIDPTR) & select_user,
                                           (size_t *) & dummy);
        status = qs_set_array_c (QS_WELLS_ON_MAP_LIST, mapdef->map_number,
                                 select_user.name, 0);
        }
    /* * GET OLD SELECT LIST AND ADD TO IT * */
    /* ------------------------------------- */
    
    else
        {
        status = sl_read_list (WELLS_DATA_TYPE, list_name, &select_list);
        status = wl_find_new_wells (add_wells, &num_wells, well_nlist, select_list,
                                    &select_num);
        if (num_wells > 0)
            {
            status = nl_set_current_point (select_list, select_num, 1);
            status = nl_add_points (select_list, num_wells, 1, d_list, v_list);
            
            ho_date_tag (date_time);
            status = nl_inq_nlist_user_struct (select_list, sizeof(SELECT_STRUCT),
                                               (VOIDPTR) & select_user,
                                               (size_t *) & dummy);
            sprintf (select_user.name, "WELLS_%s", date_time);
            status = nl_set_nlist_user_struct (select_list, &select_user,
                                               sizeof(SELECT_STRUCT));
            status = qs_set_array_c (QS_WELLS_ON_MAP_LIST, mapdef->map_number,
                                     select_user.name, 0);
            status = sl_save_list (select_list, 2);
            }
        }
    /* * FREE SELECT LIST * */
    /* -------------------- */
    
    am_free (add_wells);
    status = nl_free_nlist (select_list);
    
    return status;
    }
/* END:     */
