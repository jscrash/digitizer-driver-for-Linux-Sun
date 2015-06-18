/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SUB_SUBSTR.C*/
/*  *4    23-AUG-1990 17:25:43 JESSIE "(SPR 1) fix include files"*/
/*  *3    17-AUG-1990 22:30:23 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    11-AUG-1989 20:46:43 CONROY "(SPR 0) Revised global, selects, SQL macros & fixes"*/
/*  *1    11-AUG-1989 19:46:35 CONROY "Subsitute substring"*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SUB_SUBSTR.C*/
/*****************************************************************************/
/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1990
                    Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

                            Finder Graphics Systems
                            201 Tamal Vista Blvd.
                            Corte Madera, CA  94925
                            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

#include "esi_ts.h"

/* Function Description -----------------------------------------------------
Description:
    Replace all occurences of find_string with replace_string in source string.
            
Prototype:
    publicdef INT ts_sub_substring(CHAR *string, CHAR *find_string, CHAR *replace_string);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) string      -(CHAR *) source string being searched.
    (I) find_string -(CHAR *) the substring to find.
    (I) replace_string-(CHAR *) the substring to substitute.

Return Value/Status:
    SUCCESS - If substitution achieved;
    FAIL    - If find_string not found.
 
Scope:
    PUBLIC
   
Limitations/Assumptions:
    The substitution is performed in-place, so caller must ensure there is
    enough room, if replace_string is longer than find_string.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_sub_substring (CHAR *string, CHAR *find_string, CHAR *replace_string) 
#else
publicdef INT ts_sub_substring (string, find_string, replace_string)
CHAR *string;
CHAR *find_string;
CHAR *replace_string;
#endif
    {
    INT status = FAIL;
    INT i, lf, lr, rest, shift;
    CHAR *pos, *c1, *c2;
    
    if (string == (CHAR *)0)
        return FAIL;
    if (find_string == (CHAR *)0)
        return FAIL;
    if (replace_string == (CHAR *)0)
        return FAIL;
    
    lf = strlen (find_string);
    lr = strlen (replace_string);
    
    /*--------------------------------------------------   */
    /*   shift > 0  shift right by (lr-lf) characters      */
    /*   shift < 0  shift left by (lr-lf) characters       */
    /*--------------------------------------------------*/
    
    shift = lr - lf;
    
    pos = string;
    
    while (ts_find_substring (pos, find_string, &pos) == SUCCESS)
        {
        status = SUCCESS;
        
        /*------------------------------------*/
        /* determine number of chars to shift */
        /*------------------------------------*/
        
        rest = strlen (pos) - lf;
        if (rest > 0)
            {
            if (shift > 0)
                {
                c1 = pos + strlen (pos) + shift;
                *c1 = '\0';
                c1--;
                c2 = pos + strlen (pos) - 1;
                for (i = 0; i < rest; i++, c1--, c2--)
                    *c1 = *c2;
                }
            else if (shift < 0)
                {
                c1 = pos + lf + shift;
                c2 = pos + lf;
                for (i = 0; i < rest; i++, c1++, c2++)
                    *c1 = *c2;
                *c1 = '\0';
                }
            }
        /*----------------------------------*/
        /* substitute replace_string at pos */
        /*----------------------------------*/
        
        c1 = pos;
        c2 = replace_string;
        for (i = 0; i < lr; i++, c1++, c2++)
            *c1 = *c2;
        
        /*-----------------------------------------------------------------*/
        /* if we replaced the end of the string, make sure null-terminated */
        /*-----------------------------------------------------------------*/
        
        if (rest <= 0)
            *c1 = '\0';
        
        }
    return status;
    }
/* END:     */

