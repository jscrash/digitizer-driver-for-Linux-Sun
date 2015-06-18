/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_DITMS.C*/
/* *3    27-JUN-1990 17:29:03 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *2     5-FEB-1990 23:36:51 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:36 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_DITMS.C*/
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

NAME:    WI_ENABLE_DIALOG_ITEMS

DESCRIPTION:

    Enable or Disable individual custom dialog items using array.

    Input:

         cdlgid     = (INT) id of custom dialog as allocated by Mac.
         numitems   = (INT) number of items to modify.
         do_or_dont = (INT ARRAY) array of flags for whether to set or not.
         onoroff     = (INT ARRAY) flags to set dialog items to.

*************************************************************************/

#include "esi_gl_defs.h"
#include "esi_wi_int.x"        /* external variables accessed here */

/************************************************************************/
#define IDENT "wi_enable_dialog_items"
INT wi_enable_dialog_items(cdlgid, numitems, do_or_dont, onoroff)
/************************************************************************/

INT cdlgid;
INT numitems;
INT do_or_dont[MAX_CD_ITEMS];
INT onoroff[MAX_CD_ITEMS];

    {
    INT i;
    INT status;

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_COUNT(numitems);
    E_CHECK_ID(cdlgid,0,WI_DIALOG);

/* Loop through flags. */
    for (i=0; i < numitems; i++)
      {
        if(do_or_dont[i] != FALSE)
	  {
            wi_enable_dialog_item(cdlgid, i, onoroff[i]);
	  }
      }
    
    RETURN(SUCCESS);
  }
