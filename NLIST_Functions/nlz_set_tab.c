/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SET_TAB.C*/
/* *4    20-AUG-1990 17:08:58 KEE "(SPR -1) fixing compile error at sun"*/
/* *3    25-JUL-1990 14:54:35 VINCE "(SPR 5644) Header Standardization"*/
/* *2     9-APR-1990 10:32:17 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *1     5-MAR-1990 10:57:07 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_SET_TAB.C*/
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
    Set the internal counters and pointers to a new table.
    The 'current row' is set to the first row of the table.
    If there are no rows in the table, the current row is set to 0.
        
Prototype:
    publicdef TABLE_HEADER *nlz_set_table (NLIST_HEADER nlist, UINT table_num)
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER) Pointer of given NLIST_HEADER.        
    table_num       -(UINT) The specified table number.        

Return Value/Status:
    Function returns the value of ptable, or NULL_TABLE.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    Function assumes that the n-List has been correctly initialized.
    The requested table number is in the range 1 <= table_num <=
    total_tables.
        
    ERROR Condition:
    Ptable is set to NULL_TABLE if the above assumptions are
    incorrect.  The state of the n-List is left unaltered.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef TABLE_HEADER *nlz_set_table (NLIST_HEADER nlist, UINT table_num)
#else
publicdef TABLE_HEADER *nlz_set_table (nlist, table_num)
NLIST_HEADER nlist;
UINT table_num;
#endif
    {
    register TABLE_HEADER *ptable;
    
    /*    If the requested table is outside the
        existing tables range, returns NULL_TABLE
        and the current status not change.        */
    
    if ((table_num == 0) OR (table_num > nlist->total_tables))
        {
        return NULL_TABLE;
        }
    if (table_num EQUALS nlist->current_table)
        {
        if (nlist->current_table_ptr != NULL_TABLE)
            {
            return nlist->current_table_ptr;
            }
        }
    ptable = nlist->table_elements[table_num - 1].ptable;
    if (ptable EQUALS NULL_TABLE)
        {
        ptable = nlz_read_table_header (nlist, &nlist->table_elements[table_num - 1]);
        }
    /*    If the table is PURGED, then it is possible
        for the current table and row to still be
        active.  The following allows the table to
        be PURGED and restored without changing
        these 'pointer' values.  This is the case
        only if 1) The current_table field in the
        n-List header is set to the requested
        table, and 2) the table pointer was NULL    */
    
    if (ptable IS_NOT_EQUAL_TO NULL_TABLE)
        {
        nlist->current_table_ptr = ptable;
        if (table_num IS_NOT_EQUAL_TO nlist->current_table)
            {
            nlist->current_table = table_num;
            nlist->current_row = ptable->nrows ? 1 : 0;
            }
        }
    return ptable;
    }
/* END:     */
