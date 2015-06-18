/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_GSTR.C*/
/* *2     5-FEB-1990 23:28:28 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:53 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_GSTR.C*/

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

NAME:    WIZ_GSTR

DESCRIPTION:

    Get a string from the MAC.
    The MAC will send the length on one line. Following the carriage return
    will be that number of characters.  They might contain several newlines.

*************************************************************************/

#include "esi_c_lib.h"
#include "esi_wi_int.h"

/************************************************************************/
publicdef INT wiz_gstr(lenresp, stresp)
/************************************************************************/

INT lenresp;
CHAR *stresp;
{
    CHAR strin[1024];  /* Allow for longer input strings just in case. */
    CHAR dummy[2];
    INT slen;
    INT len_accum;

/************************************************************************/

/* Get length from MAC. */
    wiz_gnum(&slen);

/* Get the long string in substring units. */
    len_accum = 0;
    *stresp = NUL; /* Clear the response string. */
    if (slen <= lenresp)
    {
	 while(len_accum <= slen)  /*%Q For 0 length, read 1 empty line. */
	 {
	      wiz_gets(strin);
	      len_accum += strlen(strin);
	      strcat(stresp, strin);
	 }
    }
    else
    {
	strcpy(stresp, "Input String is too Long.");
    }

/* Remove last <CR> to avoid confusing INFO. */
    wiz_remove_cr(stresp);

    return (SUCCESS);
}
