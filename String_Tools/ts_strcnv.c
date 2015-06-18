/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STRCNV.C*/
/*  *2    17-AUG-1990 22:30:17 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:59 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STRCNV.C*/
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

#include "esi_ts.h"
#include "esi_ts_err.h"

/* Function Description -----------------------------------------------------
Description:
    This function takes a string which may contain nulls and converts
    any selected character (including nulls) to any other character.
    The seach and replace characters can be entered as decimals.

Prototype:
    publicdef INT ts_str_convert( CHAR *string, INT str_len, INT search_char, INT replace_char);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) source string.
    (I) str_len     -(INT) source string length.
    (I) serach_char -(INT) search character (to be replaced).
    (I) replace_char-(INT) replacement character.

Return Value/Status:
    SUCCESS - Successfull completion of search and replace (length of ]
              string > 1).
    Returns TS_EMPTY_STRING if length of source string is < 1.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_str_convert (CHAR *string, INT str_len, INT search_char,
                              INT replace_char) 
#else
publicdef INT ts_str_convert (string, str_len, search_char, replace_char)
CHAR *string;
INT str_len;
INT search_char;
INT replace_char;
#endif
    {
    INT i;
    
    /* ********************************************************************** */
    
    if (str_len < 1)
        {
        return TS_EMPTY_STRING;
        }
    for (i = 0; i < str_len; i++, string++)
        if (*string EQUALS search_char)
            *string = replace_char;
        
    return SUCCESS;
    }
/* END:     */
