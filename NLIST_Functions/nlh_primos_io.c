/* DEC/CMS REPLACEMENT HISTORY, Element NLH_PRIMOS_IO.C*/
/* *5     9-AUG-1990 22:11:52 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *4    25-JUL-1990 14:49:38 VINCE "(SPR 5644) Header Standardization"*/
/* *3     5-MAR-1990 10:43:32 GILLESPIE "(SPR 6015) Change file names to be compliant with Unix"*/
/* *2    24-SEP-1989 22:26:32 GILLESPIE "(SPR 200) New n-List 4.0 - Table/Row mods"*/
/* *1    19-JUN-1989 13:15:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element NLH_PRIMOS_IO.C*/
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

#include "esi_gl_defs.h"

#ifdef primos

#include "keys.ins.cc"
#include "esi_nl_io.h"
#include "esi_tc.h"

/* File Description ---------------------------------------------------------
Overview:
    Primos version of NLH_VMS_IO.

Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT nlh_write(FILE_UNIT rab, BYTE *buffer, INT nbytes);
    
    publicdef INT nlh_seek(FILE_UNIT rab, INT file_offset, INT direc);
    
    publicdef INT nlh_read(FILE_UNIT rab, BYTE *buffer, INT nbytes);
    
    publicdef FILE_UNIT nlh_open(FILENAME filename, NL_SHARE_MODE share_mode,
        INT *allocation);
    
    publicdef INT nlz_close(FILE_UNIT rab);
    
    publicdef INT nlz_free_locks(FILE_UNIT rab);

Private_Functions:

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_free_locks(FILE_UNIT rab);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rab             -(FILE_UNIT)

Return Value/Status:
    Function returns the value of (INT)errcode.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_free_locks (FILE_UNIT rab) 
#else
publicdef INT nlz_free_locks (rab)
FILE_UNIT rab;
#endif
    {
    fortran srch$$();
    SHORT key, errcode, primos_unit;
    
    /* change access right to read only */
    key = k$cacc + k$read;
    
    /* convert C file unit to Primos unit */
    primos_unit = (SHORT)bio$primosfileunit (rab);
    
    srch$$(key, 0, 0, primos_unit, 0, errcode);
    return(INT)errcode;
    
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlz_close(FILE_UNIT rab);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rab             -(FILE_UNIT)

Return Value/Status:
    SUCCESS - Successful completion.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlz_close (FILE_UNIT rab) 
#else
publicdef INT nlz_close (rab)
FILE_UNIT rab;
#endif
    {
    NL_CLOSE (rab);
    return SUCCESS;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef FILE_UNIT nlh_open(FILENAME filename, NL_SHARE_MODE share_mode,
        INT *allocation);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    filename        -(FILENAME)
    share_mode      -(NL_SHARE_MODE)
    allocation      -(INT *) Unused in Primos version.

Return Value/Status:
    Function returns the value of funit.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef FILE_UNIT nlh_open (FILENAME filename, NL_SHARE_MODE share_mode,
                              INT *allocation) 
#else
publicdef FILE_UNIT nlh_open (filename, share_mode, allocation)
FILENAME filename;
NL_SHARE_MODE share_mode;
INT *allocation;
#endif
    {
    FILE_UNIT funit;
    fortran satr$$();
    SHORT key, namlen, code;
    SHORT attributes[2];
    
    switch (share_mode)
        {
    case SHARE:
        funit = open (filename, 0);
        break;
    case WRITE_NOSHARE:
    case WRITE:
    case NOSHARE:
        funit = open (filename, 2);
        break;
    default:
        funit = (FILE_UNIT)NL_FILE_OPEN_ERROR;
        }
    if (funit > 0)
        {
        key = k$rwlk;
        namlen = strlen (filename);
        attributes[0] = k$updt;
        satr$$(key, (char [])filename, namlen, attributes, code);
        }
    return funit;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_read(FILE_UNIT rab, BYTE *buffer, INT nbytes);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rab             -(FILE_UNIT)
    buffer          -(BYTE *)
    nbytes          -(INT)

Return Value/Status:
    Function returns the value of ho_read.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_read (FILE_UNIT rab, BYTE *buffer, INT nbytes) 
#else
publicdef INT nlh_read (rab, buffer, nbytes)
FILE_UNIT rab;
BYTE *buffer;
INT nbytes;
#endif
    {
    return ho_read (rab, buffer, nbytes);
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_seek(FILE_UNIT rab, INT file_offset, INT direc);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rab             -(FILE_UNIT)
    file_offset     -(INT)
    direc           -(INT) Always SEEK_ABS for n-Lists.

Return Value/Status:
    Function returns the value of lseek.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_seek (FILE_UNIT rab, INT file_offset, INT direc) 
#else
publicdef INT nlh_seek (rab, file_offset, direc)
FILE_UNIT rab;
INT file_offset;
INT direc;
#endif
    {
    INT status;
    
    status = lseek ((int)rab, file_offset, direc);
    return status;
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nlh_write(FILE_UNIT rab, BYTE *buffer, INT nbytes);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    rab             -(FILE_UNIT)
    buffer          -(BYTE *)
    nbytes          -(INT)

Return Value/Status:
    Function returns the value of write.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nlh_write (FILE_UNIT rab, BYTE *buffer, INT nbytes) 
#else
publicdef INT nlh_write (rab, buffer, nbytes)
FILE_UNIT rab;
BYTE *buffer;
INT nbytes;
#endif
    {
    INT status;
    
    status = write (rab, buffer, nbytes);
    return status;
    }
/* END:     */

#endif
