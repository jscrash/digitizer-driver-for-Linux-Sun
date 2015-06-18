/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_SEL.C*/
/* *2     5-FEB-1990 23:44:45 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:58 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_RQ_SEL.C*/
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

NAME:    WI_REQUEST_SELECTION

DESCRIPTION:

    Request selection from the user.

    Input:

         prompt   = (CST) prompt for user.

    Out:

         numselected = (INT *) number of selections picked.
         selected    = (INT ARRAY) the selections.
         status      = (INT *) FAIL -> Cancelled.
                               SUCCESS -> OK.

*************************************************************************/

#include "esi_wi_int.x"
#ifndef WI_STOCK_RH
#include "wi_stock.rh"
#endif

/************************************************************************/
publicdef INT wi_request_selection(prompt, numselected, selected, status)
/************************************************************************/

CHAR prompt[STMAX];
INT *numselected;
INT selected[MAX_CD_SELECTABLE];
INT *status;
{
    PROGNAME("wi_request_selection");
#define IDENT progname
    INT cditemid;
    INT iquit;
    INT cdvalue;
    INT dlgid;
    CHAR cdtext[STMAX];

/************************************************************************/

    wiz_jp_name(IDENT);

/* Define custom dialog. */
    if (wiv.stock_select_cdlgid < 0)
    {
         *status = FAIL;
         RETURN(WI_STOCK_SELECTOR_UNDEFINED);
    }

/* Push existing interactive environment. */
    wi_push();

/* Load prompt. */
    wi_set_dialog_text(wiv.stock_select_cdlgid, CDI_WI_SSEL_PROMPT, prompt);
    wi_open_dialog(wiv.stock_select_cdlgid);

/* Request custom dialog pick */
    iquit = FALSE;
    do
    {
         wi_request_dialog_item(&dlgid, &cditemid, &cdvalue, cdtext);

/* Act on Inputs. */
         switch(cditemid)
         {
/* Process item selected. */
         case CDI_WI_SSEL_OK:
         /* Ask for list of selections. */
              wi_query_selections(wiv.stock_select_cdlgid, CDI_WI_SSEL_SELECTOR,
                     numselected, selected);
              *status = SUCCESS;
              iquit = TRUE;
              break;

         case CDI_WI_SSEL_CANCEL:
              *status = FAIL;
              *numselected = 0;
              iquit = TRUE;
              break;
         }
     } while (iquit EQUALS FALSE);

    wi_close_dialog(wiv.stock_select_cdlgid);
    wi_pop();

    RETURN(SUCCESS);
}
