/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SUBS.C*/
/*  *4     4-JAN-1991 15:16:46 GILLESPIE "(SPR 6347) Speed up searches using better termination tests"*/
/*  *3    17-AUG-1990 22:30:20 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    21-MAY-1990 12:18:33 VINCE "(SPR 1) initial checkin"*/
/*  *1    21-MAY-1990 12:07:14 VINCE "substitute one substring for another within a string"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SUBS.C*/
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

#include "esi_c_lib.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    This function substitutes the replace_string for the search_string 
    in the subject_string if the iterate_flag is true.  Recursion is used 
    to continue the search and replacement operation to the end of the 
    subject_string.

Prototype:
    publicdef INT func (arg);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) subject     -(CHAR *) source string for the substitions.
    (I) search      -(CHAR *) substring to be searched for and replaced.
    (I) replace     -(CHAR *) replacement string.
    (I) iflag       -(BOOL) iteration flag; TRUE if substition should be repeated.

Return Value/Status:
    Subject_string is modified in place and returned value is a pointer to
    subject_string.

Scope:
    PUBLIC
    PRIVATE to <module_description>

Limitations/Assumptions:
    Note: The recursion proceeds from the character following the current
    substition point; This prevents circular references i.e. change 
    "xxx" to "yxxxy" in "aaaxxxaaaxxxxxx" will result in the string
    "aaayxxxyaaayxxxyyxxxy" instead of an infinite recursion.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_subs (CHAR *subject, CHAR *search, CHAR *replace, BOOL iflag) 
#else
publicdef CHAR *ts_subs (subject, search, replace, iflag)
CHAR *subject;
CHAR *search;
CHAR *replace;
BOOL iflag;
#endif
    {
    INT slen;
    CHAR *sp;
    
    if (IS_EMPTY_STRING(search))
        return subject;             /* no search string given. */
    
    sp = ts_search (subject, search, FALSE);
    if (sp == NULL)
        return subject;             /* search string not found */
    
    slen = strlen (search);
    ts_shift (sp, -slen);           /* shift left to remove search string   */
    ts_insert (sp, replace);        /* insert replacement string        */
    
    if (iflag)
        {
        sp += strlen (replace);     /* move past current replacement */
        ts_subs (sp, search, replace, iflag);   /* replace again         */
        }
    return subject;
    }
/* END:     */
