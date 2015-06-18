/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_NDX.C*/
/* *5    25-OCT-1990 12:18:32 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    20-AUG-1990 17:08:46 KEE "(SPR -1) fixing compile error at sun"*/
/* *3    25-JUL-1990 14:52:46 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:04:09 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:55:39 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_NDX.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_INDEX.C*/
/* *2    24-SEP-1989 22:27:05 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:16:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_INDEX.C*/
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

/* Function Description -----------------------------------------------------
Description:
    According to the specified index column and retrieved column,
    to retrieve the value, if the value is NULL value or in the in the table
    break, the function returns FAIL, otherwise, return TRUE.

Prototype:
    publicdef INT nlz_get_index_value (NLIST_HEADER nlist, DOUBLE indexValue, 
        UINT Col_List[2], VOIDPTR value, BOOL returnDouble);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    Col_List        -(UINT [])
    indexValue      -(DOUBLE)
    value           -(VOIDPTR)
    returnDouble    -(BOOL)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_index_value (NLIST_HEADER nlist, DOUBLE indexValue,
                                   UINT Col_List[2], VOIDPTR value, BOOL returnDouble) 
#else
publicdef INT nlz_get_index_value (nlist, indexValue, Col_List, value, returnDouble)
NLIST_HEADER nlist;
UINT Col_List[2];
DOUBLE indexValue;
VOIDPTR value;
BOOL returnDouble;
#endif
    {
    TABLE_HEADER *ptable;
    DATA_HEADER *pdata, *pvalueData;
    COLUMN_HEADER *pinfo;
    INT status = SUCCESS;
    
    /*    start from the first table to find the        */
    /*    qualifies table.                */
    pinfo = nlist->info_ptr + Col_List[1] - 1;
    nl_set_current_row (nlist, (UINT)1, (UINT)1);
    do
        {
        ptable = nlist->current_table_ptr;
        pdata = ptable->data_ptr + Col_List[0] - 1;
        pvalueData = ptable->data_ptr + Col_List[1] - 1;
        if (pdata->start_value > indexValue)
            return NL_INVALID_NLIST1_INCREMENT;
        if (pdata->start_value EQUALS indexValue)
            break;
        if (pdata->start_value + (ptable->nrows - 1) * pdata->increment >= indexValue)
            break;
        if (nlist->current_table < nlist->total_tables)
	  {
	    while ( 
		   (status = nl_set_current_row (nlist,(UINT)1,nlist->current_table+1)) < 0 
		    AND (nlist->current_table < nlist->total_tables))
            {}
            if (status < 0)
                return status;
            }
        else
            return NL_INVALID_TABLE;
        } while (nlist->current_table <= nlist->total_tables);
    
    /*
       * having found qualified table, try to find the row.
    */
    nlist->current_row = (indexValue - pdata->start_value)
    / pdata->increment + 1;
    return(returnDouble) ? nlz_get_value (pvalueData, pinfo, nlist->current_row,
                                          (DOUBLE *)value) :
        nlz_get_current_row (nlist, (UINT)1, Col_List + 1, &value);
    }
/* END:     */
