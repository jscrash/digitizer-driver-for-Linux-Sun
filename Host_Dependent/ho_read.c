/* DEC/CMS REPLACEMENT HISTORY, Element HO_READ.C*/
/* *3    17-AUG-1990 21:53:11 VINCE "(SPR 5644) Code Cleanup"*/
/* *2    18-JUL-1989 15:35:28 GORDON "(SPR 100) Add mods from Sun port"*/
/* *1    19-JUN-1989 12:59:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element HO_READ.C*/
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
    The purpose of this routinue is to read in a buffer  
    length longer than 65,535 bytes.  Basically, The Unix I/O 
    routines only can handle a length shorter than 65,535 bytes.

Prototype:
    publicdef INT ho_read (FILE *funit, VOIDPTR record, UINT length);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    funit           -(FILE *)
    record          -(VOIDPTR)
    length          -(UINT)

Return Value/Status:
    Function returns the lengths of the new buffer.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ho_read (FILE *funit, VOIDPTR record, UINT length) 
#else
publicdef INT ho_read (funit, record, length)
FILE *funit;
VOIDPTR record;
UINT length;
#endif
    {
    INT added_length = 1, returned_length = 0;
    UINT max_length = 65535;
    UINT segment_length = 0;
    BYTE *segment;
    
    segment = (BYTE *)record;
    
    while ((length > 0) AND (added_length > 0))
        {
        segment_length = (length > max_length) ? max_length : length;
        added_length = fread ((VOIDPTR)segment, (size_t)segment_length, (size_t)1,
                              funit);
        segment += segment_length;
        length -= segment_length;
        returned_length += added_length;
        }
    if (added_length == 0)          /* we failed */
        return(INT)added_length;
    
    return(INT)returned_length;
    }
/* END:     */
