/* DEC/CMS REPLACEMENT HISTORY, Element NL_ADD_ROWS.C */
/* *7    25-OCT-1990 12:20:37 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *6    25-JUL-1990 14:55:34 VINCE "(SPR 5644) Header Standardization" */
/* *5     9-APR-1990 10:32:26 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *4    22-MAR-1990 01:35:01 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *3    12-MAR-1990 19:05:59 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:45:51 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:34:37 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_ADD_ROWS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_ADD_POINTS.C*/
/* *1    19-JUN-1989 13:17:38 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_ADD_POINTS.C*/
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
    Add one or more rows after current row.

Prototype:
    publicdef INT nl_add_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist       -(NLIST_HEADER) N-List header.
    (I) nrows       -(UINT) Number of rows to be added.
    (I) nCol        -(UINT) Number of columns referenced.
    (I) Col_List    -(UINT *) Array of column <nCol> index numbers.
    (I) Var_List    -(VOIDPTR *) Array of addresses of start of data for each
                        column.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    NL_INVALID_TABLE
    NL_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_add_rows (NLIST_HEADER nlist, UINT nrows, UINT nCol, UINT *Col_List,
                           VOIDPTR *Var_List)
#else
publicdef INT nl_add_rows (nlist, nrows, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nrows;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    TABLE_HEADER *ptable;           /*   workspace for the current table.        */
    /*-----------------------------------*/
    /* the pointer of data header of the */
    /* specified column.                 */
    /*-----------------------------------*/
    register DATA_HEADER *pdata;
    
    /*-------------------------------*/
    /* pointer to info header of the */
    /* specified column.             */
    /*-------------------------------*/
    register COLUMN_HEADER *pinfo;
    
    BYTE *new_data_ptr;             /*   the pointer of data.                    */
    VOIDPTR temp_data_ptr;          /*   temporary data pointer                  */
    size_t data_size;               /*   working space for data size.            */
    register UINT i, j;             /*   indexes.                                */
    UINT current_row;               /*   working space of current row.           */
    INT status = SUCCESS;           /*   returned flag.                          */
    
    /*  ************************************************************************** */
    /* error checking.      */
    INIT_TEST (nlist);
    
    /* it is invalid to add 0 rows */
    NROWS_TEST (nrows);
    
    current_row = nlist->current_row;
    ptable = nlz_set_table (nlist, nlist->current_table);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    /* check each data pointer and make sure it is valid (non-zero) */
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] EQUALS (VOIDPTR)NULL)
            {
            return NL_INVALID_POINTER;
            }
        }
    /* if the data has not been loaded,
       then load the data first. */
    
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows)
        {
        status = nlz_load_data (nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /* check each column for sufficient room
       to add the additional data */
    if (ptable->nrows + nrows > ptable->nallocated)
        {
        /* need to add an additional data block
           to each column that is not
           an incrementing column */
        
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
                    }
                pdata->data = new_data_ptr;
                }
            }
        ptable->nallocated += (nlist->blocksize + nrows);
        }
    /*
       * shift the data over to make room for the new data if this is not the
       * last row...
    */
    
    if (current_row IS_NOT_EQUAL_TO ptable->nrows)
        {
        for (i = 0; i < nCol; i++)
            {
            pdata = ptable->data_ptr + Col_List[i] - 1;
            if (pdata->inc_value EQUALS FALSE)
                {
                pinfo = nlist->info_ptr + Col_List[i] - 1;
                data_size = pinfo->data_size;
                
                /* create a temp. space for copying.     */
                
                temp_data_ptr = tc_alloc (ptable->nrows * data_size);
                if (temp_data_ptr == (VOIDPTR)0)
                    {
                    return NL_OVERFLOW;
                    }
                /* move the data after current row to the temperary space.    */
                
                hoblockmove ((VOIDPTR)(pdata->data + current_row * data_size),
                             temp_data_ptr, (ptable->nrows - current_row) * data_size);
                
                /* move the data back from the temperary space .   */
                
                hoblockmove (temp_data_ptr,
                             (VOIDPTR)(pdata->data + (current_row + nrows) *
                                       data_size),
                             (ptable->nrows - current_row) * data_size);
                tc_free (temp_data_ptr);
                temp_data_ptr = (VOIDPTR)0;
                }
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
        /*
           * set all non-incrementing columns to null
        */
        for (i = 0, pinfo = nlist->info_ptr, pdata = ptable->data_ptr; i < nlist->nCol;
             i++, pinfo++, pdata++)
            {
            new_data_ptr = pdata->data + (current_row)*pinfo->data_size;
            if (pdata->inc_value EQUALS FALSE)
                {
                switch (pinfo->data_type)
                    {
                    
                    /* ----------------------------------------  */
                    
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        for (j = 0; j < nrows; j++)*new_data_ptr++ = pinfo->null_value;
                        break;
                    case sizeof(SHORT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(SHORT))
                            *(SHORT *)new_data_ptr = pinfo->null_value;
                        break;
                    case sizeof(INT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(INT))
                            *(INT *)new_data_ptr = pinfo->null_value;
                        break;
                        }
                    break;
                    
                    /*   --------------------------------------  */
                    
                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(FLOAT))
                            *(FLOAT *)new_data_ptr = pinfo->null_value;
                        break;
                    case sizeof(DOUBLE):
                        for (j = 0; j < nrows; j++, new_data_ptr += sizeof(DOUBLE))
                            *(DOUBLE *)new_data_ptr = pinfo->null_value;
                        break;
                        }
                    break;
                    }
                }
            }
        }
    /*
       * now move the new data values for each column into place
    */
    for (i = 0; i < nCol; i++)
        {
        pdata = ptable->data_ptr + Col_List[i] - 1;
        if (pdata->inc_value EQUALS FALSE)
            {
            pinfo = nlist->info_ptr + Col_List[i] - 1;
            data_size = pinfo->data_size;
            hoblockmove ((VOIDPTR)Var_List[i],
                         (VOIDPTR)(pdata->data + current_row * data_size),
                         nrows * data_size);
            }
        }
    nlist->current_row = current_row + nrows;
    nlist->total_rows += nrows;
    ptable->nrows += nrows;
    
    /*
     * to update the total number of valid rows and statistics
     */
    nlz_update_valid_rows (nlist, nrows, nCol, Col_List, Var_List);
    
    ptable->data_loaded = NL_DATA_UPDATED;
    return status;
    }
/* END:     */
