/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DF_DLG.C*/
/* *2     5-FEB-1990 23:42:17 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:27 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DF_DLG.C*/

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

NAME:    WI_READ_DEFINE_DIALOG

DESCRIPTION:

    Read a custom dialog from a resource and define it.

    Input Parameters:

         cdrsid = (INT) custom dialog resource ID.
         help_key = (CST) key into help tree.

    Output Parameters:

         numitems = (INT) number of items defined in dialog.
         cdlgid = (INT *) unique custom dialog ID.

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
INT wi_read_define_dialog(cdrsid, help_key, numitems, cdlgid)
/************************************************************************/

INT cdrsid;
INT *cdlgid;
INT *numitems;
CHAR help_key[];
{
    PROGNAME("wi_read_define_dialog");
#define IDENT progname
    INT id;
    INT readstat;
    CUSTOM_DIALOG tempcdlg;
    INT status;

/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

                                  /*   Read  custom dialog. */

    status = wi_read_dialog(cdrsid, &tempcdlg);

    if (!status) {
         *numitems = tempcdlg.num_cd_items;

                                  /*   Define dialog on MAC from structure. */

         status = wi_define_dialog(&tempcdlg, help_key, cdlgid);
         }

    RETURN(status);
}
