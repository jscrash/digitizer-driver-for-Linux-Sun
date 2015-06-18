/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FIND_CHAR.C*/
/*  *4    17-AUG-1990 22:29:02 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    26-FEB-1990 19:00:57 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *2    11-AUG-1989 20:45:50 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/*  *1    11-AUG-1989 19:46:08 CONROY "Find position of char in string"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FIND_CHAR.C*/
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
    This function is identical to the standard C function strchr().
    (It was written to work around a pointer type problem in 
    Lightspeed C.)
 
Prototype:
    publicdef CHAR *ts_find_char(CHAR *string, CHAR c);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) string to search for c in.
    (I) c           -(CHAR) character to search the string for.

Return Value/Status:
    Returns pointer to first occurence of found character.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_find_char (CHAR *string, CHAR c) 
#else
publicdef CHAR *ts_find_char (string, c)
CHAR *string;
CHAR c;
#endif
    {
    CHAR *p;
    
    for (p = string; *p != NUL && *p != c; p++)
        ;
    return p;
    }
/* END:     */
