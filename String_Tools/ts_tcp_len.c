/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_LEN.C*/
/*  *3    17-AUG-1990 22:30:57 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2     6-FEB-1990 10:16:44 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:25 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_LEN.C*/
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
    TS_TCP_LEN returns the number of character strings in an array of
    character strings. This is done by scanning the pointer array for
    a null pointer.

Prototype:
    publicdef INT ts_tcp_len(TCP strings);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) strings     -(TCP) Table of Character Pointers to determine length
                     of.
                     
Return Value/Status:
    The returned value of TS_TCP_LEN is the number of character strings
    in the array.  Calling TS_TCP_LEN with a null pointer causes a 
    length of zero to be returned.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_len (TCP strings) 
#else
publicdef INT ts_tcp_len (strings)
TCP strings;
#endif
    {
    INT len = 0;                    /* Returned length of strings array */
    
    /* ********************************************************************** */
    
    /*  Calculate length of array by scanning for the null pointer terminator.
        If a null pointer was supplied, return 0 as the length.         */
    
    if (strings != (TCP)0)
        {
        while (*strings++ != (CHAR *)0)
            {
            len++;
            }
        }
    return len;
    }
/* END:     */
