/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_DITM.C*/
/* *8    17-JUN-1991 09:12:33 KEE "(SPR -1) Add (Widget)0 checking for option menu"*/
/* *7    13-JUN-1991 14:47:36 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method"*/
/* *6    27-JUN-1990 17:28:18 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-JUN-1990 10:37:35 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *4    23-MAR-1990 23:24:43 KEE "(SPR -1) Fixing Dim for Selector (Use XtSetSensitive instead)"*/
/* *3     2-MAR-1990 11:17:47 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"*/
/* *2     5-FEB-1990 23:35:32 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:18 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_DITM.C*/
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

NAME:    WI_ENABLE_DIALOG_ITEM

DESCRIPTION:

    Enable Custom Dialog Item for Picking. This can be called
    during a dialog interaction.

    Input:

         cdlgid = (INT) custom dialog ID
         cditemid = (INT) item ID, 1 -> N.
         enable_flag  = (INT) flag for item,  0 or 1.

*************************************************************************/

#include "esi_wi_int.x"        /* external definitions required here */

#ifdef USE_X
#include "ansixt.h"
#endif

/************************************************************************/
#define IDENT progname
INT wi_enable_dialog_item(cdlgid, cditemid, enable_flag)
/************************************************************************/

INT cdlgid;
INT cditemid;
INT enable_flag;

    {
    PROGNAME("wi_enable_dialog_item");
    INT status;

#ifdef USE_X
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg    arglist[1];

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_DIALOGITEM(cditemid);
    E_CHECK_ID(cdlgid,cditemid,WI_DIALOG);
    E_CHECK_WIDGET(cdlgid,cditemid);

/* Enable or disable item. */

    /* get widget and set sensitive attribute */
    iteminfo = E_WIDGET_INFO(cdlgid,cditemid);
    itemwidget = E_GET_WIDGET(cdlgid,cditemid);

    switch (iteminfo->type_id)
      {
	  case STATTEXT:
	XtSetSensitive(itemwidget, FALSE);
	/* stattext sensitive always set to false */
	break;
	  case VSCROLL:
	  case OPTIONMENU:
	if (itemwidget != (Widget) 0)
	  { /* some vscroll is not created at wi_define_dialog since
	       they are embedded in edittext and selector already */
	    XtSetSensitive(itemwidget, (Boolean) enable_flag);
	  }
	break;
	  case BTNITEM:
	  case CHKITEM:
	  case RADIOITEM:
	  case RECTITEM:
	  case EDITTEXT:
	  case SELECTOR:
	  case VALUATOR:
	XtSetSensitive(itemwidget, (Boolean) enable_flag);
	break;
	  default:
	E_WIZ_ERROR("Invalid dialog item type",progname);
	break;
      }
    
#else        /* ESITERM code */

    wiz_jp_name(IDENT);

    E_CHECK_INIT;
    E_CHECK_DIALOGITEM(cditemid);
    E_CHECK_ID(cdlgid,cditemid,WI_DIALOG);

    SEND_4_VALUES( enable_flag, cditemid, cdlgid, MC_ENABLE_CI);

#endif

    RETURN(SUCCESS);
    }
