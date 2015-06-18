/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_INT.C */
/* *5    25-OCT-1990 12:27:33 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:07:02 VINCE "(SPR 5644) Header Standardization" */
/* *3     9-APR-1990 10:34:35 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *2     5-MAR-1990 10:51:23 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:56 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_INT.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STSTR_INT.C*/
/* *1    19-JUN-1989 13:21:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STSTR_INT.C*/
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
    Set the integer variable status in the specified table.

Prototype:
    publicdef INT nl_set_table_int(NLIST_HEADER nlist, UINT table_num, INT opcode, 
        INT integer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    integer         -(INT)
    opcode          -(INT)
    table_num       -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_INVALID_CODE
    NL_INVALID_COLUMN
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_table_int (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                INT integer)
#else
publicdef INT nl_set_table_int (nlist, table_num, opcode, integer)
NLIST_HEADER nlist;
INT integer, opcode;
UINT table_num;
#endif
    {
    UINT i;
    UINT old_table_num, old_row_num;
    INT add_points, status = SUCCESS;
    COLUMN_HEADER *pinfo;
    TABLE_HEADER *ptable;
    
    INIT_TEST (nlist);
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    if (table_num EQUALS 0)
        {
        table_num = nlist->current_table;
        }
    switch (opcode)
        {
    case NL_TABLE_NROWS:
        /* error checking that all columns are INCREMENT_LIST.   */
        for (i = 0; i < nlist->nCol; i++)
            {
            pinfo = nlist->info_ptr + i;
            if (pinfo->inc_value EQUALS VALUE_LIST)
                return NL_INVALID_COLUMN;
            }
        ptable = nlz_set_table (nlist, table_num);
        if (ptable EQUALS NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        add_points = integer - ptable->nrows;
        ptable->nrows = (UINT)integer;
        nlist->total_rows += add_points;
        status = SUCCESS;
        break;
        
    default:
        status = NL_INVALID_CODE;
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
