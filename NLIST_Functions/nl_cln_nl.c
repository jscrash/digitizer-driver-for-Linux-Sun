/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_NL.C*/
/* *5    25-OCT-1990 12:21:16 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:56:09 VINCE "(SPR 5644) Header Standardization"*/
/* *3     1-JUN-1990 12:57:11 GILLESPIE "(SPR 1) Use nlz_copy_structure"*/
/* *2    12-MAR-1990 19:06:12 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:35 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_NL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_NLIST.C*/
/* *2    24-SEP-1989 22:27:34 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:17:54 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_NLIST.C*/
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

#ifndef ESI_NL_INT_H

#include "esi_nl_int.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_MATH_H

#include "esi_math.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To clone a nlist accord to a given nlist, the result nlist will
    have the same data type, data column, data size,... etc. as the 
    given nlist, the result nlist will be same as being initialized.

Prototype:
    publicdef INT nl_clone_nlist(NLIST_HEADER from_list, NLIST_HEADER *to_list);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) from_list   -(NLIST_HEADER ) The given sample nlist.
    (O) to_list     -(NLIST_HEADER *) The resultant nlist.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_clone_nlist (NLIST_HEADER from_list, NLIST_HEADER *to_list)
#else
publicdef INT nl_clone_nlist (from_list, to_list)
NLIST_HEADER from_list, *to_list;
#endif
    {
    UINT i;
    UINT nCol;
    size_t size;
    NLIST_HEADER nlist;
    COLUMN_HEADER *pinfo;
    NL_METHOD_HEADER *pmethod;
    
    INIT_TEST (from_list);
    
    nCol = from_list->nCol;
    size = sizeof(NLIST_STRUCT) + sizeof(COLUMN_HEADER) * (from_list->nCol - 1);
    nlist = *to_list = (NLIST_HEADER) tc_alloc (size);
    if (nlist == (NLIST_HEADER)0)
        {
        return NL_OVERFLOW;
        }
    hoblockmove ((VOIDPTR)from_list, (VOIDPTR)nlist, size);
    
    /*
       * Reset the BYTE string field to be NULL.
    */
    nlist->user_struct = (VOIDPTR)NULL;
    nlist->user_struct_len = 0;
    
    /*
       * copy the CHAR string field to be NULL.
    */
    nlist->name = (CHAR *)NULL;
    nlist->data_filename = (CHAR *)NULL;
    nlist->next_nlist = (CHAR *)NULL;
    nlist->prev_nlist = (CHAR *)NULL;
    nlist->current_table_ptr = nlist->deleted_tables = NULL_TABLE;
    nlist->total_rows = nlist->total_tables = nlist->nallocated_tables =
        nlist->current_table = nlist->current_row = 0;
    nlist->table_elements = (TABLE_ELEMENT *)0;
    
    /* Copy the n-List methods */
    nlist->method_list = (NL_METHOD_HEADER *)0;
    for (pmethod = from_list->method_list; pmethod != (NL_METHOD_HEADER *)0;
         pmethod = pmethod->next)
        {
        nl_set_nlist_method (nlist, pmethod->method_id, pmethod->function_name,
                             pmethod->function);
        }
    /*  
        * reset the maximum and minmum value of each column.
    */
    for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
        {
        pinfo->min = HUGE_VAL;
        pinfo->max = -HUGE_VAL;
        pinfo->sum_x = pinfo->sum_x_squared = 0.0;
        }
    return SUCCESS;
    }
/* END:     */
