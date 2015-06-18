/* DEC/CMS REPLACEMENT HISTORY, Element NL_REP_ROWS.C */
/* *9    25-OCT-1990 12:26:31 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *8    25-JUL-1990 15:05:03 VINCE "(SPR 5644) Header Standardization" */
/* *7     9-APR-1990 10:44:23 GILLESPIE "(SPR 1) Use nlz_set_table" */
/* *6    24-MAR-1990 11:54:53 GILLESPIE "(SPR 1) Change nlist->nCol to nCol" */
/* *5    23-MAR-1990 11:23:23 GILLESPIE "(SPR 5158) Only change stats on columns being replaced" */
/* *4    22-MAR-1990 01:36:08 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *3    12-MAR-1990 19:09:27 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:49:59 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:21 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_REP_ROWS.C */
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

#include "esi_nl_int.h"
#include "esi_ho.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Replace 1 or more rows in an N-list beginning with the current row.

Prototype:
    publicdef INT nl_replace_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.        
    nrows           -(UINT) Number of requested rows.            
    nCol            -(UINT) Number of requested columns.      
    Col_List        -(UINT *) List of requested columns.            
    Var_List        -(VOIDPTR *) List of pointers of returned value.  

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_INVALID_POINTER
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_replace_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                               UINT *Col_List, VOIDPTR *Var_List)
#else
publicdef INT nl_replace_rows (nlist, nrows, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    DOUBLE x;
    INT status;                     /*    returned status.                          */
    register UINT i, j;             /*    indexes.                                  */
    UINT replacePoints;             /*    actually replaced number of rows.         */
    UINT currentPoint;
    UINT appendPoints;
    size_t data_size;               /*    data size of each column.                 */
    VOIDPTR *append_value_list;
    register COLUMN_HEADER *pinfo;
    register DATA_HEADER *pdata;
    TABLE_HEADER *ptable;
    
    /*  *************************************************************************  */
    /*      error checking.                */
    
    INIT_TEST (nlist);
    
    NROWS_TEST (nrows);
    
    EMPTY_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] == (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        }
    append_value_list = (VOIDPTR *) tc_alloc (sizeof(VOIDPTR) * nCol);
    if (append_value_list == (VOIDPTR *)0)
        {
        return NL_OVERFLOW;
        }
    /*   if the requested rows are more than existing rows after current row,    */
    /*   (in the current table), only replace existing rows.                     */
    currentPoint = nlist->current_row;
    ptable = nlz_set_table (nlist, nlist->current_table);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    replacePoints = (nrows > ptable->nrows - currentPoint + 1) ?
        ptable->nrows - currentPoint + 1 : nrows;
    
    NROWS_TEST (replacePoints);
    
    /*   if the data has not been loaded, then loads them first.                */
    
    appendPoints = nrows - replacePoints;
    
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows > 0)
        {
        status = nlz_load_data (nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*   replace all non-incrementing columns */
    
    if (nlist->defer_stats)
        {
        ptable->updated = TRUE;
        }
    else
        {
        /* First, recalculate the statistics on the rows being deleted (replaced) */
        for (i = 0; i < nCol; i++)
            {
            pinfo = nlist->info_ptr + Col_List[i] - 1;
            pdata = ptable->data_ptr + Col_List[i] - 1;
            for (j = 0; j < replacePoints; j++)
                {
                if ((nlz_get_value (pdata, pinfo, nlist->current_row + j,
                                    &x) < 0) OR (x EQUALS pinfo->null_value))
                    {
                    continue;
                    }
                /* If the deleted value matches the min or max value for the column, then
                a complete pass through the data will be necessary to establish the new
                min/max value, and continuing on here is just a waste of CPU cycles... */
                if (x == pdata->min || x == pdata->max)
                    {
                    ptable->updated = TRUE;
                    goto breakout;
                    }
                pdata->n_valid_rows--;
                pinfo->n_valid_rows--;
                pdata->sum_x -= x;
                pinfo->sum_x -= x;
                x *= x;
                pdata->sum_x_squared -= x;
                pinfo->sum_x_squared -= x;
                }
            }
        }
    /* Now move the new data into place */
breakout:
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        pdata = ptable->data_ptr + Col_List[i] - 1;
        data_size = pinfo->data_size;
        /*   replace rows.                         */
        if (!pinfo->inc_value)
            {
            hoblockmove ((VOIDPTR)Var_List[i],
                         (VOIDPTR)(pdata->data + (currentPoint - 1) * data_size),
                         replacePoints * data_size);
            
            if (appendPoints > 0)
                {
                append_value_list[i] = (VOIDPTR)(((BYTE *)Var_List[i]) +
                                                 replacePoints * data_size);
                }
            }
        }
    /*   update the # of valid rows.                */
    if (NOT ptable->updated)
        {
        nlz_update_valid_rows (nlist, replacePoints, nCol, Col_List, Var_List);
        }
    ptable->data_loaded = NL_DATA_UPDATED;
    
    /* If the total of new rows is greater
       than existing rows, append the rest of rows to the end. */
    
    if (appendPoints > 0)
        {
        nlist->current_row = ptable->nrows;
        status = nl_add_rows (nlist, appendPoints, nCol, Col_List, append_value_list);
        nlist->current_row = currentPoint;
        }
    /*   free old allocated memory.              */
    
    tc_free ((VOIDPTR)append_value_list);
    nlist->updated = ptable->updated;
    return status;
    }
/* END:     */
