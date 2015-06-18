/* DEC/CMS REPLACEMENT HISTORY, Element NL_PREV_ROW.C */
/* *5    25-OCT-1990 12:24:46 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/*  3B1   3-OCT-1990 10:57:35 GILLESPIE "Change file type to stream" */
/* *4    20-AUG-1990 17:10:39 KEE "(SPR -1) fixing compile error at sun" */
/* *3    25-JUL-1990 15:02:05 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:48:26 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:42 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PREV_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRV_PNT.C*/
/* *1    19-JUN-1989 13:19:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRV_PNT.C*/
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
    Return the previous row.  Can span tables.  If at the first row,
    returns first row and 'NL_EOL' flag

Prototype:
    publicdef INT nl_previous_row(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.         
    nCol            -(UINT) Requested columns.                  
    Col_List        -(UINT *) List of requested columns.      
    Var_List        -(VOIDPTR *) List of pointer of returned values.

Return Value/Status:
    See description.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_previous_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                               VOIDPTR *Var_List)
#else
publicdef INT nl_previous_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    INT status = SUCCESS;
    UINT oldCurrentTable, oldCurrentRow;
    TABLE_HEADER *ptable;
    
    /*  *************************************************************************  */
    /*   error checking.                    */
    
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    oldCurrentTable = nlist->current_table;
    oldCurrentRow = nlist->current_row;
    
    if (nlist->current_row <= 1)
        {
        if (nlist->current_table > 1)
            {
            
            /*   skips all empty tables.                */
            
            do
                {
                ptable = nlz_set_table (nlist, nlist->current_table - 1);
                } while (nlist->current_row EQUALS 0 AND nlist->current_table > 1);
            
            /*   if there are no more non_empty tables  */
            /*   before the original current row, set  */
            /*   the current row back to the original  */
            /*   current row.                          */
            
            if (nlist->current_row EQUALS 0)
                {
                nl_set_current_row (nlist, oldCurrentRow, oldCurrentTable);
                status = NL_EOL | nlz_get_current_row (nlist, nCol, Col_List,
                                                       Var_List);
                return status;
                }
            else
                status = nl_set_current_row (nlist, ptable->nrows,
                                             nlist->current_table);
            }
        else
            {
            status = NL_EOL | nlz_get_current_row (nlist, nCol, Col_List, Var_List);
            return status;
            }
        }
    else
        {
        (nlist->current_row)--;
        }
    if (status >= 0)
        {
        status = nlz_get_current_row (nlist, nCol, Col_List, Var_List);
        }
    return status;
    }
/* END:     */
