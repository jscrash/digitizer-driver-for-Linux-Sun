/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_HLPKEY.C*/
/* *2     5-FEB-1990 23:41:22 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:16 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_HLPKEY.C*/

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

NAME:    WI_QUERY_HELPKEY

DESCRIPTION:

    Query the helpkey of a specific Menu or Dialog

    Input:

         input_id = (INT) Menu or Dialog ID from WI_RDF__
         helpkey = (CHAR *) Help key.

*************************************************************************/

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

/************************************************************************/
INT wi_query_helpkey(input_id, helpkey)
/************************************************************************/

INT input_id;
CHAR *helpkey;
{
    PROGNAME("wi_query_helpkey");
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
       strcpy(helpkey, inp_rec.helpkey);
       status = SUCCESS;
   }
   else
   {
       /* Report not defined */
         status = WI_NOT_DEFINED;
   }
   RETURN(status);
}
