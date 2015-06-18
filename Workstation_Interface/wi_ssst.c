/* DEC/CMS REPLACEMENT HISTORY, Element WI_SSST.C*/
/* *2     5-FEB-1990 23:49:13 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:43 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SSST.C*/
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

NAME:    WI_SSST

DESCRIPTION:

    Set stock selector box.

    Input:

         HELPKEY = (CST) Help string.
         NUM_SELECTIONS = (INT) Number of selections in list.
         MAX_SELECTABLE = (INT) Maximum items the user can select at one time.
         SELECTIONS = (CST *ARRAY) Array of pointers to item strings.

*************************************************************************/

#include "esi_wi_int.x"
#include "esi_am.h"
#ifndef WI_STOCK_RH
#include "wi_stock.rh"
#endif

static CHAR WI_RSRC_FILE[] = {"WI_STOCK"};
/************************************************************************/
publicdef INT wi_ssst(help_key, num_selections, max_selectable, selections)
/************************************************************************/

INT num_selections, max_selectable;
CHAR *selections[MAX_CD_SELECTIONS];
CHAR help_key[STMAX];
{
    PROGNAME("WI_SSST");
#define IDENT progname

/************************************************************************/

    wiz_jp_name(IDENT);

/* Define custom dialog. */
    if (wiv.stock_select_cdlgid < 0)
    {
       am_define_resource(AM_DIALOG,WI_RSRC_FILE, CDLG_WI_SELECT, 
              NULL_FUNCTION_PTR, &wiv.stock_select_cdlgid, 
              (VOIDPTR) 0);
    }

/* Load choices into selector. */
    wi_set_selections(wiv.stock_select_cdlgid,
              CDI_WI_SSEL_SELECTOR,
              CDI_WI_SSEL_SCROLLBAR,
              num_selections,
              max_selectable,
              selections);

    RETURN(SUCCESS);
}
