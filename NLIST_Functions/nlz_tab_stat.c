/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_TAB_STAT.C*/
/* *7    25-OCT-1990 12:20:15 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *6    25-JUL-1990 14:55:15 VINCE "(SPR 5644) Header Standardization"*/
/* *5    23-MAR-1990 22:11:34 GILLESPIE "(SPR 1) More stats changes"*/
/* *4    22-MAR-1990 01:34:52 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *3    17-MAR-1990 17:10:08 GILLESPIE "(SPR 1) Set nlist->updated flag if this table was updated"*/
/* *2    12-MAR-1990 19:05:26 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:16 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_TAB_STAT.C*/
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

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Scan the column and reset the statistics for the data.
    Table number becomes new current table.
    Current row set to first row in this table on return.

Prototype:
    publicdef INT nlz_compute_table_stats (NLIST_HEADER nlist, UINT table_num);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    ASSUMPTION: caller has made sure data are loaded into memory
    by calling nlz_load_data, if necessary.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_compute_table_stats (NLIST_HEADER nlist, UINT table_num)
#else
publicdef INT nlz_compute_table_stats (nlist, table_num)
NLIST_HEADER nlist;
UINT table_num;
#endif
    {
    TABLE_HEADER *ptable;
    register COLUMN_HEADER *pinfo;
    register DATA_HEADER *pdata;
    register UINT i;
    UINT nrows;
    register UINT current_row;
    UINT old_row_num, old_table_num;
    DOUBLE value;
    INT status = SUCCESS;
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        nlist->current_row = old_row_num;
        nlist->current_table = old_table_num;
        return NL_INVALID_TABLE;
        }
    if (NOT ptable->updated)
        {
        nlist->current_row = old_row_num;
        nlist->current_table = old_table_num;
        return SUCCESS;
        }
    /*
     * reset the statistics counters and registers for the table
     */
    
    for (i = 0, pdata = ptable->data_ptr; i < nlist->nCol; i++, pdata++)
        {
        pdata->min = HUGE_VAL;
        pdata->max = -HUGE_VAL;
        pdata->sum_x = 0.0;
        pdata->sum_x_squared = 0.0;
        pdata->n_valid_rows = 0;
        }
    /*
     * this will scan all rows in the table and update the table stats
     */
    for (current_row = 1, nrows = ptable->nrows; current_row <= nrows; current_row++)
        {
        for (i = 0, pinfo = nlist->info_ptr, pdata = ptable->data_ptr; i < nlist->nCol;
             i++, pinfo++, pdata++)
            {
            if ((nlz_get_value (pdata, pinfo, current_row,
                                &value) < 0)OR (value EQUALS pinfo->null_value))
                {
                continue;
                }
            /*
             * bump up the counters and stats for the table and list
             */
            
            pdata->sum_x += value;
            pdata->sum_x_squared += value * value;
            pdata->n_valid_rows++;
            if (value < pdata->min)
                {
                pdata->min = value;
                }
            if (value > pdata->max)
                {
                pdata->max = value;
                }
            }
        }
    /* reset current row */
    nlist->current_row = old_row_num;
    nlist->current_table = old_table_num;
    ptable->updated = FALSE;
    if (ptable->nrows > 0)
        {
        nlist->updated = TRUE;
        }
    return status;
    }
/* END:     */
