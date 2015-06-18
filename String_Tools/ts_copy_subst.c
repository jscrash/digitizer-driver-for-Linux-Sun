/*  DEC/CMS REPLACEMENT HISTORY, Element TS_COPY_SUBST.C*/
/*  *3    17-AUG-1990 22:28:56 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    21-SEP-1989 09:09:37 GILLESPIE "(SPR 1) Gulf additions"*/
/*  *1    21-SEP-1989 09:08:22 GILLESPIE "Copy substring"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_COPY_SUBST.C*/
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

#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    This function copies the substring from start_ptr to end_ptr 
    into the out_string (properly null-termianted).
    
Prototype:
    publicdef INT ts_copy_substring(CHAR *out_string, CHAR *start_ptr, CHAR *end_ptr);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) start_ptr   -(CHAR *) start of source string.
    (I) end_ptr     -(CHAR *) end of source string.
    (M) out_string  -(CHAR *) destination string.

Return Value/Status:
    SUCCESS - If copy is successfully completed.
    FAIL    - Fails if any string or substring is not defined correctly. 
    
Scope:
    PUBLIC
    
Limitations/Assumptions:
    The caller must ensure that out_string is large enough to
    contain the result.
-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_copy_substring (CHAR *out_string, CHAR *start_ptr, CHAR *end_ptr) 
#else
publicdef INT ts_copy_substring (out_string, start_ptr, end_ptr)
CHAR *start_ptr, *end_ptr;
CHAR *out_string;
#endif
    {
    CHAR *c1, *c2;
    
    if (out_string == (CHAR *)0)
        return FAIL;
    if (start_ptr == (CHAR *)0)
        return FAIL;
    if (end_ptr == (CHAR *)0)
        return FAIL;
    
    for (c1 = out_string, c2 = start_ptr; c2 <= end_ptr; c1++, c2++)
        *c1 = *c2;
    *c1 = NUL;
    return SUCCESS;
    }
/* END:     */
