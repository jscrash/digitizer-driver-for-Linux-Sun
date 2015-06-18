/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLR_NL.C*/
/* *5    25-OCT-1990 12:21:22 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:56:23 VINCE "(SPR 5644) Header Standardization"*/
/* *3    25-MAY-1990 22:57:34 GILLESPIE "(SPR 1) DON'T RE-READ TABLES JUST TO FREE THEM"*/
/* *2    12-MAR-1990 19:06:17 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:48 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLR_NL.C*/
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
    To clean up a N-list, includes all tables, the user defined structure,
    all defined character strings, ....

    After being cleared, the N-list will be same as a new initialized
    N-list.

Prototype:
    publicdef INT nl_clear_nlist (NLIST_HEADER nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) nlist       -(NLIST_HEADER) The pointer of given N-list.  

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_clear_nlist (NLIST_HEADER nlist)
#else
publicdef INT nl_clear_nlist (nlist)
NLIST_HEADER nlist;
#endif
    {
    register UINT i, j;             /*   workspace of index.           */
    UINT table_num;
    register TABLE_HEADER *ptable;  /*   pointer of currently working table.   */
    DATA_HEADER *pdata;             /*   workspace of DATA_HEADER.               */
    NL_METHOD_HEADER *pMethod, *pLast;
    
    INIT_TEST (nlist);
    
    /*   first, delete all tables.    */
    
    for (j = 0, table_num = nlist->total_tables; j < table_num; j++)
        {
        ptable = nlist->table_elements[j].ptable;
        if (ptable EQUALS NULL_TABLE)
            {
            continue;
            }
        for (i = 0, pdata = ptable->data_ptr; i < nlist->nCol; i++, pdata++)
            {
            if (pdata->data IS_NOT_EQUAL_TO (BYTE *)0)
                {
                tc_free ((VOIDPTR)pdata->data);
                }
            pdata->data = (BYTE *)0;
            }
        if (ptable->name IS_NOT_EQUAL_TO (CHAR *)NULL)
            {
            tc_free ((VOIDPTR)ptable->name);
            ptable->name = (CHAR *)0;
            }
        if (ptable->user_struct_len > 0)
            {
            tc_free ((VOIDPTR)ptable->user_struct);
            ptable->user_struct = (VOIDPTR)0;
            ptable->user_struct_len = 0;
            }
        tc_free ((VOIDPTR)ptable);
        }
    /* free the table array.            */
    if (nlist->table_elements != (TABLE_ELEMENT *)0)
        {
        tc_free ((VOIDPTR)nlist->table_elements);
        }
    /*   free all allocated memory in the HEADER.*/
    
    if (nlist->name IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)nlist->name);
        nlist->name = (CHAR *)0;
        }
    if (nlist->user_struct_len > 0)
        {
        tc_free ((VOIDPTR)nlist->user_struct);
        nlist->user_struct = (VOIDPTR)0;
        nlist->user_struct_len = 0;
        }
    if (nlist->data_filename IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)nlist->data_filename);
        nlist->data_filename = (CHAR *)0;
        }
    if (nlist->prev_nlist IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)nlist->prev_nlist);
        nlist->prev_nlist = (CHAR *)0;
        }
    if (nlist->next_nlist IS_NOT_EQUAL_TO (CHAR *)NULL)
        {
        tc_free ((VOIDPTR)nlist->next_nlist);
        nlist->next_nlist = (CHAR *)0;
        }
    for (pMethod = nlist->method_list; pMethod != (NL_METHOD_HEADER *)0;
         pMethod = pLast)
        {
        pLast = pMethod->next;
        tc_free ((VOIDPTR)pMethod);
        }
    /*      reset all INT data.                */
    
    nlist->user_struct_len = 0;
    nlist->total_rows = 0;
    nlist->nallocated_tables = 0;
    nlist->total_tables = 0;
    nlist->current_table = 0;
    nlist->current_row = 0;
    nlist->method_list = (NL_METHOD_HEADER *)0;
    
    /*      reset all pointer to NULL_TABLE.        */
    
    nlist->current_table_ptr = NULL_TABLE;
    
    return SUCCESS;
    }
/* END:     */
