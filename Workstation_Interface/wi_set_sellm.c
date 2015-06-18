/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SELLM.C*/
/* *6    23-OCT-1990 13:57:43 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:33:31 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:08:13 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:07:26 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"*/
/* *2     5-FEB-1990 23:48:42 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:35 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SELLM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SELLM.C                              */
/* *5    27-JUN-1990 17:33:31 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:08:13 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:07:26 JULIAN "(SPR 6012) change if USE_X to ifdef USE_X (Kee)"  */
/* *2     5-FEB-1990 23:48:42 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:35 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_SELLM.C                              */
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
/*NAME:    WI_SET_SELECTION_LIMIT                                                   */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set the maximum number of selectable items for a selector.                    */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cditemid = (INT) item id, 1 -> N.                                        */
/*         max_selectable = (INT) maximum number selectable by user.                */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

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
publicdef INT wi_set_selection_limit (cdlgid, cditemid, max_selectable) /*************************************************************************/

INT cdlgid;
INT cditemid;
INT max_selectable;
    {
    PROGNAME ("wi_set_selection_limit");
#define IDENT progname
    INT status;

#ifdef USE_X
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    INT *temp_sel_list;
    INT i;
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XtCallbackRec callback_arg[2];  /* call back arguments                          */
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    Boolean single_sel;
    DwtCallback callback_arg[2];    /* call back arguments                          */
#endif  /* ---------- END DECW BLOCK ---------- */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    /* get the item widget and update its max_sel info.                             */
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    switch (iteminfo->type_id)
        {
    case SELECTOR:
        if (iteminfo->item_type.selector.max_sel != max_selectable)
            {
            temp_sel_list = TI_ALLOCATE (max_selectable);
            for (i = 0; i < MIN (max_selectable, iteminfo->item_type.selector.max_sel);
                 i++)
                {
                temp_sel_list[i] = iteminfo->item_type.selector.sel_list[i];
                }
            if (iteminfo->item_type.selector.max_sel != 0)
                {
                tc_free (iteminfo->item_type.selector.sel_list);
                }
            iteminfo->item_type.selector.sel_list = temp_sel_list;
            iteminfo->item_type.selector.max_sel = max_selectable;
            }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        if (max_selectable == 1)
            {
            XtSetArg (arglist[0], XmNselectionPolicy, XmSINGLE_SELECT);
            XtSetValues (itemwidget, arglist, 1);
            
            /* Remove multiple selection callback                                   */
            callback_arg[0].callback = (XtCallbackProc)NULL;
            callback_arg[0].closure = (caddr_t)0;
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[0], XmNmultipleSelectionCallback, callback_arg);
            XtSetValues (itemwidget, arglist, 1);
            
            /* change to single selection callback                                  */
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((cdlgid << 16) | ((cditemid& 0xFF) <<
                                                                  8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[0], XmNsingleSelectionCallback, callback_arg);
            XtSetValues (itemwidget, arglist, 1);
            }
        else if (max_selectable > 1)
            {
            XtSetArg (arglist[0], XmNselectionPolicy, XmMULTIPLE_SELECT);
            XtSetValues (itemwidget, arglist, 1);
            
            /* Remove single selection callback                                     */
            callback_arg[0].callback = (XtCallbackProc)NULL;
            callback_arg[0].closure = (caddr_t)0;
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[0], XmNsingleSelectionCallback, callback_arg);
            XtSetValues (itemwidget, arglist, 1);
            
            /* change to multiple selection callback                                */
            callback_arg[0].callback = (XtCallbackProc)wi_callback;
            callback_arg[0].closure = (caddr_t)((cdlgid << 16) | ((cditemid& 0xFF) <<
                                                                  8) | (0 & 0xFF));
            callback_arg[1].callback = (XtCallbackProc)NULL;
            XtSetArg (arglist[0], XmNmultipleSelectionCallback, callback_arg);
            XtSetValues (itemwidget, arglist, 1);
            }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        /* get the single selection flag                                            */
        XtSetArg (arglist[0], DwtNsingleSelection, &single_sel);
        XtGetValues (itemwidget, arglist, 1);
        
        /* if max_selectable=1 && the widget is not in single selection mode        */
        if (max_selectable == 1 && single_sel == FALSE)
            {
            XtSetArg (arglist[0], DwtNsingleSelection, TRUE);
            XtSetValues (itemwidget, arglist, 1);
            if (XtHasCallbacks (itemwidget, DwtNextendCallback) == XtCallbackHasSome)
                {
                callback_arg[0].proc = (VoidProc)NULL;
                callback_arg[0].tag = 0;
                callback_arg[1].proc = (VoidProc)NULL;
                XtSetArg (arglist[0], DwtNextendCallback, callback_arg);
                XtSetValues (itemwidget, arglist, 1);
                /* Remove extend selection callback                                 */
                }
            }
        else if (max_selectable > 1 && single_sel == TRUE)
            {                       /* extend selection and the widget is in single selection mode  */
            XtSetArg (arglist[0], DwtNsingleSelection, FALSE);
            XtSetValues (itemwidget, arglist, 1);
            if (XtHasCallbacks (itemwidget, DwtNextendCallback) != XtCallbackHasSome)
                {
                callback_arg[0].proc = (VoidProc)wi_callback;
                callback_arg[0].tag = (cdlgid << 16) | ((cditemid& 0xFF) << 8) |
                    (0 & 0xFF);
                callback_arg[1].proc = (VoidProc)NULL;
                XtSetArg (arglist[0], DwtNextendCallback, callback_arg);
                XtSetValues (itemwidget, arglist, 1);
                /* add extend selection callback                                    */
                }
            }
#endif  /* ---------- END DECW BLOCK ---------- */
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    SEND_4_VALUES (max_selectable, cditemid, cdlgid, MC_SET_MAXSELECT);
    RETURN (SUCCESS);
#endif
    }
