/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STO_LOWER.C*/
/*  *2    17-AUG-1990 22:30:07 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:54 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STO_LOWER.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Convert a string to all lower case.

Prototype:
    publicdef CHAR *ts_sto_lower(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) string in mixed case.

Return Value/Status:
    Returns input string in all lower-case.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_sto_lower (CHAR *string) 
#else
publicdef CHAR *ts_sto_lower (string)
CHAR *string;
#endif
    {
    CHAR *ps;
    
    /* ********************************************************************** */
    
    for (ps = string; *ps; ps++)    /* Scan until NUL character. */
        if (*ps >= 'A' AND * ps <= 'Z')  /* Uppercase? */
            *ps -= 'A' - 'a';
        
    return string;
    }
/* END:     */
