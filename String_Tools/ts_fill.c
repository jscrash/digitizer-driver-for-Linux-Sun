/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FILL.C*/
/*  *4    28-AUG-1990 11:59:02 VINCE "(SPR 1) changed #ifdef primos || UNIX to be a #else of the previous #ifdef"*/
/*  *3    17-AUG-1990 22:28:59 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-FEB-1990 19:00:53 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *1    19-JUN-1989 13:33:22 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_FILL.C*/
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
    Place the specified fill character into ntimes contiguous bytes
    starting at the address specified by text.  The string will be
    null-terminated, so you must guarantee that you have ntimes+1
    characters available.
    
Prototype:
    publicdef CHAR * ts_fill(CHAR *text, INT fill_char, INT ntimes);

Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)
    (M) text        -(CHAR *) string to be filled by fill_char.
    (I) fill_char   -(CHAR) character to fill up text string.
    (I) ntimes      -(INT) number of times fill_char is placed in text string.

Return Value/Status:
    Returns the address of the first byte of text.

Scope:
    PUBLIC
    
Limitations/Assumptions:
    Be forewarned that this routine cannot check for the correctness of
    the value ntimes.
-----------------------------------------------------------------------------*/

/* BEGIN:   */
#if USE_PROTOTYPES
publicdef CHAR *ts_fill (CHAR *text, INT fill_char, INT ntimes) 
#else
publicdef CHAR *ts_fill (text, fill_char, ntimes)
CHAR *text;
INT fill_char;
INT ntimes;
#endif
    {
#ifdef vms
    
    /* LIMITATION: 65535 bytes */
    
    CHAR fill = fill_char;
    USHORT srclen = 0;
    USHORT destlen = ntimes;
    
    if (ntimes >= 0)
        {
        LIB$MOVC5 (&srclen, NULL, &fill, &destlen, text);
        text[ntimes] = 0;           /* Nul-terminate the string */
        }

#else

    CHAR character = fill_char;
    INT i = 0;
    
    while (i < ntimes)
        {
        text[i++] = character;
        }
    text[i] = '\0';                 /* Nul-terminate the string */

#endif
    return text;
    }
/* END:     */
