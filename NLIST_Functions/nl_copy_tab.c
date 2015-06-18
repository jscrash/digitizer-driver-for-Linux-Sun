/* DEC/CMS REPLACEMENT HISTORY, Element NL_COPY_TAB.C*/
/* *7    25-OCT-1990 12:21:57 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *6    25-JUL-1990 14:57:24 VINCE "(SPR 5644) Header Standardization"*/
/* *5     1-JUN-1990 12:57:20 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *4    13-APR-1990 10:56:55 GILLESPIE "(SPR 1) Minor cleanup"*/
/* *3     9-APR-1990 10:32:37 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    12-MAR-1990 19:06:54 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:58:59 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_COPY_TAB.C*/
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
#include "esi_c_lib.h"
#include "esi_ho.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Create a copy of an n-List table.

Prototype:
    publicdef INT nl_copy_table (NLIST_HEADER from_list, UINT from_Table,
                             NLIST_HEADER to_list, UINT *to_Table);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    from_list       -(NLIST_HEADER)
    to_list         -(NLIST_HEADER)
    from_Table      -(UINT)
    to_Table        -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_TWO_LISTS_NOT_SAME_DEF
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_copy_table (NLIST_HEADER from_list, UINT from_Table,
                             NLIST_HEADER to_list, UINT *to_Table)
#else
publicdef INT nl_copy_table (from_list, from_Table, to_list, to_Table)
NLIST_HEADER from_list, to_list;
UINT from_Table, *to_Table;
#endif
    {
    register UINT i, j;
    size_t size;
    UINT table_num;
    register TABLE_HEADER *poldtab;
    register TABLE_HEADER *pnewtab;
    register COLUMN_HEADER *poldinfo;
    register COLUMN_HEADER *pnewinfo;
    DATA_HEADER *polddata;
    DATA_HEADER *pnewdata;
    INT status;
    UINT original_point;
    UINT original_Table;
    
    /* error checking     1st. check the initialization.     */
    INIT_TEST (from_list);
    INIT_TEST (to_list);
    
    /*
       * error checking.    2nd. check two list are same TYPE, WIDTH, INC_VALUE...
    */
    if (from_list->nCol IS_NOT_EQUAL_TO to_list->nCol)
        {
        return NL_TWO_LISTS_NOT_SAME_DEF;
        }
    for (i = 0, poldinfo = from_list->info_ptr, pnewinfo = to_list->info_ptr;
         i < from_list->nCol; i++, poldinfo++, pnewinfo++)
        {
        if (poldinfo->data_size IS_NOT_EQUAL_TO pnewinfo->data_size OR poldinfo->
            data_type IS_NOT_EQUAL_TO pnewinfo->data_type OR poldinfo->
            inc_value IS_NOT_EQUAL_TO pnewinfo->inc_value)
            {
            return NL_TWO_LISTS_NOT_SAME_DEF;
            }
        }
    if (from_Table EQUALS 0)
        {
        from_Table = from_list->current_table;
        }
    original_point = from_list->current_row;
    original_Table = from_list->current_table;
    
    poldtab = nlz_set_table (from_list, from_Table);
    if (poldtab EQUALS NULL_TABLE)
        {
        nl_set_current_row (from_list, original_point, original_Table);
        return NL_INVALID_TABLE;
        }
    /*
       * if the table has not been loaded, then loads first.
    */
    if (poldtab->data_loaded EQUALS NL_DATA_NOT_LOADED AND poldtab->nrows)
        {
        status = nlz_load_data (from_list);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            nl_set_current_row (from_list, original_point, original_Table);
            return status;
            }
        }
    /*
       * copy the table header and column data headers
    */
    size = sizeof(TABLE_HEADER) + sizeof(DATA_HEADER) * (from_list->nCol - 1);
    status = nl_start_table (to_list, &table_num);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    pnewtab = to_list->current_table_ptr;
    hoblockmove ((VOIDPTR)poldtab, (VOIDPTR)pnewtab, size);
    *to_Table = to_list->total_tables;
    /*
       * copy the user structure, if needed
    */
    if (poldtab->user_struct IS_NOT_EQUAL_TO (VOIDPTR)NULL)
        {
        status = nlz_copy_structure (poldtab->user_struct, poldtab->user_struct_len,
                                     &pnewtab->user_struct, &pnewtab->user_struct_len);
        if (status != SUCCESS)
            {
            return status;
            }
        }
    /*
       * copy the table name, if needed
    */
    if (poldtab->name IS_NOT_EQUAL_TO NULL)
        {
        pnewtab->name = STRING_ALLOCATE (poldtab->name);
        strcpy ((char *)pnewtab->name, (char *)poldtab->name);
        }
    if (poldtab->nrows > 0)
        {
        pnewtab->nallocated = pnewtab->nrows;
        for (j = 0, polddata = poldtab->data_ptr, pnewdata = pnewtab->data_ptr,
             poldinfo = from_list->info_ptr; j < from_list->nCol;
             j++, polddata++, pnewdata++, poldinfo++)
            {
            /*
               * move the data for each non-incrementing column
            */
            if (polddata->inc_value)
                {
                continue;
                }
            size = poldtab->nrows * poldinfo->data_size;
            pnewdata->data = (BYTE *) tc_alloc (size);
            if (pnewdata->data == (BYTE *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)polddata->data, (VOIDPTR)pnewdata->data, size);
            }
        }
    /*
       *   reset the current row and current table to the first row of the copied
       *   table in the target nlist, and back to the original in the source nlist
    */
    to_list->current_table = to_list->total_tables;
    to_list->current_row = 1;
    to_list->total_rows += poldtab->nrows;
    to_list->updated = TRUE;
    nl_set_current_row (from_list, original_point, original_Table);
    
    return SUCCESS;
    }
/* END:     */
