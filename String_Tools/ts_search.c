/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SEARCH.C*/
/*  *5     4-JAN-1991 15:16:42 GILLESPIE "(SPR 6347) Speed up searches using better termination tests"*/
/*  *4    17-AUG-1990 22:29:52 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    22-MAY-1990 15:16:53 VINCE "(SPR 1) added case insensitive search"*/
/*  *2    21-MAY-1990 12:17:48 VINCE "(SPR 1) initial checkin"*/
/*  *1    21-MAY-1990 12:04:03 VINCE "search for a substring within a string"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SEARCH.C*/
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
#include "esi_c_lib.h"
#include "esi_ctype.h"

/* Function Description -----------------------------------------------------
Description:
    This function searches a source string for a substring, and returns TRUE 
    and the position in the source string when found.
    
Prototype:
    publicdef CHAR *ts_search (CHAR *source, CHAR *search, BOOL match_case);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) source_string   -(CHAR *) string to be searched.
    (I) sub_string      -(CHAR *) string to search for.
    (I) match_case      -(BOOL) TRUE if exact case matching is to be done.

Return Value/Status:
    Returns a pointer to the beginning of sub_string within source_string.

Scope:
    PUBLIC
   
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_search (CHAR *source, CHAR *search, BOOL match_case) 
#else
publicdef CHAR *ts_search (source, search, match_case)
CHAR *source;
CHAR *search;
BOOL match_case;
#endif
    {
    CHAR *pos;
    register CHAR *startptr;
    INT slen;
    register INT i;
    register CHAR test;
    
    pos = NULL;
    if ((source == NULL) || (search == NULL) || (source[0] == NUL) ||
        (search[0] == NUL))
        return pos;
    
    slen = strlen (search);
    
    if (match_case)
        {
	test = *search;
        for (startptr = source, i = strlen(source) - slen + 1; i > 0; startptr++, i--)
            {
            if ((*startptr == test) && (strncmp (startptr, search, slen) == 0))
                {
                pos = startptr;
                break;
                }
            }
        }
    else
        {
	test = _tolower(*search);
        for (startptr = source, i = strlen(source) - slen + 1; i > 0; startptr++, i--)
            {
            if ((_tolower (*startptr) == test) &&
                (ts_strncmpi (startptr, search, slen) == 0))
                {
                pos = startptr;
                break;
                }
            }
        }
    return pos;
    }
/* END:     */
