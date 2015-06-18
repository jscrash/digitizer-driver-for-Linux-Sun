/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PUSH_INT.C*/
/* *2     5-FEB-1990 23:29:13 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_PUSH_INT.C*/

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

NAME:    WIZ_PUSH_INT

DESCRIPTION:

    Push an internal integer onto the WI stack.

    Input:

         IVAL = (INT) some integer.

*************************************************************************/

#include "esi_wi_int.x"	    /* external definitions needed here */
#include "esi_er.h"
#include "esi_mg.h"

/************************************************************************/
INT wiz_push_int(ival)
/************************************************************************/

INT ival;

    {
    PROGNAME("wiz_push_int");
#define IDENT progname
    INT status;

                                  /* Check for stack bounds. */

    if ((wiv.wi_stack_pointer + 1) >= WI_STACK_SIZE)
        {
        status = WI_STACK_OVF;
        er_log(mg(WI_STACK_OVF));
        }
    else
        {
        status = SUCCESS;

                                  /* Push value. */
        wiv.wi_stack[wiv.wi_stack_pointer++] = ival;
        }

    RETURN (status);
    }
