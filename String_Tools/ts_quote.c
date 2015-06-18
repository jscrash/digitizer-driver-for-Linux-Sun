/*  DEC/CMS REPLACEMENT HISTORY, Element TS_QUOTE.C*/
/*  *4    16-OCT-1990 15:04:37 GILLESPIE "(SPR 1000) Merge Ernie deltas"*/
/*   2B1  12-OCT-1990 18:01:00 GILLESPIE "Merge Ernie Deltas"*/
/*  *3    17-AUG-1990 22:29:49 VINCE "(SPR 5644) Code Cleanup"*/
/*  *2    26-FEB-1990 19:01:08 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)"*/
/*  *1    19-JUN-1989 13:33:46 SYSTEM ""*/
/*  DEC/CMS REPLACEMENT HISTORY, Element TS_QUOTE.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element TS_QUOTE.C                                  */
/* *2    10-OCT-1990 16:25:38 VINCE "(SPR 1) modified to quote even an empty string."   */
/* *1    13-AUG-1990 17:17:05 SYSTEM "Finder 6.1"                                   */
/* DEC/CMS REPLACEMENT HISTORY, Element TS_QUOTE.C                                  */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Exploration Systems, Inc. 1989                          */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY   */
/*CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE      */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Exploration Systems, Inc.                                               */
/*          579 Market Street                                                       */
/*          San Francisco, CA  USA 94105                                            */
/*          (415) 974-1600                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    TS_QUOTE                                                              */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    This routine will duplicate any occurance of the given quote character        */
/*    within the string and add the quote character to the beggining and end        */
/*    of the string.  The string is modified in place and a pointer to the          */
/*    first byte of the string is returned as the function value.  The string       */
/*    must have been declared or allocated to have enough space for the original    */
/*    string plus the number of added qoute characters (minimum of 2).              */
/*                                                                                  */
/*    Input Arguments                                                               */
/*    ---------------                                                               */
/*    CHAR *string; input string.                                                   */
/*    CHAR quote_char;  single character used as the quote character                */
/*          (either '\'' or '"').                                                   */
/*                                                                                  */
/*    Output Arguments                                                              */
/*    ----------------                                                              */
/*    NONE                                                                          */
/*                                                                                  */
/*    Function Value                                                                */
/*    --------------                                                                */
/*    CHAR * to first character of input string.                                    */
/*    Points to the null byte if string is empty.                                   */
/*                                                                                  */
/*   ************************************************************************       */

#include "esi_ts.h"
#include "esi_c_lib.h"
#include "esi_tc.h"

/* ************************************************************************         */
#if USE_PROTOTYPES
publicdef CHAR *ts_quote (CHAR *string, CHAR quote_char)
#else
publicdef CHAR *ts_quote (string, quote_char)
CHAR *string;                       /*  string to be processed.                     */
CHAR quote_char;                    /*  character to use as a quote (either '\'' or '"')    */
#endif
    {
    CHAR *temp;                     /*  allocated work space                        */
    CHAR *tp;                       /*  working pointer for work space              */
    CHAR *cp;                       /*  working pointer for input string            */
    INT nquotes;                    /*  number of quote characters to be added (at least 2) */
    INT len;                        /*  original length of input string             */
    
    /* can't do anything with a NULL pointer                                        */
    if (string == NULL)
        return string;
    
    len = strlen (string);
    /* NOTE: process the string even if len == 0, the result will be ""             */
    
    /*  how many quotes will we need                                                */
    nquotes = 2;                    /*  one for each end                            */
    for (cp = string; *cp; cp++)
        {
        if (*cp == quote_char)
            nquotes++;
        }
    /*  allocate a work area                                                        */
    tp = temp = (CHAR *) TALLOC (len + nquotes + 1);
    
    /*  process the string                                                          */
    *tp++ = quote_char;             /*  initial qoute                               */
    for (cp = string; *cp; cp++)
        {
        *tp++ = *cp;
        if (*cp == quote_char)
            *tp++ = quote_char;     /* internal qoute                               */
        }
    *tp++ = quote_char;             /*  terminal qoute                              */
    *tp = NUL;                      /*  string terminator                           */
    
    /*  copy the new string over the                                                */
    /*    original and free the work space                                          */
    strcpy (string, temp);
#if NEED_TO_FREE
    tc_free (temp);
#endif
    return string;
    }
