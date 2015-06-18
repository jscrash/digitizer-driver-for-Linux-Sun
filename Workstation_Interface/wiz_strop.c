/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_STROP.C*/
/* *4     2-MAR-1990 11:43:29 JULIAN "(SPR 6012) fix ifdef USE_X == 0"*/
/* *3     2-MAR-1990 11:14:39 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *2     5-FEB-1990 23:30:25 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:23 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_STROP.C*/

/******************************************************************************

                Copyright Finder Graphics Systems, Inc. 1989
           Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND
MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

            Finder Graphics Systems, Inc.
            201 Tamal Vista Blvd
            Corte Madera, CA  USA 94925
            (415) 927-0100

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*************************************************************************

NAME:    WIZ_STRING_OP

DESCRIPTION:

    Send a string to the Mac with an opcode.

    Does nothing in PLAYBACK mode if 'jp_nosend' is non-zero.

    Input:

         str       = (CHAR *) long character string.
         opcode    = (INT) integer opcode that must read string.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_ts.h"
#include "esi_wi_int.x"	    /* external definitions needed here */

#define BIG_STRING_SIZE 25000
static CHAR big_string[BIG_STRING_SIZE];
/************************************************************************/
INT wiz_string_op (str, opcode)
/************************************************************************/

CHAR *str;
INT opcode;

    {
    PROGNAME("WIZ_STRING_OP");
    INT string_length;
    INT notblanks;
    CHAR *strsave;
    CHAR *strtemp;


/************************************************************************/

#define IDENT progname

/* Send 1 blank for empty lines. */
    string_length = (str EQUALS NULL) ? 0 : strlen(str);
/* %Q For some reason, this routine crashed when long strings were freed.
    As a temporary measure I have just allocated a very big string
    to hold the temporary copy.
*/
/*    strtemp = tc_alloc(string_length+2); */
/*    strsave = strtemp;  ** safe copy for free */
    if (string_length > BIG_STRING_SIZE)
    {
         RETURN(WI_STRING_TOO_BIG);
    }
    strcpy(big_string, str);
/*     if (string_length EQUALS 0)
    {
        big_string[0] = BLANK; big_string[1] = NUL;
        string_length = 1;
    }
    else */
    {
    /* Convert all line feeds to carriage returns. */
    /* %H required for Prime AND Mac edittext fields. */
         ts_convert_char(big_string, '\n', '\r');
     }
     notblanks = string_length;
     while ((big_string[notblanks-1] EQUALS ' ') AND (notblanks != 0))
     {
        notblanks--;
     }
     if (notblanks EQUALS 0)
     {
         big_string[0] = NUL;
         string_length = 0;
     }

/* Generate command line with length as parameter. */
    if (OUTPUT_ENABLED)  {

#ifndef USE_X 
       SEND_2_VALUES( string_length, opcode);
#endif
       hoputs(big_string,string_length);
    }

/*    free(strsave); */

    RETURN(SUCCESS);
    }
