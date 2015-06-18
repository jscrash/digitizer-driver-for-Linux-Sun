/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DF_MEN.C*/
/* *2     5-FEB-1990 23:42:30 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RD_DF_MEN.C*/

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

NAME:    WI_READ_DEFINE_MENU

DESCRIPTION:

    Read a menu from a resource and define it.

    Input Parameters:

         mrsid = (INT) menu resource ID.
         help_key = (CST) key into help tree.

    Output Parameters:

         numitems = (INT *) number of items in menu.
         menuid = (INT *) unique menu ID.

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
INT wi_read_define_menu(mrsid, help_key, numitems, menuid)
/************************************************************************/

INT mrsid;
INT *menuid;
INT *numitems;
CHAR help_key[];
{
    PROGNAME("wi_read_define_menu");
#define IDENT progname
    INT id;
    INPUT_RECORD inp_rec;
    MENU tempmenu;
    INT status;

/************************************************************************/

    wiz_jp_name(IDENT);

    E_CHECK_INIT;

/* Read and define menu. */

    status = wi_read_menu(mrsid, &tempmenu);
    if (!status) {
         *numitems = tempmenu.num_mn_items;
         status = wi_define_menu(&tempmenu, help_key, menuid);
         }
    if (!status) {
/* Set record for tracking. */
         inp_rec.type = WI_MENU_EVENT;
         inp_rec.numitems = tempmenu.num_mn_items;
         inp_rec.owner_id = wiv.current.owner_id;
         strcpy (inp_rec.owner_tag, wiv.current.owner_tag);

    /* if the rsrc file didn't define a helpkey, use the passed in string... */
         if (wiv.current.rsrc_helpkey[0] EQUALS NUL)
              strcpy (inp_rec.helpkey, help_key);
         else
              strcpy (inp_rec.helpkey, wiv.current.rsrc_helpkey);

         id = *menuid;
         btadd(&wiv.input_tree, &id, &inp_rec);
         }

    RETURN(status);
}
