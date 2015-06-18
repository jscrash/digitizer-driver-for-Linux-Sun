/* DEC/CMS REPLACEMENT HISTORY, Element NLM_WCLP_TX.C*/
/* *12   25-OCT-1990 12:17:29 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *11   25-JUL-1990 14:50:47 VINCE "(SPR 5644) Header Standardization"*/
/* *10   30-MAY-1990 15:50:55 GILLESPIE "(SPR 1) Fix up logic for all-inclusive windowing"*/
/* *9    13-APR-1990 10:50:54 GILLESPIE "(SPR 1) Check n-List for updated condition before window test"*/
/* *8    26-MAR-1990 23:22:00 JULIAN "(SPR 5095) change memory alloc"*/
/* *7    25-MAR-1990 13:06:02 GILLESPIE "(SPR 1) Add trivial test for n-List outside clipping rectangle"*/
/* *6    23-MAR-1990 15:31:13 JULIAN "(SPR 5095) change method in text clip"*/
/* *5    22-MAR-1990 01:34:34 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *4    20-MAR-1990 09:17:02 JULIAN "(SPR 0) minor fix"*/
/* *3    19-MAR-1990 18:31:54 GILLESPIE "(SPR 1) Fix up alling overture"*/
/* *2    19-MAR-1990 17:12:05 JULIAN "(SPR 5095) new element"*/
/* *1    19-MAR-1990 17:10:07 JULIAN "text window clip"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_WCLP_TX.C*/
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

#include "esi_nl_int.h"
#include "esi_nl_meth_hdr.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    The routine only does two dimensional clipping.  Gives a n-List and
    specified column, maximum , minimum boundaries, then returns a clipped
    n-List.  The maximum and minimum value should be same as the specified
    the data type of the column.  Any points outside of the boundary
    are rejected.

Prototype:
    publicdef INT nlm_window_clip_text(NLIST_HEADER nlist, VOIDPTR param);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The pointer given N-list. 
    param           -(VOIDPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_EMPTY_NLIST
    NL_OVERFLOW

Scope:
    PUBLIC
    
Limitations/Assumptions:
    All columns in the given n-List must be a VALUE_LIST, but not
    INCREMENT_LIST.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_window_clip_text (NLIST_HEADER nlist, VOIDPTR param) 
#else
publicdef INT nlm_window_clip_text (nlist, param)
NLIST_HEADER nlist;
VOIDPTR param;
#endif
    {
    /* **************************************************************************** */
    
    NLM_CLIP_HEADER *paramBlk = (NLM_CLIP_HEADER *)param;
    UINT nCol = 2;
    DATA_HEADER *pxdata;            /*   workspace for current data header.*/
    DATA_HEADER *pydata;            /*   workspace for current data header.*/
    COLUMN_HEADER *pinfo;           /*   workspace for current column.*/
    COLUMN_HEADER *poutinfo;
    COLUMN_HEADER *pxinfo;          /*   workspace for x column.*/
    COLUMN_HEADER *pyinfo;          /*   workspace for y column.*/
    TABLE_HEADER *ptable;           /*   workspace for current table. */
    NLIST_HEADER outNlist;          /*     Output n-List */
    register UINT i, j;
    UINT totalNdim;                 /*   the total number of columns in N-list*/
    UINT *totalDimList;             /*   the column list for all columns.*/
    INT *units, *incValue;          /*   workspace for initializing a N-list. */
    size_t *size;
    INT *type;                      /*   workspace for initializing a N-list. */
    UINT table_num;                 /*   workspace for table number.         */
    UINT returnPts;                 /*   the really return rows from NL_GET_ROWS */
    INT status = SUCCESS;           /*   returned flag. */
    UINT xdim;                      /*   first (x) column                     */
    UINT ydim;                      /*   second (y) column                    */
    DOUBLE x, y;                    /*   positions of two points.                */
    DOUBLE x1;
    DOUBLE maxX, minX;              /*   the minmum and maxmum value of 'X' nCol. */
    DOUBLE maxY, minY;              /*   the minmum and maxmum value of 'Y' nCol. */
    DOUBLE high_null[2];            /*   the high null value of two specified dims.*/
    DOUBLE low_null[2];             /*   the low null value of two specified dims.*/
    VOIDPTR *var_list;
    
    paramBlk->outNlist = (NLIST_HEADER)0;   /* set to null in case of failure */
    
    INIT_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, paramBlk->Col_List[i]);
        }
    /* **************************************************************************** */
    /*   error checking.     */
    /*   convert the minmum and maxmum value into DOUBLE */
    
    minX = paramBlk->minList[0];
    minY = paramBlk->minList[1];
    maxX = paramBlk->maxList[0];
    maxY = paramBlk->maxList[1];
    
    /*   initialize the returned N-list. */
    xdim = paramBlk->Col_List[0] - 1;
    ydim = paramBlk->Col_List[1] - 1;
    
    /* check for updated n-List */
    
    if (nlist->updated)
        {
        BOOL defer = nlist->defer_stats;
        
        nlist->defer_stats = FALSE;
        nlz_compute_nlist_stats (nlist);
        nlist->defer_stats = defer;
        }
    /* Make sure there is clipping work to perform before getting too much farther */
    pxinfo = nlist->info_ptr + xdim;
    pyinfo = nlist->info_ptr + ydim;
    
    if (pxinfo->max < minX OR pxinfo->min > maxX OR pyinfo->max < minY OR pyinfo->min >
        maxY)
        {
        return NL_EMPTY_NLIST;
        }
    /* No such luck.  Time to get to work... */
    totalNdim = nlist->nCol;
    
    /* Allocate all dynamic memory needed */
    units = (INT *)TALLOC (sizeof(INT) * totalNdim);
    if (units EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    incValue = (INT *)TALLOC (sizeof(INT) * (totalNdim));
    if (incValue EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    type = (INT *)TALLOC (sizeof(INT) * (totalNdim));
    if (type EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    size = (size_t *)TALLOC (sizeof(size_t) * (totalNdim));
    if (size EQUALS (size_t *)0)
        {
        return NL_OVERFLOW;
        }
    totalDimList = (UINT *)TALLOC (sizeof(UINT) * (totalNdim));
    if (totalDimList EQUALS (UINT *)0)
        {
        return NL_OVERFLOW;
        }
    var_list = (VOIDPTR *)TALLOC (sizeof(VOIDPTR) * (totalNdim));
    if (var_list EQUALS (VOIDPTR *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0, pinfo = nlist->info_ptr; i < totalNdim; i++, pinfo++)
        {
        totalDimList[i] = i + 1;
        units[i] = pinfo->unit;
        incValue[i] = VALUE_LIST;
        
        /* set the tolerance range of NULL for two  */
        /* specified column.                     */
        if (i == xdim)
            {
            size[i] = sizeof(DOUBLE);
            type[i] = FLOAT_TYPE;
            var_list[i] = (VOIDPTR) & x;
            x1 = pinfo->null_value;
            high_null[0] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[0] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        else if (i == ydim)
            {
            size[i] = sizeof(DOUBLE);
            type[i] = FLOAT_TYPE;
            var_list[i] = (VOIDPTR) & y;
            x1 = pinfo->null_value;
            high_null[1] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[1] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        else
            {
            size[i] = pinfo->data_size;
            type[i] = pinfo->data_type;
            var_list[i] = (VOIDPTR)TALLOC (pinfo->data_size);
            if (var_list[i]EQUALS (VOIDPTR)0)
                {
                return NL_OVERFLOW;
                }
            }
        }
    nl_clone_nlist (nlist, &paramBlk->outNlist);
    outNlist = paramBlk->outNlist;
    
    /* copy the column names and null value for each column... */
    for (i = 0, pinfo = nlist->info_ptr, poutinfo = outNlist->info_ptr; i < totalNdim;
         i++, pinfo++, poutinfo++)
        {
        strcpy ((char *)poutinfo->column_name, (char *)pinfo->column_name);
        strcpy ((char *)poutinfo->convert_format, (char *)pinfo->convert_format);
        poutinfo->null_value = pinfo->null_value;
        }
    /* set the user structure in the NLIST_HEADER.*/
    
    if (nlist->user_struct_len > 0)
        {
        status = nlz_copy_structure (nlist->user_struct, nlist->user_struct_len,
                                     &outNlist->user_struct,
                                     &outNlist->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    for (i = 1; i <= nlist->total_tables; i++)
        {
        ptable = nlz_set_table (nlist, i);
        if (ptable == NULL_TABLE)
            {
            continue;
            }
        pxdata = ptable->data_ptr + xdim;
        pydata = ptable->data_ptr + ydim;
        
        /* Address the trivial case where the entire stroke is inside the window */
        
        if (pxdata->min >= minX AND pxdata->max <= maxX AND pydata->min >=
            minY AND pydata->max <= maxY)
            {
            status = nl_copy_table (nlist, i, outNlist, &table_num);
            continue;
            }
        /* See if the x/y coordinates are all outside the clipping window */
        if (pxdata->max < minX || pxdata->min > maxX || pydata->max < minY ||
            pydata->min > maxY)
            {
            continue;
            }
        /*   get all rows in the current table, */
        /*   and convert x,y coords to DOUBLE, if necessary   */
        
        nl_start_table (outNlist, &table_num);
        status = nlz_copy_structure (ptable->user_struct, ptable->user_struct_len,
                                     &outNlist->current_table_ptr->user_struct,
                                     &outNlist->current_table_ptr->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        /* Since the last operation on nlist was a set table,
        we can safely set the current row index rather than call set_current_row */
        nlist->current_row = 1;
        
        for (j = 0; j < ptable->nrows; j++)
            {
            status = nl_get_rows_convert (nlist, (UINT)1, totalNdim, totalDimList,
                                          (VOIDPTR *)var_list, units, size, type,
                                          &returnPts);
            if (status < 0)
                {
                break;
                }
            if (x >= minX AND x <= maxX AND y >= minY AND y <= maxY)
                {
                status = nl_add_rows_convert (outNlist, (UINT)1, totalNdim, totalDimList,
                                              (VOIDPTR *)var_list, units, size, type);
                }
            }
        }
#if NEED_TO_FREE
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)type);
    tc_free ((VOIDPTR)size);
    tc_free ((VOIDPTR)totalDimList);
    for (i = 0, pinfo = nlist->info_ptr; i < totalNdim; i++, pinfo++)
        {
        if (i != xdim && i != ydim)
            {
            tc_free (var_list[i]);
            }
        }
    tc_free ((VOIDPTR)var_list);

#endif
    if (outNlist->total_rows == 0)
        {
        nl_free_nlist (paramBlk->outNlist);
        paramBlk->outNlist = (NLIST_HEADER)0;
        return NL_EMPTY_NLIST;
        }
    else
        {
        return SUCCESS;
        }
    }
/* END:     */
