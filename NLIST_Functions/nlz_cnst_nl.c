/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CNST_NL.C*/
/* *5    20-AUG-1990 17:08:24 KEE "(SPR -1) fixing compile error at sun"*/
/* *4    25-JUL-1990 14:51:45 VINCE "(SPR 5644) Header Standardization"*/
/* *3    23-MAR-1990 22:10:52 GILLESPIE "(SPR 1) More stats changes"*/
/* *2    12-MAR-1990 19:03:14 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:55:06 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CNST_NL.C*/
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
#include "esi_nl_io.h"

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    Removes the information from headerArray and reconstructs the nlist.
    In this routine, we only construct the headers part, the real data will not
    read in until be accessed.

Prototype:
    publicdef INT nlz_construct_nlist (NLIST_HEADER *nlist_ptr, VOIDPTR headerArray, 
        size_t headerArraySize);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist_ptr       -(NLIST_HEADER *)
    headerArray     -(VOIDPTR)
    headerArraySize -(size_t)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_IO_ERROR4
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_construct_nlist (NLIST_HEADER *nlist_ptr, VOIDPTR headerArray,
                                   size_t headerArraySize) 
#else
publicdef INT nlz_construct_nlist (nlist_ptr, headerArray, headerArraySize)
NLIST_HEADER *nlist_ptr;
VOIDPTR headerArray;
size_t headerArraySize;
#endif
    {
    NLIST_HEADER nlist;
    register UINT i;
    INT status;
    BOOL done = FALSE, getNdim = FALSE;
    INT *dataType = (INT *)0;
    size_t *dataWidth = (size_t *)0;
    INT *dataUnits = (INT *)0, *incValue = (INT *)0;
    UINT blockSize, nCol, intItem;
    UINT newTable;
    INT basicItems = 6, dataCode;
    size_t dataSize, excutedBytes = (size_t)0;
    UINT *intArray = (UINT *)0;
    size_t dataCodeSize = sizeof(INT) * 2;
    UINT totalTables = 0;
    size_t func_name_len = 0;
    NL_METHOD_HEADER *pMethod;
    UINT *intPtr;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    DOUBLE *doubleArray = (DOUBLE *)0;
    TABLE_HEADER *pTable;
    BYTE *ptr;
    
    /*
       * fetch those basic items for initializing n-LIST.
    */
    ptr = (BYTE *)headerArray;
    do
        {
        DECODE_DATA (ptr, &dataCode, &dataSize);
        if (dataCode IS_NOT_EQUAL_TO 101 AND getNdim EQUALS FALSE)
            {
            return NL_IO_ERROR2;
            }
        switch (dataCode)
            {
        case 101:                   /* dataCode 101 must be the first item in the array. */
            if (excutedBytes IS_NOT_EQUAL_TO 0)
                {
                return NL_IO_ERROR1;
                }
            getNdim = TRUE;
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & nCol, (size_t)sizeof(INT));
            basicItems--;
            /* allocate the memory space for dataUnits, dataWidth... */
            dataUnits = (INT *)TALLOC ((nCol + 1) * sizeof(INT));
            dataWidth = (size_t *)TALLOC ((nCol + 1) * sizeof(size_t));
            incValue = (INT *)TALLOC ((nCol + 1) * sizeof(INT));
            dataType = (INT *)TALLOC ((nCol + 1) * sizeof(INT));
            intArray = (UINT *)TALLOC ((nCol + 1) * sizeof(UINT));
            doubleArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
            break;
            
        case 109:                   /* move the units array.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)dataUnits, (size_t)dataSize);
            basicItems--;
            break;
            
        case 110:                   /*   move the Data Type Array.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)dataType, (size_t)dataSize);
            basicItems--;
            break;
            
        case 111:                   /* move the Data Width Array.    */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)dataWidth, (size_t)dataSize);
            basicItems--;
            break;
            
        case 112:                   /* move the default blockSize.    */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & blockSize, (size_t)sizeof(INT));
            basicItems--;
            break;
            
        case 120:                   /* move the Inc_value Array.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)incValue, (size_t)dataSize);
            basicItems--;
            break;
            
        default:
            break;
            
            }
        ptr += dataSize;
        excutedBytes += dataCodeSize + dataSize;
      } 
    while ((basicItems > 0) AND (excutedBytes < headerArraySize AND dataSize > 0));
    
    if (basicItems IS_NOT_EQUAL_TO 0)
        {

#if NEED_TO_FREE EQUALS TRUE
        /*
           * free the allocated memory.
        */
        if (dataType != (INT *)0)
            tc_free ((VOIDPTR)dataType);
        if (dataUnits != (INT *)0)
            tc_free ((VOIDPTR)dataUnits);
        if (dataWidth != (size_t *)0)
            tc_free ((VOIDPTR)dataWidth);
        if (incValue != (INT *)0)
            tc_free ((VOIDPTR)incValue);
        if (intArray != (UINT *)0)
            tc_free ((VOIDPTR)intArray);
        if (doubleArray != (DOUBLE *)0)
            tc_free ((VOIDPTR)doubleArray);
#endif
        return NL_IO_ERROR3;
        }
    /*
       * Initialize a N_LIST.
    */
    status = nl_init_nlist (nlist_ptr, nCol, dataUnits, dataWidth, dataType, 
			    incValue, blockSize);
    
    if (status != SUCCESS)
      {
        return status;
      }
    nlist = *nlist_ptr;
    
    /*
       * free the allocated memory.
    */
#if NEED_TO_FREE EQUALS TRUE
    if (dataType != (INT *)0)
        tc_free ((VOIDPTR)dataType);
    if (dataUnits != (INT *)0)
        tc_free ((VOIDPTR)dataUnits);
    if (dataWidth != (size_t *)0)
        tc_free ((VOIDPTR)dataWidth);
    if (incValue != (INT *)0)
        tc_free ((VOIDPTR)incValue);
#endif
    /*
       * read others data and set to the Nlist header and Table header.
    */
    do
        {
        DECODE_DATA (ptr, &dataCode, &dataSize);
        switch (dataCode)
            {
            /*
               * set to the Nlist Header.
            */
            
        case 100:                   /* copy the version number. */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->version.Revision,
                         (size_t)sizeof nlist->version.Revision);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 102:                   /* copy the name.  */
            nlist->name = (CHAR *)tc_zalloc ((size_t)(dataSize + 1));
            if (nlist->name == (CHAR *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->name, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 103:                   /* copy the user structure.   */
            nlist->user_struct_len = dataSize;
            nlist->user_struct = (VOIDPTR)tc_alloc ((size_t)dataSize);
            if (nlist->user_struct == (VOIDPTR)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->user_struct, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 104:                   /* copy the name of next n-List.   */
            nlist->next_nlist = (CHAR *)tc_zalloc (dataSize + 1);
            if (nlist->next_nlist == (CHAR *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->next_nlist, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 105:                   /* set the total number of rows.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & nlist->total_rows,
                         (size_t)sizeof(INT));
            break;
            
        case 107:                   /* store the total rows in a temporary space. */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & totalTables, (size_t)sizeof(INT));
            break;
            
        case 108:                   /* set the value of NULL.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->null_value = doubleArray[i];
                }
            break;
            
        case 113:                   /* reset the minmum value of rows.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->min = doubleArray[i];
                }
            break;
            
        case 114:                   /* reset the maxmum value of rows.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->max = doubleArray[i];
                }
            break;
            
        case 115:                   /* reset the sum of rows.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->sum_x = doubleArray[i];
                }
            break;
            
        case 116:                   /* reset the squared sum of rows.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->sum_x_squared = doubleArray[i];
                }
            break;
            
        case 117:                   /* reset the find tolerance.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->tolerance = doubleArray[i];
                }
            break;
        case 119:
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->creation_date,
                         (size_t)sizeof(DATE));
            break;
            
            /*
               * set to the Current Table Header.
            */
        case 121:                   /* copy the name of previous n-List.   */
            nlist->prev_nlist = (CHAR *)tc_zalloc (dataSize + 1);
            if (nlist->prev_nlist == (CHAR *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)nlist->prev_nlist, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 122:                   /* move the defer statistic flag.    */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & nlist->defer_stats,
                         (size_t)sizeof(BOOL));
            break;
            
        case 123:                   /* set the CONVERT_FORMAT.   */
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                hoblockmove ((VOIDPTR)(ptr + i * 10), (VOIDPTR)pinfo->convert_format,
                             (size_t)10);
                }
            break;
        case 124:                   /* set the DIM NAME.   */
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                hoblockmove ((VOIDPTR)(ptr + i * 32), (VOIDPTR)pinfo->column_name,
                             (size_t)32);
                }
            break;
            
        case 125:                   /* set the data clase of nlist.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & nlist->nlist_data_class,
                         (size_t)sizeof(INT));
            break;
            
        case 126:                   /*  set the table headers offset. */
            nlist->total_tables = totalTables;
            nlist->nallocated_tables = (totalTables > 10) ? totalTables : 10;
            nlist->table_elements = (TABLE_ELEMENT *
                                     )tc_zalloc (sizeof(TABLE_ELEMENT) *
                                                 nlist->nallocated_tables);
            if (nlist->table_elements == (TABLE_ELEMENT *)0)
                {
                return NL_OVERFLOW;
                }
            intPtr = (UINT *)ptr;
            for (i = 0; i < nlist->total_tables; i++, intPtr++)
                {
                hoblockmove ((VOIDPTR)intPtr,
                             (VOIDPTR) & nlist->table_elements[i].header_offset,
                             (size_t)sizeof(UINT));
                }
            break;
            
        case 127:                   /* size of method function name.  If the function name
                                       has changed, then we need to worry a bit about
                                       possible function name truncation.  This would be the
                                       case if other n-List versions decrease the size
                                       of function names */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & func_name_len,
                         (size_t)sizeof(UINT));
            func_name_len = MIN (func_name_len, sizeof(NLM_FUNCTION_NAME));
            break;
        case 128:                   /* method id and function name */
            /* Record type 127 should always precede */
            /* Adds method to head of method list, sets the actual
            function ptr to NULL.  An application is responsible
            for registering its functions with the n-List system before
            this method can be invoked. */
            if (func_name_len == 0)
                {
                break;
                }
            pMethod = ALLOCATE (NL_METHOD_HEADER);
            pMethod->next = nlist->method_list;
            pMethod->function = (INT_FUNCTION_PTR)0;
            nlist->method_list = pMethod;
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & pMethod->method_id,
                         (size_t)sizeof(NLIST_METHOD));
            hoblockmove ((VOIDPTR)(ptr + sizeof(NLIST_METHOD)),
                         (VOIDPTR)pMethod->function_name, (size_t)func_name_len);
            /* Make sure the string is null terminated */
            pMethod->function_name[func_name_len - 1] = 0;
            break;
        case 129:                   /* reset the number of non_null rows.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)intArray, (size_t)dataSize);
            for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
                {
                pinfo->n_valid_rows = intArray[i];
                }
            break;
            
        case 200:                   /* start a new table.   */
            nl_start_table (nlist, &newTable);
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & intItem, (size_t)sizeof(INT));
            if (newTable IS_NOT_EQUAL_TO intItem)
                {
                return NL_IO_ERROR4;
                }
            pTable = nlist->current_table_ptr;
            pTable->data_loaded = NL_DATA_NOT_LOADED;
            break;
            
        case 201:                   /* copy the user structure for the current table.  */
            pTable->user_struct_len = dataSize;
            pTable->user_struct = (VOIDPTR)tc_alloc ((size_t)dataSize);
            if (pTable->user_struct == (VOIDPTR)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)pTable->user_struct, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 202:                   /* reset the total number of rows for current table. */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & pTable->nrows, (size_t)sizeof(INT));
            break;
            
        case 203:                   /* copy the table name.  */
            pTable->name = (CHAR *)tc_zalloc (dataSize + 1);
            if (pTable->name == (CHAR *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)pTable->name, (size_t)dataSize);
            dataSize += ROUND_UP (dataSize);
            break;
            
        case 204:                   /* reset the number of non_null rows.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)intArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->n_valid_rows = intArray[i];
                }
            break;
            
        case 206:                   /* reset the minmum value of rows in the current table.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->min = doubleArray[i];
                }
            break;
            
        case 207:                   /* reset the maxmum value of rows in the current table.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->max = doubleArray[i];
                }
            break;
            
        case 208:                   /* reset the sum of rows in the current table. */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->sum_x = doubleArray[i];
                }
            break;
            
        case 209:                   /* reset the squared sum of rows in the current table.*/
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->sum_x_squared = doubleArray[i];
                }
            break;
            
        case 210:                   /* reset the start value of current table.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->start_value = doubleArray[i];
                }
            break;
            
        case 211:                   /* reset the increment value for the current table.  */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)doubleArray, (size_t)dataSize);
            for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
                {
                pdata->increment = doubleArray[i];
                }
            break;
            
        case 212:                   /* restore the total bytes of table data.      */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & pTable->totalBytesInTable,
                         (size_t)sizeof(INT));
            break;
            
        case 214:                   /* restore the record offset.   */
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & pTable->record_offset,
                         (size_t)sizeof(INT));
            break;
            
        case 999:
            done = TRUE;
#if NEED_TO_FREE EQUALS TRUE
            if (intArray != (UINT *)0)
                tc_free ((VOIDPTR)intArray);
            if (doubleArray != (DOUBLE *)0)
                tc_free ((VOIDPTR)doubleArray);
#endif
            break;
            
        default:
            break;
            }
        ptr += dataSize;
        excutedBytes += dataCodeSize + dataSize;
        } while (excutedBytes < headerArraySize AND NOT done);
    return SUCCESS;
    }
/* END:     */
