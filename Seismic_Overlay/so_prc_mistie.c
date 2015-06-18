/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_MISTIE.C*/
/* *11   30-JUL-1991 14:25:51 JESSIE "(SPR 0) fix coredump in seis mistie overlay case#3473"*/
/* *10   25-SEP-1990 09:38:24 GILLESPIE "(SPR 1) Merge ernie deltas"*/
/*  7B1  24-SEP-1990 17:25:14 GILLESPIE "Beta Deltas"*/
/* *9    29-AUG-1990 11:38:00 GILLESPIE "(SPR 1) CP Deltas"*/
/* *8    27-AUG-1990 14:09:52 JULIAN "(SPR 0) bug fixes from ernie"*/
/* *7     1-AUG-1990 13:19:01 VINCE "(SPR 5615) updated for integer line id"*/
/* *6    26-JUL-1990 17:22:07 GILLESPIE "(SPR 1) Lint run; incorporate changes for horizon end point posting"*/
/* *5    25-JUL-1990 20:05:59 VINCE "(SPR 5644) Header Standardization"*/
/* *4    29-JUN-1990 19:04:28 GILLESPIE "(SPR 1) Changes for fault posting; removal of underbrush"*/
/* *3    17-JAN-1990 15:40:11 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2    25-JUL-1989 20:07:40 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:27:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SO_PRC_MISTIE.C*/

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

#include "esi_so.h"

#ifndef ESI_C_LIB_H
#include "esi_c_lib.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_BT_H
#include "esi_bt.h"
#endif

#ifndef ESI_SE_H
#include "esi_se.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_LL_H
#include "esi_ll.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    Function to post the mistie information.

Prototype:
    publicdef INT so_process_mistie(SO_STRUCT *pso);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pso             -(SO_STRUCT *) Seismic overlay structure.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT so_process_mistie (SO_STRUCT *pso)
#else
publicdef INT so_process_mistie (pso)
SO_STRUCT *pso;
#endif
    {
    INT status;
    INT segment;    
    UINT dim_list[3];               /* local nlist stuff */
    VOIDPTR value_list[3];
    INT value_inc[3];
    size_t data_sizes[3];
    INT data_types[3];
    INT units[3];
    UINT stroke;
    INT nlines;
    BOOL line_found;
    INT i, j;
    CDP first_cdp;
    CDP last_cdp;
    INT ninter;
    DOUBLE zvalue;
    CHAR label[40];
    
    NLIST_HEADER process_list;      /* nlist of datum lines */
    LISTHEAD tie_list;              /* linked list of intersection structure */
    SEIS_TIE *pseis_tie;            /* line intersection structure */
    NLIST_HEADER mistie_nlist;      /* nlist of mistie datum lines */
    SO_TIE_STRUCT *ptie;            /* SO_TIE_STRUCT */
    SO_TIE_STRUCT *potie;           /* SO_TIE_STRUCT tie line */
    static CHAR *c_name[4] = 
        { "LINE_ID", "FIRST_CDP", "LAST_CDP", NULL};
    
    status = SUCCESS;
    
    /* set up text attributes */
    
    pso->seismic_mistie.tie_label.text_offset.x =
        pso->seismic_mistie.tie_label.text_offset.y =
        1.0 * pso->seismic_generic.shotpoint_symbol.symbol_size;

    pso->seismic_mistie.tie_label.box_flag = FALSE;
    so_angpp_to_text (&pso->seismic_mistie.tie_label, 90.0, 1);
    
    /* if exists read mistie line list nlist */
    
    if (IS_STRING (pso->seismic_mistie.datum_line_list))
        {
        status = sl_read_list (SEISMIC_DATA_TYPE,
                               pso->seismic_mistie.datum_line_list,
                               &mistie_nlist);
        if (status == SUCCESS)
            {
            process_list = mistie_nlist;
            }
        else
            {
            process_list = pso->select_list;
            }
        }
    else                            /* build the list */
        {
        process_list = pso->select_list;
        }

    /* set up n-List control arrays. */

    data_sizes[0] = data_sizes[1] = data_sizes[2] = sizeof(INT);
    data_types[0] = data_types[1] = data_types[2] = INT_TYPE;
    value_inc[0]  = value_inc[1]  = value_inc[2]  = VALUE_LIST;
    units[0]      = units[1]      = units[2]      = 0;
    
    nl_init_nlist (&mistie_nlist, 3, units, data_sizes, data_types,
                   value_inc, 0);
    
    dim_list[0] = 1;
    value_list[0] = (VOIDPTR) &pso->current_line.id;
    
    dim_list[1] = 2;
    value_list[1] = (VOIDPTR) &first_cdp;
    
    dim_list[2] = 3;
    value_list[2] = (VOIDPTR) &last_cdp;
    
    status = nl_set_nlist_info (mistie_nlist, NL_NLIST_COLUMN_NAME, 3,
                                dim_list, (VOIDPTR)c_name);
    if (status != SUCCESS)
        {
        return status;
        }

    nl_start_stroke  (mistie_nlist, &stroke);
    nl_inq_nlist_int (process_list, NL_NLIST_NPOINTS, &nlines);
    nl_first_point   (process_list, 1, dim_list, value_list);
    
    /* check each line */
    for (i = 0; i < nlines; i++)
        {
        
        /* check for existence of horizon data */
        
        line_found = btfind (&pso->seismic_mistie.mistree,
                             &pso->current_line.id, &ptie);
        if (NOT line_found)
            {
            nl_next_point (process_list, 1, dim_list, value_list);
            continue;
            }

        /* see if line goes in default direction */
        if (process_list == pso->select_list)
            {
            if (strcmp (ptie->north_or_south, pso->seismic_mistie.default_direction))
                {
                nl_next_point (process_list, 1, dim_list, value_list);
                continue;
                }
            }

        first_cdp = ptie->first_cdp;
        last_cdp = ptie->last_cdp;
        nl_add_points (mistie_nlist, 1, 3, dim_list, value_list);
        nl_next_point (process_list, 1, dim_list, value_list);
        }

    /* now process the list */

    nl_inq_nlist_int (mistie_nlist, NL_NLIST_NPOINTS, &nlines);
    nl_first_point   (mistie_nlist, 3, dim_list, value_list);
    
    wmsallocate(&segment);

    for (i = 0; i < nlines; i++)
        {
        
        line_found = btfind (&pso->seismic_mistie.mistree,
                             &pso->current_line.id, &ptie);
        /* get intersection list */
        status = se_getinters (&pso->current_line.id, first_cdp, last_cdp,
                               &tie_list);
        if (status != SUCCESS)
            {
            llfree (&tie_list);
            nl_next_point (mistie_nlist, 3, dim_list, value_list);
            continue;
            }

        ninter = llcount (&tie_list);
        
        if (ninter == 0)
            {
            llfree (&tie_list);
            nl_next_point (mistie_nlist, 3, dim_list, value_list);
            continue;
            }

        pso->seismic_interval.top_horizon.nlist     = ptie->nlist;
	pso->seismic_interval.top_horizon.line.id   = pso->current_line.id;
        pso->seismic_interval.top_horizon.first_cdp = ptie->first_cdp;
        pso->seismic_interval.top_horizon.last_cdp  = ptie->last_cdp;
        pso->seismic_interval.top_horizon.cdp_inc   = ptie->cdp_inc;
        
        llfirst (&tie_list, &pseis_tie);
        

        for (j = 0; j < ninter; j++)
            {
            
            line_found = btfind (&pso->seismic_mistie.mistree,
                                 &pseis_tie->tie.id, &potie);

            if (NOT line_found)
                {
                am_free (pseis_tie);
                llnext (&tie_list, &pseis_tie);
                continue;
                }

            /* set up the nlists for so_zvalue call. set up like an isochron */
            
            pso->seismic_interval.base_horizon.nlist     = potie->nlist;
	    pso->seismic_interval.base_horizon.line.id	 = pseis_tie->tie.id;
            pso->seismic_interval.base_horizon.first_cdp = potie->first_cdp;
            pso->seismic_interval.base_horizon.last_cdp  = potie->last_cdp;
            pso->seismic_interval.base_horizon.cdp_inc   = potie->cdp_inc;
            
            so_zvalue (pso, (CDP)pseis_tie->this_cdp, (CDP)pseis_tie->tie_cdp,
                       pseis_tie->tie_x, pseis_tie->tie_y, &zvalue);
            
            if (zvalue == SO_NULL)
                {
                strcpy (label, "ND");
                }
            else
                {
                sprintf (label, "%.0f", zvalue);
                }

            /* set up text */
            
            pso->seismic_mistie.tie_label.text_position.x = pseis_tie->tie_x;
            pso->seismic_mistie.tie_label.text_position.y = pseis_tie->tie_y;
            pso->seismic_mistie.tie_label.text_string     = label;
            
            /* draw it */
            
            mm_draw_text (&pso->seismic_mistie.tie_label, FALSE);
            am_free (pseis_tie);
            llnext (&tie_list, &pseis_tie);
            }

        llfree (&tie_list);
        nl_next_point (mistie_nlist, 3, dim_list, value_list);
        }

    wmsclose(segment);

    /* free all tie line stuff */
    
    nlines = btcount (&pso->seismic_mistie.mistree);
    
    if (nlines > 0)
        {
        btfirst (&pso->seismic_mistie.mistree, &pso->current_line.id, &ptie);
        
        for (i = 0; i < nlines; i++)
            {
            if (ptie->nlist != (NLIST_HEADER)0)
                {
                nl_free_nlist (ptie->nlist);
                }

            am_free (ptie);
            btnext (&pso->seismic_mistie.mistree, &pso->current_line.id, &ptie);
            }
        }
    btfree (&pso->seismic_mistie.mistree);
    
    status = SUCCESS;
    return status;
    }
/* END:     */
