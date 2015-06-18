/* DEC/CMS REPLACEMENT HISTORY, Element GB_RPICK_WELL.C*/
/* *6    14-AUG-1990 13:27:09 VINCE "(SPR 5644) Header Standardization"*/
/* *5     5-SEP-1989 15:08:45 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *4    11-AUG-1989 20:41:07 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/* *3    11-AUG-1989 12:42:44 GORDON "(SPR 105) use sizeof(SELECT_KEY_STRING), not 62"*/
/* *2    18-JUL-1989 14:48:41 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:52:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_RPICK_WELL.C*/
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

#include "esi_gb.h"
#include "esi_mp.h"
#include "esi_nl.h"
#include "esi_sl.h"
#include "esi_ht.h"
#include "esi_tc.h"
#include "esi_md_basis.x"

/* Function Description -----------------------------------------------------
Description:
    Function to locate and return the name of a list of wells
    which fall within the supplied rectangle.  These may be
    only the visible wells or all wells currently on the map
    depending on the value of the supplied boolean parameter.

Prototype:
    publicdef INT gb_rect_pick_well (DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, DOUBLE ymax, 
        NLIST_HEADER *select_list, BOOL full);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    xmin            -(DOUBLE)
    ymin            -(DOUBLE)
    xmax            -(DOUBLE)
    ymax            -(DOUBLE)
    select_list     -(NLIST_HEADER *) Pointer to created select list.
    full            -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT gb_rect_pick_well (DOUBLE xmin, DOUBLE ymin, DOUBLE xmax, DOUBLE ymax,
                                 NLIST_HEADER *select_list, BOOL full)
#else
publicdef INT gb_rect_pick_well (xmin, ymin, xmax, ymax, select_list, full)
DOUBLE xmin;
DOUBLE ymin;
DOUBLE xmax;
DOUBLE ymax;
NLIST_HEADER *select_list;
BOOL full;
#endif
    {
    
    INT status;                     /* RETURN STATUS */
    INT i;
    UINT j;
    DOUBLE *xy;
    SELECT_KEY_STRING *uwi_list;
    HASH_TABLE *ht;
    UINT d_list[1];
    VOIDPTR v_list[1];
    
    /* ---------------------------------------------------------------------- */
    /* Build hash table of UWI, X, Y of wells which are in overlays of the    */
    /* current map and are within the given rectangle                         */
    /* ---------------------------------------------------------------------- */
    
    status = gb_rect_map_wells (xmin, ymin, xmax, ymax, &ht, full);
    
    if (status == SUCCESS)
        {
        
        /* * RETURN FOUND WELLS OR ERROR STATUS * */
        /* -------------------------------------- */
        
        ht_get_all (ht, (UINT *) & i, (VOIDPTR *) & uwi_list, (VOIDPTR *) & xy);
        if (i > 0)
            {
            status = SUCCESS;
            
            d_list[0] = 1;
            v_list[0] = (VOIDPTR)uwi_list;
            sl_init_select_list (WELLS_DATA_TYPE, select_list);
            nl_start_stroke (*select_list, &j);
            nl_add_points (*select_list, i, 1, (UINT *)d_list, (VOIDPTR)v_list);
            tc_free ((VOIDPTR)uwi_list);
            tc_free ((VOIDPTR)xy);
            }
        else
            {
            status = FAIL;
            }
        }
    ht_free_table (&ht);
    
    return status;
    }
/* END:     */
