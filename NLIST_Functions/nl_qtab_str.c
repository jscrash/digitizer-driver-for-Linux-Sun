/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_STR.C */
/* *5    25-OCT-1990 12:26:18 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 15:04:39 VINCE "(SPR 5644) Header Standardization" */
/* *3     9-APR-1990 10:34:16 GILLESPIE "(SPR 1) Remove superflurous checks" */
/* *2     5-MAR-1990 10:49:49 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:36:13 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_STR.C */
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QSTK_STR.C*/
/* *1    19-JUN-1989 13:20:21 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QSTK_STR.C*/
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
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    Inquire the name associated with a table.

Prototype:
    publicdef INT nl_inq_table_string(NLIST_HEADER nlist, UINT table_num, INT opcode,
        size_t nallocated, CHAR *name, size_t *nbytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    nallocated      -(size_t)
    opcode          -(INT)
    nbytes          -(size_t *)
    name            -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_CODE
    NL_INVALID_POINTER
    NL_INVALID_TABLE
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_table_string (NLIST_HEADER nlist, UINT table_num, INT opcode,
                                   size_t nallocated, CHAR *name, size_t *nbytes)
#else
publicdef INT nl_inq_table_string (nlist, table_num, opcode, nallocated, name, nbytes)
NLIST_HEADER nlist;
UINT table_num;
size_t nallocated;
INT opcode;
size_t *nbytes;
CHAR *name;
#endif
    {
    TABLE_HEADER *ptable;
    INT status = FAIL;
    UINT old_table_num, old_row_num;
    
    INIT_TEST (nlist);
    
    old_row_num = nlist->current_row;
    old_table_num = nlist->current_table;
    
    if (table_num EQUALS 0)
        {
        table_num = old_table_num;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    *nbytes = 0;
    switch (opcode)
        {
    case NL_TABLE_NAME:
        if (ptable->name IS_NOT_EQUAL_TO NULL)
            {
            if (strlen ((char *)ptable->name) < nallocated)
                {
                *nbytes = strlen ((char *)ptable->name);
                strcpy ((char *)name, (char *)ptable->name);
                status = SUCCESS;
                }
            else
                status = NL_OVERFLOW;
            }
        else
            {
            status = NL_INVALID_POINTER;
            }
        break;
        
    default:
        status = NL_INVALID_CODE;
        }
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    return status;
    
    }
/* END:     */
