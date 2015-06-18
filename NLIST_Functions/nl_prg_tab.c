/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRG_TAB.C*/
/* *6    25-OCT-1990 12:24:55 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    25-JUL-1990 15:02:19 VINCE "(SPR 5644) Header Standardization"*/
/* *4     9-APR-1990 10:34:02 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *3    25-MAR-1990 10:33:18 GILLESPIE "(SPR 1) Test for null table before referring to it"*/
/* *2    12-MAR-1990 19:08:52 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:00:04 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_PRG_TAB.C*/
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    To free all memory allocated in specific table, 
    including the header itself.  The table is in the same state
    as follows NL_READ_NLIST.

Prototype:
    publicdef INT nl_purge_table (NLIST_HEADER nlist, UINT table_num);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist       -(NLIST_HEADER) The pointer to an nlist header.
    (I) table_num   -(INT) The index number of the table.
                        0 means use the current table

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_purge_table (NLIST_HEADER nlist, UINT table_num)
#else
publicdef INT nl_purge_table (nlist, table_num)
NLIST_HEADER nlist;
UINT table_num;
#endif
    {
    register UINT i;
    register TABLE_HEADER *ptable;
    
    /* error checking.    */
    
    INIT_TEST (nlist);
    if (table_num EQUALS 0)
        {
        table_num = nlist->current_table;
        }
    ptable = nlz_set_table (nlist, table_num);
    if (ptable EQUALS NULL_TABLE)
        {
        return NL_INVALID_TABLE;
        }
    if (nlist->data_filename == NULL || ptable->data_loaded == NL_DATA_UPDATED)
        {
        return SUCCESS;
        }
    /*
       * free up the user structure if needed
    */
    if (ptable->user_struct_len > 0)
        {
        tc_free ((VOIDPTR)ptable->user_struct);
        }
    /*
       * free up the table name if needed
    */
    if (ptable->name IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)ptable->name);
        }
    /*    If the data in the table has not been
        updated, we free the memory            */
    
    if (ptable->nrows > 0 && ptable->data_loaded IS_NOT_EQUAL_TO NL_DATA_NOT_LOADED)
        {
        for (i = 0; i < nlist->nCol; i++)
            {
            tc_free ((VOIDPTR)(ptable->data_ptr + i)->data);
            }
        }
    tc_free ((VOIDPTR)ptable);
    nlist->current_table_ptr = NULL_TABLE;
    nlist->table_elements[table_num - 1].ptable = NULL_TABLE;
    return SUCCESS;
    
    }
/* END:     */
