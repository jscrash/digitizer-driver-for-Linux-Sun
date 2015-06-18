/*  DEC/CMS REPLACEMENT HISTORY, Element TS_MAKE_TCP.C*/
/*  *2    17-AUG-1990 22:29:30 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:32 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_MAKE_TCP.C*/
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

/* Function Description -----------------------------------------------------
Description:
    Builds a table of character pointers given a contiguous series
    of null-terminated strings and a count of the number of such strings.
    Routine allocates space for the table of pointers and for each string
    to be loaded into the table.  To free the table either free each string
    individualy and then free the table or call TS_TCP_FREE.

Prototype:
    publicdef CHAR **ts_make_tcp(CHAR *string, INT nitems);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) input string to build table with.
    (I) nitems      -(INT) number of items to put in table.
    
Return Value/Status:
    Returns pointer to start of table.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR **ts_make_tcp (CHAR *string, INT nitems) 
#else
publicdef CHAR **ts_make_tcp (string, nitems)
CHAR *string;
INT nitems;
#endif
    {
    CHAR **ptab;
    CHAR **pstart;
    INT i;
    
    /* ********************************************************************** */
    
    pstart = ptab = TCP_ALLOCATE (nitems);
    
    for (i = 0; i < nitems; i++, string += strlen ((char *)string) + 1, ptab++)
        {
        *ptab = STRING_ALLOCATE (string);
        strcpy ((char *) * ptab, (char *)string);
        }
    return pstart;
    }
/* END:     */
