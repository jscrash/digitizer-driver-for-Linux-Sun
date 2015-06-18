/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FOR2C.C*/
/*  *3    23-AUG-1990 17:23:38 JESSIE "(SPR 1) fix include files"*/
/*  *2    17-AUG-1990 22:29:08 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:24 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FOR2C.C*/
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
    Converts a Fortran string to a C string.  It strips trailing non-printing 
    characters from the string and sets trailing blanks to NULL.
     
Prototype:
    publicdef CHAR *ts_fortran_to_c(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) input string to convert.

Return Value/Status:
    Returns a pointer to the first character of the input string, points to 
    null if string is empty.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_fortran_to_c (CHAR *string) 
#else
publicdef CHAR *ts_fortran_to_c (string)
CHAR *string;
#endif
    {
/******************************************/
    CHAR *pend;
/******************************************/
    if (string EQUALS (CHAR *)0)
        {
        return string;
        }
    for (pend = string + strlen ((char *)string) - 1;
         pend >= string AND (NOT isgraph (*pend)); pend--)
        {
        *pend = '\0';
        }
    return string;
    }
/* END:     */
