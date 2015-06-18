/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_STCT.C*/
/* *5    25-JUL-1990 15:07:09 VINCE "(SPR 5644) Header Standardization"*/
/* *4     1-JUN-1990 22:28:25 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *3     9-APR-1990 10:34:39 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    12-MAR-1990 19:10:07 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:01:31 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_STAB_STCT.C*/
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    Set the user structure the indicated table
    this becomes the new current table.

Prototype:
    publicdef INT nl_set_table_user_struct(NLIST_HEADER nlist,
        UINT table_num, VOIDPTR user_struct_ptr, size_t nbytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)
    nbytes          -(size_t)
    user_struct_ptr -(VOIDPTR)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_table_user_struct (NLIST_HEADER nlist, UINT table_num,
                                        VOIDPTR user_struct_ptr, size_t nbytes)
#else
publicdef INT nl_set_table_user_struct (nlist, table_num, user_struct_ptr, nbytes)
NLIST_HEADER nlist;
UINT table_num;
size_t nbytes;
VOIDPTR user_struct_ptr;
#endif
    {
    TABLE_HEADER *ptable;
    UINT old_table_num, old_row_num;
    INT status = SUCCESS;
    
    /* error checking    */
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
    /* delete the previous structure, if  */
    /* there is one.              */
    if (ptable->user_struct != (VOIDPTR)0)
        tc_free ((VOIDPTR)ptable->user_struct);
    status = nlz_copy_structure (user_struct_ptr, nbytes, &ptable->user_struct,
                                 &ptable->user_struct_len);
    
    /* set back to the old current row.   */
    nl_set_current_row (nlist, old_row_num, old_table_num);
    
    return status;
    }
/* END:     */
