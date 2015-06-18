/* DEC/CMS REPLACEMENT HISTORY, Element NL_START_TAB.C*/
/* *7    25-OCT-1990 12:27:44 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *6    25-JUL-1990 15:07:28 VINCE "(SPR 5644) Header Standardization"*/
/* *5     9-APR-1990 10:54:37 GILLESPIE "(SPR 1) Use nlz_set_table"*/
/* *4    12-MAR-1990 19:10:15 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *3     5-MAR-1990 10:51:38 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2     7-NOV-1989 14:25:36 GILLESPIE "(SPR 1) Change allocation of new tables to double the storage rather than add 10"*/
/* *1    24-SEP-1989 22:37:02 GILLESPIE "New n-List V4.0"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_START_TAB.C*/
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
#include "esi_tc.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_start_table (NLIST_HEADER nlist, UINT *table_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_start_table (NLIST_HEADER nlist, UINT *table_num)
#else
publicdef INT nl_start_table (nlist, table_num)
NLIST_HEADER nlist;
UINT *table_num;
#endif
    {
    UINT i;
    UINT nCol;
    TABLE_HEADER *ptable;
    TABLE_HEADER *pLastTable;
    DATA_HEADER *pdata, *pLastTableData;
    COLUMN_HEADER *pinfo_ptr;
    TABLE_ELEMENT *old_table_elements;
    
    INIT_TEST (nlist);
    
    ptable = (TABLE_HEADER *) tc_zalloc (sizeof(TABLE_HEADER) + sizeof(DATA_HEADER) *
                                         (nlist->nCol - 1));
    
    if (ptable == (TABLE_HEADER *)0)
        {
        return NL_OVERFLOW;
        }
    nCol = nlist->nCol;
    if (nlist->total_tables > 0)
        {
        /*
           * set up the link list pointers for the new table
        */
        pLastTable = nlz_set_table (nlist, nlist->total_tables);    /* set to last table */
        if (pLastTable EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        /*
           * set the default value in the new table header as same as previous table.
        */
        pLastTableData = pLastTable->data_ptr;
        pdata = ptable->data_ptr;
        for (i = 0; i < nCol; i++, pLastTableData++, pdata++)
            {
            pdata->inc_value = pLastTableData->inc_value;
            if (pdata->inc_value)
                {
                pdata->increment = pLastTableData->increment;
                pdata->start_value = pLastTableData->start_value +
                    (pLastTableData->increment * pLastTable->nrows);
                }
            }
        ptable->n_value_cols = pLastTable->n_value_cols;
        }
    else
        {
        
        /*
           * set the default value in the new table header as same as COLUMN_HEADER.
        */
        pinfo_ptr = nlist->info_ptr;
        pdata = ptable->data_ptr;
        for (i = 0; i < nCol; i++, pinfo_ptr++, pdata++)
            {
            pdata->inc_value = pinfo_ptr->inc_value;
            }
        ptable->n_value_cols = nlist->n_value_cols;
        }
    /* 
        * update the table array.
    */
    if (nlist->nallocated_tables EQUALS 0)
        {
        nlist->nallocated_tables = 10;
        nlist->table_elements = (TABLE_ELEMENT *) tc_zalloc (sizeof(TABLE_ELEMENT) *
                                                             10);
        if (nlist->table_elements == (TABLE_ELEMENT *)0)
            {
            return NL_OVERFLOW;
            }
        }
    else if (nlist->nallocated_tables EQUALS nlist->total_tables)
        {
        /*       The reason I don't call tc_realloc()
               directly is that we are not sure the
               returned memory is zeroed out.        */
        
        nlist->nallocated_tables *= 2;
        old_table_elements = (TABLE_ELEMENT *)nlist->table_elements;
        nlist->table_elements = (TABLE_ELEMENT *) tc_zalloc (sizeof(TABLE_ELEMENT) *
                                                             nlist->nallocated_tables);
        if (nlist->table_elements == (TABLE_ELEMENT *)0)
            {
            return NL_OVERFLOW;
            }
        hoblockmove ((VOIDPTR)old_table_elements, (VOIDPTR)nlist->table_elements,
                     sizeof(TABLE_ELEMENT) * nlist->total_tables);
        tc_free ((VOIDPTR)old_table_elements);
        }
    nlist->table_elements[nlist->total_tables].ptable = nlist->current_table_ptr =
        ptable;
    *table_num = nlist->current_table = ++(nlist->total_tables);
    nlist->current_row = 0;
    
    /* The following resets statistics for the stroke header */
    ptable->updated = TRUE;
    nlz_compute_table_stats (nlist, nlist->current_table);
    
    return SUCCESS;
    }
/* END:     */
