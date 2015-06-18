/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_MTXT.C*/
/* *7    23-OCT-1990 13:57:38 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:33:22 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 21:07:33 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4     2-MAR-1990 11:36:46 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *3     8-FEB-1990 16:59:04 GORDON "(SPR 0) Fix the !@#$* OR spacing problem!"*/
/* *2     5-FEB-1990 23:47:48 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:25 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_MTXT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_MTXT.C                               */
/* *6    27-JUN-1990 17:33:22 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 21:07:33 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4     2-MAR-1990 11:36:46 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *3     8-FEB-1990 16:59:04 GORDON "(SPR 0) Fix the !@#$* OR spacing problem!"    */
/* *2     5-FEB-1990 23:47:48 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:25 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_MTXT.C                               */
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
/*NAME:    WI_SET_MENU_TEXT                                                         */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set text in menu item. The text will appear                                   */
/*    the next time the dialog box is opened.                                       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         menu_id = (INT) menu ID                                                  */
/*         item_id = (INT) item ID, 1 -> N.                                         */
/*         item_text   = (CST) text to be put in item.                              */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_set_menu_text (menu_id, item_id, item_text)    /*************************************************************************/

INT menu_id;
INT item_id;
CHAR *item_text;
    {
    PROGNAME ("wi_set_menu_text");
    INT status;

#ifdef USE_X
    Widget itemwidget;
    Arg arglist[1];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (item_id < 1 || item_id > MAX_MN_ITEMS)
        {
        RETURN (WI_TOO_MANY_MNI);
        }
    E_CHECK_ID (menu_id, item_id, WI_MENU);
    E_CHECK_WIDGET (menu_id, item_id);
    
    itemwidget = E_GET_WIDGET (menu_id, item_id);
    
    /* each menu item is push button gadget, simply set its label                   */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    CompoundString = XmStringCreate (item_text, XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[0], XmNlabelString, CompoundString);
    XtSetValues (itemwidget, arglist, 1);
    XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    CompoundString = DwtLatin1String (item_text);
    XtSetArg (arglist[0], DwtNlabel, CompoundString);
    XtSetValues (itemwidget, arglist, 1);
    XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_XFlush (wiv.current_Display);

#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (item_id < 1 || item_id > MAX_MN_ITEMS)
        {
        RETURN (WI_TOO_MANY_MNI);
        }
    E_CHECK_ID (menu_id, item_id, WI_MENU);
    
    SEND_PROMPT (item_text);
    SEND_3_VALUES (item_id, menu_id, MC_SET_MENU_TEXT);
#endif
    RETURN (SUCCESS);
    }
