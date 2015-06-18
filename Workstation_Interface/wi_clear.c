/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLEAR.C*/
/* *7    23-OCT-1990 13:53:59 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:26:58 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 20:57:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4    23-MAR-1990 23:24:05 KEE "(SPR -1) Fixing Dim for Selector (Use XtSetSensitive instead)"*/
/* *3     2-MAR-1990 11:33:52 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:33:08 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLEAR.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLEAR.C                                  */
/* *6    27-JUN-1990 17:26:58 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 20:57:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4    23-MAR-1990 23:24:05 KEE "(SPR -1) Fixing Dim for Selector (Use XtSetSensitive instead)"   */
/* *3     2-MAR-1990 11:33:52 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:33:08 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:00 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_CLEAR.C                                  */
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
/*NAME:    WI_CLEAR                                                                 */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Clear a menu or dialog to it's original condition.                            */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions required here */

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
publicdef INT wi_clear (input_id)   /*************************************************************************/

INT input_id;                       /* %P Menu or Dialog ID from WI_RDF.            */
    {
    PROGNAME ("wi_clear");
#define IDENT progname
    INT id;
    INPUT_RECORD inp_rec;

#ifdef USE_X
    INT i, n, minvalue, status;
    DIALOG_MENU *thedialog_menu;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
#endif
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    
    if (input_id > 999)
        {
        /* Retrieve from Tree                                                       */
        id = input_id;
        if (btfind (&wiv.input_tree, &id, &inp_rec))
            {
            strcpy (inp_rec.owner_tag, inp_rec.orig_tag);
            wi_set_dialog_title (input_id, inp_rec.orig_title);
            }
        else
            {
            /* Report not defined                                                   */
            RETURN (WI_NOT_DEFINED);
            }
        }
#ifdef USE_X
    E_CHECK_WIDGET (input_id, 0);
    thedialog_menu = E_DIALOG_MENU (input_id);
    if ((E_WIDGET_INFO (input_id, 0))->type_id == DIALOG)
        {
        for (i = 0; i <= thedialog_menu->num_items; i++)
            {
            E_CHECK_WIDGET (input_id, i);
            iteminfo = E_WIDGET_INFO (input_id, i);
            itemwidget = E_GET_WIDGET (input_id, i);
            switch (iteminfo->type_id)
                {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */     
            case DIALOG:
                XtSetSensitive (itemwidget, TRUE);
                break;
            case BTNITEM:
                n = 0;
                if (iteminfo->item_type.btnitem.deflt_btn == TRUE)
                    {
                    XtSetArg (arglist[n], XmNdefaultButton, itemwidget);
                    n++;
                    XtSetValues (E_GET_WIDGET (input_id, 0), arglist, n);
                    /* parent itemid = 0                                            */
                    n = 0;          /* reset n for pushbutton arglist               */
                    XtSetArg (arglist[n], XmNshowAsDefault, 1);
                    n++;
                    }
                CompoundString = XmStringCreate (iteminfo->text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XmStringFree (CompoundString);
                break;
            case CHKITEM:
                n = 0;
                CompoundString = XmStringCreate (iteminfo->text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XmStringFree (CompoundString);
                XmToggleButtonSetState (itemwidget, FALSE, FALSE);
                break;
            case RADIOITEM:
                n = 0;
                CompoundString = XmStringCreate (iteminfo->text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XmStringFree (CompoundString);
                XmToggleButtonGadgetSetState (itemwidget, FALSE, FALSE);
                break;
            case STATTEXT:
                XmTextSetString (itemwidget, iteminfo->text);
                XtSetSensitive (itemwidget, FALSE);
                break;
            case EDITTEXT:
                XmTextSetString (itemwidget, "");   /* clear edittext field         */
                break;
            case SELECTOR:
                wiz_set_selector_text (input_id, i, "");
                break;
            case VALUATOR:
                n = 0;              /* set valuator to the min value                */
                XtSetArg (arglist[n], XmNminimum, &minvalue);
                n++;
                XtGetValues (itemwidget, arglist, n);
                XmScaleSetValue (itemwidget, minvalue);
                break;
            default:
                break;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            case DIALOG:
                XtSetSensitive (itemwidget, TRUE);
                break;
            case BTNITEM:
                n = 0;
                if (iteminfo->item_type.btnitem.deflt_btn == TRUE)
                    {
                    XtSetArg (arglist[n], DwtNdefaultButton, itemwidget);
                    n++;
                    XtSetValues (E_GET_WIDGET (input_id, 0), arglist, n);
                    /* parent itemid = 0                                            */
                    n = 0;          /* reset n for pushbutton arglist               */
                    XtSetArg (arglist[n], DwtNbordHighlight, TRUE);
                    n++;
                    }
                CompoundString = DwtLatin1String (iteminfo->text);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XtFree (CompoundString);
                break;
            case CHKITEM:
            case RADIOITEM:
                n = 0;
                CompoundString = DwtLatin1String (iteminfo->text);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNvalue, FALSE);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XtFree (CompoundString);
                break;
            case STATTEXT:
                DwtSTextSetString (itemwidget, iteminfo->text);
                XtSetSensitive (itemwidget, FALSE);
                break;
            case EDITTEXT:
                DwtSTextSetString (itemwidget, ""); /* clear edittext field         */
                break;
            case SELECTOR:
                wiz_set_selector_text (input_id, i, "");
                break;
            case VALUATOR:
                n = 0;              /* set valuator to the min value                */
                XtSetArg (arglist[n], DwtNminValue, &minvalue);
                n++;
                XtGetValues (itemwidget, arglist, n);
                DwtScaleSetSlider (itemwidget, minvalue);
                break;
            default:
                break;
#endif  /* ---------- END DECW BLOCK ---------- */
                }
            /* switch                                                               */
            }
        /* for loop - 1st pass                                                      */
        
        for (i = 1; i <= thedialog_menu->num_items; i++)
            {
            E_CHECK_WIDGET (input_id, i);
            if ((E_WIDGET_INFO (input_id, i))->type_id == VSCROLL)
                {
                wiz_set_scrollbar (input_id, i);
                }
            }
        }
    else                            /* menu type                                    */
        {
        for (i = 1; i <= thedialog_menu->num_items; i++)
            {
            E_CHECK_WIDGET (input_id, i);
            iteminfo = E_WIDGET_INFO (input_id, i);
            itemwidget = E_GET_WIDGET (input_id, i);
            if (iteminfo->type_id == MITEM)
                {
                n = 0;              /* restore each menu item original state        */
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */     
                CompoundString = XmStringCreate (iteminfo->text,
                                                 XmSTRING_DEFAULT_CHARSET);
                XtSetArg (arglist[n], XmNlabelString, CompoundString);
                n++;
                XtSetArg (arglist[n], XmNsensitive,
                          (Boolean)iteminfo->item_type.mitem.enable_flag);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XmStringFree (CompoundString);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
                CompoundString = DwtLatin1String (iteminfo->text);
                XtSetArg (arglist[n], DwtNlabel, CompoundString);
                n++;
                XtSetArg (arglist[n], DwtNsensitive,
                          (Boolean)iteminfo->item_type.mitem.enable_flag);
                n++;
                XtSetValues (itemwidget, arglist, n);
                XtFree (CompoundString);
#endif  /* ---------- END DECW BLOCK ---------- */
                wi_check_menu_item (input_id, i, OFF);  /* turn off check mark      */
                }
            }
        /* for loop                                                                 */
        }
    /* else                                                                         */

#else            /* ESITERM Code */
    SEND_2_VALUES (input_id, MC_CLEAR);
#endif
    RETURN (SUCCESS);
    }
