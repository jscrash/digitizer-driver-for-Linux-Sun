/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_MITMS.C*/
/* *5    23-OCT-1990 13:55:30 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:29:10 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:30:37 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:37:06 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:40 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_MITMS.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_MITMS.C                              */
/* *4    27-JUN-1990 17:29:10 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:30:37 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:37:06 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:40 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENB_MITMS.C                              */
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
/*NAME:    WI_ENABLE_MENU_ITEMS                                                     */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Enable or disable individual menu items using array.                          */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         menuid    = (INT) id of menu as allocated by Mac.                        */
/*         numitems  = (INT) number of items to modify.                             */
/*         onoroff   = (INT ARRAY) flags, 0 = disable, 1 = enable.                  */
/*                                                                                  */
/*************************************************************************          */

#include "esi_gl_defs.h"
#include "esi_wi_int.x"        /* external definitions referenced here */
#include "esi_tc.h"

#ifdef USE_X
#include "ansixt.h"
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_enable_menu_items (menuid, numitems, onoroff)  /*************************************************************************/

INT menuid;
INT numitems;
INT onoroff[MAX_MN_ITEMS];

    {
    PROGNAME ("wi_enable_menu_items");
    INT status;

#ifdef USE_X
    INT i;
    Widget itemwidget;
    WIDGET_ITEM_INFO *iteminfo;
    Arg arglist[1];
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_COUNT (numitems);
    E_CHECK_ID (menuid, 0, WI_MENU);
    
    for (i = 0; i < numitems; i++)
        {
        /* get item widget and set sensitive attribute                              */
        E_CHECK_WIDGET (menuid, i + 1);
        iteminfo = E_WIDGET_INFO (menuid, i + 1);
        itemwidget = E_GET_WIDGET (menuid, i + 1);
        
        switch (iteminfo->type_id)
            {
        case MITEM:
            if (iteminfo->item_type.mitem.check_onoff == ON)
                {
                itemwidget = iteminfo->item_type.mitem.menu_toggle_button;
                }
#ifdef MOTIF  /* ---------- BEGIN MOTIF BLOCK ---------- */
            XtSetArg (arglist[0], XmNsensitive, (Boolean)onoroff[i]);
#else  /* ---------- END MOTIF BLOCK - BEGIN DECW BLOCK ---------- */
            XtSetArg (arglist[0], DwtNsensitive, (Boolean)onoroff[i]);
#endif  /* ---------- END DECW BLOCK ---------- */
            XtSetValues (itemwidget, arglist, 1);
            break;
        case MSEPERATOR:
            break;
        default:
            E_WIZ_ERROR ("Invalid dialog item type", progname);
            break;
            }
        }
#else        /* ESITERM code */
    INT packed_flags;
    
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_COUNT (numitems);
    E_CHECK_ID (menuid, 0, WI_MENU);
    
    tc_pack_bits (numitems, onoroff, &packed_flags);    /* pack into HEX word       */
    
    SEND_4_VALUES (packed_flags, numitems, menuid, MC_SELECT_MA);
#endif 
    RETURN (SUCCESS);
    }
