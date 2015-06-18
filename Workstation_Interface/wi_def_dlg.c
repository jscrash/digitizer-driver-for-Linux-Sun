/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_DLG.C*/
/* *16   27-JUN-1991 15:39:08 SANTIAGO "(SPR 8438) removed all references to wiv.color, not correct for an X client"*/
/* *15   18-JUN-1991 13:02:17 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1*/
/*version"*/
/* *14   13-JUN-1991 14:47:08 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method"*/
/* *13    3-MAY-1991 16:13:33 KEE "(SPR -1) MOTIF/PRIORGKS, MOTIF/DECWGKS, DECW/PRIORGKS, DECW/DECGKS version"*/
/* *12   23-OCT-1990 13:54:31 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *11    2-JUL-1990 16:03:41 KEE "(SPR -1) set selector.max_sel=1 if nitems=0, see comment of the source code"*/
/* *10   27-JUN-1990 17:27:28 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *9    12-JUN-1990 10:37:20 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *8     3-MAY-1990 21:50:01 KEE "(SPR -1) menu sync fix."*/
/* *7    12-APR-1990 20:58:42 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *6    31-MAR-1990 15:40:54 KEE "(SPR 5162) process edittext forward "*/
/* *5    15-MAR-1990 13:36:19 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "*/
/* *4     2-MAR-1990 11:17:19 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"*/
/* *3     8-FEB-1990 17:56:49 JULIAN "(SPR -1) X Version some minor fixing (Kee)"*/
/* *2     5-FEB-1990 23:33:44 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:05 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_DLG.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_DLG.C                                */
/* *11    2-JUL-1990 16:03:41 KEE "(SPR -1) set selector.max_sel=1 if nitems=0, see comment of the source code" */
/* *10   27-JUN-1990 17:27:28 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *9    12-JUN-1990 10:37:20 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"  */
/* *8     3-MAY-1990 21:50:01 KEE "(SPR -1) menu sync fix."                         */
/* *7    12-APR-1990 20:58:42 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *6    31-MAR-1990 15:40:54 KEE "(SPR 5162) process edittext forward "            */
/* *5    15-MAR-1990 13:36:19 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "    */
/* *4     2-MAR-1990 11:17:19 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"    */
/* *3     8-FEB-1990 17:56:49 JULIAN "(SPR -1) X Version some minor fixing (Kee)"   */
/* *2     5-FEB-1990 23:33:44 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:05 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_DLG.C                                */
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


/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WI_DEFINE_DIALOG                                                      */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Define custom dialog on MAC                                                   */
/*                                                                                  */
/*   ************************************************************************       */

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#ifndef ESI_TS_H
#include "esi_ts.h"
#endif
#ifndef ESI_TC_H
#include "esi_tc.h"
#endif
#ifndef ESI_AM_INT_H
#include "esi_am_int.h"
#endif

#if USE_PROTOTYPES
static XtEventHandler wiz_update_front_dialog_id (Widget widget, INT data,
                                                      XEvent *event);
#else
static XtEventHandler wiz_update_front_dialog_id ();
#endif
#endif

/* **********************************************************************           */
publicdef INT wi_define_dialog (thecdlg, help_key, cdlgid)  /* **********************************************************************   */
CUSTOM_DIALOG *thecdlg;
CHAR *help_key;
INT *cdlgid;
    {
    PROGNAME ("wi_define_dialog");
#define IDENT progname
    INT i;
    INT current_cd_group;
    INT current_cd_font;
    INT current_cd_textsize;
    INT id;
    CHAR st1[ST_COMMAND_MAX];
    INPUT_RECORD inp_rec;

#ifdef USE_X

#define VALUATOR_LEFT_MARGIN  8
#define VALUATOR_MARGIN       16
#define VALUATOR_HEIGHT_RATIO 1.6
#define DIALOG_BOX_OFFSET     45
#define GROUP_SPACING         4

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */

#define ITEMS_HEIGHT          15
#define SCROLL_BAR_WIDTH      15
#define VSCROLL_X_MARGIN      14
#define VSCROLL_Y_MARGIN      5
#define VSCROLL_WIDTH_MARGIN  15
#define VSCROLL_HEIGHT_MARGIN 29
    
    /* slant_left.bits                                                              */
#define slant_left_width 16
#define slant_left_height 16
#define slant_left_x_hot -1
#define slant_left_y_hot -1
    static char slant_left_bits[] = 
        {
        0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd,
        0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77,
        0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee
        };

#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */

#define ITEMS_HEIGHT          25
#define SCROLL_BAR_WIDTH      17
#define VSCROLL_X_MARGIN      4
#define VSCROLL_Y_MARGIN      0
#define VSCROLL_WIDTH_MARGIN  17
#define VSCROLL_HEIGHT_MARGIN 0

#endif  /* ---------- END DECW BLOCK ---------- */
    INT j, k, l, n;
    Dimension selector_width;
    INT bar_xmax, bar_ymin, bar_height, radio_xmin, radio_xmax, radio_ymin, radio_ymax;
    INT selector_num = 0;
    INT last_radioitem, radio_itemnum, radio_horizontal;
    INT change_font = FALSE, small_textsize = FALSE;
    Widget popupdialog;
    Widget radioboxwidget;
    WIDGET_ITEM_INFO *iteminfo;
    DIALOG_MENU *thedialog_menu;
    Arg arglist[MAX_ARG];
    APPLICATION_LIST app;
    INT app_id;                     /* key of data in app                           */
    APPLICATION_STATIC *application_static = am_static ();
    TCP tcp = (TCP)0;
    CHAR *tmpstr;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    Pixmap slant_left_pixmap;
    Widget radioframewidget;
    XmString *list_items;
    XmString CompoundString;
    XtCallbackRec callback_arg[2];
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString *list_items;
    DwtCompString CompoundString;
    DwtCallback callback_arg[2];
#endif  /* ---------- END DECW BLOCK ---------- */
    current_cd_group = -1;          /*  Set initial group value.                    */
    current_cd_font = 0;            /*  Set initial FONT value.                     */
    current_cd_textsize = 12;       /*  Set initial TEXTSIZE value.                 */
    
    /*  Send initial custom dialog definition                                       */
    wiv.dialog_menu_count++;
    if (wiv.dialog_menu_count % 100 == 0)   /* if memory allocation needed          */
        {
        wiv.dialog_menu = (DIALOG_MENU *) tc_realloc (wiv.dialog_menu,
                                                      sizeof(DIALOG_MENU) *
                                                      (wiv.dialog_menu_count + 100));
        }
    thedialog_menu = E_DIALOG_MENU (wiv.dialog_menu_count);
    /*                                                                              */
    /*  thedialog_menu->server                                                      */
    /*  thedialog_menu->pointer                                                     */
    /*  thedialog_menu->app_id                                                      */
    /*  set at am_define_resource                                                   */
    /*                                                                              */
    
    thedialog_menu->wi_push_pop_num = 0;
    thedialog_menu->num_items = thecdlg->num_cd_items;
    /* allocate widget items array, the + 1 is for parent widget                    */
    /*   ie. popupdialog                                                            */
    thedialog_menu->items = (WIDGET_ITEM_INFO *) tc_zalloc (sizeof(WIDGET_ITEM_INFO) *
                                                            (thecdlg->num_cd_items +
                                                             1));
    
    /*                                                                              */
    /*  pop up dialog widget string = DIALOG_xx_00_00                               */
    /*  dialog item = DIALOG_xx_yy_00                                               */
    /*  where xx = cdlgid, yy == itemid, and the last two digit are reserve         */
    /*  for the push button of the polldown_menu of the dialog                      */
    /*                                                                              */
    
    iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, 0);
    iteminfo->type_id = DIALOG;
    iteminfo->group = 0;            /* group is not reference by pop up dialog      */
    sprintf (st1, "%s - %s", wiv.current.owner_tag, thecdlg->cd_title);
    iteminfo->text = STRING_ALLOCATE (st1);
    strcpy (iteminfo->text, st1);
    iteminfo->item_type.dialog.clickback_flag = FALSE;
    /* Initialize flag for wi_set_cd_clickback to be false                          */

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    n = 0;                          /* set all attributes of the pop up dialog box  */
    sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, 0, 0);
    CompoundString = XmStringCreate (thecdlg->cd_title, XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[n], XmNallowOverlap, FALSE);
    n++;
    XtSetArg (arglist[n], XmNautoUnmanage, FALSE);
    n++;
    XtSetArg (arglist[n], XmNdialogTitle, CompoundString);
    n++;
    XtSetArg (arglist[n], XmNnoResize, TRUE);
    n++;
    XtSetArg (arglist[n], XmNdefaultPosition, FALSE);
    n++;
    XtSetArg (arglist[n], XmNshadowType, XmSHADOW_ETCHED_OUT);
    n++;
    XtSetArg (arglist[n], XmNresizePolicy, XmRESIZE_NONE);
    n++;
    XtSetArg (arglist[n], XmNx, thecdlg->cd_rect.ixmin);
    n++;
    XtSetArg (arglist[n], XmNy, thecdlg->cd_rect.iymin + DIALOG_BOX_OFFSET);
    n++;
    XtSetArg (arglist[n], XmNwidth, thecdlg->cd_rect.ixmax - thecdlg->cd_rect.ixmin);
    n++;
    XtSetArg (arglist[n], XmNheight, thecdlg->cd_rect.iymax - thecdlg->cd_rect.iymin);
    n++;
    XtSetArg (arglist[n], XmNunitType, XmPIXELS);
    n++;
    popupdialog = iteminfo->widget = XmCreateBulletinBoardDialog (wiv.mainw, st1,
                                                                  arglist, n);
    XtAddEventHandler (popupdialog, StructureNotifyMask, TRUE,
                       (XtEventHandler)wiz_update_front_dialog_id,
                       (caddr_t)wiv.dialog_menu_count);
    XtAddEventHandler (XtParent (popupdialog), FocusChangeMask, TRUE,
                       (XtEventHandler)wiz_update_front_dialog_id,
                       (caddr_t)wiv.dialog_menu_count);
    XmStringFree (CompoundString);
    /* end of popupdialog creation                                                  */
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    n = 0;                          /* set all attributes of the pop up dialog box  */
    sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, 0, 0);
    CompoundString = DwtLatin1String (thecdlg->cd_title);
    XtSetArg (arglist[n], DwtNtitle, CompoundString);
    n++;
    XtSetArg (arglist[n], DwtNx, thecdlg->cd_rect.ixmin);
    n++;
    XtSetArg (arglist[n], DwtNy, thecdlg->cd_rect.iymin + DIALOG_BOX_OFFSET);
    n++;
    XtSetArg (arglist[n], DwtNwidth, thecdlg->cd_rect.ixmax - thecdlg->cd_rect.ixmin);
    n++;
    XtSetArg (arglist[n], DwtNheight, thecdlg->cd_rect.iymax - thecdlg->cd_rect.iymin);
    n++;
    XtSetArg (arglist[n], DwtNunits, DwtPixelUnits);
    n++;
    XtSetArg (arglist[n], DwtNtakeFocus, TRUE);
    n++;
    popupdialog = iteminfo->widget = DwtDialogBoxPopupCreate (wiv.mainw, st1, arglist,
                                                              n);
    XtAddEventHandler (popupdialog, StructureNotifyMask, TRUE,
                       (XtEventHandler)wiz_update_front_dialog_id,
                       (caddr_t)wiv.dialog_menu_count);
    XtAddEventHandler (XtParent (popupdialog), FocusChangeMask, TRUE,
                       (XtEventHandler)wiz_update_front_dialog_id,
                       (caddr_t)wiv.dialog_menu_count);
    XtFree (CompoundString);
    /* end of popupdialog creation                                                  */
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /* create menu item for each dialog under the "Windows" menu                    */
    /* the attributes have to be the same as that in wi_define_menu                 */
    n = 0;
    sprintf (st1, "%s - %s", wiv.current.owner_tag, thecdlg->cd_title);
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    CompoundString = XmStringCreate (st1, XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[n], XmNlabelString, CompoundString);
    n++;
    sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, 0, 1);
    XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    callback_arg[0].callback = (XtCallbackProc)wi_callback;
    callback_arg[0].closure = (caddr_t)((wiv.dialog_menu_count << 16) |
                                        ((0 & 0xFF) << 8) | (1 & 0xFF));
    callback_arg[1].callback = (XtCallbackProc)NULL;
    XtSetArg (arglist[n], XmNactivateCallback, callback_arg);
    n++;
    iteminfo->item_type.dialog.popup_icon =
        XmCreatePushButtonGadget (wiv.main_dialog_menu[0], st1, arglist, n);
    XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    CompoundString = DwtLatin1String (st1);
    XtSetArg (arglist[n], DwtNshadow, FALSE);
    n++;
    XtSetArg (arglist[n], DwtNlabel, CompoundString);
    n++;
    sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, 0, 1);
    XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    callback_arg[0].proc = (VoidProc)wi_callback;
    callback_arg[0].tag = (wiv.dialog_menu_count << 16) | ((0 & 0xFF) << 8) |
        (1 & 0xFF);
    callback_arg[1].proc = (VoidProc)NULL;
    XtSetArg (arglist[n], DwtNactivateCallback, callback_arg);
    n++;
    iteminfo->item_type.dialog.popup_icon =
        DwtPushButtonCreate (wiv.main_dialog_menu[0], st1, arglist, n);
    XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /* update individual dialog item                                                */
    /*                                                                              */
    /*  Remark : All items should be defined in reverse order except edittext.      */
    /*  I try to define/create items in forword order and everything comes out      */
    /*  in reverse, that's why all items are defined in reverse order.              */
    /*  Edittext - since the tab key sequence in edittext has to follow the         */
    /*  sequence in resource(.rs) file, that's why they can't be in reverse order   */
    /*                                                                              */
    /* now take care of all edittext first in forward order                         */
    /* thecdlg->cd_item[i] = wiv.dialog_menu[j].items[i+1], they                    */
    /*   are off by 1 because wiv.dialog_menu[j].items[0] is resverse for           */
    /*   parent widget                                                              */
    /* ----------------------------------------------------                         */
    /* ---       Process Edittext (forward order)       ---                         */
    /* ----------------------------------------------------                         */
    for (i = 0; i < thecdlg->num_cd_items; i++)
        {
        iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
        iteminfo->type_id = thecdlg->cd_item[i].cd_item_type;
        iteminfo->group = thecdlg->cd_item[i].cd_item_group;
        iteminfo->text = STRING_ALLOCATE (thecdlg->cd_item[i].cd_item_text);
        strcpy (iteminfo->text, thecdlg->cd_item[i].cd_item_text);
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        /*  Send new TEXTSIZE command if it changes.                                */
        if (thecdlg->cd_item[i].cd_item_textsize != current_cd_textsize)
            {
            current_cd_textsize = thecdlg->cd_item[i].cd_item_textsize;
            small_textsize = (thecdlg->cd_item[i].cd_item_textsize != 12) ?
                TRUE : FALSE;
            }
        if (thecdlg->cd_item[i].cd_item_type == EDITTEXT)
            {
            n = 0;
            XtSetArg (arglist[n], XmNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNy, thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], XmNwidth,
                      thecdlg->cd_item[i].cd_item_rect.ixmax -
                      thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNheight,
                      thecdlg->cd_item[i].cd_item_rect.iymax -
                      thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], XmNvalue, thecdlg->cd_item[i].cd_item_text);
            n++;
            XtSetArg (arglist[n], XmNresizeHeight, FALSE);
            n++;
            XtSetArg (arglist[n], XmNresizeWidth, FALSE);
            n++;
            XtSetArg (arglist[n], XmNwordWrap, TRUE);
            n++;
            XtSetArg (arglist[n], XmNeditMode, XmMULTI_LINE_EDIT);
            n++;
            if (small_textsize == TRUE) /* constant width text font                 */
	      {
                if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_C_FONT]);
		else
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_C_TEXT]);
	      }
            else
	      {
                if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_C_FONT]);
		else
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_C_TEXT]);
	      }
            n++;
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((wiv.dialog_menu_count << 16) |
                                                (((i + 1) & 0xFF) << 8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[n], XmNvalueChangedCallback, callback_arg);
	    n++;
            if ((i + 1) < thecdlg->num_cd_items &&
                thecdlg->cd_item[i].cd_item_group ==
                thecdlg->cd_item[i + 1].cd_item_group &&
                thecdlg->cd_item[i + 1].cd_item_type == VSCROLL)
                {                   /* edit text with scroll bar                    */
                XtSetArg (arglist[2], XmNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin + SCROLL_BAR_WIDTH +
                          GROUP_SPACING);
                if (thecdlg->cd_item[i].cd_hscroll == FALSE)
                    {
                    XtSetArg (arglist[n], XmNscrollHorizontal, FALSE);
                    n++;
                    XtSetArg (arglist[n], XmNscrollTopSide, FALSE);
                    n++;
                    }
                XtSetArg (arglist[n], XmNscrollLeftSide, FALSE);
                n++;
                XtSetArg (arglist[n], XmNscrollVertical, TRUE);
                n++;
                sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
                iteminfo->widget = XmCreateScrolledText (popupdialog, st1, arglist, n);
                
                n = 0;
                XtSetArg (arglist[n], XmNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], XmNy, thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetArg (arglist[n], XmNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin + SCROLL_BAR_WIDTH +
                          GROUP_SPACING);
                XtSetArg (arglist[n], XmNheight,
                          thecdlg->cd_item[i].cd_item_rect.iymax -
                          thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetValues (XtParent (iteminfo->widget), arglist, n);
                }
            else
                {
                sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
                iteminfo->widget = XmCreateText (popupdialog, st1, arglist, n);
                }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            /*  Send new FONT command if it changes.                                */
            if (thecdlg->cd_item[i].cd_item_font != current_cd_font)
                {
                current_cd_font = thecdlg->cd_item[i].cd_item_font;
                change_font = (thecdlg->cd_item[i].cd_item_font != 0) ? TRUE : FALSE;
                }
            /*  Send new TEXTSIZE command if it changes.                            */
            if (thecdlg->cd_item[i].cd_item_textsize != current_cd_textsize)
                {
                current_cd_textsize = thecdlg->cd_item[i].cd_item_textsize;
                small_textsize = (thecdlg->cd_item[i].cd_item_textsize != 12) ?
                    TRUE : FALSE;
                }
            if (thecdlg->cd_item[i].cd_item_type == EDITTEXT)
                {
                n = 0;
                XtSetArg (arglist[n], DwtNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], DwtNy, thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetArg (arglist[n], DwtNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], DwtNheight,
                          thecdlg->cd_item[i].cd_item_rect.iymax -
                          thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetArg (arglist[n], DwtNvalue, thecdlg->cd_item[i].cd_item_text);
                n++;
                XtSetArg (arglist[n], DwtNhalfBorder, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNresizeHeight, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNresizeWidth, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNwordWrap, TRUE);
                n++;
                if (small_textsize == TRUE) /* constant width text font             */
                    {
		      if (change_font == TRUE) 
			XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_C_FONT]);
		      else
                        XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_C_TEXT]);
                    }
                else
                    {
		      if (change_font == TRUE) 
			XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_C_FONT]);
		      else
                        XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_C_TEXT]);
                    }
                n++;
                if ((i + 1) < thecdlg->num_cd_items &&
                    thecdlg->cd_item[i].cd_item_group ==
                    thecdlg->cd_item[i + 1].cd_item_group &&
                    thecdlg->cd_item[i + 1].cd_item_type == VSCROLL)
                    {               /* edit text with scroll bar                    */
                    XtSetArg (arglist[2], DwtNwidth,
                              thecdlg->cd_item[i].cd_item_rect.ixmax -
                              thecdlg->cd_item[i].cd_item_rect.ixmin +
                              SCROLL_BAR_WIDTH + GROUP_SPACING);
                    XtSetArg (arglist[n], DwtNscrollVertical, TRUE);
                    n++;
                    }
                sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
                iteminfo->widget = DwtSTextCreate (popupdialog, st1, arglist, n);
                callback_arg[0].proc = (VoidProc)wi_callback;
                callback_arg[0].tag = (wiv.dialog_menu_count << 16) |
                    (((i + 1) & 0xFF) << 8) | (0 & 0xFF);
                callback_arg[1].proc = (VoidProc)NULL;
                XtSetArg (arglist[0], DwtNvalueChangedCallback, callback_arg);
		XtSetValues (iteminfo->widget, arglist, 1);
#endif  /* ---------- END DECW BLOCK ---------- */
                iteminfo->type_id = thecdlg->cd_item[i].cd_item_type;
                iteminfo->item_type.edittext.verifylen =
                    thecdlg->cd_item[i].cd_item_verify_len;
                iteminfo->item_type.edittext.verifymode =
                    thecdlg->cd_item[i].cd_item_verify;
                iteminfo->item_type.edittext.activate_flag = (BOOL)FALSE;
                /* initialize activate_flag to false for wi_activate                */
                }
            }
        /* Remark : Please don't change the process items order,                    */
        /*            all items except edittext, selectors, vscrolls                */
        /*  and rectitems have to be processed now                                  */
        /* ----------------------------------------------------                     */
        /* ---   Process other items now (reverse order)    ---                     */
        /* ----------------------------------------------------                     */
        current_cd_font = 0;
        current_cd_textsize = 12;
        /* reset some flag and take care of the rest of the items                   */
        for (i = thecdlg->num_cd_items - 1; i >= 0; i--)
            {
            if (thecdlg->cd_item[i].cd_item_font != current_cd_font)
                {
                current_cd_font = thecdlg->cd_item[i].cd_item_font;
                change_font = (thecdlg->cd_item[i].cd_item_font != 0) ? TRUE : FALSE;
                }
            if (thecdlg->cd_item[i].cd_item_textsize != current_cd_textsize)
                {
                current_cd_textsize = thecdlg->cd_item[i].cd_item_textsize;
                small_textsize = (thecdlg->cd_item[i].cd_item_textsize != 12) ?
                    TRUE : FALSE;
                }
            if (thecdlg->cd_item[i].cd_item_group != current_cd_group)
                {
                current_cd_group = thecdlg->cd_item[i].cd_item_group;
                l = 0;              /*  anlysis the current group                   */
                radio_itemnum = radio_xmax = radio_ymax = 0;
                radio_xmin = radio_ymin = 999;
                for (k = i; thecdlg->cd_item[k].cd_item_group == current_cd_group; k--)
                    {
                    if (thecdlg->cd_item[k].cd_item_type == RADIOITEM)
                        {
                        radio_itemnum++;    /*  find radiobox x/y min/max coordinate*/
                        if (radio_itemnum == 1)
                            {
                            last_radioitem = k;
                            sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count,
                                     last_radioitem + 1, 1);
                            /* last_radioitem is the last radio item                */
                            /*   in that group                                      */
                            /*   the radio_box widget will only store under         */
                            /*   this radio item.                                   */
                            /*                                                      */
                            }
                        radio_xmin = MIN (radio_xmin,
                                          thecdlg->cd_item[k].cd_item_rect.ixmin);
                        radio_ymin = MIN (radio_ymin,
                                          thecdlg->cd_item[k].cd_item_rect.iymin);
                        radio_xmax = MAX (radio_xmax,
                                          thecdlg->cd_item[k].cd_item_rect.ixmax);
                        radio_ymax = MAX (radio_ymax,
                                          thecdlg->cd_item[k].cd_item_rect.iymax);
                        /*  calculate radio items orientation                       */
                        if (l == 0)
                            l = k;
                        else
                            {
                            if (abs (thecdlg->cd_item[k].cd_item_rect.ixmin -
                                     thecdlg->cd_item[l].cd_item_rect.ixmin) <
                                abs (thecdlg->cd_item[k].cd_item_rect.iymin -
                                     thecdlg->cd_item[l].cd_item_rect.iymin))
                                radio_horizontal = FALSE;
                            else
                                radio_horizontal = TRUE;
                            }
                        }
                    }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                if (radio_itemnum > 0)
                    {
                    n = 0;          /* create radio box                             */
                    XtSetArg (arglist[n], XmNmarginWidth, 1);
                    n++;
                    XtSetArg (arglist[n], XmNmarginHeight, 1);
                    n++;
                    XtSetArg (arglist[n], XmNshadowType, XmSHADOW_ETCHED_IN);
                    n++;
                    XtSetArg (arglist[n], XmNx, radio_xmin);
                    n++;
                    XtSetArg (arglist[n], XmNy, radio_ymin);
                    n++;
                    XtSetArg (arglist[n], XmNwidth, radio_xmax - radio_xmin);
                    n++;
                    XtSetArg (arglist[n], XmNheight, radio_ymax - radio_ymin);
                    n++;
                    radioframewidget = XmCreateFrame (popupdialog, st1, arglist, n);
                    XtManageChild (radioframewidget);
                    
                    n = 0;
                    XtSetArg (arglist[n], XmNx, radio_xmin);
                    n++;
                    XtSetArg (arglist[n], XmNy, radio_ymin);
                    n++;
                    XtSetArg (arglist[n], XmNwidth, radio_xmax - radio_xmin);
                    n++;
                    XtSetArg (arglist[n], XmNheight, radio_ymax - radio_ymin);
                    n++;
                    XtSetArg (arglist[n], XmNborderWidth, 0);
                    n++;
		    if (radio_horizontal == TRUE)	/* 910320:sjs: changed from '? :' */
		      XtSetArg (arglist[n], XmNorientation, XmHORIZONTAL);
		    else
		      XtSetArg (arglist[n], XmNorientation, XmVERTICAL);
                    n++;
                    XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
                    n++;
                    XtSetArg (arglist[n], XmNresizeHeight, TRUE);
                    n++;
                    XtSetArg (arglist[n], XmNresizeWidth, TRUE);
                    n++;
                    st1[strlen (st1) - 1] = '2';
                    radioboxwidget = XmCreateRadioBox (radioframewidget, st1, arglist,
                                                       n);
                    XtManageChild (radioboxwidget);
                    
                    /* special forward process for radio items                      */
                    for (j = k + 1; j <= i; j++)
                        {
                        if (thecdlg->cd_item[j].cd_item_type == RADIOITEM)
                            {
                            iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, j + 1);
                            sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count,
                                     j + 1, 0);
                            CompoundString =
                                             XmStringCreate (thecdlg->cd_item[j].
                                                             cd_item_text,
                                                             XmSTRING_DEFAULT_CHARSET);
                            n = 0;
                            XtSetArg (arglist[n], XmNlabelString, CompoundString);
                            n++;
                            XtSetArg (arglist[n], XmNfontList,
                                      wiv.Fontlist[SMALL_P_TEXT]);
                            n++;
                            XtSetArg (arglist[n], XmNborderWidth, 1);
                            n++;
                            callback_arg[0].callback = (XtCallbackProc)wi_callback;
                            callback_arg[0].closure =
                                (caddr_t)((wiv.dialog_menu_count << 16) |
                                                      (((j + 1) & 0xFF) << 8) |
                                                      (0 & 0xFF));
                            callback_arg[1].callback = (XtCallbackProc)NULL;
                            XtSetArg (arglist[n], XmNvalueChangedCallback,
                                      callback_arg);
			    n++;
                            iteminfo->widget =
                                               XmCreateToggleButtonGadget (
                                                                        radioboxwidget,
                                                                           st1,
                                                                           arglist, n);
                            XmStringFree (CompoundString);
                            iteminfo->item_type.radioitem.radio_box =
                                (last_radioitem == j) ? radioboxwidget : (Widget)NULL;
                            }
                        /* end if - RADIOITEM                                       */
                        }
                    /* for                                                          */
                    }
                /* end if - (radio_itemnum > 0)                                     */
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                if (radio_itemnum > 0)
                    {
                    n = 0;          /* create radio box                             */
                    XtSetArg (arglist[n], DwtNx, radio_xmin);
                    n++;
                    XtSetArg (arglist[n], DwtNy, radio_ymin);
                    n++;
                    XtSetArg (arglist[n], DwtNwidth, radio_xmax - radio_xmin);
                    n++;
                    XtSetArg (arglist[n], DwtNheight, radio_ymax - radio_ymin);
                    n++;
                    XtSetArg (arglist[n], DwtNborderWidth, 0);
                    n++;
		    if (radio_horizontal == TRUE) 
		      XtSetArg (arglist[n], DwtNorientation, DwtOrientationHorizontal);
		    else
		      XtSetArg (arglist[n], DwtNorientation, DwtOrientationVertical);
                    n++;
                    XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
                    n++;
                    radioboxwidget = DwtRadioBoxCreate (popupdialog, st1, arglist, n);
                    
                    /* special forward process for radio items                      */
                    for (j = k + 1; j <= i; j++)
                        {
                        if (thecdlg->cd_item[j].cd_item_type == RADIOITEM)
                            {
                            iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, j + 1);
                            sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count,
                                     j + 1, 0);
                            CompoundString =
                                             DwtLatin1String (thecdlg->cd_item[j].
                                                              cd_item_text);
                            n = 0;
                            XtSetArg (arglist[n], DwtNlabel, CompoundString);
                            n++;
                            XtSetArg (arglist[n], DwtNfont,
                                      wiv.Fontlist[SMALL_P_TEXT]);
                            n++;
                            callback_arg[0].proc = (VoidProc)wi_callback;
                            callback_arg[0].tag = (wiv.dialog_menu_count << 16) |
                                (((j + 1) & 0xFF) << 8) | (0 & 0xFF);
                            callback_arg[1].proc = (VoidProc)NULL;
                            XtSetArg (arglist[n], DwtNvalueChangedCallback,
                                      callback_arg);
			    n++;
                            iteminfo->widget =
                                               DwtToggleButtonCreate (
                                                                   radioboxwidget,
                                                                      st1, arglist, n);
                            XtFree (CompoundString);
                            iteminfo->item_type.radioitem.radio_box =
                                (last_radioitem == j) ? radioboxwidget : (Widget)NULL;
                            }
                        /* end if - RADIOITEM                                       */
                        }
                    /* for                                                          */
                    }
                /* end if - (radio_itemnum > 0)                                     */
#endif  /* ---------- END DECW BLOCK ---------- */
                }
            /* end if - thecdlg->cd_item[i].cd_item_group != current_cd_group       */
            iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
            sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
            n = 0;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            XtSetArg (arglist[n], XmNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNy, thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], XmNwidth,
                      thecdlg->cd_item[i].cd_item_rect.ixmax -
                      thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNheight,
                      thecdlg->cd_item[i].cd_item_rect.iymax -
                      thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((wiv.dialog_menu_count << 16) |
                                                (((i + 1) & 0xFF) << 8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            XtSetArg (arglist[n], DwtNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], DwtNy, thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], DwtNwidth,
                      thecdlg->cd_item[i].cd_item_rect.ixmax -
                      thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], DwtNheight,
                      thecdlg->cd_item[i].cd_item_rect.iymax -
                      thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            callback_arg[0].proc = (VoidProc)wi_callback;
            callback_arg[0].tag = (wiv.dialog_menu_count << 16) | (((i + 1) & 0xFF) <<
                                                                   8) | (0 & 0xFF);
            callback_arg[1].proc = (VoidProc)NULL;
#endif  /* ---------- END DECW BLOCK ---------- */
            switch (thecdlg->cd_item[i].cd_item_type)
                {
            case BTNITEM:           /* create push button (btnitem)                 */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                CompoundString = XmStringCreate (thecdlg->cd_item[i].cd_item_text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
		if (small_textsize == TRUE)	/* 910320:sjs: changed from '? :' */
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
		else
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
                n++;
                XtSetArg (arglist[n], XmNrecomputeSize, FALSE);
                n++;
                slant_left_pixmap = XCreatePixmapFromBitmapData (wiv.current_Display,
                                                                 (XtScreen (
                                                                          wiv.
                                                                            toplevel))->
                                                                 root, slant_left_bits,
                                                                 slant_left_width,
                                                                 slant_left_height,
                                                                 BlackPixelOfScreen (
                                                                             XtScreen (
                                                                                   wiv.
                                                                              toplevel))
                                                                 , 
                                                         WhitePixelOfScreen (
                                                                         XtScreen (
                                                                          wiv.
                                                                            toplevel)),
                                                                 DefaultDepth (
                                                                   wiv.
                                                       current_Display,
                                                                   DefaultScreen (
                                                                   wiv.
                                                                     current_Display)));
                    
                XtSetArg (arglist[n], XmNlabelInsensitivePixmap, slant_left_pixmap);
                n++;
                XtSetArg (arglist[n], XmNactivateCallback, callback_arg);
		n++;
                if (thecdlg->cd_item[i].cd_item_verify == -1)   /* default button   */
                    {
                    iteminfo->item_type.btnitem.deflt_btn = TRUE;
                    XtSetArg (arglist[n], XmNshowAsDefault, 1);
                    n++;
                    iteminfo->widget = XmCreatePushButton (popupdialog, st1, arglist,
                                                           n);
                    XmStringFree (CompoundString);
                    /* set it to be default button of pop up dialog box             */
                    XtSetArg (arglist[0], XmNdefaultButton, iteminfo->widget);
                    XtSetValues (popupdialog, arglist, 1);
                    }
                else
                    {
                    iteminfo->item_type.btnitem.deflt_btn = FALSE;
                    iteminfo->widget = XmCreatePushButton (popupdialog, st1, arglist,
                                                           n);
                    XmStringFree (CompoundString);
                    }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                CompoundString = DwtLatin1String (thecdlg->cd_item[i].cd_item_text);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNshadow, FALSE);
                n++;
		if (small_textsize == TRUE) 
		  XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
		else
		  XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
                n++;
		XtSetArg (arglist[n], DwtNactivateCallback, callback_arg);
		n++;
                if (thecdlg->cd_item[i].cd_item_verify == -1)   /* default button   */
                    {
                    iteminfo->item_type.btnitem.deflt_btn = TRUE;
                    XtSetArg (arglist[n], DwtNbordHighlight, TRUE);
                    n++;
                    iteminfo->widget = DwtPushButtonCreate (popupdialog, st1, arglist,
                                                            n);
                    XtFree (CompoundString);
                    /* set it to be default button of pop up dialog box             */
                    XtSetArg (arglist[0], DwtNdefaultButton, iteminfo->widget);
                    XtSetValues (popupdialog, arglist, 1);
                    }
                else
                    {
                    iteminfo->item_type.btnitem.deflt_btn = FALSE;	    
                    iteminfo->widget = DwtPushButtonCreate (popupdialog, st1, arglist,
                                                            n);
                    XtFree (CompoundString);
                    }
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
            case CHKITEM:           /*  create togglebutton (chkitem)               */
                n = 2;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                CompoundString = XmStringCreate (thecdlg->cd_item[i].cd_item_text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
		if (small_textsize == TRUE)	/* 910320:sjs: changed from '? :' */
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
		else
		  XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
		n++;
                XtSetArg (arglist[n], XmNvalueChangedCallback, callback_arg);
		n++;
                iteminfo->widget = XmCreateToggleButton (popupdialog, st1, arglist, n);
                XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                CompoundString = DwtLatin1String (thecdlg->cd_item[i].cd_item_text);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
		if (small_textsize == TRUE) 
		  XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
		else
		  XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
                n++;
                XtSetArg (arglist[n], DwtNvalueChangedCallback, callback_arg);
		n++;
                iteminfo->widget = DwtToggleButtonCreate (popupdialog, st1, arglist,
                                                          n);
                XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
            case STATTEXT:          /*  create stext (static text)              */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[n], XmNcursorPositionVisible, FALSE);
                n++;
                XtSetArg (arglist[n], XmNeditable, FALSE);
                n++;
                XtSetArg (arglist[n], XmNsensitive, FALSE);
                n++;
                XtSetArg (arglist[n], XmNvalue, thecdlg->cd_item[i].cd_item_text);
                n++;
                XtSetArg (arglist[n], XmNresizeHeight, TRUE);
                n++;
                XtSetArg (arglist[n], XmNresizeWidth, FALSE);
                n++;
                XtSetArg (arglist[n], XmNwordWrap, TRUE);
                n++;
                XtSetArg (arglist[n], XmNborderWidth, 0);
                n++;
                if (small_textsize == TRUE)
		  {
		    if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_FONT]);
		    else
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
		  }
                else
		  {
		    if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_FONT]);
		    else
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
		  }
                n++;
                XtSetArg (arglist[n], XmNshadowThickness, 0);
                n++;
                XtSetArg (arglist[n], XmNeditMode, XmMULTI_LINE_EDIT);
                n++;
                XtSetArg (arglist[n], XmNrows, 2);
                n++;
                iteminfo->widget = XmCreateText (popupdialog, st1, arglist, n);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                XtSetArg (arglist[n], DwtNinsertionPointVisible, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNeditable, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNsensitive, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNvalue, thecdlg->cd_item[i].cd_item_text);
                n++;
                XtSetArg (arglist[n], DwtNhalfBorder, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNresizeHeight, TRUE);
                n++;
                XtSetArg (arglist[n], DwtNresizeWidth, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNwordWrap, TRUE);
                n++;
                XtSetArg (arglist[n], DwtNborderWidth, 0);
                n++;
                if (small_textsize == TRUE)
                    {
		      if (change_font == TRUE) 
			XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_FONT]);
		      else
                        XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
                    }
                else
                    {
		      if (change_font == TRUE) 
			XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_FONT]);
		      else
                        XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
                    }
                n++;
                iteminfo->widget = DwtSTextCreate (popupdialog, st1, arglist, n);
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
            case VALUATOR:          /*  create scale (valuator)                     */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[0], XmNx,
                        thecdlg->cd_item[i].cd_item_rect.ixmin - VALUATOR_LEFT_MARGIN);
                XtSetArg (arglist[2], XmNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin + VALUATOR_MARGIN);
                XtSetArg (arglist[3], XmNheight,
                          (thecdlg->cd_item[i].cd_item_rect.iymax -
                           thecdlg->cd_item[i].cd_item_rect.iymin) /
                          VALUATOR_HEIGHT_RATIO);
                CompoundString = XmStringCreate ("", XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNtitle, CompoundString);
                n++;
                XtSetArg (arglist[n], XmNshowValue, FALSE);
                n++;
                XtSetArg (arglist[n], XmNorientation, XmVERTICAL);
                n++;
                XtSetArg (arglist[n], XmNvalueChangedCallback, callback_arg);
		n++;
                iteminfo->widget = XmCreateScale (popupdialog, st1, arglist, n);
                XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                XtSetArg (arglist[0], DwtNx,
                        thecdlg->cd_item[i].cd_item_rect.ixmin - VALUATOR_LEFT_MARGIN);
                XtSetArg (arglist[2], DwtNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin + VALUATOR_MARGIN);
                XtSetArg (arglist[3], DwtNheight,
                          (thecdlg->cd_item[i].cd_item_rect.iymax -
                           thecdlg->cd_item[i].cd_item_rect.iymin) /
                          VALUATOR_HEIGHT_RATIO);
                CompoundString = DwtLatin1String ("");
                XtSetArg (arglist[n], DwtNtitle, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNshowValue, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNorientation, DwtOrientationVertical);
                n++;
                XtSetArg (arglist[n], DwtNvalueChangedCallback, callback_arg);
		n++;
                iteminfo->widget = DwtScaleCreate (popupdialog, st1, arglist, n);
                XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
            case LOCKTEXT:          /*  locktext -- is not implemented in finder    */
                break;
            case OPTIONMENU:
                iteminfo->item_type.optionmenu.xmin =
		  thecdlg->cd_item[i].cd_item_rect.ixmin;
                iteminfo->item_type.optionmenu.ymin =
		  thecdlg->cd_item[i].cd_item_rect.iymin;
                if (IS_EMPTY_STRING(thecdlg->cd_item[i].cd_item_text) EQUALS TRUE)
		  {
		    iteminfo->item_type.optionmenu.item_chosen = 0;
		  }
		else
		  {
		    for (j = 0, *tmpstr = thecdlg->cd_item[i].cd_item_text[0],
			 k = strlen(thecdlg->cd_item[i].cd_item_text);
			 j < k; j++)
		      {
			if (thecdlg->cd_item[i].cd_item_text[j] EQUALS '\n')
			  {
			    thecdlg->cd_item[i].cd_item_text[j] = '\0';
			    ts_tcp_append(&tcp, tmpstr);
			    *tmpstr = thecdlg->cd_item[i].cd_item_text[j+1];
			  }
		      }
		    if (IS_EMPTY_STRING(tmpstr) EQUALS FALSE)
		      {
			ts_tcp_append(&tcp, tmpstr);
		      }
		    wi_create_option_menu(wiv.dialog_menu_count, i+1, tcp);
		    ts_tcp_free(tcp);
		    tcp = (TCP)0;
		  }
                break;
                }
            }
        /* Remark : Please don't change the process items order,                    */
        /*            selectors and vscrolls have to be processed                   */
        /*  before rectitems                                                        */
        /* ----------------------------------------------------------------         */
        /* ---    Process Selectors and Vscrolls now (reverse order)    ---         */
        /* ----------------------------------------------------------------         */
        current_cd_group = -1;      /*  Set initial group value.                    */
        current_cd_font = 0;
        current_cd_textsize = 12;
        /* reset some flag and take care of the rest of the items                   */
        for (i = thecdlg->num_cd_items - 1; i >= 0; i--)
            {
            if (thecdlg->cd_item[i].cd_item_font != current_cd_font)
                {
                current_cd_font = thecdlg->cd_item[i].cd_item_font;
                change_font = (thecdlg->cd_item[i].cd_item_font != 0) ? TRUE : FALSE;
                }
            if (thecdlg->cd_item[i].cd_item_textsize != current_cd_textsize)
                {
                current_cd_textsize = thecdlg->cd_item[i].cd_item_textsize;
                small_textsize = (thecdlg->cd_item[i].cd_item_textsize != 12) ?
                    TRUE : FALSE;
                }
            if (thecdlg->cd_item[i].cd_item_group != current_cd_group)
                {
                current_cd_group = thecdlg->cd_item[i].cd_item_group;
                selector_num = 0;   /*  anlysis the current group                   */
                bar_xmax = 0;
                for (k = i; thecdlg->cd_item[k].cd_item_group == current_cd_group; k--)
                    {
                    if (thecdlg->cd_item[k].cd_item_type == SELECTOR)
                        {
                        bar_xmax = MAX (bar_xmax,
                                        thecdlg->cd_item[k].cd_item_rect.ixmax);
                        bar_ymin = thecdlg->cd_item[k].cd_item_rect.iymin;
                        bar_height = thecdlg->cd_item[k].cd_item_rect.iymax -
                                     thecdlg->cd_item[k].cd_item_rect.iymin;
                        selector_num++;
                        }
                    }
                }
            iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
            sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
            n = 0;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            XtSetArg (arglist[n], XmNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNy, thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], XmNwidth,
                      thecdlg->cd_item[i].cd_item_rect.ixmax -
                      thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], XmNheight,
                      thecdlg->cd_item[i].cd_item_rect.iymax -
                      thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((wiv.dialog_menu_count << 16) |
                                                (((i + 1) & 0xFF) << 8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            XtSetArg (arglist[n], DwtNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], DwtNy, thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            XtSetArg (arglist[n], DwtNwidth,
                      thecdlg->cd_item[i].cd_item_rect.ixmax -
                      thecdlg->cd_item[i].cd_item_rect.ixmin);
            n++;
            XtSetArg (arglist[n], DwtNheight,
                      thecdlg->cd_item[i].cd_item_rect.iymax -
                      thecdlg->cd_item[i].cd_item_rect.iymin);
            n++;
            callback_arg[0].proc = (VoidProc)wi_callback;
            callback_arg[0].tag = (wiv.dialog_menu_count << 16) | (((i + 1) & 0xFF) <<
                                                                   8) | (0 & 0xFF);
            callback_arg[1].proc = (VoidProc)NULL;
#endif  /* ---------- END DECW BLOCK ---------- */
            switch (thecdlg->cd_item[i].cd_item_type)
                {
            case SELECTOR:          /*  create list box (selector)                  */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                selector_width = (selector_num > 1) ?
                    (thecdlg->cd_item[i].cd_item_rect.ixmax -
                                 thecdlg->cd_item[i].cd_item_rect.ixmin + 2 *
                                 SCROLL_BAR_WIDTH + GROUP_SPACING) :
                                 (thecdlg->cd_item[i].cd_item_rect.ixmax -
                                  thecdlg->cd_item[i].cd_item_rect.ixmin + 2 *
                                  SCROLL_BAR_WIDTH);
                XtSetArg (arglist[2], XmNwidth, selector_width);
                if (small_textsize == TRUE) /* constant width text font             */
		  {
		    if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_FONT]);
		    else
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
		  }
                else
		  {
		    if (change_font == TRUE)	/* 910320:sjs: changed from '? :' */
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_FONT]);
		    else
		      XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
		  }
                n++;
                XtSetArg (arglist[n], XmNselectionPolicy, XmSINGLE_SELECT);
                n++;
                XtSetArg (arglist[n], XmNlistMarginWidth, 5);
                n++;
                XtSetArg (arglist[n], XmNlistMarginHeight, 5);
                n++;
                XtSetArg (arglist[n], XmNautomaticSelection, TRUE);
                n++;
                XtSetArg (arglist[n], XmNvisibleItemCount,
                          (thecdlg->cd_item[i].cd_item_rect.iymax -
                           thecdlg->cd_item[i].cd_item_rect.iymin) / ITEMS_HEIGHT);
                n++;
                wiz_text_to_cslist (thecdlg->cd_item[i].cd_item_text, &list_items, &l);
                if (l > 0)
                    {
                    XtSetArg (arglist[n], XmNitems, list_items);
                    n++;
                    XtSetArg (arglist[n], XmNitemCount, l);
                    n++;
                    }
                else
                    {
                    l = 1;
                    /* set to one in case application use wi_ins_cd_line to         */
                    /*   add item later and forget to set selection limit to at     */
                    /*   least one (ie. max_sel = 1)                                */
                    }
                XtSetArg (arglist[n], XmNscrolledWindowMarginWidth, 5);
                n++;
                XtSetArg (arglist[n], XmNscrolledWindowMarginHeight, 5);
                n++;
                XtSetArg (arglist[n], XmNlistSizePolicy, XmCONSTANT);
                n++;
                XtSetArg (arglist[n], XmNscrollBarDisplayPolicy, XmSTATIC);
                n++;
                XtSetArg (arglist[n], XmNsingleSelectionCallback, callback_arg); 
		n++;
                iteminfo->widget = XmCreateScrolledList (popupdialog, st1, arglist, n);
                iteminfo->item_type.selector.num_sel = 0;
                iteminfo->item_type.selector.activate_flag = (BOOL)FALSE;
                iteminfo->item_type.selector.max_sel = 1;
                iteminfo->item_type.selector.sel_list = TI_ALLOCATE (1);
                iteminfo->item_type.selector.hscroll = thecdlg->cd_item[i].cd_hscroll;
                /* free list_items                                                  */
		for (j = l-1; j >= 0; j--)
		    {
                    XmStringFree (list_items[j]);
                    }
                
                XtSetArg (arglist[2], XmNwidth, selector_width);
                n = 4;
                XtSetArg (arglist[n], XmNborderWidth, 0);
                n++;
                XtSetValues (XtParent (iteminfo->widget), arglist, n);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                selector_width = (selector_num > 1) ?
                    (thecdlg->cd_item[i].cd_item_rect.ixmax -
                                 thecdlg->cd_item[i].cd_item_rect.ixmin +
                                 SCROLL_BAR_WIDTH + GROUP_SPACING) :
                                 (thecdlg->cd_item[i].cd_item_rect.ixmax -
                                  thecdlg->cd_item[i].cd_item_rect.ixmin +
                                  SCROLL_BAR_WIDTH);
                /*  DEC - does not support set font for list box                    */
                XtSetArg (arglist[2], DwtNwidth, selector_width);
                XtSetArg (arglist[n], DwtNresize, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNmarginWidth, 0);
                n++;
                XtSetArg (arglist[n], DwtNsingleSelection, FALSE);
                n++;
                XtSetArg (arglist[n], DwtNvisibleItemsCount,
                          (thecdlg->cd_item[i].cd_item_rect.iymax -
                           thecdlg->cd_item[i].cd_item_rect.iymin) / ITEMS_HEIGHT);
                n++;
                wiz_text_to_cslist (thecdlg->cd_item[i].cd_item_text, &list_items, &l,
                                    selector_width);
                if (l > 0)
                    {
                    XtSetArg (arglist[n], DwtNitems, list_items);
                    n++;
                    XtSetArg (arglist[n], DwtNitemsCount, l);
                    n++;
                    }
                else
                    {
                    l = 1;
                    /* set to one in case application use wi_ins_cd_line to         */
                    /*   add item later and forget to set selection limit to at     */
                    /*   least one (ie. max_sel = 1)                                */
                    }
                XtSetArg (arglist[n], DwtNsingleCallback, callback_arg); 
		n++;
                XtSetArg (arglist[n], DwtNextendCallback, callback_arg); 
		n++;
                iteminfo->widget = DwtListBoxCreate (popupdialog, st1, arglist, n);
                iteminfo->item_type.selector.num_sel = 0;
                iteminfo->item_type.selector.activate_flag = (BOOL)FALSE;
                iteminfo->item_type.selector.max_sel = 1;
                iteminfo->item_type.selector.sel_list = TI_ALLOCATE (1);
                /* free list_items                                                  */
                for (j = l-1; j >= 0; j--)
                    {
                    XtFree (list_items[j]);
                    }
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
            case VSCROLL:           /*  create scroll bar (vscroll)                 */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                if (selector_num > 1)
                    {
                    n = 0;
                    XtSetArg (arglist[n], XmNx, bar_xmax + VSCROLL_X_MARGIN);
                    n++;
                    XtSetArg (arglist[n], XmNy, bar_ymin + VSCROLL_Y_MARGIN);
                    n++;
                    XtSetArg (arglist[n], XmNwidth, VSCROLL_WIDTH_MARGIN);
                    n++;
                    XtSetArg (arglist[n], XmNheight,
                              bar_height - VSCROLL_HEIGHT_MARGIN);
                    n++;
                    XtSetArg (arglist[n], XmNdragCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNincrementCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNdecrementCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNpageDecrementCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNpageIncrementCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNtoBottomCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNtoTopCallback, callback_arg);
                    n++;
                    XtSetArg (arglist[n], XmNvalueChangedCallback, callback_arg);
                    n++;
                    iteminfo->widget = XmCreateScrollBar (popupdialog, st1, arglist,
                                                          n);
                    }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                if (selector_num > 1)
                    {
                    n = 0;
                    XtSetArg (arglist[n], DwtNx, bar_xmax + VSCROLL_X_MARGIN);
                    n++;
                    XtSetArg (arglist[n], DwtNy, bar_ymin + VSCROLL_Y_MARGIN);
                    n++;
                    XtSetArg (arglist[n], DwtNwidth, VSCROLL_WIDTH_MARGIN);
                    n++;
                    XtSetArg (arglist[n], DwtNheight,
                              bar_height - VSCROLL_HEIGHT_MARGIN);
                    n++;
                    XtSetArg (arglist[n], DwtNvalueChangedCallback, callback_arg);
		    n++;
                    iteminfo->widget = DwtScrollBarCreate (popupdialog, st1, arglist,
                                                           n);
                    }
#endif  /* ---------- END DECW BLOCK ---------- */
                break;
                }
            }
        /* Remark : Please don't change the process items order,                    */
        /*            rectitems have to be processed at the end                     */
        /* -------------------------------------------------                        */
        /* ---   Process Rectitems now (reverse order)   ---                        */
        /* -------------------------------------------------                        */
        for (i = thecdlg->num_cd_items - 1; i >= 0; i--)
            {                       /* create dialog box (rectitem)                 */
            if (thecdlg->cd_item[i].cd_item_type == RECTITEM)
                {
                iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
                sprintf (st1, "DIALOG_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
                n = 0;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[n], XmNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], XmNy, thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetArg (arglist[n], XmNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], XmNheight,
                          thecdlg->cd_item[i].cd_item_rect.iymax -
                          thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                iteminfo->widget = XmCreateBulletinBoard (popupdialog, st1, arglist,
                                                          n);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                XtSetArg (arglist[n], DwtNx, thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], DwtNy, thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                XtSetArg (arglist[n], DwtNwidth,
                          thecdlg->cd_item[i].cd_item_rect.ixmax -
                          thecdlg->cd_item[i].cd_item_rect.ixmin);
                n++;
                XtSetArg (arglist[n], DwtNheight,
                          thecdlg->cd_item[i].cd_item_rect.iymax -
                          thecdlg->cd_item[i].cd_item_rect.iymin);
                n++;
                iteminfo->widget = DwtDialogBoxCreate (popupdialog, st1, arglist, n);
#endif  /* ---------- END DECW BLOCK ---------- */
                }
            }
        *cdlgid = id = wiv.dialog_menu_count;

#else        /* ESITERM code */
        CHAR new_title[STMAX];
        INT vertype;
        INT verlen;
        
        E_CHECK_INIT;
        E_CHECK_COUNT (thecdlg->num_cd_items);
        
        wiz_jp_name (progname);
        
        /*  Set initial group value.                                                */
        current_cd_group = 0;
        
        /*  Set initial FONT value.                                                 */
        current_cd_font = 0;
        
        /*  Set initial TEXTSIZE value.                                             */
        current_cd_textsize = 12;
        
        /*  Send initial custom dialog definition                                   */
        
        sprintf (st1, "Now Defining the '%s' Dialog", thecdlg->cd_title);
        wiz_prepend_tag_id (thecdlg->cd_title, wiv.current.owner_tag,
                            wiv.current.owner_id, new_title);
        SEND_PROMPT (new_title);
        SEND_6_VALUES (thecdlg->cd_rect.iymin, thecdlg->cd_rect.ixmin,
                       thecdlg->cd_rect.iymax, thecdlg->cd_rect.ixmax,
                       thecdlg->num_cd_items, MC_DEFINE_CD);
        
        wiv.playback_eatrcv = 1;    /*  eat the returned ID from the MAC            */
        wiz_set_echo (FALSE);
        wiz_gnum (cdlgid);
        wiz_set_echo (TRUE);
        wiv.playback_eatrcv = 0;
        
        wiz_acknowledge ();
        
        id = *cdlgid;
        
        /*  send individual custom dialog items                                     */
        for (i = 0; i < thecdlg->num_cd_items; i++)
            {
            
            /*  Send new group command if it changes.                               */
            if (thecdlg->cd_item[i].cd_item_group != current_cd_group)
                {
                current_cd_group = thecdlg->cd_item[i].cd_item_group;
                SEND_2_VALUES (current_cd_group, MC_SET_GR);
                }
            /*  Send new FONT command if it changes.                                */
            if (thecdlg->cd_item[i].cd_item_font != current_cd_font)
                {
                current_cd_font = thecdlg->cd_item[i].cd_item_font;
                SEND_2_VALUES (current_cd_font, MC_SET_FONT);
                }
            /*  Send new TEXTSIZE command if it changes.                            */
            if (thecdlg->cd_item[i].cd_item_textsize != current_cd_textsize)
                {
                current_cd_textsize = thecdlg->cd_item[i].cd_item_textsize;
                SEND_2_VALUES (current_cd_textsize, MC_SET_TEXTSIZE);
                }
            /*  change modal state if modal desired...                              */
            if (wiv.current.modal_state != 0)
                {
                SEND_1_VALUE (MC_SET_MODAL);
                }
            /*  Send individual custom dialog item.                                 */
            SEND_PROMPT (thecdlg->cd_item[i].cd_item_text);
            SEND_6_VALUES (thecdlg->cd_item[i].cd_item_rect.iymin,
                           thecdlg->cd_item[i].cd_item_rect.ixmin,
                           thecdlg->cd_item[i].cd_item_rect.iymax,
                           thecdlg->cd_item[i].cd_item_rect.ixmax,
                           thecdlg->cd_item[i].cd_item_type, MC_DEFINE_CI);
            
            /*  check if any verification for this item...                          */
            if (thecdlg->cd_item[i].cd_item_verify != VERIFY_NONE)
                {
                
                /*  NOTE: DefaultButtons get this field set to                      */
                /*  -1...                                                           */
                if (thecdlg->cd_item[i].cd_item_verify != -1)
                    {
                    vertype = thecdlg->cd_item[i].cd_item_verify;
                    verlen = thecdlg->cd_item[i].cd_item_verify_len;
                    wi_set_verify (id, i + 1, vertype, verlen);
                    }
                else
                    {
                    /*  Tell it to be a DefaultButton...                            */
                    wi_set_default_button (id, i + 1, 1);
                    }
                }
            }
        /* end of FOR loop                                                          */
#endif
        
        /*  Set record for tracking.                                                */
        inp_rec.type = WI_DIALOG_EVENT;
        inp_rec.owner_id = wiv.current.owner_id;
        inp_rec.file_menu_mask = 0;
        inp_rec.numitems = thecdlg->num_cd_items;
        strcpy (inp_rec.owner_title, thecdlg->cd_title);
        strcpy (inp_rec.orig_title, thecdlg->cd_title);
        strcpy (inp_rec.owner_tag, wiv.current.owner_tag);
        strcpy (inp_rec.orig_tag, wiv.current.owner_tag);
        
        /*  If the rsrc file didn't define a helpkey,                               */
        /*  use the passed in string...                                             */
        if (wiv.current.rsrc_helpkey[0] EQUALS NUL)
            {
            strcpy (inp_rec.helpkey, help_key);
            }
        else
            {
            strcpy (inp_rec.helpkey, wiv.current.rsrc_helpkey);
            }
        btdel (&wiv.input_tree, &id);
        btadd (&wiv.input_tree, &id, &inp_rec);
        
        RETURN (SUCCESS);
        }
#ifdef USE_X
    /* ************************************************************************     */
    /*                                                                              */
    /*   NAME:    WIZ_UPDATE_FRONT_DIALOG_ID                                        */
    /*                                                                              */
    /*   DESCRIPTION:                                                               */
    /*                                                                              */
    /*   Update the front dialog id and do menu syn if necessary                    */
    /*                                                                              */
    /*   ************************************************************************   */
    static XtEventHandler wiz_update_front_dialog_id (widget, data, event)
    Widget widget;
    INT data;
    XEvent *event;
    /* data is the dialog id                                                        */
        {
        if (event->type == FocusIn OR event->type == MapNotify)
            {
            wiv.frontdialog_id = data;
            if (wiv.wi_push_pop_top == 0)
                {
                wiz_menu_syn ((E_DIALOG_MENU (data))->app_id);
                }
            }
        }
#endif
