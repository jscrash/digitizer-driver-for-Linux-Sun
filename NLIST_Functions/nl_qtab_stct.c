/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_STCT.C*/
/* *4    25-OCT-1990 12:26:10 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 15:04:31 VINCE "(SPR 5644) Header Standardization"*/
/* *2    27-MAR-1990 14:03:44 GILLESPIE "(SPR 1) Fixes"*/
/* *1     5-MAR-1990 11:00:48 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QTAB_STCT.C*/
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
#include "esi_nl_msg.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    Inquire the user structure associated with a table.

Prototype:
    publicdef INT nl_inq_table_user_struct(NLIST_HEADER nlist, UINT table_num, 
        size_t nallocated, VOIDPTR user_struct_ptr, size_t *nbytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    nallocated      -(size_t)
    nbytes          -(size_t *)
    user_struct_ptr -(VOIDPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NULL_VALUES_RETURNED
    NL_INVALID_TABLE
    NL_INVALID_ARRAY_SIZE
    NL_INVALID_POINTER
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_table_user_struct (NLIST_HEADER nlist, UINT table_num,
                                        size_t nallocated, VOIDPTR user_struct_ptr,
                                        size_t *nbytes)
#else
publicdef INT nl_inq_table_user_struct (nlist, table_num, nallocated, user_struct_ptr,
                                        nbytes)
NLIST_HEADER nlist;
UINT table_num;
size_t nallocated;
size_t *nbytes;
VOIDPTR user_struct_ptr;
#endif
    {
    TABLE_HEADER *ptable;
    INT status = SUCCESS;
    UINT old_table_num, old_row_num;
    
    /* error checking.    */
    INIT_TEST (nlist);
    if (user_struct_ptr EQUALS (VOIDPTR)0)
        {
        return NL_INVALID_POINTER;
        }
    if (nallocated EQUALS 0)
        {
        return NL_INVALID_ARRAY_SIZE;
        }
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
    if (ptable->user_struct_len EQUALS 0)
        {
        nl_set_current_row (nlist, old_row_num, old_table_num);
        return NL_NULL_VALUES_RETURNED;
        }
    *nbytes = (ptable->user_struct_len > nallocated) ?
              nallocated : ptable->user_struct_len;
    status = (ptable->user_struct_len > nallocated) ? NL_OVERFLOW : SUCCESS;
    hoblockmove (ptable->user_struct, user_struct_ptr, *nbytes);
    
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    return status;
    }
/* END:     */
