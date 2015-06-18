/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_FILE.C*/
/* *6    25-OCT-1990 12:20:52 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    20-AUG-1990 17:09:05 KEE "(SPR -1) fixing compile error at sun"*/
/* *4    25-JUL-1990 14:55:54 VINCE "(SPR 5644) Header Standardization"*/
/* *3    23-MAR-1990 16:17:11 GILLESPIE "(SPR 1) Change defer stats flag"*/
/* *2    12-MAR-1990 19:06:06 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:57:30 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_CLN_FILE.C*/
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
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    This function cleans the file, when the deleted space is more than some 
    % of whole file.

Prototype:
    publicdef INT nl_clean_file (FILENAME in_filename, DOUBLE percent);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) in_filename -(FILENAME)
    (I) percent     -(DOUBLE)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NLIST_NOT_FOUND
    NL_FILE_OPEN_ERROR
    NL_FILE_NOT_FOUND
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_clean_file (FILENAME in_filename, DOUBLE percent)
#else
publicdef INT nl_clean_file (in_filename, percent)
FILENAME in_filename;
DOUBLE percent;
#endif
    {
    NLIST_HEADER nlist;
    PATHNAME tpathname;
    PATHNAME temp_file_name;
    FILE_UNIT funit;
    FILENAME filename;
    size_t nbytes;
    UINT nallocated = 0;
    UINT deleted_record_bytes = 0;
    UINT totalNlists;
    INT status;
    INT return_status = SUCCESS;
    register INT i, j;
    DOUBLE result;
    TCP nlistTable;
    NLIST_CONTROL_RECORD *control_record;
    NLIST_LOGICAL_RECORD logical_record;
    NL_SHARE_MODE share_mode;
    BYTE *user_struct_ptr;
    INT last_value;
    
    /* translate the filename first to
        get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return(NL_FILE_NOT_FOUND);
        }
    /*
       * open the specified file.
    */
    
    status = nl_inq_nlist_names (filename, &totalNlists, &nlistTable);
    if ((
    status EQUALS NL_NLIST_NOT_FOUND AND totalNlists EQUALS 0) OR (
                                                             status EQUALS SUCCESS AND 
                                                                 totalNlists EQUALS 0))
        {
        nl_close_nlist_file (filename);
        ho_delete_file (filename, &status);
        return SUCCESS;
        
        }
    if (status IS_NOT_EQUAL_TO SUCCESS)
        {
        return status;
        }
    if (nlz_get_read_funit (filename, &funit) < 0)
        {
        return NL_FILE_OPEN_ERROR;
        }
    /*
       * check the percentage of being deleted bytes.
    */
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        ts_tcp_free (nlistTable);
        return status;
        }
    /* calculate deleted record bytes. */
    if (control_record->deleted_record_offset > 0)
        {
        nlh_seek (funit, control_record->deleted_record_offset, SEEK_SET);
        nlh_read (funit, &logical_record, sizeof logical_record);
        deleted_record_bytes += (logical_record.total_bytes + 20);
        while (logical_record.next_offset > 0)
            {
            nlh_seek (funit, logical_record.next_offset, SEEK_SET);
            nlh_read (funit, (VOIDPTR)&logical_record, sizeof logical_record);
            deleted_record_bytes += (logical_record.total_bytes + 20);
            }
        }
    result = control_record->bytes_deleted + deleted_record_bytes;
    result = result / control_record->next_write_offset;
    if (result < percent)
        {
        ts_tcp_free (nlistTable);
        return SUCCESS;
        }
    /* Now create the new nlist file name by keeping the path of the old and changing
       the name to a temporary name */
    
    strcpy ((char *)tpathname, (char *)filename);
    ho_split_pathname (tpathname, tpathname, (CHAR *)NULL);
    ho_get_unique_filename (temp_file_name);
    ho_assemble_pathname (tpathname, tpathname, temp_file_name);
    
    share_mode = WRITE_NOSHARE;
    status = nlh_open (&funit, tpathname, share_mode, &nallocated);
    if ((status = nlz_init_ctrl (tpathname, funit, WRITE))IS_NOT_EQUAL_TO SUCCESS)
        {
        NL_CLOSE (funit);
        return status;
        }
    nlh_close (funit);
    
    /*
       * start to read each nlist, then copy to temp. file.
    */
    
    last_value = nlz_set_block_entries (totalNlists + 10);
    for (i = 0; i < totalNlists; i++)
        {
        status = nl_read_nlist_exclusive (&nlist, filename, nlistTable[i]);
        for (j = 0; j < nlist->total_tables; j++)
            {
            status = nl_set_current_row (nlist, 1, j + 1);
            if (status > 0)
                {
                status = nlz_load_data (nlist);
                if (status IS_NOT_EQUAL_TO SUCCESS)
                    {
                    /*  A problem was detected: cannot read this
                        n-List.  Skip and report as incomplete
                        status                      */
                    
                    return_status = NL_NLIST_NOT_FOUND;
                    break;
                    }
                nlist->current_table_ptr->data_loaded = NL_DATA_UPDATED;
                nlist->current_table_ptr->updated = TRUE;
                }
            }
        if (status EQUALS SUCCESS)
            {
            /* Force recompute of statistics */
            nlist->defer_stats = FALSE;
            nlist->updated = TRUE;
            status = nl_write_nlist_exclusive (nlist, tpathname, nlistTable[i]);
            }
        nl_free_nlist (nlist);
        }
    if ((status = nlz_get_read_funit (filename, &funit)) < 0
	OR 
	(status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    if (control_record->file_user_struct_offset > 0)
        {
        status = nlz_get_record (funit, (UINT)control_record->file_user_struct_offset,
                                 (VOIDPTR *) & user_struct_ptr, (UINT *) & nbytes,
                                 (BOOL)FALSE);
        
        if (status >= 0 AND nbytes > 0)
            {
            status = nl_set_file_user_struct (tpathname, (VOIDPTR)user_struct_ptr,
                                              (size_t)nbytes);
            tc_free ((VOIDPTR)user_struct_ptr);
            }
        }
    /* set the size of dir. block back*/
    /* to NL_DEAULT_ENTRY_IN_BLOCK.   */
    
    nlz_set_block_entries (last_value);
    
    /*  close the opening file              */
    ts_tcp_free (nlistTable);
    
    /*  return_status is not SUCCESS if any n-List  */
    /*  was incorrectly accessed            */
    
    if (return_status EQUALS SUCCESS)
        {
        nl_delete_nlist_file (filename);
        status = nlz_rename_index_file (tpathname, filename);
        }
    return return_status;
    }
/* END:     */
