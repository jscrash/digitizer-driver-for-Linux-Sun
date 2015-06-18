/* DEC/CMS REPLACEMENT HISTORY, Element NLM_WCLP_BRK.C*/
/* *15   25-OCT-1990 12:17:13 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *14   25-JUL-1990 14:50:24 VINCE "(SPR 5644) Header Standardization"*/
/* *13    1-JUN-1990 22:30:38 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *12   25-MAY-1990 23:02:21 GILLESPIE "(SPR 5251) Add prev. point to draw list if null encountered (fixed gap in contours)"*/
/* *11    2-MAY-1990 11:47:18 JULIAN "(SPR 5221) set null value on nlist to fix contours ovl problem"*/
/* *10   13-APR-1990 10:50:08 GILLESPIE "(SPR 1) Check n-List for updated condition before window test"*/
/* *9     9-APR-1990 15:41:25 GILLESPIE "(SPR 1) Set current row prior to get"*/
/* *8    25-MAR-1990 13:04:58 GILLESPIE "(SPR 1) Add trivial test for n-List outside clipping rectangle"*/
/* *7    25-MAR-1990 10:23:11 GILLESPIE "(SPR 1) Fix problem wrt dropping last point of tables"*/
/* *6    22-MAR-1990 01:34:23 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *5    12-MAR-1990 19:02:41 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *4     5-MAR-1990 10:44:18 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *3    15-FEB-1990 01:19:56 GILLESPIE "(SPR 6005) Fix y clipping limit problem"*/
/* *2    14-FEB-1990 13:18:12 GILLESPIE "(SPR 6005) Make nlist window clipping a method"*/
/* *1    14-FEB-1990 13:13:45 GILLESPIE "Window clip method"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLM_WCLP_BRK.C*/
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

#include "esi_nl_int.h"
#include "esi_nl_meth_hdr.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_AG_H

#include "esi_ag.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    The routine only does two dimensional clipping.  Gives a N-list and
    specified column, maximum , minimum boundaries, then returns a clipped
    N-list.  The maximum and minimum value should be same as the specified
    the data type of the column.
    This version uses the TABLE BREAK to seperate the clipped
    points. The total number of table will be changed
    in the output list, the table user structure will
    be copied from the parent tables in the input n-List.

Prototype:
    publicdef INT nlm_window_clip_break (NLIST_HEADER nlist, VOIDPTR param);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The pointer given N-list.
    param           -(VOIDPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_EMPTY_NLIST
    NL_OVERFLOW
    NL_INVALID_COLUMN_SIZE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    All columns in the given n-List must be a VALUE_LIST, but not
    INCREMENT_LIST.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlm_window_clip_break (NLIST_HEADER nlist, VOIDPTR param) 
#else
publicdef INT nlm_window_clip_break (nlist, param)
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
    COLUMN_HEADER *poutinfo;        /*   workspace for current column.*/
    COLUMN_HEADER *pxinfo;          /*   workspace for x column.*/
    COLUMN_HEADER *pyinfo;          /*   workspace for y column.*/
    TABLE_HEADER *ptable;           /*   workspace for current table. */
    NLIST_HEADER outNlist;          /*     Output n-List */
    UINT nallocated_points;
    register UINT i, j, indexPt;    /*   index.    */
    UINT totalNdim;                 /*   the total number of columns in N-list*/
    UINT *totalDimList;             /*   the column list for all columns.*/
    INT *units, *incValue;          /*   workspace for initializing a N-list. */
    size_t *size;
    size_t *double_size;
    INT *double_type;
    UINT blockSize;
	INT *type;
    UINT table_num;                 /*   workspace for table number.         */
    UINT resultPts;                 /*   the total number of points, after clipping*/
    UINT returnPts;                 /*   the really return rows from NL_GET_ROWS */
    INT status = SUCCESS;           /*   returned flag. */
    UINT xdim;                      /*   first (x) column                     */
    UINT ydim;                      /*   second (y) column                    */
    BOOL visible;                   /*   visible flag.  */
    BOOL lastPtNull;                /*   flag indicating last point was found to be NULL */
    BOOL thisPtNull;                /*   flag indicating current point was found to be NULL */
    DOUBLE **doubleArray;           /*   workspace for retrieving DOUBLE points. */
    DOUBLE **clipArray;             /*   the array of been clipped points.       */
    DOUBLE x1, y1, x2, y2;          /*   positions of two points.                */
    /*------------------------------------------------*/
    /* the position of first point.  Used to          */
    /* compare the position before and after clipping */
    /*------------------------------------------------*/
    DOUBLE clipx1, clipy1;
    
    /*------------------------------------------------*/
    /* the position of second point.  Used to         */
    /* compare the position before and after clipping */
    /*------------------------------------------------*/
    DOUBLE clipx2, clipy2;
    
    DOUBLE maxX, minX;              /*   the minmum and maxmum value of 'X' nCol. */
    DOUBLE maxY, minY;              /*   the minmum and maxmum value of 'Y' nCol. */
    /*------------------------------------------*/
    /* the interpretion constants of points for */
    /* others columns.                          */
    /*------------------------------------------*/
    DOUBLE const1, const2;
    
    DOUBLE high_null[2];            /*   the high null value of two specified dims.*/
    DOUBLE low_null[2];             /*   the low null value of two specified dims.*/
    
    if (param EQUALS (VOIDPTR)0)
        {
        return FAIL;
        }
    paramBlk->outNlist = (NLIST_HEADER)0;   /* set to null in case of failure */
    
    INIT_TEST (nlist);
    
    if (nCol != 2)
        {
        return NL_INVALID_COLUMN_SIZE;
        }
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
    blockSize = nlist->blocksize;
    
    /* Allocate temporary memory */
    
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
    clipArray = (DOUBLE **)TALLOC (sizeof(DOUBLE *) * (totalNdim));
    if (clipArray EQUALS (DOUBLE **)0)
        {
        return NL_OVERFLOW;
        }
    doubleArray = (DOUBLE **)TALLOC (sizeof(DOUBLE *) * (totalNdim));
    if (doubleArray EQUALS (DOUBLE **)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < totalNdim; i++)
        {
        totalDimList[i] = i + 1;
        pinfo = nlist->info_ptr + i;
        units[i] = pinfo->unit;
        size[i] = pinfo->data_size;
        type[i] = pinfo->data_type;
        incValue[i] = VALUE_LIST;
        
        /* set the tolerance range of NULL for two  */
        /* specified column.                     */
        if (i == xdim)
            {
            x1 = pinfo->null_value;
            high_null[0] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[0] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        if (i == ydim)
            {
            x1 = pinfo->null_value;
            high_null[1] = (x1 > 0) ? x1 * 1.01 : x1 * 0.99;
            low_null[1] = (x1 > 0) ? x1 * 0.99 : x1 * 1.01;
            }
        }
    nl_init_nlist (&outNlist, totalNdim, units, size, type, incValue, blockSize);
    paramBlk->outNlist = outNlist;
    
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
    /*   clips each table.  */
    
    double_size = (size_t *)TALLOC (totalNdim * sizeof(size_t));
    if (double_size EQUALS (size_t *)0)
        {
        return NL_OVERFLOW;
        }
    double_type = (INT *)TALLOC (totalNdim * sizeof(INT));
    if (double_type EQUALS (INT *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < totalNdim; i++)
        {
        double_size[i] = sizeof(DOUBLE);
        double_type[i] = FLOAT_TYPE;
        }
    /*   allocate space for returned DOUBLE points*/
    /*   and clipped points array.           */
    /*   *2 to accommodate interpolated points */
    
    nallocated_points = MIN (500, nlist->total_rows);
    
    for (j = 0; j < totalNdim; j++)
        {
        doubleArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * nallocated_points);
        if (doubleArray[j] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        clipArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) * (nallocated_points + 2));
        if (clipArray[j] == (DOUBLE *)0)
            {
            return NL_OVERFLOW;
            }
        }
    for (i = 1; i <= nlist->total_tables; i++)
        {
        if (paramBlk->autopurge AND i > 1)
            {
            nl_purge_table (nlist, (UINT)0);
            }
        ptable = nlz_set_table (nlist, i);
        
        if (ptable == NULL_TABLE OR ptable->nrows == 1)
            {
            continue;
            }
        pxdata = ptable->data_ptr + xdim;
        pydata = ptable->data_ptr + ydim;
        
        if (pxdata->max < minX OR pxdata->min > maxX OR pydata->max <
            minY OR pydata->min > maxY)
            {
            continue;
            }
        if (ptable->nrows > nallocated_points)
            {
            nallocated_points = ptable->nrows;
            for (j = 0; j < totalNdim; j++)
                {
                tc_free ((VOIDPTR)doubleArray[j]);
                tc_free ((VOIDPTR)clipArray[j]);
                doubleArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) *
                                                     nallocated_points);
                if (doubleArray[j] == (DOUBLE *)0)
                    {
                    return NL_OVERFLOW;
                    }
                clipArray[j] = (DOUBLE *)tc_alloc (sizeof(DOUBLE) *
                                                   (nallocated_points + 2));
                if (clipArray[j] == (DOUBLE *)0)
                    {
                    return NL_OVERFLOW;
                    }
                }
            }
        /*   get all rows in the current table, */
        /*   and converted to be DOUBLE.           */
        
        nl_set_current_row (nlist, (UINT)1, (UINT)0);
        status = nl_get_rows_convert (nlist, ptable->nrows, totalNdim, totalDimList,
                                      (VOIDPTR *)doubleArray, units, double_size,
                                      double_type, &returnPts);
        if (status < 0)
            {
            break;
            }
        /* Address the trivial case where the entire stroke is inside the window */
        
        if (pxdata->min >= minX AND pxdata->max <= maxX AND pydata->min >=
            minY AND pydata->max <= maxY)
            {
            nl_start_table (outNlist, &table_num);
            status = nlz_copy_structure (ptable->user_struct, ptable->user_struct_len,
                                         &outNlist->current_table_ptr->user_struct,
                                         &outNlist->current_table_ptr->
                                         user_struct_len);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            status = nl_add_rows_convert (outNlist, ptable->nrows, totalNdim,
                                          totalDimList, (VOIDPTR *)doubleArray, units,
                                          double_size, double_type);
            
            continue;
            }
        /* Prepare the loop - perform minimal processing on the
        first point */
        
        x2 = doubleArray[xdim][0];
        y2 = doubleArray[ydim][0];
        /* Check for null - note that values lower than most values perform
           fewer tests */
        lastPtNull = (x2 <= high_null[0] && x2 >= low_null[0]) ||
                     (y2 <= high_null[1] && y2 >= low_null[1]);
        resultPts = 0;
        
        /*   go through each pair of points to check */
        /*   the visibility of the segment.  If the  */
        /*   point is inside window all columns of */
        /*   the row keep same value.  If the point */
        /*   has been clipped, stores the new value  */
        /*   in the specified column, others will */
        /*   be NULL.                                */
        
        for (indexPt = 1; indexPt < returnPts; indexPt++, lastPtNull = thisPtNull)
            {
            x1 = x2;
            y1 = y2;
            x2 = doubleArray[xdim][indexPt];
            y2 = doubleArray[ydim][indexPt];
            
            thisPtNull = (x2 <= high_null[0] && x2 >= low_null[0]) ||
                         (y2 <= high_null[1] && y2 >= low_null[1]);
            if (thisPtNull)
                {
                /* if the first point is not NULL, but */
                /* the second point is NULL, check the */
                /* position of the first point, if it  */
                /* is inside the widow append the row  */
                /* and store to the new table.         */
                
                if (NOT lastPtNull)
                    {
                    if (resultPts > 0)
                        {
                        /* We must put the last value into the array before storing... */
                        for (j = 0; j < totalNdim; j++)
                            {
                            clipArray[j][resultPts] = doubleArray[j][indexPt - 1];
                            }
                        nl_start_table (outNlist, &table_num);
                        status = nlz_copy_structure (ptable->user_struct,
                                                     ptable->user_struct_len,
                                                     &outNlist->current_table_ptr->
                                                     user_struct,
                                                     &outNlist->current_table_ptr->
                                                     user_struct_len);
                        if (status IS_NOT_EQUAL_TO SUCCESS)
                            {
                            return status;
                            }
                        status = nl_add_rows_convert (outNlist, ++resultPts, totalNdim,
                                                      totalDimList,
                                                      (VOIDPTR *)clipArray, units,
                                                      double_size, double_type);
                        
                        resultPts = 0;
                        }
                    }
                continue;
                }
            if (lastPtNull)
                {
                continue;
                }
            clipx1 = x1;
            clipy1 = y1;
            clipx2 = x2;
            clipy2 = y2;
            ag_clip_1 (&clipx1, &clipy1, &clipx2, &clipy2, minX, minY, maxX, maxY,
                       &const1, &const2, &visible);
            if (visible)
                {
                if (clipx1 != x1 OR clipy1 != y1)
                    
                    /* the first point has been clipped, */
                    /* all columns will be interpreted as */
                    /* the same ratio, but the CHAR_TYPE dims */
                    /* maybe destroyed.                      */
                    {
                    if (totalNdim != nCol)
                        {
                        for (j = 0; j < totalNdim; j++)
                            {
                            clipArray[j][resultPts] = doubleArray[j][indexPt - 1] +
                                                      const1 *
                                                      (doubleArray[j][indexPt] -
                                                       doubleArray[j][indexPt - 1]);
                            }
                        }
                    else
                        {
                        clipArray[xdim][resultPts] = clipx1;
                        clipArray[ydim][resultPts] = clipy1;
                        }
                    }
                else                /*   keep all columns value same.    */
                    {
                    for (j = 0; j < totalNdim; j++)
                        clipArray[j][resultPts] = doubleArray[j][indexPt - 1];
                    }
                resultPts++;
                /* If the second point is clipped OR this is the last point ... */
                if (clipx2 != x2 OR clipy2 != y2 OR (indexPt + 1) == returnPts)
                    {
                    /*   If the second point has been clipped,
                         add the new value followed by a NULL
                         value, otherwise do nothing.    */
                    if (totalNdim != nCol)
                        {
                        for (j = 0; j < totalNdim; j++)
                            {
                            clipArray[j][resultPts] = doubleArray[j][indexPt - 1] +
                                                      const2 *
                                                      (doubleArray[j][indexPt] -
                                                       doubleArray[j][indexPt - 1]);
                            }
                        }
                    else
                        {
                        clipArray[xdim][resultPts] = clipx2;
                        clipArray[ydim][resultPts] = clipy2;
                        }
                    nl_start_table (outNlist, &table_num);
                    status = nlz_copy_structure (ptable->user_struct,
                                                 ptable->user_struct_len,
                                                 &outNlist->current_table_ptr->
                                                 user_struct,
                                                 &outNlist->current_table_ptr->
                                                 user_struct_len);
                    if (status IS_NOT_EQUAL_TO SUCCESS)
                        {
                        return status;
                        }
                    status = nl_add_rows_convert (outNlist, ++resultPts, totalNdim,
                                                  totalDimList, (VOIDPTR *)clipArray,
                                                  units, double_size, double_type);
                    
                    resultPts = 0;
                    }
                }
            }
        /* if the array has not yet been written, now is a GREAT time to do it */
        if (resultPts > 1)
            {
            nl_start_table (outNlist, &table_num);
            status = nlz_copy_structure (ptable->user_struct, ptable->user_struct_len,
                                         &outNlist->current_table_ptr->user_struct,
                                         &outNlist->current_table_ptr->
                                         user_struct_len);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            status = nl_add_rows_convert (outNlist, resultPts, totalNdim, totalDimList,
                                          (VOIDPTR *)clipArray, units, double_size,
                                          double_type);
            }
        }
    if (paramBlk->autopurge)
        {
        nl_purge_table (nlist, (UINT)0);
        }
    /*   free the allocated temp. workspace. */
    for (j = 0; j < totalNdim; j++)
        {
        tc_free ((VOIDPTR)doubleArray[j]);
        tc_free ((VOIDPTR)clipArray[j]);
        }
#if NEED_TO_FREE == TRUE
    /*   free all allocated memory.    */
    
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)type);
    tc_free ((VOIDPTR)size);
    tc_free ((VOIDPTR)totalDimList);
    tc_free ((VOIDPTR)clipArray);
    tc_free ((VOIDPTR)doubleArray);
    tc_free ((VOIDPTR)double_size);
    tc_free ((VOIDPTR)double_type);
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
