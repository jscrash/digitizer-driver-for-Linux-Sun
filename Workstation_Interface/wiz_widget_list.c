/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WIDGET_LIST.C */
/* *21    9-AUG-1991 15:03:21 MING "(SPR 0) put #endif of #ifdef USE_X to the end of the file" */
/* *20   15-JUL-1991 12:09:49 KEE "(SPR -1) Add Busy Cursor when execute Finder application code" */
/* *19   18-JUN-1991 13:01:40 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1 */
/*version" */
/* *18   13-JUN-1991 14:46:26 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method" */
/* *17    3-MAY-1991 16:13:14 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version" */
/* *16    4-DEC-1990 09:53:18 KEE "(SPR -1) fix isfloatstr bug" */
/* *15   31-OCT-1990 11:57:19 KEE "(SPR -1) Fix esi$make compile noise of last check in" */
/* *14   31-OCT-1990 11:08:32 GILLESPIE "(SPR 37) Add esi_ctype.h, USE_PROTOTYPES" */
/* *13   30-OCT-1990 11:32:32 KEE "(SPR -1) Fix compile noise at sun" */
/* *12   23-OCT-1990 13:52:51 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *11   28-JUN-1990 18:30:01 KEE "(SPR -1) don't set input focus before fi_pop" */
/* *10   27-JUN-1990 17:25:37 KEE "(SPR -1) New wi data structure using 2-D array" */
/* *9    12-JUN-1990 10:36:45 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)" */
/* *8     3-MAY-1990 21:48:39 KEE "(SPR -1) menu sync fix." */
/* *7    12-APR-1990 21:09:09 JULIAN "(SPR -1) changed ansiDwt to ansidwt" */
/* *6    21-MAR-1990 18:30:04 KEE "(SPR 5142) consume callback_list at wi_callback instead of Mac's wi_rq_event" */
/* *5    15-MAR-1990 13:35:26 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc " */
/* *4     2-MAR-1990 11:42:09 MING "(SPR 6012) mod USE_X" */
/* *3     8-FEB-1990 17:56:01 JULIAN "(SPR -1) X Version some minor fixing (Kee)" */
/* *2     5-FEB-1990 23:30:52 JULIAN "(SPR -1) X version of WI Module (Kee)" */
/* *1     5-FEB-1990 23:17:44 JULIAN "WIZ CALL FOR X " */
/* DEC/CMS REPLACEMENT HISTORY, Element WIZ_WIDGET_LIST.C */
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1989                      */
/*           Unpublished -- All rights reserved                                     */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAinsIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE   */
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

#include "esi_wi_int.x"

#ifdef USE_X

#ifndef ESI_X_H
#include "esi_x.h"
#endif

#include "ansixt.h"
#include <X11/cursorfont.h>

#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#ifndef ESI_ER_H
#include "esi_er.h"
#endif

#ifndef ESI_CTYPE_H
#include "esi_ctype.h"
#endif

#ifndef ESI_TS_H
#include "esi_ts.h"
#endif

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifndef ESI_CONF_WS_H
#include "esi_conf_ws.h"
#endif

#ifndef ESI_WM_INT_H
#include "esi_wm_int.h"
#endif

static INT event_cdlgid;
static INT event_itemid;
static INT event_class;

#if USE_PROTOTYPES
static INT isalnumstr (CHAR *str);
static INT isdigitstr (CHAR *str);
static INT isfloatstr (CHAR *str);
static INT wiz_set_event_info (INT cdlgid, INT cditemid);
static INT wiz_free_menu_syn_list (MENU_SYN_LIST *menu_syn_ptr);
#else
static INT isalnumstr ();
static INT isdigitstr ();
static INT isfloatstr ();
static INT wiz_set_event_info ();
static INT wiz_free_menu_syn_list ();
#endif

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    STATTEXT_SENSITIVE_FALSE                                              */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*  Reset all the stattext sensitive to be false of the pop up dialog: widget       */
/*                                                                                  */
/*   ************************************************************************       */

#if USE_PROTOTYPES
publicdef INT wiz_stattext_sensitive_false (INT cdlgid)
#else
publicdef INT wiz_stattext_sensitive_false (cdlgid)
INT cdlgid;
#endif
    {
    INT i, status;
    Arg arglist[1];
    
    if (1 <= cdlgid && cdlgid <= wiv.dialog_menu_count)
        {
        E_CHECK_WIDGET (cdlgid, 0);
        for (i = 1; i <= (E_DIALOG_MENU (cdlgid))->num_items; i++)
            {
            E_CHECK_WIDGET (cdlgid, i);
            if ((E_WIDGET_INFO (cdlgid, i))->type_id == STATTEXT)
                {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[0], XmNsensitive, FALSE);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                XtSetArg (arglist[0], DwtNsensitive, FALSE);
#endif  /* ---------- END DECW BLOCK ---------- */
                XtSetValues (E_GET_WIDGET (cdlgid, i), arglist, 1);
                }
            }
        return(SUCCESS);
        }
    else
        {
        am_message (AM_STATUS,
     "Dialog/Menu id or item id out of range in Routine wiz_stattext_sensitive_false");
        return(FAIL);
        }
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    DELETE_WIDGET                                                         */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Delete widget list given cdlg id or menu id                                   */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_delete_widget (INT dialog_menu_id)
#else
publicdef INT wiz_delete_widget (dialog_menu_id)
INT dialog_menu_id;
#endif
    {
    INT i, j, k, status;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    DIALOG_MENU *thedialog_menu;
    
    if (1 <= dialog_menu_id && dialog_menu_id <= wiv.dialog_menu_count)
        {
        E_CHECK_WIDGET (dialog_menu_id, 0);
        if ((E_DIALOG_MENU (dialog_menu_id))->num_items != 0)   /* not delete       */
            {
            thedialog_menu = E_DIALOG_MENU (dialog_menu_id);
            if (thedialog_menu->items != (WIDGET_ITEM_INFO *)0)
                {
                wiz_unmanagechild_widget (dialog_menu_id);
                /* unmanage all the widget first                                    */
                for (i = thedialog_menu->num_items; i >= 0; i--)
                    {
                    E_CHECK_WIDGET (dialog_menu_id, i);
                    iteminfo = E_WIDGET_INFO (dialog_menu_id, i);
                    itemwidget = E_GET_WIDGET (dialog_menu_id, i);
                    
                    if (iteminfo->text != (CHAR *)0)
                        {
                        tc_free (iteminfo->text);
                        }
                    switch (iteminfo->type_id)
                        {
                    case SELECTOR:
                        tc_free (iteminfo->item_type.selector.sel_list);
                        break;
                    case OPTIONMENU:
                        k = ts_tcp_len (iteminfo->item_type.optionmenu.tcp);
                        if (k > 0)
                            {
                            for (j = 0; j < k; j++)
                                {
                                XtDestroyWidget (iteminfo->item_type.optionmenu.
                                                 menu_button[j]);
                                }
                            tc_free (iteminfo->item_type.optionmenu.menu_button);
                            ts_tcp_free (iteminfo->item_type.optionmenu.tcp);
			    iteminfo->item_type.optionmenu.tcp = (TCP)0;
                            XtDestroyWidget (iteminfo->item_type.optionmenu.
                                             pull_down_menu);
                            }
                        break;
                    case RADIOITEM:
                        if (iteminfo->item_type.radioitem.radio_box != (Widget)NULL)
                            {
                            XtDestroyWidget (iteminfo->item_type.radioitem.radio_box);
                            }
                        break;
                    case DIALOG:
                        if (iteminfo->item_type.dialog.popup_icon != (Widget)NULL)
                            {
                            XtDestroyWidget (iteminfo->item_type.dialog.popup_icon);
                            iteminfo->item_type.dialog.popup_icon = (Widget)NULL;
                            }
                        break;
                    case DMENU:
                        XtDestroyWidget (iteminfo->item_type.menu.pull_down_menu);
                        break;
                    case MITEM:
                        if (iteminfo->item_type.mitem.menu_toggle_button !=
                            (Widget)NULL)
                            {
                            XtDestroyWidget (iteminfo->item_type.mitem.
                                             menu_toggle_button);
                            iteminfo->item_type.mitem.menu_toggle_button =
                                (Widget)NULL;
                            }
                        break;
                    default:
                        break;
                        }
                    if (itemwidget != (Widget)NULL)
                        {
                        XtDestroyWidget (itemwidget);
                        }
                    }
                tc_free (thedialog_menu->items);
                thedialog_menu->items = (WIDGET_ITEM_INFO *)0;
                thedialog_menu->num_items = 0;
                
                wiz_XFlush (wiv.current_Display);
                
                return(SUCCESS);
                }
            }
        }
    else
        {
        am_message (AM_STATUS,
                "Dialog/Menu id or item id out of range in Routine wiz_delete_widget");
        return(FAIL);
        }
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    MANAGECHILD_WIDGET                                                    */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Manage children of widget list                                                 */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_managechild_widget (INT dialog_menu_id)
#else
publicdef INT wiz_managechild_widget (dialog_menu_id)
INT dialog_menu_id;
#endif
    {
    INT i, status;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[3];

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    CHAR st1[ST_COMMAND_MAX];
    XmString CompoundString;
    XtCallbackRec callback_arg[2];
    Widget horizontalScrollBar = (Widget)NULL;
    Widget menupulldownwidget = (Widget)NULL;
    Position x, y;
    Dimension selector_width, selector_height;
#endif  /* ---------- END MOTIF BLOCK ---------- */
    if (1 <= dialog_menu_id && dialog_menu_id <= wiv.dialog_menu_count)
        {
        E_CHECK_WIDGET (dialog_menu_id, 0);
        iteminfo = E_WIDGET_INFO (dialog_menu_id, 0);
        itemwidget = E_GET_WIDGET (dialog_menu_id, 0);
        
        if (XtIsManaged (itemwidget) == FALSE)  /* if unmanage                      */
            {
            XtManageChild (itemwidget); /* manage menu or dialog                    */
            if (iteminfo->type_id == DIALOG)
                {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                if (iteminfo->item_type.dialog.popup_icon == (Widget)NULL)
                    {
                    CompoundString = XmStringCreate (iteminfo->text,
                                                     XmSTRING_DEFAULT_CHARSET);
                    sprintf (st1, "DIALOG_%d_%d_%d", dialog_menu_id, 0, 1);
                    XtSetArg (arglist[0], XmNlabelString, CompoundString);
                    XtSetArg (arglist[1], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
                    callback_arg[0].callback = (XtCallbackProc)wi_callback;
                    callback_arg[0].closure = (caddr_t)((dialog_menu_id << 16) |
                                                        ((0 & 0xFF) << 8) |
                                                        (1 & 0xFF));
                    callback_arg[1].callback = (XtCallbackProc)NULL;
                    XtSetArg (arglist[2], XmNactivateCallback, callback_arg);
                    iteminfo->item_type.dialog.popup_icon =
                        XmCreatePushButtonGadget (wiv.main_dialog_menu[0], st1,
                                                            arglist, 3);
                    XmStringFree (CompoundString);
                    }
                XtManageChild (iteminfo->item_type.dialog.popup_icon);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                if (XtIsManaged (iteminfo->item_type.dialog.popup_icon) == TRUE)
                    {               /* if menu item for each dialog under "Windows" menu is manage  */
                    /* enable it                                                    */
                    XtSetArg (arglist[0], DwtNsensitive, TRUE);
                    XtSetValues (iteminfo->item_type.dialog.popup_icon, arglist, 1);
                    }
                else
                    {               /* manage it                                    */
                    XtManageChild (iteminfo->item_type.dialog.popup_icon);
                    }
#endif  /* ---------- END DECW BLOCK ---------- */
                for (i = 1; i <= (E_DIALOG_MENU (dialog_menu_id))->num_items; i++)
                    {
                    E_CHECK_WIDGET (dialog_menu_id, i);
                    iteminfo = E_WIDGET_INFO (dialog_menu_id, i);
                    itemwidget = E_GET_WIDGET (dialog_menu_id, i);
                    
                    if (itemwidget != (Widget)NULL)
                        {
                        if (iteminfo->type_id == RADIOITEM &&
                            iteminfo->item_type.radioitem.radio_box != (Widget)NULL)
                            {
                            XtManageChild (iteminfo->item_type.radioitem.radio_box);
                            }
			XtManageChild (itemwidget);
                        if (iteminfo->type_id == SELECTOR)
                            {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                            if (iteminfo->item_type.selector.hscroll == FALSE)
                                {
                                XtSetArg (arglist[0], XmNhorizontalScrollBar,
                                          &horizontalScrollBar);
                                XtSetArg (arglist[1], XmNwidth, &selector_width);
                                XtSetArg (arglist[2], XmNheight, &selector_height);
                                XtGetValues (XtParent (itemwidget), arglist, 3);
                                /* get rid of horizontal scroll bar                 */
                                XtUnmapWidget (horizontalScrollBar);
                                /* extend the selector height                       */
                                XtResizeWidget (XtParent (itemwidget), selector_width,
                                                selector_height + 19, 0);
                                /* reset flag to avoid further extending height     */
                                iteminfo->item_type.selector.hscroll = TRUE;
                                }
                            wiz_set_scrollbar (dialog_menu_id, i);
                            }
			else if (iteminfo->type_id == OPTIONMENU)
			  {
			    XtSetArg (arglist[0], XmNsubMenuId, &menupulldownwidget);
			    XtSetArg (arglist[1], XmNx, &x);
			    XtSetArg (arglist[2], XmNy, &y);
			    XtGetValues (itemwidget, arglist, 3);
			    XtSetArg (arglist[0], XmNx, x);
			    XtSetArg (arglist[1], XmNy, y);
			    XtSetValues (XtParent(menupulldownwidget), arglist, 2);
			  }
                        }
                    }
                }
	    else 
	      {
		XtSetArg (arglist[0], XmNsubMenuId, &menupulldownwidget);
		XtSetArg (arglist[1], XmNx, &x);
		XtSetArg (arglist[2], XmNy, &y);
		XtGetValues (itemwidget, arglist, 3);
		XtSetArg (arglist[0], XmNx, x);
		XtSetArg (arglist[1], XmNy, y);
		XtSetValues (XtParent(menupulldownwidget), arglist, 2);
	      }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
	                    wiz_set_scrollbar (dialog_menu_id, i);
                            }
                        }
                    }
                }
#endif  /* ---------- END DECW BLOCK ---------- */
            wiz_XFlush (wiv.current_Display);
            }
        return(SUCCESS);
        }
    else
        {
        am_message (AM_STATUS,
           "Dialog/Menu id or item id out of range in Routine wiz_managechild_widget");
        return(FAIL);
        }
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    UNMANAGECHILD_WIDGET                                                  */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Unmanage children of widget list                                               */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_unmanagechild_widget (INT dialog_menu_id)
#else
publicdef INT wiz_unmanagechild_widget (dialog_menu_id)
INT dialog_menu_id;
#endif
    {
    INT i, status;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    XEvent event;
    Arg arglist[1];
    
    if (1 <= dialog_menu_id && dialog_menu_id <= wiv.dialog_menu_count)
        {
        E_CHECK_WIDGET (dialog_menu_id, 0);
        itemwidget = E_GET_WIDGET (dialog_menu_id, 0);
        iteminfo = E_WIDGET_INFO (dialog_menu_id, 0);
        
        if (XtIsManaged (itemwidget) == TRUE)   /* if not unmanage                  */
            {
            if (iteminfo->type_id == DIALOG)
                {
                (E_DIALOG_MENU (dialog_menu_id))->wi_push_pop_num = 0;
                /* reset push_pop_num in case dialog is closed before wi_pop        */
                for (i = 1; i <= (E_DIALOG_MENU (dialog_menu_id))->num_items; i++)
                    {
                    E_CHECK_WIDGET (dialog_menu_id, i);
                    iteminfo = E_WIDGET_INFO (dialog_menu_id, i);
                    itemwidget = E_GET_WIDGET (dialog_menu_id, i);
                    
                    if (itemwidget != (Widget)NULL)
                        {
                        if (iteminfo->type_id == RADIOITEM &&
                            iteminfo->item_type.radioitem.radio_box != (Widget)NULL)
                            {
                            XtUnmanageChild (iteminfo->item_type.radioitem.radio_box);
                            }
			XtUnmanageChild (itemwidget);
		        }
                    }
                iteminfo = E_WIDGET_INFO (dialog_menu_id, 0);
                itemwidget = E_GET_WIDGET (dialog_menu_id, 0);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                if (iteminfo->item_type.dialog.popup_icon != (Widget)NULL)
                    {
                    XtUnmanageChild (iteminfo->item_type.dialog.popup_icon);
                    /* not being unmanaged at wiz_unmanagechild_widget              */
                    XtDestroyWidget (iteminfo->item_type.dialog.popup_icon);
                    iteminfo->item_type.dialog.popup_icon = (Widget)NULL;
                    }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                /* dim the menu item for the dialog under the "Windows" menu        */
                XtSetArg (arglist[0], DwtNsensitive, FALSE);
                XtSetValues (iteminfo->item_type.dialog.popup_icon, arglist, 1);
#endif  /* ---------- END DECW BLOCK ---------- */
                }
            XtUnmanageChild (itemwidget);   /* close dialog or menu                 */
            wiz_XFlush (wiv.current_Display);
            }
        return(SUCCESS);
        }
    else
        {
        am_message (AM_STATUS,
         "Dialog/Menu id or item id out of range in Routine wiz_unmanagechild_widget");
        return(FAIL);
        }
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    ISALNUMSTR                                                            */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Is alphanumeric String?                                                       */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
static INT isalnumstr (CHAR *str)
#else
static INT isalnumstr (str)
CHAR *str;
#endif
    {
    INT i, j;
    
    for (i = 0, j = strlen (str); i < j; i++)
        if (isalnum (str[i]) == FALSE)
            return(FALSE);
    return(TRUE);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    ISDIGITSTR                                                            */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Is string an integer?                                                         */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
static INT isdigitstr (CHAR *str)
#else
static INT isdigitstr (str)
CHAR *str;
#endif
    {
    INT i, j;
    
    for (i = 0, j = strlen (str); i < j; i++)
        {
        if (isdigit (str[i]) == FALSE)
            {
            if (i != 0 || (i == 0 && str[i] != '-' && str[i] != '+'))
                return(FALSE);
            }
        }
    return(TRUE);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    ISFLOATSTR                                                            */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Is string a float?                                                            */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
static INT isfloatstr (CHAR *str)
#else
static INT isfloatstr (str)
CHAR *str;
#endif
    {
    INT status, i, j, period_count = 0;
    
    for (i = 0, j = strlen (str); i < j; i++)
        {
        if (isdigit (str[i]) == FALSE)
            {
            if (str[i] == '.')
                {
                period_count++;
                if (period_count > 1)
                    return(FALSE);
                }
            else
                {
                if (i != 0 || (i == 0 && str[i] != '-' && str[i] != '+'))
                    return(FALSE);
                }
            }
        }
    return(TRUE);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    SET_EVENT_INFO                                                        */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Set the event information for wiz_wait_event. It is only called by            */
/*    wi_callback.                                                                  */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
static INT wiz_set_event_info (INT cdlgid, INT cditemid)
#else
static INT wiz_set_event_info (cdlgid, cditemid)
INT cdlgid, cditemid;
#endif
    {
    INT status;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    Boolean onoroff;
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    if (iteminfo->type_id == RADIOITEM)
        {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        onoroff = XmToggleButtonGadgetGetState (itemwidget);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        XtSetArg (arglist[0], DwtNvalue, &onoroff);
        XtGetValues (itemwidget, arglist, 1);
#endif  /* ---------- END DECW BLOCK ---------- */
        if (onoroff == FALSE)
            return(SUCCESS);
        /* radio button turn off state is not consider an event in our system       */
        }
    event_cdlgid = cdlgid;
    event_itemid = cditemid;
    E_CHECK_WIDGET (cdlgid, 0);
    event_class = ((E_WIDGET_INFO (cdlgid,
                            0))->type_id == DIALOG) ? WI_DIALOG_EVENT : WI_MENU_EVENT;
    return(SUCCESS);
    
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    FREE_MENU_SYN_LIST                                                    */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Free syn menu list pointer                                                    */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
static INT wiz_free_menu_syn_list (MENU_SYN_LIST *menu_syn_ptr)
#else
static INT wiz_free_menu_syn_list (menu_syn_ptr)
MENU_SYN_LIST *menu_syn_ptr;
#endif
    {
    MENU_SYN_ITEM_LIST *menu_syn_item_ptr, *menu_syn_item_tmptr;
    
    for (menu_syn_item_ptr = menu_syn_ptr->item_list;
         menu_syn_item_ptr != (MENU_SYN_ITEM_LIST *)NULL;
         menu_syn_item_ptr = menu_syn_item_tmptr)
        {
        menu_syn_item_tmptr = menu_syn_item_ptr->next;
        tc_free (menu_syn_item_ptr);
        menu_syn_item_tmptr = (MENU_SYN_ITEM_LIST *)NULL;
        }
    tc_free (menu_syn_ptr);
    menu_syn_ptr = (MENU_SYN_LIST *)NULL;
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    DEL_MENU_SYN_LIST                                                     */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Delete syn menu list                                                           */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_del_menu_syn_list (INT app_id)
#else
publicdef INT wiz_del_menu_syn_list (app_id)
INT app_id;
#endif
    {
    MENU_SYN_LIST *menu_syn_ptr, *menu_syn_tmptr;
    
    if (wiv.menu_syn_listroot != (MENU_SYN_LIST *)NULL)
        {
        if (wiv.menu_syn_listroot->app_id == app_id)
            {
            menu_syn_tmptr = wiv.menu_syn_listroot->next;
            wiz_free_menu_syn_list (wiv.menu_syn_listroot);
            wiv.menu_syn_listroot = menu_syn_tmptr;
            }
        else
            {
            for (menu_syn_ptr = wiv.menu_syn_listroot;
                 menu_syn_ptr != (MENU_SYN_LIST *)NULL;
                 menu_syn_ptr = menu_syn_ptr->next)
                {
                if (menu_syn_ptr->next != (MENU_SYN_LIST *)NULL &&
                    menu_syn_ptr->next->app_id == app_id)
                    {
                    menu_syn_tmptr = menu_syn_ptr->next->next;
                    wiz_free_menu_syn_list (menu_syn_ptr->next);
                    menu_syn_ptr->next = menu_syn_tmptr;
                    break;
                    }
                }
            }
        }
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    INS_MENU_SYN_LIST                                                     */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Insert syn menu list                                                           */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_ins_menu_syn_list (INT app_id, INT menuid)
#else
publicdef INT wiz_ins_menu_syn_list (app_id, menuid)
INT app_id;
INT menuid;
#endif
    {
    MENU_SYN_LIST *menu_syn_ptr;
    MENU_SYN_ITEM_LIST *menu_syn_item_ptr;
    
    if (wiv.menu_syn_listroot == (MENU_SYN_LIST *)NULL)
        {                           /* if this is a new application menu            */
        menu_syn_ptr = ALLOCATE (MENU_SYN_LIST);
        menu_syn_ptr->app_id = app_id;
        menu_syn_ptr->item_list = ALLOCATE (MENU_SYN_ITEM_LIST);
        /* create syn menu item list                                                */
        menu_syn_ptr->item_list->id = menuid;
        menu_syn_ptr->item_list->onoroff = OFF;
        menu_syn_ptr->item_list->next = (MENU_SYN_ITEM_LIST *)NULL;
        menu_syn_ptr->next = wiv.menu_syn_listroot;
        wiv.menu_syn_listroot = menu_syn_ptr;
        }
    else
        {
        for (menu_syn_ptr = wiv.menu_syn_listroot;
             menu_syn_ptr != (MENU_SYN_LIST *)NULL; menu_syn_ptr = menu_syn_ptr->next)
            {
            if (menu_syn_ptr->app_id == app_id)
                break;
            }
        if (menu_syn_ptr == (MENU_SYN_LIST *)NULL)
            {                       /* if this is a new application menu            */
            menu_syn_ptr = ALLOCATE (MENU_SYN_LIST);
            menu_syn_ptr->app_id = app_id;
            menu_syn_ptr->item_list = ALLOCATE (MENU_SYN_ITEM_LIST);
            /* create syn menu item list                                            */
            menu_syn_ptr->item_list->id = menuid;
            menu_syn_ptr->item_list->onoroff = OFF;
            menu_syn_ptr->item_list->next = (MENU_SYN_ITEM_LIST *)NULL;
            menu_syn_ptr->next = wiv.menu_syn_listroot;
            wiv.menu_syn_listroot = menu_syn_ptr;
            }
        else
            {                       /* check menuid already in the list             */
            for (menu_syn_item_ptr = menu_syn_ptr->item_list;
                 menu_syn_item_ptr != (MENU_SYN_ITEM_LIST *)NULL;
                 menu_syn_item_ptr = menu_syn_item_ptr->next)
                {
                if (menu_syn_item_ptr->id == menuid)
                    {
                    menu_syn_item_ptr->onoroff = OFF;
                    break;
                    }
                }
            /* if not exist in the list                                             */
            if (menu_syn_item_ptr == (MENU_SYN_ITEM_LIST *)NULL)
                {
                /* insert to syn menu item list                                     */
                menu_syn_item_ptr = ALLOCATE (MENU_SYN_ITEM_LIST);
                menu_syn_item_ptr->id = menuid;
                menu_syn_item_ptr->onoroff = OFF;
                menu_syn_item_ptr->next = menu_syn_ptr->item_list;
                menu_syn_ptr->item_list = menu_syn_item_ptr;
                }
            }
        }
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    MENU_SYN                                                              */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Syn menu.  Enable menu with same current application id from the syn menu     */
/*    list.  Delete                                                                 */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_menu_syn (INT cur_app_id)
#else
publicdef INT wiz_menu_syn (cur_app_id)
INT cur_app_id;
#endif
    {
    INT i, status, delete_flag = FALSE;
    MENU_SYN_LIST *menu_syn_ptr;
    MENU_SYN_ITEM_LIST *menu_syn_item_ptr;
    
    /* close other application                                                      */
    for (i = 1; i <= wiv.dialog_menu_count; i++)
        {
        E_CHECK_WIDGET (i, 0);
        if ((E_DIALOG_MENU (i))->num_items != 0)    /* not delete                   */
            {
            if ((E_WIDGET_INFO (i, 0))->type_id == DMENU &&
                (E_DIALOG_MENU (i))->app_id != cur_app_id &&
                (E_DIALOG_MENU (i))->app_id > 1)
                {
                wiz_unmanagechild_widget (i);   /* close menu                       */
                }
            }
        }
    for (menu_syn_ptr = wiv.menu_syn_listroot; menu_syn_ptr != (MENU_SYN_LIST *)NULL;
         menu_syn_ptr = menu_syn_ptr->next)
        {
        if (menu_syn_ptr->app_id == cur_app_id)
            {                       /* syn menu list application id match current application id, then  */
            /*   open menu                                  */
            for (menu_syn_item_ptr = menu_syn_ptr->item_list;
                 menu_syn_item_ptr != (MENU_SYN_ITEM_LIST *)NULL;
                 menu_syn_item_ptr = menu_syn_item_ptr->next)
                {
                if (menu_syn_item_ptr->onoroff == TRUE)
                    {
                    wiz_managechild_widget (menu_syn_item_ptr->id);
                    }
                }
            break;
            }
        }
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WI_CALLBACK                                                           */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Callback routine of all dialog and menu.                                      */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wi_callback (Widget widget, INT tag, VOIDPTR *callback_data)
#else
publicdef INT wi_callback (widget, tag, callback_data)
Widget widget;
INT tag;
VOIDPTR *callback_data;
#endif
    {
    INT i, status;
    INT cdlgid, cditemid, item_chosen;
    INT rmin, rmax;
    INT pos, shown, inc, page_inc, maxvalue;
    INT sel_count, strlength, return_flag = FALSE, menubar_close = FALSE,
        event_consumed;
    Boolean single_sel, onoroff;
    CHAR *str;
    CHAR text[ST_MAC_MAX];
    Widget selectorscrollbar;
    WIDGET_ITEM_INFO *iteminfo;
    DIALOG_MENU *thedialog_menu;
    Arg arglist[1];
    APPLICATION_STATIC *application_static = am_static ();
    CALLBACK_STRUCT *list_ptr;
    
    if (tag == 0)                   /* if it is not our dialog or menu item         */
        {
        return(FAIL);
        }
    else
        {
        /* tag interger format : xxyyzz                                             */
        /* xx = cdlgid/menuid,  (16 bits)                                           */
        /* yy = cditemid/menuitemid, (8 bits)                                       */
        /* zz = subitemid (8 bits)                                                  */
        /* eg. push button under pulldown menu of the dialog                        */
        /* shit bits to get corresponding value                                     */
        cdlgid = (tag >> 16) & 0xFFFF;
        cditemid = (tag >> 8) & 0xFF;
        item_chosen = tag& 0xFF;
        
        /* if found in our widget array                                             */
        E_CHECK_WIDGET (cdlgid, 0);
        if ((1 <= cdlgid && cdlgid <= wiv.dialog_menu_count) && (0 <= cditemid &&
                                                                  cditemid <=
                                                                 (E_DIALOG_MENU (
                                                                               cdlgid))->
                                                                 num_items))
            {
            E_CHECK_WIDGET (cdlgid, cditemid);
            iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
            thedialog_menu = E_DIALOG_MENU (cdlgid);
            
            switch (iteminfo->type_id)
                {
            case DIALOG:
                if (item_chosen == 1)
                    {
                    /* pop up dialog and push button of                             */
                    /* that dialog under main window menu                           */
                    /* has the same cdlgid, cditemid except                         */
                    /* item_chosen.  The former one is 0                            */
                    /* whereas the other one is 1                                   */
                    /* this is the callback of the push                             */
                    /* button under main window menu, pop                           */
                    /* up dialog doesn't have any callback                          */
                    XRaiseWindow (wiv.current_Display,
                                  XtWindow (XtParent (E_GET_WIDGET (cdlgid, 0))));
                    /* raise the pop up dialog                                      */
                    if (wiv.wi_push_pop_top == 0)
                        {
                        XSetInputFocus (wiv.current_Display,
                                        XtWindow (E_GET_WIDGET (cdlgid, 0)),
                                        RevertToNone, CurrentTime);
                        }
                    /* set input focus                                              */
                    wiv.frontdialog_id = cdlgid;    /* update front dialog id       */
                    /* do menu syn if necessary                                     */
                    if (wiv.wi_push_pop_top == 0)
                        {
                        wiz_menu_syn ((E_DIALOG_MENU (cdlgid))->app_id);
                        }
                    return_flag = TRUE;
                    }
                break;
            case MITEM:
                if (XtIsManaged (wiv.mainmenubar) == TRUE)
                    {
                    XtUnmanageChild (wiv.mainmenubar);
                    menubar_close = TRUE;
                    }
                break;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            case EDITTEXT:
                /* valuechanged callback of edittext,  check the verify mode and length */
                str = XmTextGetString (widget);
                strlength = strlen (str);
                if ((iteminfo->item_type.edittext.verifylen != 0 &&
                     strlength > iteminfo->item_type.edittext.verifylen) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_ALPHANUMERIC &&
                     isalnumstr (str) == FALSE) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_INTEGER &&
                     isdigitstr (str) == FALSE) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_FLOAT &&
                     isfloatstr (str) == FALSE))
                    {               /* if verify mode/length is violated, undo last user entry  */
                    /*   and give a warning bell sound              */
                    *(str + strlength - 1) = NUL;
                    XmTextSetString (widget, str);
                    wi_beep (1);
                    }
                XtFree (str);
                return_flag = TRUE;
                break;
            case SELECTOR:
                /* test for last blank line selection and deselect it               */
                sel_count = (((XmListCallbackStruct *)callback_data)->reason ==
                             XmCR_SINGLE_SELECT) ?
                            1 :
                            ((XmListCallbackStruct *)callback_data)->
                            selected_item_count;
                item_chosen = ((XmListCallbackStruct *)callback_data)->item_position;
                
                XtSetArg (arglist[0], XmNitemCount, &maxvalue);
                XtGetValues (widget, arglist, 1);
                if (item_chosen > maxvalue - 1) /* ignore last blank line           */
                    {
                    XmListDeselectPos (widget, item_chosen);
                    return_flag = TRUE;
                    }
                else
                    {
                    if (sel_count >= iteminfo->item_type.selector.num_sel)
                        {
                        iteminfo->item_type.selector.sel_list[sel_count -
                                                              1] = item_chosen;
                        }
                    else
                        {           /* remove from sel_list                         */
                        for (i = 0; i < iteminfo->item_type.selector.num_sel; i++)
                            {
                            if (iteminfo->item_type.selector.sel_list[i] ==
                                item_chosen)
                                {
                                for (; i < (iteminfo->item_type.selector.num_sel - 1);
                                     i++)
                                    {
                                    iteminfo->item_type.selector.sel_list[i] =
                                        iteminfo->item_type.selector.sel_list[i + 1];
                                    }
                                break;
                                }
                            }
                        }
                    iteminfo->item_type.selector.num_sel = sel_count;
                    if (iteminfo->item_type.selector.activate_flag == FALSE)
                        {
                        return_flag = TRUE;
                        }
                    }
                break;
            case RADIOITEM:
                onoroff = XmToggleButtonGadgetGetState (widget);
                if (onoroff == FALSE)
                    {
                    return_flag = TRUE;
                    }
                break;
            case VSCROLL:
                /* if this is the our own scrollbar, then go back to all            */
                /* selectors and scroll all the embeded scrollbar                   */
                for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax);
                     rmin <= rmax; rmin++)
                    {
                    E_CHECK_WIDGET (cdlgid, rmin);
                    if ((E_WIDGET_INFO (cdlgid, rmin))->type_id == SELECTOR)
                        {
                        XtSetArg (arglist[0], XmNverticalScrollBar,
                                  &selectorscrollbar);
                        XtGetValues (E_GET_WIDGET (cdlgid, rmin), arglist, 1);
                        XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc,
                                              &page_inc);
                        pos = ((XmScrollBarCallbackStruct *)callback_data)->value;
                        XtSetArg (arglist[0], XmNvalue, pos);
                        XtSetValues (selectorscrollbar, arglist, 1);
                        XmScrollBarSetValues (selectorscrollbar, pos, shown, inc,
                                              page_inc, FALSE);
                        XmListSetPos (E_GET_WIDGET (cdlgid, rmin), pos + 1);
                        }
                    }
                return_flag = TRUE;
                break;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            case EDITTEXT:
                /* valuechanged callback of edittext,  check the verify mode and length */
                str = DwtSTextGetString (widget);
                strlength = strlen (str);
                if ((iteminfo->item_type.edittext.verifylen != 0 &&
                     strlength > iteminfo->item_type.edittext.verifylen) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_ALPHANUMERIC &&
                     isalnumstr (str) == FALSE) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_INTEGER &&
                     isdigitstr (str) == FALSE) ||
                    (iteminfo->item_type.edittext.verifymode == VERIFY_FLOAT &&
                     isfloatstr (str) == FALSE))
                    {               /* if verify mode/length is violated, undo last user entry  */
                    /*   and give a warning bell sound              */
                    *(str + strlength - 1) = NUL;
                    DwtSTextSetString (widget, str);
                    wi_beep (1);
                    }
                XtFree (str);
                return_flag = TRUE;
                break;
            case SELECTOR:
                /* test for last blank line selection and deselect it               */
                item_chosen = ((DwtListBoxCallbackStruct *)callback_data)->item_number;
                XtSetArg (arglist[0], DwtNitemsCount, &maxvalue);
                XtGetValues (widget, arglist, 1);
                if (maxvalue == item_chosen)
                    {
                    DwtListBoxDeselectPos (widget, item_chosen);
                    return_flag = TRUE;
                    }
                else
                    {
                    XtSetArg (arglist[0], DwtNsingleSelection, &single_sel);
                    XtGetValues (widget, arglist, 1);
                    if (single_sel == FALSE)    /* extend selection                 */
                        {
                        XtSetArg (arglist[0], DwtNselectedItemsCount, &sel_count);
                        XtGetValues (widget, arglist, 1);
                        /* see if number of items selected is more than             */
                        /* the maximum number of selection that is allowed          */
                        if (sel_count > iteminfo->item_type.selector.max_sel)
                            {
                            DwtListBoxDeselectPos (widget, item_chosen);
                            wi_beep (1);    /* deselect the last one and give       */
                            /* a warning bell sound if necessary    */
                            return_flag = TRUE;
                            }
                        else
                            {
                            iteminfo->item_type.selector.num_sel = sel_count;
                            iteminfo->item_type.selector.sel_list[sel_count -
                                                                  1] = item_chosen;
                            }
                        }
                    else            /* single selection                             */
                        {
                        iteminfo->item_type.selector.num_sel = 1;
                        iteminfo->item_type.selector.sel_list[0] = item_chosen;
                        }
                    if (iteminfo->item_type.selector.activate_flag == FALSE)
                        {
                        return_flag = TRUE;
                        }
                    }
                break;
            case RADIOITEM:
                XtSetArg (arglist[0], DwtNvalue, &onoroff);
                XtGetValues (widget, arglist, 1);
                if (onoroff == FALSE)
                    {
                    return_flag = TRUE;
                    }
                break;
            case VSCROLL:
                /* if this is the our own scrollbar, then go back to all            */
                /* selectors and scroll all the embeded scrollbar                   */
                for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax);
                     rmin <= rmax; rmin++)
                    {
                    E_CHECK_WIDGET (cdlgid, rmin);
                    if ((E_WIDGET_INFO (cdlgid, rmin))->type_id == SELECTOR)
                        {
                        XtSetArg (arglist[0], DwtNverticalScrollBar,
                                  &selectorscrollbar);
                        XtGetValues (E_GET_WIDGET (cdlgid, rmin), arglist, 1);
                        DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc,
                                               &page_inc);
                        pos = ((DwtScrollBarCallbackStruct *)callback_data)->value;
                        XtSetArg (arglist[0], DwtNvalue, pos);
                        XtSetValues (selectorscrollbar, arglist, 1);
                        DwtScrollBarSetSlider (selectorscrollbar, pos, shown, inc,
                                               page_inc, FALSE);
                        }
                    }
                return_flag = TRUE;
                break;
#endif  /* ---------- END DECW BLOCK ---------- */
            case OPTIONMENU:
                iteminfo->item_type.optionmenu.item_chosen = item_chosen;
                break;
                }
            /* switch                                                               */
            
            for (list_ptr = wiv.callback_list; list_ptr != (CALLBACK_STRUCT *)NULL;
                 list_ptr = list_ptr->next)
                {
                event_consumed = (*list_ptr->function)(cdlgid, cditemid,
                                                       list_ptr->pointer);
                if (event_consumed == SUCCESS)
                    {
                    return_flag = TRUE;
                    break;
                    }
                }
            if (return_flag == FALSE)
                {
                wiz_set_event_info (cdlgid, cditemid);
                if (thedialog_menu->server != NULL_FUNCTION_PTR)
                    {
                    if (thedialog_menu->app_id >= 0)
                        {
                        application_static->application_id = thedialog_menu->app_id;
			wiz_busy_cursor(TRUE);
                        (thedialog_menu->server)
                        (cdlgid, cditemid, thedialog_menu->pointer);
			wiz_busy_cursor(FALSE);
                        /* server and pointer get stored at am_define_resource      */
                        }
                    }
                }
            if (menubar_close == TRUE)
                {
                XtManageChild (wiv.mainmenubar);
                }
            }
        else
            {
            return(FAIL);
            }
        }
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    SET_SCROLLBAR                                                         */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Set the attributes of our own define scrollbar to be the same as              */
/*    selector embeded scrollbar                                                    */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_set_scrollbar (INT cdlgid, INT cditemid)
#else
publicdef INT wiz_set_scrollbar (cdlgid, cditemid)
INT cdlgid, cditemid;
#endif
    {
    INT i, rmin, rmax, status;
    INT minvalue, maxvalue, pos, shown, inc, page_inc;
    Dimension height;
    Widget selectorscrollbar;
    Arg arglist[5];
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax), i = rmin; i <= rmax;
         i++)
        {
        E_CHECK_WIDGET (cdlgid, i);
        if ((E_WIDGET_INFO (cdlgid, i))->type_id == SELECTOR)
            {
            XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
            XtGetValues (E_GET_WIDGET (cdlgid, i), arglist, 1);
            XtSetArg (arglist[0], XmNminimum, &minvalue);
            XtSetArg (arglist[1], XmNmaximum, &maxvalue);
            XtSetArg (arglist[2], XmNheight, &height);
            XtGetValues (selectorscrollbar, arglist, 3);
            XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            break;
            }
        }
    /* get embeded scrollbar attribute and set our own scrollbar                    */
    /*   attribute to be the same                                                   */
    for (i = rmax; i >= rmin; i--)
        {
        if ((E_WIDGET_INFO (cdlgid, i))->type_id == VSCROLL &&
            (E_GET_WIDGET (cdlgid, i) != (Widget)NULL))
            {
            XtSetArg (arglist[0], XmNsliderSize, shown);
            XtSetArg (arglist[1], XmNminimum, minvalue);
            XtSetArg (arglist[2], XmNmaximum, maxvalue);
            XtSetArg (arglist[3], XmNvalue, pos);
            XtSetArg (arglist[4], XmNheight, height);
            XtSetValues (E_GET_WIDGET (cdlgid, i), arglist, 5);
            XmScrollBarSetValues (E_GET_WIDGET (cdlgid, i), pos, shown, inc, page_inc,
                                  FALSE);
            break;
            }
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax), i = rmin; i <= rmax;
         i++)
        {
        E_CHECK_WIDGET (cdlgid, i);
        if ((E_WIDGET_INFO (cdlgid, i))->type_id == SELECTOR)
            {
            XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
            XtGetValues (E_GET_WIDGET (cdlgid, i), arglist, 1);
            XtSetArg (arglist[0], DwtNminValue, &minvalue);
            XtSetArg (arglist[1], DwtNmaxValue, &maxvalue);
            XtGetValues (selectorscrollbar, arglist, 2);
            DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            break;
            }
        }
    /* get embeded scrollbar attribute and set our own scrollbar                    */
    /*   attribute to be the same                                                   */
    for (i = rmax; i >= rmin; i--)
        {
        if ((E_WIDGET_INFO (cdlgid, i))->type_id == VSCROLL &&
            (E_GET_WIDGET (cdlgid, i) != (Widget)NULL))
            {
            XtSetArg (arglist[0], DwtNminValue, minvalue);
            XtSetArg (arglist[1], DwtNmaxValue, maxvalue);
            XtSetValues (E_GET_WIDGET (cdlgid, i), arglist, 2);
            XtSetArg (arglist[0], DwtNvalue, pos);
            XtSetValues (E_GET_WIDGET (cdlgid, i), arglist, 1);
            DwtScrollBarSetSlider (E_GET_WIDGET (cdlgid, i), pos, shown, inc, page_inc,
                                   FALSE);
            break;
            }
        }
#endif  /* ---------- END DECW BLOCK ---------- */
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    ASCII_FROM_CMP                                                        */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*     Convert compound string to ascii string.                                     */
/*                                                                                  */
/*   ************************************************************************       */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
/* (This code is a copy of demo program widgetview.c from OSF)                      */
#if USE_PROTOTYPES
publicdef CHAR *wiz_ascii_from_cmp (XmString value)
#else
publicdef CHAR *wiz_ascii_from_cmp (value)
XmString value;
#endif
    {
    XmStringContext context;
    static CHAR *ascii_string;
    
    ascii_string = (CHAR *)0;
    if (XmStringInitContext (&context, value) == TRUE)
        {
        XmStringCharSet charset;
        XmStringDirection direction;
        Boolean separator;
        
        /* Then get the next (first) text segment                                   */
        if (XmStringGetNextSegment (context, &ascii_string, &charset, &direction,
                                    &separator) == TRUE)
            {
            XmStringFreeContext (context);
            }
        else
            {
            fprintf (stderr, "Can't get next segment\n");
            }
        }
    else
        {
        fprintf (stderr, "Can't create string context\n");
        }
    return((CHAR *)ascii_string);
    }


#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
/* (Provided by Bob Holt of Digital)                                                */
#if USE_PROTOTYPES
publicdef CHAR *wiz_ascii_from_cmp (DwtCompString value)
#else
publicdef CHAR *wiz_ascii_from_cmp (value)
DwtCompString value;
#endif
    {
    /* use only in case where compound string is of type ISO Latin1                 */
    
    DwtCompStringContext context;
    static CHAR *ascii_string;
    
    ascii_string = (CHAR *)0;
    /* First get the compound string context                                        */
    if (DwtInitGetSegment (&context, value) == DwtSuccess)
        {
        long charset;
        Boolean direction;
        long language, rendition;
        
        /* Then get the next (first) text segment                                   */
#if 0 /* temporary commented out until we got answer from digitial */
        if (DwtGetNextSegment (&context, ascii_string, &charset, &direction, &language,
                               &rendition) != DwtSuccess)
            {
            fprintf (stderr, "Can't get next segment\n");
            }
#endif
        }
    else
        {
        fprintf (stderr, "Can't create string context\n");
        }
    return((CHAR *)ascii_string);
    }


#endif  /* ---------- END DECW BLOCK ---------- */

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    TEXT_TO_CSLIST                                                        */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*     Convert ascii string to Compound String list items                           */
/*                                                                                  */
/*   ************************************************************************       */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
#if USE_PROTOTYPES
publicdef INT wiz_text_to_cslist (CHAR *strings, XmString **list_items, INT *l)
#else
publicdef INT wiz_text_to_cslist (strings, list_items, l)
CHAR *strings;
XmString **list_items;
INT *l;
#endif
    {
    INT i;
    INT status = SUCCESS;
    CHAR *str;
    CHAR *temp_strings;
    CHAR *ps;
    XmString *pointer = (XmString *)0;
    
    temp_strings = (CHAR *) tc_zalloc (sizeof(CHAR) * (strlen (strings) + 3));
    IS_EMPTY_STRING (strings) ? sprintf (temp_strings, "%s ",
                                         strings) : sprintf (temp_strings, "%s\n ",
                                                             strings);
    
    if (IS_STRING (temp_strings))
        {
        /*    Count the number of newline separates temp_strings                    */
        for (i = 1, ps = temp_strings; *ps != 0; ps++)
            {
            if (*ps == '\n')
                i++;
            }
        /*    Allocate a pseudo-tcp                                                 */
        pointer = (XmString *) tc_zalloc (sizeof(XmString) * i);
        for (i = 0, str = ps = temp_strings; *ps != NUL; ps++)
            {
            if (*ps == '\n' || *ps == '\r') /* test for <CR>                        */
                {
                *ps = 0;
                pointer[i++] = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
                *ps = '\n';
                str = ++ps;
                }
            if (*ps > 0 && *ps < 32)    /* test for extra control character         */
                /* after the <CR>                           */
                {
                str = ++ps;
                }
            }
        /* test last character is a <CR>                                            */
        if (*(ps - 1) != '\n' && *(ps - 1) != '\r')
            pointer[i] = XmStringCreate (str, XmSTRING_DEFAULT_CHARSET);
        *l = i + 1;
        }
    else
        {
        *l = 0;
        status = FAIL;
        }
    tc_free (temp_strings);
    *list_items = pointer;
    return status;
    }


#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
#if USE_PROTOTYPES
publicdef INT wiz_text_to_cslist (CHAR *strings, DwtCompString **list_items, 
                                  INT *l, Dimension selector_width)
#else
publicdef INT wiz_text_to_cslist (strings, list_items, l, selector_width)
CHAR *strings;
DwtCompString **list_items;
INT *l;
Dimension selector_width;
#endif
    {
    INT i;
    INT status = SUCCESS;
    CHAR *str;
    CHAR *temp_strings;
    CHAR *ps;
    DwtCompString *pointer = (DwtCompString *)0;
    
    temp_strings = (CHAR *) tc_zalloc (sizeof(CHAR) * (strlen (strings) + 3));
    IS_EMPTY_STRING (strings) ? sprintf (temp_strings, "%s ",
                                         strings) : sprintf (temp_strings, "%s\n ",
                                                             strings);
    
    if (IS_STRING (temp_strings))
        {
        /*    Count the number of newline separates temp_strings                    */
        for (i = 1, ps = temp_strings; *ps != 0; ps++)
            {
            if (*ps == '\n')
                i++;
            }
        /*    Allocate a pseudo-tcp                                                 */
        pointer = (DwtCompString *) tc_zalloc (sizeof(DwtCompString) * i);
        for (i = 0, str = ps = temp_strings; *ps != NUL; ps++)
            {
            if (*ps == '\n' || *ps == '\r') /* test for <CR>                        */
                {
                *ps = 0;
                pointer[i++] = wiz_DwtLatin1StringSpecial (str, selector_width);
                *ps = '\n';
                str = ++ps;
                }
            if (*ps > 0 && *ps < 32)    /* test for extra control character         */
                /* after the <CR>                           */
                {
                str = ++ps;
                }
            }
        /* test last character is a <CR>                                            */
        if (*(ps - 1) != '\n' && *(ps - 1) != '\r')
            pointer[i] = wiz_DwtLatin1StringSpecial (str, selector_width);
        *l = i + 1;
        }
    else
        {
        *l = 0;
        status = FAIL;
        }
    tc_free (temp_strings);
    *list_items = pointer;
    return status;
    }


#endif  /* ---------- END DECW BLOCK ---------- */

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    GET_EVENT_INFO                                                        */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    get the event information for wiz_wait_event                                  */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_get_event_info (INT *cdlgid, INT *cditemid, INT *eventclass)
#else
publicdef INT wiz_get_event_info (cdlgid, cditemid, eventclass)
INT *cdlgid;
INT *cditemid;
INT *eventclass;
#endif
    {
    *cdlgid = event_cdlgid;
    *cditemid = event_itemid;
    *eventclass = event_class;
    
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    CLEAR_EVENT_INFO                                                      */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    clear the event information for wiz_wait_event                                */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_clear_event_info (void)
#else
publicdef INT wiz_clear_event_info ()
#endif
    {
    event_cdlgid = 0;
    event_itemid = 0;
    event_class = 0;
    
    return(SUCCESS);
    }


#ifndef MOTIF  /* ---------- BEGIN DECW BLOCK ---------- */
/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    wiz_DwtLatin1StringSpecial                                            */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Convert string to compound string and with space and counter.                  */
/*   The purpose of this routine is to make each items in selector (listbox)        */
/*   to be unique.                                                                  */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef DwtCompString wiz_DwtLatin1StringSpecial (CHAR *str, Dimension selector_width)
#else
publicdef DwtCompString wiz_DwtLatin1StringSpecial (str, selector_width)
CHAR *str;
Dimension selector_width;
#endif
    {
    INT strlength;
    INT num_space;
    CHAR *tempstr;
    CHAR *ps;
    CHAR text[ST_MAC_MAX];
    DwtCompString CompoundString;
    static INT count = 0;
    
    if (selector_width > 1000)      /* temporary bug fix                            */
        {
        sprintf (text, "Selector Width = %d in routine wiz_DwtLatin1StringSpecial.\n",
                 selector_width);
        er_log (text);
        selector_width = 1000;
        }
    count++;
    strlength = strlen (str);
    selector_width -= 17;           /* 17 = scrollbar width                         */
    num_space = ((INT)selector_width / 3) - strlength;
    /* 4 = pixel for character i                                                    */
    num_space = (num_space < 0) ? 1 : num_space;    /* number of space never        */
    /* less than 0                  */
    tempstr = (CHAR *) tc_zalloc (sizeof(CHAR) * (strlength + num_space + 6 + 1));
    /* 6 = maximum number of digit                                                  */
    strcpy (tempstr, str);
    if (tempstr[strlength - 1] == '\n' || tempstr[strlength - 1] == '\r')
        {
        tempstr[strlength - 1] = 0;
        }
    ps = tempstr + strlen (tempstr);
    for (; num_space > 0; num_space--)
        {
        *ps++ = ' ';                /* insert space                                 */
        }
    sprintf (ps, "%d", count);      /* add count to make every item in selector     */
    /* box identical, the count is always identical */
        /* for each dwtlatin1stringspecial call         */
    
    CompoundString = DwtLatin1String (tempstr);
    tc_free (tempstr);
    
    return(CompoundString);
    }


#endif   /* ---------- END DECW BLOCK ---------- */

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_GET_GROUP_RANGE                                                   */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   get the group range of certain dialog item                                     */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_get_group_range (INT cdlgid, INT cditemid, INT *rmin, INT *rmax)
#else
publicdef INT wiz_get_group_range (cdlgid, cditemid, rmin, rmax)
INT cdlgid, cditemid;
INT *rmin, *rmax;
#endif
    {
    INT i, num_items, status;
    WIDGET_ITEM_INFO *iteminfo;
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    
    for (i = cditemid; i >= 1; i--)
        {
        E_CHECK_WIDGET (cdlgid, i);
        if ((E_WIDGET_INFO (cdlgid, i))->group != iteminfo->group)
            {
            *rmin = i + 1;
            break;
            }
        }
    if (i + 1 == 1)
        {
        *rmin = 1;
        }
    E_CHECK_WIDGET (cdlgid, 0);
    for (i = cditemid, num_items = (E_DIALOG_MENU (cdlgid))->num_items; i <= num_items;
         i++)
        {
        E_CHECK_WIDGET (cdlgid, i);
        if ((E_WIDGET_INFO (cdlgid, i))->group != iteminfo->group)
            {
            *rmax = i - 1;
            break;
            }
        }
    if (i - 1 == num_items)
        {
        *rmax = num_items;
        }
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_SET_SELECTOR_TEXT                                                 */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Set string in the selector                                                     */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_set_selector_text (INT cdlgid, INT cditemid, CHAR *strings)
#else
publicdef INT wiz_set_selector_text (cdlgid, cditemid, strings)
INT cdlgid, cditemid;
CHAR *strings;
#endif
    {
    INT i, l, status;
    Dimension selector_width;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[3];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    Dimension selector_height;
    XmString *list_items;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString *list_items;
#endif  /* ---------- END DECW BLOCK ---------- */
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtSetArg (arglist[0], XmNwidth, &selector_width);
    XtSetArg (arglist[1], XmNheight, &selector_height);
    XtSetArg (arglist[2], XmNselectedItemCount, &l);
    XtGetValues (itemwidget, arglist, 3);
    
    if (l > 0)                      /* if selected item > 1, deselect all of them   */
        {
        XmListDeselectAllItems (itemwidget);
        }
    iteminfo->item_type.selector.num_sel = 0;
    
    XtSetArg (arglist[0], XmNitemCount, 0); /* clear selector box                   */
    XtSetValues (itemwidget, arglist, 1);
    wiz_text_to_cslist (strings, &list_items, &l);
    
    if (l != 0)
        {
        XtSetArg (arglist[0], XmNitemCount, l);
        XtSetArg (arglist[1], XmNitems, list_items);
        XtSetValues (itemwidget, arglist, 2);
	for (i = l-1; i >= 0; i--)
	  {
	    XmStringFree (list_items[i]);
	  }
        }
    /* following is to prevent selector resize                                      */
    XtSetArg (arglist[0], XmNwidth, selector_width);
    XtSetArg (arglist[1], XmNheight, selector_height);
    XtSetValues (itemwidget, arglist, 2);

#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    XtSetArg (arglist[0], DwtNwidth, &selector_width);
    XtSetArg (arglist[1], DwtNselectedItemsCount, &l);
    XtGetValues (itemwidget, arglist, 2);
    if (l > 0)                      /* if selected item > 1, deselect all of them   */
        {
        DwtListBoxDeselectAllItems (itemwidget);
        }
    iteminfo->item_type.selector.num_sel = 0;
    
    XtSetArg (arglist[0], DwtNitemsCount, 0);   /* clear selector box               */
    XtSetValues (itemwidget, arglist, 1);
    wiz_text_to_cslist (strings, &list_items, &l, selector_width);
    
    if (l != 0)
        {
        XtSetArg (arglist[0], DwtNitemsCount, l);
        XtSetArg (arglist[1], DwtNitems, list_items);
        XtSetValues (itemwidget, arglist, 2);
	for (i = l-1; i >= 0; i--)
            {
            XtFree (list_items[i]);
            }
        }
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_set_scrollbar (cdlgid, cditemid);   /* set the group scroll bar             */
    
    return(SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_CHECKWIDGET                                                       */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Check widget exist and validation of wi layer memory                           */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_checkwidget (INT cdlgid, INT cditemid)
#else
publicdef INT wiz_checkwidget (cdlgid, cditemid)
INT cdlgid, cditemid;
#endif
    {
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    
    static CONFIGURATION_WORKSPACE *c = (CONFIGURATION_WORKSPACE *)0;
    
    if (c == (CONFIGURATION_WORKSPACE *)0)
        {
        am_open_workspace ("CONFIGURATION", AM_GLOBAL, (VOIDPTR *)&c);
        }
    if (c->checkwi == FALSE)
        {
        return(SUCCESS);
        }
    else
        {
        if (1 <= cdlgid && cdlgid <= wiv.dialog_menu_count &&
            E_DIALOG_MENU (cdlgid) != (DIALOG_MENU *)0 &&
            (E_DIALOG_MENU (cdlgid))->num_items != 0 &&
            cditemid <= (E_DIALOG_MENU (cdlgid))->num_items)
            {
            iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
            if (iteminfo == (WIDGET_ITEM_INFO *)0)
                {
                am_message (AM_STATUS,
  "Memories of WI layer have been overwritten, please check your application codes\n");
                return(FAIL);
                }
            else
                {
                itemwidget = iteminfo->widget;  /* E_GET_WIDGET(cdlgid,cditemid);   */
                if ((iteminfo->type_id != VSCROLL && itemwidget == (Widget)NULL) ||
                    (iteminfo->type_id == MITEM &&
                     iteminfo->item_type.mitem.check_onoff == ON &&
                     iteminfo->item_type.mitem.menu_toggle_button == (Widget)NULL))
                    {
                    am_message (AM_STATUS,
                            "Cannot reference null widget in Routine wiz_checkwidget");
                    return(FAIL);
                    }
                else
                    {
                    return(SUCCESS);
                    }
                }
            }
        else
            {
            if (cdlgid <= 0 || cdlgid > wiv.dialog_menu_count ||
                (E_DIALOG_MENU (cdlgid) != (DIALOG_MENU *)0 &&
                 (E_DIALOG_MENU (cdlgid))->num_items != 0 &&
                 cditemid > (E_DIALOG_MENU (cdlgid))->num_items))
                {
                am_message (AM_STATUS,
                "Dialog/Menu id or item id out of range in Routine wiz_checkwidget\n");
                }
            else                    /* (E_DIALOG_MENU(cdlgid) == (DIALOG_MENU *) 0) */
                {
                am_message (AM_STATUS,
  "Memories of WI layer have been overwritten, please check your application codes\n");
                }
            return(FAIL);
            }
        }
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_ERROR                                                             */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   PRINT WI ERROR MESSAGE                                                         */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef VOID wiz_error (CHAR *string, CHAR *pname)
#else
publicdef VOID wiz_error (string, pname)
CHAR *string, *pname;
#endif
    {
    CHAR st1[ST_MAC_MAX];
    sprintf (st1, "%s in Routine : %s\n", string, pname);
    am_message (AM_STATUS, st1);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_XFLUSH                                                            */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   XFlush and dispatch expose and visiblity event                                 */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_XFlush (Display *display)
#else
publicdef INT wiz_XFlush (display)
Display *display;
#endif
    {
    XEvent event;
    
    XFlush (display);
    while (XCheckMaskEvent (display, ExposureMask | VisibilityChangeMask,
                            &event) == TRUE)
        {
        XtDispatchEvent (&event);
        }
    }


/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WIZ_BUSY_CURSOR                                                       */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*   Turn on/off Busy Watch Cursor when executes application codes                  */
/*                                                                                  */
/*   ************************************************************************       */
#if USE_PROTOTYPES
publicdef INT wiz_busy_cursor (BOOL onoroff)
#else
publicdef INT wiz_busy_cursor (onoroff)
BOOL onoroff;
#endif
{
  static Cursor Watch_cursor = (Cursor)0;
  WM_CURRENT *wmv_current = wm_static ();
  INT i;

  if (Watch_cursor EQUALS (Cursor)0)
    {
      Watch_cursor = XCreateFontCursor(wiv.current_Display, XC_watch);
    }
  
  if (onoroff EQUALS TRUE)
    {
      XDefineCursor(wiv.current_Display,
		    XtWindow (wiv.toplevel),
		    Watch_cursor);
    }
  else
    {
      XUndefineCursor(wiv.current_Display,
		    XtWindow (wiv.toplevel));
    }
  
  for (i = 1; i <= wiv.dialog_menu_count; i++)
    {
      if ((E_WIDGET_INFO (i, 0))->type_id EQUALS DIALOG)
	  {
	    if (onoroff EQUALS TRUE AND
		XtIsManaged (E_GET_WIDGET (i,0)) EQUALS TRUE)
	      {
		XDefineCursor(wiv.current_Display,
			      XtWindow (XtParent (E_GET_WIDGET(i,0))),
			      Watch_cursor);
	      }
	    else if (onoroff EQUALS FALSE)
	      {
		XUndefineCursor(wiv.current_Display,
				XtWindow (XtParent (E_GET_WIDGET(i,0))));
	      }
	  }
      }

  for (i = 0; i < wmv_current->max_ws; i++)
    {
      if (wmv_current->gks_widget_array[i].widget IS_NOT_EQUAL_TO (Widget)0)
	{
	  if (onoroff EQUALS TRUE)
	    {
#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
	      XDefineCursor(wiv.current_Display,
			    XtWindow (wmv_current->gks_widget_array[i].widget),
			    Watch_cursor);
#else /* ========== END PRIOR GKS = BEGIN DEC GKS ========== */
	      XDefineCursor(wiv.current_Display,
			    XtWindow (XtParent (wmv_current->gks_widget_array[i].widget)),
			    Watch_cursor);
#endif  /* ========== END DEC GKS ========== */
	    }
	  else
	    {
#ifdef PRIOR_GKS /* ========== BEGIN PRIOR GKS ========== */
	      XUndefineCursor(wiv.current_Display,
			    XtWindow (wmv_current->gks_widget_array[i].widget));
#else /* ========== END PRIOR GKS = BEGIN DEC GKS ========== */
	      XUndefineCursor(wiv.current_Display,
			    XtWindow (XtParent (wmv_current->gks_widget_array[i].widget)));
#endif  /* ========== END DEC GKS ========== */
	    }
	}
    }

  return(SUCCESS);

}

#endif

