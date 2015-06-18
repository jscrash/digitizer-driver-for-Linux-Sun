/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_ROWS.C */
/* *7    25-OCT-1990 12:22:40 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *6    25-JUL-1990 14:59:22 VINCE "(SPR 5644) Header Standardization" */
/* *5     9-APR-1990 10:33:01 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *4    30-MAR-1990 11:41:56 GILLESPIE "(SPR 1) Add more robust error checking" */
/* *3     5-MAR-1990 10:46:39 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *2    13-FEB-1990 15:47:27 GILLESPIE "(SPR 1) Add COLUMN_TEST" */
/* *1    24-SEP-1989 22:35:21 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_ROWS.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_POINTS.C*/
/* *1    19-JUN-1989 13:18:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_GET_POINTS.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Starting form the current row, returns a array of rows and flag
    associated with the current row.

    MODIFIED 30 July 89 J Gillespie

    Current row is changed to row AFTER the last
    row fetched.  If the last row in a table is retrieved, then
    nl_get_rows sets the current row to the last row in the table.

Prototype:
    publicdef INT nl_get_rows (NLIST_HEADER nlist, UINT nallocated, UINT nCol, 
        UINT *Col_List, VOIDPTR *Var_List, UINT *nrows);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.              
    nCol            -(UINT) The total requested columns.               
    nallocated      -(UINT) Allocated space in the returned array.      
    nrows           -(UINT *) Totally returned rows.                        
    Col_List        -(UINT *) A list of given columns.                   
    Var_List        -(VOIDPTR *) Returned value list.                      

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_DATA_TYPE
    NL_INVALID_POINTER
    NL_INVALID_TABLE
    Function returns status from nlz_inq_row_flag and nlz_load_data.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_get_rows (NLIST_HEADER nlist, UINT nallocated, UINT nCol,
                           UINT *Col_List, VOIDPTR *Var_List, UINT *nrows)
#else
publicdef INT nl_get_rows (nlist, nallocated, nCol, Col_List, Var_List, nrows)
NLIST_HEADER nlist;
UINT nCol;
UINT nallocated;
UINT *nrows;
UINT *Col_List;
VOIDPTR *Var_List;
#endif
    {
    UINT rows_to_move;
    UINT offset;
    register UINT i, j;
    INT status = SUCCESS;
    DOUBLE x;
    register COLUMN_HEADER *pinfo;
    DATA_HEADER *pdata;
    TABLE_HEADER *ptable;
    register CHAR *pbyte;
    
    /*  *************************************************************************  */
    /*   error checking.                         */
    
    if (nrows EQUALS (UINT *)0)
        {
        return NL_INVALID_POINTER;
        }
    else
        {
        *nrows = 0;
        }
    INIT_TEST (nlist);
    
    EMPTY_TEST (nlist);
    
    for (i = 0; i < nCol; i++)
        {
        COLUMN_TEST (nlist, Col_List[i]);
        if (Var_List[i] EQUALS (VOIDPTR)0)
            {
            return NL_INVALID_POINTER;
            }
        }
    NROWS_TEST (nallocated);
    
    /*      calculates the total number of rows        */
    /*      should be retrieved.                */
    /*      If the total number of rows after the   */
    /*      current row is less than the requested  */
    /*      rows, then just retrieve as many as        */
    /*      have, otherwise move the requested number */
    
    ptable = nlz_set_table (nlist, nlist->current_table);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    rows_to_move = ptable->nrows - nlist->current_row + 1;
    
    if (rows_to_move > nallocated)
        {
        rows_to_move = nallocated;
        }
    *nrows = rows_to_move;
    
    /*      if the table has not been loaded, then   */
    /*      loads first.                    */
    
    if (ptable->data_loaded EQUALS NL_DATA_NOT_LOADED AND ptable->nrows != 0)
        {
        status = nlz_load_data (nlist);
        if (status IS_NOT_EQUAL_TO SUCCESS)
            {
            return status;
            }
        }
    /*      retrieve rows for each column.        */
    
    for (i = 0; i < nCol; i++)
        {
        offset = nlist->current_row - 1;
        pinfo = nlist->info_ptr + Col_List[i] - 1;
        pdata = ptable->data_ptr + Col_List[i] - 1;
        pbyte = (CHAR *)Var_List[i];
        if (pdata->inc_value)
            {
            /*      cast the calculated data value into the   */
            /*      appropriate data type                */
            
            for (j = 0; j < rows_to_move; j++, offset++)
                {
                x = pdata->start_value + offset * pdata->increment;
                switch (pinfo->data_type)
                    {
                    /*    ----------------------------------------    */
                    
                case INT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(CHAR):
                        *pbyte = (CHAR)(x + NL_ROUND_ERROR);
                        pbyte += sizeof(CHAR);
                        break;
                    case sizeof(SHORT):
                        *(SHORT *)pbyte = x + NL_ROUND_ERROR;
                        pbyte += sizeof(SHORT);
                        break;
                    case sizeof(INT):
                        *(INT *)pbyte = x + NL_ROUND_ERROR;
                        pbyte += sizeof(INT);
                        break;
                        }
                    break;
                    
                    /*    ----------------------------------------    */
                    
                case FLOAT_TYPE:
                    switch (pinfo->data_size)
                        {
                    case sizeof(FLOAT):
                        *(FLOAT *)pbyte = (FLOAT)x;
                        pbyte += sizeof(FLOAT);
                        break;
                    case sizeof(DOUBLE):
                        *(DOUBLE *)pbyte = x;
                        pbyte += sizeof(DOUBLE);
                        break;
                        }
                    break;
                default:
                    status = NL_INVALID_DATA_TYPE;
                    return status;
                    }
                }
            }
        else                        /*    do this for non-incrementing columns        */
            {
            hoblockmove ((VOIDPTR)(pdata->data + offset * pinfo->data_size),
                         (VOIDPTR)pbyte, (pinfo->data_size * rows_to_move));
            }
        }
    /*    The following sets the current row to be
        the row after the last row fetched.
        This allows successive gets without having
        to keep setting the current row.  The
        test makes sure we don't fall off the end
        (i.e., when AT the last row on entry,
        then we don't want to set the current row
        ahead to a non-existent row            */
    
    nlist->current_row += rows_to_move;
    
    if (nlist->current_row > ptable->nrows)
        {
        nlist->current_row = ptable->nrows;
        }
    return nlz_inq_row_flag (nlist);
    }
/* END:     */
