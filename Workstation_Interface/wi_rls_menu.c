/* DEC/CMS REPLACEMENT HISTORY, Element WI_RLS_MENU.C*/
/* *4    27-JUN-1990 17:31:35 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 12:14:34 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:43:58 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:48 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RLS_MENU.C*/
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

NAME:    WI_RELEASE_MENU

DESCRIPTION:

    Release a menu from memory on the Mac.

    Input:

         menuid = (INT) identifier for menu.

*************************************************************************/

#include "esi_wi_int.x"

/************************************************************************/
publicdef INT wi_release_menu (menuid)
/************************************************************************/

INT menuid;
    {
    PROGNAME ("wi_release_menu");
#define IDENT progname
    INT id;
    INT status;

    wiz_jp_name (IDENT);

    E_CHECK_INIT;
    E_CHECK_ID (menuid, 0, WI_MENU);

    /* Delete from tree, report if not there. */
    id = menuid;
    if (btdel (&wiv.input_tree, &id))
        {
#ifdef USE_X
        wiz_delete_widget (menuid);
#else
        SEND_2_VALUES (menuid, MC_RELEASE_MN);
#endif
        RETURN (SUCCESS);
        }
    else
        {
        RETURN (WI_TREE_DELETE_ERR);
        }

    }

