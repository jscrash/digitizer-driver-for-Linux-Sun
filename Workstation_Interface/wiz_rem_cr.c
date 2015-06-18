/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_REM_CR.C*/
/* *2     5-FEB-1990 23:29:33 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_REM_CR.C*/

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

NAME:    WIZ_REMOVE_CR

DESCRIPTION:

    Remove any trailing carriage return in string.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_c_lib.h"
#include "esi_wi_int.h"

/************************************************************************/
#define IDENT "wiz_remove_cr"
INT wiz_remove_cr(string)
/************************************************************************/

CHAR *string;
{
    INT slen;

/*************************************************************************/

    slen = strlen(string);
    if (string[slen-1] EQUALS '\n')
    {
	 string[slen-1] = NUL;
    }

    return (SUCCESS);
}
