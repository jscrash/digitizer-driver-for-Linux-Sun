/*  DEC/CMS REPLACEMENT HISTORY, Element TS_LIKE.C*/
/*  *2    17-AUG-1990 22:29:27 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:30 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_LIKE.C*/
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

#include "esi_gl_defs.h"

/* Function Description -----------------------------------------------------
Description:
    Routine to compare a string with a pattern and return TRUE if the
    pattern matches the string, FALSE if it does not.  The pattern can
    contain a specified wild card character, which matches any number
    of characters.  Example: For the string = "ABCDE", ts_like returns
    the following (with * as the wildcard character):

         Pattern              ts_like returns
         *A*                  TRUE
         AB*DE                TRUE
         A*C*E                TRUE
         *E                   TRUE
         *BCD*                TRUE
         AB*F                 FALSE

Prototype:
    publicdef BOOL ts_like (CHAR *string, CHAR *pattern, INT wildcard);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) input string.
    (I) pattern     -(CHAR *) input pattern to match with string.
    (I) wildcard    -(INT) input wildcard character used in pattern matching.

Return Value/Status:
    TRUE  - If string matches pattern.
    FALSE - If string does not match pattern.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef BOOL ts_like (CHAR *string, CHAR *pattern, INT wildcard) 
#else
publicdef BOOL ts_like (string, pattern, wildcard)
CHAR *string;
CHAR *pattern;
INT wildcard;
#endif
    {
    CHAR *p;
    CHAR *s;
    
    /* ********************************************************************** */
    
    if (*pattern EQUALS (CHAR)wildcard)
        {
        p = pattern + 1;
        if (*p EQUALS 0)
            {
            return TRUE;
            }
        if (*string EQUALS 0)
            {
            return FALSE;
            }
        s = string;
        do
            {
            if (*p EQUALS (CHAR)wildcard)
                {
                return ts_like (s, p, wildcard);
                }
            if (*p IS_NOT_EQUAL_TO * s)
                {
                return ts_like (++string, pattern, wildcard);
                }
            if (*(++s) EQUALS 0)
                {
                return(*(++p) EQUALS 0) ? TRUE : ts_like (s, p, wildcard);
                }
            } while (*(++p));
        return ts_like (++string, pattern, wildcard);
        }
    else
        {
        if (*string EQUALS 0)
            {
            return FALSE;
            }
        if (*string++EQUALS * pattern++)
            {
            if (*pattern)
                {
                return ts_like (string, pattern, wildcard);
                }
            else
                {
                return *string ? FALSE : TRUE;
                }
            }
        }
    return FALSE;
    }
/* END:     */
