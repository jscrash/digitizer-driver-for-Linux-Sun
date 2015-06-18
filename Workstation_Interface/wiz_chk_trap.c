/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_CHK_TRAP.C*/
/* *2     5-FEB-1990 23:28:02 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:35:46 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_CHK_TRAP.C*/

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

NAME:    WIZ_CHECK_TRAPPED

DESCRIPTION:

    Check to see if an event is trapped internally.
    If it is trapped, perform the appropriate action.

*************************************************************************/

#define TEMP_HELP_LEN 16

#include "esi_wi_int.x"	    /* external definitions needed here */
#include "esi_ts.h"

static INT (*gb_address)();

/************************************************************************/
INT wiz_check_trapped(event_class, id, item_id)
/************************************************************************/

INT event_class;
INT id;
INT item_id;
    {
    PROGNAME("wiz_check_trapped");
#define IDENT progname
    INT if_trapped;

    CHAR temp_help[TEMP_HELP_LEN];

/************************************************************************/

    if_trapped = FALSE;
    switch(event_class)
    {
    case WI_MENU_EVENT:
        {
/* Check for global menu processing. */
	
        if_trapped = (*gb_address)(id, item_id);
        break;
        }

/* Trap help requests. */
    case WI_DIALOG_EVENT:
        {
        strncpy(temp_help,wiv.hold.cdtext,TEMP_HELP_LEN-2);
	temp_help[TEMP_HELP_LEN-2] = 0;
        ts_snowhite(temp_help);
        ts_sto_lower(temp_help);
        if (strcmp(HELP_STRING, temp_help ) EQUALS 0)
            {
            if_trapped = TRUE;
            wi_do_help(id);
            break;
            }
        }

    default:
        {
        if_trapped = FALSE;
        }
    } /* End switch. */

    return(if_trapped);
    }

/********************************************************************

NAME: WIZ_CHK_TRAP_INIT

DESCRIPTION: 

Store the address of gb_process_globals function.
This removes the dependency of wi routines on gb.

********************************************************************/

INT wiz_chk_trap_init(addr)
INT (*addr)();

    {
    gb_address = addr;
    return SUCCESS;
    }
