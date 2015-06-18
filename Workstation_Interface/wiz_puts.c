/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PUTS.C*/
/* *2     5-FEB-1990 23:29:24 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PUTS.C*/

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

NAME:    WIZ_PUTS

DESCRIPTION:

    Send a string to the workstation

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_wi_int.x"		/* external definitions needed here */
#include "esi_ho.h"

/************************************************************************/
INT wiz_puts (str)
/************************************************************************

This routine does not send anything out if in PLAYBACK mode if the
'jp_nosend' variable has been set non-zero.

************************************************************************/

CHAR *str;

    {
    PROGNAME("WIZ_PUTS");
#define IDENT progname

/* this acknowledgement function moved to wiz_gets()  */
/* Acknowledge Input if Terminal in receive mode. */
    if (wiv.in_receive_mode)
        {
        if (OUTPUT_ENABLED)
            {
            hoputs("\006",1);  /* Send ASCII ACK */
            }
        wiv.in_receive_mode = FALSE;
        }

/* Send string using host dependant routine. */
    if (OUTPUT_ENABLED)
        {
        hoputs(str, strlen(str));
        }

    RETURN(SUCCESS);
    }
