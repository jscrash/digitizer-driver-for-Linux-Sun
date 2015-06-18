/* DEC/CMS REPLACEMENT HISTORY, Element NL_QCUR_ROW.C */
/* *3    25-JUL-1990 15:02:30 VINCE "(SPR 5644) Header Standardization" */
/* *2     5-MAR-1990 10:48:36 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:35:46 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QCUR_ROW.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QCURPNT.C*/
/* *1    19-JUN-1989 13:19:55 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QCURPNT.C*/
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
    Retrieve the current row and table number.

Prototype:
    publicdef INT nl_inq_current_row(NLIST_HEADER nlist, UINT *pRow, UINT *pTable);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.          
    pTable          -(UINT *) Returned pointer of current table.    
    pRow            -(UINT *) Returned pointer of current row.    

Return Value/Status:
    SUCCESS - Successful completion.
    NL_EMPTY_LIST
    NL_EMPTY_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_current_row (NLIST_HEADER nlist, UINT *pRow, UINT *pTable)
#else
publicdef INT nl_inq_current_row (nlist, pRow, pTable)
NLIST_HEADER nlist;
UINT *pTable;
UINT *pRow;
#endif
    {
    INT status;                     /*%P retruned status.                        */
    
    /*  *************************************************************************  */
    INIT_TEST (nlist);
    
    *pTable = nlist->current_table;
    *pRow = nlist->current_row;
    
    /*   if this is a empty list return          */
    /*   NL_EMPTY_NLIST                          */
    /*   if there are no rows in the Table   */
    /*   return NL_EMPTY_TABLE                  */
    
    if (*pRow EQUALS 0)
        {
        if (*pTable EQUALS 0)
            {
            status = NL_EMPTY_NLIST;
            }
        else
            {
            status = NL_EMPTY_TABLE;
            }
        }
    else
        {
        status = nlz_inq_row_flag (nlist);
        }
    return status;
    }
/* END:     */
