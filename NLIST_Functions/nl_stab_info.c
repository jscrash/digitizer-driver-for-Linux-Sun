/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_INFO.C */
/* *6    25-OCT-1990 12:27:26 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *5    25-JUL-1990 15:06:54 VINCE "(SPR 5644) Header Standardization" */
/* *4     9-APR-1990 10:34:27 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *3    24-MAR-1990 11:10:08 GILLESPIE "(SPR 1) Check var_list for null pointers" */
/* *2     5-MAR-1990 10:51:16 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:54 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_INFO.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STSTR_INFO.C*/
/* *1    19-JUN-1989 13:21:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STSTR_INFO.C*/
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

/* Function Description -----------------------------------------------------
Description:
    To set the request data for specified table.

Prototype:
    publicdef INT nl_set_table_info (NLIST_HEADER nlist, UINT table_num, INT opcode, 
        UINT nCol, UINT *Col_List, VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    opcode          -(INT)
    nCol            -(UINT)
    Col_List        -(UINT *)
    Var_List        -(VOIDPTR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_INVALID_POINTER
    NL_INVALID_CODE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_table_info (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                 UINT nCol, UINT *Col_List, VOIDPTR *Var_List)
#else
publicdef INT nl_set_table_info (nlist, table_num, opcode, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT table_num;
INT opcode;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    UINT i;
	INT status = SUCCESS;
    UINT old_table_num, old_row_num;
    TABLE_HEADER *ptable;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    
    /* error checking.    */
    INIT_TEST (nlist);
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    if (table_num EQUALS 0)
        {
        table_num = old_table_num;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] == (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        }
    /* analyze the opcode and do operation.    */
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        pdata = ptable->data_ptr + Col_List[i] - 1;
        switch (opcode)
            {
        case NL_TABLE_INCREMENT:
            if (pdata->inc_value)
                {
                nlist->updated = ptable->updated = TRUE;
                switch (pinfo->data_type)
                    {
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        pdata->increment = (DOUBLE)*(CHAR *)Var_List[i];
                        break;
                    case sizeof(SHORT):
                        pdata->increment = (DOUBLE)*(SHORT *)Var_List[i];
                        break;
                    case sizeof(INT):
                        pdata->increment = (DOUBLE)*(INT *)Var_List[i];
                        break;
                        }
                    break;
                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        pdata->increment = (DOUBLE)*(FLOAT *)Var_List[i];
                        break;
                    case sizeof(DOUBLE):
                        pdata->increment = *(DOUBLE *)Var_List[i];
                        break;
                        }
                    break;
                    }
                }
            break;
            
        case NL_TABLE_START_VALUE:
            if (pdata->inc_value)
                {
                nlist->updated = ptable->updated = TRUE;
                switch (pinfo->data_type)
                    {
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        pdata->start_value = (DOUBLE)*(CHAR *)Var_List[i];
                        break;
                    case sizeof(SHORT):
                        pdata->start_value = (DOUBLE)*(SHORT *)Var_List[i];
                        break;
                    case sizeof(INT):
                        pdata->start_value = (DOUBLE)*(INT *)Var_List[i];
                        break;
                        }
                    break;
                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        pdata->start_value = (DOUBLE)*(FLOAT *)Var_List[i];
                        break;
                    case sizeof(DOUBLE):
                        pdata->start_value = *(DOUBLE *)Var_List[i];
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
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
