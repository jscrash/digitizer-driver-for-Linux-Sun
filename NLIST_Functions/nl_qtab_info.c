/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INFO.C */
/* *8    25-OCT-1990 12:25:45 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *7    25-JUL-1990 15:04:12 VINCE "(SPR 5644) Header Standardization" */
/* *6    29-MAR-1990 18:08:08 GILLESPIE "(SPR 1) Clean up logic" */
/* *5    22-MAR-1990 01:36:03 GILLESPIE "(SPR 1) Correct problems with stats on delete" */
/* *4    12-MAR-1990 19:09:21 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *3     5-MAR-1990 10:49:32 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *2    25-SEP-1989 11:15:34 GILLESPIE "(SPR 200) Add esi_nl_msg.h" */
/* *1    24-SEP-1989 22:36:02 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INFO.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INFO.C*/
/* *1    19-JUN-1989 13:20:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INFO.C*/
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
#include "esi_nl_msg.h"
#include "esi_math.h"

/* Function Description -----------------------------------------------------
Description:
    To retrieve information for the specified columns
    for the specified table.

Prototype:
    publicdef INT nl_inq_table_info (NLIST_HEADER nlist, UINT table_num, INT type, 
        UINT nCol, UINT *Col_List, VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    nCol            -(UINT)
    type            -(INT)
    Col_List        -(UINT *)
    Var_List        -(VOIDPTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NULL_VALUES_RETURNED
    NL_INVALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_table_info (NLIST_HEADER nlist, UINT table_num, INT type,
                                 UINT nCol, UINT *Col_List, VOIDPTR *Var_List)
#else
publicdef INT nl_inq_table_info (nlist, table_num, type, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT table_num, nCol;
INT type;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    register UINT i;
    INT status = SUCCESS;
    UINT old_table_num, old_row_num;
    TABLE_HEADER *ptable;
    register DATA_HEADER *pdata;
    register COLUMN_HEADER *pinfo;
    DOUBLE avge = 0.0, result = 0.0;
    
    /* error checking.    */
    INIT_TEST (nlist);
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    
    if (table_num EQUALS 0)
        {
        table_num = nlist->current_table;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        return NL_INVALID_TABLE;
    if (ptable->nrows EQUALS 0)
        return NL_EMPTY_TABLE;
    
    /* to compute the status of specified table.     */
    if (ptable->
        updated AND (
               type EQUALS NL_TABLE_AVG OR type EQUALS NL_TABLE_STD_DEV OR type EQUALS 
                     NL_TABLE_MAX OR type EQUALS NL_TABLE_MIN))
        {
        nlist->defer_stats = FALSE;
        
        /*
         * if the table has not been loaded, then loads first.
         */
        if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows > 0)
            {
            if ((status = nlz_load_data (nlist))IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            }
        nlz_compute_table_stats (nlist, table_num);
        nlist->defer_stats = TRUE;
        }
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        pdata = ptable->data_ptr + Col_List[i] - 1;
        switch (type)
            {
        case NL_TABLE_AVG:
            if (pdata->min EQUALS HUGE_VAL OR pdata->n_valid_rows EQUALS 0)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                result = pdata->sum_x / pdata->n_valid_rows;
                }
            nlz_move_data (Var_List[i], FLOAT_TYPE, sizeof(DOUBLE), result);
            continue;
            
        case NL_TABLE_INCREMENT:
            if (pdata->inc_value)
                {
                result = pdata->increment;
                }
            else
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            break;
        case NL_TABLE_MAX:
            if (pdata->min EQUALS HUGE_VAL)
                {
                result = pinfo->null_value;
                }
            else
                {
                result = pdata->max;
                }
            break;
            
        case NL_TABLE_MIN:
            if (pdata->min EQUALS HUGE_VAL)
                {
                result = pinfo->null_value;
                }
            else
                {
                result = pdata->min;
                }
            break;
            
        case NL_TABLE_NON_NULL:
            *((INT *)Var_List[i]) = (INT)pdata->n_valid_rows;
            continue;
            
        case NL_TABLE_STD_DEV:
            if (pdata->min EQUALS HUGE_VAL OR pdata->n_valid_rows EQUALS 0)
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            else
                {
                avge = pdata->sum_x / pdata->n_valid_rows;
                result = pdata->sum_x_squared - 2 * avge * pdata->sum_x +
                         pdata->n_valid_rows * avge * avge;
                result = (DOUBLE)sqrt ((double)(result / pdata->n_valid_rows));
                }
            nlz_move_data (Var_List[i], FLOAT_TYPE, sizeof(DOUBLE), result);
            continue;
            
        case NL_TABLE_START_VALUE:
            if (pdata->inc_value)
                {
                result = pdata->start_value;
                }
            else
                {
                status = NL_NULL_VALUES_RETURNED;
                result = pinfo->null_value;
                }
            break;
        default:
            nl_set_current_row (nlist, old_row_num, old_table_num);
            return NL_INVALID_CODE;
            }
        nlz_move_data (Var_List[i], pinfo->data_type, pinfo->data_size, result);
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
