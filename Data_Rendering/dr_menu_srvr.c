/* DEC/CMS REPLACEMENT HISTORY, Element DR_MENU_SRVR.C*/
/* *3     1-JUL-1991 15:04:36 KEE "(SPR 8013) Fix Polyline Color Palette Window being orphaned after CANCEL, case 2364"*/
/* *2    18-JUL-1990 08:24:36 GILLESPIE "(SPR 1) Add esi_dr_msg.h"*/
/* *1    19-JUN-1989 12:47:00 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_MENU_SRVR.C*/

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

#include    "esi_dr.h"
#include    "esi_am.h"
#include    "esi_gks.h"
#include    "esi_wi.h"
#ifndef ESI_MG_H
#include "esi_mg.h"
#endif
#ifndef ESI_TG_H
#include "esi_tg.h"
#endif
#include    "dr_menus.rh"
#include "esi_dr_msg.h"

/*------------------------------------------------------------------------*/
/*  Drafting menu server   */
/*------------------------------------------------------------------------*/

#if USE_PROTOTYPES
publicdef INT dr_draft_menu_server(INT id, INT item, DR_DATA *dr_data)
#else
publicdef INT dr_draft_menu_server(id, item, dr_data)
INT id;
INT item;
DR_DATA *dr_data;
#endif
{
    INT main_wkid = MAIN_WKID;
    BOOL ifyes, reset = FALSE;
    CHAR msg[80];
    INT status = SUCCESS;

/*  Trash any status box                       */

        am_message(AM_DIALOG,NULL);

/*  Turn off any pick echo                     */

        if(dr_data->pick_echo)
        {
            dr_data->pick_echo = FALSE;
            wm_grsgwk(main_wkid);
        }

	switch(item   )
	{

        /*----------------------------------------------------------------*/
                case DR_SELECT_TYPE:
        /*----------------------------------------------------------------*/
   
                    status = dr_select_object_type(dr_data);
		    break;

        /*----------------------------------------------------------------*/
                case DR_NEW_TYPE:
        /*----------------------------------------------------------------*/
 
                    status = dr_new_object_type(dr_data);
                    break;
 
        /*----------------------------------------------------------------*/
                case DR_DELETE_TYPE:
        /*----------------------------------------------------------------*/

                    status = dr_delete_object_type(dr_data);
                    break;

        /*----------------------------------------------------------------*/
                case DR_QUIT:
        /*----------------------------------------------------------------*/
             
                    if((dr_data->object_changed) && 
                       (dr_data->current_object_key > 0))
                    {
                        sprintf(msg,mg_message(DR_SAVE_CHANGES),
                                dr_data->current_object_key);
                        wiyesno(msg,"",&ifyes,&status); 
                        if(ifyes)
                            status = dr_save_object(dr_data);
                        else
                            status = dr_release_object(dr_data);
		    }
		    dr_term(dr_data);
                    reset = TRUE;
		    if (tg_window_id() IS_NOT_EQUAL_TO -1)
		      {
			tg_delete_window();
		      }
		break;
	}

    dr_update_menus( dr_data, reset );

    return SUCCESS;
}
