/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_ACK.C*/
/* *2     5-FEB-1990 23:27:52 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_ACK.C*/
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

NAME:    WIZ_ACKNOWLEDGE

DESCRIPTION:

     Acknowledge that all data has been received from the Mac for the
     last command.   The Mac will swallow all received data until this
     is received.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_wi_int.x"	    /* extern definitions needed here */
#include "esi_ho.h"

/************************************************************************/
publicdef INT wiz_acknowledge()
/************************************************************************

This routine does not send anything out if in PLAYBACK mode if the
'jp_nosend' variable has been set non-zero.

************************************************************************/
    {
    PROGNAME("WIZ_ACNOWLEDGE");
#define IDENT progname

/* Acknowledge Input if Terminal in receive mode. */
    if (wiv.in_receive_mode)
        {
        if (OUTPUT_ENABLED) 
            {
            hoputs("\006",1);  /* Send ASCII ACK */
            }
        wiv.in_receive_mode = FALSE;
        }

    RETURN(SUCCESS);
    }
