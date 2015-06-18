/* DEC/CMS REPLACEMENT HISTORY, Element WI_MESSAGE.C*/
/* *3     2-MAR-1990 11:32:16 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:37:55 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_MESSAGE.C*/
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

NAME:    WI_MESSAGE

DESCRIPTION:

    Send message to Macintosh

    Input:
         MESSAGE = (CST).

*************************************************************************/


#include "esi_wi_int.x"
#include "esi_c_lib.h"

/************************************************************************/
#define IDENT progname
INT wi_message(message)
/************************************************************************/

CHAR *message;
{

    PROGNAME("wi_message");
/************************************************************************/

#ifdef USE_X
    printf("%s\n",message);                 /* send message to standard out */
#else        /* ESITERM code */
    SEND_LINEOUT(message);
#endif
    RETURN(SUCCESS);
}
