/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_SCOM.C*/
/* *2     5-FEB-1990 23:29:50 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_SCOM.C*/

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

NAME:    WIZ_SCOM

DESCRIPTION:

    send command line to MAC with <CR>

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_wi_int.x"		/* external definitions needed here */

/************************************************************************/
INT wiz_scom (str)
/************************************************************************/

CHAR *str;

    {
    PROGNAME("WIZ_SCOM");
#define IDENT progname
    CHAR stl[ST_COMMAND_MAX];

/* Put MAC ON and OFF characters at beginning and end of line. */
    sprintf(stl,"%c %s %c", wiv.macon, str, wiv.macoff);
    wiz_puts(stl);

    RETURN(SUCCESS);
    }
