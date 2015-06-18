/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_DEL_NULL.C*/
/* *6    25-OCT-1990 12:18:05 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    20-AUG-1990 17:08:40 KEE "(SPR -1) fixing compile error at sun"*/
/* *4    25-JUL-1990 17:35:16 VINCE "(SPR 5644) Fixed compile errors introduced with the header changes"*/
/* *3    25-JUL-1990 14:52:15 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:03:29 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:55:24 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_DEL_NULL.C*/
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
    A help routine to delete the null values in the front of tables and the
    end of tables.
    
Prototype:
    publicdef INT nlz_delete_nulls (NLIST_HEADER nlist, UINT nCol,
                                    UINT Col_List[]);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    nCol            -(UINT)
    Col_List        -(UINT [])

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_delete_nulls (NLIST_HEADER nlist, UINT nCol, UINT Col_List[])
#else
publicdef INT nlz_delete_nulls (nlist, nCol, Col_List)
NLIST_HEADER nlist;
UINT nCol, Col_List[];
#endif
    {
    TABLE_HEADER *ptable;
    VOIDPTR *Var_List;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    DOUBLE value;
    UINT i, j, k;
    BOOL startDeletion = FALSE;
    UINT deletePoints;
    /*
       * allocates the Var_List space for return values.
    */
    
    Var_List = (VOIDPTR *)TALLOC(sizeof(VOIDPTR) * nCol);
    if (Var_List == (VOIDPTR *)0)
        {
        return NL_OVERFLOW;
        }
    for (i = 0; i < nCol; i++)
        {
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        Var_List[i] = (VOIDPTR)TALLOC(sizeof(BYTE) * pinfo->data_size);
        if (Var_List[i] == (VOIDPTR)0)
            {
            return NL_OVERFLOW;
            }
        }
    for (i = nlist->total_tables; i > 0; i--)
        {
        
        /*
           * started from the end.
        */
        ptable = nlz_set_table (nlist, i);
        if (ptable EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        nl_set_current_row (nlist, ptable->nrows, i);
        
        for (j = nlist->current_table_ptr->nrows, startDeletion = FALSE,
                 deletePoints = 0; j > 0 AND !startDeletion; j--)
            {
            for (k = 1; k < nCol AND!startDeletion; k++)
                {
                pinfo = nlist->info_ptr + Col_List[k] - 1;
                pdata = nlist->current_table_ptr->data_ptr + Col_List[k] - 1;
                nlz_get_value (pdata, pinfo, j, &value);
                if (value IS_NOT_EQUAL_TO pinfo->null_value)
                    startDeletion = TRUE;
                }
            if (!startDeletion)
                deletePoints++;
            }
        if (deletePoints > 0)
            {
            nl_set_current_row (nlist, ptable->nrows - deletePoints + 1, i);
            nl_delete_rows (nlist, deletePoints);
            }
        /*
           * started from begin.
        */
        nl_set_current_row (nlist, (UINT)1, i);
        
        for (j = 1, startDeletion = FALSE, deletePoints = 0;
             j < nlist->current_table_ptr->nrows AND !startDeletion; j++)
            {
            for (k = 1; k < nCol AND !startDeletion; k++)
                {
                pinfo = nlist->info_ptr + Col_List[k] - 1;
                pdata = nlist->current_table_ptr->data_ptr + Col_List[k] - 1;
                nlz_get_value (pdata, pinfo, j, &value);
                if (value IS_NOT_EQUAL_TO pinfo->null_value)
                    startDeletion = TRUE;
                }
            if (!startDeletion)
                deletePoints++;
            }
        if (deletePoints > 0)
            {
            nl_set_current_row (nlist, (UINT)1, i);
            nl_delete_rows (nlist, deletePoints);
            }
        }
#if NEED_TO_FREE
    for (i = 0; i < nCol; i++)
        {
        tc_free((VOIDPTR)Var_List[i]);
        }
    tc_free((VOIDPTR)Var_List);
#endif
    return SUCCESS;
    }
/* END:     */
