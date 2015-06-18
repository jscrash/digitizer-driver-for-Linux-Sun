/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIND_ROW.C */
/* *5    25-OCT-1990 12:22:24 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    20-AUG-1990 17:09:26 KEE "(SPR -1) fixing compile error at sun" */
/* *3    25-JUL-1990 14:58:37 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:46:10 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:02 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIND_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIND_POINT.C*/
/* *1    19-JUN-1989 13:18:31 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIND_POINT.C*/
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
    Locate the next row that falls within the predefined tolerance window
    for each column sfecified.  The value would be returned by  a call to
    NL_GET_NPOINTS().  This call always starts from the cuurent row.

    Will return NL_EOL bit set in flag if  could not locate a row that
    falls in tolerance. Note that the value list is input doubles
    regardless of the  column data type

Prototype:
    publicdef INT nl_find_row(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        FLOAT **Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of NLIST_HEADER.          
    nCol            -(UINT) The given columns.
    Col_List        -(UINT *)
    Var_List        -(FLOAT **)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_EOL
    
Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_find_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                           FLOAT **Var_List)
#else
publicdef INT nl_find_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
FLOAT **Var_List;
#endif
    {
    DOUBLE x;
    COLUMN_HEADER *pinfo;
    TABLE_HEADER *ptable;
    DATA_HEADER *pdata;
    BOOL found;
    UINT i;
    INT status;
    UINT table_num = 1;
    
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    /*
       * check the current pointlist.  If the current row is undefined, means the
       * current row is equals to zero, the sets the current row to be the first
       * row of the first table.
    */
    if (nlist->current_row EQUALS 0)
        {
        do
            {
            status = nl_set_current_row (nlist, (UINT)1, table_num++);
            if (status >= 0)
                break;
            } while (status EQUALS NL_INVALID_ROW AND table_num <=
                     nlist->total_tables);
        if (status < 0)
            return status;
        }
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        Col_List[i]--;
        }
    status = nlz_inq_row_flag (nlist);
    ptable = nlist->current_table_ptr;
    
    /*
       * PROGRAMMERS NOTE: Col_List values have 1 subtracted from them above,
       * so Col_List = 1 becomes Col_List = 0 for use in computing pointer
       * offsets in the main loop
    */
    while (NOT (status& NL_EOL))
        {
        /*
           * if the table has not been loaded, then loads first.
        */
        if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows)
            {
            status = nlz_load_data (nlist);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            }
        found = TRUE;
        for (i = 0; i < nCol; i++)
            {
            pinfo = nlist->info_ptr + Col_List[i];
            pdata = ptable->data_ptr + Col_List[i];
            nlz_get_value (pdata, pinfo, nlist->current_row, &x);
            if (ABS (*Var_List[i] - x) > pinfo->tolerance)
                {
                found = FALSE;
                break;
                }
            }
        if (found)
            {
            for (i = 0; i < nCol; i++)
                {
                Col_List[i]++;
                }
            status = nlz_inq_row_flag (nlist);
            return status;
            }
        /*
           * set internal counters to next row
        */
        if (nlist->current_row EQUALS ptable->nrows)
            {
            if (nlist->current_table < nlist->total_tables)
                {
                ptable = nlz_set_table (nlist, nlist->current_table + 1);
                if (ptable EQUALS NULL_TABLE)
                    {
                    return NL_INVALID_TABLE;
                    }
                while (ptable->nrows EQUALS 0 AND nlist->current_table <
                       nlist->total_tables)
                    {
                    ptable = nlz_set_table (nlist, nlist->current_table + 1);
                    }
                if (nlist->current_table EQUALS nlist->total_tables AND ptable->
                    nrows EQUALS 0)
                    {
                    status = NL_EOL | nlz_inq_row_flag (nlist);
                    }
                }
            else
                {
                status = NL_EOL | nlz_inq_row_flag (nlist);
                }
            }
        else
            {
            (nlist->current_row)++;
            }
        }
    /* end of WHILE loop */
    
    for (i = 0; i < nCol; i++)
        {
        Col_List[i]++;
        }
    return status;
    
    }
/* END:     */
