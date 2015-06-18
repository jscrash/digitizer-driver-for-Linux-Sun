/* DEC/CMS REPLACEMENT HISTORY, Element HO_WRITE.C*/
/* *2    17-AUG-1990 21:54:31 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:00:02 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_WRITE.C*/
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

#include "esi_ho.h"
#include "esi_ho_files.h"

/* Function Description -----------------------------------------------------
Description:
    The purpose of this routinue is to write a buffer which 
    length is longer than 65,535 bytes.  Basically, The Unix I/O 
    routinues only can handle the length shorter than 65,535 bytes.

Prototype:
    publicdef INT ho_write (FILE *funit, VOIDPTR record, UINT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE *)
    record          -(VOIDPTR)
    length          -(UINT)

Return Value/Status:
    Function returns either added_length or returned_length.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_write (FILE *funit, VOIDPTR record, UINT length) 
#else
publicdef INT ho_write (funit, record, length)
FILE *funit;
VOIDPTR record;
UINT length;
#endif
    {
    INT added_length = 1, returned_length = 0;
    UINT max_length = 65535;
    UINT segment_length = 0, total_length = 0;
    BYTE *segment;
    
    segment = (BYTE *)record;
    total_length = length;
    
    while ((total_length > 0) AND (added_length > 0))
        {
        segment_length = (total_length > max_length) ? max_length : total_length;
        added_length = fwrite ((VOIDPTR)segment, (size_t)segment_length, (size_t)1,
                               funit);
        segment += segment_length;
        total_length -= segment_length;
        returned_length += added_length;
        }
    if (added_length < 0)
        return added_length;
    
    return returned_length;
    }
/* END:     */
