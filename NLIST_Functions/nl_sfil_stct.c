/* DEC/CMS REPLACEMENT HISTORY, Element NL_SFIL_STCT.C*/
/* *4    25-OCT-1990 12:26:40 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *3    25-JUL-1990 15:05:27 VINCE "(SPR 5644) Header Standardization"*/
/* *2    12-MAR-1990 19:09:33 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 11:01:18 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_SFIL_STCT.C*/
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

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef ESI_HO_FILES_H
#include "esi_ho_files.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

/* Function Description -----------------------------------------------------
Description:
    To set the user structure for a n_list file.

Prototype:
    publicdef INT nl_set_file_user_struct (FILENAME in_filename, VOIDPTR user_struct_ptr, 
        size_t nbyte);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    user_struct_ptr -(VOIDPTR)
    nbyte           -(size_t)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NLIST_NOT_FOUND
    NL_FILE_NOT_FOUND
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_set_file_user_struct (FILENAME in_filename, VOIDPTR user_struct_ptr,
                                       size_t nbyte)
#else
publicdef INT nl_set_file_user_struct (in_filename, user_struct_ptr, nbyte)
FILENAME in_filename;
VOIDPTR user_struct_ptr;
size_t nbyte;
#endif
    {
    INT status = SUCCESS;
    INT exists;
    FILENAME filename;
    FILE_UNIT funit;
    NLIST_CONTROL_RECORD *control_record;
    NLIST_LOGICAL_RECORD *logical_record_ptr;
    BYTE *writtenRec;
    
    /* error checking   */
    /* translate the file name first to
    get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return NL_FILE_NOT_FOUND;
        }
    exists = nlz_chk_nlist_exist (filename, "DUMMY", &funit, (BOOL)TRUE, WRITE);
    if (exists IS_NOT_EQUAL_TO NL_NLIST_NOT_FOUND)  /* file does NOT exist - make a new one */
        {
        return NL_NLIST_NOT_FOUND;
        }
    /* get the control record.     */
    
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    /* if the requested bytes is greater
        than 0, write it out.    */
    if (nbyte > 0)
        {
        
        writtenRec = (BYTE *)tc_zalloc (nbyte + sizeof(NLIST_LOGICAL_RECORD));
        if (writtenRec == (BYTE *)0)
            {
            return NL_OVERFLOW;
            }
        logical_record_ptr = (NLIST_LOGICAL_RECORD *)writtenRec;
        logical_record_ptr->total_bytes = logical_record_ptr->bytes = nbyte;
        logical_record_ptr->next_offset = 0;
        hoblockmove ((VOIDPTR)user_struct_ptr,
                     (VOIDPTR)(writtenRec + sizeof(NLIST_LOGICAL_RECORD)),
                     (size_t)nbyte);
        if (control_record->file_user_struct_offset > 0)
            {
            logical_record_ptr->offset = control_record->file_user_struct_offset;
            control_record->file_user_struct_offset =
                                                      nlz_update_record (
                                                           funit,
                                                               (UINT)control_record->
                                                               file_user_struct_offset,
                                                                         (UINT)
                                                          (sizeof(
                                                               NLIST_LOGICAL_RECORD) +
                                                           nbyte),
                                                      (BYTE 
                                                       *)writtenRec, control_record);
            }
        else
            {
            control_record->file_user_struct_offset =
                                                      nlz_put_record (
                                                                 funit,
                                                                (UINT)0,
                                                      (UINT)sizeof(
                                                                 NLIST_LOGICAL_RECORD) 
                                                                      + 
                                                                     nbyte,
                                                           writtenRec, control_record);
            }
        tc_free ((VOIDPTR)writtenRec);
        }
    else
        {
        if (control_record->file_user_struct_offset > 0)
            {
            nlz_link_deleted_record (funit, control_record->file_user_struct_offset,
                                     control_record);
            }
        control_record->file_user_struct_offset = 0;
        status = NL_OVERFLOW;
        }
    return status;
    }
/* END:     */
