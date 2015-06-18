/* DEC/CMS REPLACEMENT HISTORY, Element WI_QUERY_INFO.C*/
/* *2     5-FEB-1990 23:39:59 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:06 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_QUERY_INFO.C*/

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

NAME:    WI_QUERY_INFO

DESCRIPTION:

    Query about a specific Menu or Dialog, initialize a WI_RSRC_INFO
    structure.

    Input:

         input_id = (INT) Menu or Dialog ID from WI_RDF__
         info = (WI_RSRC_INFO *) resource info structure.

*************************************************************************/

#include "esi_wi_int.x"
#ifndef ESI_WI_RSRC_H
#include "esi_wi_rsrc.h"
#endif

/************************************************************************/
INT wi_query_info(input_id, info)
/************************************************************************/

INT input_id;
WI_RSRC_INFO *info;
{
    PROGNAME("wi_query_info");
#define IDENT progname
    INT id;
    INPUT_RECORD inp_rec;
    INT status;

/************************************************************************/

    wiz_jp_name(IDENT);

/* Retrieve from Tree */
   id = input_id;
   if (btfind(&wiv.input_tree, &id, &inp_rec))
   {
       info->numitems = inp_rec.numitems;
       if (inp_rec.type == WI_DIALOG_EVENT)  {
           info->type = TYPE_DIALOG;
       }
       else  {
           info->type = TYPE_MENU;
       }
       strcpy(info->helpkey, inp_rec.helpkey);
       strcpy(info->title, inp_rec.orig_title);

       status = SUCCESS;
   }
   else
   {
       /* Report not defined */
         status = WI_NOT_DEFINED;
   }
   RETURN(status);
}
