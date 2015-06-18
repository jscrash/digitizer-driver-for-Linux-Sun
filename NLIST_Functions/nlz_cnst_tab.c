/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CNST_TAB.C*/
/* *4    25-OCT-1990 12:17:59 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:52:09 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:03:22 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:55:20 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_CNST_TAB.C*/
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
    To remove the infomation form headerArray and to reconstruct the table.
    In this routine, we only construct the headers part, the real data will not
    read in until be accessed.

Prototype:
    publicdef INT nlz_construct_Table (NLIST_HEADER nlist, TABLE_ELEMENT *table_element, 
        VOIDPTR headerArray, size_t headerArraySize);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_element   -(TABLE_ELEMENT *)
    headerArray     -(VOIDPTR)
    headerArraySize -(size_t)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_construct_Table (NLIST_HEADER nlist, TABLE_ELEMENT *table_element,
                                   VOIDPTR headerArray, size_t headerArraySize) 
#else
publicdef INT nlz_construct_Table (nlist, table_element, headerArray, headerArraySize)
NLIST_HEADER nlist;
TABLE_ELEMENT *table_element;
VOIDPTR headerArray;
size_t headerArraySize;
#endif
    {
    register UINT i;
    UINT nCol;
    BOOL done = FALSE;
    INT intItem;
    INT dataCode;
    size_t dataSize;
    register size_t excutedBytes = 0;
    size_t dataCodeSize = sizeof(size_t) * 2;
    DOUBLE *doubleArray = (DOUBLE *)0;
    INT *intArray = (INT *)0;
    register DATA_HEADER *pdata;
    TABLE_HEADER *pTable;
    register BYTE *ptr;
    
    ptr = (BYTE *)headerArray;
    nCol = nlist->nCol;
    
    /*
       * read others data and set to the Table header.
    */
    do
        {
        DECODE_DATA (ptr, &dataCode, &dataSize);
        switch (dataCode)
            {
            
            /*
               * set to the Current Table Header.
            */
        case 200:                   /* start a new table.   */
            intArray = (INT *)TALLOC ((nCol + 1) * sizeof(INT));
            doubleArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & intItem, (size_t)sizeof(INT));
            pTable = table_element->ptable;
            pTable->data_loaded = NL_DATA_NOT_LOADED;
            break;
            
        case 201:                   /* copy the user structure for the current table.  */
            pTable->user_struct_len = dataSize;
            pTable->user_struct = (VOIDPTR)tc_alloc ((size_t)(dataSize + 1));
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
                pdata->n_valid_rows = (UINT)intArray[i];
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
            if (intArray != (INT *)0)
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
