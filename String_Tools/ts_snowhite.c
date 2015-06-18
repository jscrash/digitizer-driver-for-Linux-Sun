/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SNOWHITE.C*/
/*  *2    17-AUG-1990 22:30:00 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:48 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SNOWHITE.C*/
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
#include "esi_c_lib.h"
#include "esi_ctype.h"

/* Function Description -----------------------------------------------------
Description:
    Strips leading and trailing non-printing characters from string and
    sets trailing blanks to NULL.
    
Prototype:
    publicdef CHAR *ts_snowhite(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) input string;

Return Value/Status:
    Returns a pointer to the first non-blank character; Points to null
    if string is empty.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_snowhite (CHAR *string) 
#else
publicdef CHAR *ts_snowhite (string)
CHAR *string;
#endif
    {
    CHAR *pstart;
    CHAR *pend;
    
    /* ********************************************************************** */
    
    if (string EQUALS (CHAR *)0)
        {
        return string;
        }
#if 0
    for (pstart = string; *pstart AND (NOT (isgraph (*pstart))); pstart++)
        ;
    
    for (pend = pstart + strlen ((char *)pstart) - 1;
         pend >= pstart AND (NOT isgraph (*pend));  * (pend--) = '\0')
        ;
#endif
    for (pstart = string; (*pstart != 0) AND (iscntrl (*pstart) OR isspace (*pstart)); )
        {
        pstart++;
        }
    for (pend = pstart + strlen ((char *)pstart) - 1;
         (pend >= pstart) AND (iscntrl (*pend) OR isspace (*pend)); )
        {
        *pend-- = 0;
        }
    while ((*pstart != 0) AND (*pstart EQUALS ' '))
        {
        pstart++;
        }
    while ((pend >= pstart) AND (*pend EQUALS ' '))
        {
        *pend-- = 0;
        }
    return pstart;
    }
/* END:     */
