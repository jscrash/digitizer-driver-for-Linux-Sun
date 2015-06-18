/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_PUT_REC.C*/
/* *4    25-OCT-1990 12:19:47 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:54:12 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:05:10 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:56:37 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_PUT_REC.C*/
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
    To write a contiguous series of bytes into the next available position in
    the file.

Prototype:
    publicdef INT nlz_put_record (FILE_UNIT funit, UINT first_offset, size_t length, 
        VOIDPTR record, NLIST_CONTROL_RECORD *control_record);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    first__offset   -(UINT)
    length          -(size_t)
    record          -(VOIDPTR)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    NL_FILE_OFFSET_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_put_record (FILE_UNIT funit, UINT first_offset, size_t length,
                              VOIDPTR record, NLIST_CONTROL_RECORD *control_record) 
#else
publicdef INT nlz_put_record (funit, first_offset, length, record, control_record)
FILE_UNIT funit;
UINT first_offset;
size_t length;
VOIDPTR record;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
    register NLIST_LOGICAL_RECORD *plogicalRec;
    register INT written_offset;
    
    /* set the pointer to the available offset.    */
    written_offset = (INT)control_record->next_write_offset;
    if (written_offset & 1)
        {
        return NL_FILE_OFFSET_ERROR;
        }
    /*
       * update the logical record part of output buffer.
    */
    plogicalRec = (NLIST_LOGICAL_RECORD *)record;
    plogicalRec->first_offset = (first_offset EQUALS 0) ? written_offset : first_offset;
    plogicalRec->offset = (UINT)written_offset;
    nlh_seek (funit, written_offset, (INT)SEEK_SET);
    nlh_write (funit, record, length);
    
    control_record->next_write_offset += (length + (length & 1));
    
    return written_offset;
    }
/* END:     */
