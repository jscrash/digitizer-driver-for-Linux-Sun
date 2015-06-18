/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SHIFT.C*/
/*  *3    17-AUG-1990 22:29:56 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    21-MAY-1990 12:18:21 VINCE "(SPR 1) initial checkin"*/
/*  *1    21-MAY-1990 12:06:18 VINCE "shift a string left or right"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SHIFT.C*/
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
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    The subject_string is shifted shift_size characters left or right
    depending on the sign of shift_size.  The string is modified in place.
    A positive shift_size shifts characters right, a negitive value shifts
    characters left.

Prototype:
    publicdef CHAR *ts_shift (CHAR *string, INT size);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) string to be shifted by size amount of characters.
    (I) size        -(INT) amount of characters to shift string by.
    
Return Value/Status:
    The return value is a pointer to subject_string.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    Characters shifted off the left of the string are lost.  As characters
    are shifted right spaces will be added to the left of the string.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_shift (CHAR *string, INT size) 
#else
publicdef CHAR *ts_shift (string, size)
CHAR *string;
INT size;
#endif
    {
    INT len, ii;
    CHAR *sp, *dp;
    
    if (size == 0)
        return string;
    
    if (size < 0)
        {
        strcpy (string, string + (-size));
        }
    else
        {
        /*  set pointers to the current end-of-string byte (zero byte) and
            the new location for the zero byte.                 */
        
        len = strlen (string);
        sp = string + len;
        dp = sp + size;
        
        /*  move each character to its new location and add spaces to the
            beginning of the string.                        */
        
        for (ii = 0; ii <= len; ii++)
            *dp-- = *sp--;
        for (ii = 0; ii < size; ii++)
            string[ii] = ' ';
        }
    return string;
    }
/* END:     */
