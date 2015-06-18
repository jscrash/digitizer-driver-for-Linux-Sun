/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_TAB.C*/
/* *5    25-OCT-1990 12:22:13 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:58:08 VINCE "(SPR 5644) Header Standardization"*/
/* *3     9-APR-1990 10:32:50 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    22-MAR-1990 01:35:11 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *1     5-MAR-1990 10:59:12 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_DEL_TAB.C*/
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
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_delete_table (NLIST_HEADER nlist, UINT table_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_num       -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_table (NLIST_HEADER nlist, UINT table_num)
#else
publicdef INT nl_delete_table (nlist, table_num)
NLIST_HEADER nlist;
UINT table_num;
#endif
    {
    UINT i;
    TABLE_HEADER *ptable;
    DATA_HEADER *pdata;
    
    INIT_TEST (nlist);
    
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    /*
       * free up the user structure if needed
    */
    if (ptable->user_struct_len > 0)
        {
        tc_free ((VOIDPTR)ptable->user_struct);
        ptable->user_struct = (VOIDPTR)0;
        ptable->user_struct_len = 0;
        }
    /*
       * free up the table name if needed
    */
    if (ptable->name IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)ptable->name);
        ptable->name = (CHAR *)0;
        }
    /*
       * free up the data portion of each column
    */
    for (i = 0, pdata = ptable->data_ptr; i < nlist->nCol; i++, pdata++)
        {
        tc_free ((VOIDPTR)pdata->data);
        pdata->data = (BYTE *)0;
        }
    /*
       * reset the links
       * if this is not the last table, then keep the table index number
       * the same as on entry.  If this is the last table, decrement the current
       * table counter
    */
    if (nlist->total_tables EQUALS 1)
        {
        nlist->current_table_ptr = NULL_TABLE;
        (nlist->current_table)--;
        }
    else if (table_num < nlist->total_tables)
        {
        hoblockmove ((VOIDPTR)&nlist->table_elements[table_num],
                     (VOIDPTR)&nlist->table_elements[table_num - 1],
                     sizeof(TABLE_ELEMENT) * (nlist->total_tables - table_num));
        nlist->current_table_ptr = nlist->table_elements[nlist->current_table -
                                                         1].ptable;
        }
    else
        {
        (nlist->current_table)--;
        nlist->current_table_ptr = nlist->table_elements[nlist->current_table -
                                                         1].ptable;
        }
    tc_zeroes ((VOIDPTR)&nlist->table_elements[nlist->total_tables - 1],
               sizeof(TABLE_ELEMENT));
    
    nlist->total_tables--;
    nlist->total_rows -= ptable->nrows;
    nlist->updated = TRUE;
    
    tc_free ((VOIDPTR)ptable);
    
    nlz_compute_nlist_stats (nlist);
    
    return SUCCESS;
    }
/* END:     */
