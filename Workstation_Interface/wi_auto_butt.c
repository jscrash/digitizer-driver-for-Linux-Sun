/* DEC/CMS REPLACEMENT HISTORY, Element WI_AUTO_BUTT.C*/
/* *5    23-OCT-1990 13:53:43 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:26:24 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:17:57 MING "(SPR 6012) mod USE_X"*/
/* *2     5-FEB-1990 23:31:39 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:36:44 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_AUTO_BUTT.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_AUTO_BUTT.C                              */
/* *4    27-JUN-1990 17:26:24 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:17:57 MING "(SPR 6012) mod USE_X"                           */
/* *2     5-FEB-1990 23:31:39 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:36:44 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_AUTO_BUTT.C                              */
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
/*NAME:    WI_SET_DEFAULT_BUTTON                                                    */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*     This function directs the Mac to set an existing BUTTON control to be        */
/*activated whenever the RETURN key is pressed when that dialog is fronted.  To     */
/*note this behavior for the user, the control will be bordered in standard         */
/*'default button' Macintosh style.                                                 */
/*                                                                                  */
/*             cdlgid = (INT) custom dialog ID                                      */
/*           cditemid = (INT) button item ID, 1 -> N.                               */
/*              state = (INT) zero - turn OFF auto activate...                      */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X
#include "ansixt.h"
#endif

#ifndef MC_AUTO_BUTTON

#define MC_AUTO_BUTTON 77    /* move to esi_wi_int.h, later */

#endif

/************************************************************************************/
publicdef INT wi_set_default_button (cdlgid, cditemid, state)   /*************************************************************************/

INT cdlgid, cditemid, state;

    {
    PROGNAME ("wi_set_default_button");
#define IDENT progname

#ifdef USE_X
    INT status;
    Widget cdlgwidget;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    
    E_CHECK_WIDGET (cdlgid, 0);
    cdlgwidget = E_GET_WIDGET (cdlgid, 0);
    /* pop up dialog box widget                                                     */
    
    E_CHECK_WIDGET (cdlgid, cditemid);
    iteminfo = E_WIDGET_INFO (cdlgid, cditemid);
    itemwidget = E_GET_WIDGET (cdlgid, cditemid);
    /* itemwidget - push button                                                     */
    
    switch (iteminfo->type_id)
        {
    case BTNITEM:
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XtSetArg (arglist[0], XmNshowAsDefault, 1);
        XtSetValues (itemwidget, arglist, 1);
        XtSetArg (arglist[0], XmNdefaultButton, itemwidget);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        XtSetArg (arglist[0], DwtNbordHighlight, TRUE);
        XtSetValues (itemwidget, arglist, 1);
        XtSetArg (arglist[0], DwtNdefaultButton, itemwidget);
#endif  /* ---------- END DECW BLOCK ---------- */
        XtSetValues (cdlgwidget, arglist, 1);   /* set default button               */
        break;
    default:
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        break;
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_DIALOGITEM (cditemid);
    SEND_4_VALUES (state, cditemid, cdlgid, MC_AUTO_BUTTON);
#endif
    RETURN (SUCCESS);
    }
