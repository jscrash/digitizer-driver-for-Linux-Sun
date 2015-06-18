/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_ALLOC.C*/
/*  *4    17-AUG-1990 22:30:34 VINCE "(SPR 5644) Code Cleanup"*/
/*  *3    26-FEB-1990 19:01:12 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *2     6-FEB-1990 10:16:02 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:08 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_ALLOC.C*/
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
    TS_TCP_ALLOC allocates room for an array of character strings. Only
    the pointer array is allocated. The pointers in the array are
    initially null. You must specify how many character strings are needed
    in the "len" parameter.

Prototype:
    publicdef TCP ts_tcp_alloc(INT len);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) len         -(INT) number of strings to alocate memory for.
    
Return Value/Status:
    TS_TCP_ALLOC returns a pointer to the allocated pointer array as it's
    value.  Specifying a "len" of zero causes the NULL pointer to be 
    returned.

Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef TCP ts_tcp_alloc (INT len) 
#else
publicdef TCP ts_tcp_alloc (len)
INT len;
#endif
    {
    TCP strings;                    /* Returned strings array pointer */
    
    /* ********************************************************************** */
    
    /*  If 0 specified, do nothing.  Otherwise allocate memory for the specified 
        number of pointers, plus one for the terminating null pointer.      */
    
    if (len)
        strings = (TCP)tc_zalloc ((size_t)((len + 1) * sizeof(CHAR *)));
    else
        strings = (TCP)NULL;
    
    return strings;
    }
/* END:     */
