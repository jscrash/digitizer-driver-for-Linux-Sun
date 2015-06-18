/* DEC/CMS REPLACEMENT HISTORY, Element XF_GET_CHOICE.C */
/* *10   14-DEC-1990 16:22:22 SANTIAGO "(SPR -1) Bug fix for popup menu in GKS widget on DECwindows" */
/* *9    30-OCT-1990 11:34:13 KEE "(SPR -1) Fix compile noise at sun" */
/* *8    23-OCT-1990 14:01:20 KEE "(SPR -1) Motif and DecWindow Merge" */
/* *7    17-AUG-1990 22:32:20 VINCE "(SPR 5644) Code Cleanup" */
/* *6    20-MAY-1990 16:45:44 SCD "(SPR 1) Move remapping of button to caller of xf_get_choice" */
/* *5    12-APR-1990 21:10:06 JULIAN "(SPR -1) changed ansiDwt to ansidwt" */
/* *4    26-FEB-1990 19:01:41 JULIAN "(SPR 6010) Port to Decstation 3100 (ULTRIX)" */
/* *3     6-FEB-1990 00:10:49 JULIAN "(SPR -1) X Version (Kee)" */
/* *2    11-DEC-1989 14:23:24 JULIAN "(SPR 1) initial checkin of X menu input routine" */
/* *1    11-DEC-1989 14:20:31 JULIAN "X Windows Popup Choice Menus." */
/* DEC/CMS REPLACEMENT HISTORY, Element XF_GET_CHOICE.C */
/************************************************************************************/
/******************************************************************************     */
/*                                                                                  */
/*                Copyright Finder Graphics Systems, Inc. 1990                      */
/*         Unpublished -- All rights reserved                                       */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*          Finder Graphics Systems, Inc.                                           */
/*          201 Tamal Vista Blvd                                                    */
/*          Corte Madera, CA  USA 94925                                             */
/*          (415) 927-0100                                                          */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */

#ifdef USE_X

#ifndef ESI_WI_INT_X
#include "esi_wi_int.x"
#endif

#ifndef ESI_X_H
#include "esi_x.h"
#endif

#include "ansixt.h"
static VOID choice_callback2 ();
INT xf_gbl_xbtn2;

/* File Description ---------------------------------------------------------       */
/*Overview:                                                                         */
/*                                                                                  */
/*Prototypes and one line descriptions of each function in the file.                */
/*Public_Functions:                                                                 */
/*    publicdef INT xf_get_choice(INT x,INT   y, CHAR **local_menu,INT        *b ); */
/*                                                                                  */
/*Private_Functions:                                                                */
/*    static void choice_callback2(Widget widget,Opaque   tag,DwtAnyCallbackStruct *callback_data); */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*                                                                                  */
/*Prototype:                                                                        */
/*    publicdef INT xf_get_choice(INT x,INT   y, CHAR **local_menu,INT        *b ); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    x               -(INT) -\                                                     */
/*    y               -(INT) Position relative to parent widget                     */
/*                        on which to center the menu.  These values                */
/*                        may be taken from root_x & root_y members                 */
/*                        of the Event structure associated with the                */
/*                        event that causes a 'choose one' action to                */
/*                        make a centered popup style menu.                         */
/*    local_menu      -(CHAR **) A TCP of null terminated menu entry strings.       */
/*                        The generated menu will be sized to look                  */
/*                        appropriate with respect to the longest string            */
/*                        passed in this list, so it is generally a good            */
/*                        idea to keep your strings somewhat similar in             */
/*                        length.                                                   */
/*    b               -(INT *) The returned menu pick code. The value depends       */
/*                        on which button was pressed, the first (top most)         */
/*                        menu entry will return a one (1).  There is no            */
/*                        defined limit to the number of entries that may be        */
/*                        in a menu, however a practical limit is around            */
/*                        64.  After this, readability an issue.                    */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    SUCCESS - Successful completion.                                              */
/*    FAIL - Unsuccessful completion.                                               */
/*                                                                                  */
/*Scope:                                                                            */
/*    PUBLIC                                                                        */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#if USE_PROTOTYPES
publicdef INT xf_get_choice (INT x, INT y, CHAR **local_menu, INT *b)
#else
publicdef INT xf_get_choice (x, y, local_menu, b)
INT x, y;
CHAR **local_menu;
INT *b;
#endif
    {
    Arg arglist[MAX_ARG];
    INT nstr, i, n, status = SUCCESS;
    Widget widgets[64];
    Widget popup;
    XEvent event;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtCallbackRec callback_arg[2];
    Widget popup_label;
    Widget popup_seperator;
    XmString CompoundString;
    XtAppContext AppContext = XtWidgetToApplicationContext (wiv.toplevel);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCallback callback_arg[2];
    Dimension width, height;
    INT maxwidth = 0;
    Widget attachdb1;
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
    if (local_menu == (CHAR **)0)
        {
        return FAIL;
        }
    else
        {
        nstr = ts_tcp_len (local_menu);
        if (nstr == 0)
            {
            return FAIL;
            }
        }
    /*                                                                              */
    /*    All buttons are assigned the same callback server.                        */
    /*                                                                              */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    /* (A)                                                                          */
    callback_arg[0].callback = (XtCallbackProc)choice_callback2;
    callback_arg[0].closure = (caddr_t)0;
    callback_arg[1].callback = (XtCallbackProc)NULL;
    
    /* (B)                                                                          */
    n = 0;
    XtSetArg (arglist[n], XmNx, x);
    n++;
    XtSetArg (arglist[n], XmNy, y);
    n++;
    popup = XmCreatePopupMenu (wiv.mainw, "MouseMenu 1", arglist, n);
    
    CompoundString = XmStringCreate ("GKS Option", XmSTRING_DEFAULT_CHARSET);
    XtSetArg (arglist[0], XmNlabelString, CompoundString);
    popup_label = XmCreateLabelGadget (popup, "MouseMenu Title", arglist, 1);
    XmStringFree (CompoundString);
    XtManageChild (popup_label);
    
    popup_seperator = XmCreateSeparatorGadget (popup, "MouseMenu Separator", arglist,
                                               0);
    XtManageChild (popup_seperator);
    
    /* (C)                                                                          */
    for (i = 0; i < nstr; i++)
        {
        /* the integer position of button.                                          */
        callback_arg[0].closure = (caddr_t)i;
        XtSetArg (arglist[0], XmNactivateCallback, callback_arg);
        widgets[i] = XmCreatePushButtonGadget (popup, local_menu[i], arglist, 1);
        XtManageChild (widgets[i]);
        }
    /* (D)                                                                          */
    XtManageChild (popup);
    xf_gbl_xbtn2 = -1;
    
    /* (E)                                                                          */
    /* While waiting on the button callback                                         */
    /* process other X events. (mainloop style )                                    */
    while (xf_gbl_xbtn2 == -1)
        {
        XtAppNextEvent (AppContext, &event);
        if (event.type == ButtonRelease)
            {
            if (event.xbutton.button == Button1)
                {
                /* Use <Btn3Up>: behavior                                           */
                event.xbutton.button = Button3;
                }
            else if (event.xbutton.button == Button3)
                {
                /* Use anything but <Btn3Up>: behavior                              */
                event.xbutton.button = Button1;
                }
            if (XtWindowToWidget (event.xbutton.display,
                                  event.xbutton.window) != popup)
                {
                XtDispatchEvent (&event);
                XtUnmanageChild (popup);
                XtManageChild (popup);
                }
            else
                {
                XtDispatchEvent (&event);
                }
            }
        else
            {
            XtDispatchEvent (&event);
            }
        }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    
    /* (A)                                                                          */
    callback_arg[0].proc = (VoidProc)choice_callback2;
    callback_arg[0].tag = 0;
    callback_arg[1].proc = (VoidProc)NULL;
    
    /* (B)                                                                          */
    n = 0;
    XtSetArg (arglist[n], DwtNtakeFocus, TRUE);
    n++;
    XtSetArg (arglist[n], DwtNstyle, DwtModal);
    n++;
    XtSetArg (arglist[n], DwtNrubberPositioning, TRUE);
    n++;
    popup = DwtAttachedDBPopupCreate (wiv.mainw, "MouseMenu 1", arglist, n);
    
    /* (C)                                                                          */
    for (i = 0; i < nstr; i++)
        {
        callback_arg[0].tag = i;
        n = 0;
        XtSetArg (arglist[n], DwtNshadow, FALSE);
        n++;
        CompoundString = DwtLatin1String (local_menu[i]);
        XtSetArg (arglist[n], DwtNlabel, CompoundString);
        n++;
        XtSetArg (arglist[n], DwtNadbLeftAttachment, DwtAttachAdb);
        n++;
        XtSetArg (arglist[n], DwtNadbRightAttachment, DwtAttachAdb);
        n++;
        XtSetArg (arglist[n], DwtNactivateCallback, callback_arg);
        n++;
        
        if (i == 0)
            {
            XtSetArg (arglist[n], DwtNadbTopAttachment, DwtAttachAdb);
            n++;
            widgets[i] = DwtPushButtonCreate (popup, local_menu[i], arglist, n);
            XtManageChild (widgets[i]);
            XtFree (CompoundString);
            attachdb1 = widgets[i];
            
            /* Get the height of this first button, use to size the rest.           */
            XtSetArg (arglist[0], DwtNheight, &height);
            XtGetValues (widgets[i], arglist, 1);
            }
        else
            {
            XtSetArg (arglist[n], DwtNadbTopAttachment, DwtAttachWidget);
            n++;
            XtSetArg (arglist[n], DwtNadbTopOffset, 0);
            n++;
            XtSetArg (arglist[n], DwtNadbTopWidget, attachdb1);
            n++;
            widgets[i] = DwtPushButtonCreate (popup, local_menu[i], arglist, n);
            XtManageChild (widgets[i]);
            XtFree (CompoundString);
            attachdb1 = widgets[i];
            }
        /* Get maxwidth for later sizing.                                           */
        XtSetArg (arglist[0], DwtNwidth, &width);
        XtGetValues (widgets[i], arglist, 1);
        maxwidth = MAX (width, maxwidth);
        }
    height *= nstr;
    n = 0;
    XtSetArg (arglist[n], DwtNheight, height);
    n++;
    XtSetArg (arglist[n], DwtNwidth, maxwidth);
    n++;
    /*                                                                              */
    /*  If the caller has no X,Y preference, put it in the middle                   */
    /*                                                                              */
    if (x + y == 0)
        {
        x = 400;
        y = 400;
        }
    else
        {
        x -= (maxwidth / 2);
        y -= (height / 2);
        }
    XtSetArg (arglist[n], DwtNx, x);
    n++;
    XtSetArg (arglist[n], DwtNy, y);
    n++;
    XtSetValues (popup, arglist, n);
    
    /* (D)                                                                          */
    XtManageChild (popup);
    xf_gbl_xbtn2 = -1;
    
    /* (E)                                                                          */
    /* While waiting on the button callback                                         */
    /* process other X events. (mainloop style )                                    */
    while (xf_gbl_xbtn2 == -1)
        {
        XNextEvent (wiv.current_Display, &event);
        if (event.type == ButtonRelease)
            {
            if (XtWindowToWidget (event.xbutton.display,
                                  event.xbutton.window) != popup)
                {
                XtDispatchEvent (&event);
                XtUnmanageChild (popup);
                XtManageChild (popup);
                }
            else
                {
                XtDispatchEvent (&event);
                }
            }
        else
            {
            XtDispatchEvent (&event);
            }
        }
#endif  /* ---------- END DECW BLOCK ---------- */
    
    /* ------------------------------------------                                   */
    /* Return the selected 'button' which was set                                   */
    /* by the choice_callback2 event handler                                        */
    /* ------------------------------------------                                   */
    *b = xf_gbl_xbtn2;
    
    /* Take down the popup menu.                                                    */
    XtUnmanageChild (popup);
    
    /* destroy all the pushbutton widgets (release allocated memory)                */
    for (i = 0; i < nstr; i++)
        {
        XtDestroyWidget (widgets[i]);
        }
    XtDestroyWidget (popup);
    return SUCCESS;
    }
/* END:                                                                             */

/* Function Description -----------------------------------------------------       */
/*Description:                                                                      */
/*    This is the callback associated with the pushbuttons on the                   */
/*    pop-up choice menu.                                                           */
/*                                                                                  */
/*Prototype:                                                                        */
/*    static void choice_callback2(Widget widget,Opaque   tag,DwtAnyCallbackStruct *callback_data); */
/*                                                                                  */
/*Parameters:     (indicate (I)/(O)/(M) for input/output/input-and-modified)        */
/*    widget          -(Widget)                                                     */
/*    tag             -(Opaque)                                                     */
/*    callback_data   -(DwtAnyCallbackStruct *)                                     */
/*                                                                                  */
/*Return Value/Status:                                                              */
/*    None.                                                                         */
/*                                                                                  */
/*Scope:                                                                            */
/*    PRIVATE to <xf_get_choice.c>                                                  */
/*                                                                                  */
/*Limitations/Assumptions:                                                          */
/*                                                                                  */
/*-----------------------------------------------------------------------------     */

/* BEGIN:                                                                           */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
#if USE_PROTOTYPES
static  void choice_callback2 (Widget widget, Opaque tag,
                                   XmAnyCallbackStruct *callback_data)
#else
static  void choice_callback2 (widget, tag, callback_data)
Widget widget;
Opaque tag;
XmAnyCallbackStruct *callback_data;
#endif
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
#if USE_PROTOTYPES
static  void choice_callback2 (Widget widget, Opaque tag,
                                   DwtAnyCallbackStruct *callback_data)
#else
static  void choice_callback2 (widget, tag, callback_data)
Widget widget;
Opaque tag;
DwtAnyCallbackStruct *callback_data;
#endif
#endif  /* ---------- END DECW BLOCK ---------- */
    {
    xf_gbl_xbtn2 = (INT)tag;
    }
/* END:                                                                             */

#endif
