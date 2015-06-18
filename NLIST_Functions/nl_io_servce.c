/* DEC/CMS REPLACEMENT HISTORY, Element NL_IO_SERVCE.C*/
/* *16   25-OCT-1990 12:23:24 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *15   20-AUG-1990 17:37:56 KEE "(SPR -1) fixing compile error at sun"*/
/* *14   25-JUL-1990 15:00:07 VINCE "(SPR 5644) Header Standardization"*/
/* *13    9-APR-1990 10:33:12 GILLESPIE "(SPR 1) Remove superflurous checks"*/
/* *12   22-MAR-1990 01:35:19 GILLESPIE "(SPR 1) Correct problems with stats on delete"*/
/* *11   18-MAR-1990 01:51:58 GILLESPIE "(SPR 1) SMALL TWEEKS, APPLE CHEEKS"*/
/* *10   17-MAR-1990 17:14:34 GILLESPIE "(SPR 1) Check return from nlz_set_table; move reading of n-list prior to writing"*/
/* *9    12-MAR-1990 19:07:19 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *8     5-MAR-1990 10:46:55 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *7    27-OCT-1989 12:42:55 GILLESPIE "(SPR 1) Change logical record to NOT include two extra bytes on even writes"*/
/* *6    27-SEP-1989 16:15:39 GORDON "(SPR 5005) Fix call to nlz_put_record in nlz_write_table"*/
/* *5    24-SEP-1989 22:40:16 GILLESPIE "(SPR 200) New n-List Version 4"*/
/* *4    18-SEP-1989 17:46:42 GILLESPIE "(SPR 100) Gulf mods"*/
/* *3    31-JUL-1989 13:34:57 GORDON "(SPR 101) fix ROUND_UP & code 126 in nlz_construct_nlist "*/
/* *2    25-JUL-1989 16:48:41 GORDON "(SPR 100) Lots o' changes for SUN4 compatibility"*/
/* *1    19-JUN-1989 13:19:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_IO_SERVCE.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_HO_H

#include "esi_ho.h"

#endif

#ifndef ESI_TC_H

#include "esi_tc.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif
static UINT nl_max_entry_in_block = NL_DEFAULT_ENTRY_IN_BLOCK;

#if USE_PROTOTYPES
static INT nlz_delete_exist_nlist (FILENAME filename, NLIST_ID name);
static INT nlz_delete_recover_nlist (FILE_UNIT funit, NLIST_ID name);
static INT nlz_read_nlist (NLIST_HEADER *nlist, FILENAME filename, NLIST_ID name,
                               NL_SHARE_MODE share_mode);
static INT nlz_stamp_version (NLIST_CONTROL_RECORD *pcontrol, CHAR *version);
static INT nlz_update_dir_and_ctrl_v3 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset, size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD *control_record);
static INT nlz_update_dir_and_ctrl_v4 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset, size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD *control_record);
static INT nlz_update_direct_and_control (FILE_UNIT funit, FILENAME filename,
                                              NLIST_ID name, NLIST_HEADER nlist,
                                              UINT header_offset, size_t totalBytes,
                                              BOOL new_dir, UINT exist_offset,
                                              NLIST_CONTROL_RECORD *control_record);
static INT nlz_write_header (FILE_UNIT funit, NLIST_HEADER nlist,
                                 UINT *header_offset, size_t *totalBytesInEntry,
                                 NLIST_CONTROL_RECORD *control_record,
                                 UINT exist_offset);
static INT nlz_write_nlist (NLIST_HEADER nlist, FILENAME filename, NLIST_ID name,
                                NL_SHARE_MODE share_mode);
static INT nlz_write_table (FILE_UNIT funit, NLIST_HEADER nlist,
                                TABLE_HEADER *pTable,
                                NLIST_CONTROL_RECORD *control_record, BOOL to_write);
static INT nlz_write_table_header (FILE_UNIT funit, NLIST_HEADER nlist,
                                       UINT *header_offset, size_t *totalBytesInEntry,
                                       NLIST_CONTROL_RECORD *control_record,
                                       UINT exist_offset);
static VOID nlz_encode_data (BYTE **ptr, INT dataCode, size_t dataSize,
                                 VOIDPTR data);
#else
static INT nlz_delete_exist_nlist ();
static INT nlz_delete_recover_nlist ();
static INT nlz_read_nlist ();
static INT nlz_stamp_version ();
static INT nlz_update_dir_and_ctrl_v3 ();
static INT nlz_update_dir_and_ctrl_v4 ();
static INT nlz_update_direct_and_control ();
static INT nlz_write_nlist ();
static INT nlz_write_header ();
static INT nlz_write_table ();
static INT nlz_write_table_header ();
static VOID nlz_encode_data ();

#endif

/* File Description ---------------------------------------------------------
Overview:
    These are the i/o routines of nlist.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT nl_write_nlist_exclusive (NLIST_HEADER nlist, FILENAME in_filename, 
        NLIST_ID name);

    publicdef INT nl_write_nlist (NLIST_HEADER nlist, FILENAME in_filename, NLIST_ID name);

    publicdef INT nl_update_nlist (NLIST_HEADER nlist);

    publicdef INT nl_read_nlist_exclusive (NLIST_HEADER *nlist, FILENAME filename, 
        NLIST_ID name);

    publicdef INT nl_read_nlist (NLIST_HEADER *nlist, FILENAME filename, NLIST_ID name);

    publicdef INT nl_delete_recover (FILENAME in_filename, NLIST_ID name);

    publicdef INT nl_delete_nlist (FILENAME in_filename, NLIST_ID name);

    publicdef INT nlz_set_block_entries(INT nblocks);

Private_Functions:
    static INT nlz_delete_exist_nlist (FILENAME filename, NLIST_ID name);

    static INT nlz_delete_recover_nlist (FILE_UNIT funit, NLIST_ID name);

    static INT nlz_read_nlist (NLIST_HEADER *nlist, FILENAME filename,
                                NLIST_ID name, NL_SHARE_MODE share_mode);
    
    static INT nlz_stamp_version (NLIST_CONTROL_RECORD *pcontrol,
                                  CHAR *version);
    
    static INT nlz_update_dir_and_ctrl_v3 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset,
                                           size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD 
                                           *control_record);
    
    static INT nlz_update_dir_and_ctrl_v4 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset,
                                           size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD 
                                           *control_record);
    
    static INT nlz_update_direct_and_control (FILE_UNIT funit,
                                              FILENAME filename,
                                              NLIST_ID name,
                                              NLIST_HEADER nlist,
                                              UINT header_offset,
                                              size_t totalBytes, BOOL new_dir,
                                              UINT exist_offset,
                                              NLIST_CONTROL_RECORD 
                                              *control_record);

    static INT nlz_write_header (FILE_UNIT funit,
                                 NLIST_HEADER nlist, UINT *header_offset,
                                 size_t *totalBytesInEntry,
                                 NLIST_CONTROL_RECORD *control_record,
                                 UINT exist_offset);

    static INT nlz_write_nlist (NLIST_HEADER nlist, FILENAME filename,
                                NLIST_ID name, NL_SHARE_MODE share_mode);

    static INT nlz_write_table(FILE_UNIT funit,
                                NLIST_HEADER nlist, TABLE_HEADER *pTable,
                                NLIST_CONTROL_RECORD *control_record,
                                BOOL to_write);
    
    static INT nlz_write_table_header  (FILE_UNIT funit, NLIST_HEADER nlist,
                                UINT *header_offset, size_t *totalBytesInEntry,
                                NLIST_CONTROL_RECORD *control_record,
                                UINT exist_offset);

    static VOID nlz_encode_data (BYTE **ptr, INT dataCode, size_t dataSize,
                                 VOIDPTR data);

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    To delete an existing nlist, if it exists, return the header offset of
    entry, otherwise return negative status code.

Prototype:
    static INT nlz_delete_exist_nlist (FILENAME filename, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NLIST_NOT_FOUND
    NL_LICENSE_EXPIRED
    NL_FILE_OPEN_ERROR
    Function returns the header offset of entry.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_delete_exist_nlist (FILENAME filename, NLIST_ID name)
#else
static INT nlz_delete_exist_nlist (filename, name)
FILENAME filename;
NLIST_ID name;
#endif
    {
    NLIST_CONTROL_RECORD *control_record;
    BYTE *ptr, *record;
    FILE_UNIT funit;
    INT temp;
    INT status;
    UINT offset, next_offset, length;
    register INT i;
    UINT mark_offset;
    INT data_type;
    size_t data_size;
    INT exists;
    size_t dataCodeSize = 2 * sizeof(INT);
    INT returnOffset = 0;
    INT eraseMarkCode = 888;
    BOOL end = FALSE;
    BOOL rightName = FALSE;
    BOOL checkName = FALSE;
    
    /* If it is the first nlist in this file, we need to initialize the file structure,
     * includes Control Record and Directory.
    
      EXISTS = NL_NLIST_NOT_FOUND file exists, n-List does not
         > 0 ->               file exists, n-List exists
         other ->             file open error - make a new one */
    
    exists = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)TRUE, SHARE);
    if (exists > 0)                 /* n-List exists in file */
        {
        offset = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)TRUE, WRITE);
        }
    else if (exists IS_NOT_EQUAL_TO NL_NLIST_NOT_FOUND) /* file does NOT exist - make a new one */
        {
        return NL_FILE_OPEN_ERROR;
        }
    else if (exists EQUALS NL_NLIST_NOT_FOUND)
        {
        return NL_NLIST_NOT_FOUND;
        }
    else if (exists EQUALS NL_LICENSE_EXPIRED)
        {
        return NL_LICENSE_EXPIRED;
        }
    if ((INT)offset < 0)
        {
        return NL_NLIST_NOT_FOUND;
        }
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    /* read the file directory.   */
    
    do
        {
        next_offset = nlz_get_record (funit, (UINT)offset, (VOIDPTR *) & record,
                                      (UINT *) & length, (BOOL)TRUE);
        if ((INT)next_offset < 0)
            {
            return(INT)next_offset;
            }
        mark_offset = offset + 20;  /* keep trace the offset of DataCode 102.  */
        ptr = record;
        DECODE_DATA (ptr, &data_type, &data_size);
        for (i = 0; i + dataCodeSize + data_size <= length AND NOT end; )
            {
            
            /*
               * looking for DataCode 300 --- start of Nlist.
            */
            switch (data_type)
                {
            case 102:
                if (checkName)
                    {
                    checkName = FALSE;
                    if (ARE_SAME (name, (char *)ptr))
                        {
                        rightName = TRUE;
                        /* if the name is found, set erased mark.  */
                        mark_offset = offset + 20 + i;
                        control_record->bytes_deleted += (sizeof(INT) * 2 + data_size);
                        }
                    else
                        rightName = FALSE;
                    }
                break;
                
            case 118:
                if (rightName)
                    {
                    hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & returnOffset,
                                 (size_t)sizeof(INT));
                    end = TRUE;
                    }
                break;
                
            case 300:
                checkName = TRUE;
                break;
                
            case 301:
                if (rightName)
                    {
                    hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & temp, (size_t)sizeof(INT));
                    control_record->bytes_deleted += temp;
                    control_record->total_nlists--;
                    }
                break;
                
            default:
                break;
                }
            if ((INT)data_size < 0)
                end = TRUE;
            else
                {
                i += dataCodeSize + data_size;
                ptr += data_size;
                if (i < length)
                    {
                    DECODE_DATA (ptr, &data_type, &data_size);
                    }
                }
            }
        tc_free ((VOIDPTR)record);
        
        } while (NOT end);
    
    /*
       * write out the erased mark.
    */
    nlh_seek (funit, (INT)mark_offset, (INT)SEEK_SET);
    nlh_write (funit, (VOIDPTR)&eraseMarkCode, sizeof(INT));
    status = nlz_put_control_record ();
    nlz_update_index_item (filename, name);
    nlh_close (funit);
    return returnOffset;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To recover a deleted nlist, if it exists, return the header offset of
    entry, otherwise return -1.

Prototype:
    static INT nlz_delete_recover_nlist (FILE_UNIT funit, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    name            -(NLIST_ID)

Return Value/Status:
    Function returns the header offset of entry, otherwise return -1.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_delete_recover_nlist (FILE_UNIT funit, NLIST_ID name)
#else
static INT nlz_delete_recover_nlist (funit, name)
FILE_UNIT funit;
NLIST_ID name;
#endif
    {
    NLIST_CONTROL_RECORD *control_record;
    BYTE *ptr, *record;
    INT temp;
    INT status;
    INT i;
    UINT offset, next_offset, length;
    INT returnOffset = 0, MarkCode = 102;
    INT data_type;
    size_t data_size;
    INT mark_offset;
    size_t dataCodeSize = 2 * sizeof(INT);
    BOOL rightName = FALSE, checkName = FALSE;
    BOOL found = FALSE, end = FALSE;
    
    /* read the control record.     */
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    /* read the file directory.   */
    offset = control_record->directory_offset;
    do
        {
        next_offset = nlz_get_record (funit, (UINT)offset, (VOIDPTR *) & record,
                                      (UINT *) & length, (BOOL)FALSE);
        if ((INT)next_offset < 0)
            {
            return(INT)next_offset;
            }
        mark_offset = offset + 20;  /* keep trace the offset of DataCode 102.  */
        ptr = record;
        DECODE_DATA (ptr, &data_type, &data_size);
        for (i = 0; i + dataCodeSize + data_size <= length AND NOT found; )
            {
            
            /*
               * looking for DataCode 300 --- start of Nlist.
            */
            switch (data_type)
                {
            case 888:
                if (checkName)
                    {
                    checkName = FALSE;
                    if (ARE_SAME (name, (char *)ptr))
                        {
                        rightName = TRUE;
                        /* if the name is found, set  mark.  */
                        mark_offset = offset + 20 + i;
                        control_record->bytes_deleted -= (sizeof(INT) * 2 + data_size);
                        }
                    else
                        rightName = FALSE;
                    }
                break;
                
            case 118:
                if (rightName)
                    {
                    found = TRUE;
                    hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & returnOffset,
                                 (size_t)sizeof(INT));
                    nlh_seek (funit, mark_offset, (INT)SEEK_SET);
                    }
                break;
                
            case 300:
                checkName = TRUE;
                break;
                
            case 301:
                if (rightName)
                    {
                    hoblockmove ((VOIDPTR)ptr, (VOIDPTR) & temp, (size_t)sizeof(INT));
                    control_record->bytes_deleted -= temp;
                    control_record->total_nlists++;
                    }
                break;
                
            default:
                break;
                }
            i += dataCodeSize + data_size;
            ptr += data_size;
            if (i < length AND NOT found)
                DECODE_DATA (ptr, &data_type, &data_size);
            }
        tc_free ((VOIDPTR)record);
        
        /*   if the current logical record is the    */
        /*   last one, set END flag to be TRUE,      */
        /*   otherwise, set the offset.              */
        
        if (next_offset EQUALS 0)
            end = TRUE;
        else
            offset = next_offset;
        
        } while (NOT found AND NOT end);
    
    /*
       * write out the erased mark.
    */
    if (found)
        {
        nlh_write (funit, (VOIDPTR)&MarkCode, sizeof(INT));
        nlz_put_control_record ();
        }
    nlh_close (funit);
    status = (found) ? returnOffset :  - 1;
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static VOID nlz_encode_data (BYTE **ptr, INT dataCode,
        size_t dataSize, VOIDPTR data);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    ptr             -(BYTE **)
    dataCode        -(INT)
    dataSize        -(size_t)
    data            -(VOIDPTR)

Return Value/Status:
    None.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static VOID nlz_encode_data (BYTE **ptr, INT dataCode, size_t dataSize,
                                 VOIDPTR data)
#else
static VOID nlz_encode_data (ptr, dataCode, dataSize, data)
BYTE **ptr;
INT dataCode;
size_t dataSize;
VOIDPTR data;
#endif
    {
    hoblockmove ((VOIDPTR) & dataCode, (VOIDPTR) * ptr, (size_t)sizeof(INT));
    *ptr += sizeof(INT);
    hoblockmove ((VOIDPTR) & dataSize, (VOIDPTR) * ptr, (size_t)sizeof(INT));
    *ptr += sizeof(INT);
    hoblockmove (data, (VOIDPTR) * ptr, (size_t)dataSize);
    *ptr += dataSize;
    
    return;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read a nlist named NAME to the file.

Prototype:
    static INT nlz_read_nlist (NLIST_HEADER *nlist,
        FILENAME in_filename, NLIST_ID in_name, NL_SHARE_MODE share);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)
    in_filename     -(FILENAME)
    in_name         -(NLIST_ID)
    share           -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    NL_FILE_NOT_FOUND
    NL_INVALID_FILENAME
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_read_nlist (NLIST_HEADER *nlist, FILENAME in_filename,
                               NLIST_ID in_name, NL_SHARE_MODE share)
#else
static INT nlz_read_nlist (nlist, in_filename, in_name, share)
NLIST_HEADER *nlist;
FILENAME in_filename;
NLIST_ID in_name;
NL_SHARE_MODE share;
#endif
    {
    INT headerOffset, headerArraySize, status;
    BYTE *headerArray;
    FILE_UNIT funit;
    FILENAME filename;
    NLIST_ID name;
    
    if (in_filename EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    /* translate the file name first to
        get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return NL_FILE_NOT_FOUND;
        }
    strcpy (name, in_name);
    ts_sto_upper (name);
    
    /* check the existence of nlist named NAME.                       */
    headerOffset = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)FALSE, share);
    if (headerOffset < 0)
        {
        return headerOffset;
        }
    /*
       * read the header of specified N_LIST.
    */
    status = nlz_get_record (funit, (UINT)headerOffset, (VOIDPTR *) & headerArray,
                             (UINT *) & headerArraySize, (BOOL)FALSE);
    if (status < 0)
        {
        return status;
        }
    /*
       *  to reconstruct the N_LIST.
    */
    status = nlz_construct_nlist (nlist, (VOIDPTR)headerArray,
                                  (size_t)headerArraySize);
    tc_free ((VOIDPTR)headerArray);
    if (status < 0)
        {
        return NL_FILE_OPEN_ERROR;
        }
    /*
       * set the data filename and N_LIST name.
    */
    
    nl_set_nlist_string (*nlist, NL_NLIST_NAME, name);
    nl_set_nlist_string (*nlist, NL_NLIST_FILENAME, filename);
    
    nl_set_current_row (*nlist, 1, 1);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Reset the value of nl_max_entry_in_block - called from nl_clean.

Prototype:
    publicdef INT nlz_set_block_entries(INT nblocks);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nblocks         -(INT)

Return Value/Status:
    Returns the previous value of nl_max_entry_in_block.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_set_block_entries (INT nblocks)
#else
publicdef INT nlz_set_block_entries (nblocks)
INT nblocks;
#endif
    {
    INT last_value = nl_max_entry_in_block;
    nl_max_entry_in_block = nblocks;
    return last_value;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This routine is intended to allow each machine to
    write an n-List into a native format, and to stamp information into the file
    that will allow network access, even to the extent that a file written on
    one machine can be cracked on another.
    The entries in this structure will enable a translator routine to decide
    what transformations will be necessary to read the file.  Note that ALL entries
    in a file should follow these conventions.  To this end, it may be desirable
    to have files not written by the local OS to be read-only files... 

Prototype:
    static INT nlz_stamp_version (NLIST_CONTROL_RECORD *pcontrol,
        CHAR *version);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pcontrol        -(NLIST_CONTROL_RECORD *)
    version         -(CHAR *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_stamp_version (NLIST_CONTROL_RECORD *pcontrol, CHAR *version)
#else
static INT nlz_stamp_version (pcontrol, version)
NLIST_CONTROL_RECORD *pcontrol;
CHAR *version;
#endif
    {
    
    ho_get_os_info (pcontrol->os_info);
    
    strcpy (pcontrol->version_stamp, version);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update the infomation of File Control Record and File Directory.  


Prototype:
    static INT nlz_update_direct_and_control (
        FILE_UNIT funit, FILENAME filename, NLIST_ID name, NLIST_HEADER nlist,
        UINT header_offset, size_t totalBytes, BOOL new_dir,
        UINT exist_offset, NLIST_CONTROL_RECORD *control_record);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    filename        -(FILENAME)
    name            -(NLIST_ID)
    nlist           -(NLIST_HEADER)
    header_offset   -(UINT)
    totalBytes      -(size_t)
    new_dir         -(BOOL)
    exist_offset    -(UINT)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    Function returns status from nlz_update_dir_and_ctrl_v4 or 
    nlz_update_dir_and_ctrl_v3.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_update_direct_and_control (FILE_UNIT funit, FILENAME filename,
                                              NLIST_ID name, NLIST_HEADER nlist,
                                              UINT header_offset, size_t totalBytes,
                                              BOOL new_dir, UINT exist_offset,
                                              NLIST_CONTROL_RECORD *control_record)
#else
static INT nlz_update_direct_and_control (funit, filename, name, nlist,
                                              header_offset, totalBytes, new_dir,
                                              exist_offset, control_record)
FILE_UNIT funit;
FILENAME filename;
NLIST_ID name;
NLIST_HEADER nlist;
UINT header_offset;
size_t totalBytes;
BOOL new_dir;
UINT exist_offset;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
    INT status;
    
    if (new_dir OR ARE_SAME (NL_VERSION_4, control_record->version_stamp))
        {
        status = nlz_update_dir_and_ctrl_v4 (funit, filename, name, nlist,
                                             header_offset, totalBytes, exist_offset,
                                             control_record);
        }
    else
        {
        status = nlz_update_dir_and_ctrl_v3 (funit, filename, name, nlist,
                                             header_offset, totalBytes, exist_offset,
                                             control_record);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update the infomation of File Control Record and File Directory.  

Prototype:
    static INT nlz_update_dir_and_ctrl_v3 (
        FILE_UNIT funit, FILENAME filename, NLIST_ID name,
        NLIST_HEADER nlist, UINT header_offset, size_t totalBytes,
        UINT exist_offset, NLIST_CONTROL_RECORD *control_record);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    filename        -(FILENAME)
    name            -(NLIST_ID)
    nlist           -(NLIST_HEADER)
    header_offset   -(UINT)
    total_Bytes     -(size_t)
    exist_offset    -(UINT)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    Function returns status from nlz_put_directories.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_update_dir_and_ctrl_v3 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset, size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD *control_record)
#else
static INT nlz_update_dir_and_ctrl_v3 (funit, filename, name, nlist, header_offset,
                                           totalBytes, exist_offset, control_record)
FILE_UNIT funit;
FILENAME filename;
NLIST_ID name;
NLIST_HEADER nlist;
UINT header_offset;
size_t totalBytes;
UINT exist_offset;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
    INT status;
    size_t dataArraySize, dataCodeSize = sizeof(INT) * 2;
	size_t nbytes = totalBytes;
    UINT new_entry_offset = 0;
    BOOL empty_dir = FALSE;
    UINT tempOffset;
    BYTE *dataArray, *ptr;
    NLIST_LOGICAL_RECORD *plogicalRec, tempLogicalRec;
    
    /* if the EXIST_OFFSET is not zero,
       means that the requested function
       is to update the directory entry */
    
    if (exist_offset > 0)
        {
        /* when we update the directory entry,
           the only changed information is
           the total of bytes in the nlist,
           so we set the offset to the right
           offset.   */
        
        tempOffset = exist_offset + sizeof(NLIST_CONTROL_RECORD) + sizeof(NLIST_ID)
        + (sizeof(INT) * 2)         /* For CODE 102 */
        + (sizeof(INT) * 3)         /* For CODE 300 */
        + (sizeof(INT) * 2);        /* For CODE 301 */
        nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
        nlh_write (funit, (VOIDPTR)&nbytes, sizeof(INT));
        return SUCCESS;
        }
    if (control_record->directory_offset EQUALS 0)
        {
        empty_dir = TRUE;
        control_record->directory_offset = control_record->next_write_offset;
        }
    tempOffset = control_record->f_directory_offset;
    control_record->f_directory_offset = control_record->next_write_offset;
    strcpy (control_record->version_stamp, NL_VERSION_3);
    control_record->total_nlists += 1;
    
    /*
       * update the File Directory.
    */
    /*
       * allocate spaces for new nlist entry.
    */
    /* spaces for Data Code 101, 105, 107, 118, 300, 301.     INTEGER */
    dataArraySize = (dataCodeSize + sizeof(INT)) * 6;
    
    /* spaces for the name of nlist.  102.                    CHAR[]. */
    dataArraySize += (dataCodeSize + strlen ((char *)name) + 1);
    dataArraySize += (dataArraySize & 1);
    
    dataArray = ptr = (BYTE *)tc_alloc ((size_t)(dataArraySize +
                                                 sizeof(NLIST_LOGICAL_RECORD) + 1));
    if (ptr == (BYTE *)0)
        {
        return NL_OVERFLOW;         /* tried to get too much memory from system */
        }
    /* Initialize logical record header */
    plogicalRec = (NLIST_LOGICAL_RECORD *)ptr;
    ptr += sizeof(NLIST_LOGICAL_RECORD);
    plogicalRec->offset = new_entry_offset = control_record->next_write_offset;
    plogicalRec->next_offset = 0;
    plogicalRec->first_offset = control_record->directory_offset;
    plogicalRec->total_bytes = dataArraySize + (dataArraySize & 1);
    plogicalRec->bytes = dataArraySize;
    
    dataArraySize += sizeof(NLIST_LOGICAL_RECORD);
    
    /*
       * move data into the buffer.
    */
    
    nlz_encode_data ((BYTE **) & ptr, (INT)300, (size_t)sizeof(INT),
                     (VOIDPTR) & control_record->total_nlists);
    nlz_encode_data ((BYTE **) & ptr, (INT)102, (size_t)(strlen ((char *)name) + 1),
                     (VOIDPTR)name);
    nlz_encode_data ((BYTE **) & ptr, (INT)301, (size_t)sizeof(INT),
                     (VOIDPTR) & nbytes);
    nlz_encode_data ((BYTE **) & ptr, (INT)101, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->nCol);
    nlz_encode_data ((BYTE **) & ptr, (INT)105, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_rows);
    nlz_encode_data ((BYTE **) & ptr, (INT)107, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_tables);
    nlz_encode_data ((BYTE **) & ptr, (INT)118, (size_t)sizeof(INT),
                     (VOIDPTR) & header_offset);
    /*
       * if the directory is empty, append the new entry, then is done.
       * Otherwise, go through all previous entry to update the Total_bytes.
    */
    status = nlz_put_directories (funit, dataArraySize, dataArray,
                                  control_record);
    if (status < 0)
        {
        return status;
        }
    if (NOT empty_dir)
        {
        if (tempOffset EQUALS 0)
            {
            tempOffset = control_record->directory_offset;
            do
                {
                nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
                nlh_read (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
                tempOffset = tempLogicalRec.next_offset;
                if (tempOffset IS_NOT_EQUAL_TO 0)
                    {
                    
                    nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
                    nlh_read (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
                    }
                } while (tempOffset IS_NOT_EQUAL_TO 0);
            }
        else
            {
            nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
            nlh_read (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
            }
        tempLogicalRec.next_offset = new_entry_offset;
        nlh_seek (funit, (INT)tempLogicalRec.offset, (INT)SEEK_SET);
        status = nlh_write (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
        }
    /*   update the bTree.            */
    status = nlz_update_index (filename, name, new_entry_offset, header_offset);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update the infomation of File Control Record and File Directory.  

Prototype:
    static INT nlz_update_dir_and_ctrl_v4 (
        FILE_UNIT funit, FILENAME filename, NLIST_ID name,
        NLIST_HEADER nlist, UINT header_offset, size_t totalBytes,
        UINT exist_offset, NLIST_CONTROL_RECORD *control_record);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    filename        -(FILENAME)
    name            -(NLIST_ID)
    nlist           -(NLIST_HEADER)
    header_offset   -(UINT)
    total_Bytes     -(size_t)
    exist_offset    -(UINT)
    control_record  -(NLIST_CONTROL_RECORD *)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_update_dir_and_ctrl_v4 (FILE_UNIT funit, FILENAME filename,
                                           NLIST_ID name, NLIST_HEADER nlist,
                                           UINT header_offset, size_t totalBytes,
                                           UINT exist_offset,
                                           NLIST_CONTROL_RECORD *control_record)
#else
static INT nlz_update_dir_and_ctrl_v4 (funit, filename, name, nlist, header_offset,
                                           totalBytes, exist_offset, control_record)
FILE_UNIT funit;
FILENAME filename;
NLIST_ID name;
NLIST_HEADER nlist;
UINT header_offset;
size_t totalBytes;
UINT exist_offset;
NLIST_CONTROL_RECORD *control_record;
#endif
    {
#define V4_NLIST_ID_SIZE    62
    INT status;
    UINT dataArraySize;
    UINT new_entry_offset;
	size_t nbytes = totalBytes;
    BOOL empty_dir = FALSE;
    UINT tempOffset;
    BYTE *dataArray, *ptr;
    NLIST_LOGICAL_RECORD tempLogicalRec, *pLogicalRec;
    
    /* if the EXIST_OFFSET is not zero,
       means that the requested function
       is to update the directory entry */
    
    if (exist_offset > 0)
        {
        /* when we update the directory entry,
           the only changed information is
           the total of bytes in the nlist,
           so we set the offset to the right
           offset.   */
        
        tempOffset = exist_offset + sizeof tempLogicalRec + V4_NLIST_ID_SIZE +
                     (sizeof(INT) * 2)/* For CODE 102 */
        + (sizeof(INT) * 3)         /* For CODE 300 */
        + (sizeof(INT) * 2);        /* For CODE 301 */
        
        nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
        nlh_write (funit, (VOIDPTR)&nbytes, sizeof(INT));
        return SUCCESS;
        }
    if (control_record->directory_offset EQUALS 0)
        {
        empty_dir = TRUE;
        control_record->directory_offset = control_record->next_write_offset;
        }
    tempOffset = control_record->f_directory_offset;
    nlz_stamp_version (control_record, (CHAR *)NL_VERSION_4);
    control_record->total_nlists += 1;
    
    /* if the whole directory is empty, */
    /* by default, initialize a ten dir.*/
    /* block following a info. record.  */
    
    if (empty_dir)
        {
        /* update the control record first. */
        
        control_record->f_directory_offset = control_record->next_write_offset;
        
        /* creates the first new dir block. */
        
        dataArraySize = sizeof tempLogicalRec + nl_max_entry_in_block *
                        (sizeof(DIRECTORY_ENTRY));
        
        dataArray = (BYTE *)TALLOC (dataArraySize);
        
        /*  load up the logical record portion of the   */
        /*  dataArray with directory info           */
        pLogicalRec = (NLIST_LOGICAL_RECORD *)dataArray;
        pLogicalRec->offset = control_record->next_write_offset;
        pLogicalRec->next_offset = 0;
        pLogicalRec->total_bytes = nl_max_entry_in_block;
        pLogicalRec->bytes = 1;
        new_entry_offset = pLogicalRec->first_offset = pLogicalRec->offset +
                           sizeof(NLIST_LOGICAL_RECORD);
        status = nlz_put_directories (funit, dataArraySize, dataArray,
                                      control_record);
#if NEED_TO_FREE EQUALS TRUE
        tc_free ((VOIDPTR)dataArray);
#endif
        if (status < 0)
            {
            return status;
            }
        }
    else
        {
        /*  if there are existing dir. block.  , checks */
        /*  the last dir. block first.  if there is an  */
        /*  available entry, put the new name in the    */
        /*  entry.                      */
        
        nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
        nlh_read (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
        
        /*  For the first INFO. logical record, the     */
        /*  TOTAL_BYTES field represent the total       */
        /*  number of dir. entry in the block, the      */
        /*  BYTES field represents the existing     */
        /*  directories.                    */
        
        if (tempLogicalRec.total_bytes > tempLogicalRec.bytes)
            {
            nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
            new_entry_offset = tempLogicalRec.first_offset + sizeof(DIRECTORY_ENTRY) *
                               tempLogicalRec.bytes;
            tempLogicalRec.bytes += 1;
            nlh_write (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
            }
        else
            {
            
            nlh_seek (funit, (INT)tempOffset, (INT)SEEK_SET);
            tempLogicalRec.next_offset = control_record->f_directory_offset =
                                         control_record->next_write_offset;
            
            /* update the link between the last  */
            /* dir. block and new dir. block.    */
            
            nlh_write (funit, (VOIDPTR)&tempLogicalRec, sizeof tempLogicalRec);
            
            /* creates a new dir. block and links*/
            /* update the control record first. */
            
            dataArraySize = sizeof tempLogicalRec + nl_max_entry_in_block *
                            (sizeof(DIRECTORY_ENTRY));
            
            dataArray = (BYTE *)TALLOC (dataArraySize);
            
            pLogicalRec = (NLIST_LOGICAL_RECORD *)dataArray;
            pLogicalRec->offset = control_record->next_write_offset;
            pLogicalRec->next_offset = 0;
            pLogicalRec->total_bytes = nl_max_entry_in_block;
            pLogicalRec->bytes = 1;
            new_entry_offset = pLogicalRec->first_offset = pLogicalRec->offset +
                               sizeof(NLIST_LOGICAL_RECORD);
            status = nlz_put_directories (funit, dataArraySize, (VOIDPTR)dataArray,
                                          control_record);

#if NEED_TO_FREE EQUALS TRUE
            tc_free ((VOIDPTR)dataArray);
#endif
            if (status < 0)
                {
                return status;
                }
            }
        }
    /*
       * update the File Directory.
    */
    dataArray = ptr = (BYTE *)TALLOC (sizeof(DIRECTORY_ENTRY));
    if (ptr == (BYTE *)0)
        return NL_OVERFLOW;         /* tried to get too much memory from system */
    
    nlh_seek (funit, (INT)new_entry_offset, (INT)SEEK_SET);
    
    /*  Load up the logical record in place     */
    pLogicalRec = (NLIST_LOGICAL_RECORD *)ptr;
    ptr += sizeof(NLIST_LOGICAL_RECORD);
    
    pLogicalRec->offset = new_entry_offset;
    pLogicalRec->next_offset = 0;
    pLogicalRec->bytes = pLogicalRec->total_bytes = sizeof(DIRECTORY_ENTRY);
    pLogicalRec->first_offset = new_entry_offset + 20;
    
    /*
       * move data into the buffer.
    */
    
    nlz_encode_data ((BYTE **) & ptr, (INT)300, (size_t)sizeof(INT),
                     (VOIDPTR) & control_record->total_nlists);
    
    /* Since the V4 directory entry can
       only accomodate 62 bytes of the
       nlist_id, we use only the first
       61 bytes of it, plus the null. */
    
    if (strlen ((char *)name) > V4_NLIST_ID_SIZE - 1)
        name[V4_NLIST_ID_SIZE - 1] = '\0';
    nlz_encode_data ((BYTE **) & ptr, (INT)102, (size_t)V4_NLIST_ID_SIZE,
                     (VOIDPTR)name);
    nlz_encode_data ((BYTE **) & ptr, (INT)301, (size_t)sizeof(INT),
                     (VOIDPTR) & nbytes);
    nlz_encode_data ((BYTE **) & ptr, (INT)101, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->nCol);
    nlz_encode_data ((BYTE **) & ptr, (INT)105, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_rows);
    nlz_encode_data ((BYTE **) & ptr, (INT)107, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_tables);
    nlz_encode_data ((BYTE **) & ptr, (INT)118, (size_t)sizeof(INT),
                     (VOIDPTR) & header_offset);
    
    status = nlh_write (funit, (VOIDPTR)dataArray, sizeof(DIRECTORY_ENTRY));
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)dataArray);
#endif
    if (status < 0)
        {
        return status;
        }
    /*   update the bTree.            */
    status = nlz_update_index (filename, name, new_entry_offset, header_offset);
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write out the header of N_LIST nas tables.    

Prototype:
    static INT nlz_write_header (FILE_UNIT funit, NLIST_HEADER nlist,
        UINT *header_offset, size_t *totalBytesInEntry,
        NLIST_CONTROL_RECORD *control_record, UINT exist_offset);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit               -(FILE_UNIT)
    nlist               -(NLIST_HEADER)
    header_offset       -(UINT *)
    totalBytesInEntry   -(size_t *)
    control_record      -(NLIST_CONTROL_RECORD *)
    exist_offset        -(UINT) If the existing offset is zero, means that this header
                            is a new header record, otherwise will be the offset
                            of existing header record for being updated.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    Function returns the header offset.
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_write_header (FILE_UNIT funit, NLIST_HEADER nlist,
                                 UINT *header_offset, size_t *totalBytesInEntry,
                                 NLIST_CONTROL_RECORD *control_record,
                                 UINT exist_offset)
#else
static INT nlz_write_header (funit, nlist, header_offset, totalBytesInEntry,
                                 control_record, exist_offset)
FILE_UNIT funit;
NLIST_HEADER nlist;
UINT *header_offset;
size_t *totalBytesInEntry;
NLIST_CONTROL_RECORD *control_record;
UINT exist_offset;
#endif
    {
    register UINT i;
    register size_t dataArraySize = 0;
    INT dataCode;
    size_t dataSize;
    size_t dataCodeSize;
    UINT nCol = nlist->nCol;
    UINT nmethods;
    NL_METHOD_HEADER *pMethod;
    size_t *dataWidth;
    INT *dataType, *units, *incValue, *n_valid_array;
    DOUBLE *minArray, *maxArray, *sumArray, *sumSquareArray, *nullValue, *findTolArray,
        *startValueArray, *increArray;
    BYTE *dataArray, *ptr;
    COLUMN_HEADER *pinfo;
    NLIST_LOGICAL_RECORD *plogicalRec;
    DATE today;
    size_t len;
    CHAR **size10_arr;
    CHAR **size32_arr;
    BOOL write_dim_names;
    
    *totalBytesInEntry = 0;
    dataCodeSize = sizeof(UINT) * 2;    /* space for each DataCode and DataSize*/
    
    /*
       * count the dataSize of N_LIST header.
    */
    /* spaces for DataCode 101, 105 - 107, 112, 118, 122, 125 INTEGER*/
    dataArraySize += (sizeof(UINT) + dataCodeSize) * 8;
    
    /* spaces for DataCode 109 - 111, 120, 129            INTEGER[].  */
    dataArraySize += (dataCodeSize + nCol * sizeof(INT)) * 5;
    /* spaces for DataCode 126.             INTEGER[]   */
    
    dataArraySize += (dataCodeSize + sizeof(INT) * nlist->total_tables);
    
    /* spaces for DataCode 123, 124              CHAR[32], CHAR[10] */
    /*
    * If there is no any column has defined column name, we need not write
    * out the column names, but if there is one column has column name,
    * we need write out column names for all columns.
*/
    for (i = 0, write_dim_names = FALSE, pinfo = nlist->info_ptr; i < nCol;
         i++, pinfo++)
        {
        if (IS_STRING (pinfo->column_name))
            {
            write_dim_names = TRUE;
            break;
            }
        }
    if (write_dim_names)
        {
        dataArraySize += (dataCodeSize + nCol * 32);
        }
    dataArraySize += (dataCodeSize + nCol * 10);
    
    /* spaces for DataCode 100, 102 - 104,       CHAR[], BYTE[]     */
    
    dataArraySize += dataCodeSize + sizeof nlist->version.Revision +
                     ROUND_UP (sizeof nlist->version.Revision);
    
    if (IS_STRING (nlist->name))
        {
        len = strlen ((char *)nlist->name);
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    if (IS_STRING (nlist->prev_nlist))
        {
        len = strlen ((char *)nlist->prev_nlist);
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    if (IS_STRING (nlist->next_nlist))
        {
        len = strlen ((char *)nlist->next_nlist);
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    if (nlist->user_struct_len IS_NOT_EQUAL_TO 0)
        {
        len = nlist->user_struct_len;
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    dataArraySize += dataCodeSize + sizeof(DATE);   /*   creation_date.         */
    
    /* spaces for DataCode 108, 113 - 117.             DOUBLE[]     */
    dataArraySize += (dataCodeSize + sizeof(DOUBLE) * nCol) * 6;
    
    /* space for any methods defined, Datacodes 127 and 128 */
    for (nmethods = 0, pMethod = nlist->method_list; pMethod != (NL_METHOD_HEADER *)0;
         nmethods++, pMethod = pMethod->next)
        {
        }
    if (nmethods > 0)
        {
        /*    one datacode for methods code flag,
            one unsigned integer for the number of characters in each method function name,
        and enough room to store a datacode, an integer
        method code and character method name for each method */
        
        len = sizeof(NLM_FUNCTION_NAME);
        dataArraySize += dataCodeSize + sizeof(UINT) + nmethods * (dataCodeSize +
                                                                   (sizeof(
                                                                         NLIST_METHOD) 
                                                                    + len +
                                                                    ROUND_UP (len)));
        }
    /* space for end of header record.           */
    dataArraySize += dataCodeSize;
    
    /*
       * allocate the space for the dataArray.
    */
    dataArray = ptr = (BYTE *)TALLOC ((size_t)(dataArraySize +
                                               sizeof(NLIST_LOGICAL_RECORD) + 1));
    if (ptr == (BYTE *)0)
        {
        return NL_OVERFLOW;         /* tried to get too much memory from system */
        }
    /* Initialize the logical record header */
    plogicalRec = (NLIST_LOGICAL_RECORD *)ptr;
    ptr += sizeof(NLIST_LOGICAL_RECORD);
    if (exist_offset EQUALS 0)
        {
        plogicalRec->total_bytes = plogicalRec->bytes = dataArraySize;
        plogicalRec->next_offset = 0;
        }
    /* otherwise, let nlz_update_record handle the logical record header */
    dataArraySize += sizeof(NLIST_LOGICAL_RECORD);
    *totalBytesInEntry += dataArraySize;
    
    /*
       * moves the info of n-LIST header.
    */
    dataWidth = (size_t *)TALLOC (nCol * sizeof(size_t));
    dataType = (INT *)TALLOC (nCol * sizeof(INT));
    units = (INT *)TALLOC (nCol * sizeof(INT));
    incValue = (INT *)TALLOC (nCol * sizeof(INT));
    n_valid_array = (INT *)TALLOC (nCol * sizeof(INT));
    minArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    maxArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    sumArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    sumSquareArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    findTolArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    nullValue = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    startValueArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    increArray = (DOUBLE *)TALLOC (nCol * sizeof(DOUBLE));
    size10_arr = (CHAR * *)TALLOC (nCol * sizeof(CHAR *));
    size32_arr = (CHAR * *)TALLOC (nCol * sizeof(CHAR *));
    
    nlz_encode_data ((BYTE **) & ptr, (INT)101, (size_t)sizeof(INT), (VOIDPTR) & nCol);
    nlz_encode_data ((BYTE **) & ptr, (INT)112, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->blocksize);
    
    for (i = 0, pinfo = nlist->info_ptr; i < nCol; i++, pinfo++)
        {
        dataWidth[i] = pinfo->data_size;
        dataType[i] = pinfo->data_type;
        units[i] = pinfo->unit;
        incValue[i] = (pinfo->inc_value) ? INCREMENT_LIST : VALUE_LIST;
        n_valid_array[i] = pinfo->n_valid_rows;
        minArray[i] = pinfo->min;
        maxArray[i] = pinfo->max;
        sumArray[i] = pinfo->sum_x;
        sumSquareArray[i] = pinfo->sum_x_squared;
        findTolArray[i] = pinfo->tolerance;
        nullValue[i] = pinfo->null_value;
        size10_arr[i] = pinfo->convert_format;
        size32_arr[i] = pinfo->column_name;
        }
    nlz_encode_data ((BYTE **) & ptr, (INT)109, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)units);
    nlz_encode_data ((BYTE **) & ptr, (INT)110, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)dataType);
    nlz_encode_data ((BYTE **) & ptr, (INT)111, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)dataWidth);
    nlz_encode_data ((BYTE **) & ptr, (INT)120, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)incValue);
    nlz_encode_data ((BYTE **) & ptr, (INT)129, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)n_valid_array);
    nlz_encode_data ((BYTE **) & ptr, (INT)122, (size_t)sizeof(BOOL),
                     (VOIDPTR) & nlist->defer_stats);
    nlz_encode_data ((BYTE **) & ptr, (INT)113, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)minArray);
    nlz_encode_data ((BYTE **) & ptr, (INT)114, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)maxArray);
    nlz_encode_data ((BYTE **) & ptr, (INT)115, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)sumArray);
    nlz_encode_data ((BYTE **) & ptr, (INT)116, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)sumSquareArray);
    nlz_encode_data ((BYTE **) & ptr, (INT)117, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)findTolArray);
    hoblockmove ((VOIDPTR)tc_today (today), (VOIDPTR)nlist->creation_date,
                 (size_t)sizeof(DATE));
    nlz_encode_data ((BYTE **) & ptr, (INT)119, (size_t)sizeof(DATE),
                     (VOIDPTR)nlist->creation_date);
    nlz_encode_data ((BYTE **) & ptr, (INT)108, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)nullValue);
    dataCode = 123;
    dataSize = sizeof(nlist->info_ptr[0].convert_format) * nCol;
    hoblockmove ((VOIDPTR) & dataCode, (VOIDPTR)ptr, (size_t)sizeof(INT));
    ptr += sizeof(INT);
    hoblockmove ((VOIDPTR) & dataSize, (VOIDPTR)ptr, (size_t)sizeof(size_t));
    ptr += sizeof(size_t);
    for (i = 0; i < nCol; i++)
        {
        hoblockmove ((VOIDPTR)size10_arr[i], (VOIDPTR)ptr, (size_t)(dataSize / nCol));
        ptr += dataSize / nCol;
        }
    if (write_dim_names)
        {
        dataCode = 124;
        dataSize = sizeof(nlist->info_ptr[0].column_name) * nCol;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        for (i = 0; i < nCol; i++)
            {
            hoblockmove ((VOIDPTR)size32_arr[i], (VOIDPTR)ptr,
                         (size_t)(dataSize / nCol));
            ptr += dataSize / nCol;
            }
        }
    nlz_encode_data ((BYTE **) & ptr, (INT)125, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->nlist_data_class);
    nlz_encode_data ((BYTE **) & ptr, (INT)105, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_rows);
    nlz_encode_data ((BYTE **) & ptr, (INT)107, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->total_tables);
    
    dataCode = 126;
    dataSize = sizeof(INT) * nlist->total_tables;
    ENCODE_DATA (ptr, &dataCode, &dataSize);
    for (i = 0; i < nlist->total_tables; i++)
        {
        hoblockmove ((VOIDPTR) & nlist->table_elements[i].header_offset, (VOIDPTR)ptr,
                     (size_t)sizeof(INT));
        ptr += sizeof(INT);
        }
    dataCode = 100;
    dataSize = sizeof(nlist->version.Revision);
    ENCODE_DATA (ptr, &dataCode, &dataSize);
    strcpy (nlist->version.Revision, NL_VERSION_4);
    hoblockmove ((VOIDPTR)nlist->version.Revision, (VOIDPTR)ptr,
                 sizeof nlist->version.Revision);
    ptr += sizeof nlist->version.Revision + ROUND_UP (sizeof nlist->version.Revision);
    
    if (IS_STRING (nlist->name))
        {
        len = strlen ((char *)nlist->name);
        dataCode = 102, dataSize = len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)nlist->name, (VOIDPTR)ptr, (size_t)len);
        ptr += len + ROUND_UP (len);
        }
    if (nlist->user_struct_len IS_NOT_EQUAL_TO 0)
        {
        dataCode = 103, dataSize = nlist->user_struct_len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)nlist->user_struct, (VOIDPTR)ptr,
                     (size_t)nlist->user_struct_len);
        ptr += nlist->user_struct_len + ROUND_UP (nlist->user_struct_len);
        }
    if (IS_STRING (nlist->prev_nlist))
        {
        len = strlen ((char *)nlist->prev_nlist);
        dataCode = 121, dataSize = len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)nlist->name, (VOIDPTR)ptr, (size_t)len);
        ptr += len + ROUND_UP (len);
        }
    if (IS_STRING (nlist->next_nlist))
        {
        len = strlen ((char *)nlist->next_nlist);
        dataCode = 104, dataSize = len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)nlist->name, (VOIDPTR)ptr, (size_t)len);
        ptr += len + ROUND_UP (len);
        }
    /* Methods processor */
    if (nmethods > 0)
        {
        dataSize = len = sizeof(NLM_FUNCTION_NAME);
        /* enter the number of characters in each method function name */
        nlz_encode_data ((BYTE **) & ptr, (INT)127, (size_t)sizeof(INT),
                         (VOIDPTR) & dataSize);
        
        /* Now write out each method in the form ID NAME,
        where ID is the method identifier, and NAME is the ASCII text name
        of the function.  Note the round up stuff */
        dataCode = 128, dataSize = sizeof(NLIST_METHOD) + len + ROUND_UP (len);
        for (pMethod = nlist->method_list; pMethod != (NL_METHOD_HEADER *)0;
                                                                                       pMethod
                                                                                        
                                                                                       =
                                                                                        
                                                                                       pMethod
                                                                                       ->
                                                                                       next
                                                                                       )
                                                                                       
            {
            ENCODE_DATA (ptr, &dataCode, &dataSize);
            hoblockmove ((VOIDPTR) & pMethod->method_id, (VOIDPTR)ptr,
                         sizeof(NLIST_METHOD));
            ptr += sizeof(NLIST_METHOD);
            hoblockmove ((VOIDPTR)pMethod->function_name, (VOIDPTR)ptr, len);
            ptr += len + ROUND_UP (len);
            }
        }
    dataCode = 999, dataSize = sizeof(INT);
    ENCODE_DATA (ptr, &dataCode, &dataSize);
    
    /*
       * write out the record.
    */
    if (exist_offset EQUALS 0)
        {
        *header_offset = nlz_put_record (funit, (UINT)0, (size_t)dataArraySize,
                                         (VOIDPTR)dataArray, control_record);
        }
    else
        {
        *header_offset = nlz_update_record (funit, (UINT)exist_offset,
                                            (size_t)dataArraySize, (VOIDPTR)dataArray,
                                            control_record);
        }
    /*
       * free the allocated memory.
    */
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)dataWidth);
    tc_free ((VOIDPTR)dataType);
    tc_free ((VOIDPTR)units);
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)n_valid_array);
    tc_free ((VOIDPTR)minArray);
    tc_free ((VOIDPTR)maxArray);
    tc_free ((VOIDPTR)sumArray);
    tc_free ((VOIDPTR)sumSquareArray);
    tc_free ((VOIDPTR)findTolArray);
    tc_free ((VOIDPTR)nullValue);
    tc_free ((VOIDPTR)startValueArray);
    tc_free ((VOIDPTR)increArray);
    tc_free ((VOIDPTR)size10_arr);
    tc_free ((VOIDPTR)size32_arr);
    tc_free ((VOIDPTR)dataArray);
#endif
    if (*(INT *)header_offset > 0)
        {
        return SUCCESS;
        }
    else
        {
        return *(INT *)header_offset;
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write a nlist named NAME to the file.  

Prototype:
    static INT nlz_write_nlist (NLIST_HEADER nlist,
        FILENAME in_filename, NLIST_ID in_name, NL_SHARE_MODE share_mode);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    in_filename     -(FILENAME)
    in_name         -(NLIST_ID)
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_FILE_OPEN_ERROR
    NL_LICENSE_EXPIRED
    NL_INVALID_NLIST_NAME
    NL_INVALID_FILENAME
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_write_nlist (NLIST_HEADER nlist, FILENAME in_filename,
                                NLIST_ID in_name, NL_SHARE_MODE share_mode)
#else
static INT nlz_write_nlist (nlist, in_filename, in_name, share_mode)
NLIST_HEADER nlist;
FILENAME in_filename;
NLIST_ID in_name;
NL_SHARE_MODE share_mode;
#endif
    {
    register UINT i;
    size_t totalBytesInEntry = 0, totalBytes = 0;
    INT status;
    FILE_UNIT funit;
    UINT header_offset = 0;
    BOOL exists = FALSE, new_ctrl = FALSE;
    TABLE_HEADER *pTable;
    NLIST_CONTROL_RECORD *control_record;
    UINT allocation = 0;
    FILENAME filename;
    NLIST_ID name;
    
    INIT_TEST (nlist);
    
    if (name EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    if (filename EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    /* load data first, if it is necessary.  */
    for (i = 0; i < nlist->total_tables; i++)
        {
        pTable = nlz_set_table (nlist, i + 1);
        if (pTable == NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        if (pTable->data_loaded EQUALS NL_DATA_NOT_LOADED)
            {
            status = nlz_load_data (nlist);
            if (status IS_NOT_EQUAL_TO SUCCESS)
                {
                return status;
                }
            }
        }
    /* translate the file name first to
        get rid of LOGICAL SYMBOL. */
    ho_translate_filename (in_filename, filename);
    
    /*   change both names to upper case.        */
    
    strcpy ((char *)name, (char *)in_name);
    ts_sto_upper ((CHAR *)name);
    nl_set_nlist_string (nlist, NL_NLIST_NAME, (CHAR *)name);
    nl_set_nlist_string (nlist, NL_NLIST_FILENAME, (CHAR *)filename);
    
    /*   If the n-List has been updated then compute */
    /*   the statistic info.                     */
    
    if (nlist->updated)
        {
        BOOL defer = nlist->defer_stats;
        nlist->defer_stats = FALSE;
        nlz_compute_nlist_stats (nlist);
        nlist->defer_stats = defer;
        }
    /* If it is the first nlist in this file, we need to initialize the file structure,
     * includes Control Record and Directory.
    
      EXISTS = NL_NLIST_NOT_FOUND file exists, n-List does not
         > 0 ->               file exists, n-List exists
         other ->             file open error - make a new one */
    
    exists = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)TRUE, SHARE);
    if (exists > 0)                 /* n-List exists in file */
        {
        
        if ((INT)funit > 0)
            {
            return NL_INVALID_NLIST_NAME;
            }
        }
#ifdef SECURITY
    else if (exists EQUALS NL_LICENSE_EXPIRED)
        {
        return NL_LICENSE_EXPIRED;
        }
#endif
    else if (exists IS_NOT_EQUAL_TO NL_NLIST_NOT_FOUND) /* file does NOT exist - make a new one */
        {
        status = nlh_open (&funit, filename, share_mode, &allocation);
        
        if (status != SUCCESS)
            {
            return NL_FILE_OPEN_ERROR;
            }
        if ((status = nlz_init_ctrl (filename, funit,
                                     share_mode))IS_NOT_EQUAL_TO SUCCESS)
            {
            NL_CLOSE (funit);
            return status;
            }
        }
    else if (exists EQUALS NL_NLIST_NOT_FOUND)
        {
        /* If the specified file exists, but
           the Nlist id does not exist, we
           re-open the file to be the required
           WRITE mode.                  */
        exists = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)TRUE, share_mode);
        }
    /* read the file control record    */
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    /*
       * first, write out all tables.
    */
    for (i = 0; i < nlist->total_tables; i++)
        {
        pTable = nlz_set_table (nlist, i + 1);
        if (pTable == NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        if ((status = nlz_write_table (funit, nlist, pTable, control_record,
                                       (BOOL)TRUE))IS_NOT_EQUAL_TO SUCCESS)
            {
            nlh_close (funit);
            return status;          /*QQQQQ later need to delete those tables have been */
            }
        /* written successfuuly.     */
        if ((status = nlz_write_table_header (funit, nlist, &header_offset,
                                              &totalBytesInEntry, control_record,
                                              (UINT)0)) != SUCCESS)
            {
            nlh_close (funit);
            return status;          /*QQQQQ later need to delete those tables have been */
            }
        nlist->table_elements[i].header_offset = header_offset;
        totalBytes += totalBytesInEntry;
        }
    /*
       * second, write out the header.
    */
    if ((status = nlz_write_header (funit, nlist, (UINT *) & header_offset,
                                    & totalBytesInEntry, control_record,
                                    (UINT)0)) < 0)
        {
        nlh_close (funit);
        return status;
        }
    totalBytes += totalBytesInEntry;
    
    /*
       * update the Dirctory
    */
    status = nlz_update_direct_and_control (funit, filename, name, nlist,
                                            (UINT)header_offset, (size_t)totalBytes,
                                            new_ctrl, (UINT)0, control_record);
    
    nlz_put_control_record ();
    nlh_close (funit);
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write out a specified table to the file.    

Prototype:
    static INT nlz_write_table (FILE_UNIT funit, NLIST_HEADER nlist,
        TABLE_HEADER *pTable, NLIST_CONTROL_RECORD *control_record,
        BOOL to_write);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    nlist           -(NLIST_HEADER)
    pTable          -(TABLE_HEADER *)
    control_record  -(NLIST_CONTROL_RECORD *)
    to_write        -(BOOL) Used to indicate that to write a record or
                        to update an existing record.   

Return Value/Status:
    SUCCESS - Successful completion.
    NL_IO_ERROR7
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_write_table (FILE_UNIT funit, NLIST_HEADER nlist,
                                TABLE_HEADER *pTable,
                                NLIST_CONTROL_RECORD *control_record, BOOL to_write)
#else
static INT nlz_write_table (funit, nlist, pTable, control_record, to_write)
FILE_UNIT funit;
NLIST_HEADER nlist;
TABLE_HEADER *pTable;
NLIST_CONTROL_RECORD *control_record;
BOOL to_write;                      
/*  to indicate that to write a record or to update an existing record. */
/*---------------------------------------*/
/* to indicate that to write a record or */
/* to update an existing record.         */
/*---------------------------------------*/

#endif
    {
    INT nCol = nlist->nCol, nrows = pTable->nrows;
    register INT i;
    INT offset;
    size_t *dataWidth;
    INT *incValue, dataArraySize;
    size_t bytes_to_move;
    BYTE *dataArray, *ptr;
    INT status;
    COLUMN_HEADER *pinfo;
    DATA_HEADER *pdata;
    NLIST_LOGICAL_RECORD *plogicalRec;
    
    if ((NOT to_write)AND (pTable->record_offset >
                           0)AND (pTable->data_loaded IS_NOT_EQUAL_TO NL_DATA_UPDATED))
        {
        return SUCCESS;
        }
    /*
       * if the total of rows is zero, then return.
    */
    if (nrows EQUALS 0)
        {
        if ((to_write)AND (pTable->record_offset EQUALS 0))
            {
            pTable->record_offset = 0;
            return SUCCESS;
            }
        else
            {
            offset = nlz_update_record (funit, (UINT)pTable->record_offset, (UINT)0,
                                        (BYTE *)NULL, control_record);
            }
        }
    /*
       * query  dataWidth, inc_value information.
    */
    i = (nCol + 1) * sizeof(INT);
    dataWidth = (size_t *)TALLOC (i);
    incValue = (INT *)TALLOC (i);
    for (i = 0, pinfo = nlist->info_ptr, pdata = pTable->data_ptr; i < nCol;
         i++, pinfo++, pdata++)
        {
        dataWidth[i] = pinfo->data_size;
        incValue[i] = pdata->inc_value;
        }
    /*
       * count the total length of dataArray.
    */
    for (i = 0, dataArraySize = 0; i < nCol; i++)
        {
        if (NOT incValue[i])
            {
            dataArraySize += dataWidth[i] * nrows;  /* space for rows.   */
            }
        }
    pTable->totalBytesInTable = dataArraySize + sizeof(NLIST_LOGICAL_RECORD);
    /*
       * allocate memory and move data.
    */
    ptr = dataArray = (BYTE *)TALLOC (pTable->totalBytesInTable + 10);
    if (ptr == (BYTE *)0)
        return NL_OVERFLOW;         /* tried to get too much memory from system */
    
    /* Initialize logical record header */
    plogicalRec = (NLIST_LOGICAL_RECORD *)ptr;
    ptr += sizeof(NLIST_LOGICAL_RECORD);
    tc_zeroes ((VOIDPTR)plogicalRec, sizeof(NLIST_LOGICAL_RECORD));
    plogicalRec->bytes = dataArraySize;
    plogicalRec->total_bytes = dataArraySize + (dataArraySize & 1);
    
    for (i = 0, pdata = pTable->data_ptr; i < nCol; i++, pdata++)
        {
        if (NOT incValue[i])
            {
            bytes_to_move = nrows * dataWidth[i];
            hoblockmove ((VOIDPTR)pdata->data, (VOIDPTR)ptr, bytes_to_move);
            ptr += bytes_to_move;
            }
        }
    /*
       * write out the record.
    */
    if (to_write OR pTable->record_offset EQUALS 0)
        /* if the requested function is to WRITE, or
        to update but it is a new table.      */
        {
        offset = nlz_put_record (funit, (UINT)0, pTable->totalBytesInTable,
                                 (VOIDPTR)dataArray, control_record);
        }
    else
        {
        offset = nlz_update_record (funit, (UINT)pTable->record_offset,
                                    (UINT)pTable->totalBytesInTable, (BYTE *)dataArray,
                                    control_record);
        }
    if (offset < 0)
        {
        status = NL_IO_ERROR7;
        }
    else
        {
        pTable->record_offset = offset;
        status = SUCCESS;
        }
#if NEED_TO_FREE EQUALS TRUE
    /*
       * free the allocated memory.
    */
    tc_free ((VOIDPTR)dataArray);
    tc_free ((VOIDPTR)dataWidth);
    tc_free ((VOIDPTR)incValue);
#endif
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write out the header of N_LIST nas tables.   

Prototype:
    static INT nlz_write_table_header (FILE_UNIT funit, NLIST_HEADER nlist,
        UINT *header_offset, size_t *totalBytesInEntry,
        NLIST_CONTROL_RECORD *control_record, UINT exist_offset);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit               -(FILE_UNIT)
    nlist               -(NLIST_HEADER)
    header_offset       -(UINT *)
    totalBytesInEntry   -(size_t *)
    control_record      -(NLIST_CONTROL_RECORD *)
    exist_offset        -(UINT) If the existing offset is zero, means that this header
                            is a new header record, otherwise will be the offset
                            of existing header record for being updated.  

Return Value/Status:
    SUCCESS - Successful completion.
    NL_OVERFLOW
    
Scope:
    PRIVATE to <nl_io_service>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_write_table_header (FILE_UNIT funit, NLIST_HEADER nlist,
                                       UINT *header_offset, size_t *totalBytesInEntry,
                                       NLIST_CONTROL_RECORD *control_record,
                                       UINT exist_offset)
#else
static INT nlz_write_table_header (funit, nlist, header_offset, totalBytesInEntry,
                                       control_record, exist_offset)
FILE_UNIT funit;
NLIST_HEADER nlist;
UINT *header_offset;
size_t *totalBytesInEntry;
NLIST_CONTROL_RECORD *control_record;
UINT exist_offset;
#endif
    {
    register UINT j;
    register size_t dataArraySize = 0;
    INT dataCode;
    size_t dataSize;
    size_t dataCodeSize;
    UINT nCol = nlist->nCol;
    INT *incValue, *validRowArray;
    DOUBLE *minArray, *maxArray, *sumArray, *sumSquareArray, *startValueArray,
        *increArray;
    BYTE *dataArray, *ptr;
    TABLE_HEADER *pTable;
    DATA_HEADER *pdata;
    NLIST_LOGICAL_RECORD *plogicalRec;
    size_t len;
    
    *totalBytesInEntry = 0;
    dataCodeSize = sizeof(INT) * 2; /* space for each DataCode and DataSize*/
    
    /*
       * count the dataSize of  table.
    */
    
    pTable = nlist->current_table_ptr;
    *totalBytesInEntry += pTable->totalBytesInTable;
    /* spaces for DataCode 200, 202, 212 - 214.        INTEGER */
    dataArraySize += (dataCodeSize + sizeof(INT)) * 6;
    
    /* spaces for DataCode 120, 204, 205.                 INTEGER[] */
    dataArraySize += (dataCodeSize + sizeof(INT) * nCol) * 3;
    
    /* spaces for DataCode 206 - 211.                      DOUBLE[] */
    dataArraySize += (dataCodeSize + sizeof(DOUBLE) * nCol) * 6;
    
    /* spaces for DataCode 201, 203.              CHAR[] and BYTE[] */
    if (IS_STRING (pTable->name))
        {
        len = strlen ((char *)pTable->name);
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    if (pTable->user_struct_len IS_NOT_EQUAL_TO 0)
        {
        len = pTable->user_struct_len;
        dataArraySize += dataCodeSize + len + ROUND_UP (len);
        }
    /* space for end of header record.           */
    dataArraySize += dataCodeSize;
    
    /*
       * allocate the space for the dataArray.
    */
    dataArray = ptr = (BYTE *)tc_alloc ((size_t)(dataArraySize +
                                                 sizeof(NLIST_LOGICAL_RECORD) + 1));
    if (ptr == (BYTE *)0)
        return NL_OVERFLOW;         /* tried to get too much memory from system */
    
    /*
       * load the logical record header
    */
    plogicalRec = (NLIST_LOGICAL_RECORD *)ptr;
    tc_zeroes ((VOIDPTR)plogicalRec, sizeof(NLIST_LOGICAL_RECORD));
    ptr += sizeof(NLIST_LOGICAL_RECORD);
    if (exist_offset EQUALS 0)
        {
        plogicalRec->total_bytes = plogicalRec->bytes = dataArraySize;
        }
    dataArraySize += sizeof(NLIST_LOGICAL_RECORD);
    *totalBytesInEntry += dataArraySize;
    
    validRowArray = (INT *)TALLOC ((nCol + 1) * sizeof(INT));
    minArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    maxArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    sumArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    sumSquareArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    startValueArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    increArray = (DOUBLE *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    incValue = (INT *)TALLOC ((nCol + 1) * sizeof(DOUBLE));
    
    /*
       * move the info for the table.
    */
    
    nlz_encode_data ((BYTE **) & ptr, (INT)200, (size_t)sizeof(INT),
                     (VOIDPTR) & nlist->current_table);
    nlz_encode_data ((BYTE **) & ptr, (INT)212, (size_t)sizeof(INT),
                     (VOIDPTR) & pTable->totalBytesInTable);
    nlz_encode_data ((BYTE **) & ptr, (INT)214, (size_t)sizeof(INT),
                     (VOIDPTR) & pTable->record_offset);
    
    if (pTable->user_struct_len IS_NOT_EQUAL_TO 0)
        {
        dataCode = 201, dataSize = pTable->user_struct_len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)pTable->user_struct, (VOIDPTR)ptr,
                     (size_t)pTable->user_struct_len);
        ptr += pTable->user_struct_len + ROUND_UP (pTable->user_struct_len);
        }
    nlz_encode_data ((BYTE **) & ptr, (INT)202, (size_t)sizeof(INT),
                     (VOIDPTR) & pTable->nrows);
    
    if (IS_STRING (pTable->name))
        {
        len = strlen ((char *)pTable->name);
        dataCode = 203, dataSize = len;
        ENCODE_DATA (ptr, &dataCode, &dataSize);
        hoblockmove ((VOIDPTR)pTable->name, (VOIDPTR)ptr, (size_t)len);
        ptr += len + ROUND_UP (len);
        }
    /*
       * if the total number of rows in the table is not greater than zero, skip it.
    */
    if (pTable->nrows > 0)
        for (j = 0, pdata = pTable->data_ptr; j < nCol; j++, pdata++)
            {
            minArray[j] = pdata->min;
            maxArray[j] = pdata->max;
            sumArray[j] = pdata->sum_x;
            sumSquareArray[j] = pdata->sum_x_squared;
            validRowArray[j] = pdata->n_valid_rows;
            }
    for (j = 0, pdata = pTable->data_ptr; j < nCol; j++, pdata++)
        {
        startValueArray[j] = pdata->start_value;
        increArray[j] = pdata->increment;
        incValue[j] = (pdata->inc_value) ? INCREMENT_LIST : VALUE_LIST;
        }
    nlz_encode_data ((BYTE **) & ptr, (INT)120, (size_t)(sizeof(INT) * nCol),
                     (VOIDPTR)incValue);
    nlz_encode_data ((BYTE **) & ptr, (INT)210, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)startValueArray);
    nlz_encode_data ((BYTE **) & ptr, (INT)211, (size_t)(sizeof(DOUBLE) * nCol),
                     (VOIDPTR)increArray);
    
    /*
       * if the total number of rows is not greater than zero, skip it.
    */
    if (pTable->nrows > 0)
        {
        nlz_encode_data ((BYTE **) & ptr, (INT)204, (size_t)(sizeof(INT) * nCol),
                         (VOIDPTR)validRowArray);
        nlz_encode_data ((BYTE **) & ptr, (INT)206, (size_t)(sizeof(DOUBLE) * nCol),
                         (VOIDPTR)minArray);
        nlz_encode_data ((BYTE **) & ptr, (INT)207, (size_t)(sizeof(DOUBLE) * nCol),
                         (VOIDPTR)maxArray);
        nlz_encode_data ((BYTE **) & ptr, (INT)208, (size_t)(sizeof(DOUBLE) * nCol),
                         (VOIDPTR)sumArray);
        nlz_encode_data ((BYTE **) & ptr, (INT)209, (size_t)(sizeof(DOUBLE) * nCol),
                         (VOIDPTR)sumSquareArray);
        }
    dataCode = 999, dataSize = sizeof(INT);
    ENCODE_DATA (ptr, &dataCode, &dataSize);
    /*    *(INT *)ptr = 0;   No space was allocated for this */
    /*
   * write out the record.
*/
    if (exist_offset EQUALS 0)
        {
        *header_offset = nlz_put_record (funit, (UINT)0, (size_t)dataArraySize,
                                         (VOIDPTR)dataArray, control_record);
        }
    else
        {
        *header_offset = nlz_update_record (funit, (UINT)exist_offset,
                                            (UINT)dataArraySize, (BYTE *)dataArray,
                                            control_record);
        }
    /*
       * free the allocated memory.
    */
#if NEED_TO_FREE EQUALS TRUE
    tc_free ((VOIDPTR)incValue);
    tc_free ((VOIDPTR)minArray);
    tc_free ((VOIDPTR)maxArray);
    tc_free ((VOIDPTR)sumArray);
    tc_free ((VOIDPTR)sumSquareArray);
    tc_free ((VOIDPTR)startValueArray);
    tc_free ((VOIDPTR)increArray);
    tc_free ((VOIDPTR)validRowArray);
#endif
    tc_free ((VOIDPTR)dataArray);
    
    if (*header_offset > 0)
        {
        return SUCCESS;
        }
    else
        {
        return *header_offset;
        }
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    Delete the specified nlist by given name.    

Prototype:
    publicdef INT nl_delete_nlist (FILENAME in_filename, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_NOT_FOUND
    NL_INVALID_FILENAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_nlist (FILENAME in_filename, NLIST_ID name)
#else
publicdef INT nl_delete_nlist (in_filename, name)
FILENAME in_filename;
NLIST_ID name;
#endif
    {
    FILENAME filename;
    INT headerOffset, status;
    
    /*
       * open a file.
    */
    if (in_filename EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    /* translate the file name first to
        get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return NL_FILE_NOT_FOUND;
        }
    ts_sto_upper ((CHAR *)name);
    
    /*
       * check the existence of nlist named NAME.
    */
    headerOffset = nlz_delete_exist_nlist (filename, name);
    if (headerOffset < 0)
        {
        return headerOffset;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    A help routine to recover the deleted files.

Prototype:
    publicdef INT nl_delete_recover (FILENAME in_filename, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NLIST_NOT_FOUND
    NL_FILE_OPEN_ERROR
    NL_INVALID_FILENAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_delete_recover (FILENAME in_filename, NLIST_ID name)
#else
publicdef INT nl_delete_recover (in_filename, name)
FILENAME in_filename;
NLIST_ID name;
#endif
    {
    FILENAME filename;
    FILE_UNIT funit;
    INT headerOffset;
    INT status;
    UINT allocation = 0;
    
    /*
       * open a file.
    */
    if (in_filename EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    ho_translate_filename (in_filename, filename);
    ts_sto_upper ((CHAR *)name);
    
    status = nlh_open (&funit, filename, WRITE, &allocation);
    
    if (status != SUCCESS)
        {
        return NL_FILE_OPEN_ERROR;
        }
    /*
       * check the existence of nlist named NAME.
    */
    headerOffset = nlz_delete_recover_nlist (funit, name);
    NL_CLOSE (funit);
    if (headerOffset < 0)
        {
        return NL_NLIST_NOT_FOUND;
        }
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read a nlist named NAME to the file.   

Prototype:
    publicdef INT nl_read_nlist (NLIST_HEADER *nlist, FILENAME filename, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)
    filename        -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    Function returns status from nlz_read_nlist.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_read_nlist (NLIST_HEADER *nlist, FILENAME filename, NLIST_ID name)
#else
publicdef INT nl_read_nlist (nlist, filename, name)
NLIST_HEADER *nlist;
FILENAME filename;
NLIST_ID name;
#endif
    {
    return nlz_read_nlist (nlist, filename, name, SHARE);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read a nlist named NAME to the file. 

Prototype:
    publicdef INT nl_read_nlist_exclusive (NLIST_HEADER *nlist, FILENAME filename, 
        NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER *)
    filename        -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    Function returns status from nlz_read_nlist.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_read_nlist_exclusive (NLIST_HEADER *nlist, FILENAME filename,
                                       NLIST_ID name)
#else
publicdef INT nl_read_nlist_exclusive (nlist, filename, name)
NLIST_HEADER *nlist;
FILENAME filename;
NLIST_ID name;
#endif
    {
    return nlz_read_nlist (nlist, filename, name, NOSHARE);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update an exsiting nlist named NAME to the file.  

Prototype:
    publicdef INT nl_update_nlist (NLIST_HEADER nlist);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_INVALID_TABLE
    NL_INVALID_FILENAME
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_update_nlist (NLIST_HEADER nlist)
#else
publicdef INT nl_update_nlist (nlist)
NLIST_HEADER nlist;
#endif
    {
    size_t totalBytesInEntry, totalBytes;
    INT status;
	UINT i;
    FILE_UNIT funit;
    UINT header_offset = 0;
    TABLE_HEADER *pTable;
    NLIST_CONTROL_RECORD *control_record;
    INT exist_dir_offset, exist_data_offset;
    FILENAME filename;
    NLIST_ID name;
    
    INIT_TEST (nlist);
    tc_zeroes ((VOIDPTR)name, sizeof(NLIST_ID));
    
    strcpy ((char *)name, (char *)nlist->name);
    strcpy ((char *)filename, (char *)nlist->data_filename);
    if (IS_EMPTY_STRING (name))
        {
        return NL_INVALID_FILENAME;
        }
    /*
       * check the existance of specified n_list.
    */
    exist_dir_offset = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)TRUE, SHARE);
    
    if (exist_dir_offset < 0 OR (INT)funit <= 0)
        {
        return exist_dir_offset;
        }
    exist_data_offset = nlz_chk_nlist_exist (filename, name, &funit, (BOOL)FALSE,
                                             WRITE);
    
    /*   If the nl_list has been updated, then compute */
    /*   the statistic info.                     */
    
    if (nlist->updated)
        {
        BOOL defer = nlist->defer_stats;
        nlist->defer_stats = FALSE;
        nlz_compute_nlist_stats (nlist);
        nlist->defer_stats = defer;
        }
    /* read the file control record    */
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    totalBytes = 0;
    /*
       * first, write out all tables.
    */
    for (i = 0; i < nlist->total_tables; i++)
        {
        pTable = nlz_set_table (nlist, i + 1);
        if (pTable == NULL_TABLE)
            {
            return NL_INVALID_TABLE;
            }
        if ((status = nlz_write_table (funit, nlist, pTable, control_record,
                                       FALSE))IS_NOT_EQUAL_TO SUCCESS)
            {
            nlh_close (funit);
            return status;          /*QQQQQ later need to delete those tables have been */
            }
        /* written successfully.     */
        if (nlist->table_elements[i].ptable != NULL_TABLE)
            {
            if ((status = nlz_write_table_header (funit, nlist, &header_offset,
                                                  &totalBytesInEntry, control_record,
                                                  nlist->table_elements[i].
                                                  header_offset))IS_NOT_EQUAL_TO 
                          SUCCESS)
                {
                nlh_close (funit);
                return status;      /*QQQQQ later need to delete those tables have been */
                }
            nlist->table_elements[i].header_offset = header_offset;
            totalBytes += totalBytesInEntry;
            }
        }
    /*
       * second, write out the header.
    */
    if ((status = nlz_write_header (funit, nlist, (UINT *) & header_offset,
                                    & totalBytesInEntry, control_record,
                                    (UINT)exist_data_offset)) < 0)
        {
        nlh_close (funit);
        return status;
        }
    totalBytes += totalBytesInEntry;
    
    /*
       * update the Dirctory
    */
    status = nlz_update_direct_and_control (funit, filename, name, nlist,
                                            (UINT)header_offset, totalBytes,
                                            (BOOL)FALSE, (UINT)exist_dir_offset,
                                            control_record);
    
    nlz_put_control_record ();
    nlh_close (funit);
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write a nlist named NAME to the file. 

Prototype:
    publicdef INT nl_write_nlist (NLIST_HEADER nlist, FILENAME in_filename, NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    in_filename     -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    Function returns status from nlz_write_nlist.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_write_nlist (NLIST_HEADER nlist, FILENAME in_filename, NLIST_ID name)
#else
publicdef INT nl_write_nlist (nlist, in_filename, name)
NLIST_HEADER nlist;
FILENAME in_filename;
NLIST_ID name;
#endif
    {
    return nlz_write_nlist (nlist, in_filename, name, WRITE);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To write a nlist named NAME to the file.  

Prototype:
    publicdef INT nl_write_nlist_exclusive (NLIST_HEADER nlist, FILENAME in_filename, 
        NLIST_ID name);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    nlist           -(NLIST_HEADER)
    in_filename     -(FILENAME)
    name            -(NLIST_ID)

Return Value/Status:
    Function returns status from nlz_write_nlist.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_write_nlist_exclusive (NLIST_HEADER nlist, FILENAME in_filename,
                                        NLIST_ID name)
#else
publicdef INT nl_write_nlist_exclusive (nlist, in_filename, name)
NLIST_HEADER nlist;
FILENAME in_filename;
NLIST_ID name;
#endif
    {
    return nlz_write_nlist (nlist, in_filename, name, WRITE_NOSHARE);
    }
/* END:     */
