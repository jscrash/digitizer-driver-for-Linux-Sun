/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_POP_INT.C*/
/* *2     5-FEB-1990 23:28:47 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:59 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_POP_INT.C*/

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

NAME:    WIZ_POP_INT

DESCRIPTION:

    Pop an internal integer from the WI stack.

    RETURN:

        Value on top of WI stack.

*************************************************************************/

#include "esi_wi_int.x"	    /* external definitions needed here */
#include "esi_mg.h"
#include "esi_er.h"

/************************************************************************/
INT wiz_pop_int()
/************************************************************************/
    
    {
    PROGNAME("wiz_pop_int");
#define IDENT progname
    INT value;

                                  /*   Check for stack bounds. */

    if (wiv.wi_stack_pointer <= 0) 
        {
        value = 1;
        er_log(mg(WI_STACK_UNDERFLOW));
        }
    else
        value = wiv.wi_stack[--wiv.wi_stack_pointer];

    return value;
    }
