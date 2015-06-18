/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_SEIS.C*/
/* *14   16-AUG-1991 14:14:55 JTM "(SPR 0) Fixed call to non-existent function."*/
/* *13   16-AUG-1991 11:37:20 JTM "(SPR 0) Cast arg to wm_gslwsc() to DOUBLE."*/
/* *12   17-JAN-1991 10:46:07 JESSIE "(SPR 6399) Add ct_transform_nlist to conver list point if the projection is different from*/
/*map's"*/
/* *11   17-AUG-1990 21:49:02 VINCE "(SPR 5644) Code Cleanup"*/
/* *10    1-APR-1990 12:20:13 GILLESPIE "(SPR 5176) Change nl_get_rows to nl_get_rows_convert"*/
/* *9    30-MAR-1990 13:36:43 GILLESPIE "(SPR 1) Add more checks for Out of Memory"*/
/* *8    14-FEB-1990 13:17:46 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *7     2-NOV-1989 13:56:42 JULIAN "(SPR 30) completed last mod"*/
/* *6    31-OCT-1989 10:50:48 JULIAN "(SPR 5039) change sl files to se files"*/
/* *5    30-OCT-1989 14:35:39 JULIAN "(SPR 5039) Seismic Select List Mods"*/
/* *4    18-OCT-1989 10:34:13 JULIAN "(SPR 0) mod param passing"*/
/* *3    11-AUG-1989 20:18:48 CONROY "(SPR 0) Install revised seismic selects & picking"*/
/* *2    25-JUL-1989 19:39:36 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 12:54:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HL_HI_SEIS.C*/
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

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_WM_CLS_H
#include "esi_wm_cls.h"
#endif

#ifndef ESI_HL_MSG_H
#include "esi_hl_msg.h"
#endif

#ifndef ESI_HL_ERR_H
#include "esi_hl_err.h"
#endif

#ifndef ESI_MG_H
#include "esi_mg.h"
#endif

#ifndef ESI_NL_H
#include "esi_nl.h"
#endif

#ifndef ESI_SE_H
#include "esi_se.h"
#endif

#ifndef ESI_SL_H
#include "esi_sl.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#include "esi_tc_err.h"
#endif

#ifndef ESI_CT_H
#include "esi_ct.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_QS_H
#include "esi_qs.h"
#endif

#include "esi_nl_meth_hdr.h"

#if USE_PROTOTYPES
static INT hl_nl_extract (FULL_LINE_NAME *line, NLIST_HEADER nlist, INT index_dim,
                              INT from, INT to, NLIST_HEADER *outNlist);
static INT hl_load_list (FULL_LINE_NAME *line, NLIST_HEADER *plist, FLOAT *i_cdp,
                             FLOAT *f_cdp, INT *entire);

#else
static INT hl_nl_extract ();
static INT hl_load_list ();

#endif

/* File Description ---------------------------------------------------------
Overview:

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT hl_hilite_seis(SELECT_STRUCT *select_struct,
        NLIST_HEADER *select_list);

Private_Functions:
    static INT hl_nl_extract(FULL_LINE_NAME *line, NLIST_HEADER nlist, 
        INT index_dim, INT from, INT to, NLIST_HEADER *outNlist);

    static INT hl_load_list(FULL_LINE_NAME *line, NLIST_HEADER *plist,
        FLOAT *i_cdp, FLOAT *f_cdp, INT *entire);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To highlight a list of seismic lines.

Prototype:
    publicdef INT hl_hilite_seis(SELECT_STRUCT *select_struct,
        NLIST_HEADER *select_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) select_struct   -(SELECT_STRUCT *) The user structure of select
                            list.
    (I) select_list     -(NLIST_HEADER ) A select list of seismic lines.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL - Unsuccessful completion.
    TC_ERR_NO_MEMORY
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT hl_hilite_seis (SELECT_STRUCT *select_struct, NLIST_HEADER *select_list)
#else
publicdef INT hl_hilite_seis (select_struct, select_list)
SELECT_STRUCT *select_struct;
NLIST_HEADER *select_list;
#endif
    {
    INT status, entire;
    GKS_INT plType, plColi;
    GKS_REAL plWidth;
    INT appFrom, winFrom, tnrFrom, groFrom;
    INT appTo, hilite_overlay_group, segTo, picTo;
    INT well_hilite_group, seismic_picking_group, seismic_hilite_group;
    INT segment, wmStatus, i;
    INT new_group;                  /* the new group of hilited lines */
    INT index_dim = 1, hori_dim = 2, vert_dim = 3;
    FLOAT from, to, temp;
    GKS_REAL hwidth;
    NLIST_HEADER plist, rplist;
    NLM_CLIP_HEADER clip;
    CHAR msg[STMAX], hilite_name[STMAX];
    SELECT_KEY_STRING *listPtr;
    SEIS_LINE_ID *line_id;
    INT *cdp1, *cdp2;
    UINT dimList[3];
    UINT n_lines;
    VOIDPTR valueList[3];
    FULL_LINE_NAME seis_line;
    SEIS_LINE_NAME linename;
    MAP_STRUCTURE *map;
    UINT dlist[2];
    UINT Col_List[2];
    PROJECTION_ID project_projection;
    PROJECTION_STRUCTURE *project_data, *map_data;

    /* *********************************************************************** */
    /* retrieve the lines first.        */
    status = nl_inq_nlist_int (*select_list, NL_NLIST_NROWS, (INT *)(&n_lines));
    if (status < 0)
        {
        return status;
        }
    if (n_lines <= 0)
        {
        return FAIL;
        }
    if (!select_struct->window_id)
        {
        return FAIL;
        }
    /* allocates the memory for retrieval.*/
    
    cdp1 = (INT *)tc_alloc (sizeof(INT) * n_lines);
    if (cdp1 EQUALS (INT *)0)
        {
        return TC_ERR_NO_MEMORY;
        }
    cdp2 = (INT *)tc_alloc (sizeof(INT) * n_lines);
    if (cdp2 EQUALS (INT *)0)
        {
        return TC_ERR_NO_MEMORY;
        }
    line_id = (SEIS_LINE_ID *)tc_alloc (sizeof(SEIS_LINE_ID) * n_lines);
    if (line_id EQUALS (INT *)0)
        {
        return TC_ERR_NO_MEMORY;
        }
    dimList[0] = 1;
    dimList[1] = 2;
    dimList[2] = 3;
    valueList[0] = (VOIDPTR)line_id;
    valueList[1] = (VOIDPTR)cdp1;
    valueList[2] = (VOIDPTR)cdp2;
    nl_set_current_row (*select_list, 1, 1);
    if ((status = nl_get_rows (*select_list, n_lines, 3, dimList, valueList,
                               &n_lines)) < 0)
        {
        goto free_memory;
        }
    wmowhere (&appFrom, &winFrom, &tnrFrom, &groFrom, &wmStatus);
    if ((status = hl_query_hilite_overlay (appFrom, select_struct->window_id,
                                           &hilite_overlay_group, &well_hilite_group,
                                           &seismic_picking_group,
                                           &seismic_hilite_group)))
        {
        goto free_memory;
        }
    /* The following attach assummes that all windows are associated
       with the same application -- G. Binns has this assumption is several
       parts of his code */
    
    wmoattach (appFrom, select_struct->window_id, select_struct->transform_id,
               seismic_hilite_group, &wmStatus);
    
    /* Create a new group for this batch of seismic hilites */
    
    wmgallocate (&new_group);
    sprintf (hilite_name, mg_message (HL_LINES_HILITE), select_struct->name);
    wmoset (new_group, WM_CLASS_SEISMIC_HILITE, "", hilite_name);
    
    wm_gsplci (select_struct->color);
    plWidth = select_struct->width;
    wm_gslwsc ((DOUBLE)plWidth);
    
    for (i = 0; i < n_lines; i++)
        {
        se_get_line (line_id[i], linename);
        strcpy (seis_line.line_name, linename);
        se_get_survey (line_id[i], seis_line.survey);
        seis_line.id = 0;
        from = cdp1[i];
        to = cdp2[i];
        if (from > to)
            {
            temp = from;
            from = to;
            to = temp;
            }
        /* read each list */
        
        if (hl_load_list (&seis_line, &plist, &from, &to, &entire))
            {
            sprintf (msg, mg_message (HL_CANT_HILITE), linename);
            }
        else
            {
            wmsallocate (&segment);
            wmoset (segment, WM_CLASS_SEISMIC_HILITE, linename, linename);
            if (NOT entire)
                {
                status = hl_nl_extract (&seis_line, plist, index_dim, (INT)from,
                                        (INT)to, &rplist);
                nl_free_nlist (plist);
                if (status < 0)
                    {
                    continue;
                    }
                plist = rplist;
                
                }
            /* add the hilite on this line (ptr to dbkey is listPtr),
               to the current group of seismic hilites */
            
            /* Clip to map window */
            
            mp_front_map (&map);
            /* transform nlist to map projection */

            qs_inq_c(QS_PROJECT_PROJECTION,project_projection, 0);
	    ct_get_projection(project_projection, &project_data);
	    ct_get_projection(map->map_projection, &map_data);

	    clip.Col_List[0] = Col_List[0] = 2;
	    clip.Col_List[1] = Col_List[1] = 3;

	     status = ct_transform_nlist(plist, project_data, &rplist,
	     map_data, Col_List, 0.0);

	     if (status == SUCCESS)
             	{
		nl_free_nlist(plist);
		plist = rplist;
		}

            clip.minList[0] = map->lower_left_xy.x;
            clip.minList[1] = map->lower_left_xy.y;
            clip.maxList[0] = map->upper_right_xy.x;
            clip.maxList[1] = map->upper_right_xy.y;
            status = nl_invoke_nlist_method (plist, NL_CLIP_METHOD, &clip);
            nl_free_nlist (plist);
            if (status < 0)
                {
                continue;
                }
            plist = clip.outNlist;
            
            /* Draw result */
            nl_draw_nlist (plist, 2, 3);
            nl_free_nlist (plist);
            }
        wmsclose (segment);
        }
    wmgclose (new_group);
    
    wmoattach (appFrom, winFrom, tnrFrom, groFrom, &wmStatus);
    status = SUCCESS;
    
free_memory:
    tc_free (cdp1);
    tc_free (cdp2);
    tc_free (line_id);
    return(status);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    An internal help routine of HL_HILITE_SEIS to extract part
    of seismic line, according to the given highlighting range.

Prototype:
    static INT hl_nl_extract(FULL_LINE_NAME *line, NLIST_HEADER nlist, INT index_dim, 
        INT from, INT to, NLIST_HEADER *outNlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) line        -(FULL_LINE_NAME *) The name structure of given seismic line.
    (I) nlist       -(NLIST_HEADER *) The pointer of n-List.
    (I) index_dim   -(INT) The index dimension.
    (I) from        -(INT) The begin value.
    (I) to          -(INT) The end value.
    (O) outNlist    -(NLIST *) The pointer of returned n-List.

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <hl_hi_seis.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT hl_nl_extract (FULL_LINE_NAME *line, NLIST_HEADER nlist, INT index_dim,
                              INT from, INT to, NLIST_HEADER *outNlist)
#else
static INT hl_nl_extract (line, nlist, index_dim, from, to, outNlist)
FULL_LINE_NAME *line;
NLIST_HEADER nlist;
INT index_dim;
INT from;
INT to;
NLIST_HEADER *outNlist;
#endif
    {
    INT status = SUCCESS;
    
    INT i, j, endPointIndex, startPointIndex;
    INT *cdps, find_from = FALSE, find_to = FALSE, new_cdp;
    DOUBLE intrp_x1, intrp_y1, intrp_x2, intrp_y2;
    UINT totalrows;
    UINT dimList[3];
    VOIDPTR valueList[3];
    INT units[3];
    size_t d_size[3];
    INT d_type[3];
    
    /***************************************************************************/
    
    units[0] = units[1] = units[2] = 0;
    d_size[0] = sizeof(INT);
    d_size[1] = d_size[2] = sizeof(DOUBLE);
    d_type[0] = INT_TYPE;
    d_type[1] = d_type[2] = FLOAT_TYPE;
    
    /* copy the whole list.      */
    
    status = nl_copy_nlist (nlist, outNlist);
    if (status)
        {
        return status;
        }
    status = nl_set_current_row (nlist, 1, 1);
    if (status < 0)
        {
        return status;
        }
    /* retrieves all existing CDPs.       */
    
    nl_inq_table_int (nlist, 1, NL_TABLE_NROWS, (INT *)(&totalrows));
    cdps = (INT *)tc_alloc (sizeof(INT) * totalrows);
    valueList[0] = (VOIDPTR)cdps;
    dimList[0] = 1, dimList[1] = 2, dimList[2] = 3;
    status = nl_get_rows_convert (nlist, totalrows, 1, dimList, valueList, units,
                                  d_size, d_type, &totalrows);
    if (status < 0)
        {
        return status;
        }
    /* find the range for the given */
    /* first cdp.           */
    for (i = 0, startPointIndex = 0; i < totalrows AND (NOT find_from);
         i++, startPointIndex++)
        {
        /* looking for the first cdp, which*/
        /* is greater than FROM.           */
        if (from > cdps[i])
            {
            continue;
            }
        find_from = 1;
        if (from < cdps[i])
            {
            se_xy_from_cdp (line, (CDP)from, -1.0, &intrp_x1, &intrp_y1);
            }
        else
            {
            find_from = 2;
            }
        }
    /* if the FROM is greater than  */
    /* all cdps, then returns FAIL. */
    if (find_from EQUALS FALSE)
        {
        nl_free_nlist (*outNlist);
        tc_free (cdps);
        return FAIL;
        }
    /* find the range for the given */
    /* second cdp.          */
    for (i = 0, endPointIndex = 0; i < totalrows AND (!find_to); i++, endPointIndex++)
        {
        /* looking for the first cdp, which*/
        /* is greater than TO.           */
        if (to > cdps[i])
            {
            continue;
            }
        find_to = 1;
        if (to < cdps[i])
            {
            se_xy_from_cdp (line, (CDP)to, -1.0, &intrp_x2, &intrp_y2);
            }
        else
            {
            find_to = 2;
            }
        }
    /* if the start row EQUALS end */
    /* row return                  */
    
    if (startPointIndex EQUALS endPointIndex)
        {
        nl_free_nlist (*outNlist);
        tc_free (cdps);
        return FAIL;
        }
    /* find the range for the given */
    /* If can not find the TO cdp.   */
    /* set the TO cdp to be the last cdp*/
    
    if (find_to EQUALS FALSE)
        {
        find_to = 2;
        }
    /* if the TO is not the existing */
    /* delete those rows after the */
    /* endPointIndex, includes the   */
    /* endPointIndex.            */
    
    if ((find_to EQUALS 1) AND (endPointIndex IS_NOT_EQUAL_TO totalrows))
        {
        nl_set_current_row (*outNlist, endPointIndex, 1);
        nl_delete_rows (*outNlist, (totalrows - endPointIndex + 1));
        new_cdp = to;
        valueList[0] = (VOIDPTR) & new_cdp;
        valueList[1] = (VOIDPTR) & intrp_x2;
        valueList[2] = (VOIDPTR) & intrp_y2;
        nl_add_rows_convert (*outNlist, 1, 3, dimList, valueList, units, d_size,
                             d_type);
        }
    /* if the TO is the existing,     */
    /* deletes those rows after the */
    /* endPointIndex.            */
    
    if ((find_to EQUALS 2) AND (endPointIndex IS_NOT_EQUAL_TO totalrows))
        {
        nl_set_current_row (*outNlist, endPointIndex + 1, 1);
        nl_delete_rows (*outNlist, (totalrows - endPointIndex));
        }
    /* if the FROM is not the existing */
    /* delete those rows before the */
    /* startPointIndex and insert a new*/
    /* row before the startPointIndex*/
    
    if (find_from EQUALS 1)
        {
        nl_set_current_row (*outNlist, startPointIndex, 1);
        new_cdp = from;
        valueList[0] = (VOIDPTR) & new_cdp;
        valueList[1] = (VOIDPTR) & intrp_x1;
        valueList[2] = (VOIDPTR) & intrp_y1;
        nl_insert_rows_convert (*outNlist, 1, 3, dimList, valueList, units, d_size,
                                d_type);
        nl_set_current_row (*outNlist, 1, 1);
        nl_delete_rows (*outNlist, (startPointIndex - 1));
        }
    /* if the FROM is the existing,   */
    /* deletes those rows before the*/
    /* startPointIndex.           */
    
    if ((find_from EQUALS 2) AND (startPointIndex IS_NOT_EQUAL_TO totalrows))
        {
        nl_set_current_row (*outNlist, 1, 1);
        nl_delete_rows (*outNlist, (startPointIndex - 1));
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To retrieval a seismic line. 

Prototype:
    static INT hl_load_list(FULL_LINE_NAME *line, NLIST_HEADER *plist, FLOAT *i_cdp, 
        FLOAT *f_cdp, INT *entire);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    line            -(FULL_LINE_NAME *)
    plist           -(NLIST_HEADER *)
    i_cdp           -(FLOAT *)
    f_cdp           -(FLOAT *)
    entire          -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <hl_hi_seis.c>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT hl_load_list (FULL_LINE_NAME *line, NLIST_HEADER *plist, FLOAT *i_cdp,
                             FLOAT *f_cdp, INT *entire)
#else
static INT hl_load_list (line, plist, i_cdp, f_cdp, entire)
FULL_LINE_NAME *line;
NLIST_HEADER *plist;
FLOAT *i_cdp, *f_cdp;
INT *entire;
#endif
    {
    INT status = SUCCESS;
    INT change_i = FALSE, change_f = FALSE;
    INT first_cdp, last_cdp;
    
    /* ********************************************************************** */
    
    *entire = FALSE;
    status = se_rd_nl_optim (line, plist);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    /* if the cdps are equals to zero.   */
    /* returns the whole list.           */
    
    status = se_first_last_cdp (line, &first_cdp, &last_cdp);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    *entire =
              (((*i_cdp EQUALS 0.0) AND (*f_cdp EQUALS 0.0)) OR ((*i_cdp
                                                              EQUALS 
                                                              first_cdp) AND (
                                                                         *f_cdp 
                                                                       EQUALS 
                                                                            last_cdp)));
        
    
    if (*entire)
        {
        return SUCCESS;
        }
    if (first_cdp >= *i_cdp)
        {
        change_i = TRUE;
        *i_cdp = first_cdp;
        }
    if (last_cdp <= *f_cdp)
        {
        change_f = TRUE;
        *f_cdp = last_cdp;
        }
    *entire = (change_i AND change_f);
    return SUCCESS;
    }
/* END:     */
