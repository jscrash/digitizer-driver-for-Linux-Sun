/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_LP_STUP.C*/
/* *6    21-AUG-1991 15:37:56 KEE "(SPR -1) Comment out Label Menu for 63 release"*/
/* *5    23-MAR-1990 23:46:26 KEE "(SPR -1) make menu items not dim for X version"*/
/* *4     2-MAR-1990 11:14:18 JULIAN "(SPR 6012) fix up USE_X logic to be IFDEF only"*/
/* *3     8-FEB-1990 18:01:16 JULIAN "(SPR -1) temporary disable all menu items (some rs files need discussion fixing - Kee)"*/
/* *2    25-JUL-1989 20:13:30 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:52 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_MN_LP_STUP.C*/

/******************************************************************************

                Copyright Exploration Systems, Inc. 1989
		   Unpublished -- All rights reserved

THIS SOFTWARE IS THE PROPRIETARY PROPERTY OF Exploration Systems, Inc.  AND MAY
CONTAIN CONFIDENTIAL TRADE SECRET INFORMATION. IT IS LICENSED FOR USE ON THE
DESIGNATED EQUIPMENT ON WHICH IT WAS ORIGINALLY INSTALLED AND  MAY NOT BE
MODIFIED, DUPLICATED OR COPIED IN ANY FORM WITHOUT PRIOR WRITTEN CONSENT OF

			Exploration Systems, Inc.
			579 Market Street
			San Francisco, CA  USA 94105
			(415) 974-1600

(The above notice does not constitute an admission of publication or
unrestricted dissemination of the work)

******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* PROGRAM:  SX_MENU_LP_SETUP.C                                              */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: SET UP LABEL POSITION MENU                                      */
/*                                                                           */
/*****************************************************************************/

#include "esi_sx.h"
#include "sx_menus.rh"
#include "esi_wi.h"

publicdef INT sx_menu_lp_setup(psx)
SX_STRUCT *psx;
{
    INT flag;
    INT status = SUCCESS;


    flag = (psx->line_selected) ? ON : OFF;

    wienmni(psx->menu_id.lp,ITEM_LP_SET_DEFPOS,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_SET_SYMTRIG,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_SET_LABTRIG,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_SET_SHOTTRIG,flag);

    flag = psx->line_selected && psx->nshots;

    wienmni(psx->menu_id.lp,ITEM_LP_AUTO_ALL_LABELS,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_AUTO_SHOT_LABELS,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_AUTO_LINE_LABELS,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_CHANGE_INDIVIDUAL,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_CHANGE_LINE,flag);
    wienmni(psx->menu_id.lp,ITEM_LP_DISPLAY_DATA_LABELS,flag);
#if 0 /* comment out for 63 release.  8-22-1991 */
    wivismn(psx->menu_id.lp,ON);
#endif
    SX_RETURN;
}

