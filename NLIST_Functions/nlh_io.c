/* DEC/CMS REPLACEMENT HISTORY, Element NLH_IO.C*/
/* *16   14-AUG-1991 12:00:32 KEE "(SPR -1) Put in File Lock/Unlock logic"*/
/* *15   17-JUL-1991 11:47:12 GILLESPIE "(SPR 3317) Remove setvbuf"*/
/* *14   16-APR-1991 10:11:03 GILLESPIE "(SPR 1) For Unix, set buffering to zero - allows file sharing between processes"*/
/* *13   25-OCT-1990 12:16:25 GILLESPIE "(SPR 31) Lint run - correct casting inconsistencies"*/
/* *12   23-AUG-1990 16:15:16 VINCE "(SPR 1) part of a mass #if HOST to #ifdef conversion"*/
/* *11    9-AUG-1990 22:11:33 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *10   25-JUL-1990 14:49:16 VINCE "(SPR 5644) Header Standardization"*/
/* *9    12-MAR-1990 19:02:07 GILLESPIE "(SPR 6016) More error checking on mallocs; re-engineer stats calcs"*/
/* *8     5-MAR-1990 10:43:20 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *7    26-FEB-1990 19:00:05 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/* *6     6-DEC-1989 11:15:08 GORDON "(SPR 0) Fix Unix definition of nlz_free_locks"*/
/* *5    24-SEP-1989 22:53:24 GILLESPIE "(SPR 200) New n-List Version 4"*/
/* *4    18-SEP-1989 17:45:44 GILLESPIE "(SPR 100) Gulf mods"*/
/* *3    29-JUL-1989 23:59:10 GILLESPIE "(SPR 1) Fix minor casting problems"*/
/* *2    20-JUL-1989 07:33:42 GORDON "(SPR 100) Add Sun port mods"*/
/* *1    19-JUN-1989 13:15:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLH_IO.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Exploration Systems
                            c/o Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_nl_int.h"
#include "esi_nl_io.h"
#include "esi_tc.h"

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

#ifdef vms

#include "rmsdef.h"
typedef struct 
    {
    /*-----------------------------------------*/
    /* block (record) number - one-based (the  */
    /* first record is record 1, not record 0) */
    /*-----------------------------------------*/
    INT last_block;
    
    INT current_block;
    INT offset;                     /*    offset from current block, in bytes        */
    size_t bytes_in_block;          /*    number of bytes in 1 file block            */
    BYTE buffer[1];
    } NL_RMS_CONTROL;

#endif

#if USE_PROTOTYPES
static INT nlz_free_locks (FILE_UNIT funit);
static FILE *nlz_open (FILENAME filename, NL_SHARE_MODE share_mode);
#else
static INT nlz_free_locks ();
static FILE *nlz_open ();
#endif

/* File Description ---------------------------------------------------------
Overview:
    N-List RMS I/O functions for VAX/VMS.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT nlh_close(FILE_UNIT funit);
    
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode,
        UINT *allocation);
    
    publicdef INT nlh_close(FILE_UNIT funit);
    
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode, 
        UINT *allocation);
    
    publicdef INT nlh_write(FILE_UNIT funit, VOIDPTR buffer, size_t nbytes);
    
    publicdef INT nlh_seek(FILE_UNIT funit, INT file_offset, INT direc);
    
    publicdef INT nlh_read(FILE_UNIT funit, VOIDPTR buffer, size_t nbytes);
    
    publicdef INT nlh_close(FILE_UNIT funit);
    
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode, 
        UINT *allocation);
    
    publicdef INT nlz_close(FILE_UNIT funit);
    
Private_Functions:
    static INT nlz_free_locks(FILE_UNIT funit);

    static FILE *nlz_open(FILENAME filename, NL_SHARE_MODE share_mode);

-----------------------------------------------------------------------------*/


#ifdef vms          /* begin VMS section */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT nlz_free_locks(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    Function returns the value of sys$free(funit->fid).
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_free_locks (FILE_UNIT funit)
#else
static INT nlz_free_locks (funit)
FILE_UNIT funit;
#endif
    {
    if (funit->type EQUALS UNIXIO)
        return SUCCESS;
    else
        return sys$free (funit->fid);
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_close(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    Function returns the value of fclose((FILE *)funit->fid) or 
    sys$close(rab -> rab$l_fab).
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_close (FILE_UNIT funit)
#else
publicdef INT nlz_close (funit)
FILE_UNIT funit;
#endif
    {
    register INT status;
    struct RAB *rab;
    
    if (funit->type EQUALS UNIXIO)
        {
        status = fclose ((FILE *)funit->fid);
        }
    else
        {
        rab = funit->fid;
        status = sys$close (rab->rab$l_fab);
        
        tc_free ((VOIDPTR)rab->rab$l_fab);
        tc_free ((VOIDPTR)rab->rab$l_ctx);
        tc_free ((VOIDPTR)rab);
        }
    tc_free ((VOIDPTR)funit);
    
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This is the Unix-style open.

Prototype:
    static FILE *nlz_open(FILENAME filename, NL_SHARE_MODE share_mode);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    share_mode      -(NL_SHARE_MODE)

Return Value/Status:
    Function returns either file handle or NULL_FILE_PTR.
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static FILE *nlz_open (FILENAME filename, NL_SHARE_MODE share_mode)
#else
static FILE *nlz_open (filename, share_mode)
FILENAME filename;
NL_SHARE_MODE share_mode;
#endif
    {
    FILE *fun;
    
    switch (share_mode)
        {
    case SHARE:
        fun = ho_open (filename, "rb", "shr=put,upd");
        break;
    case NOSHARE:
        fun = ho_open (filename, "rb", "shr=nil");
        break;
    case WRITE:
        if (hoexists (filename) IS_NOT_EQUAL_TO TRUE)
            {
            fun = ho_open (filename, "r+b", "shr=get");
            }
        else
            {
            fun = ho_open (filename, "w+b", "shr=get");
            }
        break;
    case WRITE_NOSHARE:
        if (hoexists (filename) IS_NOT_EQUAL_TO TRUE)
            {
            fun = ho_open (filename, "r+b", "shr=nil");
            }
        else
            {
            fun = ho_open (filename, "w+b", "shr=nil");
            }
        break;
    default:
        fun = NULL_FILE_PTR;
        }
    return fun;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode, 
        UINT *allocation);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pfunit          -(FILE_UNIT *)
    filename        -(FILENAME)
    share_mode      -(NL_SHARE_MODE)
    (M) allocation  -(UINT *) INPUT: # of blocks to allocate initially;
                        OUTPUT: Number of blocks currently allocated.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    NL_OVERFLOW
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_open (FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode,
                        UINT *allocation)
#else
publicdef INT nlh_open (pfunit, filename, share_mode, allocation)
FILE_UNIT *pfunit;
FILENAME filename;
NL_SHARE_MODE share_mode;
UINT *allocation;
#endif
    {
    register INT status = SUCCESS;
    struct FAB *fab;
    struct RAB *rab;
    NL_RMS_CONTROL *rms_control;
    size_t bytes_in_block;
    FILE_UNIT funit = (FILE_UNIT) tc_zalloc (sizeof(FILE_STRUCT));
    
    if (funit == (FILE_UNIT)0)
        {
        return NL_OVERFLOW;
        }
    *pfunit = (FILE_UNIT)0;         /* assume failure */
    
    fab = ALLOCATE (struct FAB);
    *fab = cc$rms_fab;
    
    fab->fab$l_fna = filename;      /* name of file                */
    fab->fab$b_fns = strlen (filename); /* length of file name */
    fab->fab$w_gbc = 0;             /* global buffer count */
    fab->fab$b_bks = 4;             /* 4 blocks per bucket */
    fab->fab$w_mrs = 1024;          /* number of bytes in record */
    fab->fab$b_org = FAB$C_SEQ;     /* sequential access */
    fab->fab$b_rfm = FAB$C_FIX;     /* fixed record size */
    fab->fab$l_alq = *allocation <= 0 ? 50 :  * allocation; /* initial allocation, in blocks */
    fab->fab$w_deq = 100;           /* extension block allocations */
    
    rab = ALLOCATE (struct RAB);
    *rab = cc$rms_rab;
    rab->rab$l_fab = fab;           /* connect fab to rab */
    rab->rab$b_ksz = sizeof(INT);   /* key size */
    rab->rab$b_mbc = 2;             /* multi-block count */
    rab->rab$b_mbf = 10;            /* multi-buffer count */
    rab->rab$b_rac = RAB$C_KEY;     /* access by record key */
    rab->rab$l_rop = RAB$M_NLK | RAB$M_RAH | RAB$M_TMO;
    rab->rab$b_tmo = 5;             /* 5 second timeout */
    
    switch (share_mode)
        {
    default:
    case SHARE:
        fab->fab$b_fac = FAB$M_GET;
        fab->fab$l_fop = 0;
        fab->fab$b_shr = FAB$M_SHRDEL | FAB$M_SHRGET | FAB$M_SHRPUT | FAB$M_SHRUPD;
        status = sys$open (fab);
        break;
    case NOSHARE:
        fab->fab$b_fac = FAB$M_GET;
        fab->fab$l_fop = 0;
        fab->fab$b_shr = FAB$M_SHRGET;
        status = sys$open (fab);
        break;
    case WRITE:
        fab->fab$b_fac = FAB$M_DEL | FAB$M_GET | FAB$M_PUT | FAB$M_UPD;
        fab->fab$l_fop = FAB$M_CIF | FAB$M_CBT;
        fab->fab$b_shr = FAB$M_SHRGET;
        rab->rab$l_rop |= RAB$M_RLK | RAB$M_UIF | RAB$M_WBH;
        status = sys$create (fab);
        break;
    case WRITE_NOSHARE:
        fab->fab$b_fac = FAB$M_DEL | FAB$M_GET | FAB$M_PUT | FAB$M_UPD;
        fab->fab$l_fop = FAB$M_CIF | FAB$M_CBT;
        fab->fab$b_shr = FAB$M_SHRGET;
        rab->rab$l_rop |= RAB$M_RLK | RAB$M_UIF | RAB$M_WBH;
        status = sys$create (fab);
        break;
        }
    funit->fid = rab;
    funit->type = RMS;
    
    /*    the following detects a Unix-style file        */
    
    if (fab->fab$b_rfm IS_NOT_EQUAL_TO FAB$C_FIX)
        {
        *allocation = 0;
        nlz_close (funit);
        funit = (FILE_UNIT) tc_zalloc (sizeof(FILE_STRUCT));
        if (funit == (FILE_UNIT)0)
            {
            return NL_OVERFLOW;
            }
        funit->fid = (struct RAB *) nlz_open (filename, share_mode);
        if ((FILE *)funit->fid EQUALS NULL_FILE_PTR)
            {
            *allocation = 0;
            nlz_close (funit);
            return NL_FILE_OPEN_ERROR;
            }
        else
            {
            funit->type = UNIXIO;
            *pfunit = funit;
            return SUCCESS;
            }
        }
    if (NOT (status& 1))
        {
        *allocation = 0;
        nlz_close (funit);
        return NL_FILE_OPEN_ERROR;
        }
    /*--------------------------------*/
    /* number of bytes in each record */
    /* carry along context info       */
    /*--------------------------------*/
    bytes_in_block = fab->fab$w_mrs;
    
    rms_control = (NL_RMS_CONTROL *) tc_zalloc (sizeof(NL_RMS_CONTROL) +
                                                bytes_in_block - 1);
    if (rms_control == (NL_RMS_CONTROL *)0)
        {
        return NL_OVERFLOW;
        }
    rms_control->bytes_in_block = bytes_in_block;
    rab->rab$l_ctx = (INT)rms_control;
    /*----------------*/
    /* address of key */
    /* (= record #)   */
    /*----------------*/
    rab->rab$l_kbf = (char *)&rms_control->last_block;
    
    /*-------------------*/
    /* address of buffer */
    /* for PUT           */
    /*-------------------*/
    rab->rab$l_rbf = (char *)rms_control->buffer;
    
    /*-------------------*/
    /* address of buffer */
    /* for GET           */
    /*-------------------*/
    rab->rab$l_ubf = (char *)rms_control->buffer;
    
    rab->rab$w_rsz = bytes_in_block;
    rab->rab$w_usz = bytes_in_block;    /* number of bytes in record for GET*/
    
    if (NOT (sys$connect (rab) & 1))
        {
        *allocation = 0;
        nlz_close (funit);
        return NL_FILE_OPEN_ERROR;
        }
    *allocation = fab->fab$l_alq;
    *pfunit = funit;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_close(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_close (FILE_UNIT funit)
#else
publicdef INT nlh_close (funit)
FILE_UNIT funit;
#endif
    {
    INT status;
    UINT allocation = 0;
    register INT i;
    INT target;
    FILE_UNIT file_unit;
    NL_FILE_DATA *dataptr = (NL_FILE_DATA *)0;
    
    target = nlz_get_file_data (funit, &dataptr);
    
    if (dataptr != (NL_FILE_DATA *)0)
        {
        /* if the file was found in the
        stack, and it was open for write,
        then reopen it for read.  Be sure
        to restore the file to the
        correct mode (exclusive or shared
        read                              */
        
        switch (dataptr->share_mode)
            {
        case SHARE:
        case NOSHARE:
            status = nlz_close (funit);
            break;
        case WRITE:
            status = nlz_close (funit);
            if (dataptr->filename[strlen ((char *)dataptr->filename) - 1] EQUALS';')
                {
                ho_translate_filename (dataptr->filename, dataptr->filename);
                }
            status = nlh_open (&file_unit, dataptr->filename, SHARE, &allocation);
            if (status >= 0)
                {
                dataptr->share_mode = SHARE;
                dataptr->funit = file_unit;
                }
            else
                {
                nlz_delete_index ((UINT)target);
                status = NL_FILE_OPEN_ERROR;
                }
            break;
            
        case WRITE_NOSHARE:
            if (dataptr->filename[strlen (dataptr->filename) - 1] EQUALS';')
                {
                ho_translate_filename (dataptr->filename, dataptr->filename);
                }
            /* when done with a file opened      */
            /* WRITE_NOSHARE, release locks only */
            nlz_free_locks (funit);
            break;
            
        default:
            status = NL_FILE_OPEN_ERROR;
            }
        }
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_read(FILE_UNIT funit, VOIDPTR buffer, size_t nbytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    buffer          -(VOIDPTR)
    nbytes          -(size_t)

Return Value/Status:
    SUCCESS - Successful completion.
    (INT)nbytes_moved
    (INT)NL_FILE_READ_ERROR
    
Scope:
    PUBLIC
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_read (FILE_UNIT funit, VOIDPTR buffer, size_t nbytes)
#else
publicdef INT nlh_read (funit, buffer, nbytes)
FILE_UNIT funit;
VOIDPTR buffer;
size_t nbytes;
#endif
    {
    register INT status;
    NL_RMS_CONTROL *rms_control;
    INT block_offset;
    size_t nbytes_moved;
    INT nbytes_to_move;
    size_t bytes_in_block;
    struct RAB *rab;
    BYTE *pbuff;
    
    if (funit->type EQUALS UNIXIO)
        {
        return ho_read ((FILE *)funit->fid, buffer, nbytes);
        }
    rab = funit->fid;
    rms_control = (NL_RMS_CONTROL *)rab->rab$l_ctx;
    block_offset = rms_control->offset;
    bytes_in_block = rms_control->bytes_in_block;
    nbytes_to_move = bytes_in_block - block_offset;
    if (nbytes < nbytes_to_move)
        {
        nbytes_to_move = nbytes;
        }
    /*    ALL n-List reads are preceded by a call to
        nlh_seek.  This call is made
        asynchronously; therefore, we must WAIT for
        completion of the seek operation before
        attempting to read                */
    
    /* wait for seek to complete */
    if (NOT (sys$wait (rab) & 1))
        {
        return(INT)NL_FILE_READ_ERROR;
        }
    rab->rab$l_rop &= NOT RAB$M_ASY;    /*    reset async flag        */
    /*----------------*/
    /* reset key      */
    /* buffer address */
    /*----------------*/
    rab->rab$l_kbf = (char *)&rms_control->last_block;
    
    if (rms_control->last_block IS_NOT_EQUAL_TO rms_control->current_block)
        {
        rms_control->last_block = rms_control->current_block;
        if (NOT (sys$get (rab) & 1))
            {
            return(INT)NL_FILE_READ_ERROR;
            }
        }
    hoblockmove ((VOIDPTR)(rms_control->buffer + block_offset), (VOIDPTR)buffer,
                 (size_t)nbytes_to_move);
    
    for (nbytes_moved = (size_t)nbytes_to_move,
         pbuff = ((BYTE *)buffer) + nbytes_moved;
         nbytes_moved + bytes_in_block < nbytes;
         nbytes_moved += bytes_in_block, pbuff += bytes_in_block)
        {
        rab->rab$l_ubf = (char *)pbuff;
        rms_control->last_block++;
        
        if (NOT (sys$get (rab) & 1))
            {
            rab->rab$l_ubf = (char *)rms_control->buffer;
            rms_control->last_block = -1;
            return(INT)nbytes_moved;
            }
        }
    rab->rab$l_ubf = (char *)rms_control->buffer;
    if (nbytes_moved < nbytes)
        {
        rms_control->last_block++;
        if (sys$get (rab) & 1)      /*    if read was successful            */
            {
            nbytes_to_move = nbytes - nbytes_moved;
            /*    move this partial buffer            */
            hoblockmove ((VOIDPTR)(rms_control->buffer), (VOIDPTR)pbuff,
                         nbytes_to_move);
            nbytes_moved += nbytes_to_move;
            }
        }
    return(INT)nbytes_moved;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_seek(FILE_UNIT funit, INT file_offset, INT direc);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    file_offset     -(INT)
    direc           -(INT) Always SEEK_SET for n-Lists.

Return Value/Status:
    Function returns the value of sys$find(rab).
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_seek (FILE_UNIT funit, INT file_offset, INT direc)
#else
publicdef INT nlh_seek (funit, file_offset, direc)
FILE_UNIT funit;
INT file_offset;
INT direc;
#endif
    {
    NL_RMS_CONTROL *rms_control;
    struct RAB *rab;
    
    if (funit->type EQUALS UNIXIO)
        {
        return fseek ((FILE *)funit->fid, file_offset, direc);
        }
    rab = funit->fid;
    rms_control = (NL_RMS_CONTROL *)rab->rab$l_ctx;
    rms_control->current_block = file_offset / rms_control->bytes_in_block; /* block (record) numbers are one-based */
    rms_control->offset = file_offset - rms_control->bytes_in_block *
        rms_control->current_block;
    rms_control->current_block++;
    rab->rab$l_kbf = (char *)&rms_control->current_block;
    rab->rab$l_rop |= RAB$M_ASY;    /*    set up async operation            */
    
    return sys$find (rab);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_write(FILE_UNIT funit, VOIDPTR buffer, size_t nbytes);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)
    buffer          -(VOIDPTR)
    nbytes          -(size_t)

Return Value/Status:
    NL_FILE_READ_ERROR
    Function returns the value of nbytes_moved.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_write (FILE_UNIT funit, VOIDPTR buffer, size_t nbytes)
#else
publicdef INT nlh_write (funit, buffer, nbytes)
FILE_UNIT funit;
VOIDPTR buffer;
size_t nbytes;
#endif
    {
    register INT status;
    NL_RMS_CONTROL *rms_control;
    struct RAB *rab;
    INT block_offset;
    size_t nbytes_moved;
    INT nbytes_to_move;
    size_t bytes_in_block;
    BYTE *pbuff;
    /*-------------------------------*/
    /* TRUE if we should use UPDATE; */
    /* FALSE means use PUT           */
    /*-------------------------------*/
    BOOL replace = FALSE;
    
    if (funit->type EQUALS UNIXIO)
        {
        return(INT) ho_write ((FILE *)funit->fid, buffer, nbytes);
        }
    rab = funit->fid;
    rms_control = (NL_RMS_CONTROL *)rab->rab$l_ctx;
    block_offset = rms_control->offset;
    rms_control->last_block = rms_control->current_block;
    bytes_in_block = rms_control->bytes_in_block;
    nbytes_to_move = bytes_in_block - block_offset;
    if (nbytes < nbytes_to_move)
        {
        nbytes_to_move = nbytes;
        }
    /*    ALL n-List writes are preceded by a call to
        nlh_seek.  This call is made
        asynchronously; therefore, we must WAIT for
        completion of the seek operation before
        attempting to write                */
    
    /*-----------------------------*/
    /* wait for seek to complete   */
    /* reset async and nolock flag */
    /*-----------------------------*/
    status = sys$wait (rab);
    
    rab->rab$l_rop &= NOT RAB$M_ASY& NOT RAB$M_NLK;
    /*----------------*/
    /* reset key      */
    /* buffer address */
    /*----------------*/
    rab->rab$l_kbf = (char *)&rms_control->last_block;
    
    if (status IS_NOT_EQUAL_TO RMS$_EOF)
        {
        if (NOT (status& 1))
            {
            return NL_FILE_READ_ERROR;
            }
        replace = TRUE;
        if (sys$get (rab) EQUALS RMS$_EOF)
            {
            rab->rab$w_rsz = bytes_in_block;
            replace = FALSE;
            }
        }
    if (NOT replace)
        {
        tc_zeroes ((VOIDPTR)rms_control->buffer, bytes_in_block);
        }
    /*    write into rms_control buffer            */
    
    hoblockmove ((VOIDPTR)buffer, (VOIDPTR)(rms_control->buffer + block_offset),
                 (size_t)nbytes_to_move);
    status = replace ? sys$update (rab) : sys$put (rab);
    
    for (nbytes_moved = nbytes_to_move, pbuff = ((BYTE *)buffer) + nbytes_moved;
         nbytes_moved + bytes_in_block < nbytes;
         nbytes_moved += bytes_in_block, pbuff += bytes_in_block)
        {
        rms_control->last_block++;
        if (replace)
            {
            rab->rab$l_rbf = (char *)rms_control->buffer;
            if (sys$get (rab) EQUALS RMS$_EOF)
                {
                /*--------------------*/
                /* number of bytes in */
                /* record for PUT     */
                /*--------------------*/
                rab->rab$w_rsz = bytes_in_block;
                
                replace = FALSE;
                }
            rab->rab$l_rbf = (char *)pbuff;
            status = replace ? sys$update (rab) : sys$put (rab);
            }
        else
            {
            rab->rab$l_rbf = (char *)pbuff;
            status = sys$put (rab);
            }
        if (NOT (status& 1))
            {
            rab->rab$l_rbf = (char *)rms_control->buffer;
            rms_control->last_block = -1;
            return nbytes_moved;
            }
        }
    rab->rab$l_rbf = (char *)rms_control->buffer;
    
    /*    check to make sure all bytes have been
        transferred.  This next block only triggers
        when less than one full block remains to be
        written                        */
    
    if (nbytes_moved < nbytes)
        {
        /*    move this partial buffer            */
        nbytes_to_move = nbytes - nbytes_moved;
        rms_control->last_block++;
        /*    read the block if this is an update        */
        /*    operation                    */
        if (replace)
            {
            if (sys$get (rab) EQUALS RMS$_EOF)
                {
                /*--------------------*/
                /* number of bytes in */
                /* record for PUT     */
                /*--------------------*/
                rab->rab$w_rsz = bytes_in_block;
                
                replace = FALSE;
                }
            }
        if (NOT replace)
            {
            tc_zeroes ((VOIDPTR)rms_control->buffer, bytes_in_block);
            }
        hoblockmove ((VOIDPTR)pbuff, (VOIDPTR)rms_control->buffer, nbytes_to_move);
        status = replace ? sys$update (rab) : sys$put (rab);
        nbytes_moved += nbytes_to_move;
        }
    rab->rab$l_rop |= RAB$M_NLK;
    return(INT)nbytes_moved;
    }
/* END:     */

#endif

#ifdef primos

#include "keys.ins.cc"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT nlz_free_locks(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    Function returns the value of (INT)errcode.
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_free_locks (FILE_UNIT funit)
#else
static INT nlz_free_locks (funit)
FILE_UNIT funit;
#endif
    {
    
    fortran srch$$();
    SHORT key, errcode, primos_unit;
    
    /* change access right to read only */
    key = k$cacc + k$read;
    
    /* convert C file unit to Primos unit */
    primos_unit = (SHORT)bio$primosfileunit (funit);
    
    srch$$(key, 0, 0, primos_unit, 0, errcode);
    return(INT)errcode;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode, 
        UINT *allocation);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pfunit          -(FILE_UNIT *)
    filename        -(FILENAME)
    share_mode      -(NL_SHARE_MODE)
    allocation      -(UINT *) Unused in Primos version.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_open (FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode,
                        UINT *allocation)
#else
publicdef INT nlh_open (pfunit, filename, share_mode, allocation)
FILE_UNIT *pfunit;
FILENAME filename;
NL_SHARE_MODE share_mode;
UINT *allocation;
#endif
    {
    FILE_UNIT funit;
    fortran satr$$();
    SHORT key, namlen, code;
    SHORT attributes[2];
    
    *pfunit = (FILE_UNIT)0;         /* assume failure */
    
    switch (share_mode)
        {
    case SHARE:
        /* WARNING - this code has not been tested on prime... JGG */
        
        funit = ho_open (filename, "r", NULL);
        break;
    case WRITE_NOSHARE:
    case WRITE:
    case NOSHARE:
        if (hoexists (filename))    /* if the file exists, we open it in
                                    update mode, since we will be reading
                                    AND writing */
            funit = ho_open (filename, "r+", NULL);
        else                        /* if the file doesn't exist, open it
                                    in write mode, since we won't need to
                                    be updating it the first time through */
            funit = ho_open (filename, "w", NULL);
    default:
        return NL_FILE_OPEN_ERROR;
        }
    if (funit > 0)
        {
        key = k$rwlk;
        namlen = strlen (filename);
        attributes[0] = k$updt;
        satr$$(key, (char [])filename, namlen, attributes, code);
        }
    if (funit == (FILE_UNIT)0)
        return NL_FILE_OPEN_ERROR;
    
    *pfunit = funit;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_close(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_close (FILE_UNIT funit)
#else
publicdef INT nlh_close (funit)
FILE_UNIT funit;
#endif
    {
    INT status;
    UINT allocation = 0;
    register INT i;
    INT target;
    FILE_UNIT file_unit;
    NL_FILE_DATA *dataptr = (NL_FILE_DATA *)0;
    
    target = nlz_get_file_data (funit, &dataptr);
    
    if (dataptr != (NL_FILE_DATA *)0)
        {
        /* if the file was found in the
        stack, and it was open for write,
        then reopen it for read.  Be sure
        to restore the file to the
        correct mode (exclusive or shared
        read                              */
        
        switch (dataptr->share_mode)
            {
        case SHARE:
        case NOSHARE:
            status = nlz_close (funit);
            break;
        case WRITE_NOSHARE:
        case WRITE:
            status = nlz_close (funit);
            status = nlh_open (&file_unit, dataptr->filename, SHARE, &allocation);
            if (status == SUCCESS)
                {
                dataptr->share_mode = SHARE;
                dataptr->funit = file_unit;
                }
            else
                {
                nlz_delete_index (target);
                status = NL_FILE_OPEN_ERROR;
                }
            break;
            
            /* 06/09/88 GS On prime, for now treat write_noshare same as write because,
            although data has already been written to file, the Primos buffers haven't been
            flushed & changing the access mode to read here will not allow the data
            to be written when Primos gets around to flushing the buffer. ffflush() won't
            work as we have a file unit, not a file pointer. */
        default:
            status = NL_FILE_OPEN_ERROR;
            }
        }
    return status;
    }
/* END:     */

#endif /* PRIMOS */

#if UNIX
#ifndef applec

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT nlz_free_locks(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_free_locks (FILE_UNIT funit)
#else
static INT nlz_free_locks (funit)
FILE_UNIT funit;
#endif
    {
    return SUCCESS;
    }
/* END:     */

#else

#include "esi_stat.h"

/* Function Description -----------------------------------------------------
Description:

Prototype:
    static INT nlz_free_locks(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    Function returns status of chmod( funit, mode ).
    
Scope:
    PRIVATE to <nlh_io>

Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
static INT nlz_free_locks (FILE_UNIT funit)
#else
static INT nlz_free_locks (funit)
FILE_UNIT funit;
#endif
    {
    INT mode;
    INT status;
    
    mode = S_IRUSR +                /* Read permission, owner */
        S_IRGRP +                   /* Read permission, group */
        S_IWGRP +                   /* Write permission, group */
        S_IROTH +                   /* Read permission, other */
        S_IWOTH;                    /* Write permission, other */
    
    fileno (funit);
    status = chmod (funit, mode);
    return status;
    }
/* END:     */

#endif    /* MPW */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_open(FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode,
        UINT *allocation);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    pfunit          -(FILE_UNIT *)
    filename        -(FILENAME)
    share_mode      -(NL_SHARE_MODE)
    allocation      -(UINT *) Unused in UNIX version.

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_open (FILE_UNIT *pfunit, FILENAME filename, NL_SHARE_MODE share_mode,
                        UINT *allocation)
#else
publicdef INT nlh_open (pfunit, filename, share_mode, allocation)
FILE_UNIT *pfunit;
FILENAME filename;
NL_SHARE_MODE share_mode;
UINT *allocation;
#endif
    {
    FILE_UNIT funit;
    INT status;
    
    switch (share_mode)
        {
    case SHARE:
        funit = ho_open (filename, "r", NULL);
        break;
    case WRITE_NOSHARE:
    case WRITE:
    case NOSHARE:
        if (hoexists (filename))    /* if the file exists, we open it in
                                    update mode, since we will be reading
                                    AND writing */
            funit = ho_open (filename, "r+", NULL);
        else                        /* if the file doesn't exist, open it
                                    in write mode, since we won't need to
                                    be updating it the first time through */
            funit = ho_open (filename, "w", NULL);

	status = ho_lock(funit, ON);
	if (status != SUCCESS)
	  return NL_FILE_OPEN_ERROR;
        break;
    default:
        return NL_FILE_OPEN_ERROR;
        }
    if (funit == (FILE_UNIT)0)
        return NL_FILE_OPEN_ERROR;
    
    *pfunit = funit;
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_close(FILE_UNIT funit);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_FILE_OPEN_ERROR
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_close (FILE_UNIT funit)
#else
publicdef INT nlh_close (funit)
FILE_UNIT funit;
#endif
    {
    INT status;
    UINT allocation = 0;
    INT target;
    FILE_UNIT file_unit;
    NL_FILE_DATA *dataptr = (NL_FILE_DATA *)0;
    
    target = nlz_get_file_data (funit, &dataptr);
    
    if (dataptr != (NL_FILE_DATA *)0)
        {
        
        /* if the file was found in the
        stack, and it was open for write,
        then reopen it for read.  Be sure
        to restore the file to the
        correct mode (exclusive or shared
        read                              */
        
        switch (dataptr->share_mode)
            {
        case SHARE:
            status = fclose (funit);
            break;
        case NOSHARE:
	    status = ho_lock(funit, OFF);
            status = fclose (funit);
            break;
        case WRITE_NOSHARE:
        case WRITE:
	    status = ho_lock(funit, OFF);
            status = fclose (funit);
            status = nlh_open (&file_unit, dataptr->filename, SHARE, &allocation);
            if (status == SUCCESS)
                {
                dataptr->share_mode = SHARE;
                dataptr->funit = file_unit;
                }
            else
                {
                nlz_delete_index (target);
                status = NL_FILE_OPEN_ERROR;
                }
            break;
        default:
            status = NL_FILE_OPEN_ERROR;
            }
        }
    return status;
    }
/* END:     */

#endif        /* End of UNIX code */
