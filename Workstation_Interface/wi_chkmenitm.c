/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHKMENITM.C*/
/* *7    23-OCT-1990 13:53:48 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:26:34 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 20:56:16 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4     2-MAR-1990 11:43:46 MING "(SPR 6012) mod USE_X"*/
/* *3     2-MAR-1990 11:25:25 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:32:06 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHKMENITM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHKMENITM.C                              */
/* *6    27-JUN-1990 17:26:34 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 20:56:16 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4     2-MAR-1990 11:43:46 MING "(SPR 6012) mod USE_X"                           */
/* *3     2-MAR-1990 11:25:25 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:32:06 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:52 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CHKMENITM.C                              */
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
/*NAME:    WI_CHECK_MENU_ITEM                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Check or Uncheck individual menu items.                                       */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/

#define IDENT "wi_check_menu_item"
publicdef INT wi_check_menu_item (menuid, itemid, onoroff)  /*************************************************************************/

INT menuid;
INT itemid;
INT onoroff;
    {
    PROGNAME ("wi_check_menu_item");

#ifdef USE_X
    INT n, status;
    CHAR st1[ST_MAC_MAX];
    Boolean tempboolean;
    Widget pull_down_menu;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[MAX_ARG];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CSText, label;
    XtCallbackRec callback_arg[2];  /* callback list                                */
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CSText, label;
    DwtCallback callback_arg[2];    /* callback list                                */
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (itemid > 16)
        {
        RETURN (WI_BAD_COUNT);
        }
    E_CHECK_ID (menuid, itemid, WI_MENU);
    E_CHECK_WIDGET (menuid, itemid);
    
    iteminfo = E_WIDGET_INFO (menuid, itemid);
    itemwidget = E_GET_WIDGET (menuid, itemid);
    
    E_CHECK_WIDGET (menuid, 0);
    pull_down_menu = (E_WIDGET_INFO (menuid, 0))->item_type.menu.pull_down_menu;
    
    switch (iteminfo->type_id)
        {
    case MITEM:
        iteminfo->item_type.mitem.check_onoff = onoroff;
        if (onoroff == ON)
            {                       /* check the menu item (ie. toggle button on)   */
            if (iteminfo->item_type.mitem.menu_toggle_button == (Widget)NULL)
                {                   /* create check mark for push button            */
                n = 0;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[n], XmNlabelString, &label);
                n++;
                XtSetArg (arglist[n], XmNsensitive, &tempboolean);
                n++;
                XtSetArg (arglist[n], XmNacceleratorText, &CSText);
                n++;
                XtSetArg (arglist[n], XmNaccelerator, st1);
                n++;
                XtGetValues (itemwidget, arglist, n);
                n = 0;              /* set toggle button attributes                 */
                XtSetArg (arglist[n], XmNlabelString, label);
                n++;
                XtSetArg (arglist[n], XmNsensitive, tempboolean);
                n++;
                if ((CSText != (XmString)NULL) && (st1 != (CHAR *)0))
                    {
                    XtSetArg (arglist[n], XmNacceleratorText, CSText);
                    n++;
                    XtSetArg (arglist[n], XmNaccelerator, st1);
                    n++;
                    }
                sprintf (st1, "MENU_%d_%d_%d", menuid, itemid, 1);
                XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
                n++;
                callback_arg[0].callback = (XtCallbackProc)wi_callback;
                callback_arg[0].closure = (caddr_t)((menuid << 16) |
                                                    ((itemid& 0xFF) << 8) |
                                                    (1 & 0xFF));
                callback_arg[1].callback = (XtCallbackProc)NULL;
                XtSetArg (arglist[n], XmNvalueChangedCallback, callback_arg); n++;
                iteminfo->item_type.mitem.menu_toggle_button =
                    XmCreateToggleButton (pull_down_menu, st1, arglist, n);
                }
            XmToggleButtonSetState (iteminfo->item_type.mitem.menu_toggle_button, ON,
                                    FALSE);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            XtSetArg (arglist[n], DwtNlabel, &label);
            n++;
            XtSetArg (arglist[n], DwtNsensitive, &tempboolean);
            n++;
            XtSetArg (arglist[n], DwtNacceleratorText, &CSText);
            n++;
            XtSetArg (arglist[n], DwtNbuttonAccelerator, st1);
            n++;
            XtGetValues (itemwidget, arglist, n);
            n = 0;                  /* set toggle button attributes                 */
            XtSetArg (arglist[n], DwtNlabel, label);
            n++;
            XtSetArg (arglist[n], DwtNsensitive, tempboolean);
            n++;
            if ((CSText != (DwtCompString)NULL) && (st1 != (CHAR *)0))
                {
                XtSetArg (arglist[n], DwtNacceleratorText, CSText);
                n++;
                XtSetArg (arglist[n], DwtNbuttonAccelerator, st1);
                n++;
                }
            sprintf (st1, "MENU_%d_%d_%d", menuid, itemid, 1);
	    XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]); n++;
            callback_arg[0].proc = (VoidProc)wi_callback;
            callback_arg[0].tag = (menuid << 16) | ((itemid& 0xFF) << 8) | (1 & 0xFF);
            callback_arg[1].proc = (VoidProc)NULL;
            XtSetArg (arglist[n], DwtNvalueChangedCallback, callback_arg); n++;
            iteminfo->item_type.mitem.menu_toggle_button =
                DwtToggleButtonCreate (pull_down_menu, st1, arglist, n);
            }
        DwtToggleButtonSetState (iteminfo->item_type.mitem.menu_toggle_button, ON,
                                 FALSE);
#endif  /* ---------- END DECW BLOCK ---------- */
        XtUnmanageChild (itemwidget);
        XtManageChild (iteminfo->item_type.mitem.menu_toggle_button);
        }
    else
        {
        if (iteminfo->item_type.mitem.menu_toggle_button != (Widget)NULL)
            {
            XtUnmanageChild (iteminfo->item_type.mitem.menu_toggle_button);
            XtManageChild (itemwidget);
            }
        }
    break;
    case MSEPERATOR:
        break;
    default:
    E_WIZ_ERROR ("Invalid dialog item type", progname);
    break;
    }
#else        /* ESITERM Code */
    INT status;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (itemid > 16)
        {
        RETURN (WI_BAD_COUNT);
        }
    E_CHECK_ID (menuid, itemid, WI_MENU);
    
    SEND_4_VALUES (onoroff, itemid, menuid, MC_CHECK_MI);
#endif 
    RETURN (SUCCESS);
    }
