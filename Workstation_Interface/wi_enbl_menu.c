/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MENU.C*/
/* *5    23-OCT-1990 13:55:18 KEE "(SPR -1) Motif and DecWindow Merge"*/
/* *4    27-JUN-1990 17:28:35 KEE "(SPR -1) New wi data structure using 2-D array"*/
/* *3     2-MAR-1990 11:29:13 VINCE "(SPR 6012) Changed usage of USE_X symbol"*/
/* *2     5-FEB-1990 23:36:27 JULIAN "(SPR -1) X version of WI Module (Kee)"*/
/* *1    19-JUN-1989 13:37:29 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MENU.C*/
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MENU.C                              */
/* *4    27-JUN-1990 17:28:35 KEE "(SPR -1) New wi data structure using 2-D array"  */
/* *3     2-MAR-1990 11:29:13 VINCE "(SPR 6012) Changed usage of USE_X symbol"      */
/* *2     5-FEB-1990 23:36:27 JULIAN "(SPR -1) X version of WI Module (Kee)"        */
/* *1    19-JUN-1989 13:37:29 SYSTEM ""                                             */
/* DEC/CMS REPLACEMENT HISTORY, Element WI_ENBL_MENU.C                              */
/******************************************************************************     */
/*                                                                                  */
/*          Copyright Finder Graphics Systems, Inc. 1989                            */
/*       Unpublished -- All rights reserved                                         */
/*                                                                                  */
/*THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Finder Graphics Systems, Inc.  AND   */
/*MAY CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE  */
/*DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE         */
/*MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF       */
/*                                                                                  */
/*        Finder Graphics Systems, Inc.                                             */
/*        201 Tamal Vista Blvd                                                      */
/*        Corte Madera, CA  USA 94925                                               */
/*        (415) 927-0100                                                            */
/*                                                                                  */
/*(The above notice does not constitute an admission of publication or              */
/*unrestricted dissemination of the work)                                           */
/*                                                                                  */
/******************************************************************************     */


/*************************************************************************          */
/*                                                                                  */
/*NAME:    WI_ENABLE_MENU                                                           */
/*                                                                                  */
/*DESCRIPTION:                                                                      */
/*                                                                                  */
/*    Enable an entire menu for the next request menu.                              */
/*                                                                                  */
/*    Input:                                                                        */
/*                                                                                  */
/*         menuid = (INT) identifier for menu.                                      */
/*         flag   = (INT) visibility flag, 0 -> invisible, 1 -> visible.            */
/*                                                                                  */
/*************************************************************************          */

#include "esi_wi_int.x"        /* external definitions needed here */

#ifdef USE_X

#if USE_PROTOTYPES
static INT wiz_set_syn_menu_onoroff (INT menuid, BOOL flag);
#else
static INT wiz_set_syn_menu_onoroff ();
#endif
#endif

/************************************************************************************/
#define IDENT progname
publicdef INT wi_enable_menu (menuid, flag) /*************************************************************************/

INT menuid;
INT flag;

    {
    PROGNAME ("wi_enable_menu");
    INT status;

#ifdef USE_X
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_ID (menuid, 0, WI_MENU);
    E_CHECK_WIDGET (menuid, 0);
    
    (flag == TRUE) ?
        wiz_managechild_widget (menuid) : wiz_unmanagechild_widget (menuid);
    
    if ((E_DIALOG_MENU (menuid))->app_id > 1)
        {
        wiz_set_syn_menu_onoroff (menuid, flag);
        }
#else        /* ESITERM code */
    wiz_jp_name (IDENT);
    
    E_CHECK_INIT;
    E_CHECK_ID (menuid, 0, WI_MENU);
    SEND_3_VALUES (flag, menuid, MC_SELECT_MN);
#endif
    RETURN (SUCCESS);
    }

/* ************************************************************************         */
/*                                                                                  */
/*   NAME:    SET_SYN_MENU_ONOROFF                                                  */
/*                                                                                  */
/*   DESCRIPTION:                                                                   */
/*                                                                                  */
/*  Set menu on or off flag at syn menu list                                        */
/*                                                                                  */
/*   ************************************************************************       */
#ifdef USE_X
static INT wiz_set_syn_menu_onoroff (menuid, flag)
INT menuid;
BOOL flag;
    {
    PROGNAME ("wiz_set_syn_menu_onoroff");
    MENU_SYN_LIST *menu_syn_ptr;
    MENU_SYN_ITEM_LIST *menu_syn_item_ptr;
    
    for (menu_syn_ptr = wiv.menu_syn_listroot; menu_syn_ptr != (MENU_SYN_LIST *)NULL;
         menu_syn_ptr = menu_syn_ptr->next)
        {
        for (menu_syn_item_ptr = menu_syn_ptr->item_list;
             menu_syn_item_ptr != (MENU_SYN_ITEM_LIST *)NULL;
             menu_syn_item_ptr = menu_syn_item_ptr->next)
            {
            if (menu_syn_item_ptr->id == menuid)
                {
                menu_syn_item_ptr->onoroff = flag;
                return(SUCCESS);
                }
            }
        }
    return(FAIL);
    }


#endif
