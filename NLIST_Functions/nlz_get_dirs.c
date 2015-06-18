/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_DIRS.C */
/* *5    25-OCT-1990 12:18:28 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies" */
/* *4    25-JUL-1990 14:52:40 VINCE "(SPR 5644) Header Standardization" */
/* *3    12-MAR-1990 19:04:04 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs" */
/* *2     5-MAR-1990 10:45:04 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix" */
/* *1    24-SEP-1989 22:33:48 GILLESPIE "New n-List V4.0" */
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_GET_DIRS.C */
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
#include "esi_tc.h"

/* Function Description -----------------------------------------------------
Description:
    To retrieve a logical directory block by offset.

Prototype:
    publicdef INT nlz_get_directories (FILE_UNIT funit, UINT first_offset, 
        DIRECTORY_ENTRY **dirs, UINT *total_dirs);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    first_offset    -(UINT)
    dirs            -(DIRECTORY_ENTRY **)
    total_dirs      -(UINT *)

Return Value/Status:
    NL_FILE_READ_ERROR
    NL_FILE_OFFSET_ERROR
    NL_OVERFLOW
    The function will return the next record offset, if it is the last record
    of all data, it will return 0.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_directories (FILE_UNIT funit, UINT first_offset,
                                   DIRECTORY_ENTRY **dirs, UINT *total_dirs) 
#else
publicdef INT nlz_get_directories (funit, first_offset, dirs, total_dirs)
FILE_UNIT funit;
UINT first_offset;
DIRECTORY_ENTRY **dirs;
UINT *total_dirs;
#endif
    {
    NLIST_LOGICAL_RECORD logical_record;
    INT status;
    
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
    *total_dirs = logical_record.bytes;
    *dirs = (DIRECTORY_ENTRY *)tc_alloc ((size_t)(sizeof(DIRECTORY_ENTRY) *
                                                  (*total_dirs + 1)));
    if (dirs == (DIRECTORY_ENTRY **)0)
        {
        return NL_OVERFLOW;
        }
    /* if there is no entry, returns.*/
    if (*total_dirs > 0)
        {
        nlh_seek (funit, (INT)logical_record.first_offset, (INT)SEEK_SET);
        nlh_read (funit, (VOIDPTR)*dirs, *total_dirs * sizeof(DIRECTORY_ENTRY));
        }
    return (INT)logical_record.next_offset;
    }
/* END:     */
