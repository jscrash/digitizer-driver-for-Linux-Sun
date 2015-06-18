/* DEC/CMS REPLACEMENT HISTORY, Element NL_RD_FBYTES.C*/
/* *2    25-JUL-1990 15:04:48 VINCE "(SPR 5644) Header Standardization"*/
/* *1     5-MAR-1990 11:00:52 GILLESPIE "Rename files for Unix compliance (SPR 6015)"*/
/* DEC/CMS REPLACEMENT HISTORY, Element NL_RD_FBYTES.C*/
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

#ifndef ESI_HO_FILES_H

#include "esi_ho_files.h"

#endif

/* Function Description -----------------------------------------------------
Description:

Prototype:
    publicdef INT nl_read_file_bytes (FILENAME in_filename, INT offset,
        VOIDPTR *returned_data, size_t *returned_bytes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    in_filename     -(FILENAME)
    offset          -(INT)
    returned_data   -(VOIDPTR *)
    size_t          -(size_t *)

Return Value/Status:
    NL_INVALID_FILENAME
    Function returns status from ho_translate_filename, nlz_get_read_funit,
    and nlz_get_record.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT nl_read_file_bytes (FILENAME in_filename, INT offset,
                                  VOIDPTR *returned_data, size_t *returned_bytes)
#else
publicdef INT nl_read_file_bytes (in_filename, offset, returned_data, returned_bytes)
FILENAME in_filename;
INT offset;
VOIDPTR *returned_data;
size_t *returned_bytes;
#endif
    {
    FILENAME filename;
    FILE_UNIT funit;
    INT status;
    
    if (in_filename EQUALS (CHAR *)0)
        {
        return NL_INVALID_FILENAME;
        }
    /* translate the file name first to
        get rid of LOGICAL SYMBOL. */
    status = ho_translate_filename (in_filename, filename);
    if (status != SUCCESS)
        {
        return status;
        }
    if ((status = nlz_get_read_funit (filename, &funit)) < 0)
        {
        return status;
        }
    return nlz_get_record (funit, (UINT)offset, returned_data, (UINT *)returned_bytes,
                           (BOOL)FALSE);
    }
/* END:     */
