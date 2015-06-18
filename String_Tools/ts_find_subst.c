/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FIND_SUBST.C*/
/*  *3    17-AUG-1990 22:29:05 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    21-SEP-1989 09:09:41 GILLESPIE "(SPR 1) Gulf additions"*/
/*  *1    21-SEP-1989 09:08:47 GILLESPIE "Locate substring in string"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FIND_SUBST.C*/
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
    Find the first occurence of the substring in the given string.
    
Prototype:
    publicdef INT ts_find_substring(CHAR *string, CHAR *substring, CHAR **pos);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
     (I) string     -(CHAR *) string being searched for substring.
     (I) substring  -(CHAR *) substring to be found.
     (O) pos        -(CHAR **) pointer to the first character of the substring 
                        in string.
                

Return Value/Status:
   SUCCESS - if substring located.
   FAIL - if no occurrence of substring in string. 

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_find_substring (CHAR *string, CHAR *substring, CHAR **pos) 
#else
publicdef INT ts_find_substring (string, substring, pos)
CHAR *string;
CHAR *substring;
CHAR **pos;
#endif
    {
    CHAR *p, *s;
    
    *pos = (CHAR *)0;
    if ((string == (CHAR *)0) || (substring == (CHAR *)0))
        return FAIL;
    
    p = string;
    while (*p != NUL)
        {
        for (; *p != NUL && *p != *substring; p++)
            ;
        *pos = p;
        if (*p != NUL)
            {
            for (s = substring; *s != NUL && *p == *s; p++, s++)
                ;
            if (*s == NUL)
                {
                return SUCCESS;
                }
            }
        }
    return FAIL;
    }
/* END:     */
