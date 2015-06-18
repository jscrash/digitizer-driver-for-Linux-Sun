/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_ITM.C*/
/* *9    17-JUN-1991 09:12:43 KEE "(SPR -1) Add (Widget)0 checking for option menu"*/
/* *8    13-JUN-1991 14:47:43 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method"*/
/* *7    24-OCT-1990 11:50:38 JULIAN "(SPR 0) fix compiling errors"*/
/* *6    23-OCT-1990 13:56:16 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:30:53 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:02:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:24:13 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:40:26 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:38:11 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_ITM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_ITM.C                              */
/* *5    27-JUN-1990 17:30:53 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:02:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:24:13 WALTERS "(SPR 6012) Fix #ifndef USE_X to #ifdef USE_X"*/
/* *2     5-FEB-1990 23:40:26 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:38:11 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_Q_DLG_ITM.C                              */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*            Finder Graphics Systems, Inc.                                         */
/*            201 Tamal Vista Blvd                                                  */
/*            Corte Madera, CA  USA 94925                                           */
/*            (415) 927-0100                                                        */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_QUERY_DIALOG_ITEM                                                     */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Query for a single item from a custom dialog box.                             */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid   = (INT) custom dialog ID.                                       */
/*         cditemid = (INT) item in custom dialog , 1 -> N.                         */
/*                                                                                  */
/*    OutPut:                                                                       */
/*                                                                                  */
/*         cdvalue  = (INT *) value of custom dialog item, usually 0 or 1.          */
/*         cdtext   = (CHAR *) text from custom dialog.                             */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
#include <Xm/Text.h>
#endif  /* ---------- END MOTIF BLOCK ---------- */

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_query_dialog_item (cdlgid, cditemid, cdvalue, cdtext)  /*************************************************************************/

INT cdlgid;
INT cditemid;
INT *cdvalue;
CHAR *cdtext;
    {
    PROGNAME ("wi_query_dialog_item");
    INT status;

#ifdef USE_X
    INT i, value, items_count;
    Boolean onoroff;
    CHAR *str;
    CHAR **tcp;
    Widget itemwidget;
    Widget selectorscrollbar, menu_button;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CStext;
    XmString *list_items;
    XmTextPosition pos;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CStext;
    DwtCompString *list_items;
    DwtTextPosition pos;
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /********************************************************************************/
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    /* I check all the funcions that call wi_query_dialog_item,                     */
    /*   only the content of the edittext and pulldnmenu are being                  */
    /*   referenced                                                                 */
    /* wi_query_dialog_item should be only referenced by edittext                   */
    /*   and pulldnmenu                                                             */
    /*   Remark : wiz_ascii_from_cmp (provided by DEC) is not quite                 */
    /*   working, therefore the cdtext of pulldnmenu is not valid                   */
    
    *cdtext = 0;
    *cdvalue = 0;
    
    E_CHECK_WIDGET (cdlgid, cditemid);
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    switch (iteminfo->type_id)
        {
    case BTNITEM:
        XtSetArg (arglist[0], XmNlabelString, &CStext);
        XtGetValues (itemwidget, arglist, 1);
        cdtext = wiz_ascii_from_cmp (CStext);
        *cdvalue = 0;
        break;
    case CHKITEM:
        onoroff = XmToggleButtonGetState (itemwidget);
        XtSetArg (arglist[0], XmNlabelString, &CStext);
        XtGetValues (itemwidget, arglist, 1);
        cdtext = wiz_ascii_from_cmp (CStext);
        *cdvalue = onoroff;
        break;
    case RADIOITEM:
        onoroff = XmToggleButtonGadgetGetState (itemwidget);
        XtSetArg (arglist[0], XmNlabelString, &CStext);
        XtGetValues (itemwidget, arglist, 1);
        cdtext = wiz_ascii_from_cmp (CStext);
        *cdvalue = onoroff;
        break;
    case STATTEXT:
        str = XmTextGetString (itemwidget);
        strcpy (cdtext, str);
        XtFree (str);
        break;
    case EDITTEXT:
        str = XmTextGetString (itemwidget); /* get the text                         */
        strcpy (cdtext, str);
        XtFree (str);
        XtSetArg (arglist[0], XmNtopPosition, &pos);
        XtGetValues (itemwidget, arglist, 1);
        *cdvalue = (INT)pos + 1;    /* cdvalue is never referenced by caller        */
        break;
    case SELECTOR:
        XtSetArg (arglist[0], XmNitems, &list_items);
        XtSetArg (arglist[1], XmNitemCount, &items_count);
        XtGetValues (itemwidget, arglist, 2);
        /* list_items return using XtGetValues is incorrect.  Since selector        */
        /*   never get call, the list_items problem will be fixed later             */
#if 0
        if (items_count > 0)
            {
            tcp = ts_tcp_alloc (items_count);
            for (i = 0; i < items_count; i++)   /* convert list of items from compound string to ascii string   */
                {
                tcp[i] = (CHAR *) tc_zalloc (sizeof(CHAR *)*ST_MAC_MAX);
                tcp[i] = wiz_ascii_from_cmp (list_items[i]);
                }
            str = ts_pack_strings (items_count, tcp);
            strcpy (cdtext, str);
            ts_tcp_free (tcp);
            tc_free (str);
            }
#endif  
        XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
        XtGetValues (itemwidget, arglist, 1);
        /* get embeded scroll bar                                                   */
        XtSetArg (arglist[0], XmNvalue, &value);
        XtGetValues (selectorscrollbar, arglist, 1);
        /* get scroll bar slider position                                           */
        *cdvalue = value;
        break;
    case VALUATOR:
        XtSetArg (arglist[0], XmNvalue, &value);
        XtGetValues (itemwidget, arglist, 1);
        *cdvalue = value;
        break;
    case OPTIONMENU:
	if (iteminfo->item_type.optionmenu.item_chosen IS_NOT_EQUAL_TO 0)
	  {
	    *cdvalue = iteminfo->item_type.optionmenu.item_chosen;
	    /* get the chosen button widget first                                       */
	    menu_button = iteminfo->item_type.optionmenu.menu_button[*cdvalue - 1];
	    XtSetArg (arglist[0], XmNlabelString, &CStext);
	    XtGetValues (menu_button, arglist, 1);
	    cdtext = wiz_ascii_from_cmp (CStext);
	  }
	    break;
    case VSCROLL:
        if (itemwidget == (Widget)NULL)
            {
            E_CHECK_WIDGET (cdlgid, cditemid - 1);
            iteminfo = E_WIDGET_INFO (cdlgid, cditemid - 1);
            itemwidget = E_GET_WIDGET (cdlgid, cditemid - 1);
            /* get the selector widget                                              */
            if (iteminfo->type_id == SELECTOR)
                {
                XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
                XtGetValues (itemwidget, arglist, 1);
                XtSetArg (arglist[0], XmNvalue, &value);
                XtGetValues (selectorscrollbar, arglist, 1);
                *cdvalue = value;
                }
            else                    /* there's no way to get edittext's scrollbar   */
                {
                E_WIZ_ERROR ("Invalid dialog item type", progname);
                }
            }
        else
            {
            XtSetArg (arglist[0], XmNvalue, &value);
            XtGetValues (itemwidget, arglist, 1);
            *cdvalue = value;
            }
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    switch (iteminfo->type_id)
        {
    case BTNITEM:
        XtSetArg (arglist[0], DwtNlabel, &CStext);
        XtGetValues (itemwidget, arglist, 1);
        cdtext = wiz_ascii_from_cmp (CStext);
        *cdvalue = 0;
        break;
    case CHKITEM:
    case RADIOITEM:
        XtSetArg (arglist[0], DwtNlabel, &CStext);
        XtSetArg (arglist[1], DwtNvalue, &onoroff);
        XtGetValues (itemwidget, arglist, 2);
        cdtext = wiz_ascii_from_cmp (CStext);
        *cdvalue = onoroff;
        break;
    case STATTEXT:
        str = DwtSTextGetString (itemwidget);
        strcpy (cdtext, str);
        XtFree (str);
        break;
    case EDITTEXT:
        str = DwtSTextGetString (itemwidget);   /* get the text                     */
        strcpy (cdtext, str);
        XtFree (str);
        XtSetArg (arglist[0], DwtNtopPosition, &pos);
        XtGetValues (itemwidget, arglist, 1);
        *cdvalue = (INT)pos + 1;    /* cdvalue is never referenced by caller        */
        break;
    case SELECTOR:
        XtSetArg (arglist[0], DwtNitems, &list_items);
        XtSetArg (arglist[1], DwtNitemsCount, &items_count);
        XtGetValues (itemwidget, arglist, 2);
        /* list_items return using XtGetValues is incorrect.  Since selector        */
        /*   never get call, the list_items problem will be fixed later             */
#if 0
        if (items_count > 0)
            {
            tcp = ts_tcp_alloc (items_count);
            for (i = 0; i < items_count; i++)   /* convert list of items from compound string to ascii string   */
                {
                tcp[i] = (CHAR *) tc_zalloc (sizeof(CHAR *)*ST_MAC_MAX);
                tcp[i] = wiz_ascii_from_cmp (list_items[i]);
                }
            str = ts_pack_strings (items_count, tcp);
            strcpy (cdtext, str);
            ts_tcp_free (tcp);
            tc_free (str);
            }
#endif  
        XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
        XtGetValues (itemwidget, arglist, 1);
        /* get embeded scroll bar                                                   */
        XtSetArg (arglist[0], DwtNvalue, &value);
        XtGetValues (selectorscrollbar, arglist, 1);
        /* get scroll bar slider position                                           */
        *cdvalue = value;
        break;
    case VALUATOR:
        XtSetArg (arglist[0], DwtNvalue, &value);
        XtGetValues (itemwidget, arglist, 1);
        *cdvalue = value;
        break;
    case OPTIONMENU:
	if (iteminfo->item_type.optionmenu.item_chosen IS_NOT_EQUAL_TO 0)
	  {
	    *cdvalue = iteminfo->item_type.optionmenu.item_chosen;
	    /* get the chosen button widget first                                       */
	    menu_button = iteminfo->item_type.optionmenu.menu_button[*cdvalue - 1];
	    XtSetArg (arglist[0], DwtNlabel, &CStext);
	    XtGetValues (menu_button, arglist, 1);
	    cdtext = wiz_ascii_from_cmp (CStext);
	  }
        break;
    case VSCROLL:
        if (itemwidget == (Widget)NULL)
            {
            iteminfo = E_WIDGET_INFO (cdlgid, cditemid - 1);
            itemwidget = E_GET_WIDGET (cdlgid, cditemid - 1);
            /* get the selector widget                                              */
            if (iteminfo->type_id == SELECTOR)
                {
                XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
                XtGetValues (itemwidget, arglist, 1);
                XtSetArg (arglist[0], DwtNvalue, &value);
                XtGetValues (selectorscrollbar, arglist, 1);
                *cdvalue = value;
                }
            else                    /* there's no way to get edittext's scrollbar   */
                {
                E_WIZ_ERROR ("Invalid dialog item type", progname);
                }
            }
        else
            {
            XtSetArg (arglist[0], DwtNvalue, &value);
            XtGetValues (itemwidget, arglist, 1);
            *cdvalue = value;
            }
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#endif  /* ---------- END DECW BLOCK ---------- */

#else        /* ESITERM code */
    INT cdlgid_temp;
    INT cditemid_temp;
    
    /* Issue request, query current state.                                          */
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    wiz_jp_name (IDENT);
    
    wiv.playback_nosend = 1;
    SEND_3_VALUES (cditemid, cdlgid, MC_QUERY_CURRENT_CI);
    wiv.playback_nosend = 0;
    
    /* Pick up report.                                                              */
    wiz_set_echo (FALSE);
    if (WAIT_RESPONSE (RQCD_CODE))
        {
        wiz_gnum (&cdlgid_temp);
        wiz_gnum (&cditemid_temp);
        wiz_gnum (cdvalue);
        wiz_gstr (8196, cdtext);    /*%Q Need longer strings.                       */
        wiz_acknowledge ();
        }
    wiz_set_echo (TRUE);
#endif
    RETURN (SUCCESS);
    }
