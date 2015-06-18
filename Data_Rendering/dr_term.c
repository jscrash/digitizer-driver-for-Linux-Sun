/* DEC/CMS REPLACEMENT HISTORY, Element DR_TERM.C*/
/* *2    26-JUN-1989 10:42:37 GILLESPIE "(SPR 20) Fix castings"*/
/* *1    19-JUN-1989 12:48:45 SYSTEM ""*/
/* DEC/CMS REPLACEMENT HISTORY, Element DR_TERM.C*/

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

#ifndef ESI_DR_H
#include "esi_dr.h"
#endif

#ifndef ESI_MP_H
#include "esi_mp.h"
#endif

#ifndef ESI_WI_H
#include "esi_wi.h"
#endif

#ifndef ESI_GKS_H
#include "esi_gks.h"
#endif

#ifndef ESI_WM_H
#include "esi_wm.h"
#endif

#ifndef ESI_AM_H
#include "esi_am.h"
#endif

#ifndef ESI_FI_H
#include "esi_fi.h"
#endif

publicdef INT dr_term(dr_data)
DR_DATA *dr_data;
{
    INT status = SUCCESS;
    MAP_STRUCTURE *mapdef;
    MENU_ID *menu_id;
    CDLG_ID *cdlg_id;

    if(dr_data->current_object > 0) wmgclose(dr_data->current_object);
    if(dr_data->current_overlay > 0)
	{
	wmgclose(dr_data->current_overlay);
	if (dr_data->named_overlay > 0)
	    {
	    wmgclose(dr_data->named_overlay);
	    }
	}
    
    status = mp_get_current_map(&mapdef);
    wmwclose(mapdef->map_window_id);
    
/*  Wipe out any orphaned status box  */

    am_message(AM_DIALOG,NULL);
    
/*	Close the little dialog that signifies electronic drafting */

    status = am_open_workspace("CDLG_ID",AM_APPLICATION,(VOIDPTR *)&cdlg_id);
    wi_close_dialog( cdlg_id->base_dialog_id );
	
    fi_pop();
    mp_dim_window(mapdef->map_name);

/* release all the resource. */
    am_release_resource(dr_data->menu.draft.id);
    am_release_resource(dr_data->menu.object.id);
    am_release_resource(dr_data->menu.line.id);
    am_release_resource(dr_data->menu.text.id);
    am_release_resource(dr_data->menu.pref.id);

/* Turn the main Mapping menus back on */

    am_open_workspace("MENU",AM_APPLICATION,(VOIDPTR *)&menu_id);
    menu_id->menu_status[0] = ON;
    mp_menu_status();

    return SUCCESS;
}
