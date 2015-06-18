/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SORT_ROW.C*/
/* *4    25-OCT-1990 12:20:07 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 17:35:23 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes"*/
/* *2    25-JUL-1990 14:55:08 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:57:11 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SORT_ROW.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SRT_PNTS.C*/
/* *1    19-JUN-1989 13:17:26 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SRT_PNTS.C*/
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
#include "esi_ho.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    This is a help routine to sort rows in the specified table, according to
    the sortFlags of given columns.

Prototype:
    publicdef INT nlz_sort_rows (NLIST_HEADER nlist, TABLE_HEADER *ptable,
        UINT nCol, UINT Col_List[], INT sortFlags[], UINT orderArray[]);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    ptable          -(TABLE_HEADER *)
    nCol            -(UINT)
    Col_List        -(UINT [])
    orderArray      -(UINT [])
    sortFlags       -(INT [])

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_sort_rows (NLIST_HEADER nlist, TABLE_HEADER *ptable,
                             UINT nCol, UINT Col_List[], INT sortFlags[],
                             UINT orderArray[])
#else
publicdef INT nlz_sort_rows (nlist, ptable, nCol, Col_List, sortFlags, orderArray)
NLIST_HEADER nlist;
TABLE_HEADER *ptable;
UINT nCol, Col_List[], orderArray[];
INT sortFlags[];
#endif
    {
    UINT i, j;
    INT status = SUCCESS;
    UINT rtnPoints;
    size_t total_bytes = ptable->nrows * sizeof(DOUBLE);
    DOUBLE *sortFloat, *preSortFloat, *sortDim;
    DOUBLE *sortString, *preSortString;
    COLUMN_HEADER *pinfo;
    BOOL first_sorting;
    
    /*
       * start sorting from the last column.
    */
    for (i = 0; i < ptable->nrows; i++)
        {
        orderArray[i] = i;
        }
    for (i = nCol, first_sorting = TRUE; i > 0; i--)
        {
        pinfo = nlist->info_ptr + Col_List[i - 1] - 1;
        if (pinfo->data_type IS_NOT_EQUAL_TO CHAR_TYPE)
            {
            sortDim = nlz_get_double_array (nlist, Col_List[i - 1], &status);
            if (status < 0)
                {
                return status;
                }
            if (status EQUALS SUCCESS)
                {
                sortFloat = (DOUBLE *)TALLOC (total_bytes);
                hoblockmove ((VOIDPTR)sortDim, (VOIDPTR)sortFloat, total_bytes);
                }
            else
                {
                sortFloat = sortDim;
                }
            /*   if more than one sorting columns,    */
            /*   re-arrange the order first.             */
            if (!first_sorting)
                {
                preSortFloat = (DOUBLE *)TALLOC (total_bytes);
                hoblockmove ((VOIDPTR)sortFloat, (VOIDPTR)preSortFloat, total_bytes);
                for (j = 0; j < ptable->nrows; j++)
                    {
                    sortFloat[j] = preSortFloat[orderArray[j]];
                    }
#if NEED_TO_FREE EQUALS TRUE
                tc_free ((VOIDPTR)preSortFloat);
#endif
                }
            nlz_sort_dim (ptable->nrows, orderArray, sortFloat, sortFlags[i - 1],
                          pinfo);
            first_sorting = FALSE;
            if (sortFloat IS_NOT_EQUAL_TO sortDim)
                {
#if NEED_TO_FREE EQUALS TRUE
                tc_free ((VOIDPTR)sortFloat);
#endif
                }
            }
        else
            {
            sortString = (DOUBLE *)TALLOC (total_bytes);
            status = nl_get_rows (nlist, ptable->nrows, (UINT)1, Col_List + i - 1,
                                  (VOIDPTR *) &sortString, &rtnPoints);
            if (!first_sorting)
                {
                preSortString = (DOUBLE *)TALLOC (total_bytes);
                hoblockmove ((VOIDPTR)sortFloat, (VOIDPTR)preSortFloat, total_bytes);
                for (j = 0; j < ptable->nrows; j++)
                    {
                    hoblockmove ((VOIDPTR)&(preSortString[orderArray[j] * pinfo->data_size]),
                                 (VOIDPTR)&(sortString[j * pinfo->data_size]), pinfo->data_size);
                    }
#if NEED_TO_FREE
                tc_free ((VOIDPTR)preSortString);
#endif
                }
            nlz_sort_dim (ptable->nrows, orderArray, sortString, sortFlags[i - 1],
                          pinfo);
#if NEED_TO_FREE
            tc_free ((VOIDPTR)sortString);
#endif
            first_sorting = FALSE;
            }
        }
    return SUCCESS;
    }
/* END:     */
