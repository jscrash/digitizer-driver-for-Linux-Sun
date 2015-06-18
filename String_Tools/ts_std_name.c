/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STD_NAME.C*/
/*  *4    17-AUG-1990 22:30:04 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3     6-SEP-1989 09:53:19 GORDON "(SPR 200) Gulf changes (August trip)"*/
/*  *2    20-JUL-1989 10:57:21 GORDON "(SPR 100) Add mods from Sun port"*/
/*  *1    19-JUN-1989 13:33:51 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STD_NAME.C*/
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

#include "esi_ctype.h"
#include "esi_c_lib.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    TS_STANDARD_NAME converts a string of words separated by
    blanks ( or by the underscore character ) into a consistent format
    where the first character of each word is upper case, 
    otherwise lowercase, and the blanks have been converted to
    underscores.  Leading & trailing blanks and control characters are
    removed.
    The conversion is done in place in the input string, and the
    returned pointer points to the first character of the 
    standard name (similar to usage of ts_snowhite).

Prototype:
    publicdef CHAR *ts_standard_name(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) input character string containing tokens 
                        to be parsed.

Return Value/Status:
    Returned pointer points to the first character of the 
    standard name (similar to usage of ts_snowhite).

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_standard_name (CHAR *string) 
#else
publicdef CHAR *ts_standard_name (string)
CHAR *string;
#endif
    {
    CHAR *p;                        /* Current character */
    CHAR *s;                        /* Pointer to returned standard name */
    INT count;                      /* Char count in current "word" */
    
    count = 1;
    for (s = p = string; *p; p++, count++)
        {
        if (*p == ' ')
            {
            *p = '_';
            count = 0;
            }
        else if (*p == '_')
            count = 0;
        else
            {
            if (count == 1)
                *p = _toupper (*p);
            else
                *p = _tolower (*p);
            }
        }
    return s;
    }
/* END:     */
