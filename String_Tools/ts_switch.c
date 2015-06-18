/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SWITCH.C*/
/*  *2    17-AUG-1990 22:30:27 VINCE "(SPR 5644) Code Cleanup"*/
/*  *1    19-JUN-1989 13:34:01 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_SWITCH.C*/
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
#include "esi_c_lib.h"

/* Function Description -----------------------------------------------------
Description:
    This routine takes a string and a Table of Character Pointers and returns
    the index of the given string within the tcp as the function value.
    This can be used to setup a switch statement where the item to be used as
    the switch is a character string.

       i.e.
        static CHAR valid_op_codes[] = { "OPEN", "CLOSE", "QUIT", 0 };

        switch (ts_switch ( op_code, valid_op_codes)) {
           case 0:  * OPEN    *  break;
           case 1:  * CLOSE   *  break;
           case 2:  * QUIT    *  break;
           default: * invalid *  break;
        }

Prototype:
    publicdef INT ts_switch (CHAR *string, CHAR **tcp);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (I) string      -(CHAR *) string to build table with.
    (M) tcp         -(CHAR **) Table of Character Pointers.
    
Return Value/Status:
    Function returns the index of a string in a Table of Character Pointers
    (i.e. function value is INT position (0 to N) or -1 if not found).

Scope:
    PUBLIC
    
Limitations/Assumptions:
    Note: 1. The TCP must be well formed; terminated by a null pointer.
          2. The first string is returned as index 0.
          3. The returned value will be -1 if the string is not found.

-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef INT ts_switch (CHAR *string, CHAR **tcp) 
#else
publicdef INT ts_switch (string, tcp)
CHAR *string;
CHAR **tcp;
#endif
    {
    INT i;
    
    for (i = 0; *tcp; tcp++, i++)
        {
        if (strcmp ((char *)string, (char *) * tcp) EQUALS 0)
            {
            return(i);
            }
        }
    return(-1);                     /* not found. */
    }
/* END:     */
