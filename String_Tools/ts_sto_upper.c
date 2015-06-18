/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STO_UPPER.C*/
/*  *2    17-AUG-1990 22:30:10 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:56 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_STO_UPPER.C*/
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
    Convert a string to all UPPER case.

Prototype:
    publicdef CHAR *ts_sto_upper(CHAR *string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) string in mixed case.

Return Value/Status:
    Returns pointer to input string converted to all upper-case.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_sto_upper (CHAR *string) 
#else
publicdef CHAR *ts_sto_upper (string)
CHAR *string;
#endif
    {
    CHAR *ps;
    
    /* ********************************************************************** */
    
    for (ps = string; *ps; ps++)    /* Scan until NUL character. */
        if (*ps >= 'a' AND * ps <= 'z')  /* Lowercase? */
            *ps -= 'a' - 'A';
        
    return string;
    }
/* END:     */
