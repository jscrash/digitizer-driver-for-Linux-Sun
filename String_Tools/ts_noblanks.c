/*  DEC/CMS REPLACEMENT HISTORY, Element TS_NOBLANKS.C*/
/*  *2    17-AUG-1990 22:29:42 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:41 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_NOBLANKS.C*/
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
    This function removes blanks from a text string. The original string is
    modified in place.  The resulting deblanked string is passed back 
    as the argument and as the function parameter.

Prototype:
    publicdef CHAR *ts_noblanks(CHAR *text);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) text        -(CHAR *) string to be de-blanked.

Return Value/Status:
    The resulting deblanked string is passed back as the argument and as
    the function parameter.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_noblanks (CHAR *text) 
#else
publicdef CHAR *ts_noblanks (text)
CHAR *text;
#endif
    {
    INT i;
    INT count;
    
    /* ********************************************************************** */
    
    count = 0;
    i = 0;
    
    while (text[i] != '\0')
        {
        if (text[i] != ' ')
            {
            text[count] = text[i];
            count++;
            }
        i++;
        }
    text[count] = '\0';
    
    return text;
    }
/* END:     */
