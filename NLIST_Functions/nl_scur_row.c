/* DEC/CMS REPLACEMENT HISTORY, Element NL_SCUR_ROW.C */
/* *5    25-JUL-1990 15:05:20 VINCE "(SPR 5644) Header Standardization" */
/* *4     9-APR-1990 10:34:20 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *3     5-MAR-1990 10:50:08 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *2    25-SEP-1989 12:02:42 GILLESPIE "(SPR 200) New n-List Mods V4.0" */
/* *1    25-SEP-1989 12:00:32 GILLESPIE "New n-List routine to set current row" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SCUR_ROW.C */
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"

/* Function Description -----------------------------------------------------
Description:
    Set the internal counters to reflect a new current row number.

Prototype:
    publicdef INT nl_set_current_row(NLIST_HEADER nlist, UINT row, UINT table_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.  
    row             -(UINT) Given row number.           
    table_num       -(UINT) Given table number.        

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    Function returns the flags associated with the new row.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_current_row (NLIST_HEADER nlist, UINT row, UINT table_num)
#else
publicdef INT nl_set_current_row (nlist, row, table_num)
NLIST_HEADER nlist;
UINT row;
UINT table_num;
#endif
    {
    register TABLE_HEADER *ptable;  /*      pointer of current table.            */
    UINT old_table_num, old_row_num;
    
    /*  ************************************************************************   */
    
    /*      error checking.                */
    
    INIT_TEST (nlist);
    
    /*    inquire the old current row.            */
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    
    /*      if the given table number is zero, set   */
    /*      it to be the current (old) table.        */
    
    if (table_num EQUALS 0)
        {
        table_num = old_table_num;
        }
    /*      set the current table.            */
    
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    /*      set the current row.            */
    
    if (row > ptable->nrows)
        {
        nlz_set_table (nlist, old_table_num);
        nlist->current_row = old_row_num;
        return NL_INVALID_ROW;
        }
    else if (row EQUALS 0)
        {
        if (ptable->nrows > 0)
            {
            nlz_set_table (nlist, old_table_num);
            nlist->current_row = old_row_num;
            return NL_INVALID_ROW;
            }
        nlist->current_row = row;
        return SUCCESS;
        }
    nlist->current_row = row;
    return nlz_inq_row_flag (nlist);
    }
/* END:     */
