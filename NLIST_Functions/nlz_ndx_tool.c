/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_NDX_TOOL.C*/
/* *8    19-JUL-1991 14:14:06 JULIAN "(SPR 0) fixed crashes when reading nlist on VMS"*/
/* *7    17-JUL-1991 11:48:15 GILLESPIE "(SPR 3317) Add call to freopen when control record read - unix shared access"*/
/* *6    25-OCT-1990 12:19:18 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *5    25-JUL-1990 14:53:34 VINCE "(SPR 5644) Header Standardization"*/
/* *4    12-APR-1990 14:35:48 GILLESPIE "(SPR 1) Add setting of status in internal routine"*/
/* *3    19-MAR-1990 16:08:50 GILLESPIE "(SPR 1) Fix nlz_chk_exist_index - set *pfunit on failure, return correct status"*/
/* *2    12-MAR-1990 19:04:43 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *1     5-MAR-1990 10:56:29 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLZ_NDX_TOOL.C*/
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

#include "esi_time.h"
static NL_FILE_DATA *nlist_files[NL_MAX_INDEX];
static UINT totalFiles = 0;

/*begin of func declaration */

#if USE_PROTOTYPES
static INT nlz_construct_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                    NL_SHARE_MODE share_mode, FILE_UNIT *pfunit);
static INT nlz_read_control_record (FILE_UNIT funit,
                                        NLIST_CONTROL_RECORD **pcontrol);
static INT nlz_construct_index_v3 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);
static INT nlz_construct_index_v4 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);

#else
static INT nlz_construct_index ();
static INT nlz_read_control_record ();
static INT nlz_construct_index_v3 ();
static INT nlz_construct_index_v4 ();

#endif
/* File Description ---------------------------------------------------------
Overview:
    This is a module of internal help routines for NLIST I/O.  
    Originally, There are some B-Tree structure designed to speed   
    up the searching of exsiting nlists in a specific Nlist file,   
    now we convert them to be the HASH TABLE structure, but most of 
    varible names and routines still use the old one, e.g. XX_BTXX_X
    and xxbtxx.                                                     

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT nlz_get_file_data (FILE_UNIT funit, NL_FILE_DATA **dataptr);
    
    publicdef INT nlz_init_ctrl (FILENAME filename, FILE_UNIT funit,
                             NL_SHARE_MODE share_mode);
    
    publicdef INT nlz_put_control_record (void);
    
    publicdef INT nlz_rename_index_file (FILENAME old_file_name,
                                     FILENAME new_file_name);
    
    publicdef INT nlz_get_control_record (FILE_UNIT funit,
                                      NLIST_CONTROL_RECORD **pcontrol);
    
    publicdef INT nlz_update_index_item (FILENAME filename, NLIST_ID idName);
    
    publicdef INT nlz_update_index (FILENAME filename, NLIST_ID idName,
                                UINT directory_offset, UINT offset);
    
    publicdef INT nlz_delete_index (INT target);
    
    publicdef INT nlz_chk_exist_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                   NL_SHARE_MODE share_mode,
                                   FILE_UNIT *pfunit);
    
    publicdef INT nlz_get_read_funit (FILENAME filename, FILE_UNIT *pfunit);
    
    publicdef INT nlz_get_target (FILENAME filename);
    
    publicdef INT nlz_build_nlist_table (FILENAME filename, UINT *totalNlists,
                                     TCP *nlistTable,
                                     NL_SHARE_MODE share_mode,
                                     FILE_UNIT *pfunit);

Private_Functions:
    static INT nlz_construct_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                    NL_SHARE_MODE share_mode,
                                    FILE_UNIT *pfunit);

    static INT nlz_read_control_record (FILE_UNIT funit,
                                        NLIST_CONTROL_RECORD **pcontrol);

    static INT nlz_construct_index_v3 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);

    static INT nlz_construct_index_v4 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);


-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    Build the table of n-List names for the existing nlists in the file. 

Prototype:
    publicdef INT nlz_build_nlist_table (FILENAME filename, UINT *totalNlists,
                                     TCP *nlistTable,
                                     NL_SHARE_MODE share_mode,
                                     FILE_UNIT *pfunit);
                                     
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    totalNlists     -(UINT *)
    nlistTable      -(TCP *)
    share_mode      -(NL_SHARE_MODE)
    pfunit          -(FILE_UNIT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_build_nlist_table (FILENAME filename, UINT *totalNlists,
                                     TCP *nlistTable, NL_SHARE_MODE share_mode,
                                     FILE_UNIT *pfunit)
#else
publicdef INT nlz_build_nlist_table (filename, totalNlists, nlistTable, share_mode,
                                     pfunit)
FILENAME filename;
UINT *totalNlists;
TCP *nlistTable;
NL_SHARE_MODE share_mode;
FILE_UNIT *pfunit;
#endif
    {
    INT status;
    register INT i;
    UINT total_items;
    TCP table;
    HASH_TABLE_P file_tree;
    NL_INDEX_DATA *dataPtr, *oldDataPtr;
    NLIST_ID *idPtr, *oldIdPtr;
    
    *totalNlists = 0;
    *nlistTable = table = (TCP)0;
    
    /* constructs or retrieves the file  */
    /* B-TREE                            */
    status = nlz_chk_exist_index (filename, &file_tree, share_mode, pfunit);
    
    if (status < 0)
        {
        return status;
        }
    /*    inquire the total existing nlists.        */
    status = ht_get_all (file_tree, &total_items, (VOIDPTR *)&idPtr,
                         (VOIDPTR *)&dataPtr);
    oldIdPtr = idPtr;
    oldDataPtr = dataPtr;
    
    if (total_items <= 0)
        {
        return status;
        }
    *nlistTable = table = TCP_ALLOCATE (total_items);
    for (i = 0, *totalNlists = 0; i < total_items; i++, dataPtr++, idPtr++)
        {
        if (dataPtr->delete)
            {
            continue;
            }
        table[*totalNlists] = (CHAR *) ALLOCATE (NLIST_ID);
        strcpy ((char *)table[*totalNlists], (char *)idPtr);
        *totalNlists += 1;
        }
    tc_free ((VOIDPTR)oldIdPtr);
    tc_free ((VOIDPTR)oldDataPtr);
    table[*totalNlists] = (CHAR *)NULL;
    
    return(status >= 0) ? SUCCESS : status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_get_target (FILENAME filename);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)

Return Value/Status:
    NL_NLIST_NOT_FOUND
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_target (FILENAME filename)
#else
publicdef INT nlz_get_target (filename)
FILENAME filename;
#endif
    {
    register UINT i;
    INT target = NL_NLIST_NOT_FOUND;
    /* go througth all files and compare
    then with the requested filename.   */
    
    for (i = 0; i < totalFiles; i++)
        {
        if (ARE_SAME ((char *)filename, (char *)nlist_files[i]->filename))
            {
            target = i;
            break;
            }
        }
    /* if the file has been located AND
    it is not the top of the list,
    make it the top of the list,
    forming a LRU stack After the
    following code, the FOUND entry
    will be at offset 0 (hence target
    is always zero on return for a
    found entry) */
    
    if (target > 0)
        {
        NL_FILE_DATA *save_file;
        
        save_file = nlist_files[target];
        for (i = target; i > 0; i--)
            {
            nlist_files[i] = nlist_files[i - 1];
            }
        nlist_files[0] = save_file;
        target = 0;
        }
    /* increment the access counter -
    may use later for more
    intelligent file management       */

#if 0
    if (target EQUALS 0)
        {
        nlist_files[target]->accesses++;
        }
#endif
    return target;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_get_read_funit (FILENAME filename, FILE_UNIT *pfunit);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    pfunit          -(FILE_UNIT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_read_funit (FILENAME filename, FILE_UNIT *pfunit)
#else
publicdef INT nlz_get_read_funit (filename, pfunit)
FILENAME filename;
FILE_UNIT *pfunit;
#endif
    {
    INT target;
    INT status = SUCCESS;
    NLIST_CONTROL_RECORD *control_record;
    NL_SHARE_MODE share_mode;
    HASH_TABLE_P IndexPtr;
    
    target = nlz_get_target (filename);
    
    if (target < 0)
        {
        return nlz_construct_index (filename, &IndexPtr, SHARE, pfunit);
        }
    *pfunit = nlist_files[target]->funit;
    /* if this file has the share_mode
    flag set, then the file is
    private and not subject to
    sharing.  If the file is not
    shared, then there is no need to
    check for consistency              */
    if (nlist_files[target]->share_mode EQUALS SHARE)
        {
        /*    if found check the modified date, if the
            modified date is same between file and
            Index, return Index header.  if not,
            re-construct B-tree.                */
        
        if ((status = nlz_get_control_record (*pfunit, &control_record)) < 0)
            {
            *pfunit = (FILE_UNIT)0;
            return status;
            }
        if (ARE_DIFFERENT ((char *)control_record->modified_date,
                           (char *)nlist_files[target]->modified_date))
            {
            /*          delete the index and create a up to
                      dateone.                    */
            
            share_mode = nlist_files[target]->share_mode;
            nlz_delete_index ((UINT)target);
            status = nlz_construct_index (filename, &IndexPtr, share_mode, pfunit);
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read the specified file and create a B-tree structure of ids in the
    memory.  Returns the file unit number opened by this routine;
    any value less than zero is an error

Prototype:
    static INT nlz_construct_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                    NL_SHARE_MODE share_mode,
                                    FILE_UNIT *pfunit);
                                    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    IndexPtr        -(HASH_TABLE_P *)
    share_mode      -(NL_SHARE_MODE)
    pfunit          -(FILE_UNIT *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PRIVATE to <nlz_ndx_tool>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_construct_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                    NL_SHARE_MODE share_mode, FILE_UNIT *pfunit)
#else
static INT nlz_construct_index (filename, IndexPtr, share_mode, pfunit)
FILENAME filename;
HASH_TABLE_P *IndexPtr;
NL_SHARE_MODE share_mode;
FILE_UNIT *pfunit;
#endif
    {
    NLIST_CONTROL_RECORD *control_record;
    INT status;
    UINT allocation = 0;            /* initial file allocation */
    
    status = nlh_open (pfunit, filename, share_mode, &allocation);
    if (status != SUCCESS)
        {
        return NL_FILE_OPEN_ERROR;
        }
    /* read the control record.          */
    
    if ((status = nlz_read_control_record (*pfunit, &control_record)) < 0)
        {
        *IndexPtr = (HASH_TABLE_P)NULL;
        return status;
        }
    if ((control_record->
         file_mark IS_NOT_EQUAL_TO NL_FILE_MARK) AND (control_record->
                                                      file_mark IS_NOT_EQUAL_TO 0))
        {
        *IndexPtr = (HASH_TABLE_P)NULL;
        return NL_INVALID_FILENAME;
        }
    status = SUCCESS;
    /*    if(control_record ->total_nlists EQUALS 0)
            {
             *IndexPtr = (HASH_TABLE_P)NULL;
             return NL_NLIST_NOT_FOUND;
            }
    */
    if (ARE_SAME (control_record->version_stamp, NL_VERSION_4))
        {
        status = nlz_construct_index_v4 (filename, *pfunit, control_record, IndexPtr,
                                         share_mode);
        }
    else
        {
        status = nlz_construct_index_v3 (filename, *pfunit, control_record, IndexPtr,
                                         share_mode);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To check the existance of index of requested B-tree, if it has been
    loaded, returns the address of B-tree, otherwise returns FAIL.

Prototype:
    publicdef INT nlz_chk_exist_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                   NL_SHARE_MODE share_mode,
                                   FILE_UNIT *pfunit);
                                   
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    IndexPtr        -(HASH_TABLE_P *)
    share_mode      -(NL_SHARE_MODE)
    pfunit          -(FILE_UNIT *)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_chk_exist_index (FILENAME filename, HASH_TABLE_P *IndexPtr,
                                   NL_SHARE_MODE share_mode, FILE_UNIT *pfunit)
#else
publicdef INT nlz_chk_exist_index (filename, IndexPtr, share_mode, pfunit)
FILENAME filename;
HASH_TABLE_P *IndexPtr;
NL_SHARE_MODE share_mode;
FILE_UNIT *pfunit;
#endif
    {
    INT status;                     /* returned status.  index.  flag of */
    INT target;                     /* finding result.                  */
    UINT allocation = 0;
    FILE_UNIT funit1;
    NLIST_CONTROL_RECORD *control_record;
    
    target = nlz_get_target (filename);
    if (target < 0)
        {
        return nlz_construct_index (filename, IndexPtr, share_mode, pfunit);
        }
    *pfunit = nlist_files[target]->funit;
    /* if found check the modified date,
    if the modified date is same
    between file and Index, return
    Index header.  if not,
    re-construct B-tree.              */
    
    if ((status = nlz_get_control_record (*pfunit, &control_record)) < 0)
        {
        return status;
        }
    if (ARE_DIFFERENT (control_record->modified_date,
                       nlist_files[target]->modified_date))
        {
        /*   delete the index and create a
         up to date one.                 */
        
        nlz_delete_index ((UINT)target);
        status = nlz_construct_index (filename, IndexPtr, share_mode, pfunit);
        if (status < 0)
            {
            return status;
            }
        }
    else if ((INT)share_mode > (INT)nlist_files[target]->share_mode)
        {
        /* the file mode has changed, either
        from READONLY to SHARE or SHARE
        to READONLY.  Close the file as
        it now exists and reopen it in
        the new mode.                     */
        
        NL_CLOSE (*pfunit);
        if (share_mode EQUALS WRITE)
            {
            share_mode = (nlist_files[target]->share_mode EQUALS SHARE) ?
                WRITE : WRITE_NOSHARE;
            }
        status = nlh_open (pfunit, filename, share_mode, &allocation);
        /* if the open in the new mode has
        failed (usually because a
        READONLY copy had been requested
        and someone else has it open for
        shared access), then put the file
        back the way it was.  And returns 
        the error status.            */
        
        if (status != SUCCESS)
            {
            status = nlh_open (&funit1, filename, nlist_files[target]->share_mode,
                               &allocation);
            *pfunit = nlist_files[target]->funit = funit1;
            }
        else
            {
            nlist_files[target]->share_mode = share_mode;
            nlist_files[target]->funit = *pfunit;
            }
        }
    *IndexPtr = nlist_files[target]->idIndex;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read the specified file and create a B-tree structure of ids in the
    memory.

Prototype:
    static INT nlz_construct_index_v3 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);
                                       
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    funit           -(FILE_UNIT)
    pcontrol        -(NLIST_CONTROL_RECORD *)
    IndexPtr        -(HASH_TABLE_P *)
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <nlz_ndx_tool>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_construct_index_v3 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode)
#else
static INT nlz_construct_index_v3 (filename, funit, pcontrol, IndexPtr, share_mode)
FILENAME filename;
FILE_UNIT funit;
NLIST_CONTROL_RECORD *pcontrol;
HASH_TABLE_P *IndexPtr;
NL_SHARE_MODE share_mode;
#endif
    {
    BYTE *ptr, *record;
    INT status;
    UINT offset;
    size_t length;
    UINT i;
    INT next_offset;                /* This can't be unsigned */
    BOOL end = FALSE, done = FALSE;
    INT data_type;
    size_t data_size;
    size_t dataCodeSize = 2 * sizeof(INT);
    UINT num_of_buckets = 0;
    NLIST_ID key;
    NL_INDEX_DATA data;
    NL_FILE_DATA *curFile;
    /* ************************************************************************* */
    /* create a new files.               */
    
    if (totalFiles EQUALS NL_MAX_INDEX)
        {
        nlz_delete_index (totalFiles - 1);
        }
    for (i = totalFiles; i > 0; i--)
        {
        nlist_files[i] = nlist_files[i - 1];
        }
    nlist_files[0] = curFile = ALLOCATE (NL_FILE_DATA);
    totalFiles++;
    curFile->funit = funit;
    
    /* read the file directory.          */
    num_of_buckets = (pcontrol->total_nlists > 10) ? pcontrol->total_nlists / 5 : 10;
    status = ht_init_hash_table (num_of_buckets, (UINT)10, HT_CHAR_KEY,
                                 sizeof(NLIST_ID), sizeof(NL_INDEX_DATA),
                                 HT_SYSTEM_TABLE, &curFile->idIndex);
    
    *IndexPtr = curFile->idIndex;
    hoblockmove ((VOIDPTR)pcontrol->modified_date, (VOIDPTR)curFile->modified_date,
                 (size_t)sizeof(DATE_TIME));
    strcpy ((char *)curFile->filename, (char *)filename);
    curFile->share_mode = share_mode;
    
    offset = pcontrol->directory_offset;
    status = SUCCESS;
    /* If the nlist file directory is 0,
        means the nlist file is empty,
        return.                         */
    if (offset EQUALS 0)
        {
        return SUCCESS;
        }
    do
        {
        next_offset = nlz_get_record (funit, offset, (VOIDPTR *)&record, &length,
                                      TRUE);
        if ((INT)next_offset < 0)
            {
            return next_offset;
            }
        data.directory_offset = offset;
        if (next_offset EQUALS 0)
            end = TRUE;
        else
            offset = next_offset;
        ptr = record;
        DECODE_DATA (ptr, &data_type, &data_size);
        for (i = 0, done = FALSE; i + dataCodeSize + data_size <= length AND!done; )
            {
            
            /*
            * looking for datacode 300 --- start of nlist.
            */
            switch (data_type)
                {
            case 102:
                tc_zeroes ((VOIDPTR)key, (size_t)sizeof(NLIST_ID));
                strcpy ((char *)key, (char *)ptr);
                break;
            case 888:
                done = TRUE;
                break;
            case 118:
                hoblockmove ((VOIDPTR)ptr, (VOIDPTR)&data.offset, (size_t)sizeof(INT));
                data.delete = FALSE;
                ht_insert (curFile->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
                done = TRUE;
                break;
                
            case 300:
            case 301:
            case 101:
            case 105:
            case 107:
                break;
                /*   if there is unorganized data    */
                /*   type, get out the loop.         */
                
            default:
                done = TRUE;
                }
            if (data_size <= 0)
                done = TRUE;
            i += dataCodeSize + data_size;
            ptr += data_size;
            if (i < length AND!done)
                DECODE_DATA (ptr, &data_type, &data_size);
            }
        tc_free ((VOIDPTR)record);
        } while (!end);
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To read the specified file and create a B-tree structure of ids in the
    memory.

Prototype:
    static INT nlz_construct_index_v4 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode);
                                       
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    funit           -(FILE_UNIT)
    pcontrol        -(NLIST_CONTROL_RECORD *)
    IndexPtr        -(HASH_TABLE_P *)
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PRIVATE to <nlz_ndx_tool>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_construct_index_v4 (FILENAME filename, FILE_UNIT funit,
                                       NLIST_CONTROL_RECORD *pcontrol,
                                       HASH_TABLE_P *IndexPtr,
                                       NL_SHARE_MODE share_mode)
#else
static INT nlz_construct_index_v4 (filename, funit, pcontrol, IndexPtr, share_mode)
FILENAME filename;
FILE_UNIT funit;
NLIST_CONTROL_RECORD *pcontrol;
HASH_TABLE_P *IndexPtr;
NL_SHARE_MODE share_mode;
#endif
    {
    BYTE *ptr;
    INT status;
    UINT offset, next_offset, length, i, j;
    BOOL end = FALSE, done = FALSE;
    INT data_type;
    size_t data_size;
    size_t dataCodeSize = 2 * sizeof(INT);
    UINT total_dirs;
    UINT num_of_buckets;
    UINT offset_of_first_entry;
    NLIST_ID key;
    NL_INDEX_DATA data;
    NL_FILE_DATA *curFile;
    DIRECTORY_ENTRY *dirs;
    /* ************************************************************************* */
    /* create a new files.               */
    /* if the max number of open files
 has been hit, get rid of the LAST
 file in the list, which is the
 least recently used (LRU) file    */
    
    if (totalFiles EQUALS NL_MAX_INDEX)
        {
        nlz_delete_index (totalFiles - 1);
        }
    /* shift all existing files down a   */
    /* notch                             */
    for (i = totalFiles; i > 0; i--)
        {
        nlist_files[i] = nlist_files[i - 1];
        }
    nlist_files[0] = curFile = ALLOCATE (NL_FILE_DATA);
    totalFiles++;
    curFile->funit = funit;
    
    /* read the file directory.          */
    num_of_buckets = (pcontrol->total_nlists > 10) ? pcontrol->total_nlists / 5 : 10;
    
    if ((status = ht_init_hash_table (num_of_buckets, (UINT)10, HT_CHAR_KEY,
                                      sizeof(NLIST_ID), sizeof(NL_INDEX_DATA),
                                      HT_SYSTEM_TABLE, &curFile->idIndex)) < 0)
        {
        return status;
        }
    *IndexPtr = curFile->idIndex;
    /* copy the time stamp from the      */
    /* control record                    */
    hoblockmove ((VOIDPTR)pcontrol->modified_date, (VOIDPTR)curFile->modified_date,
                 (size_t)sizeof(DATE_TIME));
    /* copy the control record           */
    hoblockmove ((VOIDPTR)pcontrol, (VOIDPTR)&curFile->control_record,
                 (size_t)sizeof(NLIST_CONTROL_RECORD));
    /* copy the file name                */
    strcpy ((char *)curFile->filename, (char *)filename);
    /* copy the share mode               */
    curFile->share_mode = share_mode;
    
    offset = pcontrol->directory_offset;
    /* If the file directory offset is 0,
        means there is no nlist in the file.
        just return the funits. */
    status = SUCCESS;
    if (offset EQUALS 0)
        return status;
    do                              /* build the file directory          */
        {
        next_offset = nlz_get_directories (funit, offset, &dirs, &total_dirs);
        if ((INT)next_offset < 0)
            {
            return(INT)next_offset;
            }
        /* set the offset of the first entry */
        /* in the dir. block.                */
        
        offset_of_first_entry = offset + 20;
        if (next_offset EQUALS 0)
            end = TRUE;
        else
            offset = next_offset;
        length = sizeof(DIRECTORY_ENTRY);
        for (j = 0; j < total_dirs; j++)
            {
            data.directory_offset = offset_of_first_entry + j * length;
            ptr = (BYTE *)(dirs + j);
            ptr += 20;
            
            DECODE_DATA (ptr, &data_type, &data_size);
            for (i = 0, done = FALSE; i + dataCodeSize + data_size <= length AND!done;
                 )
                {
                
                /* looking for datacode 300 ---      */
                /* start of nlist.                   */
                
                switch (data_type)
                    {
                case 102:
                    tc_zeroes ((VOIDPTR)key, sizeof(NLIST_ID));
                    strcpy ((char *)key, (char *)ptr);
                    break;
                case 888:
                    done = TRUE;
                    break;
                case 118:
                    hoblockmove ((VOIDPTR)ptr, (VOIDPTR)&data.offset,
                                 (size_t)sizeof(UINT));
                    data.delete = FALSE;
                    ht_insert (curFile->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
                    done = TRUE;
                    break;
                    
                case 300:
                case 301:
                case 101:
                case 105:
                case 107:
                    break;
                    /*   if there is unorganized data    */
                    /*   type, get out the loop.         */
                    
                default:
                    done = TRUE;
                    }
                if (data_size <= 0)
                    done = TRUE;
                i += dataCodeSize + data_size;
                ptr += data_size;
                if (i < length AND!done)
                    DECODE_DATA (ptr, &data_type, &data_size);
                }
            }
        tc_free ((VOIDPTR)dirs);
        } while (!end);
    return status;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To delete a existing index and update the status of existing files.

Prototype:
    publicdef INT nlz_delete_index (UINT target);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    target          -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_delete_index (UINT target)
#else
publicdef INT nlz_delete_index (target)
UINT target;
#endif
    {
    register UINT i;
    
    ht_free_table (&nlist_files[target]->idIndex);
    NL_CLOSE (nlist_files[target]->funit);
    tc_free ((VOIDPTR)nlist_files[target]);
    
    for (i = target + 1; i < totalFiles; i++)
        {
        nlist_files[i - 1] = nlist_files[i];
        }
    nlist_files[totalFiles - 1] = (NL_FILE_DATA *)NULL;
    totalFiles--;
    
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update the B-tree.

Prototype:
    publicdef INT nlz_update_index (FILENAME filename, NLIST_ID idName,
                                UINT directory_offset, UINT offset);
                                
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename            -(FILENAME)
    idName              -(NLIST_ID)
    directory_offset    -(UINT)
    offset              -(UINT)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_update_index (FILENAME filename, NLIST_ID idName,
                                UINT directory_offset, UINT offset)
#else
publicdef INT nlz_update_index (filename, idName, directory_offset, offset)
FILENAME filename;
NLIST_ID idName;
UINT directory_offset;
UINT offset;
#endif
    {
    INT status;
    INT target;
    NL_INDEX_DATA data;
    NLIST_ID key;
    
    target = nlz_get_target (filename);
    
    if (target < 0)
        {
        return FAIL;
        }
    /* the hash table algorithm uses an
    exact match.  since C strings are
    null terminated, and there fore
    ignore bytes beyond the last
    null, we must guarantee that all
    strings are equal before and
    after the null byte.  This is
    done by always zeroing out the
    key string                        */
    
    tc_zeroes ((VOIDPTR)key, sizeof(NLIST_ID));
    strcpy ((char *)key, (char *)idName);
    
    /*   update the hash table */
    
    status = ht_find (nlist_files[target]->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
    data.delete = FALSE;
    data.offset = offset;
    data.directory_offset = directory_offset;
    /* if exist replace it.  Because it has been
       deleted already.                    */
    
    if (status EQUALS SUCCESS)
        {
        ht_update (nlist_files[target]->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
        }
    else
        {
        ht_insert (nlist_files[target]->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To update a deleted item status in the B-tree.

Prototype:
    publicdef INT nlz_update_index_item (FILENAME filename, NLIST_ID idName);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    idName          -(NLIST_ID)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_update_index_item (FILENAME filename, NLIST_ID idName)
#else
publicdef INT nlz_update_index_item (filename, idName)
FILENAME filename;
NLIST_ID idName;
#endif
    {
    INT status;
    INT target;
    NL_INDEX_DATA data;
    NLIST_ID key;
    
    target = nlz_get_target (filename);
    
    if (target < 0)
        {
        return FAIL;
        }
    /* the hash table algorithm uses an
    exact match.  since C strings are
    null terminated, and there fore
    ignore bytes beyond the last
    null, we must guarantee that all
    strings are equal before and
    after the null byte.  This is
    done by always zeroing out the
    key string                        */
    
    tc_zeroes ((VOIDPTR)key, sizeof(NLIST_ID));
    strcpy ((char *)key, (char *)idName);
    
    /*   update the hash table */
    /*   search the specified id in the table. */
    status = ht_find (nlist_files[target]->idIndex, (VOIDPTR)key, (VOIDPTR)&data);
    if (status EQUALS SUCCESS)
        {
        data.delete = TRUE;
        status = ht_update (nlist_files[target]->idIndex, (VOIDPTR)key,
                            (VOIDPTR)&data);
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_get_control_record (FILE_UNIT funit,
                                      NLIST_CONTROL_RECORD **pcontrol);
                                      
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    pcontrol        -(NLIST_CONTROL_RECORD **)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_control_record (FILE_UNIT funit, NLIST_CONTROL_RECORD **pcontrol)
#else
publicdef INT nlz_get_control_record (funit, pcontrol)
FILE_UNIT funit;
NLIST_CONTROL_RECORD **pcontrol;
#endif
    {
    register UINT i;
    INT status = SUCCESS;
    NL_SHARE_MODE share_mode = SHARE;
    static NLIST_CONTROL_RECORD control_record;
    
    *pcontrol = &control_record;
    for (i = 0; i < totalFiles; i++)
        {
        if (nlist_files[i]->funit EQUALS funit)
            {
            share_mode = nlist_files[i]->share_mode;
            *pcontrol = &(nlist_files[i]->control_record);
            break;
            }
        }
    if (share_mode EQUALS SHARE)
        {
#if UNIX
        /* The following refreshes the file cache in case another process
        has made some changes. */
        funit = (FILE_UNIT)freopen (nlist_files[i]->filename, "r", (FILE *)funit);
        nlist_files[i]->funit = funit;
#else
	REWIND(funit);
#endif
        status = nlh_read (funit, (VOIDPTR)*pcontrol, sizeof(NLIST_CONTROL_RECORD));
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT nlz_read_control_record (FILE_UNIT funit,
                                        NLIST_CONTROL_RECORD **pcontrol);
                                        
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    pcontrol        -(NLIST_CONTROL_RECORD **)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_LICENSE_EXPIRED
    
Scope:
    PRIVATE to <nlz_ndx_tool>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_read_control_record (FILE_UNIT funit,
                                        NLIST_CONTROL_RECORD **pcontrol)
#else
static INT nlz_read_control_record (funit, pcontrol)
FILE_UNIT funit;
NLIST_CONTROL_RECORD **pcontrol;
#endif
    {
    register UINT i;
    INT status;
    static NLIST_CONTROL_RECORD control_record;
    
    /* check the security. */
#ifdef SECURITY
    time_t current_time;
    INT differ_time;
    UINT allocation = 0;
    CHAR current_tag[14];
    FILE_UNIT temp_funit;
#endif
    *pcontrol = &control_record;
    for (i = 0; i < totalFiles; i++)
        {
        if (nlist_files[i]->funit EQUALS funit)
            {
            *pcontrol = &(nlist_files[i]->control_record);
            break;
            }
        }
    REWIND (funit);
    status = nlh_read (funit, (VOIDPTR)*pcontrol, sizeof(NLIST_CONTROL_RECORD));

#ifdef SECURITY
    if (status < 0)
        return status;

#if LICENSE_RENEWED == TRUE
    (*pcontrol)->os_info[7] = 0;
#endif
    if ((*pcontrol)->os_info[7] EQUALS 1)
        {
        return NL_LICENSE_EXPIRED;
        }
    time (&current_time);
    differ_time = NL_EXPIRATION - current_time;
    if (differ_time < 0)
        {
        status = NL_LICENSE_EXPIRED;
        }
    else
        {
        ho_date_tag (current_tag);
        if (strcmp (current_tag, (*pcontrol)->modified_date) < 0)
            status = NL_LICENSE_EXPIRED;
        }
    if (status < 0)
        {
        nlz_close (funit);
        nlh_open (&temp_funit, filename, WRITE, &allocation);
        REWIND (temp_funit);
        nlh_read (temp_funit, (BYTE *)*pcontrol, sizeof(NLIST_CONTROL_RECORD));
        (*pcontrol)->os_info[7] = 1;
        REWIND (temp_funit);
        nlh_write (temp_funit, (BYTE *)*pcontrol, sizeof(NLIST_CONTROL_RECORD));
        nlz_close (temp_funit);
        }
#endif
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_rename_index_file (FILENAME old_file_name,
                                     FILENAME new_file_name);
                                     
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    old_file_name   -(FILENAME)
    new_file_name   -(FILENAME)

Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_rename_index_file (FILENAME old_file_name, FILENAME new_file_name)
#else
publicdef INT nlz_rename_index_file (old_file_name, new_file_name)
FILENAME old_file_name;
FILENAME new_file_name;
#endif
    {
    INT target;
    UINT nallocation;
    
    /* see if the new file name exists - return error if it does */
    target = nlz_get_target (new_file_name);
    if (target >= 0)
        {
        return FAIL;
        }
    /* make sure old file name exists */
    target = nlz_get_target (old_file_name);
    if (target < 0)
        {
        return FAIL;
        }
    nlz_put_control_record ();
    nlz_close (nlist_files[target]->funit);
    ho_rename (old_file_name, new_file_name);
    nlist_files[target]->share_mode = SHARE;
    strcpy ((char *)nlist_files[target]->filename, (char *)new_file_name);
    nlh_open (&(nlist_files[target]->funit), new_file_name,
              nlist_files[target]->share_mode, &nallocation);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_put_control_record (void);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    None.
    
Return Value/Status:
    SUCCESS - Successful completion.
    FAIL    - Unsuccessful completion.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_put_control_record (void)
#else
publicdef INT nlz_put_control_record ()
#endif
    {
    NL_FILE_DATA *curFile = nlist_files[0];
    INT status;
    
    /* the only routines that call this
    routine are updatin routines.
    There can be no calls to
    intervening opening routines;
    therefore, the file at the top of
    the stack is the last one opened
    for writing                       */
    
    /* stamp the modified date in the    */
    /* current structure and the file    */
    /* control record                    */
    
    strcpy ((char *)curFile->control_record.modified_date,
            (char *) ho_date_tag ((CHAR *)curFile->modified_date));
    
    REWIND (curFile->funit);
    status = nlh_write (curFile->funit, (VOIDPTR)&(curFile->control_record),
                        sizeof(NLIST_CONTROL_RECORD));
    
    status = (status >= 0) ? SUCCESS : FAIL;
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    To initialize the File Control Record.    

Prototype:
    publicdef INT nlz_init_ctrl (FILENAME filename, FILE_UNIT funit,
                             NL_SHARE_MODE share_mode);
                             
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    funit           -(FILE_UNIT)
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_IO_ERROR5
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_init_ctrl (FILENAME filename, FILE_UNIT funit,
                             NL_SHARE_MODE share_mode)
#else
publicdef INT nlz_init_ctrl (filename, funit, share_mode)
FILENAME filename;
FILE_UNIT funit;
NL_SHARE_MODE share_mode;
#endif
    {
    NLIST_CONTROL_RECORD control_record;
    INT status;
    register UINT i;
    UINT num_of_buckets = 0;
    NL_FILE_DATA *curFile;
    
    tc_zeroes ((VOIDPTR)&control_record, sizeof control_record);
    control_record.next_write_offset = 64;
    strcpy ((char *)control_record.version_stamp, NL_VERSION_4);
    control_record.file_mark = NL_FILE_MARK;
    REWIND (funit);
    nlh_write (funit, (VOIDPTR)&control_record, sizeof(NLIST_CONTROL_RECORD));
    /* create a new files.               */
    
    if (totalFiles EQUALS NL_MAX_INDEX)
        {
        nlz_delete_index (totalFiles - 1);
        }
    for (i = totalFiles; i > 0; i--)
        {
        nlist_files[i] = nlist_files[i - 1];
        }
    nlist_files[0] = curFile = ALLOCATE (NL_FILE_DATA);
    totalFiles++;
    curFile->funit = funit;
    
    /* read the file directory.          */
    num_of_buckets = 10;
    status = ht_init_hash_table (num_of_buckets, (UINT)10, HT_CHAR_KEY,
                                 sizeof(NLIST_ID), sizeof(NL_INDEX_DATA),
                                 HT_SYSTEM_TABLE, &curFile->idIndex);
    
    strcpy ((char *)curFile->filename, (char *)filename);
    curFile->share_mode = share_mode;
    hoblockmove ((VOIDPTR)&control_record, (VOIDPTR)&(curFile->control_record),
                 (size_t)sizeof(NLIST_CONTROL_RECORD));
    
    status = (status >= 0) ? SUCCESS : NL_IO_ERROR5;
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_get_file_data (FILE_UNIT funit, NL_FILE_DATA **dataptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    dataptr         -(NL_FILE_DATA **)

Return Value/Status:
    Function returns either the target value or FAIL.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_get_file_data (FILE_UNIT funit, NL_FILE_DATA **dataptr)
#else
publicdef INT nlz_get_file_data (funit, dataptr)
FILE_UNIT funit;
NL_FILE_DATA **dataptr;
#endif
    {
    UINT target;
    
    *dataptr = (NL_FILE_DATA *)0;
    for (target = 0; target < totalFiles; target++)
        {
        if (nlist_files[target]->funit == funit)
            {
            *dataptr = nlist_files[target];
            return(INT)target;
            }
        }
    return FAIL;                    /* it wasn't there */
    }
/* END:     */
