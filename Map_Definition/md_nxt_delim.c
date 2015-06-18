/* DEC/CMS REPLACEMENT HISTORY, Element MD_NXT_DELIM.C*/
/* *2    17-AUG-1990 21:57:36 VINCE "(SPR 5644) Code Cleanup"*/
/* *1    19-JUN-1989 13:07:03 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element MD_NXT_DELIM.C*/
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

#include "esi_c_lib.h"
#include "esi_gl_defs.h"
#include "esi_md_defs.h"
#include "esi_md.h"
#include "esi_md_err.h"

/* Function Description -----------------------------------------------------
Description:
    Function to find the next overlay SEPARATOR, skipping anything
    enclosed in parenthesis.

Prototype:
    publicdef CHAR *md_next_delim(CHAR *phrase,INT *status);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    phrase          -(CHAR *) String to search.
    status          -(INT *) Return status.

Return Value/Status:
    SUCCESS - Successful completion.
    MD_UNBALANCED_PARENS
    MD_BAD_KEYWORD_END
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *md_next_delim (CHAR *phrase, INT *status)
#else
publicdef CHAR *md_next_delim (phrase, status)
CHAR *phrase;
INT *status;
#endif
    {
    
    PROGNAME ("MD_NEXT_DELIM");
    INT paren_lev = -1;
    CHAR *temp;                     /* DUMMY STRING POINTER */
    
    /* ********************************************************************** */
    
    *status = SUCCESS;
    
    temp = phrase;
    FOREVER
        {
        if (*temp != '\0')
            {
            if (paren_lev <= 0 AND * temp EQUALS SEPARATOR)
                {
                break;
                }
            else if (paren_lev EQUALS 0 AND * temp != ' ')
                {
                *status = MD_BAD_KEYWORD_END;
                temp = 0;
                break;
                }
            else if (*temp EQUALS OPEN_PAREN)
                {
                if (paren_lev EQUALS - 1)
                    {
                    paren_lev = 1;
                    }
                else
                    {
                    paren_lev++;
                    }
                }
            else if (*temp EQUALS CLOSE_PAREN)
                {
                paren_lev--;
                }
            temp++;
            }
        else
            {
            temp = 0;
            break;
            }
        }
    if (paren_lev > 0)
        {
        *status = MD_UNBALANCED_PARENS;
        temp = 0;
        }
    return temp;
    }
/* END:     */
