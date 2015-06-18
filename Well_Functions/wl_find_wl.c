/* DEC/CMS REPLACEMENT HISTORY, Element WL_FIND_WL.C*/
/* *3    14-AUG-1990 13:41:01 VINCE "(SPR 5644) Header Standardization"*/
/* *2    14-SEP-1989 18:43:59 GORDON "(SPR -1) checkin from gulf"*/
/* *1    19-JUN-1989 13:40:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WL_FIND_WL.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
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

#ifndef ESI_SL_H

#include "esi_sl.h"

#endif

#ifndef ESI_HT_H

#include "esi_ht.h"

#endif

#ifndef ESI_AM_H

#include "esi_am.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Search the well list for wells not already in the select
    list.

Prototype:
    publicdef INT wl_find_new_wells(WELL_NAMES *add_wells,INT *num_wells,
        NLIST_HEADER well_nlist, NLIST_HEADER select_list,INT *select_num);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    add_wells       -(WELL_NAMES *)
    num_wells       -(INT *)
    well_nlist      -(NLIST_HEADER)
    select_list     -(NLIST_HEADER)
    select_num      -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT wl_find_new_wells (WELL_NAMES *add_wells, INT *num_wells,
                                 NLIST_HEADER well_nlist, NLIST_HEADER select_list,
                                 INT *select_num) 
#else
publicdef INT wl_find_new_wells (add_wells, num_wells, well_nlist, select_list,
                                 select_num)
WELL_NAMES *add_wells;
INT *num_wells;
NLIST_HEADER well_nlist;
NLIST_HEADER select_list;
INT *select_num;
#endif
    {
    
    INT status;
    WELL_NAMES *dum_list;
    INT i;
    HASH_TABLE *ht;
    UINT dummy;
    static DOUBLE init_array[8] = 
        {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
        };
    INT counter = 0;
    
    UINT d_list[1];
    VOIDPTR v_list[1];
    
    /* ********************************************************************** */
    
    status = SUCCESS;
    
    d_list[0] = 1;
    
    /* * PUT WELL LIST IN TCP AND SORT * */
    /* --------------------------------- */
    
    status = nl_inq_nlist_int (select_list, NL_NLIST_NPOINTS, (INT *)select_num);
    dum_list = (WELL_NAMES *)am_allocate (AM_APPLICATION,
                                          (*select_num) * sizeof(WELL_NAMES));
    v_list[0] = (VOIDPTR)dum_list;
    status = nl_set_current_point (select_list, 1, 1);
    status = nl_get_points (select_list, *select_num, 1, d_list, v_list, &dummy);
    
    i = MAX ((*select_num) * 0.2, 10.0);
    ht_init_hash_table (i, 10, HT_CHAR_KEY, sizeof(WELL_NAMES), 0,
                        HT_APPLICATION_TABLE, &ht);
    for (i = 0; i < *select_num; i++)
        {
        v_list[0] = (VOIDPTR)dum_list[i];
        dummy = strlen (dum_list[i]);
        hoblockmove (init_array, &(dum_list[i][dummy]), (sizeof(WELL_NAMES) - dummy));
        ht_insert (ht, v_list[0], 0);
        }
    /* * PUT SELECT LIST IN TCP AND SORT * */
    /* ----------------------------------- */
    
    for (i = 0; i < *num_wells; i++)
        {
        v_list[0] = (VOIDPTR)add_wells[i];
        dummy = strlen (add_wells[i]);
        hoblockmove (init_array, &(add_wells[i][dummy]), (sizeof(WELL_NAMES) - dummy));
        if (ht_find (ht, v_list[0], 0) != SUCCESS)
            {
            if (counter != i)
                {
                strcpy (add_wells[counter], add_wells[i]);
                }
            counter++;
            }
        }
    *num_wells = counter;
    
    am_free (dum_list);
    
    return status;
    }
/* END:     */
