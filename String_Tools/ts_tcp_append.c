/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_APPEND.C*/
/*  *3    17-AUG-1990 22:30:41 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2     6-FEB-1990 10:16:26 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:16 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_APPEND.C*/
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
#include "esi_tc.h"
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Append a string to the end of an existing TCP.  The null
    pointer is used to point to the new string and a new null
    pointer is created.

Prototype:
    publicdef TCP ts_tcp_append(TCP *tcp, CHAR *string);
    
Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) tcp         -(TCP *) source Table of Character Pointers.
    (I) string      -(CHAR *) string to insert into TCP.

Return Value/Status:
    Returns the modified TCP.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef TCP ts_tcp_append (TCP *tcp, CHAR *string) 
#else
publicdef TCP ts_tcp_append (tcp, string)
TCP *tcp;
CHAR *string;
#endif
    {
    INT i;
    INT len = 0;
    TCP local_tcp;
    
    /*  Search for the null pointer  */
    
    i = 0;
    if (*tcp EQUALS (TCP)NULL OR ** tcp EQUALS (CHAR *)NULL)
        {
        local_tcp = *tcp = (TCP)tc_zalloc ((size_t)(sizeof(CHAR *) * 2));
        }
    else
        {
        local_tcp = *tcp;
        len = ts_tcp_len (local_tcp);
        for (i = 0; i < len; i++)
            {
            if (local_tcp[i] EQUALS (CHAR *)0)
                {
                local_tcp[i] = (CHAR *)tc_zalloc ((size_t)(strlen ((char *)string) +
                                                           1));
                strcpy ((char *)local_tcp[i++], (char *)string);
                local_tcp[i] = (CHAR *)0;
                return *tcp;
                }
            }
        *tcp = local_tcp = (TCP)tc_realloc ((BYTE *)local_tcp,
                                            (size_t)((i + 2) * sizeof(CHAR *)));
        }
    local_tcp[i] = (CHAR *)tc_zalloc ((size_t)(strlen ((char *)string) + 1));
    strcpy ((char *)local_tcp[i++], (char *)string);
    local_tcp[i] = (CHAR *)0;
    
    return *tcp;
    }
/* END:     */
