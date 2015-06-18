/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_SORT.C*/
/*  *3    17-AUG-1990 22:31:00 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2     6-FEB-1990 10:16:54 GILLESPIE "(SPR 6003) Change CHAR ** to TCP"*/
/*  *1    19-JUN-1989 13:34:28 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_TCP_SORT.C*/
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
#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Bubble sorting a table of charactor string.

Prototype:
    publicdef INT ts_tcp_sort(TCP table);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) table       -(TCP) Table of Character Pointers to be sorted.
    
Return Value/Status:
    SUCCESS - Completion of Bubble Sort.
    
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_tcp_sort (TCP table) 
#else
publicdef INT ts_tcp_sort (table)
TCP table;
#endif
    {
    INT table_length = 0;
    INT cur_index = 0;
    INT next_index = 0;
    INT skip_length = 0;
    INT back_cur_index = 0;
    INT back_next_index = 0;
    CHAR *temp_ptr;
    
    /* ********************************************************************** */
    
    /* error checking   */
    
    if (table == (TCP)0 || *table == (CHAR *)0)
        {
        return SUCCESS;
        }
    /* count the length of table.   */
    
    while (table[table_length] != (CHAR *)0)
        table_length += 1;
    if (table_length EQUALS 1)
        {
        return SUCCESS;
        }
    /* start sorting the table.   */
    
    skip_length = table_length / 2.0;
    do
        {
        cur_index = 0;
        next_index = cur_index + skip_length;
        do
            {
            
            /* if the current one is greater than next one, then Bubble sorting back.  */
            
            if (strcmp ((char *)table[cur_index], (char *)table[next_index]) > 0)
                {
                
                /* switch those two comparing string first.    */
                
                temp_ptr = table[cur_index];
                table[cur_index] = table[next_index];
                table[next_index] = temp_ptr;
                
                /* Bubble sorting backward.    */
                
                back_cur_index = cur_index;
                back_next_index = cur_index - skip_length;
                while (back_next_index >= 0)
                    {
                    if (strcmp ((char *)table[back_next_index],
                                (char *)table[back_cur_index]) > 0)
                        {
                        temp_ptr = table[back_cur_index];
                        table[back_cur_index] = table[back_next_index];
                        table[back_next_index] = temp_ptr;
                        back_cur_index = back_next_index;
                        back_next_index -= skip_length;
                        }
                    else
                        break;
                    }
                }
            cur_index = next_index;
            next_index = cur_index + skip_length;
            } while (next_index < table_length);
        skip_length = skip_length / 2.0;
        } while (skip_length >= 1);
    
    return SUCCESS;
    }
/* END:     */
