/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DRNG.C*/
/* *6    23-OCT-1990 13:57:07 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *5    27-JUN-1990 17:32:19 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *4    12-APR-1990 21:04:50 JULIAN "(SPR -1) changed ansiDwt to ansidwt"*/
/* *3     2-MAR-1990 11:35:02 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:46:11 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:39:12 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DRNG.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DRNG.C                               */
/* *5    27-JUN-1990 17:32:19 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *4    12-APR-1990 21:04:50 JULIAN "(SPR -1) changed ansiDwt to ansidwt"          */
/* *3     2-MAR-1990 11:35:02 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:46:11 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:39:12 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_SET_DRNG.C                               */
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
/*NAME:    WI_SET_DIALOG_RANGE                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Set the range for a custom dialog item.                                       */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         cdlgid = (INT) custom dialog ID                                          */
/*         cditemid = (INT) item ID, 1 -> N.                                        */
/*         cdvmin   = (INT) minimum value.                                          */
/*         cdvmax   = (INT) maximum value.                                          */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"

#ifdef USE_X
#ifndef ESI_AM_H
#include "esi_am.h"
#endif
#ifndef ESI_X_H
#include "esi_x.h"
#endif
#include "ansixt.h"
#endif

/************************************************************************************/
publicdef INT wi_set_dialog_range (cdlgid, cditemid, cdvmin, cdvmax)    /*************************************************************************/

INT cdlgid;
INT cditemid;
INT cdvmin;
INT cdvmax;
    {
    INT status;
    PROGNAME ("wi_set_dialog_range");
#define IDENT progname

#ifdef USE_X
    Dimension cols;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[2];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    E_CHECK_WIDGET (cdlgid, cditemid);
    
    /* get the widget of this item                                                  */
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    
    switch (iteminfo->type_id)
        {
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
    case VALUATOR:
        XtSetArg (arglist[0], XmNminimum, cdvmin);
        XtSetArg (arglist[1], XmNmaximum, cdvmax);
        XtSetValues (itemwidget, arglist, 2);
        break;
    case EDITTEXT:
        XtSetArg (arglist[0], XmNcolumns, &cols);
        XtGetValues (itemwidget, arglist, 1);
        XmTextSetMaxLength (itemwidget, (INT)cols * (cdvmax - cdvmin + 1));
        break;
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
    case VALUATOR:
        XtSetArg (arglist[0], DwtNminValue, cdvmin);
        XtSetArg (arglist[1], DwtNmaxValue, cdvmax);
        XtSetValues (itemwidget, arglist, 2);
        break;
    case EDITTEXT:
        XtSetArg (arglist[0], DwtNcols, &cols);
        XtGetValues (itemwidget, arglist, 1);
        DwtSTextSetMaxLength (itemwidget, (INT)cols * (cdvmax - cdvmin + 1));
        break;
#endif  /* ---------- END DECW BLOCK ---------- */
    case VSCROLL:
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else        /* ESITERM code */
    /* Send value.                                                                  */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    E_CHECK_ID (cdlgid, cditemid, WI_DIALOG);
    
    SEND_5_VALUES (cdvmin, cdvmax, cditemid, cdlgid, MC_SET_CD_RANGE);
#endif
    RETURN (SUCCESS);
    }
