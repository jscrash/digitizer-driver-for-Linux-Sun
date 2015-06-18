/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIRST_ROW.C */
/* *4    25-OCT-1990 12:22:30 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *3    25-JUL-1990 14:58:47 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:46:31 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:19 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FIRST_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FST_PNT.C*/
/* *1    19-JUN-1989 13:18:37 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_FST_PNT.C*/
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
    Retrieve the value of the first row.

Prototype:
    publicdef INT nl_first_row(NLIST_HEADER nlist, UINT nCol, UINT *Col_List, 
        VOIDPTR *Var_List);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.     
    nCol            -(UINT) The total columns requested.         
    Col_List        -(UINT *) The list of given columns.          
    Var_List        -(VOIDPTR *) Returned value list.                  

Return Value/Status:
    Function returns the value of the first row.
    
Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_first_row (NLIST_HEADER nlist, UINT nCol, UINT *Col_List,
                            VOIDPTR *Var_List)
#else
publicdef INT nl_first_row (nlist, nCol, Col_List, Var_List)
NLIST_HEADER nlist;
UINT nCol;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    INT status;                     /*   returned flag.                          */
    UINT table_num = 1;             /*   indexes.                                */
    
    /*  ************************************************************************** */
    /*   error checking.                         */
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    
    /*   skips all empty tables, the set the    */
    /*   current row to the first row.       */
    
    do
        {
        status = nl_set_current_row (nlist, (UINT)1, table_num++);
        if (status >= 0)
            {
            status = nlz_get_current_row (nlist, nCol, Col_List, Var_List);
            break;
            }
        } while (status EQUALS NL_INVALID_ROW AND table_num <= nlist->total_tables);
    
    return status;
    
    }
/* END:     */
