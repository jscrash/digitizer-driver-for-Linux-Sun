/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_OVL.C*/
/* *13    6-JUN-1991 17:50:48 MING "(SPR 0) change seismic time to seismic surface"*/
/* *12   24-SEP-1990 17:26:08 GILLESPIE "Beta Deltas"*/
/* *11    1-AUG-1990 13:19:08 VINCE "(SPR 5615) updated for integer line id"*/
/* *10   26-JUL-1990 17:22:15 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *9    25-JUL-1990 20:06:09 VINCE "(SPR 5644) Header Standardization"*/
/* *8     2-NOV-1989 09:54:27 GILLESPIE "(SPR 30) Changes from moving mapping structure out of esi_so.h"*/
/* *7    31-OCT-1989 10:52:24 JULIAN "(SPR 5039) change sl files to se files"*/
/* *6    30-OCT-1989 14:39:46 JULIAN "(SPR 5039) Seismic Select List Mods"*/
/* *5    10-OCT-1989 10:02:52 JULIAN "(SPR 5020) mods for CT parameter change"*/
/* *4    25-AUG-1989 17:13:54 CONROY "(SPR 9999) add code to update screen only every 25 lines"*/
/* *3    11-AUG-1989 20:23:06 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *2    25-JUL-1989 20:07:48 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:22 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_OVL.C*/

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
#include "esi_so.h"
#include "esi_am.h"

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_SE_H
#include "esi_se.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to process seismic select list.

Prototype:
    publicdef INT so_process_overlay(SO_STRUCT *pso,
                                     PICKABLE_SEISMIC **pick_seis);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.
    pick_seis       -(PICKABLE_SEISMIC **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_process_overlay (SO_STRUCT *pso, PICKABLE_SEISMIC **pick_seis)
#else
publicdef INT so_process_overlay (pso, pick_seis)
SO_STRUCT *pso;
PICKABLE_SEISMIC **pick_seis;
#endif
    {
    INT status;
    INT n_lines = 0;                /* number of lines to process */
    INT iline;                      /* iline in for loop */
    UINT *nalloc, *nused;           /* picking variables */
    INT segment_id;
    INT blocksize = 100;
    INT len = 0;
    INT displayed_lines = 0;
    COORD xmin, xmax, ymin, ymax;
    NLIST_HEADER slist = (NLIST_HEADER)0;   /* temporary select list pointer */
    NLIST_HEADER process_list = (NLIST_HEADER)0;    /* final list to process */
    CHAR *select_phrases;
    CHAR text[256];
    BOOL free_phrase = FALSE;
    UINT ndim;
    UINT dim_list[1];               /* nlist dim list */

    /*--------------------------------------------------------------------*/
    /* nlist value list                                                   */
    /* The following values are set by the SELECT SLIST SEISMIC SUBSYSTEM */
    /* and must not be dicked with                                        */
    /*--------------------------------------------------------------------*/
    VOIDPTR value_list[1];
    
    INT select_line_id;
    MAP_STRUCTURE *pmap = (MAP_STRUCTURE *)pso->map;
    
    /**************************************************************************/
    
    status = SUCCESS;
    slist = (NLIST_HEADER)0;
    
    /* * MODIFICATION MADE FOR SOFTWARE PICKING * */
    /* ------------------------------------------ */
    
    if (pso->select_list != 0 OR pso->select_list != (NLIST_HEADER)0)
        {
        nl_free_nlist (pso->select_list);
        }

    /* * IF SELECT LIST PRESENT, READ IT * */
    /* ----------------------------------- */
    
    if (pso->generic.select_list != (CHAR *)0)
        {
        if (IS_STRING (pso->generic.select_list))
            {
            if (sl_read_list (SEISMIC_DATA_TYPE, pso->generic.select_list,
                              &pso->select_list) EQUALS SUCCESS)
                {
                slist = pso->select_list;
                }
            }
        }

    /* * INTERSECT WITH MAP WINDOW AND SELECT PHRASE * */
    /* ----------------------------------------------- */
    
    xmin = pmap->lower_left_xy.x;
    ymin = pmap->lower_left_xy.y;
    ct_project_xy (xmin, ymin, &xmin, &ymin);
    
    xmax = pmap->upper_right_xy.x;
    ymax = pmap->upper_right_xy.y;
    ct_project_xy (xmax, ymax, &xmax, &ymax);
    
    /* get new select list */
    sprintf (text, 
    "UPPER_RIGHT_X >= %.2f AND \
     UPPER_RIGHT_Y >= %.2f AND \
     LOWER_LEFT_X  <= %.2f AND \
     LOWER_LEFT_Y  <= %.2f",
    xmin, ymin, xmax, ymax);
    
    if (IS_STRING (pso->generic.select_by_phrase))
        {
        len = strlen (pso->generic.select_by_phrase);
        select_phrases = (CHAR *)TALLOC (len + strlen (text) + 10);
        sprintf (select_phrases, "%s AND (%s)",
                 pso->generic.select_by_phrase, text);
        free_phrase = TRUE;
        }
    else
        {
        select_phrases = text;
        }

    status = sl_select (slist, "SEIS_LINE_HDR", select_phrases, &process_list,
                        &n_lines);

#if NEED_TO_FREE
    if (free_phrase)
        {
        tc_free (select_phrases);
        }
#endif
    
    /* * CHECK FOR GOOD SELECT * */
    /* ------------------------- */
    
    if (status != SUCCESS OR n_lines == 0)
        {
        if (slist != (NLIST_HEADER)0)
            {
            nl_free_nlist (slist);
            }

        if (process_list IS_NOT_EQUAL_TO (NLIST_HEADER)0)
            {
            nl_free_nlist (process_list);
            }

        return status;
        }

    /* * MISTIE BTREE SET UP * */
    /* ----------------------- */
    
    if (pso->ov_type == SO_SEISMIC_MISTIE)
        {
        btinit (&pso->seismic_mistie.mistree, 0, sizeof pso->current_line.id,
                sizeof (SO_TIE_STRUCT *), 0);
        }

    /* * IF DEPTH, LOAD VELOCITY GRID * */
    /* -------------------------------- */
    
    if (pso->ov_type EQUALS SO_SEISMIC_DEPTH OR pso->ov_type EQUALS SO_SEISMIC_ISOPACH)
        {
        so_load_grid (pso);
        }

    /* * NLIST SETUP * */
    /* --------------- */
    
    /* NOTE: since the select list stuff insists
       on having the first column be a fixed size
       (sizeof SELECT_KEY_STRING), rather than the
       native size (sizeof SEIS_LINE_NAME), we are
       forced to retrieve the value into a local
       variable and then move it into the current
       line name.  This is because the n-List
       system does a BLOCKMOVE of the defined size
       rather than a string copy JGG           */
    
    nalloc = &pick_seis[0]->nallocated;
    nused = &pick_seis[0]->nused;
    
    ndim = 1;
    dim_list[0] = 1, value_list[0] = (VOIDPTR) & select_line_id;
    
    pso->select_list = process_list;
    nl_first_point (process_list, ndim, dim_list, value_list);

    for (iline = 0; iline < n_lines; iline++)
        {
        se_get_line   (select_line_id, pso->current_line.line_name);
        se_get_survey (select_line_id, pso->current_line.survey);
        pso->current_line.id = 0;
        
        status = so_process_line (pso, TRUE, &segment_id);
        if (status == SUCCESS)
            {

            /* allocate elements inside pick structure if necessary */

            if (*nalloc == 0)
                {
                /* as in nl_init                   */

                pick_seis[0]->segment_id = (INT *)tc_zalloc
                                           (blocksize * sizeof(INT));

                pick_seis[0]->line_name = (FULL_LINE_NAME *)tc_alloc
                                          (blocksize * sizeof(FULL_LINE_NAME));

                *nalloc = blocksize;
                }
            else if (*nused == *nalloc)
                {
                *nalloc += blocksize;

                pick_seis[0]->segment_id =
                    (INT *)tc_realloc (pick_seis[0]->segment_id,
                                       *nalloc * sizeof(INT));

                pick_seis[0]->line_name =
                    (FULL_LINE_NAME *)tc_realloc (pick_seis[0]->line_name,
                                                  *nalloc *
                                                      sizeof(FULL_LINE_NAME));
                }

            pick_seis[0]->segment_id[*nused] = segment_id;
            pick_seis[0]->line_name[*nused] = pso->current_line;

            /*
             * strcpy((char *)(pick_seis[0]->line_name + *nused),
             *   (char *)pso->current_line.line_name);
             */

            *nused += 1;
            
            /* TO STOP THE GRAPHICS OVERFLOW IN GKS, WE CALL WM_UPDATE
               (OUR CURRENT "DEFER EVERYTHING 'TIL DONE" MODE OF OPERATION
                REQUIRES THIS FOR BIG SURVEYS wi' lots o' lines) */

            displayed_lines++;

            if ((displayed_lines % 25) EQUALS 0)
                {
                wm_update ();
                }
            }

        nl_free_nlist (pso->cdp_xy);
                nl_free_nlist(pso->sym_xy);
        
        /* free horizon nlists */
        
        if ((pso->ov_type == SO_SEISMIC_ISOPACH OR
             pso->ov_type == SO_SEISMIC_ISOCHRON) AND
            pso->seismic_generic.horizons_ok)
            {
            nl_free_nlist (pso->seismic_interval.base_horizon.nlist);
            nl_free_nlist (pso->seismic_interval.top_horizon.nlist);
            }
        else if (pso->ov_type != SO_SEISMIC_MISTIE AND
                 pso->seismic_generic.horizons_ok)
            {
            nl_free_nlist (pso->seismic_surface.nlist);
            }

        nl_free_nlist (pso->disp_opt.shot_label_nlist);
        
        pso->disp_opt.shot_label_nlist = pso->cdp_xy = (NLIST_HEADER)0;

        pso->seismic_interval.base_horizon.nlist    =
            pso->seismic_interval.top_horizon.nlist =
            pso->seismic_surface.nlist              = (NLIST_HEADER)0;
        
        nl_next_point (process_list, ndim, dim_list, value_list);
        }

    if ((displayed_lines % 25) != 0)
        {
        wm_update ();
        }

    /* * DRAW MISTIE MAP IF NECESSARY * */
    /* -------------------------------- */
    
    if (pso->ov_type == SO_SEISMIC_MISTIE)
        {
        so_process_mistie (pso);
        }

    status = SUCCESS;
    if (slist != (NLIST_HEADER)0 AND slist != pso->select_list)
        {
        nl_free_nlist (slist);
        }

    /* for software graphic picking to work this n-list MUST stay around */

#if 0
    nl_free_nlist (pso->select_list);
#endif
    
    /* end mod for software picking */
    
    switch (pso->ov_type)
        {
    case SO_SEISMIC_ISOPACH:
    case SO_SEISMIC_DEPTH:
        am_free (pso->grid.array);
        break;
        
    case SO_SEISMIC:
    case SO_SEISMIC_DATA:
    default:
        break;
        }
    return status;
    }
/* END:     */
