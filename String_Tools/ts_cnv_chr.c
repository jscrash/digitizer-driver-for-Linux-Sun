/*  DEC/CMS REPLACEMENT HISTORY, Element TS_CNV_CHR.C*/
/*  *3    17-AUG-1990 22:28:52 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-FEB-1990 19:00:50 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *1    19-JUN-1989 13:33:17 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_CNV_CHR.C*/
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
    Convert occurences of oldchar to newchar.
        
Prototype:
    publicdef CHAR *ts_convert_char(CHAR *string, INT oldchar, INT newchar);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) string to be modified.
    (I) oldchar     -(CHAR) character to be replaced.
    (I) newchar     -(CHAR) character to replace oldchar with.

Return Value/Status:
    Retunrs address of first byte converted.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_convert_char (CHAR *string, INT oldchar, INT newchar) 
#else
publicdef CHAR *ts_convert_char (string, oldchar, newchar)
CHAR *string;
INT oldchar;
INT newchar;
#endif
    {
    CHAR *p = string;
    /* ********************************************************************** */
    
    if (p == NULL)
        return string;
    
    while (*p != NUL)
        {
        if (*p EQUALS oldchar)
            {
            *p = newchar;
            }
        p++;
        }
    return string;
    }
/* END:     */
