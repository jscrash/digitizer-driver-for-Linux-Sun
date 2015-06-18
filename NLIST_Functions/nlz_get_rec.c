/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_REC.C*/
/* *3    25-OCT-1990 12:18:36 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:52:53 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:55:43 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_REC.C*/
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
#include "esi_ho_files.h"
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    To retrieve a logical record by offset.

Prototype:
    publicdef INT nlz_get_record (FILE_UNIT funit, UINT first_offset, VOIDPTR *record, 
        UINT *length, BOOL only_one);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    first_offset    -(UINT)
    record          -(VOIDPTR *)
    length          -(UINT *)
    only_one        -(BOOL)

Return Value/Status:
    The function will return the next record offset; if it is the last record
    of all data, it will return 0.
    Error conditions are signalled by negative 'offsets'

Scope:
    PUBLIC
    
Limitations/Assumptions:
    The CALLER is responsible for releasing the memory allocated by this 
    routine.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_record (FILE_UNIT funit, UINT first_offset, VOIDPTR *record,
                              UINT *length, BOOL only_one) 
#else
publicdef INT nlz_get_record (funit, first_offset, record, length, only_one)
FILE_UNIT funit;
UINT first_offset;
VOIDPTR *record;
UINT *length;
BOOL only_one;
#endif
    {
    NLIST_LOGICAL_RECORD logical_record;
    INT offset;
    INT status;
    BYTE *ptr;
    
    /*
       * read the first record of File Dirctory Record or Logical Record
       * or Header record.
    */
    nlh_seek (funit, (INT)first_offset, (INT)SEEK_SET);
    status = nlh_read (funit, (VOIDPTR)&logical_record, sizeof(NLIST_LOGICAL_RECORD));
    if (status < 0)
        {
        return NL_FILE_READ_ERROR;
        }
    /* allocate the return buffer.   */
    if (first_offset IS_NOT_EQUAL_TO logical_record.offset)
        {
        return NL_FILE_OFFSET_ERROR;
        }
    *length = 0;
    *record = (VOIDPTR)(ptr = (BYTE *)tc_alloc ((size_t)logical_record.total_bytes));
    if (ptr == (BYTE *)0)
        {
        return NL_OVERFLOW;         /* tried to get too much memory from system */
        }
    offset = (INT)first_offset;
    
    /* to read whole data into the buffer.   */
    FOREVER
        {
        nlh_seek (funit, logical_record.offset + 20, (INT)SEEK_SET);
        nlh_read (funit, (VOIDPTR)ptr, logical_record.bytes);
        ptr += logical_record.bytes;
        *length += logical_record.bytes;
        
        if (only_one OR logical_record.next_offset EQUALS 0)
            {
            break;
            }
        else
            {
            offset = (INT)logical_record.next_offset;
            nlh_seek (funit, offset, (INT)SEEK_SET);
            status = nlh_read (funit, (VOIDPTR)&logical_record, sizeof(NLIST_LOGICAL_RECORD));
            if (status < 0)
                {
                return status;
                }
            if (offset IS_NOT_EQUAL_TO logical_record.offset)
                {
                return NL_FILE_OFFSET_ERROR;
                }
            if (logical_record.bytes EQUALS 0)
                {
                break;
                }
            }
        }
    return (INT)logical_record.next_offset;
    }
/* END:     */
