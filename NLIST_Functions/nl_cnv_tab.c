/* DEC/CMS REPLACEMENT HISTORY, Element NL_CNV_TAB.C*/
/* *5    25-OCT-1990 12:21:39 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:56:57 VINCE "(SPR 5644) Header Standardization"*/
/* *3     9-APR-1990 10:32:31 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    12-MAR-1990 19:06:38 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:58:41 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CNV_TAB.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Function to convert the request data for specified table.

Prototype:
    publicdef INT nl_convert_table_info (NLIST_HEADER nlist, UINT table_num, INT opcode, 
        UINT ndim, UINT *Col_List, VOIDPTR *value_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    ndim            -(UINT)
    Col_List        -(UINT *)
    opcode          -(INT)
    value_list      -(VOIDPTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_convert_table_info (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                     UINT ndim, UINT *Col_List, VOIDPTR *value_list)
#else
publicdef INT nl_convert_table_info (nlist, table_num, opcode, ndim, Col_List,
                                     value_list)
NLIST_HEADER nlist;
UINT table_num, ndim;
UINT *Col_List;
INT opcode;
VOIDPTR *value_list;
#endif
    {
    UINT i, total_tables = 1;
    INT status = SUCCESS;
    UINT target_table = 0;
    UINT old_table_num, old_row_num;
    TABLE_HEADER *pstk;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    
    /* error checking.    */
    INIT_TEST (nlist);
    
    nl_inq_current_row (nlist, &old_row_num, &old_table_num);
    if (table_num == 0)
        {
        target_table = 1;
        status = nl_inq_nlist_int (nlist, NL_NLIST_NTABLES, (INT *)&total_tables);
        }
    else
        {
        target_table = table_num;
        total_tables = 1;
        }
    if (total_tables == 1)
        {
        pstk = nlz_set_table (nlist, target_table);
        if (pstk == NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        for (i = 0; i < ndim; i++)
            {
            COLUMN_TEST (nlist, Col_List[i]);
            }
        /* analyze the opcode and do operation.    */
        for (i = 0; i < ndim; i++)
            {
            pinfo = nlist->info_ptr + Col_List[i] - 1;
            pdata = pstk->data_ptr + Col_List[i] - 1;
            switch (opcode)
                {
            case NL_TABLE_INCREMENT:
                if (pdata->inc_value)
                    {
                    switch (pinfo->data_type)
                        {
                    case INT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(CHAR):
                            pdata->increment = pdata->increment * *
                                               (CHAR *)value_list[i];
                            break;
                        case sizeof(SHORT):
                            pdata->increment = pdata->increment * *
                                               (SHORT *)value_list[i];
                            break;
                        case sizeof(INT):
                            pdata->increment = pdata->increment * *
                                (INT *)value_list[i];
                            break;
                            }
                        break;
                    case FLOAT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(FLOAT):
                            pdata->increment = pdata->increment * *
                                               (FLOAT *)value_list[i];
                            break;
                        case sizeof(DOUBLE):
                            pdata->increment = pdata->increment * *
                                               (DOUBLE *)value_list[i];
                            break;
                            }
                        break;
                        }
                    }
                break;
                
            case NL_TABLE_START_VALUE:
                if (pdata->inc_value)
                    {
                    switch (pinfo->data_type)
                        {
                    case INT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(CHAR):
                            pdata->start_value = pdata->start_value * *
                                                 (CHAR *)value_list[i];
                            break;
                        case sizeof(SHORT):
                            pdata->start_value = pdata->start_value * *
                                                 (SHORT *)value_list[i];
                            break;
                        case sizeof(INT):
                            pdata->start_value = pdata->start_value * *
                                                 (INT *)value_list[i];
                            break;
                            }
                        break;
                    case FLOAT_TYPE:
                        switch (pinfo->data_size)
                            {
                        case sizeof(FLOAT):
                            pdata->start_value = pdata->start_value * *
                                                 (FLOAT *)value_list[i];
                            break;
                        case sizeof(DOUBLE):
                            pdata->start_value = pdata->start_value * *
                                                 (DOUBLE *)value_list[i];
                            break;
                            }
                        break;
                        }
                    }
                break;
                
            default:
                status = NL_INVALID_CODE;
                }
            }
        }
    else
        {
        for (target_table = 1; target_table <= total_tables; target_table++)
            {
            pstk = nlz_set_table (nlist, target_table);
            if (pstk == NULL_TABLE)
                {
                return NL_INVALID_TABLE;
                }
            for (i = 0; i < ndim; i++)
                {
                COLUMN_TEST (nlist, Col_List[i]);
                }
            /* analyze the opcode and do operation.    */
            for (i = 0; i < ndim; i++)
                {
                pinfo = nlist->info_ptr + Col_List[i] - 1;
                pdata = pstk->data_ptr + Col_List[i] - 1;
                switch (opcode)
                    {
                case NL_TABLE_INCREMENT:
                    if (pdata->inc_value)
                        {
                        switch (pinfo->data_type)
                            {
                        case INT_TYPE:
                            switch (pinfo->data_size)
                                {
                            case sizeof(CHAR):
                                pdata->increment = pdata->increment * *
                                                   (CHAR *)value_list[i];
                                break;
                            case sizeof(SHORT):
                                pdata->increment = pdata->increment * *
                                                   (SHORT *)value_list[i];
                                break;
                            case sizeof(INT):
                                pdata->increment = pdata->increment * *
                                                   (INT *)value_list[i];
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (pinfo->data_size)
                                {
                            case sizeof(FLOAT):
                                pdata->increment = pdata->increment * *
                                                   (FLOAT *)value_list[i];
                                break;
                            case sizeof(DOUBLE):
                                pdata->increment = pdata->increment * *
                                                   (DOUBLE *)value_list[i];
                                break;
                                }
                            break;
                            }
                        }
                    break;
                    
                case NL_TABLE_START_VALUE:
                    if (pdata->inc_value)
                        {
                        switch (pinfo->data_type)
                            {
                        case INT_TYPE:
                            switch (pinfo->data_size)
                                {
                            case sizeof(CHAR):
                                pdata->start_value = pdata->start_value * *
                                                     (CHAR *)value_list[i];
                                break;
                            case sizeof(SHORT):
                                pdata->start_value = pdata->start_value * *
                                                     (SHORT *)value_list[i];
                                break;
                            case sizeof(INT):
                                pdata->start_value = pdata->start_value * *
                                                     (INT *)value_list[i];
                                break;
                                }
                            break;
                        case FLOAT_TYPE:
                            switch (pinfo->data_size)
                                {
                            case sizeof(FLOAT):
                                pdata->start_value = pdata->start_value * *
                                                     (FLOAT *)value_list[i];
                                break;
                            case sizeof(DOUBLE):
                                pdata->start_value = pdata->start_value * *
                                                     (DOUBLE *)value_list[i];
                                break;
                                }
                            break;
                            }
                        }
                    break;
                    
                default:
                    status = NL_INVALID_CODE;
                    }
                }
            }
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
