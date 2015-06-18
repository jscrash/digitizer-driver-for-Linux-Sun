/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_ROWS.C */
/* *8    25-OCT-1990 12:23:00 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *7    25-JUL-1990 16:45:50 VINCE "(SPR 5644) Fixed compile problems introduced with the new header" */
/* *6    25-JUL-1990 14:59:45 VINCE "(SPR 5644) Header Standardization" */
/* *5     9-APR-1990 10:33:06 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *4    22-MAR-1990 01:35:15 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *3    12-MAR-1990 19:07:11 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:46:46 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:25 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_ROWS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_POINTS.C*/
/* *1    19-JUN-1989 13:19:04 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INS_POINTS.C*/
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
    Adds one or more new rows to an N-list before the current row.

Prototype:
    publicdef INT nl_insert_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT Col_List[], VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.       
    nCol            -(UINT) Number of columns requested.       
    nrows           -(UINT) Number of rows requested.   
    Col_List        -(UINT) List of columns requested.  
    Var_List        -(VOIDPTR *) List of pointers row to the value.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_DATA_SIZE
    NL_INVALID_POINTER
    NL_OVERFLOW
    NL_EMPTY_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_insert_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol,
                              UINT Col_List[], VOIDPTR *Var_List)
#else
publicdef INT nl_insert_rows (nlist, nrows, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT nrows;
UINT Col_List[];
VOIDPTR *Var_List;
#endif
    {
    TABLE_HEADER *ptable;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    BYTE *new_data_ptr;
    VOIDPTR temp_data_ptr;
    size_t data_size;
    UINT i, j;
    UINT current_row;
    INT status = SUCCESS;
    
    /*  *************************************************************************  */
    /*   error checking.                         */
    
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    
    current_row = nlist->current_row;
    ptable = nlz_set_table (nlist, nlist->current_table);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_EMPTY_TABLE;
        }
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] EQUALS (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        }
    /*   if the table has not been loaded,      */
    /*   then loads first.                       */
    
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows)
        {
        status = nlz_load_data (nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    if (ptable->nrows + nrows > ptable->nallocated)
        {
        /*   need to add an additional data block to */
        /*   each column that is not an           */
        /*   incrementing column.                 */
        
        for (i = 0, pdata = ptable->data_ptr, pinfo = nlist->info_ptr; i < nlist->nCol;
             i++, pdata++, pinfo++)
            {
            if (pdata->inc_value EQUALS FALSE)
                {
                data_size = pinfo->data_size;
                new_data_ptr = (BYTE *) tc_alloc ((ptable->nallocated + nrows +
                                                   nlist->blocksize) * data_size);
                if (new_data_ptr == (BYTE *)0)
                    {
                    return NL_OVERFLOW;
                    }
                if (pdata->data IS_NOT_EQUAL_TO (BYTE *)NULL)
                    {
                    hoblockmove ((VOIDPTR)pdata->data, (VOIDPTR)new_data_ptr,
                                 ptable->nallocated * data_size);
                    tc_free ((VOIDPTR)pdata->data);
                    pdata->data = (BYTE *)0;
                    }
                pdata->data = new_data_ptr;
                }
            else                    /*   if the current row is 1, then change  */
                                    /*   the start_value.                        */
                
                if (current_row EQUALS 1)
                    {
                    pdata->start_value -= pdata->increment * nrows;
                    }
            }
        ptable->nallocated += (nlist->blocksize + nrows);
        }
    else if (current_row EQUALS 1)
        {
        for (i = 0, pdata = ptable->data_ptr, pinfo = nlist->info_ptr; i < nlist->nCol;
             i++, pdata++, pinfo++)
            {
            if (pdata->inc_value)
                
                /*   if the current row is 1, then change  */
                /*   the start_value.                        */
                
                pdata->start_value -= pdata->increment * nrows;
            }
        }
    /*   shift the data over to make room for the*/
    /*   new data.                               */
    
    for (i = 0; i < nCol; i++)
        {
        pdata = ptable->data_ptr + Col_List[i] - 1;
        if (pdata->inc_value EQUALS FALSE)
            {
            pinfo = nlist->info_ptr + Col_List[i] - 1;
            data_size = pinfo->data_size;
            
            /* create a temp. space for copying.          */
            
            temp_data_ptr = (VOIDPTR) tc_alloc (ptable->nrows * data_size);
            if (temp_data_ptr == (VOIDPTR)0)
                {
                return NL_OVERFLOW;
                }
            /*   move the data after current row to the */
            /*   temperary space.                         */
            
            hoblockmove ((VOIDPTR)(pdata->data + (current_row - 1) * data_size),
                         temp_data_ptr, (ptable->nrows - current_row + 1) * data_size);
            
            /*   move the data back from the temperary    */
            /*   space .                                  */
            
            hoblockmove (temp_data_ptr,
                         (VOIDPTR)(pdata->data + (current_row + nrows - 1) *
                                   data_size),
                         (ptable->nrows - current_row + 1) * data_size);
            tc_free (temp_data_ptr);
            temp_data_ptr = (VOIDPTR)0;
            }
        }
    /*
       * the following set of code is executed only when the programmer
       * provides a number of columns which indicates that there is incomplete
       * data for the row to be added.  For example, three columns are
       * defined, none are incrementing, but nCol is 2.  This implies that
       * one column is null.  Prior to loading the data from the value list,
       * all data values are set to the null value for the column.
       * I have chosen this method of implementation
       * since I cannot guarentee that the column list will be in order
       *
    */
    
    if (nCol < ptable->n_value_cols)
        {
        
        /*   set all non-incrementing columns to  */
        /*   null.                                   */
        
        for (i = 0, pinfo = nlist->info_ptr, pdata = ptable->data_ptr; i < nlist->nCol;
             i++, pinfo++, pdata++)
            {
            new_data_ptr = pdata->data + (current_row - 1) * pinfo->data_size;
            if (pdata->inc_value EQUALS FALSE)
                {
                switch (pinfo->data_type)
                    {
                    /*   --------------------------------------  */
                    
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        for (j = 0; j < nrows;
                                j++)*new_data_ptr++ = (BYTE)pinfo->null_value;
                        break;
                    case sizeof(SHORT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(SHORT))
                            *(SHORT *)new_data_ptr = (SHORT)pinfo->null_value;
                        break;
                    case sizeof(INT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(INT))
                            *(INT *)new_data_ptr = (INT)pinfo->null_value;
                        break;
                    default:
                        return NL_INVALID_DATA_SIZE;
                        }
                    break;
                    
                    /*   ---------------------------------  */
                    
                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(FLOAT))
                            *(FLOAT *)new_data_ptr = (FLOAT)pinfo->null_value;
                        break;
                    case sizeof(DOUBLE):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(DOUBLE))
                            *(DOUBLE *)new_data_ptr = pinfo->null_value;
                        break;
                    default:
                        return NL_INVALID_DATA_SIZE;
                        }
                    break;
                    }
                }
            }
        }
    /*   now move the new data values for each   */
    /*   column into place.                   */
    
    for (i = 0; i < nCol; i++)
        {
        pdata = ptable->data_ptr + Col_List[i] - 1;
        if (pdata->inc_value EQUALS FALSE)
            {
            pinfo = nlist->info_ptr + Col_List[i] - 1;
            data_size = pinfo->data_size;
            hoblockmove (Var_List[i],
                         (VOIDPTR)(pdata->data + (current_row - 1) * data_size),
                         nrows * data_size);
            }
        }
    nlist->current_row = current_row + nrows;
    nlist->total_rows += nrows;
    ptable->nrows += nrows;
    
    /* update the total number of valid rows and statistics */
    
    nlz_update_valid_rows (nlist, nrows, nCol, Col_List, Var_List);
    
    /*   change statistics on row addition     */
    
    ptable->data_loaded = NL_DATA_UPDATED;
    return status;
    }
/* END:     */
