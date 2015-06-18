/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_LOAD_DAT.C*/
/* *5    25-OCT-1990 12:19:08 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *4    25-JUL-1990 14:53:22 VINCE "(SPR 5644) Header Standardization"*/
/* *3     9-APR-1990 10:31:58 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *2    12-MAR-1990 19:04:37 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:56:21 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_LOAD_DAT.C*/
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
#include "esi_tc.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    To load data  for the current table.  

Prototype:
    publicdef INT nlz_load_data (NLIST_HEADER nlist);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_IO_ERROR6
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_load_data (NLIST_HEADER nlist) 
#else
publicdef INT nlz_load_data (nlist)
NLIST_HEADER nlist;
#endif
    {
    TABLE_HEADER *pTable;
    DATA_HEADER *pdata;
    COLUMN_HEADER *pinfo;
    register UINT i;
    UINT dataArraySize;
    FILE_UNIT funit;
    UINT nCol = nlist->nCol;
    size_t movedBytes;
    BYTE *dataArray, *ptr;
    INT status;
    
    pTable = nlist->current_table_ptr;
    if (pTable->nrows EQUALS 0)
        {
        return SUCCESS;
        }
    /*
       * to read the data array from file.
    */
    
    if ((status = nlz_get_read_funit (nlist->data_filename, &funit)) < 0)
        {
        return status;
        }
    if (nlz_get_record (funit, (UINT)pTable->record_offset, (VOIDPTR *) & dataArray,
                        &dataArraySize, (BOOL)FALSE) < 0)
        {
        return NL_IO_ERROR6;
        }
    /*
       * restore the data to current table.
    */
    for (i = 0, pdata = pTable->data_ptr, pinfo = nlist->info_ptr, ptr = dataArray;
         i < nCol; i++, pdata++, pinfo++)
        {
        if (NOT pdata->inc_value)
            {
            if (pdata->data IS_NOT_EQUAL_TO (BYTE *)NULL)
                {
                tc_free ((VOIDPTR)pdata->data);
                }
            movedBytes = pinfo->data_size * pTable->nrows;
            pdata->data = (BYTE *)tc_alloc ((size_t)movedBytes);
            if (pdata->data == (BYTE *)0)
                {
                return NL_OVERFLOW;
                }
            hoblockmove ((VOIDPTR)ptr, (VOIDPTR)pdata->data, (size_t)movedBytes);
            ptr += movedBytes;
            }
        }
    tc_free ((VOIDPTR)dataArray);   /* added JGG 9-Jan-88 */
    pTable->nallocated = pTable->nrows;
    pTable->data_loaded = NL_DATA_LOADED;
    
    return SUCCESS;
    }
/* END:     */
