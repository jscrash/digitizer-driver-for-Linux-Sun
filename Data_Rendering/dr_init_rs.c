/* DEC/CMS REPLACEMENT HISTORY, Element DR_INIT_RS.C*/
/* *3    12-JUN-1990 10:35:49 KEE "(SPR -1) Bug fixing and new enhancement of Electronic Drafting Text Edit for (SCD)"*/
/* *2    26-JUN-1989 10:41:38 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:46:49 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_INIT_RS.C*/

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

/******************************************************************************

NAME:         dr_init_resources

DESCRIPTION:  Initialize the menus and resources of the electronic
              drafting module


     Input:   None

     Output:  None

     Status:  SUCCESS

******************************************************************************/

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_wi.h"
#include    "esi_wi_rsrc.h"

#include    "dr_resources.rh"
#include    "dr_txt_dlg.rh"
#include    "dr_menus.rh"

/******************************************************************************/
/*   Global Definitions                                                       */
/******************************************************************************/

static CHAR DR_MENUS[] = "DR_MENUS";
static CHAR DR_RESOURCES[] = "DR_RESOURCES";
static CHAR DR_TXT_DLG[] = "DR_TXT_DLG";

/******************************************************************************/
/*   End of Global Definitions                                                */
/******************************************************************************/

INT dr_init_resources(dr_data)
DR_DATA *dr_data;
{
    PROGNAME("dr_init_resources");
    INT status;
    INT i;
    CDLG_ID *cdlg_id;
    WI_RSRC_INFO rsrc_info;

/*  Define the resources   */

    if(am_open_workspace("CDLG_ID",AM_APPLICATION,(VOIDPTR *)&cdlg_id) != SUCCESS)
	{
	am_define_workspace("CDLG_ID",AM_APPLICATION,sizeof(CDLG_ID),(VOIDPTR *)&cdlg_id);
	}

    status = am_define_resource(AM_DIALOG,DR_RESOURCES,DR_BASE_DIALOG,
                                    NULL_FUNCTION_PTR, &cdlg_id->base_dialog_id,NULL);

				/*  Make the base dialog visible   */
    wi_open_dialog(cdlg_id->base_dialog_id);

    status = am_define_resource(AM_DIALOG,DR_TXT_DLG,DR_DIALOG_ET,
                                    NULL_FUNCTION_PTR, &cdlg_id->text_dialog_id,NULL);

    status = am_define_resource(AM_DIALOG,DR_RESOURCES,DR_STOCK_SELECTOR,
                                    NULL_FUNCTION_PTR, &cdlg_id->selector_id,NULL);

    status = am_define_resource(AM_DIALOG,DR_RESOURCES,DR_ALIGNMENT_SEL,
                                    NULL_FUNCTION_PTR, &cdlg_id->alignment_id,NULL);

/*  Define the menus   */

    status = am_define_resource(AM_MENU,DR_MENUS,DR_MENU,
		   dr_draft_menu_server, &dr_data->menu.draft.id,dr_data);
    if(status == SUCCESS)
    {
	wi_query_info(dr_data->menu.draft.id,&rsrc_info);
	dr_data->menu.draft.num_items = rsrc_info.numitems;
	wi_enable_menu(dr_data->menu.draft.id,ON);
    }
	 

    status = am_define_resource(AM_MENU,DR_MENUS,DR_MENU_OB,
		   dr_object_menu_server, &dr_data->menu.object.id,dr_data);
    if(status == SUCCESS)
    {
	wi_query_info(dr_data->menu.object.id,&rsrc_info);
	dr_data->menu.object.num_items = rsrc_info.numitems;
	wi_enable_menu(dr_data->menu.object.id,ON);
    }

    status = am_define_resource(AM_MENU,DR_MENUS,DR_MENU_LI,
		   dr_line_menu_server, &dr_data->menu.line.id,dr_data);
    if(status == SUCCESS)
    {
	wi_query_info(dr_data->menu.line.id,&rsrc_info);
	dr_data->menu.line.num_items = rsrc_info.numitems;
	wi_enable_menu(dr_data->menu.line.id,ON);
    }

    status = am_define_resource(AM_MENU,DR_MENUS,DR_MENU_TX,
		   dr_text_menu_server, &dr_data->menu.text.id,dr_data);
    if(status == SUCCESS)
    {
	wi_query_info(dr_data->menu.text.id,&rsrc_info);
	dr_data->menu.text.num_items = rsrc_info.numitems;
	wi_enable_menu(dr_data->menu.text.id,ON);
    }

    status = am_define_resource(AM_MENU,DR_MENUS,DR_MENU_PR,
		   dr_pref_menu_server, &dr_data->menu.pref.id,dr_data);
    if(status == SUCCESS)
    {
	wi_query_info(dr_data->menu.pref.id,&rsrc_info);
	dr_data->menu.pref.num_items = rsrc_info.numitems;
	wi_enable_menu(dr_data->menu.pref.id,ON);
    }

/*  Init the flag arrays   */

    for(i=0;i<16;i++)
    {
        dr_data->menu.draft.items_old[i] = -1;
        dr_data->menu.draft.items_new[i] = FALSE;
        dr_data->menu.object.items_old[i] = -1;
        dr_data->menu.object.items_new[i] = FALSE;
        dr_data->menu.line.items_old[i] = -1;
        dr_data->menu.line.items_new[i] = FALSE;
        dr_data->menu.text.items_old[i] = -1;
        dr_data->menu.text.items_new[i] = FALSE;
        dr_data->menu.pref.items_old[i] = -1;
        dr_data->menu.pref.items_new[i] = FALSE;
    }

    dr_update_menus(dr_data, FALSE);
    return SUCCESS;
}
