/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_S_MPLST.C */
/* *4    24-SEP-1990 16:20:34 GILLESPIE "Beta Deltas" */
/* *3    31-JUL-1990 22:12:17 GILLESPIE "(SPR 1) Add esi_gks.h" */
/* *2    30-JUL-1990 17:22:01 JULIAN "(SPR 0) add lynx,lease select_list" */
/* *1    30-JUL-1990 17:10:25 JULIAN "create seismic map list" */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_S_MPLST.C */
/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_S_MPLST.C*/
/* *2     9-SEP-1990 14:31:35 SCD "(SPR 0) Fix picking for any type of seismic overlay."*/
/* *1    13-AUG-1990 16:40:14 SYSTEM "Finder 6.1"*/
/* DEC/CMS REPLACEMENT HISTORY, Element GB_CRE_S_MPLST.C*/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/
#include "esi_c_lib.h"
#ifndef ESI_GB_H
#include "esi_gb.h"
#endif
#ifndef ESI_GB_MSG_H
#include "esi_gb_msg.h"
#endif
#ifndef ESI_MP_H
#include "esi_mp.h"
#endif
#ifndef ESI_NL_H
#include "esi_nl.h"
#endif
#ifndef ESI_SO_H
#include "esi_so.h"
#endif
#ifndef ESI_MD_DBDEFS_H
#include "esi_md_dbdefs.h"
#endif
#ifndef ESI_WM_H
#include "esi_wm.h"
#endif
#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#define ARRAY_BLOCKSIZE 20

#if USE_PROTOTYPES
static INT gbz_create_seis_on_map_list (MAP_STRUCTURE *map,
                                             NLIST_HEADER *nlist);
#else
static INT gbz_create_seis_on_map_list ();
#endif

#if USE_PROTOTYPES
publicdef INT gb_create_seis_on_map_list (MAP_STRUCTURE *map,
                                          NLIST_HEADER *nlist, BOOL flag)
#else
publicdef INT gb_create_seis_on_map_list (map, nlist,flag)
MAP_STRUCTURE *map;
NLIST_HEADER *nlist;
BOOL flag; /* not used in this function, only to generalize the call methods */
#endif
    {
    return gbz_create_seis_on_map_list (map, nlist);
    }

/*--------------------------------------------------------------*/
/*-------------- gbz_create_seis_on_map_list -------------------*/
/*--------------------------------------------------------------*/

#if USE_PROTOTYPES
static INT gbz_create_seis_on_map_list (MAP_STRUCTURE *map, NLIST_HEADER *nlist)
#else
static INT gbz_create_seis_on_map_list (map, nlist)
MAP_STRUCTURE *map;
NLIST_HEADER *nlist;
#endif
    {
    INT status = SUCCESS;
    INT i;
    UINT j;
    UINT k;
    MD_DISPLAY *display;
    MD_COMPILED_MAPDEF *compiled;
    INT row;
    INT verb_ndx;
    PICKABLE_SEISMIC **pick_seis;
    UINT d_list[3];
    VOIDPTR v_list[3];
    INT n_used = 0;
    INT n_alloc = 0;
    FULL_LINE_NAME seis_line;
    SEIS_LINE_ID *id_arr = (SEIS_LINE_ID *)0;
    INT *cdp1_arr = (INT *)0;
    INT *cdp2_arr = (INT *)0;

    /*  Loop thru map display structure to get
        displayed seismic overlays, then loop thru
        seismic picking structures to get visible
        lines.  Insert these into a hash table to
        insure uniqueness.              */


    wmwopen (map->map_window_id);
    verb_ndx = -1;
    display = map->display;
    compiled = map->compiled;

    for (i = 0; i < display->last_row_used; i++)
        {
        if (verb_ndx != display->verb_ndx[i])
            {
            verb_ndx = display->verb_ndx[i];
            if (verb_ndx != -1 && display->overlay_displayed[i])
                {
                row = display->compiled_row[i];
                if ( mp_is_seismic_overlay(compiled->verb_code[row]) )
                    {
                    pick_seis = (PICKABLE_SEISMIC * *)compiled->picks[row];

                    for (j = 0; j < pick_seis[0]->nused; j++)
                        {
                        if (wm_inq_seg_vis (pick_seis[0]->segment_id[j]) ==
                            GVISI)
                            {
                            seis_line = pick_seis[0]->line_name[j];


                            /* * LOOK FOR FOUND LINES IN ID_ARR * */
                            /* --------------------------------- */

                            for (k = 0; k < n_used; k++)
                                {
                                if (seis_line.id == id_arr[k])
                                    {
                                    break;
                                    }
                                }


                            /* * IF NOT IN LIST, ADD * */
                            /* ----------------------- */

                            if (k == n_used)
                                {
                                if (n_used == n_alloc)
                                    {
                                    n_alloc += ARRAY_BLOCKSIZE;

                                    /* ALLOCATE ARRAYS */

                                    if (n_used == 0)
                                        {
                                        id_arr = (SEIS_LINE_ID *
                                                  )tc_alloc (
                                                n_alloc
                                                      * sizeof(SEIS_LINE_ID));
                                        cdp1_arr = (INT *
                                                  )tc_alloc (
                                                         n_alloc
                                                             * sizeof(INT));
                                        cdp2_arr = (INT *
                                                  )tc_alloc (
                                                         n_alloc
                                                             * sizeof(INT));
                                        }
                                    /* REALLOCAT ARRAYS */

                                    else
                                        {
                                        id_arr = (SEIS_LINE_ID *
                                                  )tc_realloc (
                                                             (VOIDPTR)id_arr,
                                                               n_alloc
                                                               *
                                                          sizeof(
                                                               SEIS_LINE_ID));
                                        cdp1_arr = (INT *
                                                    )tc_realloc (
                                                            (VOIDPTR)cdp1_arr,
                                                                 n_alloc
                                                                 *
                                                                 sizeof(INT));
                                        cdp2_arr = (INT *
                                                    )tc_realloc (
                                                            (VOIDPTR)cdp2_arr,
                                                                 n_alloc
                                                                 *
                                                                 sizeof(INT));
                                        }
                                    }

                                /* STORE LINE IN ARRAY */

                                id_arr[n_used] = seis_line.id;
                                cdp1_arr[n_used] = 0;
                                cdp2_arr[n_used] = 0;
                                n_used++;
                                }
                            }
                        }
                    }
                }
            }
        }

    /*  Get all the line names from the hash table  */
    /*  and put them into select list               */

    if (n_used > 0)
        {
        d_list[0] = 1;
        d_list[1] = 2;
        d_list[2] = 3;
        v_list[0] = (VOIDPTR)id_arr;
        v_list[1] = (VOIDPTR)cdp1_arr;
        v_list[2] = (VOIDPTR)cdp2_arr;
        sl_init_select_list (SEISMIC_DATA_TYPE, nlist);
        nl_start_table(*nlist, &j);
        nl_add_rows (*nlist, n_used, 3, d_list, v_list);
        }
    else
        {
        status = GB_NO_SEISMIC_ON_MAP;
        }
    tc_free (id_arr);
    tc_free (cdp1_arr);
    tc_free (cdp2_arr);

    wmwclose (map->map_window_id);
    return status;
    }

