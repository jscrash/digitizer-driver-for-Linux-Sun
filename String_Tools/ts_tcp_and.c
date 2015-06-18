/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_AND.C*/
/*  *3    17-AUG-1990 22:30:37 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2     6-FEB-1990 10:16:12 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:13 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_AND.C*/
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
    TS_TCP_AND finds the intersection of two lists of CHAR pointers.

Prototype:
    publicdef INT ts_tcp_and(TCP table1, TCP table2, TCP *table3);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) table1      -(TCP) first table to check for intersection.
    (I) table2      -(TCP) second table to check for intersections.
    (O) table3      -(TCP *) resulting table of interesections.
    
Return Value/Status:
    SUCCESS - After intersections are found.
   
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_and (TCP table1, TCP table2, TCP *table3) 
#else
publicdef INT ts_tcp_and (table1, table2, table3)
TCP table1;
TCP table2;
TCP *table3;
#endif
    {
    INT t1_len = 0;
    INT t2_len = 0;
    INT t3_len = 0;
    INT i;
    INT top = 0;
    INT middle = 0;
    INT bottom = 0;
    BOOL found = FALSE;
    TCP table_ptr;
    CHAR *ptr;
    
    /* ********************************************************************** */
    
    /*  error checking    */
    
    if (table1 == (TCP)NULL || table2 == (TCP)NULL || *table1 == (CHAR *)NULL ||
        *table2 == (CHAR *)NULL)
        {
        return SUCCESS;
        }
    /* count the length of table.    */
    
    while (table1[t1_len] != (CHAR *)0)
        t1_len++;
    while (table2[t2_len] != (CHAR *)0)
        t2_len++;
    
    /* sort two tables.   */
    
    ts_tcp_sort (table1);
    ts_tcp_sort (table2);
    t3_len = (t1_len > t2_len) ? t2_len : t1_len;
    *table3 = table_ptr = TCP_ALLOCATE (t3_len);
    
    /* binary search for the intersection.   */
    
    for (i = 0; i < t1_len; i++)
        {
        found = FALSE;
        top = 0;
        bottom = t2_len - 1;
        while (top <= bottom && !found)
            {
            middle = (top + bottom) / 2.0;
            if (ARE_SAME ((char *)table1[i], (char *)table2[middle]))
                found = TRUE;
            else
                {
                if (strcmp ((char *)table1[i], (char *)table2[middle]) > 0)
                    {
                    top = middle + 1;
                    }
                else
                    bottom = middle - 1;
                }
            }
        /* if found the same string, copy to returned table.   */
        
        if (found)
            {
            ptr = STRING_ALLOCATE (table1[i]);
            strcpy ((char *)ptr, (char *)table1[i]);
            *table_ptr++ = ptr;
            }
        }
    return SUCCESS;
    }
/* END:     */
