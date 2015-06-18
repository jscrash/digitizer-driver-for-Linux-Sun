/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_LNK_DREC.C*/
/* *3    25-OCT-1990 12:19:04 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *2    25-JUL-1990 14:53:16 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 10:56:16 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_LNK_DREC.C*/
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
#include "esi_ho_files.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_link_deleted_record (FILE_UNIT funit, UINT deleted_offset, 
        NLIST_CONTROL_RECORD *control_record);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    deleted_offset  -(UINT)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OFFSET_ERROR
    NL_FILE_READ_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_link_deleted_record (FILE_UNIT funit, UINT deleted_offset,
                                       NLIST_CONTROL_RECORD *control_record) 
#else
publicdef INT nlz_link_deleted_record (funit, deleted_offset, control_record)
FILE_UNIT funit;
UINT deleted_offset;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
    NLIST_LOGICAL_RECORD logical_record;
    INT status;
    UINT offset;
    
    nlh_seek (funit, (INT)deleted_offset, (INT)SEEK_SET);
    status = nlh_read (funit, (VOIDPTR)&logical_record, sizeof(NLIST_LOGICAL_RECORD));
    if (status < 0)
        {
        return NL_FILE_READ_ERROR;
        }
    /* allocate the return buffer.   */
    
    if (deleted_offset IS_NOT_EQUAL_TO logical_record.offset)
        {
        return NL_FILE_OFFSET_ERROR;
        }
    offset = logical_record.next_offset;
    
    while (offset IS_NOT_EQUAL_TO 0)
        {
        
        nlh_seek (funit, (INT)offset, (INT)SEEK_SET);
        status = nlh_read (funit, (VOIDPTR)&logical_record, sizeof(NLIST_LOGICAL_RECORD));
        if (status < 0)
            {
            return NL_FILE_READ_ERROR;
            }
        /* allocate the return buffer.   */
        
        if (offset IS_NOT_EQUAL_TO logical_record.offset)
            {
            return NL_FILE_OFFSET_ERROR;
            }
        offset = logical_record.next_offset;
        }
    /* link the requested record to the */
    /* top of list.                     */
    
    logical_record.next_offset = control_record->deleted_record_offset;
    control_record->deleted_record_offset = deleted_offset;
    
    nlh_seek (funit, (INT)logical_record.offset, (INT)SEEK_SET);
    status = nlh_write (funit, (VOIDPTR)&logical_record, sizeof(NLIST_LOGICAL_RECORD));
    
    return status;
    
    }
/* END:     */
