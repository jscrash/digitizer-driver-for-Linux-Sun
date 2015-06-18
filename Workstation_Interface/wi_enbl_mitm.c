/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MITM.C*/
/* *5    23-OCT-1990 13:55:24 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:28:53 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:30:05 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:36:40 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:34 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MITM.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MITM.C                              */
/* *4    27-JUN-1990 17:28:53 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:30:05 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:36:40 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:34 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MITM.C                              */
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
/*NAME:    WI_ENABLE_MENU_ITEM                                                      */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Enable or disable individual menu items.                                      */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions used here */

#ifdef USE_X
#include "ansixt.h"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_enable_menu_item (menuid, itemid, onoroff) /*************************************************************************/

INT menuid;
INT itemid;
INT onoroff;

    {
    PROGNAME ("wi_enable_menu_item");
    INT status;

#ifdef USE_X
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (itemid > 16)
        {
        RETURN (WI_BAD_COUNT);
        }
    E_CHECK_ID (menuid, itemid, WI_MENU);
    E_CHECK_WIDGET (menuid, itemid);
    
    /* get widget and set sensitive attribute                                       */
    iteminfo = E_WIDGET_INFO (menuid, itemid);
    itemwidget = E_GET_WIDGET (menuid, itemid);
    
    if (iteminfo->type_id == MITEM)
        {
        if (iteminfo->item_type.mitem.check_onoff == ON)
            {
            itemwidget = iteminfo->item_type.mitem.menu_toggle_button;
            }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
        XtSetArg (arglist[0], XmNsensitive, (Boolean)onoroff);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
        XtSetArg (arglist[0], DwtNsensitive, (Boolean)onoroff);
#endif  /* ---------- END DECW BLOCK ---------- */
        XtSetValues (itemwidget, arglist, 1);
        }
    else if (iteminfo->type_id != MSEPERATOR)
        {
        E_WIZ_ERROR ("Invalid dialog item type", progname);
        }
#else
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    if (itemid > 16)
        {
        RETURN (WI_BAD_COUNT);
        }
    E_CHECK_ID (menuid, itemid, WI_MENU);
    SEND_4_VALUES (onoroff, itemid, menuid, MC_SELECT_MI);
#endif
    RETURN (SUCCESS);
    }
