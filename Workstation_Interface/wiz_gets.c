/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_GETS.C*/
/* *2     5-FEB-1990 23:28:10 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_GETS.C*/
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

NAME:    WIZ_GETS

DESCRIPTION:

    Get a line from the workstation

*************************************************************************/

#include "esi_wi_int.x"	    /* external definitions needed here */
#include "esi_ho.h"

/************************************************************************/
INT wiz_gets (str)
/************************************************************************/

CHAR *str;
    {
    PROGNAME("WIZ_GETS");
#define IDENT progname
    INT stdnr;
    INT status;
    INT lenstr;

    if (wiv.jpmode == WI_PLAYBACK)
        {
        wiz_jp_read_line();
        strcpy(str, wiv.jpstring);
        }
    else
        {
        hogets(str);
        if (wiv.jpmode == WI_JOURNAL)
            {
            wiz_jp_write_string(str);
            }
        }

/* Set flag to provide acknowledgement to terminal. */
    wiv.in_receive_mode = TRUE;

    RETURN(SUCCESS);
    }
