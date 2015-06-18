/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_FREE.C*/
/*  *3    17-AUG-1990 22:30:54 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2     6-FEB-1990 10:16:35 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:23 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_FREE.C*/
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

#include "esi_tc.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    TS_TCP_FREE frees the storage associated with an array of character
    strings. The strings themselves are freed, and so is the array of
    pointers to those strings.

Prototype:
    publicdef INT ts_tcp_free(TCP strings);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) strings     -(TCP) table of strings to be freed.

Return Value/Status:
    TS_TCP_FREE returns SUCCESS if the call was SUCCESSFUL, or a status
    code from am_free or tc_free otherwise.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    TS_TCP_FREE should only be called for arrays of character strings
    whose storage was allocated, either via TS_TCP_ALLOC or otherwise.
    Calling TS_TCP_FREE on static character string arrays cannot work.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_free (TCP strings) 
#else
publicdef INT ts_tcp_free (strings)
TCP strings;
#endif
    {
    INT status;                     /*  Completion status               */
    TCP save_strings;               /*  Copy of argument                */
    
    /* ********************************************************************** */
    
    /*    Assume success.               */
    
    status = SUCCESS;
    
    /*    Do nothing if null pointer passed.        */
    
    if (strings IS_NOT_EQUAL_TO (TCP)0)
        {
        
        /*    Save pointer array address.           */
        
        save_strings = strings;
        
        /*    Free all of the strings.          */
        while (*strings IS_NOT_EQUAL_TO (CHAR *)0)
            {
            status = tc_free ((VOIDPTR) * strings);
            if (status != SUCCESS)
                break;
            strings++;
            }
        /*    Free the pointer array.           */
        
        if (status EQUALS SUCCESS)
            status = tc_free ((VOIDPTR)save_strings);
        }
    return status;
    }
/* END:     */
