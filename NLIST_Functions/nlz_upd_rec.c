/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_UPD_REC.C*/
/* *4    25-OCT-1990 12:20:22 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 14:55:24 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:05:32 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:21 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_UPD_REC.C*/
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

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

/* Function Description -----------------------------------------------------
Description:
    This function writes a contiguous series of bytes into the next available 
    position in the file.

Prototype:
    publicdef INT nlz_update_record (FILE_UNIT funit, UINT exist_offset, UINT length,
        BYTE *record, NLIST_CONTROL_RECORD *control_record);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    exist_offset    -(UINT)
    length          -(UINT)
    record          -(BYTE *)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OFFSET_ERROR
    NL_OFFSET
    Function also returns the value of written_offset.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_update_record (FILE_UNIT funit, UINT exist_offset, UINT length,
                                 BYTE *record, NLIST_CONTROL_RECORD *control_record)
#else
publicdef INT nlz_update_record (funit, exist_offset, length, record, control_record)
FILE_UNIT funit;
UINT exist_offset;
UINT length;
BYTE *record;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
    UINT written_offset;
    UINT local_length;
    register size_t written_bytes;
    size_t bytes_to_move;
    size_t new_allocated_bytes;
    size_t totalExistBytes;
    NLIST_LOGICAL_RECORD *logicalRec, oldLogicalRec, firstLogicalRec;
    BOOL done = FALSE, first = TRUE;
    register BYTE *ptr;
    BYTE *writtenRec, *newRec;
    
    /* set the pointer to the available offset.    */
    written_offset = exist_offset;
    if (written_offset & 1)
        {
        return NL_FILE_OFFSET_ERROR;
        }
    /* check the total length of exist record.   */
    nlh_seek (funit, (INT)exist_offset, (INT)SEEK_SET);
    nlh_read (funit, (VOIDPTR)&firstLogicalRec, sizeof firstLogicalRec);
    
    /* if the length of new record is 0, only keet the first record others
        linked to deleted record list.        */
    
    if (length EQUALS 0)
        {
        if (firstLogicalRec.next_offset > 0)
            {
            firstLogicalRec.total_bytes = firstLogicalRec.bytes;
            nlz_link_deleted_record (funit, firstLogicalRec.next_offset,
                                     control_record);
            }
        firstLogicalRec.bytes = firstLogicalRec.next_offset = 0;
        nlh_seek (funit, (INT)exist_offset, (INT)SEEK_SET);
        nlh_write (funit, (VOIDPTR)&firstLogicalRec, sizeof firstLogicalRec);
        return SUCCESS;
        }
    local_length = length + 2 + (length & 1);
    
    /* we will write the first record last, because we need to update some
       information in the logical record.
     */
    writtenRec = (BYTE *)TALLOC ((size_t)local_length);
    if (writtenRec == (BYTE *)0)
        {
        return NL_OVERFLOW;
        }
    hoblockmove ((VOIDPTR)record, (VOIDPTR)writtenRec, (size_t)length);
    ptr = writtenRec + sizeof firstLogicalRec;
    written_bytes = sizeof firstLogicalRec;
    hoblockmove ((VOIDPTR) & firstLogicalRec, (VOIDPTR) & oldLogicalRec,
                 (size_t)sizeof oldLogicalRec);
    totalExistBytes = 0;
    while (!done)
        {
        if (written_bytes < local_length)
            {
            nlh_seek (funit, (INT)oldLogicalRec.offset, (INT)SEEK_SET);
            if (oldLogicalRec.next_offset EQUALS 0)
                {
                bytes_to_move = (oldLogicalRec.total_bytes >=
                                 (local_length - written_bytes)) ?
                                (local_length - written_bytes) :
                                oldLogicalRec.total_bytes;
                totalExistBytes += oldLogicalRec.total_bytes;
                }
            else
                {
                bytes_to_move = (oldLogicalRec.bytes >=
                                 (local_length - written_bytes)) ?
                                (local_length - written_bytes) : oldLogicalRec.bytes;
                totalExistBytes += oldLogicalRec.bytes;
                }
            oldLogicalRec.bytes = bytes_to_move;
            written_bytes += bytes_to_move;
            
            /* if there are some more data to move */
            
            if (written_bytes < local_length)
                {
                nlh_write (funit, (VOIDPTR)&oldLogicalRec, sizeof oldLogicalRec);
                nlh_seek (funit, oldLogicalRec.offset + sizeof oldLogicalRec,
                          (INT)SEEK_SET);
                nlh_write (funit, (VOIDPTR)ptr, bytes_to_move);
                ptr += bytes_to_move;
                /* and there are some more old space,
                   read the next logical record.    */
                if (oldLogicalRec.next_offset > 0)
                    {
                    nlh_seek (funit, oldLogicalRec.next_offset, (INT)SEEK_SET);
                    nlh_read (funit, (VOIDPTR)&oldLogicalRec, sizeof oldLogicalRec);
                    }
                else
                    {
                    new_allocated_bytes = local_length - written_bytes +
                                          sizeof oldLogicalRec;
                    
                    newRec = (BYTE *)tc_zalloc ((size_t)new_allocated_bytes);
                    if (newRec == (BYTE *)0)
                        {
                        return NL_OVERFLOW;
                        }
                    logicalRec = (NLIST_LOGICAL_RECORD *)newRec;
                    logicalRec->bytes = logicalRec->total_bytes = local_length -
                                        written_bytes;
                    logicalRec->next_offset = 0;
                    hoblockmove ((VOIDPTR)ptr,
                                 (VOIDPTR)(newRec + sizeof oldLogicalRec),
                                 (size_t)(local_length - written_bytes));
                    totalExistBytes += (local_length - written_bytes);
                    oldLogicalRec.next_offset = nlz_put_record (funit,
                                                                (UINT)exist_offset,
                                                                (size_t)new_allocated_bytes
                                                                , (VOIDPTR)newRec,
                                                                control_record);
                    if (first)
                        {
                        firstLogicalRec.next_offset = oldLogicalRec.next_offset;
                        firstLogicalRec.bytes = oldLogicalRec.bytes;
                        }
                    else
                        {
                        /* set the previous last record link
                           to the new record.                */
                        
                        nlh_seek (funit, (INT)oldLogicalRec.offset, (INT)SEEK_SET);
                        nlh_write (funit, (VOIDPTR)&oldLogicalRec, sizeof oldLogicalRec);
                        }
                    /* else allocate some more new space.*/
                    written_bytes += (new_allocated_bytes - sizeof oldLogicalRec);
                    done = TRUE;
                    tc_free ((VOIDPTR)newRec);
                    }
                }
            else
                {
                /* if there is no more data.         */
                if (oldLogicalRec.next_offset > 0)
                    {
                    nlz_link_deleted_record (funit, oldLogicalRec.next_offset,
                                             control_record);
                    oldLogicalRec.next_offset = 0;
                    }
                if (first)
                    {
                    firstLogicalRec.next_offset = 0;
                    firstLogicalRec.bytes = oldLogicalRec.bytes;
                    }
                else
                    {
                    nlh_seek (funit, (INT)oldLogicalRec.offset, (INT)SEEK_SET);
                    nlh_write (funit, (VOIDPTR)&oldLogicalRec, sizeof oldLogicalRec);
                    }
                nlh_seek (funit, oldLogicalRec.offset + sizeof oldLogicalRec,
                          (INT)SEEK_SET);
                nlh_write (funit, (VOIDPTR)ptr, bytes_to_move);
                ptr += bytes_to_move;
                }
            first = FALSE;
            }
        else
            {
            done = TRUE;
            }
        }
    /* at last, write out the updated
       logical record of the first one.*/
    nlh_seek (funit, (INT)firstLogicalRec.offset, (INT)SEEK_SET);
    firstLogicalRec.total_bytes = totalExistBytes;
    nlh_write (funit, (VOIDPTR)&firstLogicalRec, sizeof firstLogicalRec);
#if NEED_TO_FREE
    tc_free ((VOIDPTR)writtenRec);
#endif
    return (INT)written_offset;
    }
/* END:     */
