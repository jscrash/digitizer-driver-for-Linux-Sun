/* DEC/CMS REPLACEMENT HISTORY, Element WI_TERMINATE.C*/
/* *4     9-AUG-1990 22:18:10 GILLESPIE "(SPR 2001) Change host dependency switch"*/
/* *3     2-MAR-1990 11:07:43 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *2     5-FEB-1990 23:49:43 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_TERMINATE.C*/
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

/* ************************************************************************

   NAME:    WI_TERMINATE

   DESCRIPTION:

     Terminate Workstation Interface.

   ************************************************************************ */

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_RS_H
#include "esi_rs.h"
#endif

#ifdef vms

#ifndef ESI_HO_H
#include "esi_ho.h"
#endif

#ifndef IODEF_H
#include "iodef.h"
#endif

#endif

/* ********************************************************************** */
INT wi_terminate()
/* ********************************************************************** */
{
    PROGNAME("wi_terminate");
#define IDENT progname

    E_CHECK_INIT;

    wiz_jp_name(IDENT);

/* Release stock dialogs. */
    if (wiv.stock_select_cdlgid >= 0)
    {
         wi_release_dialog(wiv.stock_select_cdlgid);
    }

/* Free record tree */
    btfree(&wiv.input_tree);

/* Close workstation resource module. */
    rsmdclos(1);

#ifdef USE_X
    exit(1);
#else 
    SEND_1OPCODE(MC_TERM);

    wiz_restore_terminal(0);

                                  /*   Terminal type is now VT100. */

    wiv.ittype = 0;

                                  /*   Indicate no longer initialized. */

    wiv.ifwiinit = FALSE;

    RETURN (SUCCESS);

#endif
}

