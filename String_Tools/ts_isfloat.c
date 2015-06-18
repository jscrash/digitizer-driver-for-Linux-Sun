/*  DEC/CMS REPLACEMENT HISTORY, Element TS_ISFLOAT.C*/
/*  *2    17-AUG-1990 22:29:23 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:27 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_ISFLOAT.C*/
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

#ifndef ESI_C_LIB_H

#include "esi_c_lib.h"

#endif

#ifndef ESI_TS_H

#include "esi_ts.h"

#endif

#include "esi_ctype.h"

#define MAX_STRING_LENGTH 128

/* Function Description -----------------------------------------------------
Description:
    Function isfloat checks a C string to see if it is a well-formed 
    floating-point number.
    
Prototype:
    publicdef INT ts_isfloat(CHAR *x);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) x           -(CHAR *) input strring to be checked.

Return Value/Status:
    Returns zero if the input string is null or blank.
    Returns +n if successfully passed float test: n is number of 
        characters scanned.
    Returns -n if error detected; n is position where error 
        condition encountered.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    Input string may have trailing blanks converted to NUL.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_isfloat (CHAR *x) 
#else
publicdef INT ts_isfloat (x)
CHAR *x;
#endif
    {
    INT position;
    CHAR *ps;
    CHAR string[MAX_STRING_LENGTH];
    BOOL exponent = FALSE;
    BOOL decimal = FALSE;
    
    /* ********************************************************************** */
    
    if (x EQUALS (CHAR *)0 OR (strlen ((char *)x) EQUALS 0))
        {
        return 0;
        }
    /*
       * skip leading white space and make copy for local mods
    */
    ps = ts_snowhite ((CHAR *)strcpy ((char *)string, (char *)x));
    position = 1;
    
    if (!*ps)
        {
        return 0;
        }
    /*
       * check for leading sign, digit or decimal point
    */
    else if (!isdigit (*ps) AND * ps != '+' AND * ps != '-' AND * ps != '.')
        {
        return -1;
        }
    else                            /*
                                       * accept only digit,v decimal point or E (for exponents) after 1st character
                                    */
        for (ps++, position++; *ps; ps++, position++)
            {
            if (position > MAX_STRING_LENGTH)
                {
                return -position;
                }
            else if (isdigit (*ps))
                continue;
            else if (*ps EQUALS '.')
                {
                /*
                   * allow only one decimal point per string
                */
                if (decimal OR exponent)
                    {
                    return -position;
                    }
                decimal = TRUE;
                continue;
                }
            /*
               * allow only one exponent (E or e) per string
            */
            else if (*ps EQUALS 'E' OR * ps EQUALS 'e')
                {
                if (exponent)
                    {
                    return -position;
                    }
                if (*(ps + 1) EQUALS '+' OR * (ps + 1) EQUALS '-')
                    {
                    ps++;
                    position++;
                    }
                exponent = TRUE;
                continue;
                }
            else
                {
                return -position;
                }
            }
    /* 1/18/89 GS - handle single "+" or "-" correctly (return error) */
    if (position == 2 && !isdigit (x[0]))
        {
        return -1;
        }
    return position - 1;
    }
/* END:     */
