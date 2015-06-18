/*  DEC/CMS REPLACEMENT HISTORY, Element TS_PACKSTR.C*/
/*  *2    17-AUG-1990 22:29:45 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:33:43 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_PACKSTR.C*/
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
    Pack a ragged array of strings into a long single string.

Prototype:
    publicdef CHAR *ts_pack_strings(INT num_strings, CHAR **string_pointers)

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) num_strings -(INT) number of string fragments.
    (I) string_pointers -(CHAR *ST[]) array of pointers to strings.

Return Value/Status:
    Returns pointer to start of packed string.
Scope:
    PUBLIC
    
Limitations/Assumptions:

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_pack_strings (INT num_strings, CHAR **string_pointers)
#else
publicdef CHAR *ts_pack_strings (num_strings, string_pointers)
INT num_strings;
CHAR **string_pointers;
#endif
    {
    size_t slength;
    size_t subsl;
    INT i;
    CHAR *packed_start = (CHAR *)0;
    CHAR *packed_string = (CHAR *)0;
    
    /* Calculate length of text. Sum lengths of each with <CR> after each item. */
    slength = 0;
    for (i = 0; i < num_strings; i++)
        {
        if (string_pointers[i] EQUALS (CHAR *)0) /* check for null pointer */
            {
            break;
            }
        slength += strlen ((char *)string_pointers[i]) + 1;
        }
    if (slength > 0)
        {
        /* Allocate memory for packed string. */
        packed_start = (CHAR *)tc_alloc (slength);
        
        /* Copy from scattered memory to long string. */
        for (i = 0, packed_string = packed_start; i < num_strings;
             i++, packed_string++)
            {
            strcpy ((char *)packed_string, (char *)string_pointers[i]);
            subsl = strlen ((char *)string_pointers[i]);
            packed_string += subsl;
            *packed_string = '\n';
            }
        *(--packed_string) = '\0';  /* Overwrite last <CR> */
        }
    return(packed_start);
    }
/* END:     */
