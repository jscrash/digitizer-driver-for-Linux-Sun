/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEL_CDLN.C*/
/* *8    23-OCT-1990 13:55:11 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *7     2-JUL-1990 15:15:03 KEE "(SPR -1) put back old generation (5)"*/
/* *6     2-JUL-1990 13:47:58 KEE "(SPR -1) Add wi_set_selection_limit "*/
/* *5    27-JUN-1990 17:28:09 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:00:20 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:17:43 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"*/
/* *2     5-FEB-1990 23:34:34 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:13 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEL_CDLN.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEL_CDLN.C                               */
/* *7     2-JUL-1990 15:15:03 KEE "(SPR -1) put back old generation (5)"            */
/* *6     2-JUL-1990 13:47:58 KEE "(SPR -1) Add wi_set_selection_limit "            */
/* *5    27-JUN-1990 17:28:09 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:00:20 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:17:43 GILLESPIE "(SPR 6012) Change USE_X define logic to always be #ifdef USE_X"    */
/* *2     5-FEB-1990 23:34:34 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:13 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_DEL_CDLN.C                               */
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
/*NAME:    WI_DELETE_CD_LINE                                                        */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Delete a line in a selector                                                   */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions required here */

#ifdef USE_X
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_delete_cd_line (cdlg_id, item_id, line_num)    /*************************************************************************/

INT cdlg_id;                        /* %P ID of dialog                              */
INT item_id;                        /* %P ID of individual control item.            */
INT line_num;                       /* %P Line number to delete (1->N).             */
    {
    PROGNAME ("wi_delete_cd_line");
    INT status;

#ifdef USE_X
    Dimension selector_width;
    Dimension selector_height;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (item_id);
    E_CHECK_ID (cdlg_id, item_id, WI_DIALOG);
    E_CHECK_WIDGET (cdlg_id, item_id);
    
    iteminfo = E_WIDGET_INFO (cdlg_id, item_id);
    itemwidget = E_GET_WIDGET (cdlg_id, item_id);
    
    switch (iteminfo->type_id)
        {
    case SELECTOR:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */ 
        XtSetArg (arglist[0], XmNwidth, &selector_width);
        XtSetArg (arglist[1], XmNheight, &selector_height);
        XtGetValues (itemwidget, arglist, 2);
        XmListDeletePos (itemwidget, line_num); /* delete item                      */
        wi_set_default_sel (cdlg_id, item_id, 0);
        wiz_set_scrollbar (cdlg_id, item_id);
        /* following is to prevent selector resize                                  */
        XtSetArg (arglist[0], XmNwidth, selector_width);
        XtSetArg (arglist[1], XmNheight, selector_height);
        XtSetValues (itemwidget, arglist, 2);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        DwtListBoxDeletePos (itemwidget, line_num); /* delete item                  */
        wi_set_default_sel (cdlg_id, item_id, 0);
        wiz_set_scrollbar (cdlg_id, item_id);
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
    E_CHECK_DIALOGITEM (item_id);
    E_CHECK_ID (cdlg_id, item_id, WI_DIALOG);
    
    SEND_4_VALUES (line_num, item_id, cdlg_id, MC_DELETE_LINE);
#endif
    return(SUCCESS);
    }
