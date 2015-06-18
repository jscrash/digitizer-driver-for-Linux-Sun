/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_MENU.C*/
/* *9    18-JUN-1991 13:02:42 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1*/
/*version"*/
/* *8    23-OCT-1990 13:55:02 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *7    27-JUN-1990 17:27:45 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *6    12-APR-1990 20:59:34 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *5    15-MAR-1990 13:36:31 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "*/
/* *4     2-MAR-1990 11:17:32 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"*/
/* *3     6-FEB-1990 00:29:24 JULIAN "(SPR -1) X Version of Wi Module (Kee)"*/
/* *2     5-FEB-1990 23:34:02 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:08 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_MENU.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_MENU.C                               */
/* *7    27-JUN-1990 17:27:45 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *6    12-APR-1990 20:59:34 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *5    15-MAR-1990 13:36:31 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "    */
/* *4     2-MAR-1990 11:17:32 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"    */
/* *3     6-FEB-1990 00:29:24 JULIAN "(SPR -1) X Version of Wi Module (Kee)"        */
/* *2     5-FEB-1990 23:34:02 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:08 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEF_MENU.C                               */
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
/*NAME:    WI_DEFINE_MENU                                                           */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Define menu                                                                 */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions required here */

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
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_define_menu (themenu, help_key, menuid)    /*************************************************************************/
MENU *themenu;
CHAR *help_key;
INT *menuid;
    {
    PROGNAME ("wi_define_menu");
    INT i;

#ifdef USE_X
    INT k, n;
    INT command_length;
    INT accelerator;
    CHAR st1[ST_COMMAND_MAX];
    CHAR st2[ST_COMMAND_MAX];
    CHAR st3[ST_COMMAND_MAX];
    Widget menupulldownwidget;      /* menupulldown widget                          */
    WIDGET_ITEM_INFO *iteminfo;
    DIALOG_MENU *thedialog_menu;
    Arg arglist[MAX_ARG];           /* widget argument list                         */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtCallbackRec callback_arg[2];  /* callback list                                */
    XmString CompoundString[2];
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCallback callback_arg[2];    /* callback list                                */
    DwtCompString CompoundString[2];
#endif  /* ---------- END DECW BLOCK ---------- */
    E_CHECK_INIT;
    E_CHECK_SLEN (themenu->mtitle);
    E_CHECK_COUNT (themenu->num_mn_items);
    
    wiz_jp_name (IDENT);
    
    wiv.dialog_menu_count++;
    if (wiv.dialog_menu_count % 100 == 0)   /* if memory allocation needed          */
        {
        wiv.dialog_menu = (DIALOG_MENU *) tc_realloc (wiv.dialog_menu,
                                                      sizeof(DIALOG_MENU) *
                                                      (wiv.dialog_menu_count + 100));
        }
    thedialog_menu = E_DIALOG_MENU (wiv.dialog_menu_count);
    /*                                                                              */
    /*  thedialog_menu->server, thedialog_menu->pointer and thedialog_menu->app_id  */
    /*  are set at am_define_resource                                               */
    /*                                                                              */
    
    thedialog_menu->wi_push_pop_num = 0;
    thedialog_menu->num_items = themenu->num_mn_items;
    /* allocate widget items array, the + 1 is for parent widget                    */
    thedialog_menu->items = (WIDGET_ITEM_INFO *) tc_zalloc (sizeof(WIDGET_ITEM_INFO) *
                                                            (themenu->num_mn_items +
                                                             1));
    
    /* for pulldownmenuentry widget, there's a corresponding                        */
    /*   pulldownmenu widget                                                        */
    iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, 0);
    iteminfo->type_id = DMENU;
    iteminfo->group = 0;            /* group is not reference by menu or its items  */
    iteminfo->text = STRING_ALLOCATE (themenu->mtitle);
    strcpy (iteminfo->text, themenu->mtitle);
    iteminfo->item_type.menu.if_static = themenu->ifstatic;
    iteminfo->item_type.menu.if_main = themenu->ifmain;

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    /*                                                                              */
    /*  pulldownmenuentry widget string = MENU_xx_00_00                             */
    /*  menupulldownwidget string = MENU_xx_00_01                                   */
    /*  menu seperator widget string = MENU_xx_yy_00                                */
    /*  menu item push button widget string = MENU_xx_yy_00                         */
    /*  menu item toggle button widget string = MENU_xx_yy_01                       */
    /*  where xx = menuid, yy = itemid                                              */
    /*                                                                              */
    /* create menupulldownmenu widget without convenience functions                 */
    /*   in order to fix our menu goes to the left hand corner bug                  */
    sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, 0, 1);
    n = 0;
    XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    menupulldownwidget = iteminfo->item_type.menu.pull_down_menu =
      XmCreatePulldownMenu (wiv.mainmenubar, st1, arglist, n);
    
    n = 0;
    CompoundString[0] = XmStringCreate (themenu->mtitle, XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[n], XmNlabelString, CompoundString[0]);
    n++;
    XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    XtSetArg (arglist[n], XmNsubMenuId, menupulldownwidget);
    n++;
    sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, 0, 0);
    iteminfo->widget = XmCreateCascadeButton (wiv.mainmenubar, st1, arglist, n);
    XmStringFree (CompoundString[0]);
    
    /* create individual menu items                                                 */
    /* themenu->item[i] = wiv.dialog_menu[j].items[i+1], they                       */
    /*   are off by 1 because wiv.dialog_menu[j].items[0] is resverse for           */
    /*   parent widget                                                              */
    for (i = 0; i < themenu->num_mn_items; i++)
        {
        iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
        iteminfo->group = 0;        /* group is not reference by menu or its items  */
        accelerator = FALSE;
        sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
        if (strcmp (themenu->item[i].command, "(-") == 0)
            {
            iteminfo->widget = XmCreateSeparatorGadget (menupulldownwidget, st1,
                                                        arglist, 0);
            XtManageChild (iteminfo->widget);
            iteminfo->type_id = MSEPERATOR;
            }
        else
            {
            iteminfo->type_id = MITEM;
            iteminfo->item_type.mitem.enable_flag = ON;
            iteminfo->item_type.mitem.check_onoff = OFF;
            iteminfo->item_type.mitem.menu_toggle_button = (Widget)NULL;
            
            command_length = strlen (themenu->item[i].command);
            n = 0;
            if (themenu->item[i].command[command_length - 2] == '/')
                {                   /* accelertor                                   */
                accelerator = TRUE;
                sprintf (st2, "%c", themenu->item[i].command[command_length - 1]);
                sprintf (st3, "^%s", st2);
                CompoundString[0] = XmStringCreate (st3, XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNacceleratorText, CompoundString[0]);
                n++;
                ts_sto_lower (st2);
                sprintf (st3, "Ctrl<KeyPress>%s:", st2);
                XtSetArg (arglist[n], XmNaccelerator, st3);
                n++;
                themenu->item[i].command[command_length - 2] = NUL;
                }
            if (themenu->item[i].command[0] == '(')
                {                   /* sensitive on/off                             */
                iteminfo->item_type.mitem.enable_flag = FALSE;
                XtSetArg (arglist[n], XmNsensitive, FALSE);
                n++;
                for (k = 0, command_length = strlen (themenu->item[i].command);
                     k < command_length; k++)
                    {
                    themenu->item[i].command[k] = themenu->item[i].command[k + 1];
                    }
                }
            CompoundString[1] = XmStringCreate (themenu->item[i].command,
                                                XmSTRING_DEFAULT_CHARSET);
            XtSetArg (arglist[n], XmNlabelString, CompoundString[1]);
            n++;
            XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[NORMAL_P_TEXT]);
            n++;
            iteminfo->text = STRING_ALLOCATE (themenu->item[i].command);
            strcpy (iteminfo->text, themenu->item[i].command);
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((wiv.dialog_menu_count << 16) |
                                                (((i + 1) & 0xFF) << 8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[n], XmNactivateCallback, callback_arg);
	    n++;
            iteminfo->widget = XmCreatePushButtonGadget (menupulldownwidget, st1,
                                                         arglist, n);
            XtManageChild (iteminfo->widget);
            if (accelerator == TRUE)    /* free acelerator compound string          */
                {
                XtInstallAllAccelerators (wiv.mainw, wiv.mainw);
                XmStringFree (CompoundString[0]);
                }
            XmStringFree (CompoundString[1]);
            }
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    /*                                                                              */
    /*  pulldownmenuentry widget string = MENU_xx_00_00                             */
    /*  menupulldownwidget string = MENU_xx_00_01                                   */
    /*  menu seperator widget string = MENU_xx_yy_00                                */
    /*  menu item push button widget string = MENU_xx_yy_00                         */
    /*  menu item toggle button widget string = MENU_xx_yy_01                       */
    /*  where xx = menuid, yy = itemid                                              */
    /*                                                                              */
    /* create menupulldownmenu widget                                               */
    n = 0;
    sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, 0, 1);
    XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    menupulldownwidget = iteminfo->item_type.menu.pull_down_menu =
        DwtMenuPulldownCreate (wiv.mainmenubar, st1, arglist, n);
    
    /* create pulldownmenuentry widget                                              */
    sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, 0, 0);
    n = 0;
    CompoundString[0] = DwtLatin1String (themenu->mtitle);
    XtSetArg (arglist[n], DwtNlabel, CompoundString[0]);
    n++;
    XtSetArg (arglist[n], DwtNsubMenuId, menupulldownwidget);
    n++;
    XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
    n++;
    iteminfo->widget = DwtPullDownMenuEntryCreate (wiv.mainmenubar, st1, arglist, n);
    XtFree (CompoundString[0]);
    
    /* create individual menu items                                                 */
    /* themenu->item[i] = wiv.dialog_menu[j].items[i+1], they                       */
    /*   are off by 1 because wiv.dialog_menu[j].items[0] is resverse for           */
    /*   parent widget                                                              */
    for (i = 0; i < themenu->num_mn_items; i++)
        {
        iteminfo = E_WIDGET_INFO (wiv.dialog_menu_count, i + 1);
        iteminfo->group = 0;        /* group is not reference by menu or its items  */
        accelerator = FALSE;
        sprintf (st1, "MENU_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
        if (strcmp (themenu->item[i].command, "(-") == 0)
            {
            iteminfo->widget = DwtSeparatorGadgetCreate (menupulldownwidget, st1,
                                                         arglist, 0);
            XtManageChild (iteminfo->widget);
            iteminfo->type_id = MSEPERATOR;
            }
        else
            {
            iteminfo->type_id = MITEM;
            iteminfo->item_type.mitem.enable_flag = ON;
            iteminfo->item_type.mitem.check_onoff = OFF;
            iteminfo->item_type.mitem.menu_toggle_button = (Widget)NULL;
            
            command_length = strlen (themenu->item[i].command);
            n = 0;
            if (themenu->item[i].command[command_length - 2] == '/')
                {                   /* accelertor                                   */
                accelerator = TRUE;
                sprintf (st2, "%c", themenu->item[i].command[command_length - 1]);
                sprintf (st3, "^%s", st2);
                CompoundString[0] = DwtLatin1String (st3);
                XtSetArg (arglist[n], DwtNacceleratorText, CompoundString[0]);
                n++;
                ts_sto_lower (st2);
                sprintf (st3, "Ctrl<KeyPress>%s:", st2);
                XtSetArg (arglist[n], DwtNbuttonAccelerator, st3);
                n++;
                themenu->item[i].command[command_length - 2] = NUL;
                }
            if (themenu->item[i].command[0] == '(')
                {                   /* sensitive on/off                             */
                iteminfo->item_type.mitem.enable_flag = FALSE;
                XtSetArg (arglist[n], DwtNsensitive, FALSE);
                n++;
                for (k = 0, command_length = strlen (themenu->item[i].command);
                     k < command_length; k++)
                    {
                    themenu->item[i].command[k] = themenu->item[i].command[k + 1];
                    }
                }
            CompoundString[1] = DwtLatin1String (themenu->item[i].command);
            XtSetArg (arglist[n], DwtNlabel, CompoundString[1]);
            n++;
            XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[NORMAL_P_TEXT]);
            n++;
            iteminfo->text = STRING_ALLOCATE (themenu->item[i].command);
            strcpy (iteminfo->text, themenu->item[i].command);
            callback_arg[0].proc = (VoidProc)wi_callback;
            callback_arg[0].tag = (wiv.dialog_menu_count << 16) | (((i + 1) & 0xFF) <<
                                                                   8) | (0 & 0xFF);
            callback_arg[1].proc = (VoidProc)NULL;
            XtSetArg (arglist[n], DwtNactivateCallback, callback_arg);
	    n++;
            iteminfo->widget = DwtPushButtonCreate (menupulldownwidget, st1, arglist,
                                                    n);
            XtManageChild (iteminfo->widget);
            if (accelerator == TRUE)    /* free acelerator compound string          */
                {
                XtInstallAllAccelerators (wiv.mainw, wiv.mainw);
                XtFree (CompoundString[0]);
                }
            XtFree (CompoundString[1]);
            }
        }
#endif  /* ---------- END DECW BLOCK ---------- */
    *menuid = wiv.dialog_menu_count;

#else        /* ESITERM code */
    INT itemp;
    
    E_CHECK_INIT;
    E_CHECK_SLEN (themenu->mtitle);
    E_CHECK_COUNT (themenu->num_mn_items);
    
    wiz_jp_name (IDENT);
    
    /* %Q Should save state and only transmit if changed.                           */
    /* Send menu mode.                                                              */
    itemp = themenu->ifstatic ? 0 : 1;
    
    SEND_2_VALUES (itemp, MC_SET_MM);
    
    /* Send menu type.                                                              */
    itemp = themenu->ifmain ? 0 : 1;
    SEND_2_VALUES (itemp, MC_SET_MT);
    
    /* Send initial menu definition                                                 */
    SEND_PROMPT (themenu->mtitle);
    SEND_1_VALUE (MC_DEFINE_MN);
    
    wiv.playback_eatrcv = 1;        /* eat the returned ID from the MAC             */
    wiz_set_echo (FALSE);
    wiz_gnum (menuid);
    wiz_set_echo (TRUE);
    wiv.playback_eatrcv = 0;
    
    wiz_acknowledge ();
    
    /* send individual menu items                                                   */
    for (i = 0; i < themenu->num_mn_items; i++)
        {
        SEND_PROMPT (themenu->item[i].command);
        SEND_1_VALUE (MC_DEFINE_MI);
        }
#endif
    RETURN (SUCCESS);
    }
