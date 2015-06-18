/* DEC/CMS REPLACEMENT HISTORY, Element WI_CR_OP_MENU.C */
/* *13   18-JUN-1991 13:02:09 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1 */
/*version" */
/* *12   13-JUN-1991 14:46:59 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method" */
/* *11    6-JUN-1991 16:42:59 MING "(SPR 0) fix list keep appending when call again" */
/* *10   23-OCT-1990 13:54:23 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *9    27-JUN-1990 17:27:18 KEE "(SPR -1) New wi data structure using 2-D array" */
/* *8    12-JUN-1990 10:37:11 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)" */
/* *7    12-APR-1990 20:57:53 JULIAN "(SPR -1) changed ansiDwt to ansidwt" */
/* *6    15-MAR-1990 13:36:11 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc " */
/* *5     2-MAR-1990 11:17:06 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X" */
/* *4     9-FEB-1990 09:53:54 JULIAN "(SPR -1) Turn the print_widget programmer internal reference to OFF " */
/* *3     8-FEB-1990 17:56:33 JULIAN "(SPR -1) X Version some minor fixing (Kee)" */
/* *2     5-FEB-1990 23:33:32 JULIAN "(SPR -1) X version of WI Module (Kee)" */
/* *1     5-FEB-1990 23:20:49 JULIAN "OPTION MENU CREATE" */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CR_OP_MENU.C */
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

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    WI_CREATE_OPTION_MENU                                                 */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*    Create Option Menu and its push button.                                       */
/*   ************************************************************************       */
#define IDENT progname
publicdef INT wi_create_option_menu (cdlgid, itemid, tcp)
INT cdlgid;
INT itemid;
CHAR **tcp;
    {
    PROGNAME ("wi_create_option_menu");
    
    INT i, n, old_tcp_len, tcp_len, status;
    BOOL destroy_button = FALSE;
    CHAR st1[ST_COMMAND_MAX];
    Widget option_menu;
    WIDGET_ITEM_INFO *option_menuinfo;
    Arg arglist[MAX_ARG];           /* widget arguments list                        */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
    XtCallbackRec callback_arg[2];  /* call back arguments                          */
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
    DwtCallback callback_arg[2];    /* call back arguments                          */
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_jp_name (IDENT);
    E_CHECK_WIDGET (cdlgid, itemid);
    
    option_menuinfo = E_WIDGET_INFO (cdlgid, itemid);
    option_menu = E_GET_WIDGET (cdlgid, itemid);
    
    if (option_menuinfo->type_id == OPTIONMENU)
        {
        old_tcp_len = ts_tcp_len (option_menuinfo->item_type.optionmenu.tcp);
        tcp_len = ts_tcp_len (tcp);
        if (old_tcp_len == tcp_len)
            {
            for (i = 0; i < tcp_len; i++)
                {                   /* test for different tcp                       */
                if (ARE_DIFFERENT (option_menuinfo->item_type.optionmenu.tcp[i],
                                   tcp[i]))
                    {
                    destroy_button = TRUE;
                    break;
                    }
                }
            }
	else
	    {
	    destroy_button = TRUE;
	    }
        if (old_tcp_len == tcp_len && destroy_button == FALSE)
            {                       /* no destroy or creation needed                */
            return(SUCCESS);
            }
        else
            {
            if (destroy_button == TRUE)
                {
                for (i = 0; i < old_tcp_len; i++)
                    {
                    XtDestroyWidget (option_menuinfo->item_type.optionmenu.menu_button[i]);
                    }
                if (old_tcp_len != 0)
                    {
                    tc_free (option_menuinfo->item_type.optionmenu.menu_button);
                    ts_tcp_free (option_menuinfo->item_type.optionmenu.tcp);
		    option_menuinfo->item_type.optionmenu.tcp = (TCP)0;
                    }
                }

	    /* create pull down menu if necessary */
	    E_CHECK_WIDGET (cdlgid, 0);
	    if (option_menuinfo->item_type.optionmenu.pull_down_menu EQUALS
		(Widget) 0)
	      {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
		sprintf (st1, "PULLDNMENU_%d_%d_%d", wiv.dialog_menu_count, i + 1, 0);
		n = 0;
                XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
                n++;
		option_menuinfo->item_type.optionmenu.pull_down_menu = 
		  XmCreatePulldownMenu (E_GET_WIDGET (cdlgid, 0), st1, arglist, n);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
		sprintf (st1, "PULLDNMENU_%d_%d_%d", wiv.dialog_menu_count, i + 1, 1);
		option_menuinfo->item_type.optionmenu.pull_down_menu = 
		  DwtMenuPulldownCreate (E_GET_WIDGET (cdlgid, 0), st1, arglist, 0);
#endif  /* ---------- END DECW BLOCK ---------- */
	      }

            option_menuinfo->item_type.optionmenu.menu_button =
                (Widget *) tc_zalloc (sizeof(Widget) * tcp_len);

            for (i = 0; i < tcp_len; i++)   /* create all the push buttons of the option menu.      */
                                            /*   Remark: this step has to be done first and the push buttons    */
                                            /*   have to be managed before we create the option menu    */
                {

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                sprintf (st1, "DIALOG_%d_%d_%d", cdlgid, itemid, i + 1);
                n = 0;
                CompoundString = XmStringCreate (tcp[i], XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
                n++;
                callback_arg[0].callback = (XtCallbackProc)wi_callback;
                callback_arg[0].closure = (caddr_t)((cdlgid << 16) |
                                                    ((itemid& 0xFF) << 8) |
                                                    ((i + 1) & 0xFF));
                callback_arg[1].callback = (XtCallbackProc)NULL;
                XtSetArg (arglist[n], XmNactivateCallback, callback_arg); 
		n++;
                option_menuinfo->item_type.optionmenu.menu_button[i] =
                    XmCreatePushButton (option_menuinfo->item_type.optionmenu.pull_down_menu,
					st1, arglist, n);
                XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                sprintf (st1, "DIALOG_%d_%d_%d", cdlgid, itemid, i + 1);
                n = 0;
                CompoundString = DwtLatin1String (tcp[i]);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
                n++;
                callback_arg[0].proc = (VoidProc)wi_callback;
                callback_arg[0].tag = (cdlgid << 16) | ((itemid& 0xFF) << 8) |
                    ((i + 1) & 0xFF);
                callback_arg[1].proc = (VoidProc)NULL;
                XtSetArg (arglist[n], DwtNactivateCallback, callback_arg);
		n++;
                option_menuinfo->item_type.optionmenu.menu_button[i] =
                    DwtPushButtonCreate (option_menuinfo->item_type.optionmenu.pull_down_menu,
					 st1, arglist, n);
                XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
                XtManageChild (option_menuinfo->item_type.optionmenu.menu_button[i]);
                ts_tcp_append (&option_menuinfo->item_type.optionmenu.tcp, tcp[i]);
                }
            if (option_menu EQUALS (Widget)0)
	      {
		sprintf (st1, "DIALOG_%d_%d_%d", cdlgid, itemid, 0);
                n = 0;              /* create the option menu                       */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
                XtSetArg (arglist[n], XmNx,
                          option_menuinfo->item_type.optionmenu.xmin - 5);
                n++;
                XtSetArg (arglist[n], XmNy,
                          option_menuinfo->item_type.optionmenu.ymin - 5);
                n++;
                CompoundString = XmStringCreate ("", XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetArg (arglist[n], XmNsubMenuId, option_menuinfo->item_type.optionmenu.pull_down_menu);
                n++;
                XtSetArg (arglist[n], XmNfontList, wiv.Fontlist[SMALL_P_TEXT]);
                n++;
		XtSetArg (arglist[n], XmNunitType, XmPIXELS);
		n++;
                option_menuinfo->widget = 
                    XmCreateOptionMenu (E_GET_WIDGET (cdlgid, 0), st1, arglist, n);
                XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                XtSetArg (arglist[n], DwtNx,
                          option_menuinfo->item_type.optionmenu.xmin - 5);
                n++;
                XtSetArg (arglist[n], DwtNy,
                          option_menuinfo->item_type.optionmenu.ymin - 5);
                n++;
                CompoundString = DwtLatin1String ("");
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNsubMenuId, option_menuinfo->item_type.optionmenu.pull_down_menu);
                n++;
                XtSetArg (arglist[n], DwtNfont, wiv.Fontlist[SMALL_P_TEXT]);
                n++;
		XtSetArg (arglist[n], DwtNunits, DwtPixelUnits);
		n++;
                option_menuinfo->widget = 
                    DwtOptionMenuCreate (E_GET_WIDGET (cdlgid, 0), st1, arglist, n);
                XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
                }
            wi_set_dialog_value (cdlgid, itemid, 1);
            /* set the first menu button in case                                    */
            }
        return(SUCCESS);
        }
    else
        {
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        }
    }


#else
publicdef INT wi_create_option_menu (cdlgid, itemid, tcp)
INT cdlgid;
INT itemid;
CHAR **tcp;
    {
    return SUCCESS;
    }
#endif
