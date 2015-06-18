/* DEC/CMS REPLACEMENT HISTORY, Element NL_COPY_NL.C*/
/* *6     4-JAN-1991 09:41:22 GILLESPIE "(SPR 1) Force reset of current row on exit"*/
/* *5    25-OCT-1990 12:21:48 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:57:11 VINCE "(SPR 5644) Header Standardization"*/
/* *3     1-JUN-1990 12:57:15 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *2    12-MAR-1990 19:06:49 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:58:53 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_COPY_NL.C*/
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
#include "esi_ts.h"
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    Create a copy of an n-List.

Prototype:
    publicdef INT nl_copy_nlist (NLIST_HEADER from_list, NLIST_HEADER *to_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    from_list       -(NLIST_HEADER)
    to_list         -(NLIST_HEADER *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_copy_nlist (NLIST_HEADER from_list, NLIST_HEADER *to_list)
#else
publicdef INT nl_copy_nlist (from_list, to_list)
NLIST_HEADER from_list, *to_list;
#endif
    {
    UINT i, j;
    UINT nCol;
    size_t size;
    INT status;
    UINT oldCurrentRow, oldCurrentTable;
    NLIST_HEADER nlist;
    TABLE_HEADER *poldtab;
    TABLE_HEADER *pnewtab;
    COLUMN_HEADER *poldinfo;
    DATA_HEADER *polddata;
    DATA_HEADER *pnewdata;
    NL_METHOD_HEADER *pmethod;
    
    INIT_TEST (from_list);
    
    oldCurrentRow = from_list->current_row;
    oldCurrentTable = from_list->current_table;
    
    nCol = from_list->nCol;
    size = sizeof(NLIST_STRUCT) + sizeof(COLUMN_HEADER) * (from_list->nCol - 1);
    nlist = *to_list = (NLIST_HEADER) tc_alloc (size);
    if (nlist == (NLIST_HEADER)0)
        {
        return NL_OVERFLOW;
        }
    hoblockmove ((VOIDPTR)from_list, (VOIDPTR)nlist, size);
    
    /*
     * copy a user structure, if needed
     */
    if (from_list->user_struct IS_NOT_EQUAL_TO (VOIDPTR)NULL)
        {
        status = nlz_copy_structure (from_list->user_struct,
                                     from_list->user_struct_len, &nlist->user_struct,
                                     &nlist->user_struct_len);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*
     * copy the character strings, if needed
     */
    if (IS_STRING(from_list->name))
        {
        nlist->name = STRING_ALLOCATE (from_list->name);
        strcpy ((char *)nlist->name, (char *)from_list->name);
        }
    if (IS_STRING(from_list->next_nlist))
        {
        nlist->next_nlist = STRING_ALLOCATE (from_list->next_nlist);
        strcpy ((char *)nlist->next_nlist, (char *)from_list->next_nlist);
        }
    if (IS_STRING(from_list->prev_nlist))
        {
        nlist->prev_nlist = STRING_ALLOCATE (from_list->prev_nlist);
        strcpy ((char *)nlist->prev_nlist, (char *)from_list->prev_nlist);
        }
    /* Copy the n-List methods */
    nlist->method_list = (NL_METHOD_HEADER *)0;
    for (pmethod = from_list->method_list; pmethod != (NL_METHOD_HEADER *)0;
         pmethod = pmethod->next)
        {
        nl_set_nlist_method (nlist, pmethod->method_id, pmethod->function_name,
                             pmethod->function);
        }
    hoblockmove ((VOIDPTR)from_list->creation_date, (VOIDPTR)nlist->creation_date,
                 sizeof(DATE));
    /* The following effectively decouples the copy from the original file
    If we copied the file, then two n-Lists could potentially try and update the same file
    which would lead to general unhappiness and lack of tranquility */

    if (IS_STRING(from_list->data_filename))
        {
        nlist->data_filename = NULL;
        }

    /*
     * now copy the tables
     */
    
    if (from_list->nallocated_tables > 0)
        {
        nlist->table_elements = (TABLE_ELEMENT *) tc_zalloc (sizeof(TABLE_ELEMENT) *
                                                             from_list->
                                                             nallocated_tables);
        if (nlist->table_elements == (TABLE_ELEMENT *)0)
            {
            return NL_OVERFLOW;
            }
        }
    for (i = 0, poldtab = nlz_set_table (from_list, i + 1);
         i < from_list->total_tables AND poldtab != NULL_TABLE;
         i++, poldtab = nlz_set_table (from_list, i + 1))
        {
        
        /*
         * if the table has not been loaded, then loads first.
         */
        if (poldtab->data_loaded EQUALS NL_DATA_NOT_LOADED AND poldtab->nrows)
            {
            status = nlz_load_data (from_list);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                nl_set_current_row (from_list, oldCurrentRow, oldCurrentTable);
                return status;
                }
            }
        /*
         * copy the table header and column data headers
         */
        size = sizeof(TABLE_HEADER) + sizeof(DATA_HEADER) * (nCol - 1);
        pnewtab = (TABLE_HEADER *) tc_alloc (size);
        if (pnewtab == (TABLE_HEADER *)0)
            {
            return NL_OVERFLOW;
            }
        hoblockmove ((VOIDPTR)poldtab, (VOIDPTR)pnewtab, size);
        /*
         * relink the new tables
         */
        nlist->table_elements[i].ptable = pnewtab;
        
        /*
         * copy the user structure, if needed
         */
        if (poldtab->user_struct IS_NOT_EQUAL_TO (VOIDPTR)NULL)
            {
            status = nlz_copy_structure (poldtab->user_struct,
                                         poldtab->user_struct_len,
                                         &pnewtab->user_struct,
                                         &pnewtab->user_struct_len);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            }
        /*
         * copy the table name, if needed
         */
        if (IS_STRING(poldtab->name))
            {
            pnewtab->name = STRING_ALLOCATE (poldtab->name);
            strcpy ((char *)pnewtab->name, (char *)poldtab->name);
            }
        if (poldtab->nrows EQUALS 0)
            {
            continue;
            }
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
            pnewtab->nallocated = pnewtab->nrows;
            pnewdata->data = (BYTE *) tc_alloc (size);
            if (pnewdata->data == (BYTE *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)polddata->data, (VOIDPTR)pnewdata->data, size);
            pnewtab->data_loaded = NL_DATA_LOADED;
            /*
             * Decouple this n-List table from any data file
             */
            pnewtab->record_offset = 0;
            pnewtab->totalBytesInTable = 0;
            }
        }
    /*
     * reset the current_Table_pointer in the new n-List to match from n-List
     */
    nl_set_current_row (from_list, oldCurrentRow, oldCurrentTable);

    nlist->current_table_ptr = NULL_TABLE;
    nl_set_current_row (nlist, oldCurrentRow, oldCurrentTable);
    
    return SUCCESS;
    }
/* END:     */
