/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEFLT.C*/
/* *6    23-OCT-1990 13:57:00 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:32:09 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:04:09 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 12:15:38 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"*/
/* *2     5-FEB-1990 23:45:55 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:09 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEFLT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEFLT.C                              */
/* *5    27-JUN-1990 17:32:09 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:04:09 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 12:15:38 JULIAN "(SPR 6012) fix if USE_X to be ifdef USE_X"    */
/* *2     5-FEB-1990 23:45:55 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:09 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DEFLT.C                              */
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
/*NAME:    WI_SET_DEFAULT_SEL                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*      Set the default selection in a selector field.                              */
/*      The default will appear highlighted on the terminal.                        */
/*      This will clear any existing selections.                                    */
/*                                                                                  */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifndef ESI_TC_H
#include "esi_tc.h"
#endif

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"

#endif

/************************************************************************************/
publicdef INT wi_set_default_sel (cdlgid, cditemid, default_sel)    /*************************************************************************/

INT cdlgid;                         /* %P IN: Custom dialog ID                      */
INT cditemid;                       /* %P IN: Item id, 1 -> N.                      */
INT default_sel;                    /* %P IN: The default selection,  1->N.         */
    {
    PROGNAME ("wi_set_default_sel");
#define IDENT progname
    INT status;

#ifdef USE_X
    INT l;
    Widget itemwidget;
    Widget selectorscrollbar;
    WIDGET_ITEM_INFO *iteminfo;
    INT pos, shown, inc, page_inc;
    Arg arglist[1];
    
    /********************************************************************************/
    
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
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        if (default_sel == 0)
            {
            XtSetArg (arglist[0], XmNselectedItemCount, &l);
            XtGetValues (itemwidget, arglist, 1);
            if (l > 0)              /* if selected item > 0, deselect all of them   */
                {
                XmListDeselectAllItems (itemwidget);
                }
            iteminfo->item_type.selector.num_sel = 0;
            }
        else
            {
            for (l = 0; l < iteminfo->item_type.selector.num_sel; l++)
                {
                if (iteminfo->item_type.selector.sel_list[l] == default_sel)
                    {
                    return(SUCCESS);
                    }
                }
            XtSetArg (arglist[0], XmNverticalScrollBar, &selectorscrollbar);
            XtGetValues (itemwidget, arglist, 1);
            /* get original scroll bar position                                     */
            XmScrollBarGetValues (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            XmListSelectPos (itemwidget, default_sel, FALSE);
            iteminfo->item_type.selector.sel_list[l] = default_sel;
            iteminfo->item_type.selector.num_sel++;
            /* restore original scroll bar position                                 */
            XtSetArg (arglist[0], XmNvalue, pos);
            XtSetValues (selectorscrollbar, arglist, 1);
            XmScrollBarSetValues (selectorscrollbar, pos, shown, inc, page_inc, FALSE);
            }
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        if (default_sel == 0)
            {
            XtSetArg (arglist[0], DwtNselectedItemsCount, &l);
            XtGetValues (itemwidget, arglist, 1);
            if (l > 0)              /* if selected item > 0, deselect all of them   */
                {
                DwtListBoxDeselectAllItems (itemwidget);
                }
            iteminfo->item_type.selector.num_sel = 0;
            }
        else
            {
            for (l = 0; l < iteminfo->item_type.selector.num_sel; l++)
                {
                if (iteminfo->item_type.selector.sel_list[l] == default_sel)
                    {
                    return(SUCCESS);
                    }
                }
            XtSetArg (arglist[0], DwtNverticalScrollBar, &selectorscrollbar);
            XtGetValues (itemwidget, arglist, 1);
            /* get original scroll bar position                                     */
            DwtScrollBarGetSlider (selectorscrollbar, &pos, &shown, &inc, &page_inc);
            DwtListBoxSelectPos (itemwidget, default_sel, FALSE);
            iteminfo->item_type.selector.sel_list[l] = default_sel;
            iteminfo->item_type.selector.num_sel++;
            /* restore original scroll bar position                                 */
            XtSetArg (arglist[0], DwtNvalue, pos);
            XtSetValues (selectorscrollbar, arglist, 1);
            DwtScrollBarSetSlider (selectorscrollbar, pos, shown, inc, page_inc,
                                   FALSE);
            }
#endif  /* ---------- END DECW BLOCK ---------- */
        wiz_XFlush (wiv.current_Display);
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
    /* Send maximum selections.                                                     */
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    SEND_4_VALUES (default_sel, cditemid, cdlgid, MC_SET_DEFAULT_SEL);
#endif
    RETURN (SUCCESS);
    }
