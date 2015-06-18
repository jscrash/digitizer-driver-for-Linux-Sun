/* DEC/CMS REPLACEMENT HISTORY, Element NL_NEXT_ROW.C */
/* *5    25-OCT-1990 12:24:19 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/*  3B1   3-OCT-1990 10:56:53 GILLESPIE "Change file type to stream" */
/* *4    20-AUG-1990 17:10:32 KEE "(SPR -1) fixing compile error at sun" */
/* *3    25-JUL-1990 15:01:14 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:48:00 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:32 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_NEXT_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_NEXT_ROW.C*/
/* *1    19-JUN-1989 13:19:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_NEXT_ROW.C*/
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
    Retrieve the value for the next row in the N-list.
    If the current row is at the last row, returns last row and
    'NL_EOL' flag.

Prototype:
    publicdef INT nl_next_row(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) The pointer to an NLIST_HEADER.   
    nCol            -(UINT) The requested columns.            
    Col_List        -(UINT *) List of requested columns.       
    Var_List        -(VOIDPTR *) List of Rower of returned value.    

Return Value/Status:
    Function returns status from nlz_get_current_row.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_next_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                           VOIDPTR *Var_List)
#else
publicdef INT nl_next_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    INT status = SUCCESS;
    UINT i;
    UINT oldCurrentTable, oldCurrentRow;
    
    /*  ************************************************************************   */
    /*   error checking.                         */
    
    INIT_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        }
    EMPTY_TEST (nlist);
    oldCurrentRow = nlist->current_row;
    oldCurrentTable = nlist->current_table;
    if (nlist->current_row EQUALS nlist->current_table_ptr->nrows)
        {
        if (nlist->current_table < nlist->total_tables)
            {
            
            /*   skips all empty tables.                */
            
            do
                {
                nlz_set_table (nlist, nlist->current_table + 1);
                } while (nlist->current_row EQUALS 0 AND nlist->current_table <
                         nlist->total_tables);
            
            /*   if there are no more un_empty tables   */
            /*   after old current table, set the current*/
            /*   row to be the original one.           */
            
            if (nlist->current_row EQUALS 0)
                {
                nl_set_current_row (nlist, oldCurrentRow, oldCurrentTable);
                status = NL_EOL | nlz_get_current_row (nlist, nCol, Col_List,
                                                       Var_List);
                return status;
                }
            }
        else
            {
            status = NL_EOL | nlz_get_current_row (nlist, nCol, Col_List, Var_List);
            return status;
            }
        }
    else
        {
        (nlist->current_row)++;
        }
    status = nlz_get_current_row (nlist, nCol, Col_List, Var_List);
    
    return status;
    }
/* END:     */
