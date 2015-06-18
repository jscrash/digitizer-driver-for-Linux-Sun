/* DEC/CMS REPLACEMENT HISTORY, Element NL_SORT_NL.C*/
/* *4    25-OCT-1990 12:27:13 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 15:06:11 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:09:51 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:01:27 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SORT_NL.C*/
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Same function as NL_SORT_STROKE, except this routines not only sorts
    the rows in each table, but also sorts the first rows of each
    table, then re-arrage the order of tables.

Prototype:
    publicdef INT nl_sort_nlist (NLIST_HEADER inList, UINT nCol, UINT Col_List[], 
        INT sortFlags[], NLIST_HEADER *outList);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    inList          -(NLIST_HEADER) The original N-list.  
    nCol            -(UINT) The number of column of being inquired
                        to sort.     
    Col_List        -(UINT []) The column index of being inquired.
    sortFlags       -(INT []) The sort method flag, NL_ASCENDING,..
    outList         -(NLIST_HEADER *) The result N-list of sorting.  

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_sort_nlist (NLIST_HEADER inList, UINT nCol, UINT Col_List[],
                             INT sortFlags[], NLIST_HEADER *outList)
#else
publicdef INT nl_sort_nlist (inList, nCol, Col_List, sortFlags, outList)
NLIST_HEADER inList;
UINT nCol;
UINT Col_List[];
INT sortFlags[];
NLIST_HEADER *outList;
#endif
    {
    NLIST_HEADER nlist;             /*   workspace for NLIST_HEADER                */
    /*------------------------------------*/
    /* workspace for the info. of current */
    /* column.                            */
    /*------------------------------------*/
    COLUMN_HEADER *pinfo;
    
    INT status = SUCCESS;           /*   return status.                            */
    /*----------------------------------------*/
    /* workspace for retrieving the first row */
    /* of each table.                         */
    /*----------------------------------------*/
    DOUBLE **floatArray;
    
    DOUBLE **returnArray;           /*   return array for one row.               */
    DOUBLE *sortArray;              /*   the sorted array.                         */
    DOUBLE **nullValue;             /*   NULL value for current column.         */
    /*---------------------------------*/
    /* array of data size of specified */
    /* columns.                        */
    /*---------------------------------*/
    size_t *dataSize;
    
    /*---------------------------------*/
    /* array of data type of specified */
    /* columns.                        */
    /*---------------------------------*/
    INT *dataType;
    
    UINT returnPts, table_num;
    UINT *order;                    /*   the order of tables.                     */
    UINT i, j;                      /*   work indexes.                             */
    UINT totalDim;
    TCP c_name;
    UINT *dimlist;
    /*--------------------------------*/
    /* workspace for initializing the */
    /* n-Lists.                       */
    /*--------------------------------*/
    INT *unit, *incValue;
    
    /* ***************************************************************************** */
    
    INIT_TEST (inList);
    EMPTY_TEST (inList);
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (inList, Col_List[i]);
        }
    status = nl_sort_table (inList, nCol, Col_List, sortFlags, &nlist);
    
    if (status < 0)
        {
        return status;
        }
    /*   allocates the workspace and initializes
         the output N-list.            */
    totalDim = nlist->nCol;
    incValue = (INT *) TALLOC (sizeof(INT) * totalDim);
    unit = (INT *) TALLOC (sizeof(INT) * totalDim);
    dataType = (INT *) TALLOC (sizeof(INT) * totalDim);
    dimlist = (UINT *) TALLOC (sizeof(UINT) * totalDim);
    dataSize = (size_t *) TALLOC (sizeof(size_t) * totalDim);
    nullValue = (DOUBLE * *) TALLOC (sizeof(DOUBLE *)*totalDim);
    c_name = (TCP) TALLOC (sizeof(CHAR *)*totalDim);
    
    for (i = 0; i < totalDim; i++)
        {
        dimlist[i] = i + 1;
        pinfo = nlist->info_ptr + i;
        dataSize[i] = pinfo->data_size;
        dataType[i] = pinfo->data_type;
        unit[i] = pinfo->unit;
        incValue[i] = (pinfo->inc_value) ? INCREMENT_LIST : VALUE_LIST;
        c_name[i] = (CHAR *) TALLOC (32);
        }
    nl_init_nlist (outList, totalDim, unit, dataSize, dataType, incValue,
                   nlist->blocksize);
    nl_inq_nlist_info (inList, NL_NLIST_COLUMN_NAME, totalDim, dimlist,
                       (VOIDPTR *)c_name);
    status = nl_set_nlist_info (*outList, NL_NLIST_COLUMN_NAME, totalDim, dimlist,
                                (VOIDPTR *)c_name);
    if (status != SUCCESS)
        {
        return status;
        }
    /*   free the workspace.      */
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)dataSize);
    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)unit);
    tc_free ((VOIDPTR)incValue);
#endif
    
    /*   retrieve the first rows of each table,
         then re-arrage the order of table.   */
    
    dataType = (INT *) TALLOC (sizeof(INT) * nCol);
    dataSize = (size_t *) TALLOC (sizeof(size_t) * nCol);
    unit = (INT *) TALLOC (sizeof(INT) * nCol);
    floatArray = (DOUBLE * *) TALLOC (sizeof(DOUBLE *)*nCol);
    returnArray = (DOUBLE * *) TALLOC (sizeof(DOUBLE *)*nCol);
    sortArray = (DOUBLE *) TALLOC (sizeof(DOUBLE) * nlist->total_tables);
    
    for (i = 0; i < nCol; i++)
        {
        unit[i] = 0;
        dataType[i] = FLOAT_TYPE;
        dataSize[i] = sizeof(DOUBLE);
        floatArray[i] = (DOUBLE *) TALLOC (sizeof(DOUBLE) * nlist->total_tables);
        nullValue[i] = (DOUBLE *) TALLOC (sizeof(DOUBLE));
        }
    order = (UINT *) TALLOC (sizeof(UINT) * nlist->total_tables);
    
    /*   inqire NULL values of all columns by */
    /*   specified types to be float.            */
    
    status = nl_inq_nlist_info_convert (nlist, NL_NLIST_NULL_VALUE, nCol, Col_List,
                                        (VOIDPTR *)nullValue, unit, dataSize,
                                        dataType);
    if (status IS_NOT_EQUAL_TO SUCCESS)
        return status;
    /*   retrieve the first row of all tables */
    /*   for each specified column.           */
    
    for (i = 0; i < nlist->total_tables; i++)
        {
        order[i] = i;
        status = nl_set_current_row (nlist, (UINT)1, i + 1);
        
        /*    if the table is empty, set its first
              row equals NULL                  */
        if (status < 0)
            {
            for (j = 0; j < nCol; j++)
                {
                
                *(floatArray[j] + i) = *(DOUBLE *)nullValue[j];
                
                }
            continue;
            }
        for (j = 0; j < nCol; j++)
            {
            returnArray[j] = floatArray[j] + i;
            }
        nl_get_rows_convert (nlist, (UINT)1, nCol, Col_List, (VOIDPTR *)returnArray,
                             unit, dataSize, dataType, &returnPts);
        }
    /* according the sorting flags, sorts each
         column, starts with the last dimen-
         mension first.                */
    
    for (i = nCol; i > 0; i--)
        {
        pinfo = nlist->info_ptr + Col_List[i - 1] - 1;
        
        /*   re-arrage the order of table first,
             according to the previous sorting.    */
        for (j = 0; j < nlist->total_tables; j++)
            {
            sortArray[j] = floatArray[i - 1][order[j]];
            }
        nlz_sort_dim (nlist->total_tables, order, sortArray, sortFlags[i - 1], pinfo);
        
        }
    /*   after sorting, re-arrage the order of
         table list.                         */
    for (i = 0; i < nlist->total_tables; i++)
        {
        nl_copy_table (nlist, order[i] + 1, *outList, &table_num);
        }
    /* free the allocated memory.      */
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)sortArray);
    tc_free ((VOIDPTR)unit);
    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)dataSize);
    tc_free ((VOIDPTR)order);
    for (i = 0; i < nCol; i++)
        {
        tc_free ((VOIDPTR)nullValue[i]);
        tc_free ((VOIDPTR)floatArray[i]);
        }
    tc_free ((VOIDPTR)nullValue);
    tc_free ((VOIDPTR)floatArray);
    tc_free ((VOIDPTR)returnArray);
#endif
    return SUCCESS;
    }
/* END:     */
