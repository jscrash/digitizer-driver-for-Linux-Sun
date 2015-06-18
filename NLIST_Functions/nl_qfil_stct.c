/* DEC/CMS REPLACEMENT HISTORY, Element NL_QFIL_STCT.C*/
/* *2    25-JUL-1990 15:02:39 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 11:00:09 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_QFIL_STCT.C*/
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
#include "esi_tc.h"
#include "esi_ho.h"

/* Function Description -----------------------------------------------------
Description:
    To inquire the user structure of a n_list file.

Prototype:
    publicdef INT nl_inq_file_user_struct (FILENAME in_filename, size_t nallocated,
        VOIDPTR user_struct_ptr, size_t *nbyte);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    nallocated      -(size_t)
    user_struct_ptr -(VOIDPTR)
    nbyte           -(size_t *)

Return Value/Status:
    SUCCESS - Successful completion.
    NL_NLIST_NOT_FOUND
    NL_FILE_NOT_FOUND
    
Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_inq_file_user_struct (FILENAME in_filename, size_t nallocated,
                                       VOIDPTR user_struct_ptr, size_t *nbyte)
#else
publicdef INT nl_inq_file_user_struct (in_filename, nallocated, user_struct_ptr, nbyte)
FILENAME in_filename;
size_t nallocated;
VOIDPTR user_struct_ptr;
size_t *nbyte;
#endif
    {
    INT status = SUCCESS;
    INT exists;
    FILENAME filename;
    FILE_UNIT funit;
    NLIST_CONTROL_RECORD *control_record;
    VOIDPTR record;
    INT length;
    
    /* error checking   */
    /* translate the file name first to
    get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return NL_FILE_NOT_FOUND;
        }
    exists = nlz_chk_nlist_exist (filename, "DUMMY", &funit, (BOOL)TRUE, SHARE);
    if (exists IS_NOT_EQUAL_TO NL_NLIST_NOT_FOUND)  /* file does NOT exist - make a new one */
        {
        return NL_NLIST_NOT_FOUND;
        }
    /* get the control record.     */
    
    if ((status = nlz_get_control_record (funit, &control_record)) < 0)
        {
        return status;
        }
    /* If there is an existing file user structure, read it*/
    if (control_record->file_user_struct_offset > 0)
        {
        status = nlz_get_record (funit, (UINT)control_record->file_user_struct_offset,
                                 &record, (UINT *) & length, (BOOL)FALSE);
        if (status < 0)
            {
            return status;
            }
        status = (nallocated >= length) ? SUCCESS : NL_OVERFLOW;
        *nbyte = (nallocated >= length) ? length : nallocated;
        hoblockmove (record, user_struct_ptr, *nbyte);
        tc_free (record);
        }
    else
        {
        *nbyte = 0;
        }
    return status;
    }
/* END:     */
