/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INT.C */
/* *5    25-OCT-1990 12:26:03 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:04:21 VINCE "(SPR 5644) Header Standardization" */
/* *3     9-APR-1990 10:34:12 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *2     5-MAR-1990 10:49:42 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:07 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_INT.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INQ_ST_INT.C*/
/* *1    19-JUN-1989 13:18:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_INQ_ST_INT.C*/
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
    To retrieve INT information for the specified table.

Prototype:
    publicdef INT nl_inq_table_int(NLIST_HEADER nlist, UINT table_num, INT opcode, 
        INT *integer);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    opcode          -(INT)
    integer         -(INT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_table_int (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                INT *integer)
#else
publicdef INT nl_inq_table_int (nlist, table_num, opcode, integer)
NLIST_HEADER nlist;
UINT table_num;
INT opcode;
INT *integer;
#endif
    {
    UINT old_table_num, old_row_num;
    register TABLE_HEADER *ptable;
    
    /* error checking.    */
    INIT_TEST (nlist);
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    
    if (table_num EQUALS 0)
        {
        table_num = old_table_num;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        return NL_INVALID_TABLE;
    
    switch (opcode)
        {
    case NL_TABLE_NROWS:
        *integer = (INT)ptable->nrows;
        break;
        
    case NL_TABLE_USER_STRUCT_LEN:
        *integer = (INT)ptable->user_struct_len;
        break;
        
    default:
        return NL_INVALID_CODE;
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return SUCCESS;
    
    }
/* END:     */
