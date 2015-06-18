/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTXT.C*/
/* *7    23-OCT-1990 13:57:27 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *6    27-JUN-1990 17:32:35 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *5    12-APR-1990 21:06:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *4    15-MAR-1990 13:36:48 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "*/
/* *3     2-MAR-1990 11:36:02 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:46:41 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:17 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTXT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTXT.C                               */
/* *6    27-JUN-1990 17:32:35 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *5    12-APR-1990 21:06:11 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *4    15-MAR-1990 13:36:48 KEE "(SPR -1) Change idverify->text stucture to be dynamic alloc "    */
/* *3     2-MAR-1990 11:36:02 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:46:41 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:17 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DTXT.C                               */
/******************************************************************************     */
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
/*NAME:    WI_SET_DIALOG_TEXT                                                       */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set text in custom dialog item. The text will appear                          */
/*    the next time the dialog box is opened.                                       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cditemid = (INT) item ID, 1 -> N.                                        */
/*         cdtext   = (CST) text to be put in item.                                 */
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
publicdef INT wi_set_dialog_text (cdlgid, cditemid, cdtext) /*************************************************************************/

INT cdlgid;
INT cditemid;
CHAR *cdtext;
    {
    PROGNAME ("wi_set_dialog_text");
#define IDENT progname
    INT status;

#ifdef USE_X
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    XmString CompoundString;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    DwtCompString CompoundString;
#endif  /* ---------- END DECW BLOCK ---------- */
    
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
    case BTNITEM:
    case CHKITEM:
    case RADIOITEM:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        CompoundString = XmStringCreate (cdtext, XmSTRING_DEFAULT_CHARSET);
        XtSetArg (arglist[0], XmNlabelString, CompoundString);
        XtSetValues (itemwidget, arglist, 1);
        XmStringFree (CompoundString);
        break;        
    case STATTEXT:
        XmTextSetString (itemwidget, cdtext);
        break;        
    case EDITTEXT:
        XmTextSetString (itemwidget, cdtext);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        CompoundString = DwtLatin1String (cdtext);
        XtSetArg (arglist[0], DwtNlabel, CompoundString);
        XtSetValues (itemwidget, arglist, 1);
        XtFree (CompoundString);
        break;        
    case STATTEXT:
        DwtSTextSetString (itemwidget, cdtext);
        break;
    case EDITTEXT:
        DwtSTextSetString (itemwidget, cdtext);
#endif  /* ---------- END DECW BLOCK ---------- */
        if (IS_EMPTY_STRING (iteminfo->text) == FALSE)
            tc_free (iteminfo->text);
        iteminfo->text = STRING_ALLOCATE (cdtext);
        strcpy (iteminfo->text, cdtext);
        /* store the text for wi_query_dialog_change checking                       */
        break;        
    case SELECTOR:
        wiz_set_selector_text (cdlgid, cditemid, cdtext);
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
    
    SEND_PROMPT (cdtext);
    SEND_3_VALUES (cditemid, cdlgid, MC_SET_CD_TEXT);
#endif
    RETURN (SUCCESS);
    }
