/* DEC/CMS REPLACEMENT HISTORY, Element CO_SET_FAULTS.C*/
/* *4    15-OCT-1990 16:48:40 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/* *3     2-AUG-1990 15:33:31 GILLESPIE "(SPR 5735) Removed uninitialized Col_List (Dim_list instead of d_list in orig. code)"*/
/* *2    20-JUN-1990 09:53:27 JULIAN "(SPR 0) replace ct_get_faults with se_get_faults"*/
/* *1    19-JUN-1989 12:44:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element CO_SET_FAULTS.C*/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Exploration Systems, Inc.
            579 Market Street
            San Francisco, CA  USA 94105
            (415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/


/* * FILE DESCRIPTION *                                                             */
/* --------------------                                                             */
/* ************************************************************************

   NAME: CO_SET_FAULTS.C

   AUTHOR: JOHN CONROY

   DESCRIPTION: Function to send the fault data to DGI for faulted gridding.

   ************************************************************************ */

/* * INCLUDE FILES *                                                                */
/* -----------------                                                                */

#include "esi_c_lib.h"

#ifndef ESI_CO_H
#include "esi_co.h"
#endif

#ifndef ESI_CO_MSG_H
#include "esi_co_msg.h"
#endif

#ifndef ESI_CO_ERR_H
#include "esi_co_err.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
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

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_NL_METH_HDR_H
#include "esi_nl_meth_hdr.h"
#endif

/* * FUNCTION TITLE *                                                               */
/* ------------------                                                               */

#if USE_PROTOTYPES
publicdef INT co_set_faults(CONTOURS *contours, INT mode)
#else
publicdef INT co_set_faults(contours, mode)
CONTOURS *contours;                 /* CONTOUR OVERLAY INFORMATION STRUCT           */
INT mode;                           /* GRIDDING OR CONTOURING                       */
#endif

/* * VARIABLE LIST *                                                                */
/* -----------------                                                                */
    {
    INT status;                     /* RETURN STATUS                                */
    NLIST_HEADER faults = (NLIST_HEADER)0;  /* NLIST OF FAULTS                      */
    NLIST_HEADER temp_faults = (NLIST_HEADER)0; /* NLIST OF FAULTS                  */
    FLOAT *x = (FLOAT *)0;          /* X ARRAY FOR FAULTS                           */
    FLOAT *y = (FLOAT *)0;          /* Y ARRAY FOR FAULTS                           */
    UINT ntables;                   /* NUMBER OF TABLES IN NLIST                    */
    UINT nrows;                     /* NUMBER OF ROWS IN NLIST                      */
    INT table_size;                 /* NUMBER OF ROWS IN TABLE                      */
    INT array_size;                 /* ALLOCATED SIZE OF X AND Y                    */
    INT max_fault_rows = 2500;      /* MAX NUMBER OF FAULT ROWS (DGI)               */
    UINT i;                         /* DUMMY INDEX                                  */
    UINT j;                         /* DUMMY INDEX                                  */
    UINT dummy;                     /* DUMMY INTEGER                                */
    UINT Col_List[2];               /* DIM LIST FOR NLIST                           */
    VOIDPTR Var_List[2];            /* VALUE LIST FOR NLIST                         */
    FLOAT xmin, ymin, scale_factor;
    MAP_STRUCTURE *map;
    NLM_CLIP_HEADER clip;
    PROJECTION_ID project_projection;
    PROJECTION_STRUCTURE *project_data, *map_data;
    
    /* **********************************************************************       */
    
    status = SUCCESS;
    
    /* * GET THE FAULTS NLIST *                                                     */
    /* ------------------------                                                     */
    
    status = se_get_faults(contours->faulted, contours->fault_source, &faults);
    if (status < 0)
        {
        goto finished;
        }
    /*  Get the map corners                                                         */
    
    status = mp_get_current_map(&map);
    
    /* transform nlist to map projection                                            */
    
    qs_inq_c(QS_PROJECT_PROJECTION, project_projection, 0);
    ct_get_projection(project_projection, &project_data);
    ct_get_projection(map->map_projection, &map_data);
    clip.Col_List[0] = Col_List[0] = 1;
    clip.Col_List[1] = Col_List[1] = 2;
    status = ct_transform_nlist(faults, project_data, &temp_faults, map_data, Col_List,
                                0.0);
    if (status == SUCCESS)
        {
        nl_free_nlist(faults);
        faults = temp_faults;
        }
    /*  Setup the clipping window                                                   */
    
    clip.minList[0] = map->lower_left_xy.x;
    clip.minList[1] = map->lower_left_xy.y;
    clip.maxList[0] = map->upper_right_xy.x;
    clip.maxList[1] = map->upper_right_xy.y;
    
    /*  Clip the nlist                                                              */
    
    status = nl_invoke_nlist_method(faults, NL_CLIP_METHOD, &clip);
    
    if (status == SUCCESS)
        {
        nl_free_nlist(faults);
        faults = clip.outNlist;
        }
    /* * GET NUMBER OF FAULT ROWS *                                                 */
    /* ------------------------------                                               */
    
    nl_inq_nlist_int(faults, NL_NLIST_NROWS, (INT *) & nrows);
    if (nrows > max_fault_rows)
        {
        status = CO_TOO_MANY_FAULT_POINTS;
        goto finished;
        }
    else if (nrows EQUALS 0)
        {
        status = CO_NO_FAULT_DATA;
        goto finished;
        }
    /* * GET NUMBER OF TABLES *                                                     */
    /* -------------------------                                                    */
    
    nl_inq_nlist_int(faults, NL_NLIST_NTABLES, (INT *) & ntables);
    
    /* * ALLOCATE X AND Y ARRAYS *                                                  */
    /* ---------------------------                                                  */
    
    array_size = nrows / ntables * 2;
    x = (FLOAT *)tc_alloc(array_size * sizeof(FLOAT));
    y = (FLOAT *)tc_alloc(array_size * sizeof(FLOAT));
    Var_List[0] = (VOIDPTR)x;
    Var_List[1] = (VOIDPTR)y;
    
    if (mode EQUALS 1)
        {
        qs_inq_f(QS_DGI_XMIN, &xmin);
        qs_inq_f(QS_DGI_YMIN, &ymin);
        qs_inq_f(QS_DGI_SCALE_FACTOR, &scale_factor);
        }
    /* * SEND FAULT DATA TO DGI ON TABLE BY TABLE BASIS *                           */
    /* ----------------------------------------------------                         */
    
    for (i = 1; i <= ntables; i++)
        {
        
        /* INSURE SIZE OF X AND Y                                                   */
        
        nl_inq_table_int(faults, i, NL_TABLE_NROWS, &table_size);
        if (table_size > array_size)
            {
            tc_free(x);
            tc_free(y);
            array_size = table_size;
            x = (FLOAT *)tc_alloc(array_size * sizeof(FLOAT));
            y = (FLOAT *)tc_alloc(array_size * sizeof(FLOAT));
            Var_List[0] = (VOIDPTR)x;
            Var_List[1] = (VOIDPTR)y;
            }
        /* GET TABLE ROWS                                                           */
        
        nl_set_current_row(faults, 1, i);
        nl_get_rows(faults, table_size, 2, Col_List, Var_List, &dummy);
        
        /* SEND TO DGI                                                              */
        
        if (mode EQUALS 0)
            {
            co_defgfl_gc(i, x, y, table_size);
            }
        else if (mode EQUALS 1)
            {
            for (j = 0; j < table_size; j++)
                {
                x[j] = (x[j] - xmin) * scale_factor;
                y[j] = (y[j] - ymin) * scale_factor;
                }
            co_defflt_gc(i, x, y, table_size);
            }
        }
    /* * CLEAN UP *                                                                 */
    /* ------------                                                                 */
    
finished:
    tc_free(x);
    tc_free(y);
    nl_free_nlist(faults);
    return status;
    }
