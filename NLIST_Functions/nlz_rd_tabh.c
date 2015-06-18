/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_RD_TABH.C*/
/* *4    25-OCT-1990 12:19:51 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:54:23 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:05:15 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:56:53 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_RD_TABH.C*/
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
#include "esi_nl_io.h"

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    To read the table header by giving a specific table header offset. 

Prototype:
    publicdef TABLE_HEADER *nlz_read_table_header (NLIST_HEADER nlist, 
        TABLE_ELEMENT *table_element);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    table_element   -(TABLE_ELEMENT *)

Return Value/Status:
    NULL_TABLE
    Function returns the value of table_element->ptable.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef TABLE_HEADER *nlz_read_table_header (NLIST_HEADER nlist,
                                               TABLE_ELEMENT *table_element)
#else
publicdef TABLE_HEADER *nlz_read_table_header (nlist, table_element)
NLIST_HEADER nlist;
TABLE_ELEMENT *table_element;
#endif
    {
    INT headerOffset;
	size_t headerArraySize;
	INT status;
    register UINT i;
    VOIDPTR headerArray;
    FILE_UNIT funit;
    COLUMN_HEADER *pinfo;
    DATA_HEADER *pdata;
    
    /* If the table exists, return the table header. */
    
    if (table_element->ptable != NULL_TABLE)
        {
        return table_element->ptable;
        }
    /* check the existence of nlist      */
    headerOffset = nlz_chk_nlist_exist (nlist->data_filename, nlist->name, &funit,
                                        (BOOL)FALSE, SHARE);
    if (headerOffset < 0)
        {
        return NULL_TABLE;
        }
    /*
       * Allocate space for specified table.
    */
    table_element->ptable = (TABLE_HEADER *)tc_zalloc ((size_t)(sizeof(TABLE_HEADER) +
                                                                sizeof(DATA_HEADER) *
                                                                (nlist->nCol - 1)));
    if (table_element->ptable == (TABLE_HEADER *)0)
        {
        return NULL_TABLE;
        }
    table_element->ptable->n_value_cols = nlist->n_value_cols;
    
    pinfo = nlist->info_ptr;
    pdata = table_element->ptable->data_ptr;
    for (i = 0; i < nlist->nCol; i++, pinfo++, pdata++)
        pdata->inc_value = pinfo->inc_value;
    
    /*
       * read the header of specified table. 
    */
    status = nlz_get_record (funit, (UINT)table_element->header_offset,
                             &headerArray, (UINT *) & headerArraySize,
                             (BOOL)FALSE);
    
    if (status < 0)
        {
        return NULL_TABLE;
        }
    /*
       *  to reconstruct the table.
    */
    status = nlz_construct_Table (nlist, table_element, headerArray,
                                  headerArraySize);
    tc_free (headerArray);
    if (status < 0)
        {
        return NULL_TABLE;
        }
    return table_element->ptable;
    }
/* END:     */
