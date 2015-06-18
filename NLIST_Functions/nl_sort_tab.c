/* DEC/CMS REPLACEMENT HISTORY, Element NL_SORT_TAB.C */
/* *6    25-OCT-1990 12:27:20 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *5    25-JUL-1990 15:06:19 VINCE "(SPR 5644) Header Standardization" */
/* *4    19-MAR-1990 17:27:55 GILLESPIE "(SPR 1) Remove n-List deferral" */
/* *3    12-MAR-1990 19:09:59 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:51:07 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:52 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SORT_TAB.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SRT_STK.C*/
/* *2    30-JUL-1989 00:00:50 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *1    19-JUN-1989 13:20:51 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SRT_STK.C*/
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

/* Function Description -----------------------------------------------------
Description:
    According to the specified column, to do sorting, if there are more
    one column, uses the last column first, all sorting only happen in
    a table.

Prototype:
    publicdef INT nl_sort_table (NLIST_HEADER nlist, UINT nCol, UINT Col_List[], 
        INT sortFlags[], NLIST_HEADER *outList);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    outList         -(NLIST_HEADER *)
    nCol            -(UINT)
    Col_List        -(UINT [])
    sortFlags       -(INT [])

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
publicdef INT nl_sort_table (NLIST_HEADER nlist, UINT nCol, UINT Col_List[],
                             INT sortFlags[], NLIST_HEADER *outList)
#else
publicdef INT nl_sort_table (nlist, nCol, Col_List, sortFlags, outList)
NLIST_HEADER nlist, *outList;
UINT nCol, Col_List[];
INT sortFlags[];
#endif
    {
    NLIST_HEADER tempList;
    UINT i, j;
    INT status;
    UINT *sortArray;
    UINT table_num, totalDim;
    UINT *totalDimList, returnP;
    INT inc_converted = FALSE;
    COLUMN_HEADER *pinfo;
    TABLE_HEADER *pstkOld, *pstkOut;
    VOIDPTR *Var_List;
    
    INIT_TEST (nlist);
    EMPTY_TEST (nlist);
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        }
    totalDim = nlist->nCol;
    /*
       * make a copy of N-list.
    */
    if ((status = nl_copy_nlist (nlist, outList)) ||
        (status = nl_copy_nlist (nlist, &tempList)) ||
        (status = nlz_convert_inc_2_real (tempList)))
        {
        return status;
        }
    /*
       * allocate a temporary memory space
    */
    Var_List = (VOIDPTR *) TALLOC (sizeof(VOIDPTR) * totalDim);
    totalDimList = (UINT *) TALLOC (sizeof(UINT) * totalDim);
    for (i = 0, pinfo = nlist->info_ptr; i < totalDim; i++)
        {
        totalDimList[i] = i + 1;
        Var_List[i] = (VOIDPTR) TALLOC (pinfo->data_size);
        }
    /*
       * sort each table.
    */
    for (table_num = 1; table_num <= nlist->total_tables; table_num++)
        {
        pstkOld = nlz_set_table (nlist, table_num);
        if (pstkOld EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        if (pstkOld->nrows < 2)
            continue;
        pstkOut = nlz_set_table (tempList, table_num);
        if (pstkOut EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        /*
           * start sorting from the last column.
        */
        sortArray = (UINT *) tc_alloc (sizeof(UINT) * (pstkOld->nrows + 1));
        if (sortArray == (UINT *)0)
            {
            return NL_OVERFLOW;
            }
        status = nlz_sort_rows (nlist, pstkOld, nCol, Col_List, sortFlags, sortArray);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            return status;
        /*
           * after sorting, replace all sorted rows.
        */
        for (j = 0; j < pstkOld->nrows; j++)
            {
            /* no replace needed if pt hasn't moved */
            if (j EQUALS sortArray[j])
                {
                continue;
                }
            inc_converted = TRUE;
            status = nl_set_current_row (nlist, sortArray[j] + 1, table_num);
            status = nl_get_rows (nlist, (UINT)1, totalDim, totalDimList, Var_List,
                                  &returnP);
            status = nl_set_current_row (tempList, j + 1, table_num);
            status = nl_replace_rows (tempList, (UINT)1, totalDim, totalDimList,
                                      Var_List);
            }
        tc_free ((VOIDPTR)sortArray);
        }
    /*
       * free the allocated memory.
    */
    if (inc_converted)
        {
        nl_free_nlist (*outList);
        *outList = tempList;
        }
    else
        {
        nl_free_nlist (tempList);
        }
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)totalDimList);
    for (i = 0; i < totalDim; i++)
        {
        tc_free ((VOIDPTR)Var_List[i]);
        }
    tc_free ((VOIDPTR)Var_List);
#endif
    return SUCCESS;
    }
/* END:     */
