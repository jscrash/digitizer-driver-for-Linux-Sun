/* DEC/CMS REPLACEMENT HISTORY, Element SX_LS_MN_SRVR.C*/
/* *5    14-SEP-1989 18:35:24 GORDON "(SPR -1) checkin from gulf"*/
/* *4     6-SEP-1989 09:35:29 GORDON "(SPR 200) Gulf changes (August trip)"*/
/* *3    11-AUG-1989 14:37:23 JULIAN "(SPR -1) statement not reached"*/
/* *2    25-JUL-1989 20:13:07 GILLESPIE "(SPR 50) Seismic system revamp"*/
/* *1    19-JUN-1989 13:29:41 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element SX_LS_MN_SRVR.C*/

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
/* PROGRAM:  SX_LS_MENU_SRVR.C                                               */
/* AUTHOR:   STEVE SPENCE                                                    */
/* FUNCTION: LINE SELECTION MENU SERVER                                      */
/*                                                                           */
/*****************************************************************************/

#include "esi_sx.h"
#include "esi_am.h"
#include "esi_wi.h"
#include "esi_gks.h"

#include "sx_menus.rh"
#include "sx_cdlgs.rh"

publicdef INT sx_ls_menu_srvr(menuid,item,psx)
INT       menuid;
INT       item;
SX_STRUCT *psx;
{
    INT status = SUCCESS;

    switch (item)
        {
        case ITEM_LS_GRAPHIC_PICKING:
             sx_graphic_picking(psx);
             break;

        case ITEM_LS_MANUAL_ENTRY:
             /* save previous line if necesary */

             if(psx->line_selected)
                 {
                 sx_save_line(psx,TRUE);
                 }
               am_define_resource(AM_DIALOG,"sx_cdlgs",GET_SURVEY_NAME,
                   sx_manual_entry_srvr, &psx->cdlg_id.dialog_line_name,psx);
               wiscdt(psx->cdlg_id.dialog_line_name, SLE_SURVEY_CODE, "");
               wiscdt(psx->cdlg_id.dialog_line_name, SLE_LINE_NAME, "");
               wiopcd(psx->cdlg_id.dialog_line_name);
               SX_RETURN;

        case ITEM_LS_REDRAW:
             wm_grsgwk(MAIN_WKID);
             so_draw_seismic_line(psx->pso);
             break;

        case ITEM_LS_SAVE:
             sx_save_line(psx,FALSE);
             break;

        case ITEM_LS_DELETE:
             sx_delete_line(psx);
             break;

        case ITEM_LS_QUIT:
             sx_shutdown(psx);
             break;

        default: break;
        }
    SX_RETURN;
}
