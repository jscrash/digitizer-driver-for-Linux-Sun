/* DEC/CMS REPLACEMENT HISTORY, Element TS_STRCMPI.C */
/* *3    17-AUG-1990 22:30:13 VINCE "(SPR 5644) Code Cleanup" */
/* *2    22-MAY-1990 15:18:25 VINCE "(SPR 1) initial checkin" */
/* *1    22-MAY-1990 14:22:21 VINCE "case insensitive string compare" */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_STRCMPI.C */
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
#include "esi_ts.h"
#include "esi_ctype.h"

/* File Description ---------------------------------------------------------
Overview:
    These two functions implement case-insensitive versions of strcpy() and 
    strncmp().
    
Prototypes and one line descriptions of each function in the file.
Public_Functions:
    publicdef INT ts_strcmpi (CHAR *master, CHAR *pattern);
    This implements case-insensitive strcmp();
    
    publicdef INT ts_strncmpi (CHAR *master, CHAR *pattern, INT len);
    This implements case-insensitive strncmp();

-----------------------------------------------------------------------------*/

/* Function Description -----------------------------------------------------
Description:
    This function compares two strings, ignoring upper or lower case.  It will
    compare up to the length of pattern.
    
Prototype:
    publicdef INT ts_strcmpi (CHAR *master, CHAR *pattern);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) master      -(CHAR *) source string to compare with.
    (I) pattern     -(CHAR *) string to compare master with.
    
Return Value/Status:
    Returns either 0 if successful, or the difference between the pattern 
    and master strings.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_strcmpi (CHAR *master, CHAR *pattern) 
#else
publicdef INT ts_strcmpi (master, pattern)
CHAR *master, *pattern;
#endif
    {
    return ts_strncmpi (master, pattern, strlen (pattern));
    }
/* END:     */

/* Function Description -----------------------------------------------------
Description:
    This function compares two strings, ignoring upper or lower case.  It will 
    compare the pattern string with the master string up to length len.
    
Prototype:
    publicdef INT ts_strcmpi (CHAR *master, CHAR *pattern, INT len);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) master      -(CHAR *) source string to compare with.
    (I) pattern     -(CHAR *) string to compare master with.
    (I) len         -(INT) compares master up to length len.
    
Return Value/Status:
    Returns either 0 if successful, or the difference between the pattern 
    and master strings.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_strncmpi (CHAR *master, CHAR *pattern, INT len) 
#else
publicdef INT ts_strncmpi (master, pattern, len)
CHAR *master, *pattern;
INT len;
#endif
    {
    INT val;
    CHAR *mp, *pp;
    
    mp = master;
    pp = pattern;
    val = len;
    
    while (*mp && (val-- > 0) && *pp && (_tolower (*mp) == _tolower (*pp)))
        {
        mp++;
        pp++;
        }
    if (*pp == NUL)
        return 0;
    
    return _tolower (*pp) - _tolower (*mp);
    }
/* END:     */
