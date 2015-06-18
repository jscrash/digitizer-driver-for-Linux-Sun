/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEF.C*/
/* *6    23-OCT-1990 13:56:52 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:32:00 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:03:30 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 12:15:27 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:45:39 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:07 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEF.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEF.C                                */
/* *5    27-JUN-1990 17:32:00 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:03:30 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 12:15:27 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"    */
/* *2     5-FEB-1990 23:45:39 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:07 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEF.C                                */
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
/*NAME:    WI_SET_DEFAULTS                                                          */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*      Set the state of the items in a selector field.                             */
/*      All selector items are affected...those not specified in the                */
/*      array are set to the opposite state as specified in the call.               */
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
publicdef INT wi_set_defaults (cdlgid, cditemid, numitems, items_array, onoroff)    
/************************************************************************************/

INT cdlgid;                         /* %P IN: Custom dialog ID                      */
INT cditemid;                       /* %P IN: Item id, 1 -> N.                      */
INT numitems;                       /* number of items in the array                 */
INT *items_array;                   /* the array of selector items                  */
INT onoroff;                        /* 0=all on but these  1=all off but these      */
    {
    INT status;
    PROGNAME ("wi_set_defaults");
    INT i;
#define IDENT progname
    /********************************************************************************/

#ifdef USE_X
    INT j, k, l;
    INT pos, shown, inc, page_inc;
    Widget itemwidget, selectorscrollbar;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    CHAR stl[ST_COMMAND_MAX];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    switch (iteminfo->type_id)
        {
    case SELECTOR:
        if (numitems > iteminfo->item_type.selector.max_sel)
            {
            E_WIZ_ERROR ("Number of default selection excess maximum selectable",
                         progname);
            }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XtSetArg (arglist[0], XmNselectedItemCount, &l);
        XtGetValues (itemwidget, arglist, 1);
        if (l > 0)                  /* if selected item > 1, deselect all of them   */
            {
            XmListDeselectAllItems (itemwidget);
            }
        XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
        XtGetValues (itemwidget, arglist, 1);
        XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
        
        /* get the slider original position                                         */
        if (onoroff == ON)
            {
            for (i = 0; i < numitems; i++)
                {
                XmListSelectPos (itemwidget, items_array[i], FALSE);
                iteminfo->item_type.selector.sel_list[i] = items_array[i];
                }
            iteminfo->item_type.selector.num_sel = numitems;    /* select items     */
            }
        else                        /* OFF                                          */
            {                       /* select those items that are not in items_array,  */
                                    /* this implementation is according to the programer manual.    */
                                    /* Anyway, there is no wi_set_defaults call with OFF flag   */
            XtSetArg (arglist[0], XmNitemCount, &k);    /* get number of items      */
            XtGetValues (itemwidget, arglist, 1);
            for (j = 0; j < k - 1; j++) /* scan each item                           */
                {
                for (i = 0; i < numitems; i++)
                    {
                    if (j != items_array[i])
                        {
                        XmListSelectPos (itemwidget, j, FALSE);
                        iteminfo->item_type.selector.sel_list[i] = j;
                        }
                    }
                iteminfo->item_type.selector.num_sel = numitems;    /* select items */
                }
            }
        /* retore the slider to its original position                               */
        XtSetArg (arglist[0], XmNvalue, pos);
        XtSetValues (selectorscrollbar, arglist, 1);
        XmScrollBarSetValues (selectorscrollbar, pos, shown, inc, page_inc, FALSE);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        XtSetArg (arglist[0], DwtNselectedItemsCount, &l);
        XtGetValues (itemwidget, arglist, 1);
        if (l > 0)                  /* if selected item > 1, deselect all of them   */
            {
            DwtListBoxDeselectAllItems (itemwidget);
            }
        XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
        XtGetValues (itemwidget, arglist, 1);
        DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc, &page_inc);
        /* get the slider original position                                         */
        if (onoroff == ON)
            {
            for (i = 0; i < numitems; i++)
                {
                DwtListBoxSelectPos (itemwidget, items_array[i], FALSE);
                iteminfo->item_type.selector.sel_list[i] = items_array[i];
                }
            iteminfo->item_type.selector.num_sel = numitems;    /* select items     */
            }
        else                        /* OFF                                          */
            {                       /* select those items that are not in items_array,  */
                                    /* this implementation is according to the programer manual.    */
                                    /* Anyway, there is no wi_set_defaults call with OFF flag   */
            XtSetArg (arglist[0], DwtNitemsCount, &k);  /* get number of items      */
            XtGetValues (itemwidget, arglist, 1);
            for (j = 0; j < k - 1; j++) /* scan each item                           */
                {
                for (i = 0; i < numitems; i++)
                    {
                    if (j != items_array[i])
                        {
                        DwtListBoxSelectPos (itemwidget, j, FALSE);
                        iteminfo->item_type.selector.sel_list[i] = j;
                        }
                    }
                iteminfo->item_type.selector.num_sel = numitems;    /* select items */
                }
            }
        /* retore the slider to its original position                               */
        XtSetArg (arglist[0], DwtNvalue, pos);
        XtSetValues (selectorscrollbar, arglist, 1);
        DwtScrollBarSetSlider (selectorscrollbar, pos, shown, inc, page_inc, FALSE);
#endif  /* ---------- END DECW BLOCK ---------- */
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
    wiz_XFlush (wiv.current_Display);

#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    /* Send the items, numitems, state, ctlid, dialog id then the opcode            */
    /* send 'smart on' character                                                    */
    sprintf (wiv.stl, "%c ", wiv.macon);
    wiz_puts (wiv.stl);
    
    /* add each argument, followed by a space                                       */
    for (i = 0; i < numitems; i++)
        {
        sprintf (wiv.stl, "%d ", items_array[i]);
        wiz_puts (wiv.stl);
        }
    /* finish with #items, flag, ctlid, dlgid, opcode & macoff                      */
    sprintf (wiv.stl, "%d %d %d %d %d %c", numitems, onoroff, cditemid, cdlgid,
             MC_SET_DEFAULTS, wiv.macoff);
    wiz_puts (wiv.stl);

#endif
    RETURN (SUCCESS);
    }
