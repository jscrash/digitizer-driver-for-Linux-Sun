/* DEC/CMS REPLACEMENT HISTORY, Element NLM_BILIN_NL.C*/
/* *8    23-OCT-1990 11:56:41 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *7    17-MAR-1990 13:01:03 JULIAN "(SPR 0) undo last mods"*/
/* *6    16-MAR-1990 11:23:18 JULIAN "(SPR 5135) add nl_strip_points"*/
/* *5    26-FEB-1990 19:00:12 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *4    26-JAN-1990 14:27:24 WALTERS "(SPR 0) Fix bug in nl_set_nlist_info"*/
/* *3    17-JAN-1990 15:36:27 WALTERS "(SPR 0) Add nl_init_nlist_info to name dimensions"*/
/* *2     9-NOV-1989 13:31:44 GORDON "(SPR 5058) Bilinear nlist resampling routine"*/
/* *1     9-NOV-1989 13:30:13 GORDON "Bilinear nlist resampling routine"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_BILIN_NL.C*/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Finder Graphics Systems, Inc.                                           */
/*          201 Tamal Vista Blvd                                                    */
/*          Corte Madera, CA  USA 94925                                             */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#include "esi_nl_int.h"
#include "esi_nl_meth_hdr.h"
#include "esi_ho.h"
#include "esi_tc.h"
#include "esi_math.h"

#if USE_PROTOTYPES
static INT nlz_insert_new_points (DOUBLE **valueList1, DOUBLE distance,
                                      DOUBLE **valueList2, UINT *total_rows);
#else
static INT nlz_insert_new_points ();
#endif

/* ******************************************************************************   */
/*                                                                                  */
/*    ROUTINE: NLM_BILINEAR_NLIST                                                   */
/*                                                                                  */
/*    DESCRIPTION                                                                   */
/*  To insert rows into every two rows, when the distance between                   */
/*    them is greater than the given distance.  The total number of rows will       */
/*    be inserted is INT(delta_diatance/distance) and new distance will be          */
/*    delta_ditance / (INT (delta_distance/distance) + 1).                          */
/*                                                                                  */
/*    **************************************************************************    */
#if USE_PROTOTYPES
publicdef INT nlm_bilinear_nlist (NLIST_HEADER nlist, VOIDPTR param)
#else
publicdef INT nlm_bilinear_nlist (nlist, param)
NLIST_HEADER nlist;                 /* the pointer given N-list.                    */
VOIDPTR param;
#endif
    {
    NLM_RESAMPLE_HEADER *p = (NLM_RESAMPLE_HEADER *)param;
    UINT i, j;                      /*   indexes.                                   */
    INT status;                     /*   the result status.                         */
    UINT table_num;                 /*   table number.                              */
    UINT currentPt;
    INT rowsInTable;
    UINT total_rows;                /*   total rows will be added to the table.     */
    size_t *dataSize;               /*   an array of data size of specified p->ncol.*/
    INT *dataType;                  /*   an array of data type of specified p->ncol.*/
    INT *units;                     /*   an array of unit of specified columns      */
    INT *incValue;                  /*   an array of INCREMENT_LIST or VALUE_LIST   */
    DOUBLE **valueList1, **valueList2;  /*   array of pointers to each column's data*/
    DOUBLE x[2], y[2];
    COLUMN_HEADER *pinfo;           /*   workspace of COLUMN_HEADER.                */
    COLUMN_HEADER *poldinfo;
    TABLE_HEADER *poldtab, *pNewtab;
    NLIST_HEADER pNew;
    
    p->outNlist = (NLIST_HEADER)0;
    if (p->ncol > 2 OR p->ncol > 2)
        return NL_INVALID_COLUMN_SIZE;
    
    if (nlist->total_rows < 1)
        return NL_EMPTY_NLIST;
    
    INIT_TEST (nlist);
    
    for (i = 0; i < p->ncol; i++)
        COLUMN_TEST (nlist, p->Col_List[i]);
    
    /*   allocate memory for initializing N-list                                    */
    
    dataSize = (size_t *) tc_alloc (sizeof(size_t) * p->ncol);
    dataType = (INT *) tc_alloc (sizeof(INT) * p->ncol);
    units = (INT *) tc_alloc (sizeof(INT) * p->ncol);
    incValue = (INT *) tc_alloc (sizeof(INT) * p->ncol);
    valueList1 = (DOUBLE * *) tc_alloc (sizeof(DOUBLE *)*p->ncol);
    valueList2 = (DOUBLE * *) tc_alloc (sizeof(DOUBLE *)*p->ncol);
    valueList1[0] = x;
    valueList1[1] = y;
    
    for (i = 0; i < p->ncol; i++)
        {
        pinfo = nlist->info_ptr + p->Col_List[i] - 1;
        dataSize[i] = pinfo->data_size;
        dataType[i] = pinfo->data_type;
        units[i] = pinfo->unit;
        incValue[i] = VALUE_LIST;
        }
    status = nl_init_nlist (&pNew, p->ncol, units, dataSize, dataType, incValue,
                            nlist->blocksize);
    if (status != SUCCESS)
        {
        return status;
        }
    p->outNlist = pNew;
	
    for (j = 0, poldinfo = nlist->info_ptr, pinfo = pNew->info_ptr; j < p->ncol;
         j++, pinfo++, poldinfo++)
        {
        dataSize[j] = sizeof(DOUBLE);
        dataType[j] = FLOAT_TYPE;
        strcpy (pinfo->column_name, poldinfo->column_name);
        }
    /* Go througth all tables to do                                                 */
    /* bi_linear operation.                                                         */
    for (table_num = 1; table_num <= nlist->total_tables; table_num++)
        {
        status = nl_start_table (pNew, &i);
        status = nl_set_current_row (nlist, 1, table_num);
        if (status < 0)
            continue;
        status = nl_inq_table_int (nlist, table_num, NL_TABLE_NROWS, &rowsInTable);
        if (status < 0 OR rowsInTable EQUALS 0)
            continue;
        
        /* retrieve every pair rows.                                                */
        for (currentPt = 1; currentPt <= rowsInTable AND status >= 0; currentPt++)
            {
            status = nl_set_current_row (nlist, currentPt, table_num);
            if (status < 0)
                continue;
            status = nl_get_rows_convert (nlist, 2, p->ncol, p->Col_List,
                                          (VOIDPTR *)valueList1, units, dataSize,
                                          dataType, &total_rows);
            if (status < 0)
                continue;
            /* if the returned rows is less                                         */
            /* than 2, means it is the last row                                     */
            /* add the last rows, then continue                                     */
            
            if (total_rows < 2)
                {
                status = nl_add_rows_convert (pNew, total_rows, p->ncol,
                                              p->Col_List, (VOIDPTR *)valueList1,
                                              units, dataSize, dataType);
                continue;
                }
            else                    /* otherwise, insert rows between               */
                /* two rows, if it is necessary.                                    */
                {
                status = nlz_insert_new_points (valueList1, p->resampleInc, valueList2,
                                                &total_rows);
                status = nl_add_rows_convert (pNew, total_rows - 1, p->ncol,
                                              p->Col_List, (VOIDPTR *)valueList2,
                                              units, dataSize, dataType);
                }
            if (status)
                return status;
            for (j = 0; j < p->ncol; j++)
                tc_free ((VOIDPTR)valueList2[j]);
            }
        }
    /*   free all allocated memory.                                                 */
    tc_free ((VOIDPTR)valueList1);
    tc_free ((VOIDPTR)valueList2);
    tc_free ((VOIDPTR)dataSize);
    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    
    /*   copy the information in the N-list header                                  */
    if (nlist->user_struct_len > 0)
        {
        status = nlz_copy_structure (nlist->user_struct, nlist->user_struct_len,
                                     &pNew->user_struct, &pNew->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*                                                                              */
    /*   * copy the n-List name, if needed                                          */
    /*                                                                              */
    if (nlist->name IS_NOT_EQUAL_TO NULL)
        {
        pNew->name = STRING_ALLOCATE (nlist->name);
        strcpy (pNew->name, nlist->name);
        }
    if (nlist->next_nlist IS_NOT_EQUAL_TO NULL)
        {
        pNew->next_nlist = STRING_ALLOCATE (nlist->next_nlist);
        strcpy (pNew->next_nlist, nlist->next_nlist);
        }
    if (nlist->prev_nlist IS_NOT_EQUAL_TO NULL)
        {
        pNew->prev_nlist = STRING_ALLOCATE (nlist->prev_nlist);
        strcpy (pNew->prev_nlist, nlist->prev_nlist);
        }
    hoblockmove (nlist->creation_date, pNew->creation_date, sizeof(DATE));
    
    /*                                                                              */
    /*   * now copy the tables                                                      */
    /*                                                                              */
    
    for (i = 0, poldtab = nlz_set_table (nlist, i + 1),
         pNewtab = nlz_set_table (pNew, i + 1);
         i < nlist->total_tables AND poldtab != NULL_TABLE;
         i++, poldtab = nlz_set_table (nlist, i + 1),
         pNewtab = nlz_set_table (pNew, i + 1))
        {
        
        /*                                                                          */
        /*   * copy the table header and column data headers                        */
        /*                                                                          */
        status = nlz_copy_structure (poldtab->user_struct, poldtab->user_struct_len,
                                     &pNewtab->user_struct, &pNewtab->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        /*                                                                          */
        /*   * copy the table name, if needed                                       */
        /*                                                                          */
        if (poldtab->name IS_NOT_EQUAL_TO NULL)
            {
            pNewtab->name = STRING_ALLOCATE (poldtab->name);
            strcpy (pNewtab->name, poldtab->name);
            }
        pNewtab->totalBytesInTable = poldtab->totalBytesInTable;
        
        }
    return SUCCESS;
    }

/* ************************************************************************         */
/*                                                                                  */
/*    ROUTINE: NLZ_INSERT_NEW_POINTS.                                               */
/*                                                                                  */
/*    DESCRIPTION:                                                                  */
/*  To insert rows into array, if the delta distance is greater than                */
/*    the given distance.                                                           */
/*                                                                                  */
/*    **********************************************************************        */
#if USE_PROTOTYPES
static INT nlz_insert_new_points (DOUBLE **valueList1, DOUBLE distance,
                                      DOUBLE **valueList2, UINT *total_rows)
#else
static INT nlz_insert_new_points (valueList1, distance, valueList2, total_rows)
DOUBLE **valueList1, **valueList2;
DOUBLE distance;
UINT *total_rows;
#endif
    {
    INT status = SUCCESS;
    INT newPoints = 0;
    INT i, j;
    DOUBLE delta_dis, deltas[2], new_dis;
    
    /* find out the distance between                                                */
    /* two rows.                                                                    */
    
    delta_dis = sqrt ((valueList1[0][1] - valueList1[0][0]) * (valueList1[0][1] -
                                                               valueList1[0][0]) +
                      (valueList1[1][1] - valueList1[1][0]) * (valueList1[1][1] -
                                                               valueList1[1][0]));
    
    /* if the distance between two                                                  */
    /* rows is not greater than                                                     */
    /* the given distance, returns                                                  */
    /* the input two rows.                                                          */
    
    if (delta_dis <= distance)
        {
        for (i = 0; i < 2; i++)
            {
            valueList2[i] = (DOUBLE *) tc_alloc (sizeof(DOUBLE) * 2);
            for (j = 0; j < 2; j++)
                valueList2[i][j] = valueList1[i][j];
            }
        *total_rows = 2;
        return status;
        
        }
    /* otherwise inserts new rows.                                                  */
    newPoints = delta_dis / distance;
    new_dis = delta_dis / (newPoints + 1);
    deltas[0] = (valueList1[0][1] - valueList1[0][0]) * new_dis / delta_dis;
    deltas[1] = (valueList1[1][1] - valueList1[1][0]) * new_dis / delta_dis;
    
    for (i = 0; i < 2; i++)
        {
        valueList2[i] = (DOUBLE *) tc_alloc (sizeof(DOUBLE) * (newPoints + 2));
        valueList2[i][0] = valueList1[i][0];
        for (j = 0; j < newPoints; j++)
            {
            valueList2[i][j + 1] = valueList2[i][j] + deltas[i];
            }
        valueList2[i][newPoints + 1] = valueList1[i][1];
        }
    *total_rows = newPoints + 2;
    return status;
    }
