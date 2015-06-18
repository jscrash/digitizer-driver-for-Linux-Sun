/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DVAL.C*/
/* *9    18-JUN-1991 13:03:11 KEE "(SPR -1) Change Menu creation method from inconvenience way to convenience way with Motif 1.1*/
/*version"*/
/* *8    17-JUN-1991 09:12:51 KEE "(SPR -1) Add (Widget)0 checking for option menu"*/
/* *7    13-JUN-1991 14:48:15 KEE "(SPR -1) Change PullDnMenu to OptionMenu and use non-conveient option menu creation method"*/
/* *6    23-OCT-1990 13:57:32 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:33:05 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:06:52 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:36:26 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:47:22 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:20 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DVAL.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DVAL.C                               */
/* *5    27-JUN-1990 17:33:05 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:06:52 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:36:26 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:47:22 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:20 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DVAL.C                               */
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
/*NAME:    WI_SET_DIALOG_VALUE                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set value in custom dialog item. The value will appear                        */
/*    the next time the dialog box is opened.                                       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cditemid = (INT) item ID, 1 -> N.                                        */
/*         cdvalue  = (INT) value for item, usually 0 or 1.                         */
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
publicdef INT wi_set_dialog_value (cdlgid, cditemid, cdvalue)   /*************************************************************************/

INT cdlgid;
INT cditemid;
INT cdvalue;
    {
    PROGNAME ("wi_set_dialog_value");
#define IDENT progname
    INT status;

#ifdef USE_X
    INT rmin, rmax;
    Widget itemwidget, selectorscrollbar;
    Widget menu_button;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];
    INT pos, shown, inc, page_inc;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */

    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    /* get the item widget and set the value                                        */
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);

#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    if (itemwidget == (Widget)NULL) /* vscroll                                      */
        {                           /* get the selector                             */
        E_CHECK_WIDGET (cdlgid, cditemid - 1);
        iteminfo = E_WIDGET_INFO (cdlgid, cditemid - 1);
        itemwidget = E_GET_WIDGET (cdlgid, cditemid - 1);
        if (iteminfo->type_id == SELECTOR)
            {
            /* get the embedded scroll bar of the selector                          */
            XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
            XtGetValues (itemwidget, arglist, 1);
            XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            XmScrollBarSetValues (selectorscrollbar, cdvalue, shown, inc, page_inc,
                                  FALSE);
            }
        else
            {
            E_WIZ_ERROR ("Invalid dialog item type", progname);
            }
        }
    else
        {
        switch (iteminfo->type_id)
            {
        case VSCROLL:
            XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            XmScrollBarSetValues (selectorscrollbar, cdvalue, shown, inc, page_inc,
                                  TRUE);
            break;
        case RADIOITEM:
            if (cdvalue == ON)
                {
                for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax);
                     rmin <= rmax; rmin++)
                    {
                    E_CHECK_WIDGET (cdlgid, rmin);
                    if ((E_WIDGET_INFO (cdlgid, rmin))->type_id == RADIOITEM)
                        {
                        XmToggleButtonGadgetSetState (E_GET_WIDGET (cdlgid, rmin), OFF,
                                                      FALSE);
                        }
                    }
                }
            XmToggleButtonGadgetSetState (itemwidget, (Boolean)cdvalue, FALSE);
            break;
        case CHKITEM:
            XmToggleButtonSetState (itemwidget, (Boolean)cdvalue, FALSE);
            break;
        case VALUATOR:
            XmScaleSetValue (itemwidget, cdvalue);
            break;
        case OPTIONMENU:
	    if (itemwidget IS_NOT_EQUAL_TO (Widget)NULL)
	      {
		menu_button = iteminfo->item_type.optionmenu.menu_button[cdvalue - 1];
		CompoundString = 
		  XmStringCreate (iteminfo->item_type.optionmenu.tcp[cdvalue - 1], 
				  XmSTRING_DEFAULT_CHARSET);
		XtSetArg (arglist[0], XmNmenuHistory, menu_button);
		XtSetValues (itemwidget, arglist, 1);
		XmStringFree (CompoundString);
		iteminfo->item_type.optionmenu.item_chosen = cdvalue;
	      }
            break;
        default:
            E_WIZ_ERROR ("Invalid dialog item type", progname);
            break;
            }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        if (itemwidget == (Widget)NULL) /* vscroll                                  */
            {                       /* get the selector                             */
            E_CHECK_WIDGET (cdlgid, cditemid - 1);
            iteminfo = E_WIDGET_INFO (cdlgid, cditemid - 1);
            itemwidget = E_GET_WIDGET (cdlgid, cditemid - 1);
            if (iteminfo->type_id == SELECTOR)
                {
                /* get the embedded scroll bar of the selector                      */
                XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
                XtGetValues (itemwidget, arglist, 1);
                DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc,
                                       &page_inc);
                DwtScrollBarSetSlider (selectorscrollbar, cdvalue, shown, inc,
                                       page_inc, FALSE);
                }
            else
                {
                E_WIZ_ERROR ("Invalid dialog item type", progname);
                }
            }
        else
            {
            switch (iteminfo->type_id)
                {
            case VSCROLL:
                DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc,
                                       &page_inc);
                DwtScrollBarSetSlider (selectorscrollbar, cdvalue, shown, inc,
                                       page_inc, TRUE);
                break;
            case RADIOITEM:
                if (cdvalue == ON)
                    {
                    for (wiz_get_group_range (cdlgid, cditemid, &rmin, &rmax);
                         rmin <= rmax; rmin++)
                        {
                        E_CHECK_WIDGET (cdlgid, rmin);
                        if ((E_WIDGET_INFO (cdlgid, rmin))->type_id == RADIOITEM)
                            {
                            XtSetArg (arglist[0], DwtNvalue, OFF);
                            XtSetValues (E_GET_WIDGET (cdlgid, rmin), arglist, 1);
                            }
                        }
                    }
                XtSetArg (arglist[0], DwtNvalue, (Boolean)cdvalue);
                XtSetValues (itemwidget, arglist, 1);
                break;
            case CHKITEM:
                XtSetArg (arglist[0], DwtNvalue, (Boolean)cdvalue);
                XtSetValues (itemwidget, arglist, 1);
                break;
            case VALUATOR:
                DwtScaleSetSlider (itemwidget, cdvalue);
                break;
            case OPTIONMENU:
		if (itemwidget IS_NOT_EQUAL_TO (Widget)NULL)
		  {
		    menu_button = iteminfo->item_type.optionmenu.menu_button[cdvalue - 1];
		    CompoundString = DwtLatin1String (iteminfo->item_type.optionmenu.tcp[cdvalue - 1]);
		    XtSetArg (arglist[0], DwtNmenuHistory, menu_button);
		    XtSetValues (itemwidget, arglist, 1);
		    XtFree (CompoundString);
		    iteminfo->item_type.optionmenu.item_chosen = cdvalue;
		  }
                break;
            default:
                E_WIZ_ERROR ("Invalid dialog item type", progname);
                break;
                }
#endif  /* ---------- END DECW BLOCK ---------- */
            wiz_XFlush (wiv.current_Display);
            }
#else        /* ESITERM code */
        wiz_jp_name (IDENT);
        
        E_CHECK_INIT;
        E_CHECK_DIALOGITEM (cditemid);
        E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
        
        SEND_4_VALUES (cdvalue, cditemid, cdlgid, MC_SET_CD_VALUE);
#endif 
        RETURN (SUCCESS);
        }

