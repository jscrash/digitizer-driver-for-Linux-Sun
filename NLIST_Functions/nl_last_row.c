/* DEC/CMS REPLACEMENT HISTORY, Element NL_LAST_ROW.C */
/* *5    25-OCT-1990 12:24:15 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:01:07 VINCE "(SPR 5644) Header Standardization" */
/* *3    12-MAR-1990 19:08:28 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:47:49 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:30 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_LAST_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_LAST_POINT.C*/
/* *1    19-JUN-1989 13:19:24 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_LAST_POINT.C*/
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
    Retrieve tha value for the last row in the N-list.  The current row
    will be set to the last row of the last table.

Prototype:
    publicdef INT nl_last_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.         
    nCol            -(UINT) The requested columns.      
    Col_List        -(UINT *) A list of requested columns.  
    Var_List        -(VOIDPTR *) A list of pointers of returned values. 

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_ROW
    Function returns status from nl_set_current_row and nlz_get_current_row.
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_last_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                           VOIDPTR *Var_List)
#else
publicdef INT nl_last_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    INT status;                     /*   returned status.                        */
    UINT i;                         /*   index.                                  */
    BOOL quit = FALSE;              /*   flag of finding the last unempty table.*/
    TABLE_HEADER *ptable;           /*   pointer of current table.              */
    
    /*  ************************************************************************   */
    /*   error checking.                         */
    
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    
    status = NL_INVALID_ROW;
    /*   to find the last valid row.           */
    
    for (i = nlist->total_tables, quit = FALSE; i > 0 && (!quit); i--)
        {
        ptable = nlz_set_table (nlist, i);
        if (ptable EQUALS NULL_TABLE || ptable->nrows EQUALS 0)
            {
            continue;
            }
        quit = TRUE;
        }
    if (quit)
        status = nl_set_current_row (nlist, ptable->nrows, i + 1);
    
    if (status >= 0 && quit)
        {
        status = nlz_get_current_row (nlist, nCol, Col_List, Var_List);
        }
    return status;
    }
/* END:     */
